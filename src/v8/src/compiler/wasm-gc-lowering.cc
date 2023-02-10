// Copyright 2022 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/compiler/wasm-gc-lowering.h"

#include "src/base/logging.h"
#include "src/common/globals.h"
#include "src/compiler/common-operator.h"
#include "src/compiler/node-properties.h"
#include "src/compiler/opcodes.h"
#include "src/compiler/operator.h"
#include "src/compiler/wasm-compiler-definitions.h"
#include "src/compiler/wasm-graph-assembler.h"
#include "src/wasm/object-access.h"
#include "src/wasm/wasm-engine.h"
#include "src/wasm/wasm-linkage.h"
#include "src/wasm/wasm-objects.h"
#include "src/wasm/wasm-subtyping.h"

namespace v8 {
namespace internal {
namespace compiler {

WasmGCLowering::WasmGCLowering(Editor* editor, MachineGraph* mcgraph,
                               const wasm::WasmModule* module)
    : AdvancedReducer(editor),
      gasm_(mcgraph, mcgraph->zone()),
      module_(module),
      dead_(mcgraph->Dead()),
      instance_node_(nullptr) {
  // Find and store the instance node.
  for (Node* start_use : mcgraph->graph()->start()->uses()) {
    if (start_use->opcode() == IrOpcode::kParameter &&
        ParameterIndexOf(start_use->op()) == 0) {
      instance_node_ = start_use;
      break;
    }
  }
  DCHECK_NOT_NULL(instance_node_);
}

Reduction WasmGCLowering::Reduce(Node* node) {
  switch (node->opcode()) {
    case IrOpcode::kWasmTypeCheck:
      return ReduceWasmTypeCheck(node);
    case IrOpcode::kWasmTypeCast:
      return ReduceWasmTypeCast(node);
    case IrOpcode::kAssertNotNull:
      return ReduceAssertNotNull(node);
    case IrOpcode::kNull:
      return ReduceNull(node);
    case IrOpcode::kIsNull:
      return ReduceIsNull(node);
    case IrOpcode::kIsNotNull:
      return ReduceIsNotNull(node);
    case IrOpcode::kRttCanon:
      return ReduceRttCanon(node);
    case IrOpcode::kTypeGuard:
      return ReduceTypeGuard(node);
    case IrOpcode::kWasmExternInternalize:
      return ReduceWasmExternInternalize(node);
    case IrOpcode::kWasmExternExternalize:
      return ReduceWasmExternExternalize(node);
    case IrOpcode::kWasmStructGet:
      return ReduceWasmStructGet(node);
    case IrOpcode::kWasmStructSet:
      return ReduceWasmStructSet(node);
    case IrOpcode::kWasmArrayGet:
      return ReduceWasmArrayGet(node);
    case IrOpcode::kWasmArraySet:
      return ReduceWasmArraySet(node);
    case IrOpcode::kWasmArrayLength:
      return ReduceWasmArrayLength(node);
    case IrOpcode::kWasmArrayInitializeLength:
      return ReduceWasmArrayInitializeLength(node);
    case IrOpcode::kStringAsWtf16:
      return ReduceStringAsWtf16(node);
    case IrOpcode::kStringPrepareForGetCodeunit:
      return ReduceStringPrepareForGetCodeunit(node);
    default:
      return NoChange();
  }
}

Node* WasmGCLowering::IsolateRoot() {
  // TODO(13449): Use root register instead of isolate.
  return gasm_.LoadImmutable(
      MachineType::Pointer(), instance_node_,
      WasmInstanceObject::kIsolateRootOffset - kHeapObjectTag);
}

Node* WasmGCLowering::RootNode(RootIndex index) {
  Node* isolate_root = IsolateRoot();
  return gasm_.LoadImmutable(MachineType::Pointer(), isolate_root,
                             IsolateData::root_slot_offset(index));
}

Node* WasmGCLowering::Null() { return RootNode(RootIndex::kNullValue); }

Node* WasmGCLowering::IsNull(Node* object) {
  Tagged_t static_null = wasm::GetWasmEngine()->compressed_null_value_or_zero();
  Node* null_value =
      static_null != 0 ? gasm_.UintPtrConstant(static_null) : Null();
  return gasm_.TaggedEqual(object, null_value);
}

// TODO(manoskouk): Use the Callbacks infrastructure from wasm-compiler.h to
// unify all check/cast implementations.
// TODO(manoskouk): Find a way to optimize branches on typechecks.
Reduction WasmGCLowering::ReduceWasmTypeCheck(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kWasmTypeCheck);

