#ifndef IMCON_H
#define IMCON_H

#include <windows.h>
#include <cstdio>
#include <string>
#include <vector>

extern std::vector<std::string> consoleLogs;
extern std::string currentInput;
extern bool showWindow;
extern bool showColumnWindow;
extern bool showCinematicTab;
extern bool showMenu;


void Log(const std::string& message);
void LogFormatted(const char* format, ...);
void HandleCommand(const std::string& command);
void DrawConsole();
void DrawCollum();
void CinematicTab();
void DrawMenu();
float GetMemoryUsage();


#endif // IMCON_H
