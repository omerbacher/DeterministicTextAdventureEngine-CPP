#include "Results.h"
#include <fstream>
#include <sstream>
#include <algorithm>

void Results::setScreenFiles(const std::string& files) // Set the screen files (maps) used in the game
{
    screenFiles = files;
}

void Results::addLifeLost(size_t time, PlayerType player) // Log a life lost event
{
    Event event;
    event.time = time;
    event.type = EventType::LifeLost;
    event.player = player;
    events.push_back(event);
}

void Results::addScreenChange(size_t time, const std::string& screenName) // Log a screen change event
{
    Event event;
    event.time = time;
    event.type = EventType::ScreenChange;
    event.info = screenName;
    events.push_back(event);
}
// Log a riddle attempt event
void Results::addRiddle(size_t time, int riddleId, const std::string& riddleText,const std::string& answer, bool solved)
{
    Event event;
    event.time = time;
    event.type = EventType::Riddle;
    event.riddle = solved ? RiddleResult::Solved : RiddleResult::Failed;
    std::string cleanText = riddleText;
    std::replace(cleanText.begin(), cleanText.end(), '\n', ' ');
    event.info = std::to_string(riddleId) + "|" + cleanText + "|" + answer;
    events.push_back(event);
}
// Log a game over event
void Results::addGameOver(size_t time, int score1, int score2)
{
    Event event;
    event.time = time;
    event.type = EventType::GameOver;
    event.info = std::to_string(score1) + "|" + std::to_string(score2);
    events.push_back(event);
}
// Log a game finished event
void Results::addGameFinished(size_t time, int score1, int score2)
{
    Event event;
    event.time = time;
    event.type = EventType::GameFinished;
    event.info = std::to_string(score1) + "|" + std::to_string(score2);
    events.push_back(event);
}
// Log a game exit event
void Results::addGameExit(size_t time, int score1, int score2)
{
    Event event;
    event.time = time;
    event.type = EventType::GameExit;
    event.info = std::to_string(score1) + "|" + std::to_string(score2);
    events.push_back(event);
}
// Save results to a file
void Results::save(const std::string& filename) const
{
    std::ofstream file(filename);

    file << "screens= " << screenFiles << "\n";

    for (const auto& event : events)
    {
        file << "time= " << event.time << " event= ";

        if (event.type == EventType::ScreenChange)
        {
            file << "ScreenChange screenName= " << event.info;
        }
        else if (event.type == EventType::LifeLost)
        {
            std::string playerName = (event.player == PlayerType::Player1) ? "Player1" : "Player2";
            file << "LifeLost player= " << playerName;
        }
        else if (event.type == EventType::Riddle)
        {
            std::istringstream iss(event.info);
            std::string riddleId, riddleText, answer;

            std::getline(iss, riddleId, '|');
            std::getline(iss, riddleText, '|');
            std::getline(iss, answer, '|');

            std::string result = (event.riddle == RiddleResult::Solved) ? "Solved" : "Failed";

            file << "Riddle riddleId= " << riddleId
                << " riddleText= " << riddleText
                << " answer= " << answer
                << " result= " << result;
        }
        else if (event.type == EventType::GameOver)
        {
            size_t pos = event.info.find('|');
            std::string score1 = event.info.substr(0, pos);
            std::string score2 = event.info.substr(pos + 1);

            file << "GameOver score1= " << score1 << " score2= " << score2;
        }
        else if (event.type == EventType::GameFinished)
        {
            size_t pos = event.info.find('|');
            std::string score1 = event.info.substr(0, pos);
            std::string score2 = event.info.substr(pos + 1);

            file << "GameFinished score1= " << score1 << " score2= " << score2;
        }
        else if (event.type == EventType::GameExit)
        {
            size_t pos = event.info.find('|');
            std::string score1 = event.info.substr(0, pos);
            std::string score2 = event.info.substr(pos + 1);

            file << "GameExit score1= " << score1 << " score2= " << score2;
        }

        file << "\n";
    }
}
// Load results from a file
void Results::load(const std::string& filename)
{
    events.clear();
    std::ifstream file(filename);
    std::string line;

    if (std::getline(file, line))
    {
        if (line.find("screens= ") == 0)
        {
            screenFiles = line.substr(9);
        }
    }

    while (std::getline(file, line))
    {
        if (line.find("time= ") != std::string::npos && line.find("event= ") != std::string::npos)
        {
            Event event;
            parseEventLine(line, event);
            events.push_back(event);
        }
    }
	// Sort events by time after loading
    events.sort([](const Event& a, const Event& b) {
        return a.time < b.time;
        });
}

