#pragma once
// declare the color constants
static constexpr int COLOR_LIGHT_PURPLE = 1;     // riddles '?'
static constexpr int COLOR_CYAN = 3;     // Bombs '@'
static constexpr int COLOR_RED = 4;              // Torch '!'
static constexpr int COLOR_GOLD = 6;              // keys 'K'
static constexpr int COLOR_GRAY = 8;              // obstacles
static constexpr int COLOR_LIGHT_GREEN = 10; // switches
static constexpr int COLOR_LIGHT_CYAN = 11;    // doors
static constexpr int COLOR_LIGHT_RED = 12; 
static constexpr int COLOR_YELLOW = 14; 

void gotoxy(int x, int y);// go to the position x,y in the screen

extern bool ColorsEnabled;

void hideCursor(); //hide the console cursor

void cls();// clear the screen

void setScreen(int width, int height); // set the screen size

void setColor(int color);// set the text color

void resetColor(); // reset the text color

void clearInputBuffer(); // clear the input from any previous key presses

void waitForKey(); // wait for a key press from the user

// String helper functions for riddles
const char* allocateString(const std::string& str); // allocate and copy string to const char*

std::string processNewlines(const std::string& str); // convert \n escape sequences to actual newlines

bool stringToBool(const std::string& value); // convert string to boolean

