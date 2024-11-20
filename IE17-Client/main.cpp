#include "main.h"
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

bool g_fSlew = false;
bool m_about = false;
bool m_legacycrash = false;
bool b_spawnactor = false;
bool g_fGhostViewer = false;
bool g_fRestartLevel = false;
bool g_debugoptions = false;
 
char* g_modBase = nullptr;


//localization
const char* levelres{ "Level Restarted" };
const char* aboutbuild{ "IE17 " STR(IE17ver) " Compiled at : " STR(__DATE__) " in " STR(__TIME__) };
const char* about{ "IE17 is a project aimed to reverse enginner some functions from Ghostbusters The Video Game Remaster. by sakis720 " };
const char* crashedgametittle{ "Game Crashed" };
const char* crashedgame{ "If you are seeing this the game has crashed, this 'Legacy' print function is parsley broken" };


using _SlewFun = void(__cdecl*)();
using _GhostViewerFunc = void(__cdecl*)();
using _CancelWalkAll = void(__cdecl*)();
using _QuitLevel = void(__cdecl*)();
using _animation = void(__cdecl*)();
using _cinemat = void(__cdecl*)();
using _channels = void(__cdecl*)();


_SlewFun Slew;
_GhostViewerFunc GhostViewer;
_CancelWalkAll CancelWalkAll;
_QuitLevel quitLevel;
_animation animDebug;
_cinemat cinematDebug;
_channels chanDebug;


//void (*ChainToLevel)(const char*);
//void (*knockBack)(Vector, float);
void (*setTeam)(int, int);
void (*WarpTo)(Vector, const char*, Vector);
void (*CacheEffect)(const char**);
int (*StartEffect)(const char*, Vector, Vector);
void (*SetLevelDescription)(const char**);
void (*CreateExplosion)(Vector, float, float, float);
void (*SetGravity)(Vector);
void (*AddLight)(Vector, float, Vector, float, float, float, float);
void (*CreateActor)(const char*, Vector);
int (*DisplayText)(int, const char*, float);
int (*DisplayTextLegacy)(int, const char*, const char*, char);

Vector CreateActorPos{ 3.35f, 1.0f, -22.22f }; //temporary coords, the cords are the player spawn cords for museum level at the docks //temporary coords, the cords are the player spawn cords for museum level at the docks
Vector LightRGB{ 5.35f, 1.0f, 40.22f };



