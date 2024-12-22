#include "player.h"
#include <iostream>
#include "main.h"
#include <thread>
#include <chrono>

using namespace std;


// Make localplayer global by declaring it outside any function.
unsigned __int64 localplayer = 0; // Global variable

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
    else {
        std::cout << "Unknown level: " << level << std::endl;
        return 1;
    }

    localplayer = GetPlayerAddress(hProcess, baseAddress, offsets);

    if (localplayer == 0) {
        std::cout << "Failed to resolve player address." << std::endl;
    }
    else {
        std::cout << "Player Address Resolved: 0x" << std::hex << localplayer << std::endl;
    }

    return 0;
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
    }
}
