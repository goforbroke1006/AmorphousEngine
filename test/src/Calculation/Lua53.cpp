//
// Created by goforbroke on 12/30/23.
//

#include <gtest/gtest.h>

#include "../../../include/Calculation/Lua53.h"

#include <fstream>
#include <filesystem>

namespace AmE {
    TEST(TestLua53_buildInitLuaCode, _00_empty_scene) {
        const auto &actual = Lua53::buildInitLuaCode({});

        EXPECT_EQ("", actual);
    }

    TEST(TestLua53_buildInitLuaCode, _01_scene_with_gObjects) {
        auto *playerGO = new GameObject(0, "Player");
        playerGO->getTransform()->mPosition.Set(10.0, 20.0, 30.0);

        auto *bot1GO = new GameObject(1, "Bot 1");
        bot1GO->getTransform()->mPosition.Set(40.0, 50.0, 60.0);
        bot1GO->getTransform()->mRotation = Quaternion::Euler(0.0, 45.0, 0.0);

        std::map<GameObjectInstanceID, GameObject *> goList = {
                {playerGO->getID(), playerGO},
                {bot1GO->getID(),   bot1GO},
        };

        const auto &actual = Lua53::buildInitLuaCode(goList);

        for (const auto &goPair: goList)
            delete goPair.second;

        std::string expected
                = "require 'Core'\n"
                  "\n"
                  "__all_game_objects[0] = GameObject:new(0, 'Player')\n"
                  "__all_game_objects[0].transform.position:Set(10.000000, 20.000000, 30.000000)\n"
                  "__all_game_objects[0].transform.rotation:Set(0.000000, 0.000000, 0.000000, 0.000000)\n"
                  "__all_game_objects[0].transform.localScale:Set(0.000000, 0.000000, 0.000000)\n"
                  "\n"
                  "__all_game_objects[1] = GameObject:new(1, 'Bot 1')\n"
                  "__all_game_objects[1].transform.position:Set(40.000000, 50.000000, 60.000000)\n"
                  "__all_game_objects[1].transform.rotation:Set(0.000000, 0.382683, 0.000000, 0.923880)\n"
                  "__all_game_objects[1].transform.localScale:Set(0.000000, 0.000000, 0.000000)\n"
                  "\n"
                  "require 'Core/LuaBehaviour'\n"
                  "\n"
                  "for _, cmpInstance in pairs(__all_components) do\n"
                  "    cmpInstance:Start()\n"
                  "end\n";

        EXPECT_EQ(expected, actual);
    }

