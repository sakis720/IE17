#include "main.h"
#include <stdio.h>
#include <iostream>

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

typedef void(__cdecl* _SlewFun)();
_SlewFun Slew;

typedef void(__cdecl* _GhostViewerFunc)();
_GhostViewerFunc GhostViewer;

//void (*ChainToLevel)(const char*);
void (*SetGravity)(Vector);
void (*AddLight)(Vector, float, Vector, float, float, float, float);
void (*CreateActor)(const char*, Vector);
int (*DisplayText)(int, const char*, float);
int (*DisplayTextLegacy)(int, const char*, const char*, char);

Vector CreateActorPos{ 3.35, 1.0, -22.22 }; //temporary coords, the cords are the player spawn cords for museum level at the docks //temporary coords, the cords are the player spawn cords for museum level at the docks
Vector LightRGB{ 5.35, 1.0, 40.22 };
Vector gravitytestforce{ 6.35, 2.0, 40.22 };


void ResLevel()
{

    typedef void(__cdecl* _ResLevelFunc)();
    _ResLevelFunc ResLevelFun;
    ResLevelFun = (_ResLevelFunc)(g_modBase + 0x1F8180);

    if (g_fRestartLevel)
    {
        g_fRestartLevel = false;
        DisplayText(TEXT_HelpMessage, "Restarting Level In: 5 sec", 1.0f);
        Sleep(1000);
        DisplayText(TEXT_HelpMessage, "Restarting Level In: 4 sec", 1.0f);
        Sleep(1000);
        DisplayText(TEXT_HelpMessage, "Restarting Level In: 3 sec", 1.0f);
        Sleep(1000);
        DisplayText(TEXT_HelpMessage, "Restarting Level In: 2 sec", 1.0f);
        Sleep(1000);
        DisplayText(TEXT_HelpMessage, "Restarting Level In: 1 sec", 1.0f);
        Sleep(1000);
        ResLevelFun();
        Sleep(7000);
        DisplayText(TEXT_HelpMessage, levelres, 10.0f);
    }
    else
    {
        g_fRestartLevel = true;
        DisplayText(TEXT_HelpMessage, "Restarting Level In: 5 sec", 1.0f);
        Sleep(1000);
        DisplayText(TEXT_HelpMessage, "Restarting Level In: 4 sec", 1.0f);
        Sleep(1000);
        DisplayText(TEXT_HelpMessage, "Restarting Level In: 3 sec", 1.0f);
        Sleep(1000);
        DisplayText(TEXT_HelpMessage, "Restarting Level In: 2 sec", 1.0f);
        Sleep(1000);
        DisplayText(TEXT_HelpMessage, "Restarting Level In: 1 sec", 1.0f);
        Sleep(1000);
        ResLevelFun();
        Sleep(7000);
        DisplayText(TEXT_HelpMessage, levelres, 10.0f);
    }
}

void AboutMod()
{

    if (m_about)
    {
        m_about = false;
        DisplayText(TEXT_HelpMessage, aboutbuild, 150.0f);
    }
    else
    {
        m_about = true;
        DisplayText(TEXT_HelpMessage, about, 10.0f);
    }
}

void SpawnActor()
{
    if (b_spawnactor)
    {
        b_spawnactor = false;
        DisplayText(TEXT_HelpMessage, "Spawned Actor: Ghostbuster", 1.5f);
        CreateActor("CGhostbuster", CreateActorPos);
        SetGravity(gravitytestforce);
        //ChainToLevel("hotel1a.lvl");
        //AddLight(CreateActorPos, 100, LightRGB, 40, 20, 1.0, 2.0);
        //cout << "Spawned ACTOR DEBUG \n"; 
    }
    else
    {
        b_spawnactor = true;
        DisplayText(TEXT_HelpMessage, "Spawned Actor: Ghosts", 1.5f);
        CreateActor("CSlimer", CreateActorPos);
        SetGravity(gravitytestforce);
        //AddLight(CreateActorPos, 100, LightRGB, 40, 20, 1.0, 2.0);
        //cout << "Spawned ACTOR DEBUG \n";
    }
}


void TestLegacyText()
{
    if (m_legacycrash)
    {
        m_legacycrash = false;
        DisplayTextLegacy(TEXTL_Default, crashedgametittle, crashedgame, 0);
    }
    else
    {
        m_legacycrash = true;
        DisplayTextLegacy(TEXTL_Default, crashedgametittle, crashedgame, 0);
    }
}

/*

void SlewEnableDisable()
{
    if (g_fSlew)
    {
        g_fSlew = false;
        DisplayText(TEXT_GenericText, "IE17 v0.01 Compiled at: Oct 1 2024");
    }
    else
    {
        g_fSlew = true;
    }
}

void GhostViewerFun()
{
    if (g_fGhostViewer)
    {
        g_fGhostViewer = false;
    }
    else
    {
        g_fGhostViewer = true;
    }
}

*/

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
            //cout << "Legacy Display was called. Game crashed :( \n";
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
    //ChainToLevel = (void(*)(const char*))(g_modBase + 0x1EF700); //hudtype msg duration
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