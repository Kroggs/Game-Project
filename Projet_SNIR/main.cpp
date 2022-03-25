#include <SFML/Graphics.hpp>

#include "game.h"
#include <Windows.h>


int main()
{
	Game game;
	game.Init();


	while (game.IsRunning())
	{ 
		if (!game.IsClosed())
		{
			game.Update();
			game.Render();
		}
		else
		{
			game.End();
		}
	}

	return EXIT_SUCCESS;
}

/*  TODO 
inventory
interactions (chests)
draw a more suitable 1st map
NPCs (salesman)
*/