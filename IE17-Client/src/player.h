#ifndef PLAYER_H
#define PLAYER_H

extern unsigned __int64 localplayer;

void getPlayer(char* Buffer, __int64 adr1);
void GetPlayerPosition();
void GetPlayerHealth();
void GetPlayerPackValue();
void GetPlayerCollider();
void MonitorLevel();

#endif // PLAYER_H
