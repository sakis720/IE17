#include "main.h"
#include "player.h"
#include "enums.h"
#include "script.h"
#include "actors.h"
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
#include <ImGuiConsole.h>

using namespace std;

bool holsterStatus = false;
int eGogglesStatus = 0;
bool fakePossessionStatus = false;

bool wasQPressed = false;


bool m_about = false;
bool m_legacycrash = false;
bool b_spawnactor = false;
bool g_fRestartLevel = false;

// Flag to keep the main thread running
bool runProgram = true;
 
char* g_modBase = nullptr;

int playerCash = 0;

void (*blockHeroMovement)(unsigned __int64*, bool*);
void (*toggleHuntMode)(unsigned __int64*, bool*);
void (*enableProtonTorpedo)(unsigned __int64*, bool*);
void (*setCommandCrossBeam)(unsigned __int64);
void (*startFakePackOverheat)(unsigned __int64*);
void (*letterbox)(bool*);
void (*queueVideo)(const char**);
void (*setMovieCaptureEnable)(bool*);
void (*allowEnemyAttack)(bool*);
void (*allowHeroControls)(bool*);
void (*allowHeroDamage)(bool*);
void (*play)(unsigned __int64, bool, int, float, float);
void (*setCameraPathActor)(unsigned __int64, unsigned __int64, float, float, float);
void (*shatter)(unsigned __int64, Vector);
void (*setSimEnable)(unsigned __int64, int);
void (*loadCheckpoint)(const char**);
void (*setCurrentObjective)(const char**);
void (*toggleReviveMode)(unsigned __int64, bool);
int (*chainToLevel)(unsigned __int64, const char*, const char*);
void (*transferHeroshipTo)(unsigned __int64, unsigned __int64);
void (*slimeMe)(unsigned __int64, bool, float);
void (*knockBack)(unsigned __int64, Vector, float);
void (*pretendToDrive)(unsigned __int64, unsigned __int64, bool, bool);
void (*mountProtonPack)(unsigned __int64, bool);
void (*fakeFireProtonGun)(unsigned __int64, bool);
char (*forceDeployTrap)(unsigned __int64, Vector);
void (*cacheRappel)(unsigned __int64);
void (*setRappelModeEnable)(unsigned __int64, bool);
void (*startRappelSwing)(unsigned __int64);
bool (*isDead)(unsigned __int64);
void (*cacheStreamingCinematAndAudio)(const char*, const char*);
void (*stopStreamingCinemat)(const char*);
void (*cacheStreamingCinemat)(const char**);
void (*cueStreamingCinemat)(const char*, float);
void (*playStreamingCinemat)(const char*);
void (*GTFO)(const char*, int);
void (*cacheSkeletalAnimationByName)(const char*);
void (*enable)(unsigned __int64, bool*, bool);
void (*setProtonBeamMaxLength)(float);
void (*setAnimation)(unsigned __int64, const char*, bool, bool);
void (*detonate)(unsigned __int64, float);
void (*attachToActorTag)(unsigned __int64, unsigned __int64, const char*, bool);
void (*setCurrentTeam)(unsigned __int64, int);
bool (*isTrapDeployed)(unsigned __int64);
void (*gatherAllDeployedInventoryItems)(unsigned __int64);
void (*readyInventoryItem)(unsigned __int64, int, bool);
void (*enableInventoryItem)(unsigned __int64, int, bool);
void (*isPackOverheated)(unsigned __int64);
void (*slamGoggleLocation)(unsigned __int64, int);
void (*setGoggleLocation)(unsigned __int64, int);
void (*setFacialExpression)(unsigned __int64, int);
void (*stopControllingActor)(unsigned __int64);
int (*warpTo)(unsigned __int64, Vector, Vector);
int (*warpToActorSeamless)(unsigned __int64, unsigned __int64);
void (*fakePossession)(unsigned __int64, bool);
void (*setFlashlightMode)(unsigned __int64, int);
void (*toggleflashlight)(unsigned __int64, int); // toggleflashlight and setFlashlightMode are diffrent functions
void (*commitSuicide)(unsigned __int64);
void (*setHealth)(unsigned __int64, float);
void (*setNothingEquipped)(unsigned __int64, bool);
void (*enableAllLights)(bool*);
void (*DanteVMaddExport)(const char*, const char*, int);
void (*buttonPrompt)(int, float);
void (*setAllowDamageTally)(bool*);
void (*fade)(float, float, float, float, float);
void (*displaySplashScreen)(const char*, float, bool, bool);
void (*CacheEffect)(const char**);
int (*StartEffect)(const char*, Vector, Vector);
void (*CreateExplosion)(Vector, float, float, float);
void (*SetGravity)(Vector);
void (*AddLight)(Vector, float, Vector, float, float, float, float);
void (*CreateActor)(const char*, Vector);
int (*DisplayText)(int, const char*, float);
int (*DisplayTextLegacy)(int, const char*, const char*, char);


