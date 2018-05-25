#include "ButtonData.h"

ButtonData::ButtonData(SDL_Texture * h, SDL_Texture * n, void(*cb)(void*), void * cbd, SDL_Rect d)
	: hovered(h), not_hovered(n), callback(cb), cbData(cbd), dim(d)
{
}

ButtonData::~ButtonData()
{
}
