//
// Created by goforbroke on 1/5/24.
//

#include <gtest/gtest.h>

#include "../../include/PrefabManager.h"

#include <filesystem>
#include <fstream>

namespace AmE {
    TEST(TestPrefabManager_loadFromFile, positive) {
        ASSERT_TRUE(std::filesystem::is_regular_file("./testdata/Prefabs/Enemy-Tank1.json"));
        auto pGameObj = PrefabManager::loadFromFile("./testdata/Prefabs/Enemy-Tank1.json");

        EXPECT_EQ("Enemy Tank 1", pGameObj->getName());

        EXPECT_EQ(0.0, pGameObj->getTransform()->mPosition.getX());
        EXPECT_EQ(0.0, pGameObj->getTransform()->mPosition.getY());
        EXPECT_EQ(0.0, pGameObj->getTransform()->mPosition.getZ());

        // TODO: Do I need to check rotation?

        EXPECT_EQ(1.0, pGameObj->getTransform()->mLocalScale.getX());
        EXPECT_EQ(1.0, pGameObj->getTransform()->mLocalScale.getY());
        EXPECT_EQ(1.0, pGameObj->getTransform()->mLocalScale.getZ());

        EXPECT_EQ(2, pGameObj->getComponents().size());

        EXPECT_TRUE(nullptr != pGameObj->getComponent("Component/MeshRender"));
        EXPECT_TRUE(nullptr != pGameObj->getComponent("Scripts/EnemyController"));

        delete pGameObj;
    }
}
