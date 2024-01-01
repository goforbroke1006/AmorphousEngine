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

    allGameObjectsTbl.PushGlobal(*L, "allGameObjects");
    allComponentsTbl.PushGlobal(*L, "allComponents");

    try {

        // 1) Include LUA modules from 'Core'
        // 2) Create all game objects (set name, set transform)
        // 3) Create all components (set gameObject, set transform)
        // 4) Set each component with args (static values, references to gameObject or transforms)
        // 5) Run 'Start()' method for each component


        luaL_loadstring(*L,

                        "require 'Core' \n"
                        ""
                        "local cube1 = GameObject:new('Cube-1') \n"
                        "cube1.transform.position:Set(0.0, 0.0, 0.0) \n"
                        "cube1.transform.rotation:Set(0.0, 0.0, 0.0, 0.0) \n"
                        "allGameObjects['Cube-1'] = cube1 \n"
                        ""
                        "local drone1 = GameObject:new('Drone-1') \n"
                        "drone1.transform.position:Set(50.0, 50.0, 50.0) \n"
                        "drone1.transform.rotation:Set(0.0, 90.0, 0.0, 0.0) \n"
                        "allGameObjects['Drone-1'] = drone1 \n"
                        ""
                        "require 'Scripts/DroneController' \n"
                        ""
                        "local drone1component1 = DroneController \n"
                        "drone1component1.gameObject = drone1 \n"
                        "drone1component1.transform = drone1.transform \n"
                        "allComponents['Drone-1-DroneController'] = drone1component1 \n"
                        ""
                        "drone1component1.motionSpeed = 1.0 \n"
                        "drone1component1.targetTr = allGameObjects['Cube-1'].transform \n"
                        ""
                        "for cmpName, cmpInstance in pairs(allComponents) do \n"
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
            luaL_loadstring(*L,

                            "for cmpName, cmpInstance in pairs(allComponents) do \n"
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