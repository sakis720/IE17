#include "main.h"
#include "gameconstants.h"
#include "player.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdint> 
#include <cstring>
#include <string>
#include <windows.h>

using namespace std;

bool m_about = false;
bool m_legacycrash = false;
bool b_spawnactor = false;
bool g_fRestartLevel = false;

// Flag to keep the main thread running
bool runProgram = true;
 
char* g_modBase = nullptr;

int playerCash = 0;

void (*setFlashlightMode)(unsigned __int64, int);
void (*commitSuicide)(unsigned __int64);
void (*setHealth)(unsigned __int64, float);
void (*setNothingEquipped)(unsigned __int64, bool);
void (*DanteVMaddExport)(const char*, const char*, int);
void (*loadcheckpoint)(const char**);
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
        }
        // You can check other key presses here in a similar manner
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
        else if (input == "spawnactorat")
        {
            cout << "Enter actor class and position (x y z): ";
            string actorType;
            float x, y, z;
            cin >> actorType >> x >> y >> z;

            if (!cin.fail())
            {
                Vector position{ x, y, z };
                CreateActor(actorType.c_str(), position);
                cout << "Spawned actor '" << actorType << "' at (" << x << ", " << y << ", " << z << ").\n";
            }
            else
            {
                cin.clear();
                cin.ignore();
                cout << "Invalid input. Please enter a valid actor class and numeric coordinates.\n";
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
                cout << "Invalid input. Please enter a valid actor class and numeric coordinates.\n";
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
            //cout << "  getlevel              - Prints the name of the current level\n";
            cout << "  restart               - Restart Level\n";
            cout << "  legacytext            - Toggle Legacy text display\n";
            cout << "  spawnactor            - Toggle Spawn Actor (Doesn't work currently)\n";
            cout << "  explosion             - Creates Explosion\n";
            cout << "  spawnactorat          - Spawn Actor at a given position\n";
            cout << "  createlight           - Create a light source at a given position\n";
            cout << "  createffect           - Create a effect at a given position and orientation\n";
            cout << "  resetgravity          - Reset the gravity\n";
            cout << "  survivalmode          - Starts Survival Mode(Prototype very broken)\n";
            cout << "  gravity               - Set gravity\n";
            cout << "  help                  - Show this help message\n";
            cout << "  exit                  - Close the console\n";
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
            cout << "Unknown command. Type 'help' for a list of commands.\n";
        }
    }
}

void TextDisplayCountdown(const char* message, int seconds)
{
    for (int i = seconds; i > 0; --i)
    {
        DisplayText(TEXT_Top, (message + to_string(i) + " sec").c_str(), 1.0f);
        Sleep(1000);
    }
}

void ResLevel()
{

    using ResLevelFunc = void(__cdecl*)();
    auto ResLevelFun = (ResLevelFunc)(g_modBase + 0x1F8180);

    g_fRestartLevel = !g_fRestartLevel;

    TextDisplayCountdown("Restarting Level In: ", 5);
    ResLevelFun();
    Sleep(7000);
    DisplayText(TEXT_HelpMessage, info::levelres, 10.0f);

}

void AboutMod()
{
    m_about = !m_about;
    
    const char* setMsg = m_about ? info::about : info::aboutbuild;

    DisplayText(TEXT_HelpMessage, setMsg, 10.0f);
}

void SpawnActor()
{
    b_spawnactor = !b_spawnactor;


    const char* actorType = b_spawnactor ? "CSlimer" : "CGhostbuster";
    const char* messageType = b_spawnactor ? "Spawned Actor: Ghosts" : "Spawned Actor: Ghostbuster";

    DisplayText(TEXT_HelpMessage, messageType, 1.5f);
    CreateActor(actorType, coordinates::CreateActorPos);

}

std::string GetCurLevel()
{
    uintptr_t pointeradr = 0x2C70030; //adr
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
        std::cout << "Not on a level." << std::endl; //Not on a level
        return "";
    }

    // Print the level name
    //std::cout << "Level Name : " << buffer << std::endl;

    return std::string(buffer); // return the level name as a std::string
}

