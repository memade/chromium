# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: policy_common_definitions.proto
"""Generated protocol buffer code."""
from google.protobuf.internal import builder as _builder
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x1fpolicy_common_definitions.proto\x12\x15\x65nterprise_management\"\x1d\n\nStringList\x12\x0f\n\x07\x65ntries\x18\x01 \x03(\t\"\x92\x01\n\rPolicyOptions\x12H\n\x04mode\x18\x01 \x01(\x0e\x32/.enterprise_management.PolicyOptions.PolicyMode:\tMANDATORY\"7\n\nPolicyMode\x12\r\n\tMANDATORY\x10\x00\x12\x0f\n\x0bRECOMMENDED\x10\x01\x12\t\n\x05UNSET\x10\x02\"a\n\x12\x42ooleanPolicyProto\x12<\n\x0epolicy_options\x18\x01 \x01(\x0b\x32$.enterprise_management.PolicyOptions\x12\r\n\x05value\x18\x02 \x01(\x08\"a\n\x12IntegerPolicyProto\x12<\n\x0epolicy_options\x18\x01 \x01(\x0b\x32$.enterprise_management.PolicyOptions\x12\r\n\x05value\x18\x02 \x01(\x03\"`\n\x11StringPolicyProto\x12<\n\x0epolicy_options\x18\x01 \x01(\x0b\x32$.enterprise_management.PolicyOptions\x12\r\n\x05value\x18\x02 \x01(\t\"\x87\x01\n\x15StringListPolicyProto\x12<\n\x0epolicy_options\x18\x01 \x01(\x0b\x32$.enterprise_management.PolicyOptions\x12\x30\n\x05value\x18\x02 \x01(\x0b\x32!.enterprise_management.StringListB/H\x03Z+chromium/policy/enterprise_management_proto')

_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, globals())
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'policy_common_definitions_pb2', globals())
if _descriptor._USE_C_DESCRIPTORS == False:

  DESCRIPTOR._options = None
  DESCRIPTOR._serialized_options = b'H\003Z+chromium/policy/enterprise_management_proto'
  _STRINGLIST._serialized_start=58
  _STRINGLIST._serialized_end=87
  _POLICYOPTIONS._serialized_start=90
  _POLICYOPTIONS._serialized_end=236
  _POLICYOPTIONS_POLICYMODE._serialized_start=181
  _POLICYOPTIONS_POLICYMODE._serialized_end=236
  _BOOLEANPOLICYPROTO._serialized_start=238
  _BOOLEANPOLICYPROTO._serialized_end=335
  _INTEGERPOLICYPROTO._serialized_start=337
  _INTEGERPOLICYPROTO._serialized_end=434
  _STRINGPOLICYPROTO._serialized_start=436
  _STRINGPOLICYPROTO._serialized_end=532
  _STRINGLISTPOLICYPROTO._serialized_start=535
  _STRINGLISTPOLICYPROTO._serialized_end=670
# @@protoc_insertion_point(module_scope)
