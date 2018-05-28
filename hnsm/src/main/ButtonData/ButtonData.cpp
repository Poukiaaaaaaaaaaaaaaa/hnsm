#include "ButtonData.h"

ButtonData::ButtonData(SDL_Texture * h, SDL_Texture * n, void(*cb)(void*), void * cbd, SDL_Rect d)
	: not_hovered(n), hovered(h), callback(cb), cbData(cbd), dim(d)
{
}

ButtonData::~ButtonData()
{
}
