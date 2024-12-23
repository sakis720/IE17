#include "player.h"
#include "main.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

// Make localplayer global by declaring it outside any function.
unsigned __int64 localplayer = 0; // Global variable
Vector playerPos;

uintptr_t GetPlayerAddress(HANDLE hProcess, uintptr_t baseAddress, const std::vector<uintptr_t>& offsets) {
    SIZE_T bytesRead;
    uintptr_t currentAddress = baseAddress;

    for (const auto& offset : offsets) {
        // read the memory at the current address
        if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(currentAddress), &currentAddress, sizeof(currentAddress), &bytesRead) || bytesRead != sizeof(currentAddress)) {
            std::cerr << "Failed to read memory at address: 0x" << std::hex << currentAddress << std::endl;
            return 0; // return 0 on fail
        }
        currentAddress += offset; //add offset to get next pointer
    }

    return currentAddress; // final adr
}

int getPlayer() {
    HANDLE hProcess = GetCurrentProcess();

    // get the current level
    std::string level = GetCurLevel();
    if (level.empty() || level.find(".lvl") == std::string::npos) {
        std::cout << "Not in a valid level!" << std::endl;
        return 1;
    }

    unsigned __int64 baseAddress;
    std::vector<unsigned __int64> offsets;

    // adjust base address and offsets based on level
    // also this is not optimal to find the local player address on each level need to fix it
    if (level == "cemetery2.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x22F54A8;
        offsets = { 0x288, 0x290, 0x8, 0x8, 0x8, 0x558, 0x0 };
    }
    else if (level == "cemetery1.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x22F0A50;
        offsets = { 0x80, 0x288, 0x290, 0x8, 0x8, 0x558, 0x0 };
    }
    else if (level == "timessquare2.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x2312E78;
        offsets = { 0x868, 0x290, 0x8, 0x8, 0x8, 0x558, 0x0 };
    }
    else if (level == "hotel2.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x2315B78;
        offsets = { 0x290, 0x8, 0x8, 0x8, 0x558, 0x0 };
    }
    else if (level == "library1a.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x22E0610;
        offsets = { 0x290, 0x8, 0x8, 0x558, 0x0 };
    }
    else if (level == "library1b.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x2C85DF0;
        offsets = { 0x258, 0x28, 0x78, 0x18, 0x0 };
    }
    else if (level == "library2.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x209FFB8;
        offsets = { 0x288, 0x288, 0x290, 0x8, 0x8, 0x570, 0x0 };
    }
    else if (level == "museum1.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x2316558;
        offsets = { 0x288, 0x290, 0x8, 0x8, 0x568, 0x0 };
    }
    else if (level == "museum2.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x22FECB8;
        offsets = { 0x288, 0x290, 0x8, 0x8, 0x8, 0x558, 0x0 };
    }
    else if (level == "museum3.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x2312F08;
        offsets = { 0x288, 0x290, 0x608, 0x288, 0x290, 0x558, 0x0 };
    }
    else if (level == "hotel1a.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x2CDF448;
        offsets = { 0xCE0, 0x288, 0x290, 0x8, 0x8, 0x560, 0x0 };
    }
    else if (level == "hotel1b.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x22E38B0;
        offsets = { 0x8C0, 0x290, 0x8, 0x8, 0x8, 0x570, 0x0 };
    }
    else if (level == "timessquare1.lvl" || "timesSquare1.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x2316B90;
        offsets = { 0x980, 0x290, 0x8, 0x8, 0x8, 0x558, 0x0 };
    }
    else if (level == "timessquare1b.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x244F3B0;
        offsets = { 0xA8, 0xC8, 0x10, 0x258, 0x8, 0x80, 0x0 };
    }
    else if (level == "abyss.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x2A45E48;
        offsets = { 0x68, 0x8, 0x8, 0x8, 0x8, 0x208, 0x0 };
    }
    else if (level == "13th_floor_boss.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x2A45EA0;
        offsets = { 0x38, 0x8, 0x8, 0x8, 0x10, 0x208, 0x0 };
    }
    else if (level == "boss_sp_side.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x28A3010;
        offsets = { 0x18, 0x68, 0x10, 0x250, 0x18, 0x58, 0x0 };
    }
    else if (level == "lost_island.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x209BCC8;
        offsets = { 0x288, 0x290, 0x8, 0x8, 0x8, 0x558, 0x0 };
    }
    else if (level == "lost_island2.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x2305648;
        offsets = { 0x80, 0x290, 0x8, 0x8, 0x8, 0x558, 0x0 };
    }
    else {
        std::cout << "Unknown level: " << level << std::endl;
        return 1;
    }

    localplayer = GetPlayerAddress(hProcess, baseAddress, offsets);

    if (localplayer == 0) {
        //std::cout << "Failed to resolve player address." << std::endl;
    }
    else {
        //std::cout << "Player Address Resolved: 0x" << std::hex << localplayer << std::endl;
    }

    return 0;
}


void GetPlayerPosition() {
    localplayer = 0;
    int result = getPlayer();
    if (result == 0 && localplayer != 0) {
        playerPos.x = *(reinterpret_cast<float*>(localplayer + 0x54)); // 0x54 offset for X
        playerPos.y = *(reinterpret_cast<float*>(localplayer + 0x58)); // 0x58 offset for Y
        playerPos.z = *(reinterpret_cast<float*>(localplayer + 0x5C)); // 0x5C offset for Z

        // Optionally print the position for debugging
        std::cout << "Player Position: ("
            << playerPos.x << ", "
            << playerPos.y << ", "
            << playerPos.z << ")" << std::endl;
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
                getPlayer(); // call getPlayer if level ends with .lvl
            }
        }
        Sleep(10);  // Small delay to avoid high CPU usage
    }
}
