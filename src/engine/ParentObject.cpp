#include "ParentObject.h"

ParentObject::ParentObject()
	: dim({ 0, 0, 0, 0 })
{
}

ParentObject::ParentObject(SDL_Rect dimn)
	: dim(dimn)
{
}

ParentObject::~ParentObject()
{
}

SDL_Rect ParentObject::get_dim() const
{
	return dim;
}

void ParentObject::set_dim(SDL_Rect dimn)
{
	dim = dimn;
}