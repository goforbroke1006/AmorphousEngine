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
                "require 'Scripts/DroneController' \n"
                ""
                "local component = DroneController \n"
                "component:Start() \n"
                "component:Update() \n"
                "component:Update() \n"
                "component:Update() \n"
                );
    } catch (const std::runtime_error &exc) {
        std::cerr << exc.what() << std::endl;
    }

    return EXIT_SUCCESS;
}