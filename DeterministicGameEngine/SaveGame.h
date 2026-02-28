#pragma once
#include "Game.h"
#include "Steps.h"

extern bool SAVE_MODE; // Global flag indicating save mode

class SaveGame : public Game { // derived from Game
public:
	SaveGame() = default; // default constructor
	virtual ~SaveGame() = default; // virtual destructor

	void run() override; // override run method

protected:
    // Custom game loop that records events
    void saveGameLoop();

    // Override methods to record events
    bool checkGameOver() override;
    bool checkLevel() override;


};