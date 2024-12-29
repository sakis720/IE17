#include "main.h"
#include "enums.h"
#include "player.h"
#include "actors.h"
#include "script.h"
#include "gameconstants.h"
#include "functions.h"
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

void TextDisplayCountdown(const char* message, int seconds)
{
    for (int i = seconds; i > 0; --i)
    {
        DisplayText(TEXT_Top, (message + to_string(i) + " sec").c_str(), 1.0f);
        Sleep(1000);
    }
}

void ResLevel()
{

    using ResLevelFunc = void(__cdecl*)();
    auto ResLevelFun = (ResLevelFunc)(g_modBase + 0x1F8180);

    g_fRestartLevel = !g_fRestartLevel;

    TextDisplayCountdown("Restarting Level In: ", 5);
    ResLevelFun();
    Sleep(7000);
    DisplayText(TEXT_HelpMessage, info::levelres, 10.0f);

}

void AboutMod()
{
    m_about = !m_about;

    const char* setMsg = m_about ? info::about : info::aboutbuild;

    DisplayText(TEXT_HelpMessage, setMsg, 10.0f);
}

void SpawnActor()
{
    b_spawnactor = !b_spawnactor;

    GetPlayerPosition();

    DisplayText(TEXT_HelpMessage, "Spawned Actor: Ghostbuster", 1.5f);
    CreateActor("CGhostbuster", playerPos);

}

void TestLegacyText()
{
    m_legacycrash = !m_legacycrash;

    DisplayTextLegacy(TEXTL_Default, errorMsg::crashedgametittle, errorMsg::crashedgame, 0);

}

