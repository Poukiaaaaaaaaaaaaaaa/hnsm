#include "Player.h"

Player::Player(Game * g, std::vector<SDL_Texture*>& sprites, SDL_Rect& dimn)
	: linked(g)
{
	dim = dimn;

	linked->layers.push_back({});
	layerIndexA = linked->layers.size() - 1;

	linked->layers[layerIndexA].push_back(GObject(
		this, sprites, false
	));

	layerIndexB = linked->layers[layerIndexA].size() - 1;

	linked->game.push_back(this);
}

Player::~Player()
{
}


void Player::update()
{

}
