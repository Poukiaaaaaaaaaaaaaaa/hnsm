#include "PObject.h"

float gravityAcc = 1.5;

PObject::PObject(float vx, float vy, bool g) : xSpeed(vx), ySpeed(vy), gravity(g)
{
	if (std::isnan(xSpeed))
	{
	}
}

PObject::PObject(vect s, bool g) : gravity(g)
{
	xSpeed = s.x;
	ySpeed = s.y;
}

void PObject::set_speed(float vx, float vy)
{
	xSpeed = vx;
	ySpeed = vy;
}

void PObject::set_speed(vect s)
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

vect PObject::get_speed()
{
	return { xSpeed,ySpeed };
}

void PObject::add_speed(float vx, float vy)
{
	xSpeed += vx;
	ySpeed += vy;
}

void PObject::add_speed(vect s)
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

void PObject::process(std::vector<PObject> & PObjects) 
{

	if (!isStatic) {

		SDL_Rect ndim = linked -> dim;
		SDL_Rect &dim = linked -> dim;
		bool xCollision = false, yCollision = false;

		ndim.x += xSpeed;
		ndim.y += ySpeed;

		if (gravity)
		{
			ySpeed -= gravityAcc;
		}

		SDL_Rect otherDim;

		for (unsigned i = 0; i < PObjects.size(); i++)
		{
			otherDim = PObjects[i].linked->dim;

			//horitonzal hit
			if (!(ndim.x > otherDim.x + otherDim.w && ndim.x + ndim.w < otherDim.x) )
			{
				xCollision = true;
				xSpeed = 0;
				break;
			}

			//vetrical hit
			if (!(ndim.y > otherDim.y + otherDim.h && ndim.y + ndim.h < otherDim.h))
			{
				yCollision = true;
				ySpeed = 0;
				break;
			}

			if (!xCollision)
			{
				dim.x = ndim.x;
			}

			if (!yCollision)
			{
				dim.y = ndim.y;
			}

		}
	}

}