  Node* object = node->InputAt(0);
  Node* rtt = node->InputAt(1);
  Node* effect_input = NodeProperties::GetEffectInput(node);
  Node* control_input = NodeProperties::GetControlInput(node);
  auto config = OpParameter<WasmTypeCheckConfig>(node->op());
  int rtt_depth = wasm::GetSubtypingDepth(module_, config.to.ref_index());
  bool object_can_be_null = config.from.is_nullable();
  bool object_can_be_i31 =
      wasm::IsSubtypeOf(wasm::kWasmI31Ref.AsNonNull(), config.from, module_);

  gasm_.InitializeEffectControl(effect_input, control_input);

  auto end_label = gasm_.MakeLabel(MachineRepresentation::kWord32);
  bool is_cast_from_any = config.from.is_reference_to(wasm::HeapType::kAny);

  // Skip the null check if casting from any and if null results in check
  // failure. In that case the instance type check will identify null as not
  // being a wasm object and return 0 (failure).
  if (object_can_be_null && (!is_cast_from_any || config.to.is_nullable())) {
    const int kResult = config.to.is_nullable() ? 1 : 0;
    gasm_.GotoIf(IsNull(object), &end_label, BranchHint::kFalse,
                 gasm_.Int32Constant(kResult));
  }

  if (object_can_be_i31) {
    gasm_.GotoIf(gasm_.IsI31(object), &end_label, gasm_.Int32Constant(0));
  }

  Node* map = gasm_.LoadMap(object);

  if (module_->types[config.to.ref_index()].is_final) {
    gasm_.Goto(&end_label, gasm_.TaggedEqual(map, rtt));
  } else {
    // First, check if types happen to be equal. This has been shown to give
    // large speedups.
    gasm_.GotoIf(gasm_.TaggedEqual(map, rtt), &end_label, BranchHint::kTrue,
                 gasm_.Int32Constant(1));

    // Check if map instance type identifies a wasm object.
    if (is_cast_from_any) {
      Node* is_wasm_obj = gasm_.IsDataRefMap(map);
      gasm_.GotoIfNot(is_wasm_obj, &end_label, BranchHint::kTrue,
                      gasm_.Int32Constant(0));
    }

    Node* type_info = gasm_.LoadWasmTypeInfo(map);
    DCHECK_GE(rtt_depth, 0);
    // If the depth of the rtt is known to be less that the minimum supertype
    // array length, we can access the supertype without bounds-checking the
    // supertype array.
    if (static_cast<uint32_t>(rtt_depth) >= wasm::kMinimumSupertypeArraySize) {
      Node* supertypes_length =
          gasm_.BuildChangeSmiToIntPtr(gasm_.LoadImmutableFromObject(
              MachineType::TaggedSigned(), type_info,
              wasm::ObjectAccess::ToTagged(
                  WasmTypeInfo::kSupertypesLengthOffset)));
      gasm_.GotoIfNot(gasm_.UintLessThan(gasm_.IntPtrConstant(rtt_depth),
                                         supertypes_length),
                      &end_label, BranchHint::kTrue, gasm_.Int32Constant(0));
    }

    Node* maybe_match = gasm_.LoadImmutableFromObject(
        MachineType::TaggedPointer(), type_info,
        wasm::ObjectAccess::ToTagged(WasmTypeInfo::kSupertypesOffset +
                                     kTaggedSize * rtt_depth));

    gasm_.Goto(&end_label, gasm_.TaggedEqual(maybe_match, rtt));
  }

  gasm_.Bind(&end_label);

  ReplaceWithValue(node, end_label.PhiAt(0), gasm_.effect(), gasm_.control());
  node->Kill();
  return Replace(end_label.PhiAt(0));  // Meaningless argument.
}

Reduction WasmGCLowering::ReduceWasmTypeCast(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kWasmTypeCast);

