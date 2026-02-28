#include "RiddleBank.h"
#include "Game.h"
#include "SaveGame.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include "Utils.h"
#include <fstream>

extern bool SAVE_MODE;

RiddleBank::RiddleBank() : riddleCount(0), loadedSuccessfully(false)
{
    // Try to load riddles from file
    std::ifstream file("Riddles.txt");

    if (!file.is_open()) {
        std::cout << "\n========================================" << std::endl;
        std::cout << "ERROR: Could not open Riddles.txt file!" << std::endl;
        std::cout << "Please make sure Riddles.txt is in the same folder as the game." << std::endl;
        std::cout << "========================================\n" << std::endl;
        loadedSuccessfully = false;
        return;
    }

    // Read riddles from file
    std::string line;
    while (riddleCount < MAX_RIDDLES && std::getline(file, line)) {
        int id = std::stoi(line);

        std::string question;
        if (!std::getline(file, question)) break;
        question = processNewlines(question);

        std::string answer;
        if (!std::getline(file, answer)) break;

        std::string hint;
        if (!std::getline(file, hint)) break;

        riddles[riddleCount++] = Riddle(id,
            allocateString(question),
            allocateString(answer),
            allocateString(hint));
    }

    file.close();

    if (riddleCount > 0) {
        loadedSuccessfully = true;
    }
    else {
        std::cout << "ERROR: No riddles were loaded from Riddles.txt!" << std::endl;
        loadedSuccessfully = false;
    }
}

Riddle* RiddleBank::getRiddleById(int riddleID)
{
    for (size_t i = 0; i < riddleCount; ++i) {
        if (riddles[i].getRiddleID() == riddleID) {
            return &riddles[i];
        }
    }
    return nullptr;
}

void RiddleBank::addRiddle(const Riddle& r)
{
    if (riddleCount < MAX_RIDDLES) {
        riddles[riddleCount++] = r;
    }
}