using _SlewFun = void(__cdecl*)();
using _GhostViewerFunc = void(__cdecl*)();
using _CancelWalkAll = void(__cdecl*)();
using _QuitLevel = void(__cdecl*)();
using _animation = void(__cdecl*)();
using _cinemat = void(__cdecl*)();
using _channels = void(__cdecl*)();
using _resgravity = void(__cdecl*)();


_SlewFun Slew;
_GhostViewerFunc GhostViewer;
_CancelWalkAll CancelWalkAll;
_QuitLevel quitLevel;
_animation animDebug;
_cinemat cinematDebug;
_channels chanDebug;
_resgravity resetgravity;


typedef void (*OriginalFunctionType)(char* Buffer, __int64 adr1, __int64 adr2, __int64 adr3);
OriginalFunctionType originalFunction = nullptr;

void __stdcall HookedFunction(char* Buffer, __int64 adr1, __int64 adr2, __int64 adr3) {

    bool debugMode = false;  // change this to true if you want to enable logging

    if (debugMode) {

        std::ofstream logFile("dante_reg.txt", std::ios_base::app);

        std::cout << "Buffer (Type of): " << (Buffer ? Buffer : "NULL") << std::endl;
        std::cout << "Local Object Address 1: 0x" << adr1 << std::hex << std::endl;
        std::cout << "Local Object Address 2: 0x" << adr2 << std::hex << std::endl;
        std::cout << "Local Object Address 3: 0x" << adr3 << std::hex << std::endl;

        LogFormatted("Buffer (Type of): %s", (Buffer ? Buffer : "NULL"));
        LogFormatted("Local Object Address 1: 0x%p", adr1);
        LogFormatted("Local Object Address 2: 0x%p", adr2);
        LogFormatted("Local Object Address 3: 0x%p", adr3);

        logFile << "Buffer (Type of): " << (Buffer ? Buffer : "NULL") << std::endl;
        logFile << "Local Object Address 1: 0x" << std::hex << adr1 << std::endl;
        logFile << "Local Object Address 2: 0x" << std::hex << adr2 << std::endl;
        logFile << "Local Object Address 3: 0x" << std::hex << adr3 << std::endl;

        logFile.close();
    }

    // call the original function if needed
    if (originalFunction) {
        originalFunction(Buffer, adr1, adr2, adr3);
    }

    getPlayer(Buffer, adr1);
	getGhostbusters(Buffer, adr1);
	getEcto(Buffer, adr1);
	getCMainView(Buffer, adr1);
	getEmmit(Buffer, adr1);
}

