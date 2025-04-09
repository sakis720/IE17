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

/*
 * Windows key code
 * https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
 */
std::unordered_map<std::string, int> keyMap = {  //windows virtual key codes | temporary
       {"A", 0x41},
        {"B", 0x42},
        {"C", 0x43},
        {"D", 0x44},
        {"E", 0x45},
        {"F", 0x46},
        {"G", 0x47},
        {"H", 0x48},
        {"I", 0x49},
        {"J", 0x4A},
        {"K", 0x4B},
        {"L", 0x4C},
        {"M", 0x4D},
        {"N", 0x4E},
        {"O", 0x4F},
        {"P", 0x50},
        {"Q", 0x51},
        {"R", 0x52},
        {"S", 0x53},
        {"T", 0x54},
        {"U", 0x55},
        {"V", 0x56},
        {"W", 0x57},
        {"X", 0x58},
        {"Y", 0x59},
        {"Z", 0x5A},

        {"0", 0x30},
        {"1", 0x31},
        {"2", 0x32},
        {"3", 0x33},
        {"4", 0x34},
        {"5", 0x35},
        {"6", 0x36},
        {"7", 0x37},
        {"8", 0x38},
        {"9", 0x39},

        {"Minus",        VK_OEM_MINUS},
        {"Equal",        VK_OEM_PLUS},
        {"RightBracket", VK_OEM_6},
        {"LeftBracket",  VK_OEM_4},
        {"Quote",        VK_OEM_7},
        {"Semicolon",    VK_OEM_1},

        {"Return",       VK_RETURN   },
        {"Tab",          VK_TAB      },
        {"Space",        VK_SPACE    },
        {"Delete",       VK_BACK     },
        {"Escape",       VK_ESCAPE   },
        {"Command",      VK_LMENU    },   // left ALT
        {"Shift",        VK_LSHIFT   },
        {"CapsLock",     VK_CAPITAL  },
        {"Option",       VK_MENU     },
        {"Control",      VK_CONTROL  },
        {"RightCommand", VK_RWIN     },
        {"RightShift",   VK_RSHIFT   },

        {"Home",          VK_HOME    },
        {"PageUp",        VK_PRIOR   },
        {"PageDown",      VK_NEXT    },
        {"ForwardDelete", VK_DELETE  },
        {"End",           VK_END     },
        {"LeftArrow",     VK_LEFT    },
        {"RightArrow",    VK_RIGHT   },
        {"DownArrow",     VK_DOWN    },
        {"UpArrow",       VK_UP      },

        {"F1",  VK_F1},
        {"F2",  VK_F2},
        {"F3",  VK_F3},
        {"F4",  VK_F4},
        {"F5",  VK_F5},
        {"F6",  VK_F6},
        {"F7",  VK_F7},
        {"F8",  VK_F8},
        {"F9",  VK_F9},
        {"F10", VK_F10},
        {"F11", VK_F11},
        {"F12", VK_F12},
        {"F13", VK_F13},
        {"F14", VK_F14},
        {"F15", VK_F15},
        {"F16", VK_F16},
        {"F17", VK_F17},
        {"F18", VK_F18},
        {"F19", VK_F19},
        {"F20", VK_F20},
};

std::unordered_map<std::string, bool> keyState; 

int Lua_isKeyDown(lua_State* L) {
    const char* keyName = luaL_checkstring(L, 1);
    auto it = keyMap.find(keyName);
    if (it == keyMap.end()) {
        lua_pushboolean(L, false);
        return 1;
    }

    int keyCode = it->second;
    short keyStateCurrent = GetAsyncKeyState(keyCode);

    bool isDown = (keyStateCurrent & 0x8000) != 0;
    bool wasDown = keyState[keyName];  //retrieve previous state of the key

    if (isDown && !wasDown) {
        //key just pressed
        keyState[keyName] = true;
        lua_pushboolean(L, true);
    }
    else if (!isDown && wasDown) {
        //key just released
        keyState[keyName] = false;  //update the key state to "up"
        lua_pushboolean(L, false); 
    }
    else {
        lua_pushboolean(L, false);
    }

    return 1;
}

int Lua_DisplayText(lua_State* L)
{

    lua_Integer luaInt = luaL_checkinteger(L, 1);
    if (luaInt > INT_MAX || luaInt < INT_MIN) {
        lua_pushstring(L, "Message ID is out of range for int");
        lua_error(L);
        return 0;
    }
    int messageId = static_cast<int>(luaInt);

    const char* text = luaL_checkstring(L, 2);
    lua_Number luaNum = luaL_checknumber(L, 3);
    float duration = static_cast<float>(luaNum); 

    // call the C++ function
    int result = DisplayText(messageId, text, duration);

    // push the result back to Lua
    lua_pushinteger(L, result);
    return 1;
}

int Lua_CreateNewActor(lua_State* L)
{
    const char* className = luaL_checkstring(L, 1);

    Vector wPos;
    wPos.x = static_cast<float>(luaL_checknumber(L, 2));
    wPos.y = static_cast<float>(luaL_checknumber(L, 3));
    wPos.z = static_cast<float>(luaL_checknumber(L, 4));

    unsigned __int64 result = CreateNewActor(className, wPos);

    lua_pushinteger(L, result);
    return 1;
}

int Lua_GetPlayerPosition(lua_State* L) {
    //update player pos
    GetPlayerPosition();

    // push the player position as a lua table onto the stack
    lua_newtable(L); 

    lua_pushstring(L, "x");
    lua_pushnumber(L, playerPos.x);  // push the x position
    lua_settable(L, -3);

    lua_pushstring(L, "y");
    lua_pushnumber(L, playerPos.y);  // push the y position
    lua_settable(L, -3);

    lua_pushstring(L, "z");
    lua_pushnumber(L, playerPos.z);  // push the z position
    lua_settable(L, -3);

    return 1; 
}