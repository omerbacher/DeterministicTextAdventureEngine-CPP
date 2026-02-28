#include <iostream>
#include "Point.h"
#include "utils.h"
#include "Screen.h"

// draw point at current position with character c
void Point::draw(char c) {
	gotoxy(x, y);
	std::cout << c;
}

// move point in the current direction
void Point::move() {
	if (x + dir.dirx < 0 || x + dir.dirx > Screen::WIDTH) {
		dir.dirx = -dir.dirx;
	}
	if (y + dir.diry < 0 || y + dir.diry > Screen::HEIGHT) {
		dir.diry = -dir.diry;
	}
	x += dir.dirx;
	y += dir.diry;
}
// erase point from current direction
void Point::erase() const {
	gotoxy(x, y);
	std::cout << ' ';
}
// set new coordinates for the point in the screen boundries
void Point::set(int new_x, int new_y) {
	if (new_x >= 0 && new_x <= Screen::WIDTH) {
		x = new_x;
	}
	if (new_y >= 0 && new_y <= Screen::HEIGHT) {
		y = new_y;
	}
}