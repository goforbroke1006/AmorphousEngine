//
// Created by goforbroke on 12/30/23.
//

#include <gtest/gtest.h>

#include <fstream>
#include <filesystem>

#include "../../../include/GameObject.h"
#include "../../../include/Calculation/Lua53.h"

TEST(TestLua53_buildInitLuaCode, _00_empty_scene) {
    const auto &actual = Lua53::buildInitLuaCode({});

    EXPECT_EQ("", actual);
}

TEST(TestLua53_buildInitLuaCode, _01_scene_with_gObjects) {
    auto *playerGO = new GameObject("go 0 :: Player", "Player");
    playerGO->mTransform->mPosition.Set(10.0, 20.0, 30.0);

    auto *bot1GO = new GameObject("go 1 :: Bot 1", "Bot 1");
    bot1GO->mTransform->mPosition.Set(40.0, 50.0, 60.0);
    bot1GO->mTransform->mRotation.Set(0.0, 45.0, 0.0);

    std::map<std::string, GameObject *> goList = {
            {playerGO->mID, playerGO},
            {bot1GO->mID,   bot1GO},
    };

    const auto &actual = Lua53::buildInitLuaCode(goList);

    for (const auto &goPair: goList)
        delete goPair.second;

    std::string expected
            = "require 'Core'\n"
              "\n"
              "allGameObjects['go 0 :: Player'] = GameObject:new('go 0 :: Player', 'Player')\n"
              "allGameObjects['go 0 :: Player'].transform.position:Set(10.000000, 20.000000, 30.000000)\n"
              "allGameObjects['go 0 :: Player'].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.0)\n"
              "allGameObjects['go 0 :: Player'].transform.localScale:Set(0.000000, 0.000000, 0.000000)\n"
              "\n"
              "allGameObjects['go 1 :: Bot 1'] = GameObject:new('go 1 :: Bot 1', 'Bot 1')\n"
              "allGameObjects['go 1 :: Bot 1'].transform.position:Set(40.000000, 50.000000, 60.000000)\n"
              "allGameObjects['go 1 :: Bot 1'].transform.rotation:Set(0.000000, 45.000000, 0.000000, 1.0)\n"
              "allGameObjects['go 1 :: Bot 1'].transform.localScale:Set(0.000000, 0.000000, 0.000000)\n"
              "\n"
              "for _, cmpInstance in pairs(allComponents) do\n"
              "    cmpInstance:Start()\n"
              "end\n";

    EXPECT_EQ(expected, actual);
}

