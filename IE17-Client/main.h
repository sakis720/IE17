#pragma once
#include <windows.h>
#include "include/MinHook.h"
#include <cstdio>

extern bool g_fSlew;
extern char* g_modBase;

DWORD WINAPI DLLInject(HMODULE hModule);
void SlewEnableDisable();
void RunMod();