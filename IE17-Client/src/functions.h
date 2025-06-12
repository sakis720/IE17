#ifndef FUN_H
#define FUN_H

#include <windows.h>
#include <cstdio>
#include <string>

typedef unsigned __int64(__cdecl* SINGLETON_newActor)(const char* className, Vector position);

void AboutMod();
void ResLevel();
void TextDisplayCountdown(const char* message, int seconds);
void TestLegacyText();
void fadein();
void OpenShop(Vector);
void HandleShopChoice(int, Vector);
int CheckPlayerInput();
void RunMod();
void unlockall(unsigned __int64);
void knockBackActor(unsigned __int64 actor);
void warpToPlayer(unsigned __int64 actor);
void playAnim(unsigned __int64 actor, const char* anim);
void loadLevel(const char* level);
void slimeGB(unsigned __int64 actor);
void gbLoaded();
void setObjective(const char* objective);
unsigned __int64 CreateNewActor(const char* className, Vector wPos);
unsigned __int64 getRoom(unsigned __int64* actor);
void playCinemat(const char* cinemat);
bool AreAllGhostsDefeated();
void setFOV(float fov);

#endif // FUN_H