  Node* object = node->InputAt(0);
  Node* rtt = node->InputAt(1);
  Node* effect_input = NodeProperties::GetEffectInput(node);
  Node* control_input = NodeProperties::GetControlInput(node);
  auto config = OpParameter<WasmTypeCheckConfig>(node->op());
  int rtt_depth = wasm::GetSubtypingDepth(module_, config.to.ref_index());
  bool object_can_be_null = config.from.is_nullable();
  bool object_can_be_i31 =
      wasm::IsSubtypeOf(wasm::kWasmI31Ref.AsNonNull(), config.from, module_);

  gasm_.InitializeEffectControl(effect_input, control_input);

  auto end_label = gasm_.MakeLabel();
  bool is_cast_from_any = config.from.is_reference_to(wasm::HeapType::kAny);

  // Skip the null check if casting from any and if null results in check
  // failure. In that case the instance type check will identify null as not
  // being a wasm object and trap.
  if (object_can_be_null && (!is_cast_from_any || config.to.is_nullable())) {
    Node* is_null = IsNull(object);
    if (config.to.is_nullable()) {
      gasm_.GotoIf(is_null, &end_label, BranchHint::kFalse);
    } else if (!v8_flags.experimental_wasm_skip_null_checks) {
      gasm_.TrapIf(is_null, TrapId::kTrapIllegalCast);
    }
  }

  if (object_can_be_i31) {
    gasm_.TrapIf(gasm_.IsI31(object), TrapId::kTrapIllegalCast);
  }

  Node* map = gasm_.LoadMap(object);

  if (module_->types[config.to.ref_index()].is_final) {
    gasm_.TrapUnless(gasm_.TaggedEqual(map, rtt), TrapId::kTrapIllegalCast);
    gasm_.Goto(&end_label);
  } else {
    // First, check if types happen to be equal. This has been shown to give
    // large speedups.
    gasm_.GotoIf(gasm_.TaggedEqual(map, rtt), &end_label, BranchHint::kTrue);

    // Check if map instance type identifies a wasm object.
    if (is_cast_from_any) {
      Node* is_wasm_obj = gasm_.IsDataRefMap(map);
      gasm_.TrapUnless(is_wasm_obj, TrapId::kTrapIllegalCast);
    }

    Node* type_info = gasm_.LoadWasmTypeInfo(map);
    DCHECK_GE(rtt_depth, 0);
    // If the depth of the rtt is known to be less that the minimum supertype
    // array length, we can access the supertype without bounds-checking the
    // supertype array.
    if (static_cast<uint32_t>(rtt_depth) >= wasm::kMinimumSupertypeArraySize) {
      Node* supertypes_length =
          gasm_.BuildChangeSmiToIntPtr(gasm_.LoadImmutableFromObject(
              MachineType::TaggedSigned(), type_info,
              wasm::ObjectAccess::ToTagged(
                  WasmTypeInfo::kSupertypesLengthOffset)));
      gasm_.TrapUnless(gasm_.UintLessThan(gasm_.IntPtrConstant(rtt_depth),
                                          supertypes_length),
                       TrapId::kTrapIllegalCast);
    }

    Node* maybe_match = gasm_.LoadImmutableFromObject(
        MachineType::TaggedPointer(), type_info,
        wasm::ObjectAccess::ToTagged(WasmTypeInfo::kSupertypesOffset +
                                     kTaggedSize * rtt_depth));

    gasm_.TrapUnless(gasm_.TaggedEqual(maybe_match, rtt),
                     TrapId::kTrapIllegalCast);
    gasm_.Goto(&end_label);
  }

  gasm_.Bind(&end_label);

  ReplaceWithValue(node, object, gasm_.effect(), gasm_.control());
  node->Kill();
  return Replace(object);
}

Reduction WasmGCLowering::ReduceAssertNotNull(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kAssertNotNull);
  Node* effect = NodeProperties::GetEffectInput(node);
  Node* control = NodeProperties::GetControlInput(node);
  Node* object = NodeProperties::GetValueInput(node, 0);
  gasm_.InitializeEffectControl(effect, control);
  if (!v8_flags.experimental_wasm_skip_null_checks) {
    gasm_.TrapIf(IsNull(object), TrapIdOf(node->op()));
  }

