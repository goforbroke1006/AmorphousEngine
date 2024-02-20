//
// Created by goforbroke on 12/30/23.
//

#include <gtest/gtest.h>

#include <LuaCpp/LuaContext.hpp>

TEST(TestObject_file, Object_file_compilation) {
    LuaCpp::LuaContext ctx;
    EXPECT_NO_THROW(ctx.CompileString("", "require 'Core/Object'"));
}

TEST(TestObject_Instantiate, create_2_clones) {
    /**
     * Create one origin GameObject.
     * Create two clones from this origin.
     * Ensure that clones got components.
     * Ensure that Update func invocation works.
     */

    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileFile("test2", "./testdata/Object/Instantiate/create_2_clones.lua");
    ctx.Run("test2");

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0\n" // no GOs
              "0\n" // no components
              // create clone1
              "Some entity (Clone)\n" // clone1.name
              "1\n" // GOs on scene
              "1\n" // components owners
              "2\n" // component of clone1
              "Scripts/BotController\n"
              "Scripts/EnemyController\n"
              //
              // create clone2
              "Some entity (Clone)\n" // clone2.name
              "2\n"
              "2\n"
              "2\n"
              "Scripts/BotController\n"
              "Scripts/EnemyController\n"
              //
              "1.0\n" // clone1 motionSpeed
              "1.0\n" // clone2 motionSpeed
              "(10.0, 20.0, 30.0)\n"
              "(10.0, 20.0, 30.0)\n"
              "(10.0, 20.0, 32.0)\n"
              "(10.0, 20.0, 34.0)\n", output);
}

TEST(TestObject_Destroy, create_two_detroy_both) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileFile("test", "./testdata/Object/Destroy/create_two_detroy_both.lua");
    ctx.Run("test");

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0\n"
              "0\n"
              "Some entity (Clone)\n"
              "1\n"
              "1\n"
              "Some entity (Clone)\n"
              "2\n"
              "2\n"
              // destroy 1
              "1\n"
              "1\n"
              // destroy 2
              "0\n"
              "0\n", output);
}

TEST(TestObject_FindObjectsOfType, positive1) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileFile("test", "./testdata/Object/FindObjectsOfType/positive1.lua");
    ctx.Run("test");

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("2\n"
              "1\n"
              "1\n", output);
}
