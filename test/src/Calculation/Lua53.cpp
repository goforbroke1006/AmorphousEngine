//
// Created by goforbroke on 12/30/23.
//

#include <gtest/gtest.h>

#include "../../../include/Calculation/Lua53.h"

#include <fstream>
#include <filesystem>

namespace AmE {

    TEST(TestLua53_buildInitLuaCode, _02_scene_with_2_GO_and_same_cmp) {
        // TODO: create test with 2 GOs and same component.
        // TODO: ensure that component modify both GOs independently.
    }

    TEST(TestLua53_update, _00_update_frame) {
        // create Component/Camera.lua
        if (!std::filesystem::is_directory("Component"))
            std::filesystem::create_directory("Component");
        ASSERT_TRUE(std::filesystem::is_directory("Component"));

        {
            std::ofstream of("Component/Camera.lua", std::ofstream::out | std::ofstream::trunc);
            of << "require 'Core/LuaBehaviour'\n"
                  "require 'Core/Color'\n"
                  "\n"
                  "Camera = LuaBehaviour:new()\n"
                  "\n"
                  "Camera.backgroundColor = Color:new()\n"
                  "Camera.nearClipPlane = 0.1\n"
                  "Camera.farClipPlane = 1000.0\n"
                  "\n"
                  "function Camera:Start()\n"
                  "    --Debug.Log(\"Camera :: Start\");\n"
                  "end\n"
                  "\n"
                  "function Camera:Update()\n"
                  "    --Debug.Log(\"Camera :: Update\");\n"
                  "end\n";
            of.close();
            ASSERT_TRUE(std::filesystem::is_regular_file("Component/Camera.lua"));
        }

        // create Scripts/DroneController.lua
        if (!std::filesystem::is_directory("Scripts"))
            std::filesystem::create_directory("Scripts");
        ASSERT_TRUE(std::filesystem::is_directory("Scripts"));

        {
            std::ofstream of("Scripts/DroneController.lua", std::ofstream::out | std::ofstream::trunc);
            of << "require 'Core'"
                  ""
                  "DroneController = LuaBehaviour:new()\n"
                  "\n"
                  "DroneController.motionSpeed = 0.0\n"
                  "DroneController.targetTr = Transform\n"
                  "\n"
                  "function DroneController:Start()\n"
                  "    Debug.Log(\"DroneController :: Start\");\n"
                  "end\n"
                  "\n"
                  "function DroneController:Update()\n"
                  "    Debug.Log(\"DroneController :: Update\");\n"
                  "\n"
                  "    local move = Vector3.right * self.motionSpeed * Time.deltaTime\n"
                  "    print('  next move ' .. move.x .. ' ' .. move.y .. ' ' .. move.z)\n"
                  "    self.transform:Translate(move);\n"
                  "end\n";
            of.close();
            ASSERT_TRUE(std::filesystem::is_regular_file("Scripts/DroneController.lua"));
        }

//        EXPECT_EQ(0.0, box1GO->getTransform()->mPosition.getX());
//        EXPECT_EQ(0.0, box1GO->getTransform()->mPosition.getY());
//        EXPECT_EQ(0.0, box1GO->getTransform()->mPosition.getZ());
//
//        EXPECT_EQ(4.0, drone1GO->getTransform()->mPosition.getX());
//        EXPECT_EQ(5.0, drone1GO->getTransform()->mPosition.getY());
//        EXPECT_EQ(6.0, drone1GO->getTransform()->mPosition.getZ());

        auto *sceneState = SceneState::loadFromFile(
                "",
                "testdata/Scenes/TestLua53_update--_00_update_frame.json");

        Lua53 target("./");
        target.initialize(sceneState);

        auto *preUpdateFrameData = new InputsState();

        target.update(preUpdateFrameData, sceneState);
        target.update(preUpdateFrameData, sceneState);
        target.update(preUpdateFrameData, sceneState);

        auto *box1GO = sceneState->getSceneGameObject(1);
        EXPECT_EQ(0.0, box1GO->getTransform()->mPosition.getX());
        EXPECT_EQ(0.0, box1GO->getTransform()->mPosition.getY());
        EXPECT_EQ(0.0, box1GO->getTransform()->mPosition.getZ());

        auto *drone1GO = sceneState->getSceneGameObject(2);
        EXPECT_NE(4.0, drone1GO->getTransform()->mPosition.getX());
//    EXPECT_NE(5.0, drone1GO->mTransform->mPosition.mY); // TODO:
//    EXPECT_NE(6.0, drone1GO->mTransform->mPosition.mZ); // TODO:
    }

