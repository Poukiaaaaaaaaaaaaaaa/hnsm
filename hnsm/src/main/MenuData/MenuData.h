#pragma once

#include <iostream>
#include <vector>

//SDL headers
#include <SDL2/SDL.h>

//main headers
#include "ButtonData/ButtonData.h"
class MenuData
{
public:
	SDL_Texture * bg;
	std::vector<ButtonData> bd;

	MenuData(SDL_Texture * bGrd, std::vector<ButtonData> bDat)
		: bg(bGrd), bd(bDat)
	{

	}
};
