#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <ctgmath>
#include <cmath>
#include "ParentObject.h"

enum hitboxType
{
	RECT,
};

struct speed
{
	float x; float y;
};

class PObject
{
private :
	float xSpeed;
	float ySpeed;
	bool isStatic;
	bool gravity;

public:	
	void process();
	void set_speed(float vx, float vy);
	void set_speed(speed s);
	void set_xSpeed(float vx);
	void set_ySpeed(float vy);
	void add_speed(float vx, float vy);
	void add_speed(speed s);
	void add_xSpeed(float vx);
	void add_ySpeed(float vy);
	speed get_speed();
	PObject(float vx = 0, float vy = 0, bool g = true);
	PObject(speed s, bool g = true);
};
