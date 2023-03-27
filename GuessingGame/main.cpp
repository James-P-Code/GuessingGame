// Author:  James Pochas
// Date:  March 23, 2023
// Assignment:  M3 Capstone Project: Design-Draft a Program

#include "GuessingGame.h"

// Most of the program's functionality is handled within the GuessingGame and Tree classes.
// Here we just create a GuessingGame object and start the game.
// When we return to this function we check the game's error status to return the overall exit status of the program
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