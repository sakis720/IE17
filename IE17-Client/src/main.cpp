#include "main.h"
#include "functions.h"
#include "enums.h"
#include "actors.h"
#include "player.h"
#include "lua_functions.h"
#include <iostream>
#include <thread>
#include <fstream>
#include <chrono>
#include <script.h>
#include <filesystem> 
#include <unordered_map>

namespace fs = std::filesystem;

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

std::vector<lua_State*> activeLuaStates;

std::unordered_map<char, bool> keyStates;


char* g_modBase = nullptr;

int playerCash = 0;

bool enableDebug = false;

void enableDebugMenu(bool flag);

unsigned __int64 (*startTalking)(unsigned __int64, const char*);
unsigned __int64 (*startPreparedTalking)(unsigned __int64, const char*, int);
char (*setGhostbusterHeatlhState)(unsigned __int64 bustet_object, int state);
unsigned __int64 (*flinch)(unsigned __int64 buster_object);
unsigned __int64 (*Singleton_getRoom)(unsigned __int64* object);
unsigned __int64 (*setMusic)(unsigned __int64 database_entry);
unsigned __int64 (*hideHack)(unsigned __int64* buster_object);
void (*removeSlimeDecals)(unsigned __int64 buster_object);
void (*dbNarrativeStop)();
float (*dbNarrative)(unsigned __int64 database_entry);
void (*die)(unsigned __int64 object);
void (*blockHeroMovement)(unsigned __int64* buster_object, bool* state);
void (*toggleHuntMode)(unsigned __int64* object, bool* state);
void (*enableProtonTorpedo)(unsigned __int64* buster_object, bool* state);
void (*setCommandCrossBeam)(unsigned __int64 buster_object);
void (*startFakePackOverheat)(unsigned __int64* buster_object);
void (*letterbox)(bool* state);
void (*queueVideo)(const char** videoname);
void (*setMovieCaptureEnable)(bool* state);
void (*allowEnemyAttack)(bool* state);
void (*allowHeroControls)(bool* state);
void (*allowHeroDamage)(bool* state);
void (*play)(unsigned __int64 object, bool foward, int pattern, float newSpeed, float rampTime);
void (*setCameraPathActor)(unsigned __int64, unsigned __int64, float, float, float);
void (*shatter)(unsigned __int64 object, Vector WShatterPos);
void (*setSimEnable)(unsigned __int64 object, int flag);
void (*loadCheckpoint)(const char** checkpointName);
void (*setCurrentObjective)(const char** objDesc);
void (*toggleReviveMode)(unsigned __int64 buster_object, bool state);
int (*chainToLevel)(unsigned __int64 buster_object, const char* levelName, const char* lvlCheckpoint);
void (*transferHeroshipTo)(unsigned __int64 buster_object, unsigned __int64 buster_object_whom);
void (*slimeMe)(unsigned __int64 buster_object, bool fromFront, float decalDuration);
void (*knockBack)(unsigned __int64 buster_object, Vector WSourcePos, float impulse);
void (*pretendToDrive)(unsigned __int64 buster_object, unsigned __int64 car_object, bool driverSeatFlag, bool putTrapOnRoofFlag);
void (*mountProtonPack)(unsigned __int64 buster_object, bool state);
void (*fakeFireProtonGun)(unsigned __int64 buster_object, bool state);
char (*forceDeployTrap)(unsigned __int64 buster_object, Vector WSourcePos);
void (*cacheRappel)(unsigned __int64 buster_object);
void (*setRappelModeEnable)(unsigned __int64 buster_object, bool state);
void (*startRappelSwing)(unsigned __int64 buster_object);
bool (*isDead)(unsigned __int64 object);
void (*cacheStreamingCinematAndAudio)(const char* cinematName, const char* audioFileName);
void (*stopStreamingCinemat)(const char* cinematName);
void (*cacheStreamingCinemat)(const char** cinematName);
void (*cueStreamingCinemat)(const char* cinematName, float intialCursorPos);
void (*playStreamingCinemat)(const char* cinematName);
void (*GTFO)(const char* msg, int flag);
void (*cacheSkeletalAnimationByName)(const char* animationName);
int (*enable)(unsigned __int64 CGameBase, unsigned __int64 actor, bool flag); //broken
void (*setProtonBeamMaxLength)(float length);
void (*setAnimation)(unsigned __int64 object, const char* animationName, bool useSkelFileExit);
void (*detonate)(unsigned __int64 car_object, float timer);
void (*attachToActorTag)(unsigned __int64 global_object, unsigned __int64 object, const char* tagName, bool useCurrentRelativePosition);
void (*setCurrentTeam)(unsigned __int64 object, int type);
bool (*isTrapDeployed)(unsigned __int64 buster_object);
void (*gatherAllDeployedInventoryItems)(unsigned __int64 buster_object);
void (*readyInventoryItem)(unsigned __int64 buster_object, int itemToSwitchTo, bool state); //look at EInventoryItem
void (*enableInventoryItem)(unsigned __int64 buster_object, int itemToSwitchTo, bool state); //look at EInventoryItem
void (*isPackOverheated)(unsigned __int64); //need to check if this is correct
void (*slamGoggleLocation)(unsigned __int64 buster_object, int location); //look at EGoggles
void (*setGoggleLocation)(unsigned __int64 buster_object, int location); // look at EGoggles
void (*setFacialExpression)(unsigned __int64 buster_object, int newExpression); // look at EGhostbusterFacialExpression
void (*stopControllingActor)(unsigned __int64 buster_object);
int (*warpTo)(unsigned __int64 object, Vector pos, Vector orient);
int (*warpToActorSeamless)(unsigned __int64 object, unsigned __int64 global_object);
void (*fakePossession)(unsigned __int64 buster_object, bool state);
void (*setFlashlightMode)(unsigned __int64 buster_object, int newMode); //look at EFlashlightMode
void (*toggleflashlight)(unsigned __int64 buster_object, int newMode); // toggleflashlight and setFlashlightMode are diffrent functions
void (*commitSuicide)(unsigned __int64 buster_object);
void (*setHealth)(unsigned __int64, float); // need to be fixed
void (*setNothingEquipped)(unsigned __int64 buster_object, bool state);
void (*enableAllLights)(bool*);
void (*DanteVMaddExport)(const char*, const char*, int);
void (*buttonPrompt)(int buttonAction, float duration); //look at EButtonAction
void (*setAllowDamageTally)(bool* state);
void (*fade)(float opacity, float r, float g, float b, float duration);
void (*displaySplashScreen)(const char* textureName, float duration, bool stretch, bool clear);
void (*CacheEffect)(const char** effectName);
int (*StartEffect)(const char* effectName, Vector position, Vector orientation);
void (*CreateExplosion)(Vector pos, float radius, float damageStrength, float speed);
void (*SetGravity)(Vector velocity);
void (*AddLight)(Vector pos, float radius, Vector rgb, float intensity, float duration, float rampUp, float rampDown);
void (*CreateActor)(const char* className, Vector wPos);
unsigned __int64 (*Singleton_newActor)(const char* className, Vector wPos);
int (*DisplayText)(int messageId, const char* text, float duration); // look at EHudMessage
bool (*DisplayTextLegacy)(unsigned int messageId, const char* textDown, const char* textUp);


