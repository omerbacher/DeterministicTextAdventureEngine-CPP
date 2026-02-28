#include "Riddle.h"
#include <string>
#include <cctype>
#include "Screen.h"
#include <iostream>
#include <cstring>	

// Check if the provided answer is correct- if correct, mark the riddle as solved, helped by AI
bool Riddle::checkAnswer(const char* userAnswer)
 {
     // Convert both strings to lowercase:
      std::string user = userAnswer;
      std::string correct = answer;

      for (char& c : user)
           c = std::tolower((unsigned char)c);

      for (char& c : correct)
          c = std::tolower((unsigned char)c);

       if (user == correct) {
            solved = true;
            return true;
        }
       return false;
}
bool Riddle::isSolved() const { // Check if the riddle has been solved
	return solved;
}

int Riddle::getRiddleID() const // Get the riddle ID
{
	return riddleID;
}
bool Riddle::isRiddle(char ch) { // Check if the character represents a riddle
	return ch == '?';
}
//AI helped to convert const char* to std::string
std::string Riddle::getQuestion() const {
    return std::string(question);
}
void Riddle::markAsSolved()
{
    solved = true;
}
//AI generated function to get hint
const char* Riddle::getHint() const
{
    return hint;
}
Point Riddle::getPosition() const
{
    return pos;
}
void Riddle::setPosition(const Point& p)
{
    pos = p;
}
void Riddle::resetRiddle() {
    solved = false;       
    pos = Point(0, 0);
}