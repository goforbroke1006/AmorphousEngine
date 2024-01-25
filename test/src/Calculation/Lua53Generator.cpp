//
// Created by goforbroke on 12/30/23.
//

#include <gtest/gtest.h>

#include "../../../include/Calculation/Lua53Generator.h"

namespace AmE {
    TEST(TestLua53Generator_buildInitLuaCode, _00_empty_scene) {
        auto pSceneState = new SceneState();

        const auto &actual = Lua53Generator::buildInitLuaCode(pSceneState);

        EXPECT_EQ("", actual);
    }

    TEST(TestLua53Generator_buildInitLuaCode, _01_scene_with_gObjects) {
        auto *pSceneState = SceneState::loadFromFile(
                ".",
                "testdata/Scenes/TestLua53Generator_buildInitLuaCode--_01_scene_with_gObjects.json");

        const auto &actual = Lua53Generator::buildInitLuaCode(pSceneState);

        delete pSceneState;

        std::string expected
                = "require 'Core'\n"
                  "require 'Component'\n"
                  "\n"
                  "__all_game_objects[0] = GameObject:new(0, 'Player')\n"
                  "__all_game_objects[0].transform.position:Set(10.000000, 20.000000, 30.000000)\n"
                  "__all_game_objects[0].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.000000)\n"
                  "__all_game_objects[0].transform.localScale:Set(1.000000, 1.000000, 1.000000)\n"
                  "\n"
                  "__all_game_objects[1] = GameObject:new(1, 'Bot 1')\n"
                  "__all_game_objects[1].transform.position:Set(40.000000, 50.000000, 60.000000)\n"
                  "__all_game_objects[1].transform.rotation:Set(0.000000, 0.382683, 0.000000, 0.923880)\n"
                  "__all_game_objects[1].transform.localScale:Set(1.000000, 1.000000, 1.000000)\n"
                  "\n"
                  "require 'Core/LuaBehaviour'\n"
                  "\n"
                  "for _, cmpInstance in pairs(__all_components) do\n"
                  "    cmpInstance:Start()\n"
                  "end\n";

        EXPECT_EQ(expected, actual);
    }

    TEST(TestLua53Generator_buildInitLuaCode, _01_scene_with_gObjects_and_cmp) {
        auto pSceneState = SceneState::loadFromFile(
                ".",
                "testdata/Scenes/TestLua53Generator_buildInitLuaCode--_01_scene_with_gObjects_and_cmp.json");

        const auto &actual = Lua53Generator::buildInitLuaCode(pSceneState);

        delete pSceneState;

        std::string expected =
                "require 'Core'\n"
                "require 'Component'\n"
                "\n"
                "__all_game_objects[0] = GameObject:new(0, 'Main Camera')\n"
                "__all_game_objects[0].transform.position:Set(100.000000, 100.000000, 100.000000)\n"
                "__all_game_objects[0].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.000000)\n"
                "\n"
                "__all_game_objects[1] = GameObject:new(1, 'Player')\n"
                "__all_game_objects[1].transform.position:Set(40.000000, 50.000000, 60.000000)\n"
                "__all_game_objects[1].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.000000)\n"
                "__all_game_objects[1].transform.localScale:Set(1.000000, 1.000000, 1.000000)\n"
                "\n"
                "__all_game_objects[2] = GameObject:new(2, 'Bot 1')\n"
                "__all_game_objects[2].transform.position:Set(4.000000, 5.000000, 6.000000)\n"
                "__all_game_objects[2].transform.rotation:Set(0.000000, 0.707107, 0.000000, 0.707107)\n"
                "__all_game_objects[2].transform.localScale:Set(1.000000, 1.000000, 1.000000)\n"
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
                "require 'Scripts/ThirdCameraController'\n"
                "\n"
                "function ThirdCameraController:new()\n"
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
                "__all_components['1 :: ThirdCameraController'] = ThirdCameraController:new()\n"
                "__all_components['1 :: ThirdCameraController'].__name = 'ThirdCameraController'\n"
                "__all_components['1 :: ThirdCameraController'].gameObject = __all_game_objects[1]\n"
                "__all_components['1 :: ThirdCameraController'].transform = __all_game_objects[1].transform\n"
                "\n"
                "__all_components['1 :: ThirdCameraController'].camera = __all_game_objects[0].transform\n"
                "__all_components['1 :: ThirdCameraController'].enabled = true\n"
                "__all_components['1 :: ThirdCameraController'].runningSpeed = 2.000000\n"
                "\n"
                "for _, cmpInstance in pairs(__all_components) do\n"
                "    cmpInstance:Start()\n"
                "end\n";

        EXPECT_EQ(expected, actual);
    }
}

namespace AmE {
    TEST(TestLua53Generator_generateInitCode, for_GameObject) {
        auto pGameObj = new GameObject(-1, "Some object");
        pGameObj->addComponent(
                new Component(
                        "CharacterController",
                        "Component/CharacterController",
                        {
                                {
                                        "center",
                                        Property{"center", PropType::Kind::PropTypeVector3, Vector3(0.0, 0.0, 0.0)},
                                },
                                {
                                        "height",
                                        Property{"height", PropType::Kind::PropTypeDouble, 1.6},
                                },
                        }
                ));

        auto actual = Lua53Generator::generateInitCode(
                "__global_prefab_game_objects['./Prefabs/Oleg.json']",
                pGameObj,
                false);

        const std::string &expected =
                "__global_prefab_game_objects['./Prefabs/Oleg.json'] = GameObject:new(-1, 'Some object')\n"
                "__global_prefab_game_objects['./Prefabs/Oleg.json'].transform.position:Set(0.000000, 0.000000, 0.000000)\n"
                "__global_prefab_game_objects['./Prefabs/Oleg.json'].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.000000)\n"
                "__global_prefab_game_objects['./Prefabs/Oleg.json'].transform.localScale:Set(1.000000, 1.000000, 1.000000)\n"
                "\n"
                "__global_prefab_game_objects['./Prefabs/Oleg.json'].__components['CharacterController'] = CharacterController\n"
                "__global_prefab_game_objects['./Prefabs/Oleg.json'].__components['CharacterController'].__name = 'CharacterController'\n"
                "__global_prefab_game_objects['./Prefabs/Oleg.json'].__components['CharacterController'].gameObject = nil\n"
                "__global_prefab_game_objects['./Prefabs/Oleg.json'].__components['CharacterController'].transform = nil\n"
                "__global_prefab_game_objects['./Prefabs/Oleg.json'].__components['CharacterController'].center = Vector:new(0.000000, 0.000000, 0.000000)\n"
                "__global_prefab_game_objects['./Prefabs/Oleg.json'].__components['CharacterController'].enabled = true\n"
                "__global_prefab_game_objects['./Prefabs/Oleg.json'].__components['CharacterController'].height = 1.600000\n"
                "\n";
        EXPECT_EQ(expected, actual);
    }
}


