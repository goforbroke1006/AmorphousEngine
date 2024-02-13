//
// Created by goforbroke on 12/30/23.
//

#include <gtest/gtest.h>

#include <LuaCpp/LuaContext.hpp>

TEST(TestObject, file_compilation) {
    LuaCpp::LuaContext ctx;
    EXPECT_NO_THROW(ctx.CompileString("", "require 'Core/Object'"));
}

/**
 * Create one origin GameObject.
 * Create two clones from this origin.
 * Ensure that clones got components.
 * Ensure that Update func invocation works.
 */
TEST(TestObject_Instantiate, create_2_clones_and_modify_props_independently) {
    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/__lib'\n"
            "require 'Core/__external'\n"
            "require 'Core/LuaBehaviour'\n"
            "require 'Core/Object'\n"
            "require 'Core/GameObject'\n"
            ""
            "-- ensure that scene empty\n"
            "print(table_length(__all_game_objects))\n"
            "print(table_length(__all_components))\n"
            ""
            "BotController = LuaBehaviour:new()\n"
            "BotController.motionSpeed = 1.0\n"
            "function BotController:Start()  end\n"
            "function BotController:Update()\n"
            "  self.transform:Translate(Vector3.forward * self.motionSpeed);\n"
            "end\n"
            ""
            "EnemyController = LuaBehaviour:new()\n"
            "function EnemyController:Start()  end\n"
            "function EnemyController:Update()  end\n"
            ""
            "local orig = GameObject:new(-1, 'Some entity')\n"
            "orig.transform.position:Set(10.0, 20.0, 30.0)\n"
            "orig.__components['BotController'] = BotController\n"
            "orig.__components['BotController'].__type_name = 'BotController'\n"
            "orig.__components['EnemyController'] = EnemyController\n"
            "orig.__components['EnemyController'].__type_name = 'EnemyController'\n"
            ""
            "local clone1 = Object.Instantiate(orig)\n"
            "print(clone1.name)\n"
            "print(table_length(__all_game_objects))\n"
            "print(table_length(__all_components))\n"
            ""
            "local clone2 = Object.Instantiate(orig)\n"
            "print(clone2.name)\n"
            "print(table_length(__all_game_objects))\n"
            "print(table_length(__all_components))\n"
            ""
            "-- ensure components were registered\n"
            "print(__all_components['0 :: BotController'].motionSpeed)\n"
            "print(__all_components['1 :: BotController'].motionSpeed)\n"
            ""
            "-- makes bot 2 faster\n"
            "__all_components['1 :: BotController'].motionSpeed = 2.0\n"
            ""
            "-- check Update works fine\n"
            "print(__all_game_objects[0].transform.position:ToString())\n"
            "print(__all_game_objects[1].transform.position:ToString())\n"
            "__all_components['0 :: BotController']:Update()\n"
            "__all_components['0 :: BotController']:Update()\n"
            "__all_components['1 :: BotController']:Update()\n"
            "__all_components['1 :: BotController']:Update()\n"
            "print(__all_game_objects[0].transform.position:ToString())\n"
            "print(__all_game_objects[1].transform.position:ToString())\n"
    );

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("0\n"
              "0\n"
              "Some entity (Clone)\n"
              "1\n"
              "2\n"
              "Some entity (Clone)\n"
              "2\n"
              "4\n"
              "1.0\n"
              "1.0\n"
              "(10.0, 20.0, 30.0)\n"
              "(10.0, 20.0, 30.0)\n"
              "(10.0, 20.0, 32.0)\n"
              "(10.0, 20.0, 34.0)\n", output);
}

TEST(TestObject_Destroy, create_two_detroy_both) {
//    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun(
            "require 'Core/__lib'\n"
            "require 'Core/__external'\n"
            "require 'Core/LuaBehaviour'\n"
            "require 'Core/Object'\n"
            "require 'Core/GameObject'\n"
            ""
            "-- ensure that scene empty\n"
            "print(table_length(__all_game_objects))\n"
            "print(table_length(__all_components))\n"
            ""
            "BotController = LuaBehaviour:new()\n"
            "BotController.motionSpeed = 1.0\n"
            "function BotController:Start()  end\n"
            "function BotController:Update()\n"
            "  self.transform:Translate(Vector3.forward * self.motionSpeed);\n"
            "end\n"
            ""
            "EnemyController = LuaBehaviour:new()\n"
            "function EnemyController:Start()  end\n"
            "function EnemyController:Update()  end\n"
            ""
            "local orig = GameObject:new(-1, 'Some entity')\n"
            "orig.transform.position:Set(10.0, 20.0, 30.0)\n"
            "orig.__components['BotController'] = BotController\n"
            "orig.__components['BotController'].__type_name = 'BotController'\n"
            "orig.__components['EnemyController'] = EnemyController\n"
            "orig.__components['EnemyController'].__type_name = 'EnemyController'\n"
            ""
            "local clone1 = Object.Instantiate(orig)\n"
            "print(clone1.name)\n"
            "print(table_length(__all_game_objects))\n"
            "print(table_length(__all_components))\n"
            ""
            "local clone2 = Object.Instantiate(orig)\n"
            "print(clone2.name)\n"
            "print(table_length(__all_game_objects))\n"
            "print(table_length(__all_components))\n"
            ""
            "-- check Update works fine\n"
            "__before_update_frame(); __on_update_frame(); __after_update_frame();\n"
            ""
            "Object.Destroy(clone1)\n"
            "print(table_length(__all_game_objects))\n"
            "print(table_length(__all_components))\n"
            ""
            "Object.Destroy(clone2)\n"
            "print(table_length(__all_game_objects))\n"
            "print(table_length(__all_components))\n"
    );

//    std::string output = testing::internal::GetCapturedStdout();
//    EXPECT_EQ("0\n"
//              "0\n"
//              "Some entity (Clone)\n"
//              "1\n"
//              "2\n"
//              "Some entity (Clone)\n"
//              "2\n"
//              "4\n"
//              "1\n"
//              "2\n"
//              "0\n"
//              "0\n", output);
}

TEST(TestObject_FindObjectsOfType, create_two_detroy_both) {
//    testing::internal::CaptureStdout();

    LuaCpp::LuaContext ctx;
    ctx.CompileFile("test", "./testdata/Object/positive1.lua");
    ctx.Run("test");

//    std::string output = testing::internal::GetCapturedStdout();
//    EXPECT_EQ("2\n"
//              "1\n"
//              "1\n", output);
}
