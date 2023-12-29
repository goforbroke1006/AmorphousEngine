//
// Created by goforbroke on 12/29/23.
//

#include <cstdlib> // EXIT_SUCCESS
#include <iostream>

#include <LuaCpp.hpp>

extern "C" {
static int DebugLog(lua_State *L) {
    if (!lua_isstring(L, 0)) {
        lua_pushliteral(L, "incorrect argument");
        lua_error(L);
    }
    size_t len;
    const char *str = lua_tostring(L, 1);
    printf("%s", str);
    return 0;
}
}

int main() {
    LuaCpp::LuaContext luaCtx;
    luaCtx.CompileStringAndRun("print('Hello World!!!')");

//    try {
//        luaCtx.CompileStringAndRun("function print_hello() print('Hello from function') end \n"
//                   "print_hello()");
//    } catch (const std::runtime_error &exc) {
//        std::cerr << exc.what() << std::endl;
//    }

    std::shared_ptr<LuaCpp::Registry::LuaLibrary> lib = std::make_shared<LuaCpp::Registry::LuaLibrary>("Debug");
    lib->AddCFunction("Log", DebugLog);
    luaCtx.AddLibrary(lib);

    try {
        luaCtx.CompileStringAndRun("Debug.Log(\"Hello from function\")");
    } catch (const std::runtime_error &exc) {
        std::cerr << exc.what() << std::endl;
    }


//    luaCtx.Run("library.Log('Hello World!!!')");
//    luaCtx.CompileFile("print-hello", "./Scripts/print-hello.lua");

//    luaCtx.CompileString("lib", "function print_hello() \n print('Hello from function') \n end");
//    luaCtx.Run("print-hello");

//    std::shared_ptr<LuaCpp::Engine::LuaTTable> debugLogTable = std::make_shared<LuaCpp::Engine::LuaTTable>();
//    luaCtx.AddGlobalVariable("__debug_log_table", debugLogTable);
//
//    luaCtx.CompileFile("library", "./Scripts/library.lua");
////    luaCtx.CompileFile("RotatesAroundY", "./Scripts/RotatesAroundY.lua");
//
//    luaCtx.Run("DebugLog('Hello World!!!')");
////    luaCtx.Run("rotatingObj = RotatesAroundY:new()");
////    luaCtx.Run("rotatingObj.Start()");
////    luaCtx.Run("rotatingObj.Update()");
//
//    std::map<LuaCpp::Engine::Table::Key, std::shared_ptr<LuaCpp::Engine::LuaType>> logsMap = debugLogTable->getValues();
//    for (const auto &it: logsMap) {
//        std::cout << it.first.ToString() << " = " << it.second->ToString() << std::endl;
//    }

    return EXIT_SUCCESS;
}