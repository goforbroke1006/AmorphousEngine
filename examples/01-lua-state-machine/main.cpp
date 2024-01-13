//
// Created by goforbroke on 12/29/23.
//

#include <cstdlib>  // EXIT_SUCCESS
#include <iostream> // std::cout std::cerr

#include <LuaCpp.hpp>

int main() {
    LuaCpp::LuaContext ctx;
    const std::unique_ptr<LuaCpp::Engine::LuaState> &L = ctx.newState();

    LuaCpp::Engine::LuaTTable allGameObjectsTbl;
    LuaCpp::Engine::LuaTTable allComponentsTbl;

    allGameObjectsTbl.PushGlobal(*L, "__all_game_objects");
    allComponentsTbl.PushGlobal(*L, "__all_components");

    try {

        // 1) Include LUA modules from 'Core'
        // 2) Create all game objects (set name, set transform)
        // 3) Create all components (set gameObject, set transform)
        // 4) Set each component with args (static values, references to gameObject or transforms)
        // 5) Run 'Start()' method for each component

        luaL_loadstring(*L,

                        "require 'Core' \n"
                        ""
                        "__all_game_objects['Cube-1'] = GameObject:new('Cube-1') \n"
                        "__all_game_objects['Cube-1'].transform.position:Set(0.0, 0.0, 0.0) \n"
                        "__all_game_objects['Cube-1'].transform.rotation:Set(0.0, 0.0, 0.0, 0.0) \n"
                        ""
                        "__all_game_objects['Drone-1'] = GameObject:new('Drone-1') \n"
                        "__all_game_objects['Drone-1'].transform.position:Set(50.0, 50.0, 50.0) \n"
                        "__all_game_objects['Drone-1'].transform.rotation:Set(0.0, 90.0, 0.0, 0.0) \n"
                        ""
                        "require 'Scripts/DroneController' \n"
                        ""
                        "__all_components['Drone-1-DroneController'] = DroneController \n"
                        "__all_components['Drone-1-DroneController'].gameObject = __all_game_objects['Drone-1'] \n"
                        "__all_components['Drone-1-DroneController'].transform = __all_game_objects['Drone-1'].transform \n"
                        ""
                        "__all_components['Drone-1-DroneController'].motionSpeed = 1.0 \n"
                        "__all_components['Drone-1-DroneController'].targetTr = __all_game_objects['Cube-1'].transform \n"
                        ""
                        "for cmpName, cmpInstance in pairs(__all_components) do \n"
                        "    cmpInstance:Start() \n"
                        "end \n"
        );

        int res = lua_pcall(*L, 0, 0, 0);
        if (res != LUA_OK) {
            std::cerr << "ERROR: " << res << " " << lua_tostring(*L, 1) << std::endl
                      << lua_error(*L) << std::endl;
            L->PrintStack(std::cout);
        }

        allGameObjectsTbl.PopGlobal(*L);
        allComponentsTbl.PopGlobal(*L);

        std::cout << "Scene initialization finished..." << std::endl;
        std::cout << "Game objects: " << allGameObjectsTbl.getValues().size() << std::endl;
        std::cout << "Components:   " << allComponentsTbl.getValues().size() << std::endl;

        //
        //
        //

        for (int idx = 0; idx < 5; ++idx) {
            luaL_loadstring(
                    *L,

                    "for cmpName, cmpInstance in pairs(__all_components) do \n"
                    "     cmpInstance:Update() \n"
                    "end \n"
            );

            int res = lua_pcall(*L, 0, 0, 0);
            if (res != LUA_OK) {
                std::cerr << "ERROR: " << res << " " << lua_tostring(*L, 1) << std::endl
                          << lua_error(*L) << std::endl;
                L->PrintStack(std::cout);

                continue;
            }

            allGameObjectsTbl.PopGlobal(*L);
            allComponentsTbl.PopGlobal(*L);

            // extract transforms

            auto &go = (LuaCpp::Engine::LuaTTable &) allGameObjectsTbl.getValue(LuaCpp::Engine::Table::Key("Drone-1"));
            auto &transform = (LuaCpp::Engine::LuaTTable &) go.getValue(LuaCpp::Engine::Table::Key("transform"));
            auto &position = (LuaCpp::Engine::LuaTTable &) transform.getValue(LuaCpp::Engine::Table::Key("position"));

            std::cout << "  [CPP] "
                      << position.getValue(LuaCpp::Engine::Table::Key("x")).ToString() << " "
                      << position.getValue(LuaCpp::Engine::Table::Key("y")).ToString() << " "
                      << position.getValue(LuaCpp::Engine::Table::Key("z")).ToString() << " "
                      << std::endl;
        }


    } catch (const std::runtime_error &exc) {
        std::cerr << exc.what() << std::endl;
    }

    return EXIT_SUCCESS;
}