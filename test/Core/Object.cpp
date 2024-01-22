//
// Created by goforbroke on 12/30/23.
//

#include <gtest/gtest.h>

#include <LuaCpp/LuaContext.hpp>

TEST(TestObject, file_compilation) {
    LuaCpp::LuaContext ctx;
    EXPECT_NO_THROW(ctx.CompileString("", "require 'Core/Vector3'"));
}

TEST(TestObject_Instantiate, signature_1_original_nil_nil_nil) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/__lib'\n"
            "require 'Core/__external'\n"
            "require 'Core/Object'\n"
            "require 'Core/GameObject'\n"
            ""
            "print(table_length(__all_game_objects))\n"
            ""
            "local orig = GameObject:new(-1, 'Some entity')\n"
            "orig.transform.position:Set(10.0, 20.0, 30.0)\n"
            ""
            "print(table_length(__all_game_objects))\n"
            ""
            "local clone = Object.Instantiate(orig)\n"
            "print(clone.name)\n"
            "print(table_length(__all_game_objects))\n"
            "print(__all_game_objects[0].name)\n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0\n"
              "0\n"
              "Some entity (Clone)\n"
              "1\n"
              "Some entity (Clone)\n", output);
}
