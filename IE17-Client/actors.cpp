#include "player.h"
#include "actors.h"
#include "main.h"
#include <iostream>
#include <thread>
#include <cstring>
#include <chrono>

using namespace std;

unsigned __int64 egon = 0;
unsigned __int64 winston = 0;
unsigned __int64 venkman = 0;
unsigned __int64 ray = 0;
unsigned __int64 ecto = 0;

void getGhostbusters(char* Buffer, __int64 adr1) {

    if (strstr(Buffer, "CGhostbuster Egon") != nullptr) {
        egon = static_cast<unsigned __int64>(adr1);
    }

    if (strstr(Buffer, "CGhostbuster Winston") != nullptr) {
        winston = static_cast<unsigned __int64>(adr1);
    }

    if (strstr(Buffer, "CGhostbuster Venkman") != nullptr) {
        venkman = static_cast<unsigned __int64>(adr1);
    }
    else if (strstr(Buffer, "CGhostbuster Peter") != nullptr) {
        venkman = static_cast<unsigned __int64>(adr1);
    }

    if (strstr(Buffer, "CGhostbuster Ray") != nullptr) {
        ray = static_cast<unsigned __int64>(adr1);
    }
}

void getEcto(char* Buffer, __int64 adr1) {

    if (strstr(Buffer, "CEctoCarEffects ecto1") != nullptr) {
        ecto = static_cast<unsigned __int64>(adr1);
    }
}