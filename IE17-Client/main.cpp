#include "main.h"
#include <stdio.h>
#include <iostream>

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
        DisplayText(TEXT_HelpMessage, "IE17 " STR(IE17ver) " Compiled at : " STR(__DATE__) " in " STR(__TIME__), 150.0f);
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

    g_modBase = (char*)GetModuleHandle(NULL);
    DisplayText = (int(*)(int, const char*, float))(g_modBase + 0x2494A0); //hudtype msg duration
    DisplayTextLegacy = (int(*)(int, const char*, const char*, char))(g_modBase + 0x2A6C90);

    RunMod();

    MH_Uninitialize();
    fclose((FILE*)stdin);
    fclose((FILE*)stdout);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 1;
}