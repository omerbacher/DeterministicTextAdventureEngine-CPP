#include "SaveGame.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "Torch.h"

bool SAVE_MODE = false;

void SaveGame::run() // override run method helped by AI
{
    LOAD_MODE = false;
    while (currStatus != GameModes::EXIT)
    {
        if (currStatus == GameModes::MENU)
        {
            cls();
            clearInputBuffer();
            showMenu();
        }
        else if (currStatus == GameModes::INSTRUCTIONS)
        {
            showInstructions();
        }
        else if (currStatus == GameModes::NEW_GAME)
        {
            getAllScreenFileNames(screenFileNames);

			if (screenFileNames.empty()) // No screen files found
            {
                cls();
                std::cout << "ERROR: No screen files found (adv-world*.screen)!" << std::endl;
                waitForKey();
                currStatus = GameModes::MENU;
                continue;
            }

			allLevels.clear(); // Clear previous levels
            bool successLoad = true;
			for (const auto& fileName : screenFileNames) // Load all screen files
            {
                Screen tempScreen;
                if (!tempScreen.loadMapFromFile(fileName, (int)screenFileNames.size()))
                {
                    successLoad = false;
                    break;
                }
                allLevels.push_back(tempScreen);
            }

			if (!successLoad) // Failed to load screen files
            {
                std::cout << "\nERROR: Failed to load screen files!" << std::endl;
                std::cout << "Press any key to return to Menu..." << std::endl;
                waitForKey();
                currStatus = GameModes::MENU;
                continue;
            }

            resetGame();
            currentLevelIdx = 0;
            currentScreen = allLevels[currentLevelIdx];
            gameResults = Results();

            std::string screensString = "";
            for (size_t i = 0; i < screenFileNames.size(); i++)
            {
                if (i > 0) screensString += "|";
                screensString += screenFileNames[i];
            }
            gameResults.setScreenFiles(screensString);

            //  INITIALIZE STEPS RECORDING 
            recordedSteps.initForRecording(screenFileNames);
            riddleBank.attachSteps(&recordedSteps, false);
            //  INITIALIZE SCREEN CHANGE EVENT
            gameResults.addScreenChange(eventTimer, screenFileNames[currentLevelIdx]);

            initLevel(screenFileNames[currentLevelIdx]);
            saveGameLoop();  // Use custom game loop that records events
            cls();
            //  SAVE FILES AFTER GAME ENDS 
            gameResults.save("adv-world.result");
            recordedSteps.saveSteps("adv-world.steps"); // save recorded steps 
            cls();  // Clear the screen first
            gotoxy(18, 10);
            std::cout << "\n========================================" << std::endl;
            std::cout << "       GAME SAVED SUCCESSFULLY" << std::endl;
            std::cout << "  - adv-world.result" << std::endl;
            std::cout << "  - adv-world.steps" << std::endl;
            std::cout << "========================================\n" << std::endl;
            waitForKey();

            currStatus = GameModes::MENU;
        }
    }

    UIScreens::showExitMessage();
}

