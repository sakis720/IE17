#include "main.h"
#include "functions.h"
#include "enums.h"
#include "actors.h"
#include "player.h"
#include "lua_functions.h"
#include <iostream>
#include <thread>
#include <fstream>
#include <chrono>
#include <script.h>
#include <unordered_map>
#include <filesystem> 

int Lua_DisplayText(lua_State* L) {
    int hudType = static_cast<int>(luaL_checknumber(L, 1));
    const char* message = luaL_checkstring(L, 2);
    float duration = static_cast<float>(luaL_checknumber(L, 3));

    DisplayText(hudType, message, duration);
    return 0;
}

int Lua_GetLocalPlayer(lua_State* L) {
    lua_pushinteger(L, static_cast<lua_Integer>(localplayer));
    return 1;
}

int Lua_Flinch(lua_State* L) {
    unsigned __int64 actor = static_cast<unsigned __int64>(luaL_checkinteger(L, 1));
    unsigned __int64 result = flinch(actor);
    lua_pushinteger(L, static_cast<lua_Integer>(result));
    return 1;
}

int Lua_knockBack(lua_State* L)
{
	unsigned __int64 actor = static_cast<unsigned __int64>(luaL_checkinteger(L, 1));
    float impulse = static_cast<float>(luaL_checknumber(L, 2));
    GetPlayerPosition();
	knockBack(actor, playerPos, impulse);
	return 0;
}

int Lua_GetPlayerPosition(lua_State* L) {
    GetPlayerPosition();

    lua_newtable(L);  // Create table

    //add x
    lua_pushstring(L, "x");
    lua_pushnumber(L, playerPos.x);
    lua_settable(L, -3);

    //add y
    lua_pushstring(L, "y");
    lua_pushnumber(L, playerPos.y);
    lua_settable(L, -3);

    //add z
    lua_pushstring(L, "z");
    lua_pushnumber(L, playerPos.z);
    lua_settable(L, -3);

    return 1;
}

int Lua_loadLevel(lua_State* L)
{
	const char* levelName = luaL_checkstring(L, 1);
	loadLevel(levelName);
	return 0;
}

void RegisterGameFunctions(lua_State* L) {
    lua_register(L, "DisplayText", Lua_DisplayText);
    lua_register(L, "GetLocalPlayer", Lua_GetLocalPlayer);
    lua_register(L, "Flinch", Lua_Flinch);
    lua_register(L, "knockBack", Lua_knockBack);
    lua_register(L, "GetPlayerPos", Lua_GetPlayerPosition);
    lua_register(L, "loadlevel", Lua_loadLevel);
}

void CallLuaInitFunction(lua_State* L) {
    lua_getglobal(L, "init");
    if (lua_isfunction(L, -1)) {
        if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
            std::cerr << "Error in init function: " << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1);
        }
    }
    else {
        lua_pop(L, 1); // Not a function
    }
}

void OnKeyDown(char key, lua_State* L) {
    lua_getglobal(L, "keyDown");
    if (lua_isfunction(L, -1)) {
        lua_pushstring(L, std::string(1, key).c_str());
        if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
            std::cerr << "Error in keyDown: " << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1);
        }
    }
    else {
        lua_pop(L, 1); // Not a function
    }
}

void OnKeyUp(char key, lua_State* L) {
    lua_getglobal(L, "keyUp");
    if (lua_isfunction(L, -1)) {
        lua_pushstring(L, std::string(1, key).c_str());
        if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
            std::cerr << "Error in keyUp: " << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1);
        }
    }
    else {
        lua_pop(L, 1);
    }
}

void DispatchLuaKeyEvent(lua_State* L, const char* funcName, char key) {
    lua_getglobal(L, funcName);
    if (lua_isfunction(L, -1)) {
        char keyStr[2] = { key, '\0' };
        lua_pushstring(L, keyStr);
        if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
            std::cerr << "Lua error in " << funcName << ": " << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1);  // remove error message
        }
    }
    else {
        lua_pop(L, 1);  // not a function, pop nil
    }
}

void OnKeyDown(lua_State* L, char key) {
    DispatchLuaKeyEvent(L, "keyDown", key);
}

void OnKeyUp(lua_State* L, char key) {
    DispatchLuaKeyEvent(L, "keyUp", key);
}