#include "GuessingGame.h"

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