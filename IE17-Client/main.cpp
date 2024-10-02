#include "main.h"
#include <stdio.h>

using namespace std;

bool g_fSlew = false;
bool m_about = false;
bool g_fGhostViewer = false;
bool g_fRestartLevel = false;
char* g_modBase = nullptr;

typedef void(__cdecl* _SlewFun)();
_SlewFun Slew;

typedef void(__cdecl* _GhostViewerFunc)();
_GhostViewerFunc GhostViewer;


int (*DisplayText)(int, const char*);


void ResLevel()
{

    typedef void(__cdecl* _ResLevelFunc)();
    _ResLevelFunc ResLevelFun;
    ResLevelFun = (_ResLevelFunc)(g_modBase + 0x1F8180);

    if (g_fRestartLevel)
    {
        g_fRestartLevel = false;
        ResLevelFun();
        Sleep(7000);
        DisplayText(TEXT_HelpMessage, "Level Has Been Restarted");
    }
    else
    {
        g_fRestartLevel = true;
        ResLevelFun();
        Sleep(7000);
        DisplayText(TEXT_HelpMessage, "Level Has Been Restarted");
    }
}


void AboutMod()
{
    if (m_about)
    {
        m_about = false;
        DisplayText(TEXT_GenericText, "IE17 v0.02 Compiled at: Oct 3 2024");
    }
    else
    {
        m_about = true;
        DisplayText(TEXT_HelpMessage, "IE17 is a project aimed to reverse enginner some functions from Ghostbusters The Video Game Remaster. by sakis720 ");
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
    DisplayText = (int(*)(int, const char*))(g_modBase + 0x2494A0);

    RunMod();

    MH_Uninitialize();
    FreeLibraryAndExitThread(hModule, 0);
    return 1;
}