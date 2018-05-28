#pragma once
#include <iostream>
#include <vector>

//SDL headers
#include <SDL2\SDL.h>

//engine headers
#include "Game.h"
#include "GObject.h"
#include "ParentObject.h"

//main headers
#include "Button\Button.h"
#include "MenuData\MenuData.h"
#include "UI.h"

class Menu : public UI
{
	Game * linked;

	unsigned layerIndex;

	ParentObject background;
	unsigned bgIndex;

	std::vector<Button*> buttons;
	bool isOpened;

public:
	Menu(Game& g, MenuData data, ElemType t);
	~Menu();

	void open();
	void close();

	void update();
	void events();
};
