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
 
char* g_modBase = nullptr;

//localization
const char* levelres{ "Level Restarted" };
const char* aboutbuild{ "IE17 " STR(IE17ver) " Compiled at : " STR(__DATE__) " in " STR(__TIME__) };
const char* about{ "IE17 is a project aimed to reverse enginner some functions from Ghostbusters The Video Game Remaster. by sakis720 " };
const char* crashedgametittle{ "Game Crashed" };
const char* crashedgame{ "If you are seeing this the game has crashed, this 'Legacy' print function is parsley broken" };

using _SlewFun = void(__cdecl*)();
using _GhostViewerFunc = void(__cdecl*)();

_SlewFun Slew;
_GhostViewerFunc GhostViewer;

//void (*ChainToLevel)(const char*);
//void (*knockBack)(Vector, float);
void (*WarpTo)(Vector, float);
void (*CacheEffect)(const char**);
int (*StartEffect)(const char*, Vector, Vector);
void (*SetLevelDescription)(const char**);
void (*CreateExplosion)(Vector, float, float, float);
void (*SetGravity)(Vector);
void (*AddLight)(Vector, float, Vector, float, float, float, float);
void (*CreateActor)(const char*, Vector);
int (*DisplayText)(int, const char*, float);
int (*DisplayTextLegacy)(int, const char*, const char*, char);

Vector CreateActorPos{ 3.35, 1.0, -22.22 }; //temporary coords, the cords are the player spawn cords for museum level at the docks //temporary coords, the cords are the player spawn cords for museum level at the docks
Vector LightRGB{ 5.35, 1.0, 40.22 };
Vector gravitytestforce{ 6.35, 2.0, 40.22 };
Vector tempEffectOrient{ 90 };

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
    Slew = (_SlewFun)(g_modBase + 0x1F9D50);
    GhostViewer = (_GhostViewerFunc)(g_modBase + 0x1F8360);

    while (true)
    {
        if (GetAsyncKeyState(VK_F2) & 1)
        {
            GhostViewer();
           //cout << "Ghost Viewer Function Was Called \n";
        }

        if (GetAsyncKeyState(VK_F1) & 1)
        {
            Slew();
            //cout << "Slew Function Was Called \n";
        }
        if (GetAsyncKeyState(VK_F3) & 1)
        {
            AboutMod();
        }
        if (GetAsyncKeyState(VK_F4) & 1)
        {
            ResLevel();
            //cout << "The Level has been restarted \n";
        }
        if (GetAsyncKeyState(VK_F5) & 1)
        {
            TestLegacyText();
            //cout << "Legacy Display was called. Game crashed :( \n";
        }
        if (GetAsyncKeyState(VK_F6) & 1)
        {
            SpawnActor();
            Sleep(1000); //Cheap method to make it wait until you can recall the function so the game doesn't crash
        }
        /*
        if (GetAsyncKeyState(VK_TAB) & 1)
        {
            break; //bgale to dll
        }
        */
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
    cout << "\n";
    cout << "Controls: \n";
    cout << "\n";
    cout << "Slew(Noclip): F1 \n";
    cout << "Ghost Viewer: F2 \n";
    cout << "About: F3 \n";
    cout << "Restart Level: F4 \n";
    cout << "Display Legacy Print (IT WILL CRASH YOUR GAME): F5 \n";
    cout << "Spawn Actor: F6 \n";

    g_modBase = (char*)GetModuleHandle(NULL); 
    //ChainToLevel = (void(*)(const char*))(g_modBase + 0x1EF700); 
    //knockBack = (void(*)(Vector, float))(g_modBase + 0xED100);
    WarpTo = (void(*)(Vector, float))(g_modBase + 0x2C4520); // *********************** broken for the time being ***********************
    CacheEffect = (void(*)(const char**))(g_modBase + 0x35A380); //filename
    StartEffect = (int(*)(const char*, Vector, Vector))(g_modBase + 0x35A730); // filename, pos, orient //needs cacheeffect to work
    SetLevelDescription = (void(*)(const char**))(g_modBase + 0x2D8820); // *********************** broken for the time being ***********************
    CreateExplosion = (void(*)(Vector, float, float, float))(g_modBase + 0x1E9170); //pos, radius, damageStrength, speed
    SetGravity = (void(*)(Vector))(g_modBase + 0x1ECC40); //sets gravity
    AddLight = (void(*)(Vector, float, Vector, float, float, float, float))(g_modBase + 0x1ECB20); //vector pos, float radius, vector rgb, float intensity, float duration, float rampUp = 0.0, float rampDown = 0.0
    CreateActor = (void(*)(const char*, Vector))(g_modBase + 0x2C0D50); //const char class, vector pos(x,y,z)
    DisplayText = (int(*)(int, const char*, float))(g_modBase + 0x2494A0); //hudtype msg duration
    DisplayTextLegacy = (int(*)(int, const char*, const char*, char))(g_modBase + 0x2A6C90); //int hudtype, const char* msgtittle, const char* msg, int ?(duration??)

    RunMod();

    MH_Uninitialize();
    fclose((FILE*)stdin);
    fclose((FILE*)stdout);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 1;
}