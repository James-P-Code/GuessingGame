#pragma once

#include <iostream>
#include <limits> // std::numeric_limits<std::streamsize>::max()
#include <string>
#include <array>
#include "Tree.h"

class GuessingGame
{
private:
	static constexpr int PREVIOUS_QUESTION_AMOUNT = 5;
	static constexpr char POSITIVE_RESPONSE = 'Y';
	static constexpr char NEGATIVE_RESPONSE = 'N';
	Tree questionTree;
	std::string previousQandA[PREVIOUS_QUESTION_AMOUNT];
	std::array<std::pair<std::string, char>, PREVIOUS_QUESTION_AMOUNT> previousQuestions;
	bool errorStatusState = false;

	void askQuestion();
	void makeGuess(const std::string& nodeMessage);
	void getNewQuestion(const std::string& nodeMessage);
	const char yesOrNoResponse() const;
	void playAgain();
	void saveQuestionAndResponse(const std::string& nodeMessage, const char yesOrNo);

public:
	GuessingGame();
	void startGame();
	const bool errorStatus();
};