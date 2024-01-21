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
            "require 'Core/LuaBehaviour' \n"
            ""
            "local lb = LuaBehaviour:new() \n"
            "print("
            "    lb.transform.position.x .. ' ' .. "
            "    lb.transform.position.y .. ' ' .. "
            "    lb.transform.position.z"
            ")  \n"
            "print("
            "    lb.gameObject.transform.position.x .. ' ' .. "
            "    lb.gameObject.transform.position.y .. ' ' .. "
            "    lb.gameObject.transform.position.z"
            ")  \n"
            ""
            "lb.transform.position:Set(1.0, 2.0, 3.0) \n"
            "print("
            "    lb.transform.position.x .. ' ' .. "
            "    lb.transform.position.y .. ' ' .. "
            "    lb.transform.position.z"
            ")  \n"
            "print("
            "    lb.gameObject.transform.position.x .. ' ' .. "
            "    lb.gameObject.transform.position.y .. ' ' .. "
            "    lb.gameObject.transform.position.z"
            ")  \n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0.0 0.0 0.0\n"
              "0.0 0.0 0.0\n"
              "1.0 2.0 3.0\n"
              "1.0 2.0 3.0\n", output);
}
