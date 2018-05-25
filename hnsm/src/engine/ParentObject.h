#pragma once

//SDL headers
#include <SDL2\SDL.h>

//engine headers
#include "UI.h"

class ParentObject : public UI
{
public:
	SDL_Rect dim;

	ParentObject();
	ParentObject(SDL_Rect);
	~ParentObject();

	SDL_Rect get_dim() const;
	void set_dim(SDL_Rect);
};
