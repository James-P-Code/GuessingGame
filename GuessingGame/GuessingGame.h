// Author:  James Pochas
// Date:  March 23, 2023
// Assignment:  M3 Capstone Project: Design-Draft a Program

#pragma once

#include <iostream>
#include <limits> // std::numeric_limits<std::streamsize>::max()
#include <string>
#include "Tree.h"

// This class runs the actual game part of this project.  This includes asking questions, getting responses to those questions,
// attempting to guess what the user is thinking of, getting new info from the user in the case of an incorrect guess and
// asking the user if they want to play the game again

class GuessingGame
{
public:
	void startGame();
	const bool errorStatus() const;

private:
	static constexpr int previousQuestionAmount = 5;  // how many recent questions and responses should be saved, used to size the array
	static constexpr char positiveResponse = 'Y';
	static constexpr char negativeResponse = 'N';
	Tree questionTree;
	std::string previousQandA[previousQuestionAmount];
	int currentQuestionNumber = 0; // used as an index for the previous questions and answers array
	bool errorStatusState = false;

	void askQuestion();
	const char yesOrNoResponse() const;
	void saveQuestionAndResponse(const std::string& nodeMessage, const char yesOrNo);
	void makeGuess(const std::string& nodeMessage);
	void getNewQuestion(const std::string& nodeMessage);
	void playAgain();
	void resetArray();
};