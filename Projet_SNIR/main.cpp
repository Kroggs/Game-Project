#include <SFML/Graphics.hpp>

#include "game.h"
#include <Windows.h>
#include <SFML/Network.hpp>

int main()
{
	Game game;
	game.Init();
	game.InitShaders();

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
*

//Optimisation
	Load Map from file
	Load map with chunks
	Optimise Collision detection
	remove LevelInformation struct & optimise map class


//Gameplay
	inventory
	interactions (chests)
	draw a more suitable 1st map
	NPCs (salesman)

//Network
	client -> queryServer -> parsePlayerList -> getPositions
*/