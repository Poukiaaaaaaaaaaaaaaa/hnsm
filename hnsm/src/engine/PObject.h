#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <ctgmath>
#include <cmath>
#include "ParentObject.h"

#include <SDL2\SDL.h>

enum hitboxType
{
	RECT,
};

struct vect
{
	float x; float y;
};

class PObject
{
private :
	vect speed;
	bool isStatic;
	bool gravity;
	void(*callback)(SDL_Rect * dim, vect * s);

public:	
	ParentObject * linked;


	void process(std::vector<PObject> & PObjects);
	void set_speed(vect s);
	void set_xSpeed(float vx);
	void set_ySpeed(float vy);
	void add_speed(vect s);
	void add_xSpeed(float vx);
	void add_ySpeed(float vy);
	vect get_speed();
	PObject(PObject* object, vect s, bool g = true, void(*cb)(SDL_Rect * dim, vect * s) = nullptr);
};
