#pragma once
#include <iostream>
#include <cstring>
#include "Utils.h"
#include "Point.h"
class Riddle
{
private:
	int riddleID; // riddle ID
	const char* question; // riddle question
	const char* answer; // riddle answer
	bool solved; // is riddle solved
	const char* hint; // riddle hint
	Point pos; // position of the riddle in the screen
public:
	Riddle() : riddleID(-1), question(nullptr), answer(nullptr), solved(false), hint(nullptr), pos(0, 0) {}
	Riddle(int id, const char* q, const char* a, const char* h) :riddleID(id), question(q), answer(a), solved(false), hint(h), pos(0, 0) {}
	// Copy Constructor
	Riddle(const Riddle& other) : riddleID(other.riddleID), solved(other.solved), pos(other.pos)
	{
		question = other.question ? allocateString(other.question) : nullptr;
		answer = other.answer ? allocateString(other.answer) : nullptr;
		hint = other.hint ? allocateString(other.hint) : nullptr;
	}

	// Assignment Operator
	Riddle& operator=(const Riddle& other)
	{
		if (this != &other)
		{
			// Free existing memory
			if (question) delete[] question;
			if (answer) delete[] answer;
			if (hint) delete[] hint;

			// Copy data
			riddleID = other.riddleID;
			solved = other.solved;
			pos = other.pos;
			question = other.question ? allocateString(other.question) : nullptr;
			answer = other.answer ? allocateString(other.answer) : nullptr;
			hint = other.hint ? allocateString(other.hint) : nullptr;
		}
		return *this;
	}

	// Destructor
	~Riddle()
	{
		if (question) delete[] question;
		if (answer) delete[] answer;
		if (hint) delete[] hint;
	}
	static constexpr int COLOR = 1; // Light Purple
	// functions declaration
	bool checkAnswer(const char* userAnswer); // check if the answer is correct
	bool isSolved() const; // check if the riddle is solved
	void markAsSolved(); // mark the riddle as solved
	int getRiddleID() const; // get the riddle ID
	static bool isRiddle(char ch);	 // check if a character represents a riddle
	std::string getQuestion() const; // get the riddle question
	const char* getHint() const; // get the riddle hint
	Point getPosition() const; // get the position of the riddle
	void setPosition(const Point& p); // set the position of the riddle
	void resetRiddle(); // reset the riddle to unsolved state
};

