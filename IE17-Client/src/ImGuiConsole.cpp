#include "main.h"
#include "player.h"
#include "enums.h"
#include "actors.h"
#include "script.h"
#include "functions.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include <thread>
#include <cstdint> 
#include <filesystem> 
#include <cstring>
#include <string>
#include <algorithm>
#include <windows.h>
#include <conio.h>


bool showWindow = false;
bool showColumnWindow = false;

// Console data
std::vector<std::string> consoleLogs;
std::string currentInput;

void Log(const std::string& message) {
    consoleLogs.push_back(message);
    if (consoleLogs.size() > 1000) {
        consoleLogs.erase(consoleLogs.begin()); // Remove oldest log to prevent overflow
    }
}

void LogFormatted(const char* format, ...) {
    static char buffer[512]; // Temporary buffer for formatted string
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    consoleLogs.push_back(std::string(buffer));
    if (consoleLogs.size() > 1000) {
        consoleLogs.erase(consoleLogs.begin()); // Prevent overflow
    }
}

void HandleCommand(const std::string& command) {
    if (command == "help") {
        Log("Available commands:");
        Log("  slew                  - Toggle Slew (Noclip)");
        Log("  quitLevel             - Quit the current level");
        Log("  animdebug             - Enable animation debug overlay");
        Log("  cinematdebug          - Enable cinematics debug overlay");
        Log("  channeldebug          - Enable channel debug overlay");
        Log("  cancelwalk            - Disable walk animation for all AI");
        Log("  ghostviewer           - Toggle Ghost Viewer");
        Log("  checkversion          - Current IE17 version");
        Log("  about                 - Show about information");
        Log("  restart               - Restart the level");
        Log("  gbloaded              - Check Ghostbuster loading status");
        Log("  explosion             - Create an explosion");
        Log("  createlight           - Create a light source");
        Log("  gravity               - Set gravity");
        Log("  exit                  - Exit the program");
    }
    if (command == "hotkeys") {
        Log("Available Hotkeys:");
        Log("  F1                    - Toggle Slew (Noclip)");
        Log("  F2                    - Enables the animation debug overlay");
        Log("  F3                    - Enables the channel debug overlay");
        Log("  F4                    - Enables the cinematics debug overlay");
        Log("  F5                    - Unlock all weapons");
        Log("  8                     - Prints players position");
        Log("  9                     - Spawn Ghostbuster at players position");
        Log("  E                     - Toggle flashlight");
        Log("  Q                     - Holster/Unholster proton wand");
        Log("  Z                     - Returns true or false if trap is deployed");
        Log("  P                     - Fake possess player");
        Log("  G                     - Toggle ecto goggles position");
    }
    else if (command == "spawnactor") {
        SpawnActor();
        Log("Actor spawned.");
    }
    else if (command == "survivalmode") {
        RunMod();
    }
    else if (command == "gbloaded") {
        gbloaded();
    }

    else if (command == "restart") {
        ResLevel();
        Log("Level Restarted.");
    } 
    else if (command == "clear") {
        consoleLogs.clear();
    }
    else if (command == "checkversion") {
        Log("IE17 Version: " STR(IE17ver));
    }
    else {
        Log("Unknown command: " + command);
        Log("Type 'help' for a list of commands.");
    }
}

void DrawConsole() {

    static bool previousShowWindow = false; // To track changes in showWindow

    // Check if showWindow state has changed
    if (showWindow != previousShowWindow) {
        ImGuiIO& io = ImGui::GetIO();

        if (showWindow) {
            io.MouseDrawCursor = true; // Hide the cursor when the window is shown
        }
        else {
            io.MouseDrawCursor = false; // Show the cursor when the window is hidden
        }

        previousShowWindow = showWindow; // Update the previousShowWindow state
    }

    if (showWindow)
    {

        ImVec2 windowSize(500.0f, 500.0f);

        ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
        ImGui::Begin("IE17 Console", nullptr, ImGuiWindowFlags_NoResize);

        // Output log
        ImGui::BeginChild("ScrollRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
        for (const auto& log : consoleLogs) {
            ImGui::TextUnformatted(log.c_str());
        }
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f); // Auto-scroll to bottom
        }
        ImGui::EndChild();

        // Input text
        ImGui::Separator();
        static char inputBuffer[256] = ""; // Temporary buffer
        if (ImGui::InputText("##CommandInput", inputBuffer, IM_ARRAYSIZE(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
            currentInput = std::string(inputBuffer); // Copy buffer to std::string
            if (!currentInput.empty()) {
                HandleCommand(currentInput); // Process the command
                currentInput.clear(); // Clear the current input
                inputBuffer[0] = '\0'; // Clear the buffer
            }

        }
        ImGui::End();
    }
}

void DrawCollum()
{
    static bool previousShowWindow = false; // To track changes in showWindow

    // Check if showWindow state has changed
    if (showColumnWindow != previousShowWindow) {
        ImGuiIO& io = ImGui::GetIO();

        if (showColumnWindow) {
            io.MouseDrawCursor = true; // Hide the cursor when the window is shown
        }
        else {
            io.MouseDrawCursor = false; // Show the cursor when the window is hidden
        }

        previousShowWindow = showColumnWindow; // Update the previousShowWindow state
    }

    if (showColumnWindow)
    {
        ImVec2 windowSize(500.0f, 500.0f);

        ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
        const char* names[] = { "Player", "Winston", "Venkman", "Ray", "Egon" };
        static int selectedIndex = 0; // This will store the selected index
        static unsigned __int64 selectedEntity = localplayer; // Default to localPlayer

        ImGui::Begin("Debug Menu", nullptr, ImGuiWindowFlags_NoResize);

        if (ImGui::BeginCombo("Select Ghostbuster", names[selectedIndex])) {
            for (int i = 0; i < IM_ARRAYSIZE(names); ++i) {
                bool isSelected = (selectedIndex == i);
                if (ImGui::Selectable(names[i], isSelected)) {
                    selectedIndex = i;

                    // Update selected entity based on the name selected
                    switch (selectedIndex) {
                    case 0: selectedEntity = localplayer; break;
                    case 1: selectedEntity = winston; break;
                    case 2: selectedEntity = venkman; break;
                    case 3: selectedEntity = ray; break;
                    case 4: selectedEntity = egon; break;
                    }
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        // Button to trigger fake possession with the selected entity
        if (ImGui::Button("Possess Selected")) {
            fakePossession(selectedEntity, true);
        }
        if (ImGui::Button("Unlock All")) {
            unlockall(selectedEntity);
        }
        if (ImGui::Button("Knock Back")) {
            knockBackActor(selectedEntity);
        }
        ImGui::End();
    }
}