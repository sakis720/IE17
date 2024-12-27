#include "main.h"
#include "player.h"
#include "enums.h"
#include "actors.h"
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

void script()
{

	const char* cinemat06 = "cs_cem_01.cinemat";

	cacheStreamingCinemat(&cinemat06);

	Sleep(3000);

	cueStreamingCinemat(cinemat06, 0.0f);

	std::string msg = std::string("Playing cinematic: ") + cinemat06;
	DisplayText(TEXT_HelpMessage, msg.c_str(), 5.0f);

	playStreamingCinemat(cinemat06);
	
}