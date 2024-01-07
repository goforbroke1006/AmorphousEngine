//
// Created by goforbroke on 12/30/23.
//

#include <gtest/gtest.h>

#include <LuaCpp.hpp>

TEST(TestVector3, file_compilation) {
    LuaCpp::LuaContext ctx;
    EXPECT_NO_THROW(ctx.CompileString("", "require 'Core/Vector3'"));
}

TEST(TestVector3_predefined, zero) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3.zero \n"
            "print(vec.x .. ' ' .. vec.y .. ' ' .. vec.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.0 0.0 0.0\n", output);
}

TEST(TestVector3_predefined, forward) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3.forward \n"
            "print(vec.x .. ' ' .. vec.y .. ' ' .. vec.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.0 0.0 1.0\n", output);
}

TEST(TestVector3_predefined, back) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3.back \n"
            "print(vec.x .. ' ' .. vec.y .. ' ' .. vec.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.0 0.0 -1.0\n", output);
}

TEST(TestVector3_predefined, up) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3.up \n"
            "print(vec.x .. ' ' .. vec.y .. ' ' .. vec.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.0 1.0 0.0\n", output);
}

TEST(TestVector3_predefined, down) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3.down \n"
            "print(vec.x .. ' ' .. vec.y .. ' ' .. vec.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.0 -1.0 0.0\n", output);
}

TEST(TestVector3_predefined, right) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3.right \n"
            "print(vec.x .. ' ' .. vec.y .. ' ' .. vec.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("1.0 0.0 0.0\n", output);
}

TEST(TestVector3_predefined, left) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3.left \n"
            "print(vec.x .. ' ' .. vec.y .. ' ' .. vec.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("-1.0 0.0 0.0\n", output);
}

TEST(TestVector3_magnitude, magnitude_of_zero) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(0.0, 0.0, 0.0) \n"
            "local len = vec:magnitude() \n"
            "print(len) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.0\n", output);
}

TEST(TestVector3_magnitude, magnitude_of_1_1_1) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(1.0, 1.0, 1.0) \n"
            "local len = vec:magnitude() \n"
            "print(len) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("1.7320508075689\n", output);
}

TEST(TestVector3_magnitude, magnitude_of_0_5_0) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(0.0, 5.0, 0.0) \n"
            "local len = vec:magnitude() \n"
            "print(len) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("5.0\n", output);
}

TEST(TestVector3_normalized, normalized_of_0_0_0) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(0.0, 0.0, 0.0) \n"
            "vec = vec:normalized() \n"
            "print(vec.x .. ' ' .. vec.y .. ' ' .. vec.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.0 0.0 0.0\n", output);
}

TEST(TestVector3_normalized, normalized_of_5_0_0) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(5.0, 0.0, 0.0) \n"
            "vec = vec:normalized() \n"
            "print(vec.x .. ' ' .. vec.y .. ' ' .. vec.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("1.0 0.0 0.0\n", output);
}

TEST(TestVector3_normalized, normalized_of_5_5_5) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(5.0, 5.0, 5.0) \n"
            "vec = vec:normalized() \n"
            "print(vec.x .. ' ' .. vec.y .. ' ' .. vec.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.57735026918963 0.57735026918963 0.57735026918963\n", output);
}

TEST(TestVector3_Normalize, normalization_of_0_0_0) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(0.0, 0.0, 0.0) \n"
            "vec:Normalize() \n"
            "print(vec.x .. ' ' .. vec.y .. ' ' .. vec.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.0 0.0 0.0\n", output);
}

TEST(TestVector3_Distance, _0_0_0_between_0_0_0) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(0.0, 0.0, 0.0) \n"
            "local other = Vector3:new(0.0, 0.0, 0.0) \n"
            "local distance = vec:Distance(other) \n"
            "print(distance) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.0\n", output);
}

TEST(TestVector3_Distance, _0_0_0_between_0_5_0) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(0.0, 0.0, 0.0) \n"
            "local other = Vector3:new(0.0, 5.0, 0.0) \n"
            "local distance = vec:Distance(other) \n"
            "print(distance) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("5.0\n", output);
}

TEST(TestVector3_Distance, _1_1_1_between_5_5_5) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(1.0, 1.0, 1.0) \n"
            "local other = Vector3:new(5.0, 5.0, 5.0) \n"
            "local distance = vec:Distance(other) \n"
            "print(distance) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("6.9282032302755\n", output);
}

TEST(TestVector3_Normalize, normalization_of_5_5_5) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(5.0, 5.0, 5.0) \n"
            "vec:Normalize() \n"
            "print(vec.x .. ' ' .. vec.y .. ' ' .. vec.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.57735026918963 0.57735026918963 0.57735026918963\n", output);
}

TEST(TestVector3___add, _0_0_0_plus_0_0_0) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec1 = Vector3:new(0.0, 0.0, 0.0) \n"
            "local vec2 = Vector3:new(0.0, 0.0, 0.0) \n"
            "local res = vec1 + vec2 \n"
            "print(res.x .. ' ' .. res.y .. ' ' .. res.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.0 0.0 0.0\n", output);
}

