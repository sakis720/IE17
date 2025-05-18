#include "main.h"
#include "functions.h"
#include "enums.h"
#include "gameconstants.h"
#include "actors.h"
#include "player.h"
#include <iostream>
#include <thread>
#include <unordered_set>
#include <chrono>
#include <vector>


typedef unsigned __int64(__cdecl* SINGLETON_newActor)(const char* className, Vector position);

typedef unsigned __int64(__cdecl* SINGLETON_getRoom)(unsigned __int64* actor);

std::unordered_set<unsigned __int64> activeGhosts;

void TextDisplayCountdown(const char* message, int seconds)
{
    for (int i = seconds; i > 0; --i)
    {
        DisplayText(TEXT_Top, (message + std::to_string(i) + " sec").c_str(), 1.0f);
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

void TestLegacyText()
{
    m_legacycrash = !m_legacycrash;

    DisplayTextLegacy(TEXTL_Default, errorMsg::crashedgametittle, errorMsg::crashedgame);

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

void setObjective(const char* objective)
{
    // Set the current objective
    setCurrentObjective(&objective);
}

void OpenShop(Vector GhostbusterSpawn) {
    DisplayText(TEXT_Top, "Welcome to the Shop!", 5.0f);
    Sleep(5000);
    DisplayText(TEXT_HelpMessage, "1. Call Help - $250", 5.0f);
    Sleep(2000);
    DisplayText(TEXT_HelpMessage, "2. Skip Shop - Proceed to Next Wave", 3.0f);
    Sleep(3000);

    const ULONGLONG shopDuration = 10000; // Use ULONGLONG for 64-bit values.
    ULONGLONG startTime = GetTickCount64(); // Use GetTickCount64 instead of GetTickCount.
    int choice = -1;

    while (GetTickCount64() - startTime < shopDuration) { // Use GetTickCount64 for comparisons.
        ULONGLONG remainingTime = shopDuration - (GetTickCount64() - startTime);
        DisplayText(TEXT_Top, ("Shop closes in: " + std::to_string(remainingTime / 1000) + " seconds").c_str(), 1.0f);

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
            CreateNewActor("CGhostbuster", pos); // spawn actor
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

    DisplayText(TEXT_Top, ("Remaining Cash: $" + std::to_string(playerCash)).c_str(), 3.0f);
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

void unlockall(unsigned __int64 actor)
{
    enableInventoryItem(actor, eInventoryProtonGun, true);
    enableInventoryItem(actor, eInventorySlimeGun, true);
    enableInventoryItem(actor, eInventoryRailgun, true);
    enableInventoryItem(actor, eInventoryShotgun, true);
}

void knockBackActor(unsigned __int64 actor)
{
    GetPlayerPosition();
    knockBack(actor, playerPos, 30.0f);
}

void warpToPlayer(unsigned __int64 actor)
{
    GetPlayerPosition();
    warpTo(actor, playerPos, playerPos);
}

void playAnim(unsigned __int64 actor, const char* anim)
{
    setAnimation(actor, anim, false);
}

void loadLevel(const char* level)
{
    chainToLevel(localplayer, level, "");
}

void slimeGB(unsigned __int64 actor)
{
    slimeMe(actor, true, 20.0f);
}

void gbLoaded()
{
    if (localplayer != 0)
    {
        std::cout << "Player             LOADED\n";
    }
    else
    {
        std::cout << "Player             NOT LOADED\n";
    }

    if (egon != 0)
    {
        std::cout << "Egon               LOADED\n";
    }
    else
    {
        std::cout << "Egon               NOT LOADED\n";
    }

    if (venkman != 0)
    {
        std::cout << "Venkman            LOADED\n";
    }
    else
    {
        std::cout << "Venkman            NOT LOADED\n";
    }

    if (winston != 0)
    {
        std::cout << "Winston            LOADED\n";
    }
    else
    {
        std::cout << "Winston            NOT LOADED\n";
    }

    if (ray != 0)
    {
        std::cout << "Ray                LOADED\n";
    }
    else
    {
        std::cout << "Ray                NOT LOADED\n";
    }
}

unsigned __int64 CreateNewActor(const char* className, Vector wPos)
{
    bool debug = false;

    unsigned __int64 addr = Singleton_newActor(className, wPos);

    if (debug)
    {
        std::cout << "Actor " << className << " created at address: 0x" << std::hex << addr << std::dec << std::endl;
    }

    // return the address
    return addr;
}

void playCinemat(const char* cinemat)
{
    cacheStreamingCinemat(&cinemat);

    Sleep(1000);

    cueStreamingCinemat(cinemat, 0.0f);
    playStreamingCinemat(cinemat);
}

unsigned __int64 getRoom(unsigned __int64* actor)
{
    bool debug = true;

    //need fixing returning error "Called on NULL object"
    unsigned __int64 roomName = Singleton_getRoom(actor);

    std::cout << "Actor: " << std::hex << actor << " is in the room: " << std::hex << roomName << std::endl;

    // return the address
    return roomName;
}

bool AreAllGhostsDefeated()
{
    for (auto it = activeGhosts.begin(); it != activeGhosts.end();)
    {
        bool debug = false;
        bool dead = isDead(*it);

        if (debug)
        {
            std::cout << "Checking ghost at address: 0x" << std::hex << *it << std::dec << " | Is dead: " << dead << std::endl;
        }

        if (dead)
        {
            it = activeGhosts.erase(it); // Remove defeated ghosts from the set
        }
        else
        {
            ++it;
        }
    }
    return activeGhosts.empty(); // Return true if no ghosts are left
}

void RunMod()
{
    /*
                                    SURVIVAL MODE PROTOTYPE
    This prototype was made to show what we can achieve from the current functions that have been reversed

    Note that this prototype is very w.i.p., and the coordinates that are displayed below(GhostSpawner1, GhostSpawner2, GhostSpawner3)

    TODO:
    *Make a clear state function that clears everything(Ghostbusters, Ghosts, Objects etc)
    */

    const std::string requiredLevelCemetery2 = "cemetery2.lvl"; // Target level name

    std::string currentLevel = GetCurLevel(); // Get current level

    int wave = 1;
    int baseGhostsPerWave = 1;  // Initial ghost count per wave

    bool debugstrings = false;

    bool state = false;

    // For tracking play time
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    std::chrono::time_point<std::chrono::steady_clock> currentTime;
    double playTimeSeconds = 0.0;

    setProtonBeamMaxLength(400.0f);
    setAllowDamageTally(&state);

    if (currentLevel == requiredLevelCemetery2)
    {
        Vector GhostSpawner1{ 82.21f, 13.24f, -144.59f };
        Vector GhostSpawner2{ 97.70f, 16.30f, -235.15f };
        Vector GhostSpawner3{ 51.86f, 16.21f, -215.04f };

        Vector GhostbusterSpawn{ 31.57f, 8.45f, -134.15f };
        Vector playerSpawn{ 26.29f, 7.23f, -122.33f };
        Vector playerOrient{ 26.29f, 7.23f, -122.33f };

        Vector GhostSpawnerOrientation{ 90 };

        const char* ani = "put_on_pack";
        const char* effectname = "chief_spawn.tfb";
        const char* msg = "Welcome to Survival Mode";

        setCurrentObjective(&msg);
        warpTo(localplayer, playerSpawn, playerOrient);
        setGoggleLocation(localplayer, eGogglesOnFace);
        readyInventoryItem(localplayer, eInventoryNothing, true);
        setAnimation(localplayer, ani, false);

        toggleReviveMode(ray, false);

        fadein();
        CacheEffect(&effectname); // Cache effect to avoid errors

        // Start the timer
        startTime = std::chrono::steady_clock::now();

        // Function to display survival stats
        auto displaySurvivalStats = [&]() {
            // Calculate minutes and seconds
            int minutes = static_cast<int>(playTimeSeconds) / 60;
            int seconds = static_cast<int>(playTimeSeconds) % 60;

            // Format the time string
            std::string timeStr = std::to_string(minutes) + "m " + std::to_string(seconds) + "s";

            // Display stats
            /*
            std::cout << "Game Over" << std::endl;
            std::cout << "Waves Survived: " << wave - 1 << std::endl;
            std::cout << "Survival Time: " << timeStr << std::endl;
            std::cout << "Total Cash: $" << playerCash << std::endl;
            std::cout << "Total Ghosts Defeated: " << activeGhosts.size() << std::endl;
            */
            };

        // Create a separate thread for checking player status
        std::thread playerStatusThread([&]() {
            while (true) {
                // Update playtime
                currentTime = std::chrono::steady_clock::now();
                playTimeSeconds = std::chrono::duration<double>(currentTime - startTime).count();

                // Check if player is dead
                bool isPlayerAlive = !isDead(localplayer);
                if (!isPlayerAlive) {
                    displaySurvivalStats();
                    return; // Exit the thread
                }

                // Sleep for 500ms (0.5 seconds)
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            });

        // Detach the thread so it runs independently
        playerStatusThread.detach();

        // Infinite wave loop (removed maxWaves limit)
        while (true)
        {
            bool isPlayerAlive = !isDead(localplayer);
            if (!isPlayerAlive) {
                displaySurvivalStats();
                break;
            }

            readyInventoryItem(localplayer, eInventoryProtonGun, true);

            int ghostsToSpawn = baseGhostsPerWave + (wave - 1) * 1;  // Increase ghost count with waves
            float spawnDelay = 1.0f;

            TextDisplayCountdown(("Wave " + std::to_string(wave) + " starting in: ").c_str(), 5);
            DisplayText(TEXT_Top, ("Wave " + std::to_string(wave) + " begins!").c_str(), 5.0f);

            for (int i = 0; i < ghostsToSpawn; ++i)
            {
                // Check player status before each ghost spawn
                if (isDead(localplayer)) {
                    displaySurvivalStats();
                    return;
                }

                // Randomize spawn location (pick one of the spawners)
                Vector selectedSpawner;
                switch (rand() % 3) {
                case 0: selectedSpawner = GhostSpawner1; break;
                case 1: selectedSpawner = GhostSpawner2; break;
                case 2: selectedSpawner = GhostSpawner3; break;
                default: selectedSpawner = GhostSpawner1; // Fallback in case something unexpected happens.
                }

                unsigned __int64 ghostAddress = CreateNewActor("CSlimer", selectedSpawner);

                // add the ghost to the tracking list
                activeGhosts.insert(ghostAddress);
                //std::cout << "Ghost added to tracking list. Address: 0x" << std::hex << ghostAddress << std::dec << std::endl;

                StartEffect(effectname, selectedSpawner, GhostSpawnerOrientation);

                Sleep(static_cast<DWORD>(spawnDelay * 1000));
            }

            if (debugstrings)
            {
                std::cout << "Number of active ghosts: " << activeGhosts.size() << std::endl;
            }

            // wait until all ghosts are defeated or player dies
            while (!AreAllGhostsDefeated())
            {
                if (isDead(localplayer)) {
                    displaySurvivalStats();
                    return;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Check every 100ms
            }

            // Calculate current play time for wave completion message
            currentTime = std::chrono::steady_clock::now();
            playTimeSeconds = std::chrono::duration<double>(currentTime - startTime).count();
            int minutes = static_cast<int>(playTimeSeconds) / 60;
            int seconds = static_cast<int>(playTimeSeconds) % 60;
            std::string timeStr = std::to_string(minutes) + "m " + std::to_string(seconds) + "s";

            // Wave completed, give rewards
            int waveCompletionBonus = wave * 50;
            playerCash += waveCompletionBonus;

            DisplayText(TEXT_Top, ("Wave " + std::to_string(wave) + " complete! Bonus: $" +
                std::to_string(waveCompletionBonus) +
                "@@newline@@Total Cash: $" + std::to_string(playerCash) +
                "@@newline@@Time: " + timeStr).c_str(), 5.0f);

            Sleep(5000);

            // Prepare for next wave
            OpenShop(GhostbusterSpawn);
            DisplayText(TEXT_Top, "Prepare for the next wave!", 5.0f);
            Sleep(5000);

            // Increment wave counter for next wave
            ++wave;
        }
    }
    else
    {
        chainToLevel(localplayer, "cemetery2.lvl", "Underground");
        Sleep(2000);
        std::cout << "You have to call survivalmode again.\n";
    }
}