#pragma once
#include <iostream>
#include <vector>

//SDL headers
#include <SDL2\SDL.h>

#undef _main

class Animation
{
public:
	SDL_Texture *t;

	SDL_Rect clipSize;
	unsigned clipc;
	unsigned currentClip;
	std::vector<SDL_Rect> clips;

	int delay; //ms
	Uint64 tick_delay;
	Uint64 start;
	Uint64 end;
	
	Animation(SDL_Texture *t, unsigned clipCount, SDL_Rect clipSize, Uint32 delay = 0);
	~Animation();
	unsigned animate();
};