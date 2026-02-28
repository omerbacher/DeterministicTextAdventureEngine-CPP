#include "LoadGame.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <filesystem>
#include "Torch.h"
#include "Key.h"
#include "Switch.h"
#include "Door.h"
#include <sstream>// for silent mode comparison (AI helper)

bool LOAD_MODE = false;

void LoadGame::run() // override run method helped by AI
{
    LOAD_MODE = true;
	if (!isSilentMode) // if not silent mode, show load mode message
    {
        cls();
        std::cout << "\n========================================" << std::endl;
        std::cout << "       LOAD MODE - Game Replay" << std::endl;
        std::cout << "========================================\n" << std::endl;
        Sleep(1000);
    }

    // Load expected results file
    Results expectedResults;
    try
    {
        expectedResults.load("adv-world.result");
    }
    catch (const std::exception&)
    {
        if (!isSilentMode)
        {
            std::cout << "ERROR: Could not load adv-world.result!" << std::endl;
            std::cout << "Make sure you ran -save mode first." << std::endl;
        }
        else
        {
            std::cout << "ERROR: adv-world.result not found!" << std::endl;
        }
        return;
    }

    // Check if adv-world.steps exists
    loadedSteps = Steps::loadSteps("adv-world.steps");
    if (!loadedSteps.hasMoreSteps())
    {
        if (!isSilentMode)
        {
            std::cout << "ERROR: Could not load adv-world.steps!" << std::endl;
            std::cout << "Steps file not found. Make sure you ran -save mode first." << std::endl;
        }
        else
        {
            std::cout << "ERROR: adv-world.steps not found!" << std::endl;
        }
        return;
    }
    loadedSteps.resetReplay();

    // Load all screen files from directory
    getAllScreenFileNames(screenFileNames);

    if (screenFileNames.empty())
    {
        if (!isSilentMode)
        {
            std::cout << "ERROR: No screen files found!" << std::endl;
        }
        else
        {
            std::cout << "ERROR: No screen files found!" << std::endl;
        }
        return;
    }

    // Load all level screens
    allLevels.clear();
    for (const auto& fileName : screenFileNames)
    {
        Screen tempScreen;
        if (!tempScreen.loadMapFromFile(fileName, (int)screenFileNames.size()))
        {
            if (!isSilentMode)
            {
                std::cout << "ERROR: Could not load screen file: " << fileName << std::endl;
            }
            else
            {
                std::cout << "ERROR: Could not load screen file: " << fileName << std::endl;
            }
            return;
        }
        allLevels.push_back(tempScreen);
    }

    // Initialize game state
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
    gameResults.addScreenChange(eventTimer, screenFileNames[currentLevelIdx]);

    // Initialize level - only draw if NOT in silent mode
    if (!isSilentMode)
    {
        initLevel(screenFileNames[currentLevelIdx]);
    }
    else
    {
        initLevelSilent(screenFileNames[currentLevelIdx]);
    }

    riddleBank.attachSteps(&loadedSteps, true);
	std::streambuf* oldCoutBuffer = nullptr;// for silent mode (helped by AI)
	std::stringstream trashStream;// for silent mode
    // Tell RiddleBank if we're in SILENT mode
    if (isSilentMode)
    {
        riddleBank.setSilentMode(true);
		oldCoutBuffer = std::cout.rdbuf(trashStream.rdbuf());// redirect cout to trash
    }

    // Run game loop
    replayGameLoop();

    // ===== SILENT MODE: Save to temp, compare, then delete =====
    if (isSilentMode)
    {
		std::cout.rdbuf(oldCoutBuffer);// restore cout

        // Save to temporary file
        gameResults.save("adv-world.result.tmp");

        // Load the temp file to compare
        Results tempResults;
        try
        {
            tempResults.load("adv-world.result.tmp");
           
        }
        catch (const std::exception&)
        {
            std::cout << "ERROR: Could not load temp results file!" << std::endl;
            return;
        }
        Screen::setSilentMode(false);
        cls();
        gotoxy(0, 0);

        std::string failureReason;
        bool testPassed = tempResults.compareWith(expectedResults, failureReason);

        if (testPassed)
        {
           
            std::cout << "TEST PASSED: Game replay matches expected results!" << std::endl;
        }
        else
        {
            std::cout << "TEST FAILED: Game replay does NOT match expected results!" << std::endl;
            std::cout << failureReason << std::endl;
        }

         //Clean up temp file
        try
        {
            std::filesystem::remove("adv-world.result.tmp");
        }
        catch (const std::exception&)
        {
            // If cleanup fails, it's not critical
        }
    }
	else // Non-silent mode: show completion message
    {
        cls();
        gotoxy(18, 10);
        std::cout << "\n========================================" << std::endl;
        std::cout << "       GAME REPLAY COMPLETED" << std::endl;
        std::cout << "========================================\n" << std::endl;
        waitForKey();
    }
}

