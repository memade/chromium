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

#include "src/tint/transform/transform.h"
#include "src/tint/clone_context.h"
#include "src/tint/program_builder.h"

#include "gtest/gtest.h"

namespace tint::transform {
namespace {

using namespace tint::number_suffixes;  // NOLINT

// Inherit from Transform so we have access to protected methods
struct CreateASTTypeForTest : public testing::Test, public Transform {
    ApplyResult Apply(const Program*, const DataMap&, DataMap&) const override {
        return SkipTransform;
    }

    const ast::Type* create(std::function<type::Type*(ProgramBuilder&)> create_sem_type) {
        ProgramBuilder sem_type_builder;
        auto* sem_type = create_sem_type(sem_type_builder);
        Program program(std::move(sem_type_builder));
        CloneContext ctx(&ast_type_builder, &program, false);
        return CreateASTTypeFor(ctx, sem_type);
    }

    ProgramBuilder ast_type_builder;
};

TEST_F(CreateASTTypeForTest, Basic) {
    EXPECT_TRUE(create([](ProgramBuilder& b) { return b.create<type::I32>(); })->Is<ast::I32>());
    EXPECT_TRUE(create([](ProgramBuilder& b) { return b.create<type::U32>(); })->Is<ast::U32>());
    EXPECT_TRUE(create([](ProgramBuilder& b) { return b.create<type::F32>(); })->Is<ast::F32>());
    EXPECT_TRUE(create([](ProgramBuilder& b) { return b.create<type::Bool>(); })->Is<ast::Bool>());
    EXPECT_EQ(create([](ProgramBuilder& b) { return b.create<type::Void>(); }), nullptr);
}

TEST_F(CreateASTTypeForTest, Matrix) {
    auto* mat = create([](ProgramBuilder& b) {
        auto* column_type = b.create<type::Vector>(b.create<type::F32>(), 2u);
        return b.create<type::Matrix>(column_type, 3u);
    });
    ASSERT_TRUE(mat->Is<ast::Matrix>());
    ASSERT_TRUE(mat->As<ast::Matrix>()->type->Is<ast::F32>());
    ASSERT_EQ(mat->As<ast::Matrix>()->columns, 3u);
    ASSERT_EQ(mat->As<ast::Matrix>()->rows, 2u);
}

TEST_F(CreateASTTypeForTest, Vector) {
    auto* vec =
        create([](ProgramBuilder& b) { return b.create<type::Vector>(b.create<type::F32>(), 2u); });
    ASSERT_TRUE(vec->Is<ast::Vector>());
    ASSERT_TRUE(vec->As<ast::Vector>()->type->Is<ast::F32>());
    ASSERT_EQ(vec->As<ast::Vector>()->width, 2u);
}

TEST_F(CreateASTTypeForTest, ArrayImplicitStride) {
    auto* arr = create([](ProgramBuilder& b) {
        return b.create<type::Array>(b.create<type::F32>(), b.create<type::ConstantArrayCount>(2u),
                                     4u, 4u, 32u, 32u);
    });
    ASSERT_TRUE(arr->Is<ast::Array>());
    ASSERT_TRUE(arr->As<ast::Array>()->type->Is<ast::F32>());
    ASSERT_EQ(arr->As<ast::Array>()->attributes.Length(), 0u);

    auto* size = arr->As<ast::Array>()->count->As<ast::IntLiteralExpression>();
    ASSERT_NE(size, nullptr);
    EXPECT_EQ(size->value, 2);
}

TEST_F(CreateASTTypeForTest, ArrayNonImplicitStride) {
    auto* arr = create([](ProgramBuilder& b) {
        return b.create<type::Array>(b.create<type::F32>(), b.create<type::ConstantArrayCount>(2u),
                                     4u, 4u, 64u, 32u);
    });
    ASSERT_TRUE(arr->Is<ast::Array>());
    ASSERT_TRUE(arr->As<ast::Array>()->type->Is<ast::F32>());
    ASSERT_EQ(arr->As<ast::Array>()->attributes.Length(), 1u);
    ASSERT_TRUE(arr->As<ast::Array>()->attributes[0]->Is<ast::StrideAttribute>());
    ASSERT_EQ(arr->As<ast::Array>()->attributes[0]->As<ast::StrideAttribute>()->stride, 64u);

    auto* size = arr->As<ast::Array>()->count->As<ast::IntLiteralExpression>();
    ASSERT_NE(size, nullptr);
    EXPECT_EQ(size->value, 2);
}

// crbug.com/tint/1764
TEST_F(CreateASTTypeForTest, AliasedArrayWithComplexOverrideLength) {
    // override O = 123;
    // type A = array<i32, O*2>;
    //
    // var<workgroup> W : A;
    //
    ProgramBuilder b;
    auto* arr_len = b.Mul("O", 2_a);
    b.Override("O", b.Expr(123_a));
    auto* alias = b.Alias("A", b.ty.array(b.ty.i32(), arr_len));

    Program program(std::move(b));

    auto* arr_ty = program.Sem().Get(alias);

    CloneContext ctx(&ast_type_builder, &program, false);
    auto* ast_ty = tint::As<ast::TypeName>(CreateASTTypeFor(ctx, arr_ty));
    ASSERT_NE(ast_ty, nullptr);
    EXPECT_EQ(ast_type_builder.Symbols().NameFor(ast_ty->name->symbol), "A");
}

TEST_F(CreateASTTypeForTest, Struct) {
    auto* str = create([](ProgramBuilder& b) {
        auto* decl = b.Structure("S", {});
        return b.create<sem::Struct>(decl, decl->source, decl->name, utils::Empty, 4u /* align */,
                                     4u /* size */, 4u /* size_no_padding */);
    });
    ASSERT_TRUE(str->Is<ast::TypeName>());
    EXPECT_EQ(ast_type_builder.Symbols().NameFor(str->As<ast::TypeName>()->name->symbol), "S");
}

}  // namespace
}  // namespace tint::transform
