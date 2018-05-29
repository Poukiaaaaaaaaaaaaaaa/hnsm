#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cmath>
#include <fstream>

//SDL headers
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <SDL2\SDL_ttf.h>
#include <SDL2\SDL_mixer.h>

//PortAudio headers
#include <PortAudio\portaudio.h>

//engine headers
#include "Log.h"
#include "GObject.h"
#include "Game.h"
#include "Misc.h"

//main headers
#include "Button\Button.h"
#include "ButtonData\ButtonData.h"
#include "Menu\Menu.h"
#include "RollMenu\RollMenu.h"

#include "FileConfig/FileConfig.h"

#undef _main

std::string configPath = "../config/gameconfig";

int main(int argc, char* args[])
{

	SDL_Init(SDL_INIT_EVERYTHING); Pa_Initialize();

	/*
	 *	Création de l'objet global (voir Game.h, Game.cpp) dans
	 *	le dossier "src/engine".
	 *
	 */
	Game g(
		"He Needs Some Milk!",
		configPath,
		SDL_WINDOW_SHOWN,
		SDL_RENDERER_ACCELERATED,
		/**/
	);

	/*
	 *	Boucle principale.
	 *	Voir Game.h pour toutes les méthodes utilisées.
	 *
	 */
	while (!g.shouldQuit)
	{
		g.tick_start();

		g.events();
		g.process();
		g.render();

		g.tick_end();
	}

	SDL_Quit(); Pa_Terminate();
	return EXIT_SUCCESS;
}