void LoadGame::initLevelSilent(const std::string& filename, int specificDoor) // initialize level without drawing
{
	currentRoomMeta = currentScreen.getRoomMeta(); // get current room metadata
	currentScreen.resetTorchState(); // reset torch state

	if (currentRoomMeta.hasKeyPosition()) // checks key from metadata if exists
    {
		int doorID = currentRoomMeta.getKeyDoorID(); // get door ID for key
		if (doorID != -1 && !Door::openDoors[doorID] && player1.getItemId() != doorID && player2.getItemId() != doorID) // check if door is closed and players don't have the key
        {
			Key::placeFromMetadata(currentScreen); // place key from metadata
        }
    }

	currentRoomMeta.placeLightSwitchFromMetadata(currentScreen); // place light switch from metadata

    if (specificDoor == -1 && currentLevelIdx == 0)
    {
        gameTimer = maxGameTime;
        timerActive = true;
    }
	// Attach necessary components
    riddleBank.attachPositionToRoom(currentScreen);
    riddleBank.attachResults(&gameResults, &eventTimer);

    player1.attachResults(&gameResults, &eventTimer, PlayerType::Player1);
    player2.attachResults(&gameResults, &eventTimer, PlayerType::Player2);

    player1.setSilentMode(true);
    player2.setSilentMode(true);

    player1.setScreen(currentScreen);
    player2.setScreen(currentScreen);

    if (!player1.isActive())
        player1.activate();
    if (!player2.isActive())
        player2.activate();

    player1.setPosition(Point(P1_START_X, P1_START_Y, Direction::directions[Direction::STAY], '&'));
    player2.setPosition(Point(P2_START_X, P2_START_Y, Direction::directions[Direction::STAY], '$'));

    if (!Switch::exists(currentScreen))
        Door::allSwitchesAreOn();
}

void LoadGame::gameLoop() // override game loop
{
    replayGameLoop();
}

void LoadGame::replayGameLoop() // main replay game loop
{
    bool gameRunning = true;
    Point p1PosLastFrame = player1.getPosition();
    Point p2PosLastFrame = player2.getPosition();
    player1LastPos = p1PosLastFrame;
    player2LastPos = p2PosLastFrame;
	int timeAccumulator = 0;
	bool wasDark = currentScreen.getRoomMeta().isDark();
    if (!isSilentMode)
    {
		redrawGame(); // Draw full screen at start
    }

    while (gameRunning)
    {
        eventTimer++;
        // Update timer
        if (timerActive)
        {
            timeAccumulator += GAME_DELAY;
            if (timeAccumulator>=1000)
            {
                gameTimer--;
                timeAccumulator -= 1000;

                if (gameTimer <= 0)
                {
                    gameResults.addGameOver(eventTimer, player1.getScore(), player2.getScore());
                    gameRunning = false;
                    break;
                }
            }
        }

        Steps::Step currentStep;
		while (loadedSteps.getNextStep(eventTimer, currentStep)) // process all steps for the current timer tick
        {
            if (currentStep.PlayerNum == 0)
            {
				// if the key is 'h' or 'H', exit the game
                if (currentStep.key == 'h' || currentStep.key == 'H')
                {
					gameResults.addGameExit(eventTimer, player1.getScore(), player2.getScore()); // log game exit
                    gameRunning = false;
					break; // exit the loop
                }
            }
            else {
                char key = currentStep.key;
                player1.keyPressed(key);
                player2.keyPressed(key);
            }
        }
        if (!gameRunning) break;
        // Ignore all user input in LOAD mode, but check for automatic steps
        if (_kbhit())
        {
			auto i = _getch(); // discard input
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

        // Render only if not in silent mode
        if (!isSilentMode)
        {
            bool isNowDark = currentScreen.getRoomMeta().isDark();
            if(wasDark&&!isNowDark)
            {
                // Room just became lit
                currentScreen.drawMap();
			}
            wasDark = isNowDark;
            if (isNowDark)
            {
                if (Torch::playerHasTorch(player1))
                    currentScreen.drawMapWithTorch(player1);
                else if (Torch::playerHasTorch(player2))
                    currentScreen.drawMapWithTorch(player2);
            }

            drawActiveBomb();
            updateDisplay();

            Sleep(20);
        }

        // Check end conditions
        if (checkGameOver())
        {
            gameRunning = false;
        }

        // Handle door transitions
        if ((!player1.isActive() || !player2.isActive()) && activeDoor != ' ')
        {
            if (player1.isActive())
            {
                player1.rememberPosition();
                if (!isSilentMode)
                    player1.erase();
            }
            if (player2.isActive())
            {
                player2.rememberPosition();
                if (!isSilentMode)
                    player2.erase();
            }

            if (!isSilentMode)
            {
                redrawGame();
            }

            p1PosLastFrame = player1.getPosition();
            p2PosLastFrame = player2.getPosition();
            player1LastPos = p1PosLastFrame;
            player2LastPos = p2PosLastFrame;
            activeDoor = ' ';
            currentScreen.setDark(currentScreen.getRoomMeta().isDark());
        }

        p1PosLastFrame = player1.getPosition();
        p2PosLastFrame = player2.getPosition();
       
    }
}

bool LoadGame::checkGameOver() // override check game over
{
	if (player1.isDead() || player2.isDead()) // check if any player is dead
    {
		if (!isSilentMode) // show game over message only if not in silent mode
        {
            UIScreens::showGameOverMessage();
        }
        gameResults.addGameOver(eventTimer, player1.getScore(), player2.getScore());
        resetGame();
        return true;
    }

    if (checkLevel())
    {
        return true;
    }

    return false;
}

bool LoadGame::checkLevel() // override check level completion
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

            if (!isSilentMode)
            {
                initLevel(screenFileNames[currentLevelIdx], doorId);
            }
            else
            {
				
                // Fix: Properly initialize the level logic even in silent mode
                
                initLevelSilent(screenFileNames[currentLevelIdx], doorId);
                placePlayersAtEntrance(doorId);

            }

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
            // You won the game
            if (!isSilentMode)
            {
                showWinScreen();
            }

            gameResults.addGameFinished(eventTimer, player1.getScore(), player2.getScore());

            return true;
        }
    }
    return false;
}

void LoadGame::setSilentMode(bool isSilent) { // set silent mode
    this->silentMode = isSilent;
    riddleBank.setSilentMode(isSilent);
}