TEST(TestLua53_buildInitLuaCode, _01_scene_with_gObjects_and_cmp) {
    auto *mainCameraGO = new GameObject("go 0 :: Main Camera", "Main Camera");
    mainCameraGO->mTransform->mPosition.Set(100.0, 100.0, 100.0);
    mainCameraGO->mComponents["Camera"] =
            Component(
                    "Camera",
                    "Component/Camera",
                    {
                            {"backgroundColor",
                             Property{"backgroundColor", PropType::PropTypeColor, Color{0.25, 0.75, 0.25, 1.0}}},
                    }
            );

    auto *playerGO = new GameObject("go 1 :: Player", "Player");
    playerGO->mTransform->mPosition.Set(1.0, 2.0, 3.0);
    playerGO->mComponents["CharacterController"] =
            Component(
                    "CharacterController",
                    "Scripts/CharacterController",
                    {
                            {"camera",
                                    Property{"camera", PropType::PropTypeGameObjectTransform, "go 0 :: Main Camera"}},
                            {"runningSpeed",
                                    Property{"runningSpeed", PropType::PropTypeDouble, 2.0}}
                    }
            );

    auto *bot1GO = new GameObject("go 2 :: Bot 1", "Bot 1");
    bot1GO->mTransform->mPosition.Set(4.0, 5.0, 6.0);
    bot1GO->mTransform->mRotation.Set(0.0, 90.0, 0.0);

    std::map<std::string, GameObject *> goList = {
            {mainCameraGO->mID, mainCameraGO},
            {playerGO->mID,     playerGO},
            {bot1GO->mID,       bot1GO},
    };

    const auto &actual = Lua53::buildInitLuaCode(goList);

    std::string expected
            = "require 'Core'\n"
              "\n"
              "allGameObjects['go 0 :: Main Camera'] = GameObject:new('go 0 :: Main Camera', 'Main Camera')\n"
              "allGameObjects['go 0 :: Main Camera'].transform.position:Set(100.000000, 100.000000, 100.000000)\n"
              "allGameObjects['go 0 :: Main Camera'].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.0)\n"
              "\n"
              "allGameObjects['go 1 :: Player'] = GameObject:new('go 1 :: Player', 'Player')\n"
              "allGameObjects['go 1 :: Player'].transform.position:Set(1.000000, 2.000000, 3.000000)\n"
              "allGameObjects['go 1 :: Player'].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.0)\n"
              "allGameObjects['go 1 :: Player'].transform.localScale:Set(0.000000, 0.000000, 0.000000)\n"
              "\n"
              "allGameObjects['go 2 :: Bot 1'] = GameObject:new('go 2 :: Bot 1', 'Bot 1')\n"
              "allGameObjects['go 2 :: Bot 1'].transform.position:Set(4.000000, 5.000000, 6.000000)\n"
              "allGameObjects['go 2 :: Bot 1'].transform.rotation:Set(0.000000, 90.000000, 0.000000, 1.0)\n"
              "allGameObjects['go 2 :: Bot 1'].transform.localScale:Set(0.000000, 0.000000, 0.000000)\n"
              "\n"
              "require 'Component/Camera'\n"
              "\n"
              "local cmp = Camera\n"
              "cmp.__name = 'Camera'\n"
              "cmp.gameObject = allGameObjects['go 0 :: Main Camera']\n"
              "cmp.transform = allGameObjects['go 0 :: Main Camera'].transform\n"
              "\n"
              "cmp.backgroundColor = Color:new(0.250000, 0.750000, 0.250000, 1.000000)\n"
              "cmp.enabled = true\n"
              "cmp.farClipPlane = 1000.000000\n"
              "cmp.nearClipPlane = 0.100000\n"
              "\n"
              "allComponents['go 0 :: Main Camera :: Camera'] = cmp\n"
              "\n"
              "require 'Scripts/CharacterController'\n"
              "\n"
              "local cmp = CharacterController\n"
              "cmp.__name = 'CharacterController'\n"
              "cmp.gameObject = allGameObjects['go 1 :: Player']\n"
              "cmp.transform = allGameObjects['go 1 :: Player'].transform\n"
              "\n"
              "cmp.camera = allGameObjects['go 0 :: Main Camera'].transform\n"
              "cmp.enabled = true\n"
              "cmp.runningSpeed = 2.000000\n"
              "\n"
              "allComponents['go 1 :: Player :: CharacterController'] = cmp\n"
              "\n"
              "for _, cmpInstance in pairs(allComponents) do\n"
              "    cmpInstance:Start()\n"
              "end\n";

    EXPECT_EQ(expected, actual);
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

    auto *mainCameraGO = new GameObject("id 0 :: Main Camera", "Main Camera");
    mainCameraGO->mTransform->mPosition.Set(100.0, 100.0, 100.0);
    mainCameraGO->mComponents["Camera"] =
            Component(
                    "Camera",
                    "Component/Camera",
                    {
                            {"backgroundColor",
                             Property{"backgroundColor", PropType::PropTypeColor, Color{0.25, 0.75, 0.25, 1.0}}},
                    }
            );

    auto *box1GO = new GameObject("id 1 :: Box 1", "Box 1");
    box1GO->mTransform->mPosition.Set(0.0, 0.0, 0.0);

    auto *drone1GO = new GameObject("id 2 :: Drone 1", "Drone 1");
    drone1GO->mTransform->mPosition.Set(4.0, 5.0, 6.0);
    drone1GO->mTransform->mRotation.Set(45.0, 45.0, 45.0);
    drone1GO->mComponents["DroneController"] =
            Component(
                    "DroneController",
                    "Scripts/DroneController",
                    {
                            {"motionSpeed",
                                    Property{"motionSpeed", PropType::PropTypeDouble, 2.0}},
                            {"targetTr",
                                    Property{"targetTr", PropType::PropTypeGameObjectTransform,
                                             (std::string) "id 1 :: Box 1"}},
                    }
            );

    std::map<std::string, GameObject *> goList = {
            {mainCameraGO->mID, mainCameraGO},
            {box1GO->mID,       box1GO},
            {drone1GO->mID,     drone1GO},
    };

    EXPECT_EQ(0.0, box1GO->mTransform->mPosition.mX);
    EXPECT_EQ(0.0, box1GO->mTransform->mPosition.mY);
    EXPECT_EQ(0.0, box1GO->mTransform->mPosition.mZ);

    EXPECT_EQ(4.0, drone1GO->mTransform->mPosition.mX);
    EXPECT_EQ(5.0, drone1GO->mTransform->mPosition.mY);
    EXPECT_EQ(6.0, drone1GO->mTransform->mPosition.mZ);

    Lua53 target("./");
    target.initialize(goList);
    target.update(goList);
    target.update(goList);
    target.update(goList);

    EXPECT_EQ(0.0, box1GO->mTransform->mPosition.mX);
    EXPECT_EQ(0.0, box1GO->mTransform->mPosition.mY);
    EXPECT_EQ(0.0, box1GO->mTransform->mPosition.mZ);

    EXPECT_NE(4.0, drone1GO->mTransform->mPosition.mX);
//    EXPECT_NE(5.0, drone1GO->mTransform->mPosition.mY); // TODO:
//    EXPECT_NE(6.0, drone1GO->mTransform->mPosition.mZ); // TODO:
}