  ReplaceWithValue(node, object, gasm_.effect(), gasm_.control());
  node->Kill();
  return Replace(object);
}

Reduction WasmGCLowering::ReduceNull(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kNull);
  return Replace(Null());
}

Reduction WasmGCLowering::ReduceIsNull(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kIsNull);
  Node* object = NodeProperties::GetValueInput(node, 0);
  return Replace(IsNull(object));
}

Reduction WasmGCLowering::ReduceIsNotNull(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kIsNotNull);
  Node* object = NodeProperties::GetValueInput(node, 0);
  return Replace(gasm_.Word32Equal(IsNull(object), gasm_.Int32Constant(0)));
}

Reduction WasmGCLowering::ReduceRttCanon(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kRttCanon);
  int type_index = OpParameter<int>(node->op());
  Node* maps_list = gasm_.LoadImmutable(
      MachineType::TaggedPointer(), instance_node_,
      WasmInstanceObject::kManagedObjectMapsOffset - kHeapObjectTag);
  return Replace(gasm_.LoadImmutable(
      MachineType::TaggedPointer(), maps_list,
      wasm::ObjectAccess::ElementOffsetInTaggedFixedArray(type_index)));
}

Reduction WasmGCLowering::ReduceTypeGuard(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kTypeGuard);
  Node* alias = NodeProperties::GetValueInput(node, 0);
  ReplaceWithValue(node, alias);
  node->Kill();
  return Replace(alias);
}

Reduction WasmGCLowering::ReduceWasmExternInternalize(Node* node) {
  // TODO(7748): This is not used right now. Either use the
  // WasmExternInternalize operator and implement its lowering here, or remove
  // it entirely.
  UNREACHABLE();
}

// TODO(7748): WasmExternExternalize is a no-op. Consider removing it.
Reduction WasmGCLowering::ReduceWasmExternExternalize(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kWasmExternExternalize);
  Node* object = NodeProperties::GetValueInput(node, 0);
  ReplaceWithValue(node, object);
  node->Kill();
  return Replace(object);
}

Reduction WasmGCLowering::ReduceWasmStructGet(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kWasmStructGet);
  WasmFieldInfo info = OpParameter<WasmFieldInfo>(node->op());

  gasm_.InitializeEffectControl(NodeProperties::GetEffectInput(node),
                                NodeProperties::GetControlInput(node));

  MachineType type = MachineType::TypeForRepresentation(
      info.type->field(info.field_index).machine_representation(),
      info.is_signed);

  Node* load =
      info.type->mutability(info.field_index)
          ? gasm_.LoadFromObject(type, NodeProperties::GetValueInput(node, 0),
                                 gasm_.FieldOffset(info.type, info.field_index))
          : gasm_.LoadImmutableFromObject(
                type, NodeProperties::GetValueInput(node, 0),
                gasm_.FieldOffset(info.type, info.field_index));
  return Replace(load);
}

Reduction WasmGCLowering::ReduceWasmStructSet(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kWasmStructSet);
  WasmFieldInfo info = OpParameter<WasmFieldInfo>(node->op());

  gasm_.InitializeEffectControl(NodeProperties::GetEffectInput(node),
                                NodeProperties::GetControlInput(node));

  Node* object = NodeProperties::GetValueInput(node, 0);
  Node* value = NodeProperties::GetValueInput(node, 1);

  Node* store =
      info.type->mutability(info.field_index)
          ? gasm_.StoreToObject(
                ObjectAccessForGCStores(info.type->field(info.field_index)),
                object, gasm_.FieldOffset(info.type, info.field_index), value)
          : gasm_.InitializeImmutableInObject(
                ObjectAccessForGCStores(info.type->field(info.field_index)),
                object, gasm_.FieldOffset(info.type, info.field_index), value);
  return Replace(store);
}

