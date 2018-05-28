#pragma once

enum ElemType
{
	GUI, GAME
};

class UI 
{
public:
	virtual void update();
	virtual void events();

	UI();
	~UI();
};
