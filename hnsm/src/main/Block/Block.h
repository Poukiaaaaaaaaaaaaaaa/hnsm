#pragma once

#include <SDL2\SDL.h>

#include "ParentObject.h"
#include "GObject.h"

class Block : public UI
{
public:
	ParentObject ptn;
	GObject gameObjet;


public:
	Block(floating_rect dim, SDL_Texture* texture);
	~Block();

	GObject getGObject() const;

	void update();
	void events();

private:
	Block();
};