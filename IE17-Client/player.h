#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <Windows.h>

extern unsigned __int64 localplayer;

uintptr_t GetPlayerAddress(HANDLE hProcess, uintptr_t baseAddress, const std::vector<uintptr_t>& offsets);
int getPlayer();

#endif // PLAYER_H