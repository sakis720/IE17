#include "main.h"

bool g_fSlew = false;
char* g_modBase = nullptr;

typedef void(__cdecl* _FunctionOne)();
_FunctionOne Slew;


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

void RunMod()
{
    Slew = (_FunctionOne)(g_modBase + 0x1F9D50);

    while (true)
    {

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