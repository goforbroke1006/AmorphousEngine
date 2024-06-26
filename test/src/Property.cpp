//
// Created by goforbroke on 1/5/24.
//

#include <gtest/gtest.h>

#include "../../include/Core/Property.h"

namespace AmE {
    TEST(TestProperty_parseValue, _double) {
        auto actual = Property::parseValue(PropType::PropTypeDouble, "0.123");

        EXPECT_EQ(0.123, std::any_cast<double>(actual));
    }

    TEST(TestProperty_parseValue, _str) {
        auto actual = Property::parseValue(PropType::PropTypeString, "Hello World");

        EXPECT_EQ("Hello World", std::any_cast<std::string>(actual));
    }

    TEST(TestProperty_parseValue, _Color) {
        auto actual = std::any_cast<Color>(
                Property::parseValue(PropType::PropTypeColor, "0.5 0.75 0.5 1.0"));

        auto expected = Color(0.5, 0.75, 0.5, 1.0);

        EXPECT_EQ(expected.mR, actual.mR);
        EXPECT_EQ(expected.mG, actual.mG);
        EXPECT_EQ(expected.mB, actual.mB);
        EXPECT_EQ(expected.mA, actual.mA);
    }

    TEST(TestProperty_parseValue, _Vector3) {
        auto actual = std::any_cast<Vector3>(
                Property::parseValue(PropType::PropTypeVector3, "0.15 0.25 0.35"));

        auto expected = AmE::Vector3(0.15, 0.25, 0.35);

        EXPECT_EQ(expected.getX(), actual.getX());
        EXPECT_EQ(expected.getY(), actual.getY());
        EXPECT_EQ(expected.getZ(), actual.getZ());
    }

    TEST(TestProperty_parseValue, _GameObject) {
        auto actual = std::any_cast<int>(
                Property::parseValue(PropType::PropTypeGameObject, "1"));

        auto expected = 1;

        EXPECT_EQ(expected, actual);
    }
}
