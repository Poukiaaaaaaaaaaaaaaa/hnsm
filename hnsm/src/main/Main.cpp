#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cmath>

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

#undef _main

std::vector<std::string> imagePaths = {
	"../res/img/normal.png",
	"../res/img/hovered.png",
	"../res/img/boat.jpg"
};

std::vector<std::string> audioPaths = {
};

std::vector<std::string> musicPaths = {
};

void callback(void* data)
{
	((Game*)data)->shouldQuit = true;
}

int main(int argc, char* args[])
{

	SDL_Init(SDL_INIT_EVERYTHING); Pa_Initialize();

	Game g(
		"He Needs Some Milk",
		"config/gameconfig",
		(SDL_WindowFlags)(SDL_WINDOW_SHOWN),
		(SDL_RendererFlags)(SDL_RENDERER_ACCELERATED),
		imagePaths,	audioPaths,	musicPaths
	);

	// tests (enlever si besoin)
	int s = g.get_w() / 15;
	std::vector<ButtonData> button = {
		ButtonData(g.t[1], g.t[0], callback, &g, { 0, 0, s, s })
	};
	Menu rm(g, MenuData(g.t[2], button), GUI);
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