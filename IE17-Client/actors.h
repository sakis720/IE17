#ifndef ACTORS_H
#define ACTORS_H

#include <vector>
#include <string>
#include <windows.h>

extern unsigned __int64 egon;
extern unsigned __int64 winston;
extern unsigned __int64 venkman;
extern unsigned __int64 ray;
extern unsigned __int64 ecto;
extern unsigned __int64 mainview;

void getGhostbusters(char* Buffer, __int64 adr1);
void getEcto(char* Buffer, __int64 adr1);
void getCMainView(char* Buffer, __int64 adr1);

#endif // ACTORS_H
