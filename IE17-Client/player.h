#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <windows.h>

extern unsigned __int64 localplayer;

void getPlayer(char* Buffer, __int64 adr1);
void GetPlayerPosition();
void MonitorLevel();

#endif // PLAYER_H