// Continuously check for key presses in a separate thread
void HandleKeyPresses()
{
    Slew = (_SlewFun)(g_modBase + 0x1F9D50);
    GhostViewer = (_GhostViewerFunc)(g_modBase + 0x1F8360);
    CancelWalkAll = (_CancelWalkAll)(g_modBase + 0x1F81B0);
    quitLevel = (_QuitLevel)(g_modBase + 0x1F8170);
    animDebug = (_animation)(g_modBase + 0x1F7FB0);
    cinematDebug = (_cinemat)(g_modBase + 0x1F7FC0);
    chanDebug = (_channels)(g_modBase + 0x1F7FD0);
    resetgravity = (_resgravity)(g_modBase + 0x1F82E0);

    while (runProgram)
    {
        if (GetAsyncKeyState(VK_F1) & 1) {
            Slew();  // Call the Slew function
            Sleep(500);  // Prevent multiple triggers within a short time
        } else if (GetAsyncKeyState(VK_F2) & 1) {
            animDebug();  // Call the animDebug function
            Sleep(500);  // Prevent multiple triggers within a short time
        } else if (GetAsyncKeyState(VK_F3) & 1) {
            chanDebug();  // Call the chanDebug function
            Sleep(500);  // Prevent multiple triggers within a short time
        } else if (GetAsyncKeyState(VK_F4) & 1) { 
            cinematDebug();  // Call the cinematDebug function
            Sleep(500);  // Prevent multiple triggers within a short time
        }else if (GetAsyncKeyState(VK_F6) & 1) { 
            //script();  // Call the cinematDebug function
            Sleep(500);  // Prevent multiple triggers within a short time
        }
        else if (GetAsyncKeyState(VK_F5) & 1) { //enable all equipment

			enableInventoryItem(localplayer, eInventoryProtonGun, true);
			enableInventoryItem(localplayer, eInventorySlimeGun, true);
			enableInventoryItem(localplayer, eInventoryRailgun, true);
			enableInventoryItem(localplayer, eInventoryShotgun, true);

			//blockHeroMovement(&player, &state);
			//toggleHuntMode(&player, &state); idk what exactly this does.
			//const char* video = "logo";
			//queueVideo(&video);
            //bool state = true;
            //setMovieCaptureEnable(&state);
            //Sleep(2500);
            //shatter(emmit, playerPos);
            //shatter(emmit2, playerPos);
			//const char* checkpoint = "Underground";
			//loadCheckpoint(&checkpoint);
			//attachToActorTag(localplayer, emmit, "mouth", false); // R_hand/L mouth
			//warpToActorSeamless(localplayer, egon);
			//pretendToDrive(egon, ecto, false, false); // for some reason player(localplayer) can't drive or sit as a passenger
			//cacheRappel(localplayer);
			//setRappelModeEnable(localplayer, true);
            //Sleep(2000);
            //setRappelModeEnable(localplayer, false);
			//startRappelSwing(localplayer);
            //const char* cinemat06 = "cs_cem_01.cinemat";
            //cacheStreamingCinemat(&cinemat06);
            //Sleep(3000);
			//cueStreamingCinemat(cinemat06, 0.0f);
			//playStreamingCinemat(cinemat06);
            //GTFO("Game Crashed idk why", -10);
			//setProtonBeamMaxLength(15.0f); //max 400.0f
			//const char* ani = "upgrading_pack"; 
            //setAnimation(localplayer, ani, false, false);
            //cout << "Animaiton set to: " << ani << "\n";

            Sleep(500);  // Prevent multiple triggers within a short time
        }
        else if (GetAsyncKeyState('8') & 1) {
            GetPlayerPosition();  // Call the cinematDebug function

            std::cout << "Player Position: ("
                << playerPos.x << ", "
                << playerPos.y << ", "
                << playerPos.z << ")" << std::endl;

            Sleep(500);  // Prevent multiple triggers within a short time
        }
        else if (GetAsyncKeyState('9') & 1) {
            SpawnActor();

            std::cout << "Ghostbuster Spawned: ("
                << playerPos.x << ", "
                << playerPos.y << ", "
                << playerPos.z << ")" << std::endl;

            Sleep(500);  // Prevent multiple triggers within a short time
        }
        else if (GetAsyncKeyState('E') & 1) {
			if (localplayer != 0) {  // Call the function only if localplayer value is set
				g_modBase = (char*)GetModuleHandle(NULL);  // Update g_modBase value
				//setFlashlightMode(localplayer, eFlashlightModeUVLight);
				toggleflashlight(localplayer, eFlashlightModeNormal); //the eFlashlightModeNormal is useless for toggleflashlight
				Sleep(500);  // Prevent multiple triggers within a short time
			}
        }
        else if (GetAsyncKeyState('=') & 1) {
            if (localplayer != 0) {  // call the function only if localplayer value is set
                g_modBase = (char*)GetModuleHandle(NULL);  // update g_modBase value
                // call IsTrapDeployed to check if a trap is deployed
                bool trapDeployed = isTrapDeployed(localplayer);
                if (trapDeployed) {
                    cout << "Trap is deployed!\n";
                }
                else {
                    cout << "Trap isn't deployed.\n";
                }
                Sleep(500);  // Prevent multiple triggers within a short time
            }
        }
        else if (GetAsyncKeyState(']') & 1) {
			if (localplayer != 0) {  // Call the function only if localplayer value is set
				g_modBase = (char*)GetModuleHandle(NULL);  // Update g_modBase value
				if (!fakePossessionStatus) {
					fakePossession(localplayer, true);
				} else {
					fakePossession(localplayer, false);
				}
				fakePossessionStatus = !fakePossessionStatus;
				Sleep(500);  // Prevent multiple triggers within a short time
			}
        }
        else if (GetAsyncKeyState('[') & 1) {
			if (localplayer != 0) {  // Call the function only if localplayer value is set
				g_modBase = (char*)GetModuleHandle(NULL);  // Update g_modBase value
				if (eGogglesStatus == 0) {
					eGogglesStatus = eGogglesOnFace;
					setGoggleLocation(localplayer, eGogglesOnFace);
				} else if (eGogglesStatus == 1) {
					eGogglesStatus = eGogglesOnBelt;
					setGoggleLocation(localplayer, eGogglesOnBelt);
				} else if (eGogglesStatus == 2) {
					eGogglesStatus = eGogglesOnHead;
					setGoggleLocation(localplayer, eGogglesOnHead);
				}
				Sleep(500);  // Prevent multiple triggers within a short time
			}
        }
        if (GetAsyncKeyState('Q') & 0x8000) {  // Check if 'Q' is currently pressed
            if (!wasQPressed) {  
                wasQPressed = true;  // mark 'Q' as pressed

                if (localplayer != 0) {  // Call the function only if localplayer value is set
                    g_modBase = (char*)GetModuleHandle(NULL);  // Update g_modBase value
                    if (!holsterStatus) {
                        readyInventoryItem(localplayer, eInventoryNothing, true);
                        // setGoggleLocation(localplayer, eGogglesOnHead);
                    }
                    else {
                        readyInventoryItem(localplayer, eInventoryProtonGun, true);
                        // setGoggleLocation(localplayer, eGogglesOnFace);
                    }
                    holsterStatus = !holsterStatus;
                }
            }
        }
        else {
            wasQPressed = false;  // reset the flag when 'Q' is released
        }
        Sleep(10);  // Small delay to avoid high CPU usage
    }
}