Reduction WasmGCLowering::ReduceWasmArrayGet(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kWasmArrayGet);
  WasmElementInfo info = OpParameter<WasmElementInfo>(node->op());

  Node* object = NodeProperties::GetValueInput(node, 0);
  Node* index = NodeProperties::GetValueInput(node, 1);

  gasm_.InitializeEffectControl(NodeProperties::GetEffectInput(node),
                                NodeProperties::GetControlInput(node));

  Node* offset = gasm_.WasmArrayElementOffset(index, info.type->element_type());

  MachineType type = MachineType::TypeForRepresentation(
      info.type->element_type().machine_representation(), info.is_signed);

  Node* value = info.type->mutability()
                    ? gasm_.LoadFromObject(type, object, offset)
                    : gasm_.LoadImmutableFromObject(type, object, offset);

  return Replace(value);
}

Reduction WasmGCLowering::ReduceWasmArraySet(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kWasmArraySet);
  const wasm::ArrayType* type = OpParameter<const wasm::ArrayType*>(node->op());

  Node* object = NodeProperties::GetValueInput(node, 0);
  Node* index = NodeProperties::GetValueInput(node, 1);
  Node* value = NodeProperties::GetValueInput(node, 2);

  gasm_.InitializeEffectControl(NodeProperties::GetEffectInput(node),
                                NodeProperties::GetControlInput(node));

  Node* offset = gasm_.WasmArrayElementOffset(index, type->element_type());

  ObjectAccess access = ObjectAccessForGCStores(type->element_type());

  Node* store =
      type->mutability()
          ? gasm_.StoreToObject(access, object, offset, value)
          : gasm_.InitializeImmutableInObject(access, object, offset, value);

  return Replace(store);
}

Reduction WasmGCLowering::ReduceWasmArrayLength(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kWasmArrayLength);
  Node* object = NodeProperties::GetValueInput(node, 0);

  gasm_.InitializeEffectControl(NodeProperties::GetEffectInput(node),
                                NodeProperties::GetControlInput(node));

  Node* length = gasm_.LoadImmutableFromObject(
      MachineType::Uint32(), object,
      wasm::ObjectAccess::ToTagged(WasmArray::kLengthOffset));

  return Replace(length);
}

Reduction WasmGCLowering::ReduceWasmArrayInitializeLength(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kWasmArrayInitializeLength);
  Node* object = NodeProperties::GetValueInput(node, 0);
  Node* length = NodeProperties::GetValueInput(node, 1);

  gasm_.InitializeEffectControl(NodeProperties::GetEffectInput(node),
                                NodeProperties::GetControlInput(node));

  Node* set_length = gasm_.InitializeImmutableInObject(
      ObjectAccess{MachineType::Uint32(), kNoWriteBarrier}, object,
      wasm::ObjectAccess::ToTagged(WasmArray::kLengthOffset), length);

  return Replace(set_length);
}

Reduction WasmGCLowering::ReduceStringAsWtf16(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kStringAsWtf16);
  Node* effect = NodeProperties::GetEffectInput(node);
  Node* control = NodeProperties::GetControlInput(node);
  Node* str = NodeProperties::GetValueInput(node, 0);

  gasm_.InitializeEffectControl(effect, control);

  auto done = gasm_.MakeLabel(MachineRepresentation::kTaggedPointer);
  Node* instance_type = gasm_.LoadInstanceType(gasm_.LoadMap(str));
  Node* string_representation = gasm_.Word32And(
      instance_type, gasm_.Int32Constant(kStringRepresentationMask));
  gasm_.GotoIf(gasm_.Word32Equal(string_representation,
                                 gasm_.Int32Constant(kSeqStringTag)),
               &done, str);
  gasm_.Goto(&done, gasm_.CallBuiltin(Builtin::kWasmStringAsWtf16,
                                      Operator::kPure, str));
  gasm_.Bind(&done);
  ReplaceWithValue(node, done.PhiAt(0), gasm_.effect(), gasm_.control());
  node->Kill();
  return Replace(done.PhiAt(0));
}

