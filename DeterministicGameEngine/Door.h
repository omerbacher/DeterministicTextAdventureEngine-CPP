#pragma once
#include "Player.h"
// forward declaration
class Screen; 
class RoomMeta;
class Door
{
private:
	int id; // door number 1-9
	bool open; //  is the door open ?
	int destinationLevel; // level to which the door leads
public:
	Door() : id(-1), open(false), destinationLevel(-1) {} // default constructor
	Door(int doorID, int dest = -1) : id(doorID), open(false), destinationLevel(dest) {} // constructor with id and optional destination level
	
	static constexpr int MAX_DOOR_ID = 10; // max door id +1 (doors 1-9)
	static constexpr int COLOR = 11; // Light Cyan
	static bool switchesAreOn; // are all switches on ?
	static bool openDoors[MAX_DOOR_ID]; // openDoors[1] -> door with id 1
	
	
	//GET DATA
	int getId() const { return id; } // get door id
	bool isOpen() const { return open; } // is the door open ?
	int getDestinationLevel() const { return destinationLevel; } // returns -1 if not set
	void setDestinationLevel(int level) { destinationLevel = level; } // set destination level

	//DOOR LOGIC
	bool tryOpen(int keyId); // tries to open the door with the given key id
	bool canPass() const { return open; } // can the player pass through the door ?
	static bool isDoorChar(char c) { return (c >= '1' && c <= '9'); } // is the char a door char ?
	static bool handleDoor(Player& p, Screen& screen, char& foundDoor, bool isSilent = false); // handle door interaction
	void setOpen() { open = true; } // SET DOOR OPEN
   //SET AWITCHES ON
	static void allSwitchesAreOn() { switchesAreOn = true; } // set all switches on

};



