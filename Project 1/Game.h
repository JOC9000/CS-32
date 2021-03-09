#ifndef GAME_H

#define GAME_H

class Arena;

#include <string>

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nVampires);
	~Game();

	// Mutators
	void play();

private:
	Arena* m_arena;

	// Helper functions
	std :: string takePlayerTurn();
};
#endif