using _SlewFun = void(__cdecl*)();
using _GhostViewerFunc = void(__cdecl*)();
using _CancelWalkAll = void(__cdecl*)();
using _QuitLevel = void(__cdecl*)();
using _animation = void(__cdecl*)();
using _cinemat = void(__cdecl*)();
using _channels = void(__cdecl*)();
using _resgravity = void(__cdecl*)();
using _danteThreads = void(__cdecl*)();


_SlewFun Slew;
_GhostViewerFunc GhostViewer;
_CancelWalkAll CancelWalkAll;
_QuitLevel quitLevel;
_animation animDebug;
_cinemat cinematDebug;
_channels chanDebug;
_resgravity resetgravity;
_danteThreads danteThreads;


typedef void (*OriginalFunctionType)(char* Buffer, __int64 adr1, __int64 adr2, __int64 adr3);
OriginalFunctionType originalFunction = nullptr;

void __stdcall HookedFunction(char* Buffer, __int64 adr1, __int64 adr2, __int64 adr3) {

    bool debugMode = true;  // change this to true if you want to enable logging

    if (debugMode) {

        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm tm_buffer;
        localtime_s(&tm_buffer, &now_time_t); // Use localtime_s

        std::ofstream logFile("dante_reg.txt", std::ios_base::app);

        //std::cout << "Buffer (Type of): " << (Buffer ? Buffer : "NULL") << std::endl;
        //std::cout << "Local Object Address 1: 0x" << adr1 << std::hex << std::endl;
        //std::cout << "Local Object Address 2: 0x" << adr2 << std::hex << std::endl;
        //std::cout << "Local Object Address 3: 0x" << adr3 << std::hex << std::endl;

        logFile << "[" << std::put_time(&tm_buffer, "%Y-%m-%d %H:%M:%S") << "] ";
        logFile << "Buffer (Type of): " << (Buffer ? Buffer : "NULL") << std::endl;
        logFile << "Local Object Address 1: 0x" << std::hex << adr1 << std::endl;
        logFile << "Local Object Address 2: 0x" << std::hex << adr2 << std::endl;
        logFile << "Local Object Address 3: 0x" << std::hex << adr3 << std::endl;

        logFile.close();
    }

    if (!Buffer) {
        if (debugMode) {
            std::cout << "Buffer is null!" << std::endl;
        }
        return;
    }

    // call the original function if needed
    if (originalFunction) {
        originalFunction(Buffer, adr1, adr2, adr3);
    }
    else {
        if (debugMode) {
            std::cout << "originalFunction is null!" << std::endl;
        }
    }

    getPlayer(Buffer, adr1);
    getGhostbusters(Buffer, adr1);
    getEcto(Buffer, adr1);
    getCMainView(Buffer, adr1);
    //getEmmit(Buffer, adr1);
}

