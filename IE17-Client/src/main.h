#include <iostream>
#include "../MinHook/MinHook.h"

#include "lua.hpp"

#define STR_(X) #X
#define IE17ver v0.09
#define STR(X) STR_(X)


//most simple vector
struct Vector
{
    float x;
    float y;
    float z;
};

extern bool holsterBool;

extern bool showWindow;

extern bool m_about;
extern bool m_legacycrash;
extern bool b_spawnactor;
extern bool g_fRestartLevel;
extern char* g_modBase;
extern bool keyholsterPressed;
extern bool wasQPressed;

extern int playerCash;
extern Vector playerPos;

extern unsigned __int64 (*flinch)(unsigned __int64 buster_object);
extern unsigned __int64 (*Singleton_getRoom)(unsigned __int64* object);
extern unsigned __int64 (*setMusic)(unsigned __int64 database_entry);
extern unsigned __int64 (*hideHack)(unsigned __int64* buster_object);
extern void (*removeSlimeDecals)(unsigned __int64 buster_object);
extern void (*dbNarrativeStop)();
extern float (*dbNarrative)(unsigned __int64 database_entry);
extern void (*die)(unsigned __int64 object);
extern void (*blockHeroMovement)(unsigned __int64* buster_object, bool* state);
extern void (*toggleHuntMode)(unsigned __int64* object, bool* state);
extern void (*enableProtonTorpedo)(unsigned __int64* buster_object, bool* state);
extern void (*setCommandCrossBeam)(unsigned __int64 buster_object);
extern void (*startFakePackOverheat)(unsigned __int64* buster_object);
extern void (*letterbox)(bool* state);
extern void (*queueVideo)(const char** videoname);
extern void (*setMovieCaptureEnable)(bool* state);
extern void (*allowEnemyAttack)(bool* state);
extern void (*allowHeroControls)(bool* state);
extern void (*allowHeroDamage)(bool* state);
extern void (*play)(unsigned __int64 object, bool foward, int pattern, float newSpeed, float rampTime);
extern void (*setCameraPathActor)(unsigned __int64, unsigned __int64, float, float, float);
extern void (*shatter)(unsigned __int64 object, Vector WShatterPos);
extern void (*setSimEnable)(unsigned __int64 object, int flag);
extern void (*loadCheckpoint)(const char** checkpointName);
extern void (*setCurrentObjective)(const char** objDesc);
extern void (*toggleReviveMode)(unsigned __int64 buster_object, bool state);
extern int (*chainToLevel)(unsigned __int64 buster_object, const char* levelName, const char* lvlCheckpoint);
extern void (*transferHeroshipTo)(unsigned __int64 buster_object, unsigned __int64 buster_object_whom);
extern void (*slimeMe)(unsigned __int64 buster_object, bool fromFront, float decalDuration);
extern void (*knockBack)(unsigned __int64 buster_object, Vector WSourcePos, float impulse);
extern void (*pretendToDrive)(unsigned __int64 buster_object, unsigned __int64 car_object, bool driverSeatFlag, bool putTrapOnRoofFlag);
extern void (*mountProtonPack)(unsigned __int64 buster_object, bool state);
extern void (*fakeFireProtonGun)(unsigned __int64 buster_object, bool state);
extern char (*forceDeployTrap)(unsigned __int64 buster_object, Vector WSourcePos);
extern void (*cacheRappel)(unsigned __int64 buster_object);
extern void (*setRappelModeEnable)(unsigned __int64 buster_object, bool state);
extern void (*startRappelSwing)(unsigned __int64 buster_object);
extern bool (*isDead)(unsigned __int64 object);
extern void (*cacheStreamingCinematAndAudio)(const char* cinematName, const char* audioFileName);
extern void (*stopStreamingCinemat)(const char* cinematName);
extern void (*cacheStreamingCinemat)(const char** cinematName);
extern void (*cueStreamingCinemat)(const char* cinematName, float intialCursorPos);
extern void (*playStreamingCinemat)(const char* cinematName);
extern void (*GTFO)(const char* msg, int flag);
extern void (*cacheSkeletalAnimationByName)(const char* animationName);
extern int (*enable)(unsigned __int64, unsigned __int64, bool);
extern void (*setProtonBeamMaxLength)(float length);
extern void (*setAnimation)(unsigned __int64 object, const char* animationName, bool useSkelFileExit);
extern void (*detonate)(unsigned __int64 car_object, float timer);
extern void (*attachToActorTag)(unsigned __int64 global_object, unsigned __int64 object, const char* tagName, bool useCurrentRelativePosition);
extern void (*setCurrentTeam)(unsigned __int64 object, int type);
extern bool (*isTrapDeployed)(unsigned __int64 buster_object);
extern void (*gatherAllDeployedInventoryItems)(unsigned __int64 buster_object);
extern void (*readyInventoryItem)(unsigned __int64 buster_object, int itemToSwitchTo, bool state);
extern void (*enableInventoryItem)(unsigned __int64 buster_object, int itemToSwitchTo, bool state);
extern void (*isPackOverheated)(unsigned __int64);
extern void (*slamGoggleLocation)(unsigned __int64 buster_object, int location);
extern void (*setGoggleLocation)(unsigned __int64 buster_object, int location);
extern void (*setFacialExpression)(unsigned __int64 buster_object, int newExpression);
extern void (*stopControllingActor)(unsigned __int64 buster_object);
extern int (*warpTo)(unsigned __int64 object, Vector pos, Vector orient);
extern int (*warpToActorSeamless)(unsigned __int64 object, unsigned __int64 global_object);
extern void (*fakePossession)(unsigned __int64 buster_object, bool state);
extern void (*setFlashlightMode)(unsigned __int64 buster_object, int newMode);
extern void (*toggleflashlight)(unsigned __int64 buster_object, int newMode);
extern void (*commitSuicide)(unsigned __int64 buster_object);
extern void (*setHealth)(unsigned __int64, float);
extern void (*setNothingEquipped)(unsigned __int64 buster_object, bool state);
extern void (*enableAllLights)(bool*);
extern void (*DanteVMaddExport)(const char*, const char*, int);
extern void (*buttonPrompt)(int buttonAction, float duration);
extern void (*setAllowDamageTally)(bool* state);
extern void (*fade)(float opacity, float r, float g, float b, float duration);
extern void (*displaySplashScreen)(const char* textureName, float duration, bool stretch, bool clear);
extern void (*CacheEffect)(const char** effectName);
extern int (*StartEffect)(const char* effectName, Vector position, Vector orientation);
extern void (*CreateExplosion)(Vector pos, float radius, float damageStrength, float speed);
extern void (*SetGravity)(Vector velocity);
extern void (*AddLight)(Vector pos, float radius, Vector rgb, float intensity, float duration, float rampUp, float rampDown);
extern void (*CreateActor)(const char* className, Vector wPos);
extern unsigned __int64 (*Singleton_newActor)(const char* className, Vector wPos);
extern int (*DisplayText)(int messageId, const char* text, float duration);
extern bool (*DisplayTextLegacy)(unsigned int messageId, const char* textDown, const char* textUp);


DWORD WINAPI DLLAttach(HMODULE hModule);
void __stdcall HookedFunction(char* Buffer, __int64 adr1, __int64 adr2, __int64 adr3);
void HandleInput();
void LoadAllLuaScripts();
void ReloadAllLuaScripts();
std::string GetCurLevel();