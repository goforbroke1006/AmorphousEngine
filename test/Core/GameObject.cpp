//
// Created by goforbroke on 12/30/23.
//

#include <gtest/gtest.h>

#include <LuaCpp/LuaContext.hpp>

TEST(TestGameObject_file, file_compilation) {
    LuaCpp::LuaContext ctx;
    EXPECT_NO_THROW(ctx.CompileString("", "require 'Core/GameObject'"));
}

TEST(TestGameObject_fields, positive) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/GameObject'\n"
            ""
            "local go = GameObject:new(10, 'Some GO')\n"
            "print(go.__instanceID ~= nil)\n"
            "print(go.name ~= nil)\n"
            "print(go.transform ~= nil)\n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("true\n"
              "true\n"
              "true\n", output);
}

TEST(TestGameObject_IsA, positive) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/GameObject'\n"
            ""
            "local go = GameObject:new(10, 'Some GO')\n"
            "print(go:IsA('GameObject'))\n"
            "print(go:IsA('Object'))\n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("true\n"
              "true\n", output);
}

