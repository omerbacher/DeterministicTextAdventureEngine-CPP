#pragma once
#include <utility>
#include <cmath>
#include "Direction.h"

class Point {
	int x, y;
	Direction dir = Direction::directions[Direction::RIGHT];
	char ch;
	
	// screen boundries
	static constexpr int MAX_X = 79;
	static constexpr int MAX_Y = 24;

	//Taken from lab exercise number 4
public:

	// constructor
	Point() :x(0),y(0),dir(Direction::directions[Direction::STAY]),ch('$'){}
	Point(int x1, int y1, const Direction& the_dir, char c) {
		x = x1;
		y = y1;
		dir = the_dir;
		ch = c;
	}
	Point(int x1, int y1) {
		x = x1;
		y = y1;
		dir = Direction::directions[Direction::STAY];
		ch = ' ';
	}

	//draing functions
	void draw() {
		draw(ch);
	}
	void draw(char c);
	void erase() const;

	//moving and direction functions
	void move();
	void changeDirection(const Direction& new_dir) {
		dir = new_dir;
	}
	const Direction& getDirection() const { return dir; };
	Direction getOppositeDirection() const {
		return Direction(-dir.getX(), -dir.getY());
	}
	//data functions
	int getX() const {
		return x;
	}
	int getY() const {
		return y;
	}
	char getChar() const {
		return ch;
	}
	
	void set(int new_x, int new_y); 
};
