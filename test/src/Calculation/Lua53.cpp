//
// Created by goforbroke on 12/30/23.
//

#include <gtest/gtest.h>

#include "../../../include/GameObject.h"
#include "../../../include/Calculation/Lua53.h"

TEST(TestLua53_buildInitLuaCode, _00_empty_scene) {
    Lua53 target;
    const auto &actual = target.buildInitLuaCode({});

    EXPECT_EQ("", actual);
}

TEST(TestLua53_buildInitLuaCode, _01_scene_with_gObjects) {
    GameObject::sNextNumID = 0;

    std::vector<GameObject *> goList = {
            GameObject::build("Player"),
            GameObject::build("Bot 1"),
    };

    Lua53 target;
    const auto &actual = target.buildInitLuaCode(goList);

    for (auto *go: goList)
        delete go;

    std::string expected = ""
                           "require 'Core'\n"
                           "\n"
                           "allGameObjects['go 0 :: Player'] = GameObject:new('Player')\n"
                           "allGameObjects['go 0 :: Player'].transform.position:Set(0.0, 0.0, 0.0)\n"
                           "allGameObjects['go 0 :: Player'].transform.rotation:Set(0.0, 0.0, 0.0, 0.0)\n"
                           "\n"
                           "allGameObjects['go 1 :: Bot 1'] = GameObject:new('Bot 1')\n"
                           "allGameObjects['go 1 :: Bot 1'].transform.position:Set(0.0, 0.0, 0.0)\n"
                           "allGameObjects['go 1 :: Bot 1'].transform.rotation:Set(0.0, 0.0, 0.0, 0.0)\n"
                           "\n"
                           "for cmpName, cmpInstance in pairs(allComponents) do\n"
                           "    cmpInstance:Start()\n"
                           "end\n";

    EXPECT_EQ(expected, actual);
}

TEST(TestLua53_buildInitLuaCode, _01_scene_with_gObjects_and_cmp) {
    GameObject::sNextNumID = 0;

    std::vector<GameObject *> goList = {
            GameObject::build("Player"),
            GameObject::build("Bot 1"),
    };
    goList[0]->mComponents.push_back(
            Component{
                    "CharacterController",
                    "Scripts/CharacterController",
                    {
                            Prop{"camera", "allGameObjects['Main camera'].transform"},
                            Prop{"runningSpeed", "2.0"}
                    }
            });

    Lua53 target;
    const auto &actual = target.buildInitLuaCode(goList);

    std::string expected = ""
                           "require 'Core'\n"
                           "\n"
                           "allGameObjects['go 0 :: Player'] = GameObject:new('Player')\n"
                           "allGameObjects['go 0 :: Player'].transform.position:Set(0.0, 0.0, 0.0)\n"
                           "allGameObjects['go 0 :: Player'].transform.rotation:Set(0.0, 0.0, 0.0, 0.0)\n"
                           "\n"
                           "allGameObjects['go 1 :: Bot 1'] = GameObject:new('Bot 1')\n"
                           "allGameObjects['go 1 :: Bot 1'].transform.position:Set(0.0, 0.0, 0.0)\n"
                           "allGameObjects['go 1 :: Bot 1'].transform.rotation:Set(0.0, 0.0, 0.0, 0.0)\n"
                           "\n"
                           "require '/Scripts/CharacterController'\n"
                           "\n"
                           "local cmp = CharacterController\n"
                           "cmp.gameObject = allGameObjects['go 0 :: Player']\n"
                           "cmp.transform  = allGameObjects['go 0 :: Player'].transform\n"
                           "allComponents['go 0 :: Player :: CharacterController'] = cmp\n"
                           "\n"
                           "allComponents['go 0 :: Player :: CharacterController'].camera = allGameObjects['Main camera'].transform\n"
                           "allComponents['go 0 :: Player :: CharacterController'].runningSpeed = 2.0\n"
                           "\n"
                           "for cmpName, cmpInstance in pairs(allComponents) do\n"
                           "    cmpInstance:Start()\n"
                           "end\n";

    EXPECT_EQ(expected, actual);
}
