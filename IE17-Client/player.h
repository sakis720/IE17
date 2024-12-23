#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <windows.h>

extern unsigned __int64 localplayer;

uintptr_t GetPlayerAddress(HANDLE hProcess, uintptr_t baseAddress, const std::vector<uintptr_t>& offsets);
int getPlayer();
void GetPlayerPosition();
void MonitorLevel();

#endif // PLAYER_H
