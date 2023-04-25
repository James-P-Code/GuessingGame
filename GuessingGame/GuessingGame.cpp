// Author:  James Pochas
// Date:  April 15, 2023
// Assignment:  M4 Capstone Project: Capstone Preparation

#include "GuessingGame.h"

// This is the main function to start the game.  It will call a function that attempts to load saved data from a text file
// into the tree.  If there is something wrong with the file stream it will output an error message and ask the user if
// they would like to create a new save file with a small amount of default questions.  If the user does not choose to create
//  a new save file it set the error state boolean to true and then return to main().  If the file loads correctly or the user
// does create a new file in the case of an error it will then call the function that starts the process of asking questions

// Requirement #3:  Input and Output
// Requirement #9:  Control
void GuessingGame::startGame()
{
	std::cout << "Guessing Game!\nPlease think of an object and I will try to guess it!\n\n";
	
	if (!questionTree.loadFromFile())
	{
		std::cerr << "Error loading file!";
		std::cout << "\nWould you like to create a new file?\n";
		if (yesOrNoResponse() == positiveResponse)
		{
			questionTree.createNewTreeFile();
		}
		else
		{
			errorStatusState = true;
			return;
		}
	}

	askQuestion();
}

// Used to report the error status in the event that the save file could not properly load
const bool GuessingGame::errorStatus() const
{
	return errorStatusState;
}

// This function retrieves the string data from the current node of the tree.  While the current node of the tree is not a leaf node
//  it will ask questions, and get the user's response to those questions.  It will also call other functions that save the question and
//  response to an array, and move down the tree based on the user's response.  If the current node of the tree has no child nodes it will
//  not ask a question and instead call a function that attepmts to guess what the user is thinking of

// Requirement #3:  Input and Output
// Requirement #7:  Iteration
// Requirement #8:  Interaction
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
		currentQuestionNumber++;
	}

	makeGuess(nodeMessage);
}

// Gets the user's repsonse to any Yes/No question

// Requirement #3:  Input and Output
// Requirement #7:  Iteration
// Requirement #8:  Interaction
// Requirement #9:  Control
const char GuessingGame::yesOrNoResponse() const
{
	char userResponse;

	do
	{
		std::cout << "(Y)es or (N)o: ";
		std::cin >> userResponse;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  //  we only want the first character so we'll ignore everything else
		userResponse = std::toupper(userResponse);  // we'll just validate against uppercase Y or N

		if (userResponse != positiveResponse && userResponse != negativeResponse)
		{
			std::cout << "Invalid Response!  Please try again...\n";
		}
	} while (userResponse != positiveResponse && userResponse != negativeResponse);

	return userResponse;
}

// Saves some of the recent questions and responses to an array

// Requirement #5:  Arrays
void GuessingGame::saveQuestionAndResponse(const std::string& question, const char userResponse)
{
	previousQandA[currentQuestionNumber % previousQuestionAmount] = question + " YOUR RESPONSE: ";
	previousQandA[currentQuestionNumber % previousQuestionAmount].append(userResponse == positiveResponse ? "Yes" : "No");
}

// This function will be called once the tree comes to a node that is a leaf (a node with null child nodes).
// A leaf node in this tree means that there are no more questions to ask, and it is time to guess what the
// user might be thinking of.  If the program does not correctly guess what the user is thinking of it will
// call a function that gets info from the user to add to the tree

// Requirement #3:  Input and Output
// Requirement #9:  Control
// Requirement #8:  Interaction
void GuessingGame::makeGuess(const std::string& guess)
{
	std::cout << "Are you thinking of " << guess << "?\n";

	if (yesOrNoResponse() == positiveResponse)
	{
		std::cout << "\nI win!!!\n";
		playAgain();
	}
	else
	{
		getNewQuestion(guess);
	}
}

// If the program did not correctly guess what the user was thinking of it will prompt the user for
// information so that it might be able to guess that object in the future.  The user will be prompted
// to enter what object they were thinking of, and then a a brief reminder of some of the recent questions
// and responses will be displayed.  The user will then be asked to enter a new Yes/No question that
// can be added to the tree for future sessions.  Finally, it will ask the user if they would like to
// play again

// Requirement #3:  Input and Output
// Requirement #5:  Arrays
// Requirement #7:  Iteration
// Requirement #8:  Interaction
// Requirement #9:  Control
void GuessingGame::getNewQuestion(const std::string& guess)
{
	std::string newGuess, newQuestion;
	char newQuestionAnswer;

	std::cout << "\nWhat were you thinking of?\n";
	std::getline(std::cin, newGuess);

	std::cout << "As a reminder here are a few of the previous questions and responses:\n";

	for (const std::string& previousQuestion : previousQandA)
	{
		if (!previousQuestion.empty())
		{
			std::cout << previousQuestion << '\n';
		}
	}

	std::cout << "\nPlease enter a question that can be answered with Yes/No to help me distinguish " << guess << " from " << newGuess << '\n';
	std::getline(std::cin, newQuestion);

	if (newQuestion.back() != '?')
	{
		newQuestion += '?';
	}

	std::cout << "What is the answer to your question for " << newGuess << '\n';
	newQuestionAnswer = yesOrNoResponse();

	questionTree.addNewQuestion(newGuess, newQuestion, newQuestionAnswer);
	playAgain();
}

// Ask the user if they would like to play again, and if so reset the game to a starting state, otherwise display a quit message

// Requirement #3:  Input and Output
// Requirement #8:  Interaction
// Requirement #9:  Control
void GuessingGame::playAgain()
{
	std::cout << "Would you like to play again?\n";
	
	if (yesOrNoResponse() == positiveResponse)
	{
		currentQuestionNumber = 0;
		resetArray();
		questionTree.resetTreePosition();
		askQuestion();
	}
	else
	{
		std::cout << "Thank you for playing!\n";
	}
}

// Resets the array of recent questions and responses, used if the player wants to play again

// Requirement #5:  Arrays
// Requirement #7:  Iteration
void GuessingGame::resetArray()
{
	for (std::string& previousQuestion : previousQandA)
	{
		previousQuestion.clear();
	}
}