#include "main.h"

bool g_fSlew = false;
bool g_fGhostViewer = false;
char* g_modBase = nullptr;

typedef void(__cdecl* _SlewFun)();
_SlewFun Slew;

typedef void(__cdecl* _GhostViewerFunc)();
_GhostViewerFunc GhostViewer;

void SlewEnableDisable()
{
    if (g_fSlew)
    {
        g_fSlew = false;
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

void RunMod()
{
    Slew = (_SlewFun)(g_modBase + 0x1F9D50);
    GhostViewer = (_GhostViewerFunc)(g_modBase + 0x1F8360);

    while (true)
    {
        if (GetAsyncKeyState(VK_BACK) & 1)
        {
            GhostViewer();
        }

        if (GetAsyncKeyState(VK_UP) & 1)
        {
            Slew();
        }
        if (GetAsyncKeyState(VK_TAB) & 1)
        {
            break; //bgale to dll
        }
    }
}

DWORD WINAPI DLLAttach(HMODULE hModule)
{
    MH_Initialize();
    g_modBase = (char*)GetModuleHandle(NULL);

    RunMod();

    MH_Uninitialize();
    FreeLibraryAndExitThread(hModule, 0);
    return 1;
}