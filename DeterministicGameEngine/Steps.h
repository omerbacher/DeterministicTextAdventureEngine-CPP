#pragma once
#include <vector>
#include <string>
#include <utility>
#include "Player.h"
class Steps {
public:
    struct Step
    {
        size_t iteration; // Game cycle / iteration
        int PlayerNum;  // Player number (1 ,2 OR 0)  
        char key;        // Key pressed
    };

private:
    size_t riddleStepIndex;   // For riddle system 
    std::vector<Step> steps; // Vector of pairs: (game cycle / iteration, key pressed)
    size_t currentStepIndex;  // Current step index for replay mode
	std::string screenFiles; // Names of screen files associated with the steps
public:
    Steps() : currentStepIndex(0),riddleStepIndex(0) {  // Constructor
    }
	void setScreenFiles(const std::string& files) // Set screen file names
    { 
        screenFiles = files; 
    } 
    void addStep(size_t iteration,int playerNum, char step); // Add a step to the recording

    static Steps loadSteps(const std::string& filename);   // Load steps from file (static factory method)
    void saveSteps(const std::string& filename) const;    // Save steps to file

    bool getNextStep(size_t currentIteration, Step& outStep);  // Get the next step for replay mode
    void resetReplay(); // Reset replay to the beginning
	bool hasMoreSteps() const; // Check if there are more steps to replay
    void clearSteps();  // Clear all steps (reset everything)  
	void initForRecording(const std::vector<std::string>& fileNames); // Initialize for recording with screen file names
	void addStepIfValid(size_t iteration, char ch, const Player& p1, const Player& p2);// Add step if key belongs to a player
	bool getNextRiddleStep (Step& outStep);
   
};
