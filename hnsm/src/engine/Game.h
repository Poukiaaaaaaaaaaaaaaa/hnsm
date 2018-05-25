#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <sstream>

//SDL headers
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <SDL2\SDL_ttf.h>
#include <SDL2\SDL_mixer.h>

//libsndfile headers
#include <libsndfile\sndfile.h>

//engine headers
#include "Log.h"
#include "GObject.h"
#include "Audio.h"
#include "Pair.h"
#include "UI.h"

#include <PObject.h>

#undef _main

enum FullscreenMode
{
	FULLSCREEN, DESKTOP, WINDOWED
};

class Game
{
private:
	//physics engine members
	static const int g = 5;

	//window specifics
	std::string title;
	SDL_WindowFlags wflags;
	SDL_RendererFlags rflags;

	//framerate management
	Uint64 freq;
	Uint64 start = 0;
	Uint64 old_start = 0;
	Uint64 end = 0;
	Uint64 last = 0;

public:
	//framerate management
	int fps_target;
	Uint64 tick_target;

	//utility members
	bool shouldQuit = false;
	bool paused = false;

	//event handling
	SDL_Event e;
	int mouse_x, mouse_y;
	int pmouse_x, pmouse_y;

	//GUI
	std::vector<UI*> gui;
	std::vector<UI*> game;

	//graphics management
	SDL_Window *w;
	SDL_Rect dim;
	SDL_Renderer *r;
	std::vector<SDL_Texture*> t; //array containing every texture
	std::vector<std::vector<GObject>> layers;
	std::vector<PObject> physicalEngine;
	Pair camera;

	//audio members
	Audio a;
	std::vector<AudioData> sound;
	std::vector<AudioData> loop;

public:
	//framerate management
	void tick_start();
	void tick_end();
	int get_current_framerate() const;
	int get_frame_length() const; //in ms
	void set_frame_rate(unsigned fps);

	Game(std::string title, std::string cfgPath,
		SDL_WindowFlags wflags, SDL_RendererFlags rflags,
		std::vector<std::string> &imagePaths,
		std::vector<std::string> &audioPaths,
		std::vector<std::string> &musicPaths
	);

	//utility
	void resize(int w, int h);
	void setFullscreenMode(FullscreenMode);

	//camera movements
	void moveCamera(int x, int y);
	void restoreCamera();
	//void setCamera(int x, int y);

	//getters
	int get_w() const;
	int get_h() const;
	Uint64 get_freq() const;
	SDL_WindowFlags getWindowFlags() const;
	SDL_RendererFlags getRendererFlags() const;
	void setRendererFlags(SDL_RendererFlags flags);

	//main functions
	void events();
	void process();
	void render();

	//cleanup
	~Game();
	void cleanup();
};
