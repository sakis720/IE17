#include "main.h"
#include "enums.h"
#include "actors.h"
#include "functions.h"
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
char (*setGhostbusterHeatlhState)(unsigned __int64 bustet_object, int state);
unsigned __int64 (*Singleton_getRoom)(unsigned __int64* object);
unsigned __int64 (*setMusic)(unsigned __int64 database_entry);
unsigned __int64 (*hideHack)(unsigned __int64* buster_object);
void (*removeSlimeDecals)(unsigned __int64 buster_object);
void (*dbNarrativeStop)();
float (*dbNarrative)(unsigned __int64 database_entry);
void (*die)(unsigned __int64 object);
void (*blockHeroMovement)(unsigned __int64* buster_object, bool* state);
void (*toggleHuntMode)(unsigned __int64* object, bool* state);
void (*enableProtonTorpedo)(unsigned __int64* buster_object, bool* state);
void (*setCommandCrossBeam)(unsigned __int64 buster_object);
void (*startFakePackOverheat)(unsigned __int64* buster_object);
void (*letterbox)(bool* state);
void (*queueVideo)(const char** videoname);
void (*setMovieCaptureEnable)(bool* state);
void (*allowEnemyAttack)(bool* state);
void (*allowHeroControls)(bool* state);
void (*allowHeroDamage)(bool* state);
void (*play)(unsigned __int64 object, bool foward, int pattern, float newSpeed, float rampTime);
void (*setCameraPathActor)(unsigned __int64, unsigned __int64, float, float, float);
void (*shatter)(unsigned __int64 object, Vector WShatterPos);
void (*setSimEnable)(unsigned __int64 object, int flag);
void (*loadCheckpoint)(const char** checkpointName);
void (*setCurrentObjective)(const char** objDesc);
void (*toggleReviveMode)(unsigned __int64 buster_object, bool state);
void (*transferHeroshipTo)(unsigned __int64 buster_object, unsigned __int64 buster_object_whom);
void (*slimeMe)(unsigned __int64 buster_object, bool fromFront, float decalDuration);
void (*pretendToDrive)(unsigned __int64 buster_object, unsigned __int64 car_object, bool driverSeatFlag, bool putTrapOnRoofFlag);
void (*mountProtonPack)(unsigned __int64 buster_object, bool state);
void (*fakeFireProtonGun)(unsigned __int64 buster_object, bool state);
char (*forceDeployTrap)(unsigned __int64 buster_object, Vector WSourcePos);
void (*cacheRappel)(unsigned __int64 buster_object);
void (*setRappelModeEnable)(unsigned __int64 buster_object, bool state);
void (*startRappelSwing)(unsigned __int64 buster_object);
bool (*isDead)(unsigned __int64 object);
void (*cacheStreamingCinematAndAudio)(const char* cinematName, const char* audioFileName);
void (*stopStreamingCinemat)(const char* cinematName);
void (*cacheStreamingCinemat)(const char** cinematName);
void (*cueStreamingCinemat)(const char* cinematName, float intialCursorPos);
void (*playStreamingCinemat)(const char* cinematName);
void (*GTFO)(const char* msg, int flag);
void (*enable)(unsigned __int64, unsigned __int64*, bool); //broken
void (*setProtonBeamMaxLength)(float length);
void (*detonate)(unsigned __int64 car_object, float timer);
void (*attachToActorTag)(unsigned __int64 global_object, unsigned __int64 object, const char* tagName, bool useCurrentRelativePosition);
void (*setCurrentTeam)(unsigned __int64 object, int type);
bool (*isTrapDeployed)(unsigned __int64 buster_object);
void (*gatherAllDeployedInventoryItems)(unsigned __int64 buster_object);
void (*isPackOverheated)(unsigned __int64); //need to check if this is correct
void (*slamGoggleLocation)(unsigned __int64 buster_object, int location); //look at EGoggles
void (*setGoggleLocation)(unsigned __int64 buster_object, int location); // look at EGoggles
void (*setFacialExpression)(unsigned __int64 buster_object, int newExpression); // look at EGhostbusterFacialExpression
void (*stopControllingActor)(unsigned __int64 buster_object);
int (*warpTo)(unsigned __int64 object, Vector pos, Vector orient);
void (*fakePossession)(unsigned __int64 buster_object, bool state);
void (*setFlashlightMode)(unsigned __int64 buster_object, int newMode); //look at EFlashlightMode
void (*toggleflashlight)(unsigned __int64 buster_object, int newMode); // toggleflashlight and setFlashlightMode are diffrent functions
void (*commitSuicide)(unsigned __int64 buster_object);
void (*setHealth)(unsigned __int64, float); // need to be fixed
void (*setNothingEquipped)(unsigned __int64 buster_object, bool state);
void (*enableAllLights)(bool*);
void (*DanteVMaddExport)(const char*, const char*, int);
void (*buttonPrompt)(int buttonAction, float duration); //look at EButtonAction
void (*setAllowDamageTally)(bool* state);
void (*fade)(float opacity, float r, float g, float b, float duration);
void (*displaySplashScreen)(const char* textureName, float duration, bool stretch, bool clear);
void (*CacheEffect)(const char** effectName);
int (*StartEffect)(const char* effectName, Vector position, Vector orientation);
void (*CreateExplosion)(Vector pos, float radius, float damageStrength, float speed);
void (*SetGravity)(Vector velocity);
void (*AddLight)(Vector pos, float radius, Vector rgb, float intensity, float duration, float rampUp, float rampDown);
bool (*DisplayTextLegacy)(unsigned int messageId, const char* textDown, const char* textUp);
*/

