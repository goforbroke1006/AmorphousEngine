//
// Created by goforbroke on 12/29/23.
//

#include <cstdlib> // EXIT_SUCCESS
#include <iostream>

#include <LuaCpp.hpp>

#include "Debug.h"

int main() {
    LuaCpp::LuaContext luaCtx;
    luaCtx.CompileStringAndRun("print('Hello World!!!')");

    std::shared_ptr<LuaCpp::Registry::LuaLibrary> lib = std::make_shared<LuaCpp::Registry::LuaLibrary>("Debug");
    lib->AddCFunction("Log", DebugLog);
    lib->AddCFunction("LogWarning", DebugLogWarning);
    lib->AddCFunction("LogError", DebugLogError);
    luaCtx.AddLibrary(lib);

    try {
        luaCtx.CompileStringAndRun("Debug.Log('Hello from function!!!')");
        luaCtx.CompileStringAndRun("Debug.LogWarning('Hello from function!!!')");
        luaCtx.CompileStringAndRun("Debug.LogError('Hello from function!!!')");
    } catch (const std::runtime_error &exc) {
        std::cerr << exc.what() << std::endl;
    }

    return EXIT_SUCCESS;
}