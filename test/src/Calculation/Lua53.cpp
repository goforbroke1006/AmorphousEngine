//
// Created by goforbroke on 12/30/23.
//

#include <gtest/gtest.h>

#include "../../../include/GameObject.h"
#include "../../../include/Calculation/Lua53.h"

TEST(TestLua53_buildInitLuaCode, _00_empty_scene) {
    const auto &actual = Lua53::buildInitLuaCode({});

    EXPECT_EQ("", actual);
}

TEST(TestLua53_buildInitLuaCode, _01_scene_with_gObjects) {
    GameObject::sNextNumID = 0;

    GameObject *playerGO = GameObject::build("Player");
    playerGO->mTransform->mPosition.Set(10.0, 20.0, 30.0);

    GameObject *bot1GO = GameObject::build("Bot 1");
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
              "allGameObjects['go 0 :: Player'] = GameObject:new('Player')\n"
              "allGameObjects['go 0 :: Player'].transform.position:Set(10.000000, 20.000000, 30.000000)\n"
              "allGameObjects['go 0 :: Player'].transform.rotation:Set(0.000000, 0.000000, 0.000000, 0.0)\n"
              "\n"
              "allGameObjects['go 1 :: Bot 1'] = GameObject:new('Bot 1')\n"
              "allGameObjects['go 1 :: Bot 1'].transform.position:Set(40.000000, 50.000000, 60.000000)\n"
              "allGameObjects['go 1 :: Bot 1'].transform.rotation:Set(0.000000, 45.000000, 0.000000, 0.0)\n"
              "\n"
              "for _, cmpInstance in pairs(allComponents) do\n"
              "    cmpInstance:Start()\n"
              "end\n";

    EXPECT_EQ(expected, actual);
}

TEST(TestLua53_buildInitLuaCode, _01_scene_with_gObjects_and_cmp) {
    GameObject::sNextNumID = 0;

    GameObject *playerGO = GameObject::build("Player");
    playerGO->mTransform->mPosition.Set(1.0, 2.0, 3.0);
    playerGO->mComponents.push_back(
            Component{
                    "CharacterController",
                    "Scripts/CharacterController",
                    {
                            Prop{"camera", "allGameObjects['Main camera'].transform"},
                            Prop{"runningSpeed", "2.0"}
                    }
            });

    GameObject *bot1GO = GameObject::build("Bot 1");
    bot1GO->mTransform->mPosition.Set(4.0, 5.0, 6.0);
    bot1GO->mTransform->mRotation.Set(0.0, 90.0, 0.0);

    std::map<std::string, GameObject *> goList = {
            {playerGO->mID, playerGO},
            {bot1GO->mID,   bot1GO},
    };

    const auto &actual = Lua53::buildInitLuaCode(goList);

    std::string expected
            = "require 'Core'\n"
              "\n"
              "allGameObjects['go 0 :: Player'] = GameObject:new('Player')\n"
              "allGameObjects['go 0 :: Player'].transform.position:Set(1.000000, 2.000000, 3.000000)\n"
              "allGameObjects['go 0 :: Player'].transform.rotation:Set(0.000000, 0.000000, 0.000000, 0.0)\n"
              "\n"
              "allGameObjects['go 1 :: Bot 1'] = GameObject:new('Bot 1')\n"
              "allGameObjects['go 1 :: Bot 1'].transform.position:Set(4.000000, 5.000000, 6.000000)\n"
              "allGameObjects['go 1 :: Bot 1'].transform.rotation:Set(0.000000, 90.000000, 0.000000, 0.0)\n"
              "\n"
              "require 'Scripts/CharacterController'\n"
              "\n"
              "local cmp = CharacterController\n"
              "cmp.gameObject = allGameObjects['go 0 :: Player']\n"
              "cmp.transform  = allGameObjects['go 0 :: Player'].transform\n"
              "allComponents['go 0 :: Player :: CharacterController'] = cmp\n"
              "\n"
              "allComponents['go 0 :: Player :: CharacterController'].camera = allGameObjects['Main camera'].transform\n"
              "allComponents['go 0 :: Player :: CharacterController'].runningSpeed = 2.0\n"
              "\n"
              "for _, cmpInstance in pairs(allComponents) do\n"
              "    cmpInstance:Start()\n"
              "end\n";

    EXPECT_EQ(expected, actual);
}
