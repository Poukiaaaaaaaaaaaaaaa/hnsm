#include "Menu.h"

Menu::Menu(Game& g, MenuData data, ElemType t)
	: linked(&g), isOpened(true)
{
	linked->layers.push_back({});
	layerIndex = linked->layers.size() - 1;

	background = ParentObject({ 0, 0, linked->get_w(), linked->get_h() });

	linked->layers[layerIndex].push_back(GObject(
		&background, { data.bg }, true
	));
	bgIndex = linked->layers[layerIndex].size() - 1;

	for (unsigned i = 0; i < data.bd.size(); i++)
	{
		buttons.push_back(new Button(*linked, layerIndex, data.bd[i]));
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

Menu::~Menu()
{
	linked->layers[layerIndex].clear();

	for (unsigned i = 0; i < buttons.size(); i++)
	{
		delete buttons[i];
	}
}

void Menu::open()
{
	if (!isOpened)
	{
		isOpened = true;

		linked->layers[layerIndex][bgIndex].set_should_draw(true);

		for (unsigned i = 0; i < buttons.size(); i++)
		{
			linked->layers[layerIndex][buttons[i]->indexb].set_should_draw(true);
		}
	}
}

void Menu::close()
{
	if (isOpened)
	{
		isOpened = false;

		linked->layers[layerIndex][bgIndex].set_should_draw(false);

		for (unsigned i = 0; i < buttons.size(); i++)
		{
			linked->layers[layerIndex][buttons[i]->indexb].set_should_draw(false);
		}
	}
}

void Menu::update()
{
	if (isOpened)
	{
		for (unsigned i = 0; i < buttons.size(); i++)
		{
			buttons[i]->update();
		}
	}
}

void Menu::events()
{
	if (isOpened)
	{
		for (unsigned i = 0; i < buttons.size(); i++)
		{
			buttons[i]->events();
		}
	}
}
