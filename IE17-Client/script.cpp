#include "main.h"
#include "player.h"
#include "enums.h"
#include "actors.h"
#include "script.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include <thread>
#include <cstdint> 
#include <filesystem> 
#include <cstring>
#include <string>
#include <algorithm>
#include <windows.h>

using namespace std;

//created script.cpp to have a clean cpp file and for testing functions.

#include <chrono>

void script()
{
	bool state = false;

	setAllowDamageTally(&state);
}