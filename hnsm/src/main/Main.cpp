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

const std::string configPath = "../config/gameconfig";

void callback(void* data)
{
	((Game*)data)->shouldQuit = true;
}

int main(int argc, char* args[])
{

	SDL_Init(SDL_INIT_EVERYTHING); Pa_Initialize();

	Game g(
		"He Needs Some Milk",
		configPath,
		(SDL_WindowFlags)(SDL_WINDOW_SHOWN),
		(SDL_RendererFlags)(SDL_RENDERER_ACCELERATED)
	);

	// Encore d'autres tests
	FileConfig level(g.config.getPath("levels"));
	 
	g.loadLevel(level.getPath("test"));

	// tests (enlever si besoin)
	/*FileConfig texturePaths(g.config.getStr("texture"));

	int s = g.get_w() / 10;
	 
	std::vector<ButtonData> button = {
		ButtonData(IMG_LoadTexture(g.r, texturePaths.getPath("button_normal").c_str()),
		           IMG_LoadTexture(g.r, texturePaths.getPath("button_hovered").c_str()),
				   callback, &g, { 0, 0, s, s })
	};
	Menu rm(g, MenuData(IMG_LoadTexture(g.r, texturePaths.getPath("background").c_str()), button), GUI);*/
	// tests

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
