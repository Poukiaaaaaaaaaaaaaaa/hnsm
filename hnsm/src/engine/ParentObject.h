#pragma once

//SDL headers
#include <SDL2\SDL.h>

//engine headers
#include "UI.h"

class ParentObject : public UI
{
public:
	floating_rect dim;

	ParentObject();
	ParentObject(floating_rect);
	~ParentObject();

	floating_rect get_dim() const;
	void set_dim(floating_rect);
};
