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

#ifndef SRC_TINT_AST_SAMPLER_H_
#define SRC_TINT_AST_SAMPLER_H_

#include <string>

#include "src/tint/ast/type.h"
#include "src/tint/type/sampler_kind.h"

namespace tint::ast {

/// A sampler type.
class Sampler final : public Castable<Sampler, Type> {
  public:
    /// Constructor
    /// @param pid the identifier of the program that owns this node
    /// @param nid the unique node identifier
    /// @param src the source of this node
    /// @param kind the kind of sampler
    Sampler(ProgramID pid, NodeID nid, const Source& src, type::SamplerKind kind);
    /// Move constructor
    Sampler(Sampler&&);
    ~Sampler() override;

    /// @returns true if this is a comparison sampler
    bool IsComparison() const { return kind == type::SamplerKind::kComparisonSampler; }

    /// @param symbols the program's symbol table
    /// @returns the name for this type that closely resembles how it would be
    /// declared in WGSL.
    std::string FriendlyName(const SymbolTable& symbols) const override;

    /// Clones this type and all transitive types using the `CloneContext` `ctx`.
    /// @param ctx the clone context
    /// @return the newly cloned type
    const Sampler* Clone(CloneContext* ctx) const override;

    /// The sampler type
    const type::SamplerKind kind;
};

}  // namespace tint::ast

#endif  // SRC_TINT_AST_SAMPLER_H_