//need to put this somewhere else takes too much of a space
void HandleInput()
{
    string input;
    while (runProgram)
    {
        cout << "> "; 
        if (!getline(cin, input))
        {
            break;
        }

        size_t first = input.find_first_not_of(" \t");
        if (first == string::npos)
            continue; // Empty input
        input = input.substr(first, input.find_last_not_of(" \t") - first + 1);

        if (input == "slew")
        {
            Slew();
            cout << "Slew (Noclip) toggled.\n";
        }
        else if (input == "quitlevel")
        {
            quitLevel();
            cout << "Level Quitted.\n";
        }
        else if (input == "animdebug")
        {
            animDebug();
            cout << "Animation debug overlay function was called.\n";
        }
        else if (input == "cinematdebug")
        {
            cinematDebug();
            cout << "Cinemat debug overlay function was called.\n";
        }
        else if (input == "channeldebug")
        {
            chanDebug();
            cout << "Channel debug overlay function was called.\n";
        }
        else if (input == "cancelwalk")
        {
            CancelWalkAll();
            cout << "Walk is disabled on all AI??????.\n";
        }
        else if (input == "ghostviewer")
        {
            GhostViewer();
            cout << "Ghost Viewer toggled.\n";
        }
        else if (input == "resetgravity")
        {
            resetgravity();
            cout << "Gravity Reseted.\n";
        }
        else if (input == "about")
        {
            AboutMod();
            cout << "About information toggled.\n";
        }
        /*
        else if (input == "getlevel")
        {
            std::string level = GetCurLevel();
        }
        */
        else if (input == "restart")
        {
            ResLevel();
            cout << "Restarting level...\n";
        }
        else if (input == "legacytext")
        {
            TestLegacyText();
            cout << "Legacy text display toggled.\n";
        }
        else if (input == "spawnactor")
        {
            SpawnActor();
            cout << "Spawn Actor toggled.\n";
        }
        else if (input == "survivalmode")
        {
            RunMod();
            //cout << "Survival Mode(Prototype).\n";
        }
        else if (input == "gbloaded")
        {
			if (localplayer != 0)
			{
				cout << "Player                  LOADED.\n";
			}
			else
			{
				cout << "Player                  NOT loaded.\n";
			}

            if (egon != 0)
            {
                cout << "Egon                    LOADED.\n";
            }
            else
            {
                cout << "Egon                    NOT loaded.\n";
            }

            if (winston != 0)
            {
                cout << "Winston                 LOADED.\n";
            }
            else
            {
                cout << "Winston                 NOT loaded.\n";
            }

            if (venkman != 0)
            {
                cout << "Venkman                 LOADED.\n";
            }
            else
            {
                cout << "Venkman                 NOT loaded.\n";
            }

            if (ray != 0)
            {
                cout << "Ray                     LOADED.\n";
            }
            else
            {
                cout << "Ray                     NOT LOADED.\n";
            }
        }
        else if (input == "explosion")
        {
            cout << "Enter explosion parameters (x y z radius strength speed): ";
            float x, y, z, radius, strength, speed;
            if (cin >> x >> y >> z >> radius >> strength >> speed)
            {
                Vector position{ x, y, z };
                CreateExplosion(position, radius, strength, speed);
                cout << "Explosion created at (" << x << ", " << y << ", " << z 
                     << ") with radius " << radius 
                     << ", strength " << strength 
                     << ", and speed " << speed << ".\n";
            }
            else
            {
                cin.clear(); // Clear the error state
                cin.ignore();
                cout << "Invalid parameters. Please enter numeric values.\n";
            }
        }
        else if (input == "createlight")
        {
            cout << "Enter position (x y z), radius, RGB color (r g b), intensity, duration, ramp-up, and ramp-down: ";
            float px, py, pz, radius, r, g, b, intensity, duration, rampUp, rampDown;
            cin >> px >> py >> pz >> radius >> r >> g >> b >> intensity >> duration >> rampUp >> rampDown;

            if (!cin.fail())
            {
                Vector position{ px, py, pz };
                Vector color{ r, g, b };

                AddLight(position, radius, color, intensity, duration, rampUp, rampDown);
                cout << "Light added at (" << px << ", " << py << ", " << pz << ") with radius " << radius
                    << ", color (" << r << ", " << g << ", " << b << "), intensity " << intensity
                    << ", duration " << duration << ", ramp-up " << rampUp << ", and ramp-down " << rampDown << ".\n";
            }
            else
            {
                cin.clear();
                cin.ignore();
                cout << "Invalid input. Please enter valid numeric values for the light parameters.\n";
            }
        }
        else if (input == "createffect")
        {
            cout << "Enter effect file name, position (x y z), and orientation (x y z): ";
            string effectFilename;
            float px, py, pz, ox, oy, oz;

            cin >> effectFilename >> px >> py >> pz >> ox >> oy >> oz;

            if (!cin.fail())
            {
                Vector position{ px, py, pz };
                Vector orientation{ ox, oy, oz };

                int result = StartEffect(effectFilename.c_str(), position, orientation);
                if (result >= 0)
                {
                    cout << "Effect '" << effectFilename << "' started at (" << px << ", " << py << ", " << pz
                        << ") with orientation (" << ox << ", " << oy << ", " << oz << ").\n";
                }
                else
                {
                    cout << "Failed to start effect '" << effectFilename << "' make sure you added .tfb at the end.\n";
                }
            }
            else
            {
                cin.clear();
                cin.ignore();
                cout << "Invalid input. Please enter a valid effect name, position, and orientation.\n";
            }
        }
        else if (input == "gravity")
        {
            cout << "Enter gravity parameters (x y z): ";
            float x, y, z;
            cin >> x >> y >> z;

            if (!cin.fail())
            {
                Vector position{ x, y, z };
                SetGravity(position);
                cout << "Set gravity with values (" << x << ", " << y << ", " << z << ").\n";
            }
            else
            {
                cin.clear();
                cin.ignore();
                cout << "Invalid input. Please enter a valid numeric coordinates.\n";
            }
        }
        else if (input.find("help") == 0)
        {
            cout << "Available commands:\n";
            cout << "  slew                  - Toggle Slew (Noclip)\n";
            cout << "  quitLevel             - Quit's the current level\n";
            cout << "  animdebug             - Enables the animation debug overlay\n";
            cout << "  cinematdebug          - Enables the cinematics debug overlay\n";
            cout << "  channeldebug          - Enables the channel debug overlay\n";
            cout << "  cancelwalk            - Disables the walk animation?\n";
            cout << "  ghostviewer           - Toggle Ghost Viewer\n";
            cout << "  about                 - Toggle About\n";
            cout << "  restart               - Restart Level\n";
            cout << "  gbloaded              - Check if Ghostbuster are loaded\n";
            cout << "  legacytext            - Toggle Legacy text display\n";
            cout << "  spawnactor            - Spawn Actor\n";
            cout << "  explosion             - Creates Explosion\n";
            cout << "  createlight           - Create a light source at a given position\n";
            cout << "  createffect           - Create a effect at a given position and orientation\n";
            cout << "  resetgravity          - Reset the gravity\n";
            cout << "  survivalmode          - Starts Survival Mode(Prototype very broken)\n";
            cout << "  gravity               - Set gravity\n";
            cout << "  help                  - Show this help message\n";
            cout << "  exit                  - Close the console\n";
        }
        else if (input.find("hotkeys") == 0)
        {
            cout << "Available Hotkeys:\n";
            cout << "  F1                    - Toggle Slew (Noclip)\n";
            cout << "  F2                    - Enables the animation debug overlay\n";
            cout << "  F3                    - Enables the channel debug overlay\n";
            cout << "  F4                    - Enables the cinematics debug overlay\n";
            cout << "  F5                    - Unlock all weapons\n";
            cout << "  8                     - Prints players position\n";
            cout << "  9                     - Spawn Ghostbuster at players position\n";
            cout << "  E                     - Toggle flashlight\n";
            cout << "  Q                     - Holster/Unholster proton wand\n";
            cout << "  Z                     - Returns true or false if trap is deployed\n";
            cout << "  P                     - Fake possess player\n";
            cout << "  G                     - Toggle ecto goggles position\n";
            }
        else if (input == "exit")
        {
            cout << "Exiting program.\n";
            FreeConsole();
            runProgram = false;
        }
        else if (input == "clear" || input == "cls")
        {
            system("cls");
            Sleep(10);
            cout << "*************************** \n"; //Re-print the infromation
            cout << "     IE17 is hooked! \n";
            cout << "*************************** \n";
            cout << "Version: " STR(IE17ver) "\n";
        }
        else
        {
            cout << "Unknown command. Type 'help' for a list of commands or 'hotkeys' for a list of hotkeys.\n";
        }
        Sleep(10);  // Small delay to avoid high CPU usage
    }
}