void SaveGame::saveGameLoop() // custom game loop that records events
{
    bool gameRunning = true;
    Point p1PosLastFrame = player1.getPosition();
    Point p2PosLastFrame = player2.getPosition();
    player1LastPos = p1PosLastFrame;
    player2LastPos = p2PosLastFrame;

	int timeAccumulator = 0;
    const DWORD timerInterval = 1000;
    redrawGame();

    while (gameRunning)
    {
        eventTimer++;

        // Update timer
        if (timerActive)
        {
			timeAccumulator += GAME_DELAY;
            if(timeAccumulator>=(int)timerInterval)
            {
                gameTimer--;
				timeAccumulator -= timerInterval;

                if (gameTimer <= 0)
                {
                    // Game over - time ran out
                    UIScreens::showGameOverMessage();
                    gameResults.addGameOver(eventTimer, player1.getScore(), player2.getScore());
                    resetGame();
                    currStatus = GameModes::MENU;
                    gameRunning = false;
                    break;
                }
            }
        }

        // Handle pause request from riddle
        if (pauseRequestedFromRiddle)
        {
            pauseRequestedFromRiddle = false;
            handlePause(currentScreen, gameRunning);
            clearInputBuffer();
            if (!gameRunning)
            {
                gameResults.addGameExit(eventTimer, player1.getScore(), player2.getScore());
                break;
            }
            redrawGame();
            p1PosLastFrame = player1.getPosition();
            p2PosLastFrame = player2.getPosition();
            continue;
        }

        // Handle input
        if (_kbhit())
        {
            char ch = _getch();

            if (ch == ESC)
            {
                handlePause(currentScreen, gameRunning);
                clearInputBuffer();
                if (!gameRunning)
                {
                    gameResults.addGameExit(eventTimer, player1.getScore(), player2.getScore());
                    break;
                }
                redrawGame();
                p1PosLastFrame = player1.getPosition();
                p2PosLastFrame = player2.getPosition();
                continue;
            }
            else
            {
                recordedSteps.addStepIfValid(eventTimer, ch, player1, player2); // Record valid player steps
                player1.keyPressed(ch);
                player2.keyPressed(ch);
            }
        }

        // Update game state
        updateBomb();
        updatePlayers();

        player1LastPos = p1PosLastFrame;
        player2LastPos = p2PosLastFrame;

        bool p1Moved = player1.isActive() &&
            ((p1PosLastFrame.getX() != player1.getPosition().getX()) ||
                (p1PosLastFrame.getY() != player1.getPosition().getY()));
        bool p2Moved = player2.isActive() &&
            ((p2PosLastFrame.getX() != player2.getPosition().getX()) ||
                (p2PosLastFrame.getY() != player2.getPosition().getY()));

        // Draw based on dark / torch state
        bool isDark = currentScreen.getRoomMeta().isDark();
        if (isDark)
        {
            if (Torch::playerHasTorch(player1))
            {
                currentScreen.drawMapWithTorch(player1);
            }
            else if (Torch::playerHasTorch(player2))
            {
                currentScreen.drawMapWithTorch(player2);
            }
        }

        drawActiveBomb();
        updateDisplay();

        // Check end conditions
        if (checkGameOver())
        {
            gameRunning = false;
            break;
        }

        // Handle door transitions
        if ((!player1.isActive() || !player2.isActive()) && activeDoor != ' ')
        {
            if (player1.isActive())
            {
                player1.rememberPosition();
                player1.erase();
            }
            if (player2.isActive())
            {
                player2.rememberPosition();
                player2.erase();
            }
            redrawGame();
            p1PosLastFrame = player1.getPosition();
            p2PosLastFrame = player2.getPosition();
            player1LastPos = p1PosLastFrame;
            player2LastPos = p2PosLastFrame;
            activeDoor = ' ';
            currentScreen.setDark(currentScreen.getRoomMeta().isDark());
        }

        p1PosLastFrame = player1.getPosition();
        p2PosLastFrame = player2.getPosition();

        Sleep(GAME_DELAY);
    }
}

bool SaveGame::checkGameOver() // override checkGameOver to record game over event
{
    if (player1.isDead() || player2.isDead())
    {
        UIScreens::showGameOverMessage();
        gameResults.addGameOver(eventTimer, player1.getScore(), player2.getScore());
        recordedSteps.addStep(eventTimer, 0, ' '); // ===== NEW: Record the "any key" press =====
        resetGame();
        currStatus = GameModes::MENU;
        return true;
    }

    if (checkLevel())
    {
        currStatus = GameModes::MENU;
        return true;
    }

    return false;
}

bool SaveGame::checkLevel() // override checkLevel to record screen changes
{
    if (!player1.isActive() && !player2.isActive())
    {
        allLevels[currentLevelIdx] = currentScreen;

        int doorId = activeDoor - '0';
        Door* door = currentScreen.getDoorById(doorId);
        if (!door) return false;

        int nextLevelIdx = door->getDestinationLevel();

        if (nextLevelIdx >= 0 && nextLevelIdx < (int)allLevels.size())
        {
            char p1Item = player1.getHeldItem();
            int p1ItemId = player1.getItemId();
            char p2Item = player2.getHeldItem();
            int p2ItemId = player2.getItemId();

            currentLevelIdx = nextLevelIdx;
            currentScreen = allLevels[currentLevelIdx];
            currentScreen.setDark(currentScreen.getRoomMeta().isDark());
            initLevel(screenFileNames[currentLevelIdx], doorId);

            //  RECORD SCREEN CHANGE 
            gameResults.addScreenChange(eventTimer, screenFileNames[currentLevelIdx]);

            if (p1Item != ' ' && p1Item != 0)
                player1.GrabItem(p1Item, p1ItemId);
            if (p2Item != ' ' && p2Item != 0)
                player2.GrabItem(p2Item, p2ItemId);
            activeDoor = ' ';
            return false;
        }
        else
        {
			clearInputBuffer();// clear input buffer before showing win screen
            // You won the game
            showWinScreen();
            //  RECORD GAME FINISHED 
            gameResults.addGameFinished(eventTimer, player1.getScore(), player2.getScore());
            recordedSteps.addStep(eventTimer, 0, ' '); // Record the "any key" press
			activeDoor = ' ';// reset active door for safety
            return true;
        }
    }
    return false;
}