void Results::parseEventLine(const std::string& line, Event& event) // Parse a single event line from the results file
{
    size_t pos = line.find("time= ");
    if (pos != std::string::npos)
    {
        event.time = std::stoll(line.substr(pos + 6));
    }

    pos = line.find("event= ");
    if (pos == std::string::npos) return;

    std::string eventPart = line.substr(pos + 7);

    if (eventPart.find("ScreenChange") == 0)
    {
        event.type = EventType::ScreenChange;
        pos = line.find("screenName= ");
        if (pos != std::string::npos)
        {
            event.info = line.substr(pos + 12);
        }
    }
    else if (eventPart.find("LifeLost") == 0)
    {
        event.type = EventType::LifeLost;
        pos = line.find("player= ");
        if (pos != std::string::npos)
        {
            std::string playerName = line.substr(pos + 8);
            event.player = (playerName == "Player1") ? PlayerType::Player1 : PlayerType::Player2;
        }
    }
    else if (eventPart.find("Riddle") == 0)
    {
        event.type = EventType::Riddle;

        pos = line.find("riddleId= ");
        size_t endPos = line.find(" ", pos + 10);
        std::string riddleId = line.substr(pos + 10, endPos - pos - 10);

        pos = line.find("riddleText= ");
        endPos = line.find(" answer= ", pos);
        std::string riddleText = line.substr(pos + 12, endPos - pos - 12);

        pos = line.find("answer= ");
        endPos = line.find(" result= ", pos);
        std::string answer = line.substr(pos + 8, endPos - pos - 8);

        pos = line.find("result= ");
        std::string result = line.substr(pos + 8);
        event.riddle = (result == "Solved") ? RiddleResult::Solved : RiddleResult::Failed;

        event.info = riddleId + "|" + riddleText + "|" + answer;
    }
    else if (eventPart.find("GameOver") == 0)
    {
        event.type = EventType::GameOver;

        pos = line.find("score1= ");
        size_t endPos = line.find(" score2= ", pos);
        std::string score1 = line.substr(pos + 8, endPos - pos - 8);

        pos = line.find("score2= ");
        std::string score2 = line.substr(pos + 8);

        event.info = score1 + "|" + score2;
    }
    else if (eventPart.find("GameFinished") == 0)
    {
        event.type = EventType::GameFinished;

        pos = line.find("score1= ");
        size_t endPos = line.find(" score2= ", pos);
        std::string score1 = line.substr(pos + 8, endPos - pos - 8);

        pos = line.find("score2= ");
        std::string score2 = line.substr(pos + 8);

        event.info = score1 + "|" + score2;
    }
    else if (eventPart.find("GameExit") == 0)
    {
        event.type = EventType::GameExit;

        pos = line.find("score1= ");
        size_t endPos = line.find(" score2= ", pos);
        std::string score1 = line.substr(pos + 8, endPos - pos - 8);

        pos = line.find("score2= ");
        std::string score2 = line.substr(pos + 8);

        event.info = score1 + "|" + score2;
    }
}

bool Results::empty() const // Check if there are no events logged
{
    return events.empty();
}

Event Results::pop()
{
    Event event = events.front();
    events.pop_front();
    return event;
}

