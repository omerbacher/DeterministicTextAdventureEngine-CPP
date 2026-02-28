#include "UIScreens.h"
#include <iostream>
#include <conio.h>
#include "Utils.h"
#include <windows.h>
#include "Screen.h"

// prints the given screen to console
void UIScreens::printScreen(const char** screen)
{
    cls();
    for (int i = 0; i < HEIGHT; i++)
    {
        gotoxy(0, i);
        std::cout << screen[i];
    }
}

// MENU 

void UIScreens::showMenu()
{
    static const char* MENU_SCREEN[HEIGHT] =
    {
        "                                                                                ",
        "                          =================================                      ",
        "                                   TEXT ADVENTURE                                ",
        "                                        2025                                     ",
        "                          =================================                      ",
        "                                                                                ",
        "                                    MAIN  MENU                                   ",
        "                                                                                ",
        "                            [1]  Start a New Game                                ",
        "                            [3]  Load Saved Game                                 ",
        "                            [8]  Instructions & Controls                         ",
        "                            [2]  Colors: ON                                      ",
        "                            [9]  Exit                                            ",
        "                                                                                ",
        "                                                                                ",
        "                       Tip: If you get lost that's on you.                      ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
    };

    printScreen(MENU_SCREEN);
    gotoxy(0, 11);
    std::cout << "                            [2]  Colors: "
        << (ColorsEnabled ? "ON" : "OFF");
}

//instructions screen

void UIScreens::showInstructions()
{
    static const char* INSTRUCTIONS_SCREEN[HEIGHT] =
    {
        "                                                                                ", // 0
        "                          G A M E   I N S T R U C T I O N S                      ", // 1
        "                                                                                ", // 2
        "  A cooperative maze for two players. Push, pull, think, panic together          ", // 3
        "  because teamwork makes the dream work (and prevents unnecessary respawns).     ", // 4
        "                                                                                ", // 5
        "   PLAYER 1 ($)                             PLAYER 2 (&)                         ", // 6
        "   W - Up        D - Right                 I - Up        L - Right               ", // 7
        "   X - Down      A - Left                  M - Down      J - Left                ", // 8
        "   S - Stay      E - Action                K - Stay      O - Action              ", // 9
        "                                                                                ", // 10
        "                            ITEMS & ELEMENTS                                    ", // 11
        "                                                                                ", // 12
        "   K    - Key         | Opens doors. No, not your heart.                         ", // 13
        "   *    - Obstacle    | Push together (or cry).                                  ", // 14
        "   @    - Bomb        | Big boom. Destroys *, w. Try not to be nearby.            ", // 15
        "   !    - Torch       | Lights the way. Batteries not included.                  ", // 16
        "   ?    - Riddle      | Solve it or stare until it solves you.                   ", // 17
        "   A    - Light Switch| Turns lights ON/OFF in dark rooms. Magic? No.            ", // 18
        "   /\\   - Switch      | ON/OFF. Dramatic click guaranteed.                       ", // 19
        "   ssss - temp Wall   | Temporary wall. Controlled by a switch… usually.         ", // 20
        "                                                                                 ", // 21
        "        TIME LIMIT: 6 minutes. Fail to finish and you die. No pressure.          ", // 22
        "       -   6 minutes in heaven... after it , it will become HELL...  -          ", // 23
        "                      Press ANY key to return to the Main Menu...                 "  // 24
    };

    printScreen(INSTRUCTIONS_SCREEN);
}

// pause screen

void UIScreens::showPauseScreen()
{
    static const char* PAUSE_SCREEN[HEIGHT] =
    {
        "                                                                                ",
        "                                   GAME PAUSED                                  ",
        "                                                                                ",
        "                                 ================                                ",
        "                                  The game is paused                             ",
        "                                 ================                                ",
        "                                                                                ",
        "                      Press ESC to return to the game                            ",
        "                      Press S   to save game                                     ",
        "                      Press H   to return to the main menu                       ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                "
    };

    printScreen(PAUSE_SCREEN);
}
// win screen

void UIScreens::showWinScreen()
{
    static const char* WIN_SCREEN[HEIGHT] =
    {
        "                                                                                ",
        "                                   YOU  WIN!                                     ",
        "                                                                                ",
        "                          =============================                          ",
        "                             Congratulations, team!                              ",
        "                          =============================                          ",
        "                                                                                ",
        "               You solved puzzles, pushed obstacles, flipped switches,           ",
        "               collected keys, avoided disasters, and escaped the maze!          ",
        "                                                                                ",
        "                     (Honestly? I didn't think you'd make it:)                   ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                         Press ANY key to return to Main Menu                   ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                "
    };

    printScreen(WIN_SCREEN);
    
}

void UIScreens::showExitMessage()
{
    cls();
    std::cout << "\n\n";
    std::cout << "=============================================\n";
    std::cout << "           THANKS FOR PLAYING! :)\n";
    std::cout << "=============================================\n\n";

    Sleep(1200); // time to read the massage
}

void UIScreens::showGameOverMessage()
{
    static const char* GAME_OVER_SCREEN[Screen::HEIGHT] =
    {
        "                                                                                ",//1
        "                                OOPS... YOU DIED!                                ",//2
        "                                                                                ",//3
        "                          =============================                          ",//4
        "                          Even heroes need more practice!                        ",//5
        "                                                                                ",//6
        "                    I knew you wouldn't make it this time...                     ",//7
        "                                                                                ",//8
        "                 Try again! Maybe you'll fail a bit further ;)                   ",
        "                                                                                ",
        "                                                                                ",
        "              @         @@@@@@    @@@@@@    @@@@@@    @@@@@@@                   ",
        "              @         @    @    @         @         @     @                  ",
        "              @         @    @    @@@@@     @@@@@     @@@@@@@                  ",
        "              @         @    @         @    @         @    @                   ",
        "              @@@@@@    @@@@@@    @@@@@@    @@@@@@    @     @                   ",
        "                                                                                ",
        "                                                                                ",
        "                        Press ANY key to return to Main Menu                      ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
        "                                                                                ",
    };

    printScreen(GAME_OVER_SCREEN);
	if (!LOAD_MODE) {  // if not in load mode, wait for key press
        waitForKey();
    }
	else {  // if in LOAD mode, just sleep
        Sleep(80);
    }
}

void UIScreens::showInventoryFullMessage(const Screen& currentScreen) {
    Screen::drawAnimatedBox(10, 5, 50, 12);
    gotoxy(13, 7);
    std::cout << "Inventory Full!";
    gotoxy(13, 9);
    std::cout << "You can only carry one item at a time.";
    gotoxy(13, 11);
    std::cout << "Press any key to continue...";

	if (!LOAD_MODE) {  // if not in LOAD mode,
        waitForKey();
    }
	else {  //  in LOAD mode, just sleep
        Sleep(80);
    }

    Screen::closeAnimatedBox(10, 5, 50, 12);
}