// Continuously check for key presses in a separate thread
void HandleKeyPresses()
{
    Slew = (_SlewFun)(g_modBase + 0x1F9D50);
    GhostViewer = (_GhostViewerFunc)(g_modBase + 0x1F8360);
    CancelWalkAll = (_CancelWalkAll)(g_modBase + 0x1F81B0);
    quitLevel = (_QuitLevel)(g_modBase + 0x1F8170);
    animDebug = (_animation)(g_modBase + 0x1F7FB0);
    cinematDebug = (_cinemat)(g_modBase + 0x1EC310);
    chanDebug = (_channels)(g_modBase + 0x1F7FD0);
    resetgravity = (_resgravity)(g_modBase + 0x1F82E0);
    danteThreads = (_danteThreads)(g_modBase + 0x1F2610);

    while (runProgram)
    {

        if (GetAsyncKeyState(VK_F1) & 0x8000) {
            Slew();  // Call the Slew function
            Sleep(500);  // Prevent multiple triggers within a short time
        }
        else if (GetAsyncKeyState(VK_F2) & 0x8000) {
            animDebug();  // Call the animDebug function
            Sleep(500);  // Prevent multiple triggers within a short time
        }
        else if (GetAsyncKeyState(VK_F3) & 0x8000) {
            chanDebug();  // Call the chanDebug function
            Sleep(500);  // Prevent multiple triggers within a short time
        }
        else if (GetAsyncKeyState(VK_F4) & 0x8000) {
            cinematDebug();  // Call the cinematDebug function
            Sleep(500);  // Prevent multiple triggers within a short time
        }
        else if (GetAsyncKeyState(VK_F5) & 0x8000) { //enable all equipment

            enableInventoryItem(localplayer, eInventoryProtonGun, true);
            enableInventoryItem(localplayer, eInventorySlimeGun, true);
            enableInventoryItem(localplayer, eInventoryRailgun, true);
            enableInventoryItem(localplayer, eInventoryShotgun, true);

            Sleep(500);
        }
        else if (GetAsyncKeyState(VK_F6) & 0x8000) { //enable all equipment
            ReloadAllLuaScripts();
            Sleep(500);
        }
        else if (GetAsyncKeyState(VK_F7) & 1) {
            enableDebug = !enableDebug;
            enableDebugMenu(enableDebug);  // This now toggles the patch on/off
            Sleep(500);
        }
        else if (GetAsyncKeyState('8') & 0x8000) {
            GetPlayerPosition();

            std::cout << "Player Position: ("
                << playerPos.x << ", "
                << playerPos.y << ", "
                << playerPos.z << ")" << std::endl;

            Sleep(500);  // Prevent multiple triggers within a short time
        }
        else if (GetAsyncKeyState('9') & 0x8000) {
            GetPlayerPosition();

            CreateNewActor("CGhostbuster", playerPos);

            std::cout << "Ghostbuster Spawned: ("
                << playerPos.x << ", "
                << playerPos.y << ", "
                << playerPos.z << ")" << std::endl;

            Sleep(500);  // Prevent multiple triggers within a short time
        }
        else if (GetAsyncKeyState('E') & 0x8000) {
            if (localplayer != 0) {  // Call the function only if localplayer value is set
                g_modBase = (char*)GetModuleHandle(NULL);  // Update g_modBase value
                //setFlashlightMode(localplayer, eFlashlightModeUVLight);
                toggleflashlight(localplayer, eFlashlightModeNormal); //the eFlashlightModeNormal is useless for toggleflashlight
                Sleep(500);  // Prevent multiple triggers within a short time
            }
        }
        else if (GetAsyncKeyState('Z') & 0x8000) {
            if (localplayer != 0) {  // call the function only if localplayer value is set
                // call IsTrapDeployed to check if a trap is deployed
                bool trapDeployed = isTrapDeployed(localplayer);
                if (trapDeployed) {
                    std::cout << "Trap is deployed!\n";
                }
                else {
                    std::cout << "Trap isn't deployed.\n";
                }
                Sleep(500);  // Prevent multiple triggers within a short time
            }
        }
        else if (GetAsyncKeyState('P') & 0x8000) {
            if (localplayer != 0) {  // Call the function only if localplayer value is set
                if (!fakePossessionStatus) {
                    fakePossession(localplayer, true);
                }
                else {
                    fakePossession(localplayer, false);
                }
                fakePossessionStatus = !fakePossessionStatus;
                Sleep(500);  // Prevent multiple triggers within a short time
            }
        }
        else if (GetAsyncKeyState('G') & 0x8000) {
            if (localplayer != 0) {  // Call the function only if localplayer value is set
                if (eGogglesStatus == 0) {
                    eGogglesStatus = eGogglesOnFace;
                    setGoggleLocation(localplayer, eGogglesOnFace);
                }
                else if (eGogglesStatus == 1) {
                    eGogglesStatus = eGogglesOnBelt;
                    setGoggleLocation(localplayer, eGogglesOnBelt);
                }
                else if (eGogglesStatus == 2) {
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

        for (char key = '0'; key <= '9'; ++key) {
            SHORT keyState = GetAsyncKeyState(key);
            bool isDown = (keyState & 0x8000) != 0;
            bool wasDown = keyStates[key];

            if (isDown && !wasDown) {
                for (auto L : activeLuaStates) {
                    DispatchLuaKeyEvent(L, "keyDown", key);
                }
            }
            else if (!isDown && wasDown) {
                for (auto L : activeLuaStates) {
                    DispatchLuaKeyEvent(L, "keyUp", key);
                }
            }

            keyStates[key] = isDown;
        }
        

        for (char key = 'A'; key <= 'Z'; ++key) {
            SHORT keyState = GetAsyncKeyState(key);
            bool isDown = (keyState & 0x8000) != 0;
            bool wasDown = keyStates[key];

            if (isDown && !wasDown) {
                for (auto L : activeLuaStates) {
                    DispatchLuaKeyEvent(L, "keyDown", key);
                }
            }
            else if (!isDown && wasDown) {
                for (auto L : activeLuaStates) {
                    DispatchLuaKeyEvent(L, "keyUp", key);
                }
            }

            keyStates[key] = isDown;
        }


        Sleep(10);  // Small delay to avoid high CPU usage
    }
}

//need to put this somewhere else takes too much of a space
void HandleInput()
{
    std::string input;
    while (runProgram)
    {
        std::cout << "> ";
        if (!getline(std::cin, input))
        {
            break;
        }

        size_t first = input.find_first_not_of(" \t");
        if (first == std::string::npos)
            continue; // Empty input
        input = input.substr(first, input.find_last_not_of(" \t") - first + 1);

        if (input == "slew")
        {
            Slew();
            std::cout << "Slew (Noclip) toggled.\n";
        }
        else if (input == "quitlevel")
        {
            quitLevel();
            std::cout << "Level Quitted.\n";
        }
        else if (input == "animdebug")
        {
            animDebug();
            std::cout << "Animation debug overlay function was called.\n";
        }
        else if (input == "cinematdebug")
        {
            cinematDebug();
            std::cout << "Cinemat debug overlay function was called.\n";
        }
        else if (input == "channeldebug")
        {
            chanDebug();
            std::cout << "Channel debug overlay function was called.\n";
        }
        else if (input == "cancelwalk")
        {
            CancelWalkAll();
            std::cout << "Walk is disabled on all AI??????.\n";
        }
        else if (input == "ghostviewer")
        {
            GhostViewer();
            std::cout << "Ghost Viewer toggled.\n";
        }
        else if (input == "setObjective")
        {
            std::cout << "Enter the objective description: ";
            std::string objective;
            std::getline(std::cin, objective);
            setObjective(objective.c_str());
            std::cout << "Objective set to: " << objective << "\n";
        }
        else if (input == "resetgravity")
        {
            resetgravity();
            std::cout << "Gravity Reseted.\n";
        }
        else if (input == "about")
        {
            AboutMod();
            std::cout << "About information toggled.\n";
        }
        else if (input == "playFX")
        {
            std::cout << "Enter the name of the effect: ";
            std::string effectName;
            std::getline(std::cin, effectName);

            const char* effectNameCStr = effectName.c_str();

            GetPlayerPosition();
            CacheEffect(&effectNameCStr);
            StartEffect(effectName.c_str(), playerPos, playerPos);

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
            std::cout << "Restarting level...\n";
        }
        else if (input == "reloadlua")
        {
            ReloadAllLuaScripts();
        }
        else if (input == "legacytext")
        {
            TestLegacyText();
            std::cout << "Legacy text display toggled.\n";
        }
        else if (input == "spawnactor")
        {
            GetPlayerPosition();
            CreateNewActor("CGhostbuster", playerPos);
            std::cout << "Spawn Actor toggled.\n";
        }
        else if (input == "survivalmode")
        {
            RunMod();
            //cout << "Survival Mode(Prototype).\n";
        }
        else if (input == "loadlevel")
        {
            std::cout << "Enter the level name: ";

            std::string levelName;
            std::getline(std::cin, levelName);

            GetPlayerPosition();

            loadLevel(levelName.c_str());
        }
        else if (input == "spawnactoroftype")
        {
            std::cout << "Enter the class to create: ";

            std::string className;
            std::getline(std::cin, className);

            GetPlayerPosition();

            CreateNewActor(className.c_str(), playerPos);
        }
        else if (input == "playCinemat")
        {
            std::cout << "Enter the name of the cinematic: ";

            std::string cinemat;
            std::getline(std::cin, cinemat);

            playCinemat(cinemat.c_str());
        }
        else if (input == "gbloaded")
        {
            if (localplayer != 0)
            {
                std::cout << "Player                  LOADED.\n";
            }
            else
            {
                std::cout << "Player                  NOT loaded.\n";
            }

            if (egon != 0)
            {
                std::cout << "Egon                    LOADED.\n";
            }
            else
            {
                std::cout << "Egon                    NOT loaded.\n";
            }

            if (winston != 0)
            {
                std::cout << "Winston                 LOADED.\n";
            }
            else
            {
                std::cout << "Winston                 NOT loaded.\n";
            }

            if (venkman != 0)
            {
                std::cout << "Venkman                 LOADED.\n";
            }
            else
            {
                std::cout << "Venkman                 NOT loaded.\n";
            }

            if (ray != 0)
            {
                std::cout << "Ray                     LOADED.\n";
            }
            else
            {
                std::cout << "Ray                     NOT LOADED.\n";
            }
        }
        else if (input == "ectoloaded")
        {
            if (ecto != 0)
            {
                std::cout << "Ecto                    LOADED.\n";
            }
            else
            {
                std::cout << "Ecto                    NOT loaded.\n";
            }

        }
        else if (input == "explosion")
        {
            std::cout << "Enter explosion parameters (x y z radius strength speed): ";
            float x, y, z, radius, strength, speed;
            if (std::cin >> x >> y >> z >> radius >> strength >> speed)
            {
                Vector position{ x, y, z };
                CreateExplosion(position, radius, strength, speed);
                std::cout << "Explosion created at (" << x << ", " << y << ", " << z
                    << ") with radius " << radius
                    << ", strength " << strength
                    << ", and speed " << speed << ".\n";
            }
            else
            {
                std::cin.clear(); // Clear the error state
                std::cin.ignore();
                std::cout << "Invalid parameters. Please enter numeric values.\n";
            }
        }
        else if (input == "createlight")
        {
            std::cout << "Enter radius, RGB color (r g b), intensity, duration, ramp-up, and ramp-down: ";
            float radius, r, g, b, intensity, duration, rampUp, rampDown;
            std::cin >> radius >> r >> g >> b >> intensity >> duration >> rampUp >> rampDown;

            if (!std::cin.fail())
            {
                GetPlayerPosition();
                Vector color{ r, g, b };

                AddLight(playerPos, radius, color, intensity, duration, rampUp, rampDown);
                std::cout << "Light added with radius " << radius
                    << ", color (" << r << ", " << g << ", " << b << "), intensity " << intensity
                    << ", duration " << duration << ", ramp-up " << rampUp << ", and ramp-down " << rampDown << ".\n";
            }
            else
            {
                std::cin.clear();
                std::cin.ignore();
                std::cout << "Invalid input. Please enter valid numeric values for the light parameters.\n";
            }
        }
        else if (input == "createffect")
        {
            std::cout << "Enter effect file name, position (x y z), and orientation (x y z): ";
            std::string effectFilename;
            float px, py, pz, ox, oy, oz;

            std::cin >> effectFilename >> px >> py >> pz >> ox >> oy >> oz;

            if (!std::cin.fail())
            {
                Vector position{ px, py, pz };
                Vector orientation{ ox, oy, oz };

                int result = StartEffect(effectFilename.c_str(), position, orientation);
                if (result >= 0)
                {
                    std::cout << "Effect '" << effectFilename << "' started at (" << px << ", " << py << ", " << pz
                        << ") with orientation (" << ox << ", " << oy << ", " << oz << ").\n";
                }
                else
                {
                    std::cout << "Failed to start effect '" << effectFilename << "' make sure you added .tfb at the end.\n";
                }
            }
            else
            {
                std::cin.clear();
                std::cin.ignore();
                std::cout << "Invalid input. Please enter a valid effect name, position, and orientation.\n";
            }
        }
        else if (input == "gravity")
        {
            std::cout << "Enter gravity parameters (x y z): ";
            float x, y, z;
            std::cin >> x >> y >> z;

            if (!std::cin.fail())
            {
                Vector position{ x, y, z };
                SetGravity(position);
                std::cout << "Set gravity with values (" << x << ", " << y << ", " << z << ").\n";
            }
            else
            {
                std::cin.clear();
                std::cin.ignore();
                std::cout << "Invalid input. Please enter a valid numeric coordinates.\n";
            }
        }
        else if (input.find("help") == 0)
        {
            std::cout << "Available commands:\n";
            std::cout << "  slew                  - Toggle Slew (Noclip)\n";
            std::cout << "  quitLevel             - Quit's the current level\n";
            std::cout << "  animdebug             - Enables the animation debug overlay\n";
            std::cout << "  cinematdebug          - Enables the cinematics debug overlay\n";
            std::cout << "  channeldebug          - Enables the channel debug overlay\n";
            std::cout << "  cancelwalk            - Disables the walk animation?\n";
            std::cout << "  ghostviewer           - Toggle Ghost Viewer\n";
            std::cout << "  about                 - Toggle About\n";
            std::cout << "  restart               - Restart Level\n";
            std::cout << "  gbloaded              - Check if Ghostbuster are loaded\n";
            std::cout << "  legacytext            - Toggle Legacy text display\n";
            std::cout << "  spawnactor            - Spawn Actor\n";
            std::cout << "  reloadlua             - Reload lua scripts\n";
            std::cout << "  playCinemat           - Play a cinematic\n";
            std::cout << "  explosion             - Creates Explosion\n";
            std::cout << "  createlight           - Create a light source at a given position\n";
            std::cout << "  createffect           - Create a effect at a given position and orientation\n";
            std::cout << "  resetgravity          - Reset the gravity\n";
            std::cout << "  loadlevel             - Load to a level\n";
            std::cout << "  spawnactoroftype      - Spawns an actor of your class choice\n";
            std::cout << "  survivalmode          - Starts Survival Mode(Prototype very broken)\n";
            std::cout << "  gravity               - Set gravity\n";
            std::cout << "  help                  - Show this help message\n";
            std::cout << "  exit                  - Close the console\n";
        }
        else if (input.find("hotkeys") == 0)
        {
            std::cout << "Available Hotkeys:\n";
            std::cout << "  F1                    - Toggle Slew (Noclip)\n";
            std::cout << "  F2                    - Enables the animation debug overlay\n";
            std::cout << "  F3                    - Enables the channel debug overlay\n";
            std::cout << "  F4                    - Enables the cinematics debug overlay\n";
            std::cout << "  F5                    - Unlock all weapons\n";
            std::cout << "  F6                    - Reload Lua\n";
            std::cout << "  F7                    - Enable debug menu\n";
            std::cout << "  8                     - Prints players position\n";
            std::cout << "  9                     - Spawn Ghostbuster at players position\n";
            std::cout << "  E                     - Toggle flashlight\n";
            std::cout << "  Q                     - Holster/Unholster proton wand\n";
            std::cout << "  Z                     - Returns true or false if trap is deployed\n";
            std::cout << "  P                     - Fake possess player\n";
            std::cout << "  G                     - Toggle ecto goggles position\n";
        }
        else if (input == "exit")
        {
            std::cout << "Exiting program.\n";
            FreeConsole();
            runProgram = false;
        }
        else if (input == "clear" || input == "cls")
        {
            system("cls");
            Sleep(10);
            std::cout << "*************************** \n"; //Re-print the infromation
            std::cout << "     IE17 is hooked! \n";
            std::cout << "*************************** \n";
            std::cout << "Version: " STR(IE17ver) "\n";
        }
        else
        {
            std::cout << "Unknown command. Type 'help' for a list of commands or 'hotkeys' for a list of hotkeys.\n";
        }
        Sleep(10);  // Small delay to avoid high CPU usage
    }
}

void LoadAllLuaScripts() {
    const std::string folder = "IE17_Mods";

    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.path().extension() == ".lua") {
            lua_State* L = luaL_newstate();
            luaL_openlibs(L);

            RegisterGameFunctions(L);

            //attempt to load and execute the Lua file
            std::string path = entry.path().string();
            if (luaL_dofile(L, path.c_str()) != LUA_OK) {
                std::string error = lua_tostring(L, -1);
                std::cerr << "[Lua Error] Failed to load " << path << ":\n" << error << "\n";
                std::cerr << "[Lua Error] " << path << ": " << error << "\n";

                lua_pop(L, 1);
                lua_close(L);
            }
            else {
                std::cout << "[Lua] Loaded: " << entry.path().filename() << "\n";

                CallLuaInitFunction(L);

                activeLuaStates.push_back(L);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
}

void ReloadAllLuaScripts() {
    //close all previous Lua states
    for (lua_State* L : activeLuaStates) {
        lua_close(L);
    }
    activeLuaStates.clear();

    //reload all scripts
    LoadAllLuaScripts();
}

void enableDebugMenu(bool enableDebug) {
    BYTE* debugFlag = reinterpret_cast<BYTE*>(g_modBase + 0x20D4915);
    *debugFlag = enableDebug ? 1 : 0;

    uintptr_t* basePtr = reinterpret_cast<uintptr_t*>(g_modBase + 0xDCF680);
    if (basePtr && *basePtr) {
        BYTE* secondFlag = reinterpret_cast<BYTE*>(*basePtr + 0x1D8);
        *secondFlag = enableDebug ? 1 : 0;
    }
    else {
        std::cerr << "Pointer chain is null.\n";
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

void CreateModsFolder()
{
    const std::string modsFolder = "IE17_Mods";

    if (!fs::exists(modsFolder)) {
        try {
            if (fs::create_directory(modsFolder)) {
                //std::cout << "Folder '" << modsFolder << "' created successfully\n";
            }
            else {
                //std::cout << "Failed to create folder '" << modsFolder << "'.\n";
            }
        }
        catch (const fs::filesystem_error&) {
            //std::cerr << "Filesystem error: " << e.what() << '\n';
        }
    }
    else {
        //std::cout << "Folder '" << modsFolder << "' already exists\n";
    }
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
    // Initialize MinHook
    if (MH_Initialize() != MH_OK) {
        MessageBoxA(NULL, "Failed to initialize MinHook.", "Error", MB_OK | MB_ICONERROR);
        return 0; // Abort injection
    }

    // Allocate console
    if (!AllocConsole()) {
        MessageBoxA(NULL, "Failed to allocate console.", "Error", MB_OK | MB_ICONERROR);
        return 0; // Abort injection
    }

    // Redirect console I/O
    if (freopen_s((FILE**)stdin, "CONIN$", "r", stdin) != 0 ||
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout) != 0) {
        MessageBoxA(NULL, "Failed to redirect console I/O.", "Error", MB_OK | MB_ICONERROR);
        FreeConsole();
        return 0; // Abort injection
    }


    // Set console title
    SetConsoleTitleA("IE17 Build " STR(IE17ver));

    // Print welcome message
    std::cout << "*************************** \n";
    std::cout << "     IE17 is hooked! \n";
    std::cout << "*************************** \n";
    std::cout << "Version: " STR(IE17ver) "\n";

    g_modBase = (char*)GetModuleHandle(NULL);
    startTalking = (unsigned __int64(*)(unsigned __int64, const char*))(g_modBase + 0x75BB0);
    startPreparedTalking = (unsigned __int64(*)(unsigned __int64, const char*, int))(g_modBase + 0x75A00);
    setGhostbusterHeatlhState = (char(*)(unsigned __int64, int))(g_modBase + 0xD0280);
    flinch = (unsigned __int64(*)(unsigned __int64))(g_modBase + 0xD18B0);
    Singleton_getRoom = (unsigned __int64(*)(unsigned __int64*))(g_modBase + 0x2BDBD0);
    setMusic = (unsigned __int64(*)(unsigned __int64))(g_modBase + 0x411FE0);
    hideHack = (unsigned __int64(*)(unsigned __int64*))(g_modBase + 0xEE2D0);
    removeSlimeDecals = (void(*)(unsigned __int64))(g_modBase + 0x30D620);
    dbNarrativeStop = (void(*)())(g_modBase + 0x1F8410);
    dbNarrative = (float(*)(unsigned __int64))(g_modBase + 0x1ECEB0);
    Singleton_newActor = (unsigned __int64(*)(const char*, Vector))(g_modBase + 0x2C0D50); //yes i know its a duplicate
    die = (void(*)(unsigned __int64))(g_modBase + 0x3B27B0); //CBreaker
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
    enable = (int(*)(unsigned __int64, unsigned __int64, bool))(g_modBase + 0x2DA340);
    setProtonBeamMaxLength = (void(*)(float))(g_modBase + 0x277A50); // min 10.0f  max 400.0f
    setAnimation = (void(*)(unsigned __int64, const char*, bool))(g_modBase + 0x77440);
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
    DisplayTextLegacy = (bool(*)(unsigned int, const char*, const char*))(g_modBase + 0x2A6C90); //int hudtype, const char* msgtittle, const char* msg, int ?(duration??)
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

    CreateModsFolder();
    SetTerminalOnTop();

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

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
    WaitForSingleObject(hThread, INFINITE);

    // Clean up
    MH_DisableHook(GlobalRegisterFunc1);
    MH_Uninitialize();
    fclose((FILE*)stdin);
    fclose((FILE*)stdout);
    FreeConsole();

    lua_close(L);

    FreeLibraryAndExitThread(hModule, 0);
    return 1;
}