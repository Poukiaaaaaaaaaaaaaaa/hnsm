#include "ButtonData.h"

ButtonData::ButtonData(SDL_Texture * n, SDL_Texture * h, void(*cb)(void*), void * cbd, floating_rect d)
	: not_hovered(n), hovered(h), callback(cb), cbData(cbd), dim(d)
{
}

ButtonData::~ButtonData()
{
}