std::string GetCurLevel()
{
    uintptr_t pointeradr = 0x2C72528; //adr
    uintptr_t offset = 0x370; // Offset

    uintptr_t pointerAddress;
    SIZE_T bytesRead;

    if (!ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(g_modBase + pointeradr),
        &pointerAddress, sizeof(pointerAddress), &bytesRead)) {
        std::cout << "Failed to read first pointer. Error: " << GetLastError() << std::endl;
        return "";
    }

    uintptr_t finalAddress = pointerAddress + offset; // Final address

    char buffer[17] = { 0 }; // 16 characters + null terminator

    // Read the string at the final address
    if (!ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(finalAddress),
        buffer, 16, &bytesRead)) {
        //std::cout << "Not on a level." << std::endl; //Not on a level
        return "";
    }

    // Print the level name
    //std::cout << "Level Name : " << buffer << std::endl;

    return std::string(buffer); // return the level name as a std::string
}

void SetTerminalOnTop()
{
    HWND consoleWindow = GetConsoleWindow(); //get handle
    if (consoleWindow != nullptr)
    {
        SetWindowPos(consoleWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
}

DWORD WINAPI DLLAttach(HMODULE hModule)
{
    MH_Initialize();
    //AllocConsole();
    //SetConsoleTitleA("IE17 Build " STR(IE17ver));

    //freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
    //freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

    //cout << "*************************** \n";
    //cout << "     IE17 is hooked! \n";
    //cout << "*************************** \n";
    //cout << "Version: " STR(IE17ver) "\n";

    g_modBase = (char*)GetModuleHandle(NULL);
    blockHeroMovement = (void(*)(unsigned __int64*, bool*))(g_modBase + 0xED660);
    toggleHuntMode = (void(*)(unsigned __int64*, bool*))(g_modBase + 0xED480);
    enableProtonTorpedo = (void(*)(unsigned __int64*, bool*))(g_modBase + 0xEDE30);
    setCommandCrossBeam = (void(*)(unsigned __int64))(g_modBase + 0xEC640);
    startFakePackOverheat = (void(*)(unsigned __int64*))(g_modBase + 0xED750);
	letterbox = (void(*)(bool*))(g_modBase + 0x2D87A0); // don't know what it enables or disables
    queueVideo = (void(*)(const char**))(g_modBase + 0x2D87E0);
	setMovieCaptureEnable = (void(*)(bool*))(g_modBase + 0x2D8850); //make the game fast like the old movies this why its called movie capture
    allowEnemyAttack = (void(*)(bool*))(g_modBase + 0x2D8420);
    allowHeroControls = (void(*)(bool*))(g_modBase + 0x2D8440);
    allowHeroDamage = (void(*)(bool*))(g_modBase + 0x2D8460);
    play = (void(*)(unsigned __int64, bool, int, float, float))(g_modBase + 0x1C3F0); //anim models
    setCameraPathActor = (void(*)(unsigned __int64, unsigned __int64, float, float, float))(g_modBase + 0x1FF760);
    shatter = (void(*)(unsigned __int64, Vector))(g_modBase + 0x49EC70); //only for CGlass
    setSimEnable = (void(*)(unsigned __int64, int))(g_modBase + 0x89940);
    loadCheckpoint = (void(*)(const char**))(g_modBase + 0x1F81F0);
    setCurrentObjective = (void(*)(const char**))(g_modBase + 0x1F8200);
    toggleReviveMode = (void(*)(unsigned __int64, bool))(g_modBase + 0xD0EE0);
    chainToLevel = (int(*)(unsigned __int64, const char*, const char*))(g_modBase + 0x1EF700);
    transferHeroshipTo = (void(*)(unsigned __int64, unsigned __int64))(g_modBase + 0xD81A0);
    slimeMe = (void(*)(unsigned __int64, bool, float))(g_modBase + 0xD0F50);
    knockBack = (void(*)(unsigned __int64, Vector, float))(g_modBase + 0xED100);
    pretendToDrive = (void(*)(unsigned __int64, unsigned __int64, bool, bool))(g_modBase + 0xEAC40); //first bool is should he be in a drive seat and second is the supertrap.
    mountProtonPack = (void(*)(unsigned __int64, bool))(g_modBase + 0xE4640);
    fakeFireProtonGun = (void(*)(unsigned __int64, bool))(g_modBase + 0xE9060);
    cacheRappel = (void(*)(unsigned __int64))(g_modBase + 0xE1D70);
    forceDeployTrap = (char(*)(unsigned __int64, Vector))(g_modBase + 0xE4C80);
    setRappelModeEnable = (void(*)(unsigned __int64, bool))(g_modBase + 0xE1F70);
    startRappelSwing = (void(*)(unsigned __int64))(g_modBase + 0xE21E0);
	isDead = (bool(*)(unsigned __int64))(g_modBase + 0x7B170); //we can use this for the survival mode
    cacheStreamingCinematAndAudio = (void(*)(const char*, const char*))(g_modBase + 0x477500);
    stopStreamingCinemat = (void(*)(const char*))(g_modBase + 0x477B60);
    playStreamingCinemat = (void(*)(const char*))(g_modBase + 0x478930);
    cueStreamingCinemat = (void(*)(const char*, float))(g_modBase + 0x4779B0);
    cacheStreamingCinemat = (void(*)(const char**))(g_modBase + 0x476520);
	GTFO = (void(*)(const char*, int))(g_modBase + 0x2D11C0); //very 2000's function | GTFO is error msg int must be -10 | good for debbuging a function like a breakpoint
    cacheSkeletalAnimationByName = (void(*)(const char*))(g_modBase + 0x2D9AF0);
    enable = (void(*)(unsigned __int64, bool*, bool))(g_modBase + 0x2DA340);
    setProtonBeamMaxLength = (void(*)(float))(g_modBase + 0x277A50); // min 10.0f  max 400.0f
    setAnimation = (void(*)(unsigned __int64, const char*, bool, bool))(g_modBase + 0x77440);
    detonate = (void(*)(unsigned __int64, float))(g_modBase + 0x690F0); //car function
    attachToActorTag = (void(*)(unsigned __int64, unsigned __int64, const char*, bool))(g_modBase + 0x2BEE80);
    setCurrentTeam = (void(*)(unsigned __int64, int))(g_modBase + 0x15B20);
    isTrapDeployed = (bool(*)(unsigned __int64))(g_modBase + 0xDE370);
    gatherAllDeployedInventoryItems = (void(*)(unsigned __int64))(g_modBase + 0xE4770);
    readyInventoryItem = (void(*)(unsigned __int64, int, bool))(g_modBase + 0xE3F10);
    enableInventoryItem = (void(*)(unsigned __int64, int, bool))(g_modBase + 0xE4530);
    isPackOverheated = (void(*)(unsigned __int64))(g_modBase + 0xEA6D0);
    slamGoggleLocation = (void(*)(unsigned __int64, int))(g_modBase + 0xD51D0);
    setGoggleLocation = (void(*)(unsigned __int64, int))(g_modBase + 0xD50E0);
    setFacialExpression = (void(*)(unsigned __int64, int))(g_modBase + 0xCD370);
    stopControllingActor = (void(*)(unsigned __int64))(g_modBase + 0x76FD0);
    warpToActorSeamless = (int(*)(unsigned __int64, unsigned __int64))(g_modBase + 0xCDA20);
    warpTo = (int(*)(unsigned __int64, Vector, Vector))(g_modBase + 0x2C4520);
    fakePossession = (void(*)(unsigned __int64, bool))(g_modBase + 0xEC1E0);
    setFlashlightMode = (void(*)(unsigned __int64, int))(g_modBase + 0xE5AD0);
    toggleflashlight = (void(*)(unsigned __int64, int))(g_modBase + 0xE3BF0);
    commitSuicide = (void(*)(unsigned __int64))(g_modBase + 0xCE560);
    setHealth = (void(*)(unsigned __int64, float))(g_modBase + 0x7A890);
    setNothingEquipped = (void(*)(unsigned __int64, bool))(g_modBase + 0xE45A0);
    enableAllLights = (void(*)(bool*))(g_modBase + 0x2E3810);
    DanteVMaddExport = (void(*)(const char*, const char*, int))(g_modBase + 0x2CEC90);
    buttonPrompt = (void(*)(int, float))(g_modBase + 0x2494D0);
    setAllowDamageTally = (void(*)(bool*))(g_modBase + 0x1F8160);
    fade = (void(*)(float, float, float, float, float))(g_modBase + 0x1ECCA0); //float opacity, float r, float g, float b, float duration
    displaySplashScreen = (void(*)(const char*, float, bool, bool))(g_modBase + 0x1ECD50); //string textureName, float duration, bool stretch = false, bool clear = true    
    CacheEffect = (void(*)(const char**))(g_modBase + 0x35A380);
    StartEffect = (int(*)(const char*, Vector, Vector))(g_modBase + 0x35A730); // filename, pos, orient //needs cacheeffect to work
    CreateExplosion = (void(*)(Vector, float, float, float))(g_modBase + 0x1E9170); //pos, radius, damageStrength, speed
    SetGravity = (void(*)(Vector))(g_modBase + 0x1ECC40); //sets gravity
    AddLight = (void(*)(Vector, float, Vector, float, float, float, float))(g_modBase + 0x1ECB20); //vector pos, float radius, vector rgb, float intensity, float duration, float rampUp = 0.0, float rampDown = 0.0
    CreateActor = (void(*)(const char*, Vector))(g_modBase + 0x2C0D50); //const char class, vector pos(x,y,z)
    DisplayText = (int(*)(int, const char*, float))(g_modBase + 0x2494A0); //hudtype msg duration
    DisplayTextLegacy = (int(*)(int, const char*, const char*, char))(g_modBase + 0x2A6C90); //int hudtype, const char* msgtittle, const char* msg, int ?(duration??)
	void* GlobalRegisterFunc1 = (void*)((uintptr_t)GetModuleHandle(NULL) + 0x2CED00); //Thanks Malte0641 for the address

    // create the hook
    if (MH_CreateHook(GlobalRegisterFunc1, &HookedFunction, reinterpret_cast<LPVOID*>(&originalFunction)) != MH_OK) {
        MessageBoxA(NULL, "Failed to create hook.", "Error", MB_OK | MB_ICONERROR);
        MH_Uninitialize();
        return 1;
    }

    // enable the hook
    if (MH_EnableHook(GlobalRegisterFunc1) != MH_OK) {
        MessageBoxA(NULL, "Failed to enable hook.", "Error", MB_OK | MB_ICONERROR);
        MH_Uninitialize();
        return 1;
    }

    SetTerminalOnTop();
    //Start the key press detection in a separate thread
    std::thread keyPressThread(HandleKeyPresses);
    std::thread monitorThread(MonitorLevel);

    
    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HandleInput, NULL, 0, NULL);
    if (!hThread) {
        MessageBoxA(NULL, "Failed to create input handling thread.", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }


    WaitForSingleObject(hThread, INFINITE);



    // Ensure the key press thread finishes before exiting the program
    keyPressThread.join();
    monitorThread.join();

    //if (!AllocConsole()) {
    //    MessageBoxA(NULL, "Failed to allocate console.", "Error", MB_OK | MB_ICONERROR);
    //    FreeConsole();
    //    return 0; // Abort injection
    //}


    MH_Uninitialize();
    MH_DisableHook(GlobalRegisterFunc1);
    //fclose((FILE*)stdin);
    //fclose((FILE*)stdout);
    //FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 1;
}