#define REGISTER_WORLD_FUNCTION(L, name) \
    lua_pushcfunction(L, World::Lua_##name); \
    lua_setfield(L, -2, #name);

#define REGISTER_GHOSTBUSTER_FUNCTION(L, name) \
    lua_pushcfunction(L, Ghostbuster::Lua_##name); \
    lua_setfield(L, -2, #name);

#define REGISTER_PLAYER_FUNCTION(L, name) \
    lua_pushcfunction(L, Player::Lua_##name); \
    lua_setfield(L, -2, #name);

#define REGISTER_ACTOR_FUNCTION(L, name) \
    lua_pushcfunction(L, Actor::Lua_##name); \
    lua_setfield(L, -2, #name);

class World {
public:
    static int Lua_DisplayText(lua_State* L) {
        int hudType = static_cast<int>(luaL_checknumber(L, 1));
        const char* message = luaL_checkstring(L, 2);
        float duration = static_cast<float>(luaL_checknumber(L, 3));

        DisplayText(hudType, message, duration);
        return 0;
    }

    static int Lua_loadLevel(lua_State* L)
    {
        const char* levelName = luaL_checkstring(L, 1);
        loadLevel(levelName);
        return 0;
    }

    static int Lua_warpToActorSeamless(lua_State* L)
    {
		unsigned __int64 actor = static_cast<unsigned __int64>(luaL_checkinteger(L, 1));
		unsigned __int64 targetActor = static_cast<unsigned __int64>(luaL_checkinteger(L, 2));
		int result = warpToActorSeamless(actor, targetActor);
		lua_pushinteger(L, static_cast<lua_Integer>(result));
		return 1;
    }

	static int Lua_CreateActor(lua_State* L) {
		const char* className = luaL_checkstring(L, 1);
		Vector position;
		position.x = static_cast<float>(luaL_checknumber(L, 2));
		position.y = static_cast<float>(luaL_checknumber(L, 3));
		position.z = static_cast<float>(luaL_checknumber(L, 4));
		unsigned __int64 actor = CreateNewActor(className, position);
		lua_pushinteger(L, static_cast<lua_Integer>(actor));
		return 1;
	}

	static int Lua_setGravity(lua_State* L) {
		Vector gravity;
		gravity.x = static_cast<float>(luaL_checknumber(L, 1));
		gravity.y = static_cast<float>(luaL_checknumber(L, 2));
		gravity.z = static_cast<float>(luaL_checknumber(L, 3));
		SetGravity(gravity);
		return 0;
	}

    static int Lua_warpTo(lua_State* L)
    {
		unsigned __int64 actor = static_cast<unsigned __int64>(luaL_checkinteger(L, 1));
		Vector position;
		position.x = static_cast<float>(luaL_checknumber(L, 2));
		position.y = static_cast<float>(luaL_checknumber(L, 3));
		position.z = static_cast<float>(luaL_checknumber(L, 4));
		Vector orientation;
		orientation.x = static_cast<float>(luaL_checknumber(L, 5));
		orientation.y = static_cast<float>(luaL_checknumber(L, 6));
		orientation.z = static_cast<float>(luaL_checknumber(L, 7));
		int result = warpTo(actor, position, orientation);
		lua_pushinteger(L, static_cast<lua_Integer>(result));
		return 1;
    }

};

class Ghostbuster {
public:

    static int Lua_Flinch(lua_State* L) {
        unsigned __int64 actor = static_cast<unsigned __int64>(luaL_checkinteger(L, 1));
        unsigned __int64 result = flinch(actor);
        lua_pushinteger(L, static_cast<lua_Integer>(result));
        return 1;
    }

    static int Lua_knockBack(lua_State* L)
    {
        unsigned __int64 actor = static_cast<unsigned __int64>(luaL_checkinteger(L, 1));
        float impulse = static_cast<float>(luaL_checknumber(L, 2));
        GetPlayerPosition();
        knockBack(actor, playerPos, impulse);
        return 0;
    }

    static int Lua_GetGhostbustersTeam(lua_State* L) {
        // Validate that we have the expected number of ghostbusters
        constexpr int expectedGhostbusters = 4;

        // Push each Ghostbuster's ID onto the stack
        lua_pushinteger(L, static_cast<lua_Integer>(egon));
        lua_pushinteger(L, static_cast<lua_Integer>(winston));
        lua_pushinteger(L, static_cast<lua_Integer>(ray));
        lua_pushinteger(L, static_cast<lua_Integer>(venkman));

        // Return the count of values pushed onto the stack
        return expectedGhostbusters;
    }

    static int Lua_enableInventoryItem(lua_State* L)
    {
		unsigned __int64 actor = static_cast<unsigned __int64>(luaL_checkinteger(L, 1));
		int itemIndex = static_cast<int>(luaL_checknumber(L, 2));
		bool state = static_cast<bool>(lua_toboolean(L, 3));
		enableInventoryItem(actor, itemIndex, state);
		return 0;
    }

    static int Lua_readyInventoryItem(lua_State* L)
    {
		unsigned __int64 actor = static_cast<unsigned __int64>(luaL_checkinteger(L, 1));
		int itemIndex = static_cast<int>(luaL_checknumber(L, 2));
		bool state = static_cast<bool>(lua_toboolean(L, 3));
		readyInventoryItem(actor, itemIndex, state);
		return 0;
    }
};

class Player {
public:

    static int Lua_GetLocalPlayer(lua_State* L) {
        lua_pushinteger(L, static_cast<lua_Integer>(localplayer));
        return 1;
    }

    static int Lua_GetPlayerPos(lua_State* L) {
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
};

class Actor {
public:
    static int Lua_setAnimation(lua_State* L)
	{
		unsigned __int64 actor = static_cast<unsigned __int64>(luaL_checkinteger(L, 1));
		const char* animationName = luaL_checkstring(L, 2);
		setAnimation(actor, animationName, false);
		return 0;
	}

	static int Lua_cacheAnimation(lua_State* L)
	{
		const char* animationName = luaL_checkstring(L, 1);
		cacheSkeletalAnimationByName(animationName);
		return 0;
	}
};

void RegisterGameFunctions(lua_State* L) {

    lua_newtable(L);

    REGISTER_WORLD_FUNCTION(L, DisplayText);
    REGISTER_WORLD_FUNCTION(L, loadLevel);
	REGISTER_WORLD_FUNCTION(L, warpToActorSeamless);
	REGISTER_WORLD_FUNCTION(L, CreateActor);
	REGISTER_WORLD_FUNCTION(L, setGravity);
	REGISTER_WORLD_FUNCTION(L, warpTo);

    lua_setglobal(L, "world");

    lua_newtable(L);

	REGISTER_GHOSTBUSTER_FUNCTION(L, Flinch);
	REGISTER_GHOSTBUSTER_FUNCTION(L, knockBack);
	REGISTER_GHOSTBUSTER_FUNCTION(L, GetGhostbustersTeam);
	REGISTER_GHOSTBUSTER_FUNCTION(L, enableInventoryItem);
	REGISTER_GHOSTBUSTER_FUNCTION(L, readyInventoryItem);

    lua_setglobal(L, "ghostbuster");

    lua_newtable(L);

	REGISTER_PLAYER_FUNCTION(L, GetLocalPlayer);
	REGISTER_PLAYER_FUNCTION(L, GetPlayerPos);

    lua_setglobal(L, "player");

    lua_newtable(L);

	REGISTER_ACTOR_FUNCTION(L, setAnimation);
	REGISTER_ACTOR_FUNCTION(L, cacheAnimation);

    lua_setglobal(L, "actor");
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