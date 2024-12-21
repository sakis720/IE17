#include "player.h"
#include <iostream>
#include "main.h"

using namespace std;

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
    Sleep(5000);
    HANDLE hProcess = GetCurrentProcess();

    // get the current level
    std::string level = GetCurLevel();
    if (level.empty()) {
        std::cout << "Could not determine current level!" << std::endl;
        return 1;
    }

    unsigned __int64 baseAddress;
    std::vector<unsigned __int64> offsets;

    // adjust base address and offsets based on level
    // also this is not optimal to find the local player address on each level need to fix it
    if (level == "cemetery2.lvl") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x24558D8;
        offsets = { 0xD8, 0x18, 0x20, 0x40, 0x0 };
    }
    else if (level == "lvl2") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x24558D8;
        offsets = { 0xD8, 0x18, 0x24, 0x40, 0x0 }; 
    }
    else if (level == "lvl3") {
        baseAddress = reinterpret_cast<unsigned __int64>(g_modBase) + 0x24558D8;
        offsets = { 0xD8, 0x1C, 0x20, 0x44, 0x0 };
    }
    else {
        std::cout << "Unknown level: " << level << std::endl;
        return 1;
    }

    // Resolve the player address
    unsigned __int64 localplayer = GetPlayerAddress(hProcess, baseAddress, offsets);

    if (localplayer == 0) {
        std::cout << "Failed to resolve player address." << std::endl;
    }
    else {
        std::cout << "Player Address Resolved: 0x" << std::hex << localplayer << std::endl;
    }

    return 0;
}