//
// Created by goforbroke on 12/30/23.
//

#include <gtest/gtest.h>

#include <LuaCpp/LuaContext.hpp>

TEST(TestLuaBehaviour, file_compilation) {
    LuaCpp::LuaContext ctx;
    EXPECT_NO_THROW(ctx.CompileString("", "require 'Core/LuaBehaviour'"));
}

TEST(TestTransform_LuaBehaviour, transform_ref) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/LuaBehaviour'\n"
            ""
            "local lb = LuaBehaviour:new()\n"
            "print(lb.transform.position:ToString())\n"
            "print(lb.gameObject.transform.position:ToString())\n"
            ""
            "lb.transform.position:Set(1.0, 2.0, 3.0) \n"
            ""
            "print(lb.transform.position:ToString())\n"
            "print(lb.gameObject.transform.position:ToString())\n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("(0.0, 0.0, 0.0)\n"
              "(0.0, 0.0, 0.0)\n"
              "(1.0, 2.0, 3.0)\n"
              "(1.0, 2.0, 3.0)\n", output);
}

TEST(TestTransform_LuaBehaviour___make_clone, positive_1) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/LuaBehaviour'\n"
            ""
            "local origin = LuaBehaviour:new()\n"
            "origin.someProp = 123\n"
            "origin.transform.position:Set(1.0, 2.0, 3.0)\n"
            "print(origin.transform.position:ToString())\n"
            ""
            "local clone = LuaBehaviour.__make_clone(origin)\n"
            "origin.someProp = 101\n"
            "print(clone.transform.position:ToString())\n"
            "print(clone.someProp)\n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("(1.0, 2.0, 3.0)\n"
              "(1.0, 2.0, 3.0)\n"
              "123\n", output);
}
