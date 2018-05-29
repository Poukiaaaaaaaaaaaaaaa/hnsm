#pragma once

//SDL headers
#include <SDL2/SDL.h>

struct ButtonData
{
	SDL_Texture * not_hovered;
	SDL_Texture * hovered;
	void(*callback)(void*);
	void * cbData;  floating_rect dim;

	ButtonData(SDL_Texture * n, SDL_Texture * h, void(*cb)(void*), void * cbData, floating_rect d);
	~ButtonData();
};
