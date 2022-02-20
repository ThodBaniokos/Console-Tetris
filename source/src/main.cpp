#include <iostream>
#include "Game.hpp"

using namespace std;

int main(void)
{
	// init rand's seed with current time
	srand(time(nullptr));

	// create game class
	Game game;

	// run game
	game.run();

	// cleanup game
	game.exit();

	return 0;
}
