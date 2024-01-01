//
// Created by goforbroke on 12/29/23.
//

#include <cstdlib> // EXIT_SUCCESS
#include <iostream> // std::cout std::cerr

#include <LuaCpp.hpp>

int main() {
    LuaCpp::LuaContext ctx;

    try {
        LuaCpp::Engine::LuaTTable allGameObjectsTbl;
        LuaCpp::Engine::LuaTTable allComponentsTbl;

        {
            ctx.CompileString("scene_initialization",

                              "allGameObjects = {} \n"
                              "allComponents = {} \n"
                              ""
                              "require 'Core/GameObject' \n"
                              ""
                              "local cube1 = GameObject:new() \n"
                              "cube1.transform.position:Set(0.0, 0.0, 0.0) \n"
                              "allGameObjects['Cube-1'] = cube1 \n"
                              ""
                              "local drone1 = GameObject:new() \n"
                              "allGameObjects['Drone-1'] = drone1 \n"
                              ""
                              "require 'Scripts/DroneController' \n"
                              ""
                              "local drone1component1 = DroneController \n"
                              "drone1component1.gameObject = drone1 \n"
                              "allComponents['Drone-1-DroneController'] = drone1component1 \n"
                              ""
                              "drone1component1.motionSpeed = 1.0 \n"
                              "drone1component1.targetObj = allGameObjects['Cube-1'] \n"
                              ""
                              "for _, cmp in ipairs(allComponents) do \n"
                              "    cmp:Start() \n"
                              "end \n"
            );
            const std::unique_ptr<LuaCpp::Engine::LuaState> &L =
                    ctx.newStateFor("scene_initialization");

            allGameObjectsTbl.PushGlobal(*L, "allGameObjects");
            allComponentsTbl.PushGlobal(*L, "allComponents");

            int res = lua_pcall(*L, 0, 0, 0);
            if (res != LUA_OK) {
                std::cerr << "ERROR: " << res << " " << lua_tostring(*L, 1) << std::endl
                          << lua_error(*L) << std::endl;
                L->PrintStack(std::cout);
            }

            allGameObjectsTbl.PopGlobal(*L);
            allComponentsTbl.PopGlobal(*L);
        }

        std::cout << "Scene initialization finished..." << std::endl;
        std::cout << "Game objects: " << allGameObjectsTbl.getValues().size() << std::endl;
        std::cout << "Components:   " << allComponentsTbl.getValues().size() << std::endl;

        //
        //
        //

        ctx.CompileString("on_frame_updated",

                          "function tablelength(T)\n"
                          "  local count = 0\n"
                          "  for _ in pairs(T) do count = count + 1 end\n"
                          "  return count\n"
                          "end \n"
                          ""
                          "print('[LUA] Game objects: ' .. tablelength(allGameObjects) ) \n"
                          "print('[LUA] Components:   ' .. tablelength(allComponents) ) \n"
                          ""
                          "for _, cmp in ipairs(allComponents) do \n"
                          "    cmp:Start() \n"
                          "end \n"
        );

        for (int idx = 0; idx < 5; ++idx) {
            const std::unique_ptr<LuaCpp::Engine::LuaState> &L =
                    ctx.newStateFor("on_frame_updated");

            allGameObjectsTbl.PushGlobal(*L, "allGameObjects");
            allComponentsTbl.PushGlobal(*L, "allComponents");


            int res = lua_pcall(*L, 0, 0, 0);
            if (res != LUA_OK) {
                std::cerr << "ERROR: " << res << " " << lua_tostring(*L, 1) << std::endl
                          << lua_error(*L) << std::endl;
                L->PrintStack(std::cout);

                continue;
            }

            allGameObjectsTbl.PopGlobal(*L);
            allComponentsTbl.PopGlobal(*L);
        }


    } catch (const std::runtime_error &exc) {
        std::cerr << exc.what() << std::endl;
    }

    return EXIT_SUCCESS;
}