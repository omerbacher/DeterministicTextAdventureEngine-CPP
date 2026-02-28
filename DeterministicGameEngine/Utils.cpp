#include <windows.h>
#include <iostream>
#include <cstdlib>
#include "Utils.h"
#include <conio.h>
#include <limits>
#include "Screen.h"

using std::cout; 
using std::endl;   

bool ColorsEnabled = true;

//Taken from lab exercise 10.11.25
void gotoxy(int x, int y) {
    
    std::cout.flush();
    COORD coord = { 0,0 };
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//Taken from lab exercise 10.11.25
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Hide the cursor
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}
//Taken from lab exercise 10.11.25
void cls() {
 
    system("cls");
}

//setscreen taken from AI
void setScreen(int width, int height) {
    HANDLE hScreen = GetStdHandle(STD_OUTPUT_HANDLE);
    
    SMALL_RECT windowSize = { 0, 0, (short)(width - 1), (short)(height - 1) };
    SetConsoleWindowInfo(hScreen, TRUE, &windowSize);
  
    COORD bufferSize = { (short)width, (short)height };
    SetConsoleScreenBufferSize(hScreen, bufferSize);
}
//AI generated function to change text color in console
void setColor(int color) {
    if (ColorsEnabled)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }
}
void resetColor() {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // 7 is the default color
}
// clear the input from any previous key presses
void clearInputBuffer()
{
    while (_kbhit()) auto i = _getch();
}
// wait for a key press from the user
void waitForKey()
    {
        clearInputBuffer(); 
		auto i = _getch();            
    }

// Helper function to allocate and copy a string to const char*
const char* allocateString(const std::string& str) {
    char* newStr = new char[str.length() + 1];
    strcpy_s(newStr, str.length() + 1, str.c_str());
    return newStr;
}

// Helper function to replace \n with actual newlines
std::string processNewlines(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.length(); ++i) {
        if (i + 1 < str.length() && str[i] == '\\' && str[i + 1] == 'n') {
            result += '\n';
            ++i; // Skip the 'n'
        }
        else {
            result += str[i];
        }
    }
    return result;
}

// Helper function to convert string to boolean
bool stringToBool(const std::string& value) {
    return value == "true" || value == "TRUE";
}