void fadein() {

    float opacity = 255.0f;
    float r = 1.0f, g = 1.0f, b = 1.0f;  // You can set any color values here
    float duration = 0.1f;    // Duration for the fade effect (can be set to a very low value)

    // decrease opacity quickly
    while (opacity > 0) {
        fade(opacity, r, g, b, duration);
        opacity -= 10.0f;  // decrease opacity by 10 (adjust it to your liking)
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    //make it 0.0f to make the game visible again
    fade(0.0f, r, g, b, duration);
}

void OpenShop(Vector GhostbusterSpawn) {
    DisplayText(TEXT_Top, "Welcome to the Shop!", 5.0f);
    Sleep(5000);
    DisplayText(TEXT_HelpMessage, "1. Call Help - $250", 5.0f);
    Sleep(5000);
    DisplayText(TEXT_HelpMessage, "2. Skip Shop - Proceed to Next Wave", 5.0f);
    Sleep(5000);

    const ULONGLONG shopDuration = 20000; // Use ULONGLONG for 64-bit values.
    ULONGLONG startTime = GetTickCount64(); // Use GetTickCount64 instead of GetTickCount.
    int choice = -1;

    while (GetTickCount64() - startTime < shopDuration) { // Use GetTickCount64 for comparisons.
        ULONGLONG remainingTime = shopDuration - (GetTickCount64() - startTime);
        DisplayText(TEXT_Top, ("Shop closes in: " + to_string(remainingTime / 1000) + " seconds").c_str(), 1.0f);

        // check for player input
        choice = CheckPlayerInput();
        if (choice != -1) {
            break; // exit loop if player makes a choice
        }

        Sleep(500); // small delay to avoid spamming the display
    }

    // process the player choice or default to skipping the shop
    if (choice == -1) {
        DisplayText(TEXT_Top, "Time's up! No items purchased.", 3.0f);
        Sleep(3000);
    }
    else {
        HandleShopChoice(choice, GhostbusterSpawn);
    }
}

void HandleShopChoice(int choice, Vector pos) {
    switch (choice) {
    case 1:
        if (playerCash >= 250) { // if player money over 100
            playerCash -= 250; //players cash - 100
            CreateActor("CGhostbuster", pos); // spawn actor
            DisplayText(TEXT_Top, "Help is here!", 3.0f);
            Sleep(3000);
        }
        else {
            DisplayText(TEXT_Top, "Not enough cash to call for help!", 3.0f); // no money no honey
            Sleep(3000);
        }
        break;


    case 2:
        DisplayText(TEXT_Top, "No items purchased. Get ready for the next wave!", 3.0f);
        Sleep(3000);
        break;

    default:
        DisplayText(TEXT_Top, "Invalid choice. Proceeding to the next wave.", 3.0f);
        Sleep(3000);
        break;
    }

    DisplayText(TEXT_Top, ("Remaining Cash: $" + to_string(playerCash)).c_str(), 3.0f);
    Sleep(3000);
}

int CheckPlayerInput() {

    if (GetAsyncKeyState('1') & 0x8000) {
        return 1;
    }
    if (GetAsyncKeyState('2') & 0x8000) {
        return 2;
    }
    return -1; // No key pressed
}

void RunMod()
{
    /*
                                    SURVIVAL MODE PROTOTYPE
    This prototype was made to show what we can achieve from the current functions that have been reversed

    Note that this prototype is very w.i.p., and the coordinates that are displayed below(GhostSpawner1, GhostSpawner2, GhostSpawner3)

    TODO:
    *Track the ghost that have been spawned, isDead function seems to be the way to go for this only need to find the ghosts adress and not muanually set it.
    *Make a clear state function that clears everything(Ghostbusters, Ghosts, Objects etc)
    */

    const std::string requiredLevelTimesSquare2 = "timessquare2.lvl"; // Target level name
    const std::string requiredLevelCemetery2 = "cemetery2.lvl"; // Target level name

    std::string currentLevel = GetCurLevel(); //get current level

    int wave = 1;
    int maxWaves = 10;
    int baseGhostsPerWave = 1;  // initial ghost count per wave

    bool state = false;

    setProtonBeamMaxLength(400.0f);

    setAllowDamageTally(&state);

    //const char* ghostTypes[] = { "CSlimer", "CBiped" }; //ghost type CSlime and CBiped
    //int ghostTypeCount = sizeof(ghostTypes) / sizeof(ghostTypes[0]);

    //check if the current level is the same as the required level timessquare2.lvl
    if (currentLevel == requiredLevelTimesSquare2) {
        // std::cout << "DEBUG: Level Times Sq 2" << std::endl;

        Vector GhostSpawner1{ 703.5f, 95.67f, -816.0f };
        Vector GhostSpawner2{ 719.5f, 95.54f, -907.5f };
        Vector GhostSpawner3{ 724.0f, 95.5f, -823.5f };

        Vector GhostSpawnerOrientation{ 90 };

        const char* effectname = "chief_spawn.tfb";

        fadein();
        CacheEffect(&effectname); //cache effect because if not the game will pop up a error

        while (wave <= maxWaves)
        {


            int ghostsToSpawn = baseGhostsPerWave + (wave - 1) * 1;  // increase ghost count with waves
            float spawnDelay = 1.0f;

            TextDisplayCountdown(("Wave " + to_string(wave) + " starting in: ").c_str(), 5);

            DisplayText(TEXT_Top, ("Wave " + to_string(wave) + " begins!").c_str(), 5.0f);

            //const char* selectedGhostType = ghostTypes[(wave - 1) / 2 % ghostTypeCount]; //select ghost type on based wave every 2 waves pick one

            //if ghost type CBiped increase the ghost spawn by 6
            //if (strcmp(selectedGhostType, "CBiped") == 0)
            //{
            //    ghostsToSpawn += 6;
            //}

            for (int i = 0; i < ghostsToSpawn; ++i)
            {
                // randomize spawn location (pick one of the spawners)
                Vector selectedSpawner;
                switch (rand() % 3) {
                case 0: selectedSpawner = GhostSpawner1; break;
                case 1: selectedSpawner = GhostSpawner2; break;
                case 2: selectedSpawner = GhostSpawner3; break;
                default: selectedSpawner = GhostSpawner1; // Fallback in case something unexpected happens.
                }


                CreateActor("CSlimer", selectedSpawner);
                StartEffect(effectname, selectedSpawner, GhostSpawnerOrientation);

                Sleep(static_cast<DWORD>(spawnDelay * 1000));
            }

            // if wave is 3 the waveDuration goes to 50 sec, and after 4 onwards all of the waves get 40 sec more
            DWORD waveDuration = 30000;
            if (wave == 4 || wave > 4)
            {
                waveDuration += 40000;
            }
            else if (wave == 3)
            {
                waveDuration += 20000;
            }


            Sleep(waveDuration);

            // increment wave
            ++wave;


            if (wave <= maxWaves)
            {
                DisplayText(TEXT_Top, "Prepare for the next wave!", 5.0f);
                Sleep(5000);
            }
        }

    }
    else if (currentLevel == requiredLevelCemetery2) {
        cout << "DEBUG: Level Cemetery 2 \n";

        Vector GhostSpawner1{ 82.21f, 13.24f, -144.59f };
        Vector GhostSpawner2{ 97.70f, 16.30f, -235.15f };
        Vector GhostSpawner3{ 51.86f, 16.21f, -215.04f };

        Vector GhostbusterSpawn{ 31.57f, 8.45f, -134.15f };

        Vector playerSpawn{ 26.29f, 7.23f, -122.33f };
        Vector playerOrient{ 26.29f, 7.23f, -122.33f };

        Vector GhostSpawnerOrientation{ 90 };

        const char* ani = "put_on_pack";

        const char* effectname = "chief_spawn.tfb";

        warpTo(localplayer, playerSpawn, playerOrient);
        setGoggleLocation(localplayer, eGogglesOnFace);
        readyInventoryItem(localplayer, eInventoryNothing, true);

        setAnimation(localplayer, ani, false, false);
        //setAnimation(localplayer, "amb_scratch_pke", false, false);


        toggleReviveMode(ray, false);

        fadein();
        CacheEffect(&effectname); //cache effect because if not the game will pop up a error
        cout << "DEBUG: Effect Cached\n";


        while (wave <= maxWaves)
        {
            //doesn't work because in the current level our companion Ray is alive so it will never say isDead until Ray is dead too.
            bool isplayeralive = isDead(localplayer);
            if (isplayeralive)
            {
                //DisplayText(TEXT_Top, "Player is Dead, Ending Game", 5.0f);
                cout << "DEBUG: Player is Dead\n";
                //break;
            }
            else
            {
                cout << "DEBUG: Player is Alive\n";
            }


            readyInventoryItem(localplayer, eInventoryProtonGun, true);

            int ghostsToSpawn = baseGhostsPerWave + (wave - 1) * 1;  // increase ghost count with waves
            float spawnDelay = 1.0f;

            TextDisplayCountdown(("Wave " + to_string(wave) + " starting in: ").c_str(), 5);

            DisplayText(TEXT_Top, ("Wave " + to_string(wave) + " begins!").c_str(), 5.0f);


            for (int i = 0; i < ghostsToSpawn; ++i)
            {
                // randomize spawn location (pick one of the spawners)
                Vector selectedSpawner;
                switch (rand() % 3) {
                case 0: selectedSpawner = GhostSpawner1; break;
                case 1: selectedSpawner = GhostSpawner2; break;
                case 2: selectedSpawner = GhostSpawner3; break;
                default: selectedSpawner = GhostSpawner1; // Fallback in case something unexpected happens.
                }


                CreateActor("CSlimer", selectedSpawner);
                StartEffect(effectname, selectedSpawner, GhostSpawnerOrientation);

                Sleep(static_cast<DWORD>(spawnDelay * 1000));
            }


            // if wave is 3 the waveDuration goes to 50 sec, and after 4 onwards all of the waves get 40 sec more
            DWORD waveDuration = 30000;
            if (wave == 4 || wave > 4)
            {
                waveDuration += 40000;
            }
            else if (wave == 3)
            {
                waveDuration += 20000;
            }


            Sleep(waveDuration);

            // increment wave
            ++wave;


            if (wave <= maxWaves)
            {
                int waveCompletionBonus = wave * 50;
                playerCash += waveCompletionBonus;

                // check if the current wave is the last wave (maxWaves)
                if (wave == maxWaves)
                {
                    cacheSkeletalAnimationByName("amb_wave_to_crowd");
                    setAnimation(localplayer, "amb_wave_to_crowd", false, false);
                    DisplayText(TEXT_Top, "Survival Mode Complete! Well done.", 10.0f);
                    readyInventoryItem(localplayer, eInventoryNothing, true);
                    Sleep(10000);
                    readyInventoryItem(localplayer, eInventoryProtonGun, true);
                }
                else
                {
                    DisplayText(TEXT_Top, ("Wave " + to_string(wave - 1) + " complete! Bonus: $" + to_string(waveCompletionBonus) + " | Total Cash: $" + to_string(playerCash)).c_str(), 5.0f);
                }

                Sleep(5000);

                OpenShop(GhostbusterSpawn);
                cout << "DEBUG: OpenShop must be called\n";

                DisplayText(TEXT_Top, "Prepare for the next wave!", 5.0f);
                Sleep(5000);
            }
        }
    }
    //if not the same level
    else {
        chainToLevel(localplayer, "cemetery2.lvl", "Underground");
        Sleep(2000);
        cout << "You have to call survivalmode again.\n";
    }


}