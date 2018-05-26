#include "Game.h"

void Game::tick_start()
{
	start = SDL_GetPerformanceCounter();
	SDL_GetMouseState(&mouse_x, &mouse_y);
}

void Game::tick_end()
{
	end = SDL_GetPerformanceCounter();

	if (tick_target != 0 && end - start < tick_target)
		SDL_Delay((Uint32)((float)(tick_target - end + start) / freq * 1000));

	pmouse_x = mouse_x; pmouse_y = mouse_y;

	old_start = start;
	last = SDL_GetPerformanceCounter();
}

int Game::get_current_framerate() const
{
	return (int)((float)freq / (last - old_start));
}

int Game::get_frame_length() const
{
	return (int)(((float)tick_target / freq) * 1000);
}

void Game::set_frame_rate(unsigned fps)
{
	fps_target = fps;
	tick_target = (fps > 0) ? freq / fps_target : 0;
}

Game::Game(std::string title, std::string cfgPath,
	SDL_WindowFlags wflags, SDL_RendererFlags rflags,
	std::vector<std::string> &imagePaths,
	std::vector<std::string> &audioPaths,
	std::vector<std::string> &musicPaths
) : title(title), freq(SDL_GetPerformanceFrequency()),
	wflags(wflags), rflags(rflags)
{
	if (TTF_Init())
	{
		Log::toSdlError("error.log", "TTF_Init: ", __FILE__, __LINE__);
	}

	int wx, wy, ww, wh;
	std::ifstream config(cfgPath);
	config >> fps_target >> ww >> wh;

	tick_target = fps_target ? freq / fps_target : 0;
	config.close();

	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode(0, &dm);

	if (!ww || !wh)
	{
		ww = dm.w; wh = dm.h;

		std::ofstream writeCfg(cfgPath);
		writeCfg << fps_target << '\n' << ww << "\n" << wh << "\n";
		writeCfg.close();
	}

	wx = dm.w / 2 - ww / 2;
	wy = dm.h / 2 - wh / 2;

	dim = { wx, wy, ww, wh };

	w = SDL_CreateWindow(title.c_str(), wx, wy, ww, wh, wflags);
	if (w == nullptr)
	{
		Log::toSdlError("error.log", "SDL_CreateWindow: ", __FILE__, __LINE__);
	}

	r = SDL_CreateRenderer(w, -1, rflags);
	if (r == nullptr)
	{
		Log::toSdlError("error.log", "SDL_CreateRenderer: ", __FILE__, __LINE__);
	}

	for (unsigned i = 0; i < imagePaths.size(); i++)
	{
		t.push_back(IMG_LoadTexture(r, imagePaths[i].c_str()));
		if (t[i] == nullptr)
		{
			Log::toSdlError("error.log", "IMG_LoadTexture: ", __FILE__, __LINE__);
		}
	}

	//load the audio data
	for (unsigned i = 0; i < audioPaths.size(); i++)
	{
		sound.push_back(AudioData(audioPaths[i].c_str()));
	}

	for (unsigned i = 0; i < musicPaths.size(); i++)
	{
		loop.push_back(AudioData(musicPaths[i].c_str()));
	}

	camera.x = camera.y = 0;

	a.startStream();
}

void Game::moveCamera(int xShift, int yShift)
{
	camera.x += xShift;
	camera.y -= yShift;
}

void Game::restoreCamera()
{
	camera.x = camera.y = 0;
}

void Game::events()
{
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			shouldQuit = true;
		}
	}

	for (unsigned i = 0; i < gui.size(); i++)
	{
		gui[i]->events();
	}

	if (!isPaused)
	{
		for (unsigned i = 0; i < game.size(); i++)
		{
			game[i]->events();
		}
	}
}

void Game::process()
{
	for (unsigned i = 0; i < gui.size(); i++)
	{
		gui[i]->update();
	}

	if (!isPaused)
	{
		
		for (unsigned i = 0; i < game.size(); i++)
		{
			physicalEngine[i].process();	
		}
	
		for (unsigned i = 0; i < game.size(); i++)
		{
			game[i]->update();
		}
	}
}

void Game::render()
{
	SDL_RenderClear(r);
	for (unsigned i = 0; i < layers.size(); i++)
	{
		for (unsigned j = 0; j < layers[i].size(); j++)
		{
			layers[i][j].draw(r, camera);
		}
	}
	SDL_RenderPresent(r);
}

void Game::resize(int width, int height)
{
	SDL_SetWindowSize(w, width, height);
	dim.w = width; dim.h = height;
}

void Game::setFullscreenMode(FullscreenMode mode)
{
	switch (mode)
	{
	case FULLSCREEN:
		SDL_SetWindowFullscreen(w, SDL_WINDOW_FULLSCREEN);
		break;

	case DESKTOP:
		SDL_SetWindowFullscreen(w, SDL_WINDOW_FULLSCREEN_DESKTOP);
		break;

	case WINDOWED:
		SDL_SetWindowFullscreen(w, NULL);
		break;
	}
}

int Game::get_w() const
{
	return dim.w;
}

int Game::get_h() const
{
	return dim.h;
}

Uint64 Game::get_freq() const
{
	return freq;
}

SDL_WindowFlags Game::getWindowFlags() const
{
	return wflags;
}

SDL_RendererFlags Game::getRendererFlags() const
{
	return rflags;
}

void Game::setRendererFlags(SDL_RendererFlags flags)
{
	SDL_DestroyRenderer(r);
	r = SDL_CreateRenderer(w, -1, flags);
}

Game::~Game()
{
	cleanup();
}

void Game::cleanup()
{
	SDL_DestroyWindow(w);
	SDL_DestroyRenderer(r);
	a.stopStream(ABORT);
}
