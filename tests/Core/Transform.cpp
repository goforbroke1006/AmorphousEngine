//
// Created by goforbroke on 12/30/23.
//

#include <gtest/gtest.h>

#include <LuaCpp.hpp>

TEST(TestTransform, file_compilation) {
    LuaCpp::LuaContext ctx;
    EXPECT_NO_THROW(ctx.CompileString("", "require 'Core/Transform'"));
}

TEST(TestTransform_Translate, move_zero) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Transform' \n"
            ""
            "local tr = Transform:new() \n"
            "local move = Vector3.zero \n"
            "tr:Translate(move) \n"
            "print(tr.position.x .. ' ' .. tr.position.y .. ' ' .. tr.position.z) \n"
            "print(tr.rotation.x .. ' ' .. tr.rotation.y .. ' ' .. tr.rotation.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.0 0.0 0.0\n"
              "0.0 0.0 0.0\n", output);
}

TEST(TestTransform_Translate, move_forward) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Transform' \n"
            ""
            "local tr = Transform:new() \n"
            "local move = Vector3.forward \n"
            "tr:Translate(move) \n"
            "print(tr.position.x .. ' ' .. tr.position.y .. ' ' .. tr.position.z) \n"
            "print(tr.rotation.x .. ' ' .. tr.rotation.y .. ' ' .. tr.rotation.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.0 0.0 1.0\n"
              "0.0 0.0 0.0\n", output);
}