    TEST(TestLua53_update, _01_input_Fire1) {
        // create Scripts/PlayerInput.lua
        if (!std::filesystem::is_directory("Scripts"))
            std::filesystem::create_directory("Scripts");
        ASSERT_TRUE(std::filesystem::is_directory("Scripts"));

        {
            std::ofstream of("Scripts/PlayerInput.lua", std::ofstream::out | std::ofstream::trunc);
            of << "require 'Core'"
                  ""
                  "PlayerInput = LuaBehaviour:new()\n"
                  "\n"
                  "function PlayerInput:Start()\n"
                  "    -- do nothing\n"
                  "end\n"
                  "\n"
                  "function PlayerInput:Update()\n"
                  "    if Input.GetButtonDown('Fire1') then\n"
                  "        Debug.Log(\"PlayerInput :: Fire1 pressed\");\n"
                  "    end\n"
                  "    if Input.GetButton('Fire1') then\n"
                  "        Debug.Log(\"PlayerInput :: Fire1 hold\");\n"
                  "    end\n"
                  "    if Input.GetButtonUp('Fire1') then\n"
                  "        Debug.Log(\"PlayerInput :: Fire1 released\");\n"
                  "    end\n"
                  "end\n";
            of.close();
            ASSERT_TRUE(std::filesystem::is_regular_file("Scripts/PlayerInput.lua"));
        }

        auto *sceneState = SceneState::loadFromFile(
                "",
                "testdata/Scenes/TestLua53_update--_01_input_Fire1.json");

        Lua53 target("./");
        target.initialize(sceneState);

        auto *preUpdateFrameData = new InputsState();

        {
            preUpdateFrameData->pressed[KeyCode_Mouse0] = true;
            preUpdateFrameData->released[KeyCode_Mouse0] = false;

            testing::internal::CaptureStdout();
            target.update(preUpdateFrameData, sceneState);
            std::string output = testing::internal::GetCapturedStdout();
            EXPECT_EQ("DEBUG: PlayerInput :: Fire1 pressed\n"
                      "DEBUG: PlayerInput :: Fire1 hold\n", output);
        }

        {
            preUpdateFrameData->pressed[KeyCode_Mouse0] = true;
            preUpdateFrameData->released[KeyCode_Mouse0] = false;

            testing::internal::CaptureStdout();
            target.update(preUpdateFrameData, sceneState);
            std::string output = testing::internal::GetCapturedStdout();
            EXPECT_EQ("DEBUG: PlayerInput :: Fire1 hold\n", output);
        }

        {
            preUpdateFrameData->pressed[KeyCode_Mouse0] = true;
            preUpdateFrameData->released[KeyCode_Mouse0] = false;

            testing::internal::CaptureStdout();
            target.update(preUpdateFrameData, sceneState);
            std::string output = testing::internal::GetCapturedStdout();
            EXPECT_EQ("DEBUG: PlayerInput :: Fire1 hold\n", output);
        }

        {
            preUpdateFrameData->pressed[KeyCode_Mouse0] = false;
            preUpdateFrameData->released[KeyCode_Mouse0] = true;

            testing::internal::CaptureStdout();
            target.update(preUpdateFrameData, sceneState);
            std::string output = testing::internal::GetCapturedStdout();
            EXPECT_EQ("DEBUG: PlayerInput :: Fire1 released\n", output);
        }
    }
}