Reduction WasmGCLowering::ReduceStringPrepareForGetCodeunit(Node* node) {
  DCHECK_EQ(node->opcode(), IrOpcode::kStringPrepareForGetCodeunit);
  Node* effect = NodeProperties::GetEffectInput(node);
  Node* control = NodeProperties::GetControlInput(node);
  Node* original_string = NodeProperties::GetValueInput(node, 0);

  gasm_.InitializeEffectControl(effect, control);

  auto dispatch =
      gasm_.MakeLoopLabel(MachineRepresentation::kTaggedPointer,  // String.
                          MachineRepresentation::kWord32,   // Instance type.
                          MachineRepresentation::kWord32);  // Offset.
  auto next = gasm_.MakeLabel(MachineRepresentation::kTaggedPointer,  // String.
                              MachineRepresentation::kWord32,  // Instance type.
                              MachineRepresentation::kWord32);  // Offset.
  auto direct_string =
      gasm_.MakeLabel(MachineRepresentation::kTaggedPointer,  // String.
                      MachineRepresentation::kWord32,         // Instance type.
                      MachineRepresentation::kWord32);        // Offset.

  // These values will be used to replace the original node's projections.
  // The first, "string", is either a SeqString or Smi(0) (in case of external
  // string). Notably this makes it GC-safe: if that string moves, this pointer
  // will be updated accordingly.
  // The second, "offset", has full register width so that it can be used to
  // store external pointers: for external strings, we add up the character
  // backing store's base address and any slice offset.
  // The third, "character width", is a shift width, i.e. it is 0 for one-byte
  // strings, 1 for two-byte strings, kCharWidthBailoutSentinel for uncached
  // external strings (for which "string"/"offset" are invalid and unusable).
  auto done =
      gasm_.MakeLabel(MachineRepresentation::kTagged,        // String.
                      MachineType::PointerRepresentation(),  // Offset.
                      MachineRepresentation::kWord32);       // Character width.

  Node* original_type = gasm_.LoadInstanceType(gasm_.LoadMap(original_string));
  gasm_.Goto(&dispatch, original_string, original_type, gasm_.Int32Constant(0));

  gasm_.Bind(&dispatch);
  {
    auto thin_string = gasm_.MakeLabel(MachineRepresentation::kTaggedPointer);
    auto cons_string = gasm_.MakeLabel(MachineRepresentation::kTaggedPointer);

    Node* string = dispatch.PhiAt(0);
    Node* instance_type = dispatch.PhiAt(1);
    Node* offset = dispatch.PhiAt(2);
    static_assert(kIsIndirectStringTag == 1);
    static constexpr int kIsDirectStringTag = 0;
    gasm_.GotoIf(gasm_.Word32Equal(
                     gasm_.Word32And(instance_type, gasm_.Int32Constant(
                                                        kIsIndirectStringMask)),
                     gasm_.Int32Constant(kIsDirectStringTag)),
                 &direct_string, string, instance_type, offset);

    // Handle indirect strings.
    Node* string_representation = gasm_.Word32And(
        instance_type, gasm_.Int32Constant(kStringRepresentationMask));
    gasm_.GotoIf(gasm_.Word32Equal(string_representation,
                                   gasm_.Int32Constant(kThinStringTag)),
                 &thin_string, string);
    gasm_.GotoIf(gasm_.Word32Equal(string_representation,
                                   gasm_.Int32Constant(kConsStringTag)),
                 &cons_string, string);

    // Sliced string.
    Node* new_offset = gasm_.Int32Add(
        offset,
        gasm_.BuildChangeSmiToInt32(gasm_.LoadImmutableFromObject(
            MachineType::TaggedSigned(), string,
            wasm::ObjectAccess::ToTagged(SlicedString::kOffsetOffset))));
    Node* parent = gasm_.LoadImmutableFromObject(
        MachineType::TaggedPointer(), string,
        wasm::ObjectAccess::ToTagged(SlicedString::kParentOffset));
    Node* parent_type = gasm_.LoadInstanceType(gasm_.LoadMap(parent));
    gasm_.Goto(&next, parent, parent_type, new_offset);

    // Thin string.
    gasm_.Bind(&thin_string);
    Node* actual = gasm_.LoadImmutableFromObject(
        MachineType::TaggedPointer(), string,
        wasm::ObjectAccess::ToTagged(ThinString::kActualOffset));
    Node* actual_type = gasm_.LoadInstanceType(gasm_.LoadMap(actual));
    // ThinStrings always reference (internalized) direct strings.
    gasm_.Goto(&direct_string, actual, actual_type, offset);

    // Flat cons string. (Non-flat cons strings are ruled out by
    // string.as_wtf16.)
    gasm_.Bind(&cons_string);
    Node* first = gasm_.LoadImmutableFromObject(
        MachineType::TaggedPointer(), string,
        wasm::ObjectAccess::ToTagged(ConsString::kFirstOffset));
    Node* first_type = gasm_.LoadInstanceType(gasm_.LoadMap(first));
    gasm_.Goto(&next, first, first_type, offset);

    gasm_.Bind(&next);
    gasm_.Goto(&dispatch, next.PhiAt(0), next.PhiAt(1), next.PhiAt(2));
  }

  gasm_.Bind(&direct_string);
  {
    Node* string = direct_string.PhiAt(0);
    Node* instance_type = direct_string.PhiAt(1);
    Node* offset = direct_string.PhiAt(2);

    Node* is_onebyte = gasm_.Word32And(
        instance_type, gasm_.Int32Constant(kStringEncodingMask));
    // Char width shift is 1 - (is_onebyte).
    static_assert(kStringEncodingMask == 1 << 3);
    Node* charwidth_shift =
        gasm_.Int32Sub(gasm_.Int32Constant(1),
                       gasm_.Word32Shr(is_onebyte, gasm_.Int32Constant(3)));

    auto external = gasm_.MakeLabel();
    Node* string_representation = gasm_.Word32And(
        instance_type, gasm_.Int32Constant(kStringRepresentationMask));
    gasm_.GotoIf(gasm_.Word32Equal(string_representation,
                                   gasm_.Int32Constant(kExternalStringTag)),
                 &external);

    // Sequential string.
    static_assert(SeqOneByteString::kCharsOffset ==
                  SeqTwoByteString::kCharsOffset);
    Node* final_offset = gasm_.Int32Add(
        gasm_.Int32Constant(
            wasm::ObjectAccess::ToTagged(SeqOneByteString::kCharsOffset)),
        gasm_.Word32Shl(offset, charwidth_shift));
    gasm_.Goto(&done, string, gasm_.BuildChangeInt32ToIntPtr(final_offset),
               charwidth_shift);

    // External string.
    gasm_.Bind(&external);
    gasm_.GotoIf(
        gasm_.Word32And(instance_type,
                        gasm_.Int32Constant(kUncachedExternalStringMask)),
        &done, string, gasm_.IntPtrConstant(0),
        gasm_.Int32Constant(kCharWidthBailoutSentinel));
    Node* resource = gasm_.BuildLoadExternalPointerFromObject(
        string, ExternalString::kResourceDataOffset,
        kExternalStringResourceDataTag, IsolateRoot());
    Node* shifted_offset = gasm_.Word32Shl(offset, charwidth_shift);
    final_offset = gasm_.IntPtrAdd(
        resource, gasm_.BuildChangeInt32ToIntPtr(shifted_offset));
    gasm_.Goto(&done, gasm_.SmiConstant(0), final_offset, charwidth_shift);
  }

  gasm_.Bind(&done);
  Node* base = done.PhiAt(0);
  Node* final_offset = done.PhiAt(1);
  Node* charwidth_shift = done.PhiAt(2);

  Node* base_proj = NodeProperties::FindProjection(node, 0);
  Node* offset_proj = NodeProperties::FindProjection(node, 1);
  Node* charwidth_proj = NodeProperties::FindProjection(node, 2);
  if (base_proj) {
    ReplaceWithValue(base_proj, base, gasm_.effect(), gasm_.control());
    base_proj->Kill();
  }
  if (offset_proj) {
    ReplaceWithValue(offset_proj, final_offset, gasm_.effect(),
                     gasm_.control());
    offset_proj->Kill();
  }
  if (charwidth_proj) {
    ReplaceWithValue(charwidth_proj, charwidth_shift, gasm_.effect(),
                     gasm_.control());
    charwidth_proj->Kill();
  }

  // Wire up the dangling end of the new effect chain.
  ReplaceWithValue(node, node, gasm_.effect(), gasm_.control());

  node->Kill();
  return Replace(base);
}

}  // namespace compiler
}  // namespace internal
}  // namespace v8
