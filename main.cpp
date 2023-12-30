//
// Created by goforbroke on 12/29/23.
//

#include <cstdlib> // EXIT_SUCCESS
#include <iostream>

#include <LuaCpp.hpp>

int main() {
    LuaCpp::LuaContext ctx;

    try {
        ctx.CompileStringAndRun(
                "require 'Core/Debug'\n"
                "require 'Core/Time'\n"
                "require 'Core/Space'\n"
                "require 'Core/Transform'\n"
                ""
                "require 'Scripts/RotatesAroundY' \n"
                ""
                "local instance = RotatesAroundY; \n"
                "instance:Start() \n"
                "instance:Update() \n"
                );
    } catch (const std::runtime_error &exc) {
        std::cerr << exc.what() << std::endl;
    }

    return EXIT_SUCCESS;
}