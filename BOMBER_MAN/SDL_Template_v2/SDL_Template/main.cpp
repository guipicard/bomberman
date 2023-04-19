#include <SDL.h>
#include "Game.h"

int main(int argc, char* argv[])
{
	Game* game = new Game();
	game->Run();
	delete game;
	game = nullptr;
	return 0;
}
