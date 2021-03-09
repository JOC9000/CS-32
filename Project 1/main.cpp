#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;
#include "globals.h"
#include "Vampire.h"
#include "Player.h"
#include "Arena.h"
#include "Game.h"
#include "History.h"

int main()
{
	// Create a game
	// Use this instead to create a mini-game:   Game g(3, 5, 2);
	Game g(5, 4, 5);

	// Play the game
	g.play();
}
