// Author:  James Pochas
// Date:  March 23, 2023
// Assignment:  M3 Capstone Project: Design-Draft a Program

#pragma once

#include <string>
#include <fstream>
#include <memory>

// This class defines the binary tree structure that is a core part of this program.  
// The main structure of the tree consists of Nodes, where each node contains a string that represents
// either a question to ask or an item to guess.  Each Node can also point to 2 child Nodes, where each
// child nodes represents the data that will be used depending on the user's yes/no answer to a question.
// A yes answer corresponds with the positiveNode, and a no answer to a negativeNode.
// Additionally the class also keeps track of the root node of the entire tree structure, as well as the
// game's current position within the tree (currentNode).  This class is designed to never expose any of
// these pointers outside of the class

class Tree
{
private:
	struct Node
	{
		std::string message;
		std::unique_ptr<Node> positiveNode;
		std::unique_ptr<Node> negativeNode;

		Node() = delete;
		Node(const std::string& message) : message(message) {}
	};

	std::unique_ptr<Node> root;
	Node* currentNode = nullptr; // used to track the current position within the tree
	static constexpr char nullChildMarker = '#'; // used in the save file for marking null nodes
	static constexpr char positiveResponse = 'Y';
	static constexpr char negativeResponse = 'N';

	void saveToFile(std::ofstream& saveFile, const Node* nodeToSave) const;
	void loadFromFile(std::ifstream& saveFile, std::unique_ptr<Node>& nodeToLoad);

public:
	void createNewTreeFile();
	const bool loadFromFile();
	const std::string& getMessage() const;
	const bool isCurrentNodeLeaf() const;
	void moveBasedOnResponse(const char yesOrNo);
	void addNewQuestion(const std::string& newGuess, const std::string& newQuestion, const char yesOrNo);
	void resetTreePosition();
};