void TestLegacyText()
{
    m_legacycrash = !m_legacycrash;

    DisplayTextLegacy(TEXTL_Default, errorMsg::crashedgametittle, errorMsg::crashedgame, 0);

}

void fadein() {

    float opacity = 255.0f;   
    float r = 1.0f, g = 1.0f, b = 1.0f;  // You can set any color values here
    float duration = 0.1f;    // Duration for the fade effect (can be set to a very low value)

    // decrease opacity quickly
    while (opacity > 0) {
        fade(opacity, r, g, b, duration);
        opacity -= 10.0f;  // decrease opacity by 10 (adjust it to your liking)
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    //make it 0.0f to make the game visible again
    fade(0.0f, r, g, b, duration);
}

void OpenShop(Vector GhostbusterSpawn) {
    DisplayText(TEXT_Top, "Welcome to the Shop!", 5.0f);
    Sleep(5000);
    DisplayText(TEXT_HelpMessage, "1. Call Help - $250", 5.0f);
    Sleep(5000);
    DisplayText(TEXT_HelpMessage, "2. Skip Shop - Proceed to Next Wave", 5.0f);
    Sleep(5000);

    const DWORD shopDuration = 20000;
    DWORD startTime = GetCurrentTime();
    int choice = -1;

    while (GetCurrentTime() - startTime < shopDuration) {
        DWORD remainingTime = shopDuration - (GetCurrentTime() - startTime);
        DisplayText(TEXT_Top, ("Shop closes in: " + to_string(remainingTime / 1000) + " seconds").c_str(), 1.0f);

        // check for player input
        choice = CheckPlayerInput();
        if (choice != -1) {
            break; // exit loop if player makes a choice
        }

        Sleep(500); // small delay to avoid spamming the display
    }

    // process the player choice or default to skipping the shop
    if (choice == -1) {
        DisplayText(TEXT_Top, "Time's up! No items purchased.", 3.0f);
        Sleep(3000);
    }
    else {
        HandleShopChoice(choice, GhostbusterSpawn);
    }
}

void HandleShopChoice(int choice, Vector pos) {
    switch (choice) {
    case 1: 
        if (playerCash >= 250) { // if player money over 100
            playerCash -= 250; //players cash - 100
            CreateActor("CGhostbuster", pos); // spawn actor
            DisplayText(TEXT_Top, "Help is here!", 3.0f);
            Sleep(3000);
        }
        else {
            DisplayText(TEXT_Top, "Not enough cash to call for help!", 3.0f); // no money no honey
            Sleep(3000);
        }
        break;

    case 2:
        DisplayText(TEXT_Top, "No items purchased. Get ready for the next wave!", 3.0f);
        Sleep(3000);
        break;

    default:
        DisplayText(TEXT_Top, "Invalid choice. Proceeding to the next wave.", 3.0f);
        Sleep(3000);
        break;
    }

    DisplayText(TEXT_Top, ("Remaining Cash: $" + to_string(playerCash)).c_str(), 3.0f);
    Sleep(3000);
}

int CheckPlayerInput() {

    if (GetAsyncKeyState('1') & 0x8000) {
        return 1;
    }
    if (GetAsyncKeyState('2') & 0x8000) { 
        return 2;
    }
    return -1; // No key pressed
}

void SetTerminalOnTop()
{
    HWND consoleWindow = GetConsoleWindow(); //get handle
    if (consoleWindow != nullptr)
    {
        SetWindowPos(consoleWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
}

void RunMod()
{
    /*
                                    SURVIVAL MODE PROTOTYPE
    This prototype was made to show what we can achieve from the current functions that have been reversed

    Note that this prototype is very w.i.p., and the coordinates that are displayed below(GhostSpawner1, GhostSpawner2, GhostSpawner3) 

    TODO:
    *Track the ghost that have been spawned, because now the waves end after 20 seconds
    *Find player coordinates
    *Make a clear state function that clears everything(Ghostbusters, Ghosts, Objects etc)
    */

    const std::string requiredLevelTimesSquare2 = "timessquare2.lvl"; // Target level name
    const std::string requiredLevelCemetery2 = "cemetery2.lvl"; // Target level name

    std::string currentLevel = GetCurLevel(); //get current level

    int wave = 1;
    int maxWaves = 10; 
    int baseGhostsPerWave = 1;  // initial ghost count per wave

    //const char* ghostTypes[] = { "CSlimer", "CBiped" }; //ghost type CSlime and CBiped
    //int ghostTypeCount = sizeof(ghostTypes) / sizeof(ghostTypes[0]);

    //check if the current level is the same as the required level timessquare2.lvl
    if (currentLevel == requiredLevelTimesSquare2) {
       // std::cout << "DEBUG: Level Times Sq 2" << std::endl;

        Vector GhostSpawner1{ 703.5f, 95.67f, -816.0f };
        Vector GhostSpawner2{ 719.5f, 95.54f, -907.5f };
        Vector GhostSpawner3{ 724.0f, 95.5f, -823.5f };

        Vector GhostSpawnerOrientation{ 90 };

        const char* effectname = "chief_spawn.tfb";

        fadein();
        CacheEffect(&effectname); //cache effect because if not the game will pop up a error

        while (wave <= maxWaves)
        {


            int ghostsToSpawn = baseGhostsPerWave + (wave - 1) * 1;  // increase ghost count with waves
            float spawnDelay = 1.0f;

            TextDisplayCountdown(("Wave " + to_string(wave) + " starting in: ").c_str(), 5);

            DisplayText(TEXT_Top, ("Wave " + to_string(wave) + " begins!").c_str(), 5.0f);

            //const char* selectedGhostType = ghostTypes[(wave - 1) / 2 % ghostTypeCount]; //select ghost type on based wave every 2 waves pick one

            //if ghost type CBiped increase the ghost spawn by 6
            //if (strcmp(selectedGhostType, "CBiped") == 0)
            //{
            //    ghostsToSpawn += 6;
            //}

            for (int i = 0; i < ghostsToSpawn; ++i)
            {
                // randomize spawn location (pick one of the spawners)
                Vector selectedSpawner;
                switch (rand() % 3) {
                case 0: selectedSpawner = GhostSpawner1; break;
                case 1: selectedSpawner = GhostSpawner2; break;
                case 2: selectedSpawner = GhostSpawner3; break;
                }


                CreateActor("CSlimer", selectedSpawner);
                StartEffect(effectname, selectedSpawner, GhostSpawnerOrientation);

                Sleep(static_cast<DWORD>(spawnDelay * 1000));
            }

            // if wave is 3 the waveDuration goes to 50 sec, and after 4 onwards all of the waves get 40 sec more
            DWORD waveDuration = 30000;
            if (wave == 4 || wave > 4)
            {
                waveDuration += 40000;
            }
            else if (wave == 3)
            {
                waveDuration += 20000;
            }


            Sleep(waveDuration);

            // increment wave
            ++wave;


            if (wave <= maxWaves)
            {
                DisplayText(TEXT_Top, "Prepare for the next wave!", 5.0f);
                Sleep(5000);
            }
        }
        
    }
    else if (currentLevel == requiredLevelCemetery2) {
        cout << "DEBUG: Level Cemetery 2 \n";

        Vector GhostSpawner1{ 82.21f, 13.24f, -144.59f };
        Vector GhostSpawner2{ 97.70f, 16.30f, -235.15f };
        Vector GhostSpawner3{ 51.86f, 16.21f, -215.04f };

        Vector GhostbusterSpawn{ 31.57f, 8.45f, -134.15f };

        Vector GhostSpawnerOrientation{ 90 };

        const char* effectname = "chief_spawn.tfb";


        fadein();
        CacheEffect(&effectname); //cache effect because if not the game will pop up a error
        cout << "DEBUG: Effect Cached\n";


        while (wave <= maxWaves)
        {


            int ghostsToSpawn = baseGhostsPerWave + (wave - 1) * 1;  // increase ghost count with waves
            float spawnDelay = 1.0f;

            TextDisplayCountdown(("Wave " + to_string(wave) + " starting in: ").c_str(), 5);

            DisplayText(TEXT_Top, ("Wave " + to_string(wave) + " begins!").c_str(), 5.0f);


            for (int i = 0; i < ghostsToSpawn; ++i)
            {
                // randomize spawn location (pick one of the spawners)
                Vector selectedSpawner;
                switch (rand() % 3) {
                case 0: selectedSpawner = GhostSpawner1; break;
                case 1: selectedSpawner = GhostSpawner2; break;
                case 2: selectedSpawner = GhostSpawner3; break;
                }


                CreateActor("CSlimer", selectedSpawner);
                StartEffect(effectname, selectedSpawner, GhostSpawnerOrientation);

                Sleep(static_cast<DWORD>(spawnDelay * 1000));
            }

            // if wave is 3 the waveDuration goes to 50 sec, and after 4 onwards all of the waves get 40 sec more
            DWORD waveDuration = 30000;
            if (wave == 4 || wave > 4)
            {
                waveDuration += 40000;
            }
            else if (wave == 3)
            {
                waveDuration += 20000;
            }


            Sleep(waveDuration);

            // increment wave
            ++wave;


            if (wave <= maxWaves)
            {
                int waveCompletionBonus = wave * 50;
                playerCash += waveCompletionBonus;

                // check if the current wave is the last wave (maxWaves)
                if (wave == maxWaves)
                {
                    DisplayText(TEXT_Top, "Survival Mode Complete! Well done.", 10.0f);
                }
                else
                {
                    DisplayText(TEXT_Top, ("Wave " + to_string(wave - 1) + " complete! Bonus: $" + to_string(waveCompletionBonus) + " | Total Cash: $" + to_string(playerCash)).c_str(), 5.0f);
                }

                Sleep(5000);

                OpenShop(GhostbusterSpawn);
                cout << "DEBUG: OpenShop must be called\n";

                DisplayText(TEXT_Top, "Prepare for the next wave!", 5.0f);
                Sleep(5000);
            }
        }
    }
    //if not the same level
    else {
        std::cout << "You are not on the correct level" << std::endl;
        std::cout << "Current Level: " << currentLevel << std::endl; //print current level example cemetery1.lvl
    }
   

}

DWORD WINAPI DLLAttach(HMODULE hModule)
{
    MH_Initialize();
    AllocConsole();
    SetConsoleTitleA("IE17 Build " STR(IE17ver));

    freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

    cout << "*************************** \n";
    cout << "     IE17 is hooked! \n";
    cout << "*************************** \n";
    cout << "Version: " STR(IE17ver) "\n";

    g_modBase = (char*)GetModuleHandle(NULL); 
    setFlashlightMode = (void(*)(unsigned __int64, int))(g_modBase + 0xE3BF0);
    commitSuicide = (void(*)(unsigned __int64))(g_modBase + 0xCE560);
    setHealth = (void(*)(unsigned __int64, float))(g_modBase + 0x7A890);
    setNothingEquipped = (void(*)(unsigned __int64, bool))(g_modBase + 0xE45A0);
    DanteVMaddExport = (void(*)(const char*, const char*, int))(g_modBase + 0x2CEC90);
    loadcheckpoint = (void(*)(const char**))(g_modBase + 0x2DD820);
    buttonPrompt = (void(*)(int, float))(g_modBase + 0x2494D0);
    setAllowDamageTally = (void(*)(bool*))(g_modBase + 0x76FD0);
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
    
    getPlayer();
    SetTerminalOnTop();
    // Start the key press detection in a separate thread
    thread keyPressThread(HandleKeyPresses);

    
    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HandleInput, NULL, 0, NULL);
    if (!hThread) {
        MessageBoxA(NULL, "Failed to create input handling thread.", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }


    WaitForSingleObject(hThread, INFINITE);


    // Ensure the key press thread finishes before exiting the program
    keyPressThread.join();

    if (!AllocConsole()) {
        MessageBoxA(NULL, "Failed to allocate console.", "Error", MB_OK | MB_ICONERROR);
        FreeConsole();
        return 0; // Abort injection
    }

    MH_Uninitialize();
    fclose((FILE*)stdin);
    fclose((FILE*)stdout);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 1;
}