#include "PObject.h"

PObject::PObject(float vx, float vy, bool g) : xSpeed(vx), ySpeed(vy), gravity(g)
{
	if (std::isnan(xSpeed))
	{
	}
}

PObject::PObject(speed s, bool g) : gravity(g)
{
	xSpeed = s.x;
	ySpeed = s.y;
}

void PObject::set_speed(float vx, float vy)
{
	xSpeed = vx;
	ySpeed = vy;
}

void PObject::set_speed(speed s)
{
	xSpeed = s.x;
	ySpeed = s.y;
}

void PObject::set_xSpeed(float vx)
{
	xSpeed = vx;
}

void PObject::set_ySpeed(float vy)
{
	ySpeed = vy;
}

speed PObject::get_speed()
{
	return { xSpeed,ySpeed };
}

void PObject::add_speed(float vx, float vy)
{
	xSpeed += vx;
	ySpeed += vy;
}

void PObject::add_speed(speed s)
{
	xSpeed += s.x;
	ySpeed += s.y;
}

void PObject::add_xSpeed(float vx)
{
	xSpeed += vx;
}

void PObject::add_ySpeed(float vy)
{
	xSpeed += vy;
}

void PObject::process() {}
