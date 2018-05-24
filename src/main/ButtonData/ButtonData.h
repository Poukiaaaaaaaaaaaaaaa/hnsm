#pragma once

//SDL headers
#include <SDL2/SDL.h>

struct ButtonData
{
	SDL_Texture * hovered;
	SDL_Texture * not_hovered;
	void(*callback)(void*);
	void * cbData;  SDL_Rect dim;

	ButtonData(SDL_Texture * h, SDL_Texture * n, void(*cb)(void*), void * cbData, SDL_Rect d);
	~ButtonData();
};
