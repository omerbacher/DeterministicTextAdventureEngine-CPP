#pragma once
#include "Game.h"
#include "Steps.h"
#include "Screen.h"
#include <filesystem>  // Add this for file operations in silent mode

extern bool LOAD_MODE;
class LoadGame : public Game { // derived from Game
public:
	// constructor
	LoadGame(bool silent = false) :Game(), isSilentMode(silent) {
		Screen::setSilentMode(silent);
		this->silentMode = silent;
		riddleBank.setSilentMode(silent);
	} 
	virtual ~LoadGame() = default; // virtual destructor

	void run() override; // override run method

protected:
	bool isSilentMode; // flag for silent mode
	void gameLoop() override;
	// Replay game loop that ignores user input
	void replayGameLoop();
	// Initialize level without drawing (for silent mode)
	void initLevelSilent(const std::string& filename, int specificDoor = -1);
	// Override checkGameOver to hide screens in silent mode
	bool checkGameOver() override;
	// Override checkLevel to hide screens in silent mode
	bool checkLevel() override;
	Steps loadedSteps; // steps loaded from file
	void setSilentMode(bool isSilent);
};