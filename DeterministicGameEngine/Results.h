#pragma once
#include <list>
#include <string>
#include <cstddef>

enum class EventType {
    ScreenChange,   // Changed Screen
    LifeLost,       // Player lost a life
    Riddle,         // Riddle attempted
    GameOver,       // Game ended - player died
    GameFinished,   // Game ended - player won
    GameExit        // Game ended - player quit/exited
};

enum class RiddleResult { // Riddle attempt result
    Solved,
    Failed
};

enum class PlayerType {
    Player1,  // Player & 
    Player2   // Player $
};

struct Event { // Game event structure
    size_t time = 0;
    EventType type = EventType::LifeLost;
    RiddleResult riddle = RiddleResult::Failed;
    PlayerType player = PlayerType::Player1;
    std::string info;
};

class Results { // 
private:
	std::list<Event> events; // List of game events
	std::string screenFiles; // Screen files used
    void parseEventLine(const std::string& line, Event& event); // 

public:
	void setScreenFiles(const std::string& files); // Set screen files used

	void addLifeLost(size_t time, PlayerType player); // Log life lost event
	void addScreenChange(size_t time, const std::string& screenName); // Log screen change event
	void addRiddle(size_t time, int riddleId, const std::string& riddleText, const std::string& answer, bool solved); // Log riddle attempt event
	void addGameOver(size_t time, int score1, int score2); // Log game over event
	void addGameFinished(size_t time, int score1, int score2); // Log game finished event
	void addGameExit(size_t time, int score1, int score2);  // Log game exit event

	void save(const std::string& filename) const; // Save results to file
	void load(const std::string& filename); // Load results from file

	bool empty() const; // Check if results are empty
	Event pop(); // Pop the first event
    bool compareWith(const Results& other, std::string& failureReason) const;
	std::string eventTypeToString(EventType type) const; // Convert EventType to string
    std::string eventToDebugString(const Event& e) const;
};