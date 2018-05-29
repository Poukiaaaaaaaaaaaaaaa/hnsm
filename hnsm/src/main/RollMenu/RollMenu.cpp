#include "RollMenu.h"

#define HOVERED 1
#define NOT_HOVERED 0

RollMenu::RollMenu(
	Game& g, SDL_Texture * h, SDL_Texture * n, SDL_Rect d, std::vector<ButtonData>& bd, ElemType t, unsigned layer
) : linked(&g), isOpened(false), canPress(true), layerIndexa(layer)
{
	dim = d;

	linked->layers[layerIndexa].push_back(GObject(
		this, { n, h }, true
	));
	layerIndexb = linked->layers[layerIndexa].size() - 1;

	for (unsigned i = 0; i < bd.size(); i++)
	{
		bd[i].dim = { d.x, ((int)i + 1) * d.h + d.y, d.w, d.h };
		buttons.push_back(new Button(*linked, layerIndexa, bd[i]));
		linked->layers[layerIndexa][buttons[i]->indexb].set_should_draw(false);
	}

	switch (t)
	{
	case GUI:
		linked->gui.push_back(this);
		break;

	case GAME:
		linked->game.push_back(this);
		break;
	}
}

RollMenu::~RollMenu()
{
	linked->layers[layerIndexa].clear();

	for (unsigned i = 0; i < buttons.size(); i++)
	{
		delete buttons[i];
	}
}

void RollMenu::open()
{
	for (unsigned i = 0; i < buttons.size(); i++)
	{
		linked->layers[layerIndexa][buttons[i]->indexb].set_should_draw(true);
	}

	isOpened = true;
}

void RollMenu::close()
{
	for (unsigned i = 0; i < buttons.size(); i++)
	{
		linked->layers[layerIndexa][buttons[i]->indexb].set_should_draw(false);
	}

	isOpened = false;
}

bool RollMenu::isHovered()
{
	if (linked->mouse_x > dim.x &&
		linked->mouse_y > dim.y &&
		linked->mouse_x < dim.x + dim.w &&
		linked->mouse_y < dim.y + dim.h
		) return true;
	else return false;
}

bool RollMenu::isClicked()
{
	if (linked->e.button.state == SDL_PRESSED &&
		linked->e.button.button == SDL_BUTTON_LEFT &&
		isHovered() && canPress == true
		)
	{
		canPress = false;
		return true;
	}
	else if (linked->e.button.state == SDL_RELEASED)
	{
		canPress = true;
		return false;
	}
	
	return false;
}

void RollMenu::update()
{
	if (isHovered())
	{
		linked->layers[layerIndexa][layerIndexb].set_current_texture(HOVERED);
	}
	else
	{
		linked->layers[layerIndexa][layerIndexb].set_current_texture(NOT_HOVERED);
	}

	if (isOpened)
	{
		for (unsigned i = 0; i < buttons.size(); i++)
		{
			buttons[i]->update();
		}
	}
}

void RollMenu::events()
{
	if (isClicked())
	{
		if (isOpened)
		{
			close();
		}
		else if (!isOpened)
		{
			open();
		}
	}

	if (isOpened)
	{
		for (unsigned i = 0; i < buttons.size(); i++)
		{
			buttons[i]->events();
		}
	}
}