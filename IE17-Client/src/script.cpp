#include "script.h"
#include "main.h"
#include "enums.h"
#include "player.h"
#include <iostream>

//created script.cpp to have a clean cpp file and for testing functions.

void script()
{
	if (localplayer != 0)
	{
		GetPlayerPackValue();
		GetPlayerPosition();
		GetPlayerHealth();

		std::cout << "Player X: " << playerPos.x << " Y: " << playerPos.y << " Z: " << playerPos.z << std::endl;
	}
	else
	{
		std::cout << "Player is null or in the main menu." << std::endl;
	}
}