void HandleInput()
{
    Slew = (_SlewFun)(g_modBase + 0x1F9D50);
    GhostViewer = (_GhostViewerFunc)(g_modBase + 0x1F8360);
    CancelWalkAll = (_CancelWalkAll)(g_modBase + 0x1F81B0);
    quitLevel = (_QuitLevel)(g_modBase + 0x1F8170);
    animDebug = (_animation)(g_modBase + 0x1F7FB0);
    cinematDebug = (_cinemat)(g_modBase + 0x1F7FC0);
    chanDebug = (_channels)(g_modBase + 0x1F7FD0);

    string input;
    while (true)
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
        else if (input == "about")
        {
            AboutMod();
            cout << "About information toggled.\n";
        }
        else if (input == "restart")
        {
            ResLevel();
            cout << "Restarting level...\n";
        }
        else if (input == "legacy")
        {
            TestLegacyText();
            cout << "Legacy text display toggled.\n";
        }
        else if (input == "spawnactor")
        {
            SpawnActor();
            cout << "Spawn Actor toggled.\n";
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
            cout << "  restart               - Restart Level\n";
            cout << "  legacy                - Toggle Legacy text display\n";
            cout << "  spawnactor            - Toggle Spawn Actor\n";
            cout << "  explosion             - Creates Explosion\n";
            cout << "  spawnactorat          - Spawn Actor at a given position\n";
            cout << "  createlight           - Create a light source at a given position\n";
            cout << "  createffect           - Create a effect at a given position and orientation\n";
            cout << "  gravity               - Set gravity\n";
            cout << "  help                  - Show this help message\n";
            cout << "  exit                  - Close the console\n";
        }
        else if (input.find("exit") == 0)
        {
            cout << "Exiting console input thread.\n";
            FreeConsole();
            break;
        }
        else if (input == "clear" || input == "cls")
        {
            system("cls");
            Sleep(100);
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
        DisplayText(TEXT_HelpMessage, (message + to_string(i) + " sec").c_str(), 1.0f);
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
    DisplayText(TEXT_HelpMessage, levelres, 10.0f);

}

void AboutMod()
{
    m_about = !m_about;

    const char* setMsg = m_about ? about : aboutbuild;

    DisplayText(TEXT_HelpMessage, setMsg, 10.0f);
}

void SpawnActor()
{
    b_spawnactor = !b_spawnactor;

    const char* actorType = b_spawnactor ? "CSlimer" : "CGhostbuster";
    const char* messageType = b_spawnactor ? "Spawned Actor: Ghosts" : "Spawned Actor: Ghostbuster";

    //StartEffect("ghostbusters_down.tfb", CreateActorPos, tempEffectOrient);
    //SetLevelDescription("Testing");
    DisplayText(TEXT_HelpMessage, messageType, 1.5f);
    CreateActor(actorType, CreateActorPos);

}


void TestLegacyText()
{
    m_legacycrash = !m_legacycrash;

    DisplayTextLegacy(TEXTL_Default, crashedgametittle, crashedgame, 0);

}


void RunMod()
{
    /*
    Slew = (_SlewFun)(g_modBase + 0x1F9D50);
    GhostViewer = (_GhostViewerFunc)(g_modBase + 0x1F8360);
    CancelWalkAll = (_CancelWalkAll)(g_modBase + 0x1F81B0);
    quitLevel = (_QuitLevel)(g_modBase + 0x1F8170);
    animDebug = (_animation)(g_modBase + 0x1F7FB0);
    cinematDebug = (_cinemat)(g_modBase + 0x1F7FC0);
    chanDebug = (_channels)(g_modBase + 0x1F7FD0);

    while (true) {
        if (GetAsyncKeyState(VK_F1) & 1) Slew();
        if (GetAsyncKeyState(VK_F2) & 1) GhostViewer();
        if (GetAsyncKeyState(VK_F3) & 1) AboutMod();
        if (GetAsyncKeyState(VK_F4) & 1) ResLevel();
        if (GetAsyncKeyState(VK_F5) & 1) TestLegacyText();
        if (GetAsyncKeyState(VK_F6) & 1) {
            SpawnActor();
            Sleep(1000); // Prevent rapid calls
        }
    }
    */
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
    //ChainToLevel = (void(*)(const char*))(g_modBase + 0x1EF700); 
    //knockBack = (void(*)(Vector, float))(g_modBase + 0xED100);
    setTeam = (void(*)(int, int))(g_modBase + 0x15B40); //filename
    WarpTo = (void(*)(Vector, const char*, Vector))(g_modBase + 0x2C4520); // *********************** broken for the time being ***********************
    CacheEffect = (void(*)(const char**))(g_modBase + 0x35A380); //filename
    StartEffect = (int(*)(const char*, Vector, Vector))(g_modBase + 0x35A730); // filename, pos, orient //needs cacheeffect to work
    SetLevelDescription = (void(*)(const char**))(g_modBase + 0x2D8820); // *********************** broken for the time being ***********************
    CreateExplosion = (void(*)(Vector, float, float, float))(g_modBase + 0x1E9170); //pos, radius, damageStrength, speed
    SetGravity = (void(*)(Vector))(g_modBase + 0x1ECC40); //sets gravity
    AddLight = (void(*)(Vector, float, Vector, float, float, float, float))(g_modBase + 0x1ECB20); //vector pos, float radius, vector rgb, float intensity, float duration, float rampUp = 0.0, float rampDown = 0.0
    CreateActor = (void(*)(const char*, Vector))(g_modBase + 0x2C0D50); //const char class, vector pos(x,y,z)
    DisplayText = (int(*)(int, const char*, float))(g_modBase + 0x2494A0); //hudtype msg duration
    DisplayTextLegacy = (int(*)(int, const char*, const char*, char))(g_modBase + 0x2A6C90); //int hudtype, const char* msgtittle, const char* msg, int ?(duration??)

    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HandleInput, NULL, 0, NULL);
    if (!hThread) {
        MessageBoxA(NULL, "Failed to create input handling thread.", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    WaitForSingleObject(hThread, INFINITE);

    RunMod();

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