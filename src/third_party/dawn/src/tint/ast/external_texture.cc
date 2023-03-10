// Copyright 2021 The Tint Authors.
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

#include "src/tint/ast/external_texture.h"

#include "src/tint/program_builder.h"
#include "src/tint/type/texture_dimension.h"

TINT_INSTANTIATE_TYPEINFO(tint::ast::ExternalTexture);

namespace tint::ast {

// ExternalTexture::ExternalTexture() : Base(type::TextureDimension::k2d) {}
ExternalTexture::ExternalTexture(ProgramID pid, NodeID nid, const Source& src)
    : Base(pid, nid, src, type::TextureDimension::k2d) {}

ExternalTexture::ExternalTexture(ExternalTexture&&) = default;

ExternalTexture::~ExternalTexture() = default;

std::string ExternalTexture::FriendlyName(const SymbolTable&) const {
    return "texture_external";
}

const ExternalTexture* ExternalTexture::Clone(CloneContext* ctx) const {
    return ctx->dst->create<ExternalTexture>();
}

}  // namespace tint::ast
