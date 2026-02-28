#pragma once
#include "Point.h"

// forward declarations
class Screen;
class Player;
class Results;

class Bomb {
	Point position; // Bomb position
	int timer; // Countdown timer
	char playerChar; // Character of the player who placed the bomb
	int roomID; // ID of the room where the bomb is placed

	Results* gameResults = nullptr;  // game results logger
	size_t eventTimer = 0;           // event timer

public:

	Bomb(Point p, char pChar, int ID) : position(p), timer(50), playerChar(pChar), roomID(ID) {} // Constructor
	bool tick(Screen& screen, Player& p1, Player& p2, int currentRoomID); // returns true if exploded
	void explode(Screen& screen, Player& p1, Player& p2, bool isInCurrentRoom); // Updated signature
	Point getPosition() const { return position; } // Getter for position
	int getRoomID() const { return roomID; } // Getter for room ID
	int getTimer() const { return timer; } // Getter for timer
	void attachResults(Results* results, size_t timer); // Attach results to the results file
	static constexpr int EXPLOSION_RANGE = 3; // Explosion range
	static constexpr int COLOR = 3; // color for bomb display
};
