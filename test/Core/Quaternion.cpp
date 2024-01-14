//
// Created by goforbroke on 12/30/23.
//

#include <gtest/gtest.h>

#include <LuaCpp.hpp>

TEST(TestQuaternion, file_compilation) {
    LuaCpp::LuaContext ctx;
    EXPECT_NO_THROW(ctx.CompileString("", "require 'Core/Quaternion'"));
}

TEST(TestQuaternion_isA, positive_1) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Quaternion' \n"
            ""
            "local qtr = Quaternion.identity \n"
            "print(qtr:IsA('Quaternion')) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("true\n", output);
}

TEST(TestQuaternion_isA, negative_all) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Quaternion' \n"
            ""
            "local qtr = Quaternion.identity \n"
            "print(qtr:IsA('Vector3')) \n"
            "print(qtr:IsA('Transform')) \n"
            "print(qtr:IsA('GameObject')) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("false\n"
              "false\n"
              "false\n", output);
}

TEST(TestQuaternion___mul, look_forward_rotate_right) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            "require 'Core/Quaternion' \n"
            ""
            "local vec = Vector3:new(0.0, 0.0, 1.0)\n"
            "local qtr = Quaternion.Euler(0.0, 90.0, 0.0)\n"
            ""
            "local res = qtr * vec \n"
            ""
            "print(res:ToString()) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("(1.0, 0.0, 0.0)\n", output);
}

TEST(TestQuaternion___mul, look_forward_rotate_left) {
//    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            "require 'Core/Quaternion' \n"
            ""
            "local vec = Vector3:new(0.0, 0.0, 1.0)\n"
            "local qtr = Quaternion.Euler(0.0, -270.0, 0.0)\n"
            ""
            "local res = qtr * vec \n"
            ""
            "print(res:ToString()) \n"
    );

//    std::string output = testing::internal::GetCapturedStdout();
//    EXPECT_EQ("(-1.0, 0.0, 0.0)\n", output);
}
