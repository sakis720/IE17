#include <windows.h>
#include "include/MinHook.h"
#include <cstdio>
#include <string>
#define STR_(X) #X
#define IE17ver v0.06
#define STR(X) STR_(X)

//most simple vector
struct Vector
{
    float x;
    float y;
    float z;
};

extern bool holsterBool;

extern bool m_about;
extern bool m_legacycrash;
extern bool b_spawnactor;
extern bool g_fRestartLevel;
extern char* g_modBase;
extern bool keyholsterPressed;
extern bool wasQPressed;

extern Vector playerPos;

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
extern void (*attachToActorTag)(unsigned __int64, unsigned __int64, bool, const char*);
extern void (*setCurrentTeam)(unsigned __int64, int);
extern bool (*isTrapDeployed)(unsigned __int64);
extern void (*gatherAllDeployedInventoryItems)(unsigned __int64);
extern void (*readyInventoryItem)(unsigned __int64, int, bool);
extern void (*enableInventoryItem)(unsigned __int64, int, bool);
extern void (*isPackOverheated)(unsigned __int64);
extern void (*setGoggleLocation)(unsigned __int64, int);
extern void (*setFacialExpression)(unsigned __int64, int);
extern void (*stopControllingActor)(unsigned __int64);
extern int (*warpTo)(unsigned __int64, Vector, Vector);
extern void (*fakePossession)(unsigned __int64, bool);
extern void (*toggleflashlight)(unsigned __int64, int);
extern void (*setFlashlightMode)(unsigned __int64, int);
extern void (*commitSuicide)(unsigned __int64);
extern void (*setHealth)(unsigned __int64, float);
extern void (*setNothingEquipped)(unsigned __int64, bool);
extern void (*enableAllLights)(bool*);
extern void (*DanteVMaddExport)(const char*, const char*, int);
extern void (*loadcheckpoint)(const char*);
extern void (*buttonPrompt)(int, float);
extern void (*setAllowDamageTally)(bool*);
extern void (*fade)(float, float, float, float, float);
extern void (*displaySplashScreen)(const char*, float, bool, bool);
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
void AboutMod();
void OHKO();
void GodMode(bool state);
void ResLevel();
void TestLegacyText();
void SpawnActor();
std::string GetCurLevel();
void fadein();
void OpenShop(Vector);
void HandleShopChoice(int, Vector);
int CheckPlayerInput();
void RunMod();