#include <windows.h>
#include "../include/MinHook.h"
#include <cstdio>
#include <string>
#define STR_(X) #X
#define IE17ver v0.07
#define STR(X) STR_(X)

//most simple vector
struct Vector
{
    float x;
    float y;
    float z;
};

/*
struct SSpawnInfo {
    string classTypeName;
	string citFilename;
    string variantName;
};
*/

extern bool holsterBool;

extern bool m_about;
extern bool m_legacycrash;
extern bool b_spawnactor;
extern bool g_fRestartLevel;
extern char* g_modBase;
extern bool keyholsterPressed;
extern bool wasQPressed;

extern int playerCash;
extern Vector playerPos;

extern void (*setSimEnable)(unsigned __int64, int);
extern void (*loadCheckpoint)(const char**);
extern void (*setCurrentObjective)(const char**);
extern void (*toggleReviveMode)(unsigned __int64, bool);
extern int (*chainToLevel)(unsigned __int64, const char*, const char*);
extern void (*transferHeroshipTo)(unsigned __int64, unsigned __int64);
extern void (*slimeMe)(unsigned __int64, bool, float);
extern void (*knockBack)(unsigned __int64, Vector, float);
extern void (*pretendToDrive)(unsigned __int64, unsigned __int64, bool, bool);
extern void (*mountProtonPack)(unsigned __int64, bool);
extern void (*fakeFireProtonGun)(unsigned __int64, bool);
extern char (*forceDeployTrap)(unsigned __int64, Vector);
extern void (*cacheRappel)(unsigned __int64);
extern void (*setRappelModeEnable)(unsigned __int64, bool);
extern void (*startRappelSwing)(unsigned __int64);
extern bool (*isDead)(unsigned __int64);
extern void (*cacheStreamingCinematAndAudio)(const char*, const char*);
extern void (*stopStreamingCinemat)(const char*);
extern void (*cacheStreamingCinemat)(const char**);
extern void (*cueStreamingCinemat)(const char*, float);
extern void (*playStreamingCinemat)(const char*);
extern void (*GTFO)(const char*, int);
extern void (*cacheSkeletalAnimationByName)(const char*);
extern void (*enable)(unsigned __int64, bool, bool);
extern void (*setProtonBeamMaxLength)(float);
extern void (*setAnimation)(unsigned __int64, const char*, bool, bool);
extern void (*detonate)(unsigned __int64, float);
extern void (*attachToActorTag)(unsigned __int64, unsigned __int64, const char*, bool);
extern void (*setCurrentTeam)(unsigned __int64, int);
extern bool (*isTrapDeployed)(unsigned __int64);
extern void (*gatherAllDeployedInventoryItems)(unsigned __int64);
extern void (*readyInventoryItem)(unsigned __int64, int, bool);
extern void (*enableInventoryItem)(unsigned __int64, int, bool);
extern void (*isPackOverheated)(unsigned __int64);
extern void (*slamGoggleLocation)(unsigned __int64, int);
extern void (*setGoggleLocation)(unsigned __int64, int);
extern void (*setFacialExpression)(unsigned __int64, int);
extern void (*stopControllingActor)(unsigned __int64);
extern int (*warpTo)(unsigned __int64, Vector, Vector);
extern int (*warpToActorSeamless)(unsigned __int64, unsigned __int64);
extern void (*fakePossession)(unsigned __int64, bool);
extern void (*toggleflashlight)(unsigned __int64, int);
extern void (*setFlashlightMode)(unsigned __int64, int);
extern void (*commitSuicide)(unsigned __int64);
extern void (*setHealth)(unsigned __int64, float);
extern void (*setNothingEquipped)(unsigned __int64, bool);
extern void (*enableAllLights)(bool*);
extern void (*DanteVMaddExport)(const char*, const char*, int);
extern void (*buttonPrompt)(int, float);
extern void (*setAllowDamageTally)(bool*);
extern void (*fade)(float, float, float, float, float);
extern void (*displaySplashScreen)(const char*, float, bool, bool);
extern void (*CreateActor)(const char*, Vector);
extern void (*CacheEffect)(const char**);
extern int (*StartEffect)(const char*, Vector, Vector);
extern void (*CreateExplosion)(Vector, float, float, float);
extern void (*SetGravity)(Vector);
extern void (*AddLight)(Vector, float, Vector, float, float, float, float);
extern int (*DisplayText)(int, const char*, float);
extern int (*DisplayTextLegacy)(int, const char*, const char*, char);

DWORD WINAPI DLLAttach(HMODULE hModule);
void __stdcall HookedFunction(char* Buffer, __int64 adr1, __int64 adr2, __int64 adr3);
void HandleInput();
std::string GetCurLevel();