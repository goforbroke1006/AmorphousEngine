//
// Created by goforbroke on 12/29/23.
//

#include <cstdlib> // EXIT_SUCCESS
#include <iostream>

#include <LuaCpp.hpp>

#include "Debug.h"

int main() {
    LuaCpp::LuaContext ctx;
    ctx.CompileStringAndRun("print('Hello World!!!')");

    /*try {
        std::shared_ptr<LuaCpp::Registry::LuaLibrary> lib = std::make_shared<LuaCpp::Registry::LuaLibrary>("Debug");
        lib->AddCFunction("Log", DebugLog);
        lib->AddCFunction("LogWarning", DebugLogWarning);
        lib->AddCFunction("LogError", DebugLogError);
        ctx.AddLibrary(lib);

        ctx.CompileStringAndRun("Debug.Log('Hello from function!!!')");
        ctx.CompileStringAndRun("Debug.LogWarning('Hello from function!!!')");
        ctx.CompileStringAndRun("Debug.LogError('Hello from function!!!')");
    } catch (const std::runtime_error &exc) {
        std::cerr << exc.what() << std::endl;
    }*/

    try {
        ctx.CompileFolder("./Scripts/", "Scripts", false);
        ctx.CompileStringAndRun("require 'Scripts/Debug' \n"
                                "Debug.Log('Gotcha!')");
    } catch (const std::runtime_error &exc) {
        std::cerr << exc.what() << std::endl;
    }

    return EXIT_SUCCESS;
}