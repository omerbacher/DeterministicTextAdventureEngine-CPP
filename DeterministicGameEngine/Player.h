#pragma once
#include "Point.h"
#include "Bomb.h"
#include "results.h"
#include <cstddef> //for size_t

// forward declarations
class Screen; 
class Key;   
class Bomb;   

class Player
{
private:
	

	static constexpr size_t NUM_KEYS = 6; //numbers of controlled keys
	Point position; // current pos
	Point prevPos; // prev pos
	char keys[NUM_KEYS]; // keys for controlling player
	Screen* screen;// current screen
	char heldItem; // current held item
	int itemId; // id of the held item
	int points=0; // score
	int lives=3; // lives
	Key* myKey= nullptr; // player's key
	bool hasKey = false; // does the player have a key
	bool active = true;// does the player is active

	bool bombDroppedRequest = false; 
	Point lastDropPos;               
	// Results logging silent mode
	bool isSilentMode = false;
	Results* gameResults = nullptr;
	size_t* eventTimerPtr = nullptr;
	PlayerType type;
public:
	//constructor
	Player(const Point& start_point, const char(&the_keys)[NUM_KEYS])
		:position(start_point), screen(nullptr), heldItem(0), itemId(-1),points(0),
		lives(3),myKey(nullptr),hasKey(false),active(true), type(PlayerType::Player1)
	{
		// initialize control keys
		for (size_t i = 0; i < NUM_KEYS; ++i) {
			keys[i] = the_keys[i];
		}
	}

	// moving and drawing functions
	void draw(); // 
	void move();
	void keyPressed(char ch);
	void erase() const;
	int getX() const;
	int getY() const;
	char getChar() const;
	const Point getPosition() const { return position; }// get player's current position
	void setScreen(Screen& newScreen);// set player's current screen
	void rememberPosition();
	void stepBack();// move player back to previous position

	// Item functions
	bool hasItem() const;
	bool hasItem(char item) const;
	char getHeldItem() const;
	int getItemId() const;
	void dropHeldItem();
	void DropItem();
	void GrabItem(char item, int id = -1);
	void setPosition(const Point& pos);// set player's position

	// Key functions
	void setKey(bool val) { hasKey = val; }
	bool useKeyForDoor(char doorChar) const;
	void keyUsed();
	
	
	//Directions functions
	const Direction& getDirection() const
	{
		return position.getDirection();
	}
	Direction getOppositeDirection() const
	{
		return position.getOppositeDirection();
	}
	
    void setDirection(const Direction& dir) {position.changeDirection(dir);}

	// Score and Lives functions
	void addPoints(int pts);
	void losePoints(int pts);
	int getScore() const;
	void setScore(int s) { points = s; } // set score
	void addLives();
	int getLives() const;
	void setLives(int l) { lives = l; } // set lives
	void loseLife();
	bool isDead() const;
	void setHeldItem(char item, int id) { heldItem = item; itemId = id; }// set held item

	// Player active status
	bool isActive() const { return active; }
	void setInactive() { active = false; }
	void activate() {active = true;}

	bool hasDroppedBomb() const { return bombDroppedRequest; }
	Point getLastDropPos() const { return lastDropPos; }
	void clearBombRequest() { bombDroppedRequest = false; }

	void resetStats() { heldItem = 0; lives = 3; points = 0; }

	bool isMyKey(char ch) const; // check if the key move (A,W,D,X,E | I,J,L,M,O)  belongs to the player (& or $)

	// Silent mode and results attachment
	void setSilentMode(bool silent) { isSilentMode = silent; }
	void attachResults(Results* res, size_t* timer, PlayerType pType) {
		gameResults = res;
		eventTimerPtr = timer;
		type = pType;
	
	}
};

