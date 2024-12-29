#ifndef FUN_H
#define FUN_H

#include <windows.h>
#include <cstdio>
#include <string>

void AboutMod();
void ResLevel();
void SpawnActor();
void TextDisplayCountdown(const char* message, int seconds);
void TestLegacyText();
void fadein();
void OpenShop(Vector);
void HandleShopChoice(int, Vector);
int CheckPlayerInput();
void RunMod();

#endif // FUN_H
