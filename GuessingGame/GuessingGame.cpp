#include "GuessingGame.h"

GuessingGame::GuessingGame()
{
	for (auto& pq : previousQuestions)
	{
		pq.first = "";
		pq.second = ' ';
	}
}

void GuessingGame::startGame()
{
	std::cout << "Guessing Game!\nPlease think of an object and I will try to guess it!\n\n";
	
	if (!questionTree.loadFromFile())
	{
		std::cerr << "Error loading file!";
		std::cout << "\nWould you like to create a new file?\n";
		if (yesOrNoResponse() == POSITIVE_RESPONSE)
		{
			questionTree.createNewTreeFile();
		}
		else
		{
			errorStatusState = true;
			return;
		}
	}

	if (questionTree.getMessage().empty())
	{
		std::cerr << "Error loading questions!";
		errorStatusState = true;
		return;
	}
	else
	{
		askQuestion();
	}
}

void GuessingGame::askQuestion()
{
	std::string nodeMessage = questionTree.getMessage();

	while (!questionTree.isCurrentNodeLeaf())
	{
		std::cout << nodeMessage << '\n';
		const char userRespone = yesOrNoResponse();

		saveQuestionAndResponse(nodeMessage, userRespone);
		questionTree.moveBasedOnResponse(userRespone);
		nodeMessage = questionTree.getMessage();
	}

	makeGuess(nodeMessage);
}

// Gets the user's repsonse to any Yes/No question
const char GuessingGame::yesOrNoResponse() const
{
	char userResponse;

	do
	{
		std::cout << "(Y)es or (N)o: ";
		std::cin >> userResponse;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  //  we only want the first character so we'll ignore everything else
		userResponse = std::toupper(userResponse);  // we'll just validate against uppercase Y or N

		if (userResponse != POSITIVE_RESPONSE && userResponse != NEGATIVE_RESPONSE)
		{
			std::cout << "Invalid Response!  Please try again...\n";
		}
	}  while (userResponse != POSITIVE_RESPONSE && userResponse != NEGATIVE_RESPONSE);

	return userResponse;
}

void GuessingGame::playAgain()
{
	std::cout << "Would you like to play again?\n";
	
	if (yesOrNoResponse() == POSITIVE_RESPONSE)
	{
		questionTree.resetTreePosition();
		askQuestion();
	}
	else
	{
		std::cout << "Thank you for playing!\n";
	}
}

/* This function will be called once the tree comes to a node that is a leaf (a node with null child nodes).
*  A leaf node in this tree means that there are no more questions to ask, and it is time to guess what the
*  user might be thinking of  */
void GuessingGame::makeGuess(const std::string& guess)
{
	std::cout << "Are you thinking of " << guess << "?\n";

	if (yesOrNoResponse() == POSITIVE_RESPONSE)
	{
		std::cout << "\nI win!!!\n";
		playAgain();
	}
	else
	{
		getNewQuestion(guess);
	}
}

/* If the program did not correctly guess what the user was thinking of it will prompt the user for
*  information so that it might be able to guess that object in the future.  The user will be prompted
*  to enter what object they were thinking of, a Yes/No question to help guess that object, and the answer
*  to that question.  The tree will then be updated with this new info.  The current Node (the object that
*  the program guessed) will be updated to instead contain the user's new question.  The positive and negative
*  nodes will be updated with the object that was just guessed, and the object the user was thinking of. */
void GuessingGame::getNewQuestion(const std::string& guess)
{
	std::string newGuess, newQuestion;
	char newQuestionAnswer;

	std::cout << "\nWhat were you thinking of?\n";
	std::getline(std::cin, newGuess);

	std::cout << "As a reminder here are the previous " << PREVIOUS_QUESTION_AMOUNT << " questions and responses:\n";
	
	for (const auto& prev : previousQuestions)
	{
		if (!prev.first.empty())
		{
			std::cout << prev.first << " Your Response: " << prev.second << '\n';
		}
	}

	std::cout << "\nPlease enter a Yes/No question to help me distinguish " << guess << " from " << newGuess << '\n';
	std::getline(std::cin, newQuestion);

	std::cout << "What is the answer to your question for " << newGuess << '\n';
	newQuestionAnswer = yesOrNoResponse();

	questionTree.addNewQuestion(newGuess, newQuestion, newQuestionAnswer);
	playAgain();
}

void GuessingGame::saveQuestionAndResponse(const std::string& question, const char userResponse)
{
	if (previousQuestions.back().first.empty())
	{
		for (size_t i = 0; i < PREVIOUS_QUESTION_AMOUNT; ++i)
		{
			if (previousQuestions[i].first.empty())
			{
				previousQuestions[i].first = question;
				previousQuestions[i].second = userResponse;
				break;
			}
		}
	}
	else
	{
		for (size_t i = 0; i < PREVIOUS_QUESTION_AMOUNT - 1; ++i)
		{
			previousQuestions[i + 1] = previousQuestions[i];
		}
		previousQuestions.front().first = question;
		previousQuestions.front().second = userResponse;
	}
}

const bool GuessingGame::errorStatus()
{
	return errorStatusState;
}