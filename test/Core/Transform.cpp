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

TEST(TestTransform_Translate, from_origin_no_rot_move_forward) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Transform' \n"
            ""
            "local tr = Transform:new() \n"
            "tr.position:Set(0.0, 0.0, 0.0)\n"
            "tr.rotation:Set(0.0, 0.0, 0.0, 0.0)\n"
            ""
            "tr:Translate(Vector3.forward) \n"
            ""
            "print(tr.position:ToString()) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("(0.0, 0.0, 1.0)\n", output);
}

TEST(TestTransform_Translate, from_origin_no_rot_move_left) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Transform' \n"
            ""
            "local tr = Transform:new() \n"
            "tr.position:Set(0.0, 0.0, 0.0)\n"
            "tr.rotation:Set(0.0, 0.0, 0.0, 0.0)\n"
            ""
            "tr:Translate(Vector3.left) \n"
            ""
            "print(tr.position:ToString()) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("(-1.0, 0.0, 0.0)\n", output);
}

TEST(TestTransform_Translate, from_origin_no_rot_move_up) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Transform' \n"
            ""
            "local tr = Transform:new() \n"
            "tr.position:Set(0.0, 0.0, 0.0)\n"
            "tr.rotation:Set(0.0, 0.0, 0.0, 0.0)\n"
            ""
            "tr:Translate(Vector3.up) \n"
            ""
            "print(tr.position:ToString()) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("(0.0, 1.0, 0.0)\n", output);
}

TEST(TestTransform_Translate, from_0_0_5_rot_right_move_forward) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Transform' \n"
            ""
            "local tr = Transform:new() \n"
            "tr.position:Set(0.0, 0.0, 5.0)\n"
            "tr.rotation = Quaternion.Euler(0.0, 90.0, 0.0)\n"
            ""
            "tr:Translate(Vector3.forward) \n"
            ""
            "print(tr.position:ToString()) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("(1.0, 0.0, 5.0)\n", output);
}
