// Copyright 2020 The Tint Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "src/tint/ast/array.h"

#include <cmath>
#include <utility>

#include "src/tint/program_builder.h"

TINT_INSTANTIATE_TYPEINFO(tint::ast::Array);

namespace tint::ast {

namespace {
// Returns the string representation of an array size expression.
std::string SizeExprToString(const Expression* size, const SymbolTable& symbols) {
    if (auto* ident = size->As<IdentifierExpression>()) {
        return symbols.NameFor(ident->identifier->symbol);
    }
    if (auto* literal = size->As<IntLiteralExpression>()) {
        return std::to_string(literal->value);
    }
    // This will never be exposed to the user as the Resolver will reject this
    // expression for array size.
    return "<invalid>";
}
}  // namespace

Array::Array(ProgramID pid,
             NodeID nid,
             const Source& src,
             const Type* subtype,
             const Expression* cnt,
             utils::VectorRef<const Attribute*> attrs)
    : Base(pid, nid, src), type(subtype), count(cnt), attributes(std::move(attrs)) {}

Array::Array(Array&&) = default;

Array::~Array() = default;

std::string Array::FriendlyName(const SymbolTable& symbols) const {
    std::ostringstream out;
    for (auto* attr : attributes) {
        if (auto* stride = attr->As<ast::StrideAttribute>()) {
            out << "@stride(" << stride->stride << ") ";
        }
    }
    out << "array";
    if (type) {
        out << "<" << type->FriendlyName(symbols);
        if (count) {
            out << ", " << SizeExprToString(count, symbols);
        }
        out << ">";
    }
    return out.str();
}

const Array* Array::Clone(CloneContext* ctx) const {
    // Clone arguments outside of create() call to have deterministic ordering
    auto src = ctx->Clone(source);
    auto* ty = ctx->Clone(type);
    auto* cnt = ctx->Clone(count);
    auto attrs = ctx->Clone(attributes);
    return ctx->dst->create<Array>(src, ty, cnt, std::move(attrs));
}

}  // namespace tint::ast
