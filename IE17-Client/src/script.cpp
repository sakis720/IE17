#include "script.h"
#include "main.h"
#include "enums.h"
#include <iostream>

//created script.cpp to have a clean cpp file and for testing functions.

void script()
{
    const char* aggelosgay = "O Aggelos einai gay";

    setCurrentObjective(&aggelosgay);
    Sleep(10000);

    DisplayText(TEXT_Top, aggelosgay, 5.0f);

    Sleep(6000);

    displaySplashScreen("ui\\aggelos.tga", 20.0f, true, true);
}