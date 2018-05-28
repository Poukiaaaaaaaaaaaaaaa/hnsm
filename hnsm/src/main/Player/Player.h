#pragma once

#include <iostream>
#include <vector>

//SDL headers
#include <SDL2\SDL.h>

//engine headers
#include "Game.h"
#include "ParentObject.h"

class Player : public ParentObject
{
	Game * linked;

	unsigned layerIndexA;
	unsigned layerIndexB;

public:
	struct
	{
		float maxSpeed;
		float speed;
		float acc;
	} walk;

	Player(Game * g, std::vector<SDL_Texture*>& sprites, SDL_Rect&);
	~Player();

	void update();
};