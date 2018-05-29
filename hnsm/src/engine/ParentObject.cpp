#include "ParentObject.h"

ParentObject::ParentObject()
	: dim({ 0, 0, 0, 0 })
{
}

ParentObject::ParentObject(floating_rect dimn)
	: dim(dimn)
{
}

ParentObject::~ParentObject()
{
}

floating_rect ParentObject::get_dim() const
{
	return dim;
}

void ParentObject::set_dim(floating_rect dimn)
{
	dim = dimn;
}
