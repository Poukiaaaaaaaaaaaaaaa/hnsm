#include "PObject.h"
#include "Log.h"

float gravityAcc = 1.5;

PObject::PObject(ParentObject* object, vect s, bool g, void(*cb)(floating_rect * dim, vect* s)) : gravity(g), speed(s), callback(cb), linked(object), isStatic(false)
{
}

void PObject::set_speed(vect s)
{
	speed = s;
}

void PObject::set_xSpeed(float vx)
{
	speed.x = vx;
}

void PObject::set_ySpeed(float vy)
{
	speed.y = vy;
}

vect PObject::get_speed()
{
	return speed;
}

void PObject::add_speed(vect s)
{
	speed = s;
}

void PObject::add_xSpeed(float vx)
{
	speed.x += vx;
}

void PObject::add_ySpeed(float vy)
{
	speed.y += vy;
}

void PObject::process(std::vector<PObject> & PObjects) 
{

	if (!isStatic) {

		floating_rect ndim = linked -> dim;
		floating_rect &dim = linked -> dim;
		bool xCollision = false, yCollision = false;

		if (callback)
		{
			callback(&ndim, &speed);
		}

		ndim.x += speed.x;
		ndim.y += speed.y;

		if (gravity)
		{
			speed.y += gravityAcc;
		}

		floating_rect otherDim;

		for (unsigned i = 0; i < PObjects.size(); i++)
		{
			if (!(&PObjects[i] == this)) {
				otherDim = PObjects[i].linked->dim;

				//horitonzal hit
				if (!(ndim.x > otherDim.x + otherDim.w && ndim.x + ndim.w < otherDim.x))
				{
					Log::toFile("debug.txt", "collision");
					xCollision = true;
					speed.x = 0;
					break;
				}

				//vetrical hit
				if (!(ndim.y > otherDim.y + otherDim.h && ndim.y + ndim.h < otherDim.y))
				{
					yCollision = true;
					speed.y = 0;
					break;
				}

			}


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
