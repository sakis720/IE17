#pragma once
#include <windows.h>
#include "include/MinHook.h"
#include <cstdio>

extern bool g_fSlew;
extern bool g_fGhostViewer;
extern char* g_modBase;

DWORD WINAPI DLLAttach(HMODULE hModule);
void SlewEnableDisable();
void GhostViewerFun();
void RunMod();