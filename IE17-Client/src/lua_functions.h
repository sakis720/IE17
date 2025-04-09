#ifndef LUA_F
#define LUA_F

#include "lua.hpp"

int Lua_DisplayText(lua_State* L);
int Lua_CreateNewActor(lua_State* L);
int Lua_GetPlayerPosition(lua_State* L);
int Lua_isKeyDown(lua_State* L);

#endif LUA_F