Riddle* RiddleBank::getRiddleAt(int x, int y)
{
    for (size_t i = 0; i < riddleCount; ++i) {
        Point p = riddles[i].getPosition();
        if (p.getX() == x && p.getY() == y) {
            return &riddles[i];
        }
    }
    return nullptr;
}
// Attach riddles to their positions in the given room screen
void RiddleBank::attachPositionToRoom(Screen& screen)
{
    const RoomMeta& meta = screen.getRoomMeta();
    bool hasError = false;
    std::string errorMessages = "";

    for (int i = 0; i < meta.getRiddleCount(); i++)
    {
        int riddleID = meta.getRiddleID(i);
        int x = meta.getRiddleX(i);
        int y = meta.getRiddleY(i);

        Riddle* r = getRiddleById(riddleID);
        if (!r)
        {
            hasError = true;
            errorMessages += "ERROR: Riddle ID " + std::to_string(riddleID) +
                " not found in Riddles.txt!\n";
            continue;
        }

        if (r->isSolved())
        {
            continue;
        }

        if (x < 0 || x >= Screen::WIDTH || y < 0 || y >= Screen::MAP_HEIGHT)
        {
            hasError = true;
            errorMessages += "ERROR: Riddle ID " + std::to_string(riddleID) +
                " has invalid position (" + std::to_string(x) +
                ", " + std::to_string(y) + ")!\n";
            continue;
        }

        char cell = screen.getCharAt(x, y);
        if (cell == '?')
        {
            r->setPosition(Point(x, y));
            continue;
        }

        if (cell == ' ')
        {
            r->setPosition(Point(x, y));
            screen.setCharAt(x, y, '?');
            continue;
        }

        hasError = true;
        errorMessages += "ERROR: Riddle ID " + std::to_string(riddleID) +
            " position (" + std::to_string(x) + ", " +
            std::to_string(y) + ") is not empty or '?'. Current: '" +
            cell + "'\n";
    }

    const int W = Screen::WIDTH;
    const int H = Screen::MAP_HEIGHT;

    for (int y = 0; y < H; ++y)
    {
        for (int x = 0; x < W; ++x)
        {
            if (screen.getCharAt(x, y) != '?')
                continue;

            if (getRiddleAt(x, y) != nullptr)
                continue;
            screen.setCharAt(x, y, ' ');
        }
    }

    if (hasError) {
        gotoxy(0, 23);
        std::cout << errorMessages;
        std::cout << "Press any key to continue..." << std::endl;
        waitForKey();
        cls();
    }
}
// Check the answer for a given riddle ID
RiddleOutcome RiddleBank::checkAnswerFor(int riddleID, const std::string& answer)
{
    Riddle* r = getRiddleById(riddleID);
    if (!r) return RiddleOutcome::NotFound;

    if (r->isSolved()) return RiddleOutcome::AlreadySolved;

    if (r->checkAnswer(answer.c_str())) {
        return RiddleOutcome::Correct;
    }
    return RiddleOutcome::Incorrect;
}
// Display the riddle question in the box
void RiddleBank::displayRiddleQuestion(Riddle* r, int bx, int by)
{
    gotoxy(bx + 2, by + 1);
    std::cout << "Riddle:";

    std::string q = r->getQuestion();
    int line = by + 3;
    std::string temp;

    for (char c : q)
    {
        if (c == '\n')
        {
            gotoxy(bx + 2, line++);
            std::cout << temp;
            temp.clear();
        }
        else temp += c;
    }
    if (!temp.empty())
    {
        gotoxy(bx + 2, line++);
        std::cout << temp;
    }
}
// Get the user's answer to the riddle
std::string RiddleBank::getUserAnswer(int bx, int answerLine, int answerInputLine,
    int hintorResultLine, Riddle* r)
{
    gotoxy(bx, answerLine);
    std::cout << "Answer (press H for hint): ";
    gotoxy(bx, answerInputLine);
    std::cout << ">>> " << std::string(40, ' ');

    std::string ans = "";
    int cursorX = bx + 4;
    int cursorY = answerInputLine;

    gotoxy(cursorX, cursorY);

    while (true)
    {
        if (eventTimerPtr != nullptr)
        {
            (*eventTimerPtr)++;
        }
        Sleep(20);
        char c = getRiddleInputChar();

        if (c == ESC)
        {
            Game::pauseRequestedFromRiddle = true;
            return "";
        }

        if (c == '\r')
        {
            return ans;
        }
        else if ((c == HINT || c == tolower(HINT)) && ans.empty())
        {
            gotoxy(bx, hintorResultLine);
            std::cout << std::string(50, ' ');
            gotoxy(bx, hintorResultLine);
            std::cout << "Hint: " << r->getHint();
            gotoxy(cursorX + (int)ans.length(), cursorY);
            continue;
        }
        if (c == BACKSPACE)
        {
            if (!ans.empty())
            {
                ans.pop_back();
                gotoxy(cursorX + (int)ans.length(), cursorY);
                std::cout << ' ';
                gotoxy(cursorX + (int)ans.length(), cursorY);
            }
            continue;
        }
        if (ans.length() < 40)
        {
            ans.push_back(c);
            gotoxy(cursorX + (int)ans.length() - 1, cursorY);
            std::cout << c;
        }
    }
}
// Handle the riddle interaction when the player steps on a riddle cell
void RiddleBank::handleRiddle(Player& player, Screen& screen, int level)
{
    int x = player.getX();
    int y = player.getY();
    char cell = screen.getCharAt(x, y);

    if (cell != '?')
        return;

    Riddle* r = getRiddleAt(x, y);
    if (!r || r->isSolved())
        return;

    // In SILENT load mode - process riddle silently without drawing
    if (isSilentLoadMode)
    {
        processLoadModeRiddle(player, screen, r, x, y);
        player.stepBack();
        return;
    }

    // NORMAL GAME MODE OR REGULAR LOAD MODE 
    const int bx = 15, by = 5, bw = 50, bh = 12;

    screen.drawAnimatedBox(bx, by, bw, bh);
    gotoxy(bx + 2, by + 2); std::cout << "You stepped on a riddle.";
    gotoxy(bx + 2, by + 4); std::cout << "Answer it? (Y/N): ";

    while (true)
    {
        if (eventTimerPtr != nullptr)
        {
            (*eventTimerPtr)++;
        }
        Sleep(20);
        char choice = getRiddleInputChar();

        if (choice == ESC)
        {
            Game::pauseRequestedFromRiddle = true;
            screen.closeAnimatedBox(bx, by, bw, bh);
            player.stepBack();
            return;
        }
        if (tolower(choice) == tolower(NO))
        {
            screen.closeAnimatedBox(bx, by, bw, bh);
            player.stepBack();
            screen.setCharAt(x, y, '?');

            if (screen.isDark())
                screen.drawMapWithTorch(player);
            else
                screen.drawMap();
            player.draw();

            return;
        }

        if (tolower(choice) == tolower(YES))
        {
            break;
        }
    }
    screen.closeAnimatedBox(bx, by, bw, bh);

    screen.drawAnimatedBox(bx, by, bw, bh);
    displayRiddleQuestion(r, bx, by);

    int answerLine = by + 7;
    int answerInputLine = by + 8;
    int hintorResultLine = by + 9;

    std::string answer = getUserAnswer(bx + 2, answerLine, answerInputLine,
        hintorResultLine, r);

    if (Game::pauseRequestedFromRiddle)
    {
        screen.closeAnimatedBox(bx, by, bw, bh);
        player.stepBack();
        return;
    }

    bool correct = r->checkAnswer(answer.c_str());
    if ((SAVE_MODE || isLoadMode) && gameResults != nullptr)
    {
        PlayerType playerType = (player.getChar() == '&') ? PlayerType::Player1 : PlayerType::Player2;
        gameResults->addRiddle(*eventTimerPtr, r->getRiddleID(), r->getQuestion(), answer, correct);
        
    }
    gotoxy(bx + 2, hintorResultLine);
    std::cout << std::string(50, ' ');

    gotoxy(bx + 2, hintorResultLine);

    if (correct)
    {
        std::cout << "Correct! +100 points";
        player.addPoints(100);
        r->markAsSolved();
        screen.setCharAt(x, y, ' ');
       
    }
    else
    {
        std::cout << "Wrong! -1 life";
        player.loseLife();
        screen.setCharAt(x, y, '?');
    }

    Sleep(700);
    screen.closeAnimatedBox(bx, by, bw, bh);

    player.stepBack();
    if (screen.isDark())
        screen.drawMapWithTorch(player);
    else
        screen.drawMap();
    player.draw();
}