    TEST(TestLua53_buildInitLuaCode, _01_scene_with_gObjects_and_cmp) {
        auto *mainCameraGO = new GameObject(0, "Main Camera");
        mainCameraGO->getTransform()->mPosition.Set(100.0, 100.0, 100.0);
        mainCameraGO->getComponents()["Camera"] =
                Component(
                        "Camera",
                        "Component/Camera",
                        {
                                {"backgroundColor",
                                 Property{"backgroundColor", PropType::PropTypeColor, Color{0.25, 0.75, 0.25, 1.0}}},
                        }
                );

        auto *playerGO = new GameObject(1, "Player");
        playerGO->getTransform()->mPosition.Set(1.0, 2.0, 3.0);
        playerGO->getComponents()["CharacterController"] =
                Component(
                        "CharacterController",
                        "Scripts/CharacterController",
                        {
                                {"camera",
                                        Property{"camera", PropType::PropTypeGameObjectTransform, 0}},
                                {"runningSpeed",
                                        Property{"runningSpeed", PropType::PropTypeDouble, 2.0}}
                        }
                );

        auto *bot1GO = new GameObject(2, "Bot 1");
        bot1GO->getTransform()->mPosition.Set(4.0, 5.0, 6.0);
        bot1GO->getTransform()->mRotation = Quaternion::Euler(0.0, 90.0, 0.0);

        std::map<GameObjectInstanceID, GameObject *> goList = {
                {mainCameraGO->getID(), mainCameraGO},
                {playerGO->getID(),     playerGO},
                {bot1GO->getID(),       bot1GO},
        };

        const auto &actual = Lua53::buildInitLuaCode(goList);

        std::string expected
                = "require 'Core'\n"
                  "\n"
                  "__all_game_objects[0] = GameObject:new(0, 'Main Camera')\n"
                  "__all_game_objects[0].transform.position:Set(100.000000, 100.000000, 100.000000)\n"
                  "__all_game_objects[0].transform.rotation:Set(0.000000, 0.000000, 0.000000, 0.000000)\n"
                  "\n"
                  "__all_game_objects[1] = GameObject:new(1, 'Player')\n"
                  "__all_game_objects[1].transform.position:Set(1.000000, 2.000000, 3.000000)\n"
                  "__all_game_objects[1].transform.rotation:Set(0.000000, 0.000000, 0.000000, 0.000000)\n"
                  "__all_game_objects[1].transform.localScale:Set(0.000000, 0.000000, 0.000000)\n"
                  "\n"
                  "__all_game_objects[2] = GameObject:new(2, 'Bot 1')\n"
                  "__all_game_objects[2].transform.position:Set(4.000000, 5.000000, 6.000000)\n"
                  "__all_game_objects[2].transform.rotation:Set(0.000000, 0.707107, 0.000000, 0.707107)\n"
                  "__all_game_objects[2].transform.localScale:Set(0.000000, 0.000000, 0.000000)\n"
                  "\n"
                  "require 'Core/LuaBehaviour'\n"
                  "\n"
                  "require 'Component/Camera'\n"
                  "\n"
                  "function Camera:new()\n"
                  "    local instance = LuaBehaviour:new()\n"
                  "    setmetatable(instance, self)\n"
                  "    self.__index = self\n"
                  "    return instance\n"
                  "end\n"
                  "\n"
                  "require 'Scripts/CharacterController'\n"
                  "\n"
                  "function CharacterController:new()\n"
                  "    local instance = LuaBehaviour:new()\n"
                  "    setmetatable(instance, self)\n"
                  "    self.__index = self\n"
                  "    return instance\n"
                  "end\n"
                  "\n"
                  "__all_components['0 :: Camera'] = Camera:new()\n"
                  "__all_components['0 :: Camera'].__name = 'Camera'\n"
                  "__all_components['0 :: Camera'].gameObject = __all_game_objects[0]\n"
                  "__all_components['0 :: Camera'].transform = __all_game_objects[0].transform\n"
                  "\n"
                  "__all_components['0 :: Camera'].backgroundColor = Color:new(0.250000, 0.750000, 0.250000, 1.000000)\n"
                  "__all_components['0 :: Camera'].enabled = true\n"
                  "__all_components['0 :: Camera'].farClipPlane = 1000.000000\n"
                  "__all_components['0 :: Camera'].nearClipPlane = 0.100000\n"
                  "\n"
                  "__all_components['1 :: CharacterController'] = CharacterController:new()\n"
                  "__all_components['1 :: CharacterController'].__name = 'CharacterController'\n"
                  "__all_components['1 :: CharacterController'].gameObject = __all_game_objects[1]\n"
                  "__all_components['1 :: CharacterController'].transform = __all_game_objects[1].transform\n"
                  "\n"
                  "__all_components['1 :: CharacterController'].camera = __all_game_objects[0].transform\n"
                  "__all_components['1 :: CharacterController'].enabled = true\n"
                  "__all_components['1 :: CharacterController'].runningSpeed = 2.000000\n"
                  "\n"
                  "for _, cmpInstance in pairs(__all_components) do\n"
                  "    cmpInstance:Start()\n"
                  "end\n";

        EXPECT_EQ(expected, actual);
    }

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

        auto *mainCameraGO = new GameObject(0, "Main Camera");
        mainCameraGO->getTransform()->mPosition.Set(100.0, 100.0, 100.0);
        mainCameraGO->getComponents()["Camera"] =
                Component(
                        "Camera",
                        "Component/Camera",
                        {
                                {"backgroundColor",
                                 Property{"backgroundColor", PropType::PropTypeColor, Color{0.25, 0.75, 0.25, 1.0}}},
                        }
                );

