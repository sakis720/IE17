#include "main.h"
#include <stdio.h>

using namespace std;

bool g_fSlew = false;
bool m_about = false;
bool m_legacycrash = false;
bool g_fGhostViewer = false;
bool g_fRestartLevel = false;
char* g_modBase = nullptr;

typedef void(__cdecl* _SlewFun)();
_SlewFun Slew;

typedef void(__cdecl* _GhostViewerFunc)();
_GhostViewerFunc GhostViewer;


int (*DisplayText)(int, const char*, float);
int (*DisplayTextLegacy)(int, const char*, const char*, char);


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
        DisplayText(TEXT_HelpMessage, "Level Has Been Restarted", 10.0f);
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
        DisplayText(TEXT_HelpMessage, "Level Has Been Restarted", 10.0f);
    }
}

void AboutMod()
{
    if (m_about)
    {
        m_about = false;
        DisplayText(TEXT_HelpMessage, "IE17 v0.02 Compiled at: Oct 5", 150.0f);
    }
    else
    {
        m_about = true;
        DisplayText(TEXT_HelpMessage, "IE17 is a project aimed to reverse enginner some functions from Ghostbusters The Video Game Remaster. by sakis720 ", 10.0f);
    }
}


void TestLegacyText()
{
    if (m_legacycrash)
    {
        m_legacycrash = false;
        DisplayTextLegacy(TEXT_Default, "Game Crashed", "If you are seeing this the game has crashed, this 'Legacy' print function is parsley broken", 0);
    }
    else
    {
        m_legacycrash = true;
        DisplayTextLegacy(TEXT_Default, "Game Crashed", "If you are seeing this the game has crashed, this 'Legacy' print function is parsley broken", 0);
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
        }

        if (GetAsyncKeyState(VK_F1) & 1)
        {
            Slew();
        }
        if (GetAsyncKeyState(VK_F3) & 1)
        {
            AboutMod();
        }
        if (GetAsyncKeyState(VK_F4) & 1)
        {
            ResLevel();
        }
        if (GetAsyncKeyState(VK_F5) & 1)
        {
            TestLegacyText();
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
    g_modBase = (char*)GetModuleHandle(NULL);
    DisplayText = (int(*)(int, const char*, float))(g_modBase + 0x2494A0); //hudtype msg duration
    DisplayTextLegacy = (int(*)(int, const char*, const char*, char))(g_modBase + 0x2A6C90);

    RunMod();

    MH_Uninitialize();
    FreeLibraryAndExitThread(hModule, 0);
    return 1;
}