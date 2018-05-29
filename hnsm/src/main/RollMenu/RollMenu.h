#pragma once
#include <iostream>

//engine headers
#include "Game.h"

//SDL headers
#include <SDL2\SDL.h>

//engine headers
#include "UI.h"
#include "ParentObject.h"
#include "Misc.h"

//main headers
#include "Button\Button.h"
#include "MenuData\MenuData.h"
#include "ButtonData\ButtonData.h"

class RollMenu : public ParentObject
{
	Game * linked;

	unsigned layerIndexa;
	unsigned layerIndexb;
	bool canPress;
	bool isOpened;

	std::vector<Button*> buttons;

public:
	RollMenu(Game& g, SDL_Texture * h, SDL_Texture * n, floating_rect d, std::vector<ButtonData>& bd,
		ElemType t, unsigned layer
	);
	~RollMenu();

	void open();
	void close();

	bool isHovered();
	bool isClicked();

	void update();
	void events();
};