        auto *box1GO = new GameObject(1, "Box 1");
        box1GO->getTransform()->mPosition.Set(0.0, 0.0, 0.0);

        auto *drone1GO = new GameObject(2, "Drone 1");
        drone1GO->getTransform()->mPosition.Set(4.0, 5.0, 6.0);
        drone1GO->getTransform()->mRotation = Quaternion::Euler(45.0, 45.0, 45.0);
        drone1GO->getComponents()["DroneController"] =
                Component(
                        "DroneController",
                        "Scripts/DroneController",
                        {
                                {"motionSpeed",
                                        Property{"motionSpeed", PropType::PropTypeDouble, 2.0}},
                                {"targetTr",
                                        Property{"targetTr", PropType::PropTypeGameObjectTransform, 1}},
                        }
                );

        std::map<GameObjectInstanceID, GameObject *> goList = {
                {mainCameraGO->getID(), mainCameraGO},
                {box1GO->getID(),       box1GO},
                {drone1GO->getID(),     drone1GO},
        };

        EXPECT_EQ(0.0, box1GO->getTransform()->mPosition.mX);
        EXPECT_EQ(0.0, box1GO->getTransform()->mPosition.mY);
        EXPECT_EQ(0.0, box1GO->getTransform()->mPosition.mZ);

        EXPECT_EQ(4.0, drone1GO->getTransform()->mPosition.mX);
        EXPECT_EQ(5.0, drone1GO->getTransform()->mPosition.mY);
        EXPECT_EQ(6.0, drone1GO->getTransform()->mPosition.mZ);

        Lua53 target("./");
        target.initialize(goList);

        auto *preUpdateFrameData = new InputsState();
        auto *sceneState = new SceneState(goList);

        target.update(preUpdateFrameData, sceneState);
        target.update(preUpdateFrameData, sceneState);
        target.update(preUpdateFrameData, sceneState);

        EXPECT_EQ(0.0, box1GO->getTransform()->mPosition.mX);
        EXPECT_EQ(0.0, box1GO->getTransform()->mPosition.mY);
        EXPECT_EQ(0.0, box1GO->getTransform()->mPosition.mZ);

        EXPECT_NE(4.0, drone1GO->getTransform()->mPosition.mX);
//    EXPECT_NE(5.0, drone1GO->mTransform->mPosition.mY); // TODO:
//    EXPECT_NE(6.0, drone1GO->mTransform->mPosition.mZ); // TODO:
    }

    TEST(TestLua53_update, _01_update_frame_with_input_press_Fire1) {
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

        auto *playerGO = new GameObject(0, "Player");
        playerGO->getComponents()["PlayerInput"] = Component("PlayerInput", "Scripts/PlayerInput", {});

        std::map<GameObjectInstanceID, GameObject *> goList = {
                {playerGO->getID(), playerGO},
        };

        Lua53 target("./");
        target.initialize(goList);

        auto *preUpdateFrameData = new InputsState();
        auto *sceneState = new SceneState(goList);

        preUpdateFrameData->pressed[KeyCode_Mouse0] = true;
        preUpdateFrameData->released[KeyCode_Mouse0] = false;

        testing::internal::CaptureStdout();
        target.update(preUpdateFrameData, sceneState);
        std::string output = testing::internal::GetCapturedStdout();
        EXPECT_EQ("DEBUG: PlayerInput :: Fire1 pressed\n"
                  "DEBUG: PlayerInput :: Fire1 hold\n", output);
    }

    TEST(TestLua53_update, _01_update_frame_with_input_full_Fire1) {
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

        auto *playerGO = new GameObject(0, "Player");
        playerGO->getComponents()["PlayerInput"] = Component("PlayerInput", "Scripts/PlayerInput", {});

        std::map<GameObjectInstanceID, GameObject *> goList = {
                {playerGO->getID(), playerGO},
        };

        Lua53 target("./");
        target.initialize(goList);

        auto *preUpdateFrameData = new InputsState();
        auto *sceneState = new SceneState(goList);

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
