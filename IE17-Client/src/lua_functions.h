#ifndef LUA_F
#define LUA_F

#include "lua.hpp"

extern void RegisterGameFunctions(lua_State* L);
extern void DispatchLuaKeyEvent(lua_State* L, const char* funcName, char key);
extern void OnKeyDown(lua_State* L, char key);
extern void OnKeyUp(lua_State* L, char key);
extern void CallLuaInitFunction(lua_State* L);

#endif LUA_F