bool Results::compareWith(const Results& other, std::string& failureReason) const // Compare this Results with another, providing detailed failure reasons
{
	failureReason = ""; // reset reason

	// first, check screen files
    if (screenFiles != other.screenFiles)
    {
        failureReason = "Screen files don't match!\n";
        failureReason += "Expected: " + other.screenFiles + "\n";
        failureReason += "Got: " + screenFiles;
        return false;
    }

    auto itActual = events.begin();
    auto itExpected = other.events.begin();
    int eventIndex = 0; 
    std::string passedEventsLog = "Events matched successfully so far:\n";

	// loop through both event lists
    while (itActual != events.end() && itExpected != other.events.end())
    {
        const Event& actual = *itActual;
        const Event& expected = *itExpected;

        bool mismatch = false;
        std::string diffDetail = "";

		// check each field for mismatches
        if (actual.time != expected.time) {
            mismatch = true;
            diffDetail = "Time mismatch (Expected: " + std::to_string(expected.time) + ", Got: " + std::to_string(actual.time) + ")";
        }
        else if (actual.type != expected.type) {
            mismatch = true;
            diffDetail = "Event type mismatch (Expected: " + eventTypeToString(expected.type) + ", Got: " + eventTypeToString(actual.type) + ")";
        }
        else if (actual.info != expected.info) {
            mismatch = true;
            diffDetail = "Data/Info mismatch (Expected: '" + expected.info + "', Got: '" + actual.info + "')";
        }
        else if (actual.type == EventType::LifeLost && actual.player != expected.player) {
            mismatch = true;
            diffDetail = "Player mismatch";
        }
        else if (actual.type == EventType::Riddle && actual.riddle != expected.riddle) {
            mismatch = true;
            diffDetail = "Riddle result mismatch";
        }

        if (mismatch) {
            failureReason = "!!! MISMATCH AT EVENT #" + std::to_string(eventIndex) + " !!!\n";
            failureReason += diffDetail + "\n\n";
            failureReason += "EXPECTED: [" + eventToDebugString(expected) + "]\n";
            failureReason += "GOT:      [" + eventToDebugString(actual) + "]\n\n";
            failureReason += passedEventsLog;
            return false;
        }

		// if all fields match, log the successful event
        passedEventsLog += "  v Event #" + std::to_string(eventIndex) + ": " + eventTypeToString(actual.type) + "\n";

        ++itActual;
        ++itExpected;
        ++eventIndex;
    }

	// check for extra/missing events
    if (events.size() != other.events.size())
    {
        failureReason = "!!! EVENT COUNT MISMATCH !!!\n";
        failureReason += "Expected: " + std::to_string(other.events.size()) + " events\n";
        failureReason += "Got: " + std::to_string(events.size()) + " events\n\n";

        if (itExpected != other.events.end()) {
            failureReason += "Next missing event expected: [" + eventToDebugString(*itExpected) + "]\n";
        }
        else if (itActual != events.end()) {
            failureReason += "Got extra unexpected event: [" + eventToDebugString(*itActual) + "]\n";
        }

        failureReason += "\n" + passedEventsLog;
        return false;
    }

	return true; // all events matched
}
// Helper function to convert EventType to string
std::string Results::eventTypeToString(EventType type) const
{
    switch (type)
    {
    case EventType::ScreenChange:   return "ScreenChange";
    case EventType::LifeLost:       return "LifeLost";
    case EventType::Riddle:         return "Riddle";
    case EventType::GameOver:       return "GameOver";
    case EventType::GameFinished:   return "GameFinished";
    case EventType::GameExit:       return "GameExit";
    default:                        return "Unknown";
    }
}


// helper function to convert an Event to a debug string
std::string Results::eventToDebugString(const Event& e) const {
    std::string res = "Time: " + std::to_string(e.time) + ", Type: " + eventTypeToString(e.type);
    if (!e.info.empty()) res += ", Info: " + e.info;
    if (e.type == EventType::LifeLost) {
        res += (e.player == PlayerType::Player1 ? " (Player1)" : " (Player2)");
    }
    if (e.type == EventType::Riddle) {
        res += (e.riddle == RiddleResult::Solved ? " (Solved)" : " (Failed)");
    }
    return res;
}