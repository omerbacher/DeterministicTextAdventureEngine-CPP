#include "Steps.h"
#include <fstream>
#include <iostream>


// Add a step to the recording
void Steps::addStep(size_t iteration, int playerNum, char step) {
    steps.push_back({ iteration, playerNum, step });
}

// Load steps from file helped by AI
Steps Steps::loadSteps(const std::string& filename) {
    Steps loadedSteps;
    std::ifstream steps_file(filename);

    if (!steps_file.is_open()) {// Check if file opened successfully
        std::cout << "Error: Could not open steps file: " << filename << std::endl;
        return loadedSteps;
    }

    // Read screen files line (first line) - format: "Loading the screens: file1|file2|file3"
    std::string screensLine;
    if (std::getline(steps_file, screensLine)) {
        size_t pos = screensLine.find(':');
        if (pos != std::string::npos) {
            std::string screens = screensLine.substr(pos + 1);
            // Trim leading space
            size_t start = screens.find_first_not_of(" \t\r\n");
            if (start != std::string::npos) {
                screens = screens.substr(start);
            }
            loadedSteps.setScreenFiles(screens);
        }
    }

    // Read number of steps line (second line) - format: "Number of steps: 7"
    std::string numberLine;
    if (std::getline(steps_file, numberLine)) {
        size_t pos = numberLine.find(':');
        if (pos != std::string::npos) {
            std::string numStr = numberLine.substr(pos + 1);
            // Trim whitespace
            size_t start = numStr.find_first_not_of(" \t\r\n");
            if (start != std::string::npos) {
                numStr = numStr.substr(start);
            }
            size_t size = std::stoul(numStr);

            // Read each step - format: "TIME: 6 PLAYER: 1 KEY: d"
            std::string line;
            for (size_t i = 0; i < size; ++i) {
                if (std::getline(steps_file, line)) {
                    // Find positions of keywords
                    size_t timePos = line.find("TIME:");
                    size_t playerPos = line.find("PLAYER:");
                    size_t keyPos = line.find("KEY:");

                    if (timePos != std::string::npos &&
                        playerPos != std::string::npos &&
                        keyPos != std::string::npos) {

                        // Extract time (between "TIME: " and " PLAYER:")
                        std::string timeStr = line.substr(timePos + 6, playerPos - timePos - 7);
                        size_t iteration = std::stoul(timeStr);

                        // Extract player number (between "PLAYER: " and " KEY:")
                        std::string playerStr = line.substr(playerPos + 8, keyPos - playerPos - 9);
                        int playerNum = std::stoi(playerStr);

                        // Extract key (after "KEY: ")
                        std::string keyStr = line.substr(keyPos + 5);
                        char key = keyStr[0];

                        loadedSteps.addStep(iteration, playerNum, key);
                    }
                }
            }
        }
    }

    steps_file.close();
    return loadedSteps;
}

// Save steps to file helped by AI
void Steps::saveSteps(const std::string& filename) const {
    std::ofstream steps_file(filename);

    if (!steps_file.is_open()) {
        std::cout << "Error: Could not create steps file: " << filename << std::endl;
        return;
    }

    steps_file << "Loading the screens: " << screenFiles << '\n'; // Write screen files line (first line)

    steps_file << "Number of steps: " << steps.size() << "\n";  // Write number of steps (second line)

    // Write each step: iteration and key
    for (const auto& step : steps) {
        steps_file << "TIME: " << step.iteration
            << " PLAYER: " << step.PlayerNum
            << " KEY: " << step.key << "\n";
    }

    steps_file.close(); // Close the file

    std::cout << "Saved " << steps.size()
        << " steps to " << filename << std::endl;
}

// Get the next step for replay
bool Steps::getNextStep(size_t currentIteration, Step& outStep) {

    
	// Check if we've reached the end of the steps
    if (currentStepIndex >= steps.size()) {
        return false;
    }

    // Check if the current iteration matches the next step's iteration
    if (steps[currentStepIndex].iteration == currentIteration) {
        outStep = steps[currentStepIndex];
        currentStepIndex++;
        return true;
    }
    return false;
}

// Reset replay to beginning
void Steps::resetReplay() {
    currentStepIndex = 0;
    riddleStepIndex = 0;
}

// Check if there are more steps to replay
bool Steps::hasMoreSteps() const {
    return currentStepIndex < steps.size();
}


// Clear all steps
void Steps::clearSteps() {
    steps.clear();
    currentStepIndex = 0;
    riddleStepIndex = 0;
    screenFiles = "";
}

// Initialize for recording with screen file names
void Steps::initForRecording(const std::vector<std::string>& fileNames) {

    clearSteps(); // Clear any existing steps


    std::string screensString = ""; // Concatenate file names with '|' delimiter
    for (size_t i = 0; i < fileNames.size(); i++) {  // Iterate through file names
        if (i > 0) screensString += "|"; // Add delimiter for all but first
        screensString += fileNames[i]; // Append file name
    }

    screenFiles = screensString; // Set the concatenated string
}

// Add step if key belongs to a player
void Steps::addStepIfValid(size_t iteration, char ch, const Player& p1, const Player& p2) {
    if (p1.isMyKey(ch)) {// player 1 is identified with the sign '&' - we will register it as playerNum 1
        addStep(iteration, 1, ch);
    }
    else if (p2.isMyKey(ch)) { // player 2 is identified with the sign '$' - we will register it as playerNum 2
        addStep(iteration, 2, ch);
    }
    
}

// Get next riddle step (PlayerNum == 0)
bool Steps::getNextRiddleStep(Step& outStep) {
    // Scan forward from riddleStepIndex looking for PlayerNum == 0
    while (riddleStepIndex < steps.size()) {
        if (steps[riddleStepIndex].PlayerNum == 0) {
            outStep = steps[riddleStepIndex];
            riddleStepIndex++;
            return true;
        }
        riddleStepIndex++;  // Skip non-riddle steps
    }
    return false;  // No more riddle steps
}

