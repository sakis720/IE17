#ifndef FUN_H
#define FUN_H

#include <windows.h>
#include <cstdio>
#include <string>

typedef unsigned __int64(__cdecl* SINGLETON_newActor)(const char* className, Vector position);

std::unordered_set<unsigned __int64> activeGhosts;

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
unsigned __int64 CreateNewActor(const char* className, Vector wPos);
bool AreAllGhostsDefeated();

#endif // FUN_H
