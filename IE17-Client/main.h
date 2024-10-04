#pragma once
#include <windows.h>
#include "include/MinHook.h"
#include <cstdio>

enum EHudTypes
{
    TEXT_DebugText = 0,
    TEXT_GenericText = 1,
    TEXT_SpiritGuideUpdated = 2,
    TEXT_ObjectivesUpdated = 3,
    TEXT_HelpMessage = 13,
};

extern bool g_fSlew;
extern bool m_about;
extern bool g_fGhostViewer;
extern bool g_fRestartLevel;
extern char* g_modBase;

extern int (*DisplayText)(int, const char*, float);

DWORD WINAPI DLLAttach(HMODULE hModule);
//void SlewEnableDisable();
void AboutMod();
void ResLevel();
//void GhostViewerFun();
void RunMod();