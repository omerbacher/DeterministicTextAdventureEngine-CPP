#pragma once
#include <string>
#include "Riddle.h"
#include "Player.h"
#include "Screen.h"
# include "Results.h"
# include "Steps.h"

enum class RiddleOutcome { // Riddle attempt outcome
    Correct,
    Incorrect,
    AlreadySolved,
    NotFound
};

class RiddleBank
{
private:
	static constexpr int ESC = 27; // ESC key code
	static constexpr char YES = 'Y'; // Yes character
	static constexpr char NO = 'N'; // No character
	static constexpr char HINT = 'H'; // Hint character
	static constexpr int BACKSPACE = 8; // Backspace key code
	static constexpr size_t MAX_RIDDLES = 14; // Maximum number of riddles
	Riddle riddles[MAX_RIDDLES]; // Array to hold riddles
	size_t riddleCount = 0; // Current count of riddles
	bool loadedSuccessfully = false; // Flag to indicate if riddles loaded successfully
	Results* gameResults = nullptr; // Pointer to game results
	size_t* eventTimerPtr = nullptr; // Pointer to event timer
	Steps* recordedSteps = nullptr; // Pointer to recorded steps
	bool isLoadMode = false; // Flag for load mode
	bool isSilentLoadMode = false; // Flag for silent load mode
	char getRiddleInputChar(); // Get character input for riddle
	void processLoadModeRiddle(Player& player, Screen& screen, Riddle* r, int x, int y); // Process riddle in load mode
public:
	RiddleBank();// Constructor
	bool isLoaded() const { return loadedSuccessfully; } // Check if riddles loaded successfully
	Riddle* getRiddleById(int riddleID); // Get riddle by ID
	Riddle* getRiddleAt(int x, int y); // Get riddle at specific position
	void attachPositionToRoom(Screen& screen); // Attach riddle positions to the room
	void addRiddle(const Riddle& r); // Add a riddle to the bank
	RiddleOutcome checkAnswerFor(int riddleID, const std::string& answer);// Check answer for a specific riddle
	std::string getUserAnswer(int bx, int answerLine, int answerInputLine, int hintorResultLine, Riddle* r); // Get user answer for a riddle
	void displayRiddleQuestion(Riddle* r, int bx, int by); // Display riddle question
	void handleRiddle(Player& player, Screen& screen, int level); // Handle riddle interaction
	void resetAllRiddles(); // Reset all riddles to unsolved state
	void attachResults(Results* results, size_t* timerPtr); // Attach results and timer pointer
	void attachSteps(Steps* steps, bool loadmode = false); // Attach steps for recording or loading
	void setSilentMode(bool silent) { isSilentLoadMode = silent; } // Set silent mode
};