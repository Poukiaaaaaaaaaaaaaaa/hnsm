#pragma once

#include <functional>

//engine headers
#include "GObject.h"
#include "Game.h"

//engine headers
#include "ParentObject.h"
#include "Misc.h"

//main headers
#include "ButtonData\ButtonData.h"

class Button : public ParentObject
{

public:
	Game * linked;
	unsigned indexa;
	unsigned indexb;

	bool canPress;
	void * callbackData;
	void(*callbackFunction)(void*);

	Button();
	Button(Game& g, int layerIndex, ButtonData& data);
	~Button();

	bool isHovered();
	bool isClicked();
	void update();
	void events();
};
