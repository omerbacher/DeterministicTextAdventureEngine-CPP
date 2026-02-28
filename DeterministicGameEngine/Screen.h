#pragma once
#include <iostream>
#include <array>
#include <string>
#include <vector>
#include "Point.h"
#include "Door.h"
#include "Utils.h"
#include "RoomMeta.h"


enum class RoomLevel {
	ROOM_1 = 0,      // room 1 leadsTo=1
	ROOM_2 = 1,      // room 2 leadsTo=1
	ROOM_3 = 2,      // room 3 leadsTo=1
	ROOM_4 = 3,      //  WIN SCREEN
	NUM_ROOMS = 4,   // total number of rooms
	INVALID = -1     // invalid room
};

class Player; // forward declaration

// class to manage the screen layout and interactions
class Screen {
public:
	static constexpr int WIDTH = 80; // screen width
	static constexpr int HEIGHT = 25; // screen height
	static constexpr int MAP_HEIGHT = 23; // map height
	static constexpr int NUM_MAPS = 4; // number of maps
	static constexpr int MAX_DOORS = 10; // maximum number of doors

private:
	char screen[HEIGHT][WIDTH + 1];
	Door doors [10]; // array to hold doors
	bool dark = false; // is the screen dark
	Point legendPos; // position of the legend
	void clearScreenBuffer(); // clears the screen buffer
	bool isLegendPositionValid(int x, int y, const std::string& filename); // checks if the legend position is valid
	RoomMeta roomMeta; // metadata for the room
	static bool silentMode;
public:
	Screen();
	// Map drawing
	bool loadMapFromFile(const std::string& filename, int maxRooms); // returns true if loaded successfully
	void drawMap() const; // draws the map to the screen
	Point getLegendPos() const { return legendPos; } // position of the legend
	void drawMapWithTorch(const Player& p1) const; // draws the map with torch effect
	bool isDark() const; // checks if the screen is dark
	void setDark(bool isDark); // sets the dark state of the screen
	void drawDark() const; // draws the dark screen
	static void resetTorchState(); // resets the torch state
	// Text drawing
	void setCharAt(const Point& p, char ch);
	void setCharAt(int x, int y, char ch);
	void setCharAtSilent(int x, int y, char ch); // sets char without drawing
	void drawStatusBar(char inv1, int lives1, int score1, char inv2, int lives2, int score2, int timeRemaining, int gameTimer = -1); // draws the status bar
	static void drawBox(int x, int y, int width, int height); // draws a box
	static void clearBox(int x, int y, int width, int height);	 // clears a box
	static void drawAnimatedBox(int x, int y, int w, int h); // draws an animated box
	static void closeAnimatedBox(int x, int y, int w, int h); // closes an animated box
	void printInBox(int x, int y, const std::string& message); // prints message in a box
	void showMessage(const std::string& msg); // shows a message on the screen
	// Bomb explosion logic
	void explodeBomb(int centerX, int centerY);
	//Map logic
	char getCharAt(const Point& p) const;
	char getCharAt(int x, int y) const;
	bool isWall(const Point& p) const;
	bool isObstacle(const Point& p) const;
	Door* getDoor(const Point& p);
	Door* getDoorById(int doorId);
	//Map data
	int getWidth() const;
	int getHeight() const;
	const char* getMapRow(int row) const; // get a specific row of the map

	void applyColor(char c) const;

	RoomMeta getRoomMeta() const
	{
		return roomMeta;
	}
	// Setter for roomMeta
	void setRoomMeta(const RoomMeta& meta)
	{
		roomMeta = meta;
	}
	// Metadata placement and validation
	bool validateMetadata(const std::string& filename, int maxRooms);
	bool validateKey(const std::string& filename);
	bool validateDoors(const std::string& filename, int maxRooms);

	static void setSilentMode(bool mode) { silentMode = mode; }
	static bool isSilent() { return silentMode; }
};