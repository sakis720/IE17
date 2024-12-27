#include "player.h"
#include "main.h"
#include "actors.h"
#include <iostream>
#include <thread>
#include <cstring>
#include <chrono>

using namespace std;

unsigned __int64 localplayer = 0; // global variable
Vector playerPos;

void getPlayer(char* Buffer, __int64 adr1) {

	std::string level = GetCurLevel(); //adding this here because MonitorLevel() is freaking out

	// added CGhostbuster in all of them because if its searchs for example 'Player' it can find a diffrent object that has the 'Player' in the name.
	// so i added CGhostbuster to make sure it finds the object that is class CGhostbuster.
    if (strstr(Buffer, "CGhostbuster Ghostbuster0") != nullptr) {
        localplayer = static_cast<unsigned __int64>(adr1);
    }
    else if (strstr(Buffer, "CGhostbuster GB_hero") != nullptr) { 
        localplayer = static_cast<unsigned __int64>(adr1);
    }
    else if (strstr(Buffer, "CGhostbuster theHero2") != nullptr) {
        localplayer = static_cast<unsigned __int64>(adr1);
    }
    else if (strstr(Buffer, "CGhostbuster player") != nullptr) {
        localplayer = static_cast<unsigned __int64>(adr1);
    }
    else if (strstr(Buffer, "CGhostbuster Player") != nullptr) {
        localplayer = static_cast<unsigned __int64>(adr1);
    }
    else if (strstr(Buffer, "CGhostbuster theHero") != nullptr) {
        localplayer = static_cast<unsigned __int64>(adr1);
    }
    else if (strstr(Buffer, "CGhostbuster Eric") != nullptr) {
        localplayer = static_cast<unsigned __int64>(adr1);
    }
    
    
}


void GetPlayerPosition() {
    if (localplayer != 0) {
        playerPos.x = *(reinterpret_cast<float*>(localplayer + 0x54)); // 0x54 offset for X
        playerPos.y = *(reinterpret_cast<float*>(localplayer + 0x58)); // 0x58 offset for Y
        playerPos.z = *(reinterpret_cast<float*>(localplayer + 0x5C)); // 0x5C offset for Z
    }
    else {
        std::cout << "Error: Failed to get player position or localplayer is null." << std::endl;
    }
}


void MonitorLevel() {
    std::string lastLevel = ""; // to track the last detected level
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::string level = GetCurLevel();
        if (level.empty()) {
            if (!lastLevel.empty()) {
            }
            lastLevel = "";
            continue; // skip further processing
        }

        if (level != lastLevel) {
            std::cout << "Detected level: " << level << std::endl;

            lastLevel = level;

            if (level.ends_with(".lvl")) {
                //getPlayer(); // call getPlayer if level ends with .lvl
            }
        }
        Sleep(10);  // Small delay to avoid high CPU usage
    }
}