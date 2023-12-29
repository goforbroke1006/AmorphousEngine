//
// Created by goforbroke on 12/29/23.
//

#ifndef OGRITY_DEBUG_H
#define OGRITY_DEBUG_H

#include <cstdio> // printf

#include <lua.h> // lua_isstring lua_tostring

extern "C" {

static int DebugLog(lua_State *L) {
    if (!lua_isstring(L, 0)) {
        lua_pushliteral(L, "incorrect argument");
        lua_error(L);
    }
    const char *str = lua_tostring(L, 1);
    printf("DEBUG: %s\n", str);
    return 0;
}

static int DebugLogWarning(lua_State *L) {
    if (!lua_isstring(L, 0)) {
        lua_pushliteral(L, "incorrect argument");
        lua_error(L);
    }
    const char *str = lua_tostring(L, 1);
    printf("WARN:  %s\n", str);
    return 0;
}

static int DebugLogError(lua_State *L) {
    if (!lua_isstring(L, 0)) {
        lua_pushliteral(L, "incorrect argument");
        lua_error(L);
    }
    const char *str = lua_tostring(L, 1);
    printf("ERROR: %s\n", str);
    return 0;
}

}

#endif //OGRITY_DEBUG_H
