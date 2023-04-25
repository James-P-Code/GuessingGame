// Author:  James Pochas
// Date:  April 15, 2023
// Assignment:  M4 Capstone Project: Capstone Preparation

#include "GuessingGame.h"

// Most of the program's functionality is handled within the GuessingGame and Tree classes.
// Here we just create a GuessingGame object and start the game.
// When we return to this function we check the game's error status to return the overall exit status of the program

// Requirement #4: Variables
// Requirement #9: Control
int main()
{
	GuessingGame game;
	game.startGame();

	if (game.errorStatus())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}