void RiddleBank::resetAllRiddles()
{
    for (size_t i = 0; i < riddleCount; ++i)
    {
        riddles[i].resetRiddle();
    }
}
// Attach results and timer pointer for logging
void RiddleBank::attachResults(Results* results, size_t* timerPtr)
{
    gameResults = results;
    eventTimerPtr = timerPtr;
}
// Attach steps for load/save mode
void RiddleBank::attachSteps(Steps* steps, bool loadmode)
{
    recordedSteps = steps;
    isLoadMode = loadmode;
}
// Get a character input for the riddle, handling load/save modes
char RiddleBank::getRiddleInputChar()
{
    char c;

    if (isLoadMode && recordedSteps != nullptr && eventTimerPtr != nullptr)
    {
        while (true)
        {
            Steps::Step step;
            if (recordedSteps->getNextStep(*eventTimerPtr, step))
            {
                if (step.PlayerNum == 0)
                {
                    c = step.key;
                    return c;
                }
                continue;
            }

            (*eventTimerPtr)++;
            Sleep(1);
        }
    }
    else
    {
        c = _getch();

        if (SAVE_MODE && recordedSteps != nullptr && eventTimerPtr != nullptr)
        {
            recordedSteps->addStep(*eventTimerPtr, 0, c);
        }

        return c;
    }
}
// Process the riddle in load mode using recorded steps
void RiddleBank::processLoadModeRiddle(Player& player, Screen& screen, Riddle* r, int x, int y)
{
    Steps::Step riddleStep;
    std::string fullAnswer = "";
    size_t finalIteration = 0;
	bool firstStep = true; // flag for first step
    bool declined = false;

	// first build the full answer from recorded steps
    while (recordedSteps->getNextRiddleStep(riddleStep)) {
		finalIteration = riddleStep.iteration; // keep updating final iteration
        char riddleKey = riddleStep.key;
		// handle first step (Y/N)
        if (firstStep) {
            
            char key = tolower(riddleStep.key);

			if (key == 'n') { // player declined
                declined = true;
                break;
            }
            if (key == 'y') { 
                firstStep = false;
                continue;
            }
			continue; //ignore other keys at first step
        }
        if ((riddleKey == 'h' || riddleKey == 'H') && fullAnswer.empty()) {
            continue;
        }
        if (riddleKey == 8) {
            if (!fullAnswer.empty()) {
                fullAnswer.pop_back();
            }
			continue;
        }
		// check for end of answer
        if (riddleStep.key == ' ' || riddleStep.key == '\0' || riddleStep.key == '\r') {
            break;
        }

		// append character to full answer
        fullAnswer += riddleStep.key;
    }

	// advance event timer to final iteration
    if (eventTimerPtr != nullptr && *eventTimerPtr < finalIteration) {
        size_t currentT = *eventTimerPtr;
        for (size_t t = currentT + 1; t <= finalIteration; t++) {
            Steps::Step dummy;
            while (recordedSteps->getNextStep(t, dummy));
        }
        *eventTimerPtr = finalIteration;
    }

	// player declined to answer
    if (declined) {
        player.stepBack();
        return;
    }

	// check the answer
    bool correct = r->checkAnswer(fullAnswer.c_str());

    if (gameResults != nullptr) {
        gameResults->addRiddle(finalIteration, r->getRiddleID(), r->getQuestion(), fullAnswer, correct);
    }

	//update player and screen based on correctness
    if (correct) {
        player.addPoints(100);
        r->markAsSolved();
        if (!isSilentLoadMode) screen.setCharAt(x, y, ' ');
        else screen.setCharAtSilent(x, y, ' ');
    }
    else {
        player.loseLife();
        if (!isSilentLoadMode) screen.setCharAt(x, y, '?');
        else screen.setCharAtSilent(x, y, '?');
    }
}