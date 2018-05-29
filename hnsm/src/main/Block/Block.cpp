#include "Block.h"

Block::Block(floating_rect dim, SDL_Texture* texture)
	: ptn(dim), gameObjet(GObject(&ptn, { texture }, false))
{
}

GObject Block::getGObject() const
{
	return gameObjet;
}

Block::~Block() {}

void Block::update() {}
void Block::events() {}