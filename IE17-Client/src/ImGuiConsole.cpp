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
#include <psapi.h>


bool showWindow = false;
bool showColumnWindow = false;
bool showCinematicTab = false;
bool showMenu = false;

std::vector<std::string> consoleLogs;
std::string currentInput;

float GetMemoryUsage()
{
    PROCESS_MEMORY_COUNTERS memCounters;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memCounters, sizeof(memCounters)))
    {
        return static_cast<float>(memCounters.WorkingSetSize) / (1024.0f * 1024.0f); // convert bytes to mb
    }
    return 0.0f;
}

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
        Log("  cinematic             - Opens cinematics tab");
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
        Log("***************************");
        Log("     IE17 is hooked!  ");
        Log("***************************");
        Log("Version: " STR(IE17ver));
    }
    else if (command == "checkversion") {
        Log("IE17 Version: " STR(IE17ver));
    }
    else if (command == "cinematic") {
        showCinematicTab = true;
	}
	else if (command == "exit") {
		Log("Exiting...");
		std::this_thread::sleep_for(std::chrono::seconds(1));
		exit(0);
        
    }
    else {
        Log("Unknown command: " + command);
        Log("Type 'help' for a list of commands.");
    }
}

void DrawConsole() {

    static bool hasLoggedInitMessage = false;

    if (!hasLoggedInitMessage) {
        Log("***************************");
        Log("     IE17 is hooked!  ");
        Log("***************************");
        Log("Version: " STR(IE17ver) );
        hasLoggedInitMessage = true;
    }

    if (showWindow) {
        ImVec2 windowSize(414, 423);
        ImGui::SetNextWindowSize(windowSize, 0);
        ImGui::Begin("IE17 Console", nullptr, ImGuiWindowFlags_NoResize);

        ImGui::BeginChild("ScrollRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);

        ImGuiIO& io = ImGui::GetIO();
        float fps = io.Framerate;
        float memory_usage_mb = GetMemoryUsage();
        float frame_time_ms = 1000.0f / io.Framerate;

        ImGui::TextColored(ImVec4(0.6f, 0.8f, 0.2f, 1.0f), "Performance Metrics:");
        ImGui::Text("  FPS: %.1f", fps);
        ImGui::Text("  Frame Time: %.3f ms", frame_time_ms);
        ImGui::Text("  Memory Usage: %.1f MB", memory_usage_mb);
        ImGui::Separator();

        for (const auto& log : consoleLogs) {
            ImGui::TextUnformatted(log.c_str());
        }

        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();

        // input text
        ImGui::Separator();
        static char inputBuffer[256] = "";
        if (ImGui::InputText("##CommandInput", inputBuffer, IM_ARRAYSIZE(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
            currentInput = std::string(inputBuffer);
            if (!currentInput.empty()) {
                HandleCommand(currentInput);
                currentInput.clear();
                inputBuffer[0] = '\0';
            }
        }
        ImGui::End();
    }
}

void DrawCollum()
{

    if (showColumnWindow)
    {
        ImVec2 windowSize(419, 190);
        //ImVec2 position(4, 315);
        ImGui::SetNextWindowSize(windowSize, 0);
        //ImGui::SetNextWindowPos(position, ImGuiCond_Always);
        const char* names[] = { "Player", "Winston", "Venkman", "Ray", "Egon" };
        static int selectedIndex = 0; // this will store the selected index
        static unsigned __int64 selectedEntity = localplayer; // fefault to localplayer

        ImGui::Begin("Debug Menu", nullptr, ImGuiWindowFlags_NoResize);

        if (ImGui::BeginCombo("Select Ghostbuster", names[selectedIndex])) {
            for (int i = 0; i < IM_ARRAYSIZE(names); ++i) {
                bool isSelected = (selectedIndex == i);
                if (ImGui::Selectable(names[i], isSelected)) {
                    selectedIndex = i;

                    // update selected entity based on the name selected
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

		// buttons to trigger actions.
        if (ImGui::Button("Possess Selected")) {
            fakePossession(selectedEntity, true);
        }
        if (ImGui::Button("Unlock All")) {
            unlockall(selectedEntity);
        }
        if (ImGui::Button("Knock Back")) {
            knockBackActor(selectedEntity);
        }
        if (ImGui::Button("Warp To Player")) {
            warpToPlayer(selectedEntity);
        }
        ImGui::End();
    }
}

void CinematicTab()
{
    static int currentSelection = 0;
    const char* cinematicNames[] = {
    "cs_ts_09.cinemat", "cs_ts_03.cinemat", "cs_ts_02.cinemat",
    "cs_h1_brush_off.cinemat", "cs_h1_08.cinemat", "cine_h1_elevator_02.cinemat",
    "cine_h1_elevator_01.cinemat", "cs_fh2_scene.cinemat",
    "cs_fh1_01_005.cinemat", "cs_fh1_01_004.cinemat", "fh1_01_003.cinemat", "cs_cem_03.cinemat",
    "cs_cem_02.cinemat", "cs_cem_01.cinemat", "h1_mngr_bllrm_ent_3_final.cinemat", "ts2_ilyssa_rescued_stairs.cinemat"
    }; //all of the cinematics listed

    if (showCinematicTab)
    {
        constexpr int cinematicCount = sizeof(cinematicNames) / sizeof(cinematicNames[0]);

        ImVec2 windowSize(450.0f, 97.0f);
        //ImVec2 position(4, 464);
        ImGui::SetNextWindowSize(windowSize, 0);
        //ImGui::SetNextWindowPos(position, ImGuiCond_Always);
        ImGui::Begin("Cinematics Tab", nullptr, ImGuiWindowFlags_NoResize);

        if (ImGui::BeginCombo("Cinematics", cinematicNames[currentSelection])) {
            for (int i = 0; i < cinematicCount; i++) {
                const bool isSelected = (currentSelection == i);
                if (ImGui::Selectable(cinematicNames[i], isSelected)) {
                    currentSelection = i;
                }
                // set the initial focus when opening the combo
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        // Button to play the selected cinematic
        if (ImGui::Button("Play")) {
            cacheStreamingCinemat(&cinematicNames[currentSelection]);
            Sleep(3000); //need to make some how else because the whole application stops.
            cueStreamingCinemat(cinematicNames[currentSelection], 0);
            playStreamingCinemat(cinematicNames[currentSelection]);
        }
        ImGui::End();
    }
}

void DrawMenu() {

    static bool previousShowWindow = false;

    if (showMenu != previousShowWindow) {
        ImGuiIO& io = ImGui::GetIO();

        if (showMenu) {
            io.MouseDrawCursor = true;
        }
        else {
            io.MouseDrawCursor = false;
        }

        previousShowWindow = showMenu;
    }
    if (showMenu)
    {
        ImVec2 size(250, 340);
        ImGui::SetNextWindowSize(size, 0);
        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoResize);
        if (ImGui::CollapsingHeader("Debug"))
        {
            if (ImGui::Button("Console"))
            {
                showWindow = !showWindow;
            }
            if (ImGui::Button("Ghostbusters Tweaking"))
            {
                showColumnWindow = !showColumnWindow;
            }
            if (ImGui::Button("Cinematic Tab"))
            {
                showCinematicTab = !showCinematicTab;
            }
        }

        if (ImGui::CollapsingHeader("Options"))
        {
            const char* levelNames[] = {
                "13th_floor_boss.lvl",
                "abyss.lvl",
                "boss_sp_side.lvl",
                "cemetery1.lvl",
                "cemetery2.lvl",
                "firehouse.lvl",
                "hotel1a.lvl",
                "hotel1b.lvl",
                "hotel2.lvl",
                "library1a.lvl",
                "library1b.lvl",
                "library2.lvl",
                "lost_island.lvl",
                "lost_island2.lvl",
                "museum1.lvl",
                "museum2.lvl",
                "museum3.lvl",
                "timessquare1.lvl",
                "timessquare1b.lvl",
                "timessquare2.lvl"
            };
            static int selectedLevelIndex = 0;

            if (ImGui::Combo("Level", &selectedLevelIndex, levelNames, IM_ARRAYSIZE(levelNames)))
            {
                loadLevel(levelNames[selectedLevelIndex]);
            }
        }
        ImGui::End();
    }
}