TEST(TestVector3___add, _v1_plus_v2) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec1 = Vector3:new(0.1, 0.2, 0.3) \n"
            "local vec2 = Vector3:new(4.0, 5.0, 6.0) \n"
            "local res = vec1 + vec2 \n"
            "print(res.x .. ' ' .. res.y .. ' ' .. res.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("4.1 5.2 6.3\n", output);
}

TEST(TestVector3___add, _v1_plus_v2_plus_v3) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec1 = Vector3:new(0.1, 0.2, 0.3) \n"
            "local vec2 = Vector3:new(4.0, 5.0, 6.0) \n"
            "local vec3 = Vector3:new(0.0, 0.0, 0.0) \n"
            "local res = vec1 + vec2 + vec3 \n"
            "print(res.x .. ' ' .. res.y .. ' ' .. res.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("4.1 5.2 6.3\n", output);
}

TEST(TestVector3___sub, _v1_minus_v2) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec1 = Vector3:new(4.0, 5.0, 6.0) \n"
            "local vec2 = Vector3:new(0.1, 0.2, 0.3) \n"
            "local res = vec1 - vec2 \n"
            "print(res.x .. ' ' .. res.y .. ' ' .. res.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("3.9 4.8 5.7\n", output);
}

TEST(TestVector3___sub, _0_0_0_minus_v2) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec1 = Vector3.zero \n"
            "local vec2 = Vector3:new(0.1, 0.2, 0.3) \n"
            "local res = vec1 - vec2 \n"
            "print(res.x .. ' ' .. res.y .. ' ' .. res.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("-0.1 -0.2 -0.3\n", output);
}

TEST(TestVector3___mul, _0_0_0_mul_5) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(0.0, 0.0, 0.0) \n"
            "local res = vec * 5 \n"
            "print(res.x .. ' ' .. res.y .. ' ' .. res.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.0 0.0 0.0\n", output);
}

TEST(TestVector3___mul, _5_mul_0_0_0) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(0.0, 0.0, 0.0) \n"
            "local res = 5 * vec \n"
            "print(res.x .. ' ' .. res.y .. ' ' .. res.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.0 0.0 0.0\n", output);
}

TEST(TestVector3___mul, _2_mul_0_0_0_mul_2) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(0.0, 0.0, 0.0) \n"
            "local res = 2 * vec * 2 \n"
            "print(res.x .. ' ' .. res.y .. ' ' .. res.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.0 0.0 0.0\n", output);
}

TEST(TestVector3___mul, _3_mul_1_2_3) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(1.1, 2.2, 3.3) \n"
            "local res = 3 * vec \n"
            "print(res.x .. ' ' .. res.y .. ' ' .. res.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("3.3 6.6 9.9\n", output);
}

TEST(TestVector3___mul, _05_mul_1_2_3) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(1.0, 2.0, 3.0) \n"
            "local res = 0.5 * vec \n"
            "print(res.x .. ' ' .. res.y .. ' ' .. res.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.5 1.0 1.5\n", output);
}

TEST(TestVector3___mul, _m1_mul_1_2_3) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            ""
            "local vec = Vector3:new(1.0, 2.0, 3.0) \n"
            "local res = -1.0 * vec \n"
            "print(res.x .. ' ' .. res.y .. ' ' .. res.z) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("-1.0 -2.0 -3.0\n", output);
}

TEST(TestVector3_Dot, _positive_1) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            "\n"
            "local vec1 = Vector3:new(1.0, 2.0, 3.0)\n"
            "local vec2 = Vector3:new(6.0, 7.0, 8.0)\n"
            "\n"
            "local res = Vector3.Dot(vec1, vec2)\n"
            "print(res) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("44.0\n", output);
}

TEST(TestVector3_Dot, _positive_2) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            "\n"
            "local vec1 = Vector3:new(-1.0,  2.0, -3.0)\n"
            "local vec2 = Vector3:new( 1.0, -2.0,  3.0)\n"
            "\n"
            "local res = Vector3.Dot(vec1, vec2)\n"
            "print(res) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("-14.0\n", output);
}

TEST(TestVector3_Cross, _positive_1) {
    // https://www.nagwa.com/en/explainers/616184792816/
    // Example 1

    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            "\n"
            "local vec1 = Vector3:new(1.0, 0.0, 0.0)\n"
            "local vec2 = Vector3:new(3.0, 2.0, 4.0)\n"
            "\n"
            "local res = Vector3.Cross(vec1, vec2)\n"
            "print(res:ToString()) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("(0.0, -4.0, 2.0)\n", output);
}

TEST(TestVector3_Cross, _positive_2) {
    // https://www.nagwa.com/en/explainers/616184792816/
    // Example 2

    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/Vector3' \n"
            "\n"
            "local vec1 = Vector3:new( 1.0, -1.0, 0.0)\n"
            "local vec2 = Vector3:new(-7.0, -8.0, 6.0)\n"
            "\n"
            "local res = Vector3.Cross(vec1, vec2)\n"
            "print(res:ToString()) \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("(-6.0, -6.0, -15.0)\n", output);
}
