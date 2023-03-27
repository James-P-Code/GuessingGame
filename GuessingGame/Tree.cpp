// Author:  James Pochas
// Date:  March 23, 2023
// Assignment:  M3 Capstone Project: Design-Draft a Program

#include "Tree.h"

// If there is an issue with the save file then this function can create a new save file with a minimal amount of questions. 
// The user will be asked if they would like to create a new save file, and if so then this function will be called.
// After creating and saving the new file it will prepare to start the game by setting the current Node to the root
void Tree::createNewTreeFile()
{
	root = std::make_unique<Node>("Is it alive?");
	root->positiveNode = std::make_unique<Node>("Does it walk on 4 legs?");
	root->positiveNode->negativeNode = std::make_unique<Node>("Human");
	root->positiveNode->positiveNode = std::make_unique<Node>("Dog");
	root->negativeNode = std::make_unique<Node>("Does it have wheels?");
	root->negativeNode->positiveNode = std::make_unique<Node>("Car");
	root->negativeNode->negativeNode = std::make_unique<Node>("Computer");

	std::ofstream saveFile("questions.txt");
	saveToFile(saveFile, root.get());
	saveFile.close();
	currentNode = root.get();
}

// This saves the data in the tree to a file.  The parameters specify the file stream that will be used to save the file, 
//  and a pointer to the node that is currently being saved to the file.  Because this file will later be used to load the tree
// the null nodes are marked using a special character.  If a node is not null then its data is written to the file, and
//  recursive calls are made to save that node's child nodes
void Tree::saveToFile(std::ofstream& saveFile, const Node* nodeToSave) const
{
	if (nodeToSave)
	{
		saveFile << nodeToSave->message << '\n';
		saveToFile(saveFile, nodeToSave->positiveNode.get());
		saveToFile(saveFile, nodeToSave->negativeNode.get());
	}
	else
	{
		saveFile << nullChildMarker << '\n';
	}
}

// This function is the public version of what is called to load the save file.  The reasoning here is that the node pointers
// are never exposed outside of this class, but loading the file will at first require a pointer to the root node.  This public
// function is meant to facilitate that process.  This function also does basic error checking on the file stream of the save file.
// If there is an error this function will return false, otherwise it will return true
const bool Tree::loadFromFile()
{
	std::ifstream saveFile("questions.txt");

	if (!saveFile)
	{
		return false;
	}

	loadFromFile(saveFile, root);
	saveFile.close();
	currentNode = root.get();
	
	return true;
}

// This is the private version of the file loading function.  The parameters are the file stream to read from and a reference to 
// the node's pointer within the tree.  The following article explains my reasons for what type of pointer or reference I use:
// https://herbsutter.com/2013/06/05/gotw-91-solution-smart-pointer-parameters/
// This function reads a line from the file stream and checks if it is the special character used to mark a null node.  If it
// is not this special character then it makes recursive calls to load the the child nodes from the file
void Tree::loadFromFile(std::ifstream& saveFile, std::unique_ptr<Node>& nodeToLoad)
{
	std::string fileInput;
	std::getline(saveFile, fileInput);

	if (fileInput.front() != nullChildMarker)
	{
		nodeToLoad = std::make_unique<Node>(fileInput);
		loadFromFile(saveFile, nodeToLoad->positiveNode);
		loadFromFile(saveFile, nodeToLoad->negativeNode);
	}
}

// If the program guesses incorrectly it will ask the user for some information so it can "learn" new items to guess.  Much of that
// process is handled in the GuessingGame class, but that info will need to be added to the tree and then the new tree data must
// be saved.  That is what this function does.  The parameters are the new item that can be guessed, a new question that can help
// to identify that item, and the yes/no answer to that new question.  The data that is saved to the child nodes depends on the
// yes/no answer to the question, so that is handled first.  Then the data of the currentNode (the current position in the tree
// at the end of the game) is changed to the new question the user provided.  The new tree is then saved to the save file
void Tree::addNewQuestion(const std::string& newGuess, const std::string& newQuestion, const char yesOrNo)
{
	if (yesOrNo == positiveResponse)
	{
		currentNode->positiveNode = std::make_unique<Node>(newGuess);
		currentNode->negativeNode = std::make_unique<Node>(currentNode->message);
	}
	else if (yesOrNo == negativeResponse)
	{
		currentNode->negativeNode = std::make_unique<Node>(newGuess);
		currentNode->positiveNode = std::make_unique<Node>(currentNode->message);
	}

	currentNode->message = newQuestion;

	std::ofstream saveFile("questions.txt");
	saveToFile(saveFile, root.get());
	saveFile.close();
}

// Used by the GuessingGame class to get the data of the current position in the tree
const std::string& Tree::getMessage() const
{
	return currentNode->message;
}

// The game will either ask the user a question, or attempt to guess what the user is thinking of.  If the current position in the tree
// has child nodes then asking a question is the correct action, otherwise it's time to attempt a guess.  This function checks if the
// currentNode's child nodes are null or not, and this result is used by the guessingGame class to determine the next action
const bool Tree::isCurrentNodeLeaf() const
{
	if (currentNode->positiveNode && currentNode->negativeNode)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// The path that is taken down the tree depends on the user's response to any questions.  The currentNode pointer is used to keep
// track of the current position within the tree.  This function moves down the tree based on the user's yes/no response
void Tree::moveBasedOnResponse(const char yesOrNo)
{
	if (yesOrNo == positiveResponse)
	{
		currentNode = currentNode->positiveNode.get();
	}
	else if (yesOrNo == negativeResponse)
	{
		currentNode = currentNode->negativeNode.get();
	}
}

// Reset the current position in the tree back to the root, used to reset the game and play again
void Tree::resetTreePosition()
{
	currentNode = root.get();
}