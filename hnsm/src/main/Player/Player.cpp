#include "Player.h"
#include "Game.h"

Player::Player(Game * g, std::vector<SDL_Texture*> sprites, floating_rect dimn)
	: linked(g)
{
	dim = dimn;

	layerIndexA = 5;

	linked->layers[layerIndexA].push_back(GObject(
		this, sprites, false
	));

	linked->physicalEngine.push_back(PObject(
		this, {0,0}
	));

	layerIndexB = linked->layers[layerIndexA].size() - 1;

	linked->game.push_back(this);
}

Player::Player() 
{

}

Player::~Player()
{
}


void Player::update()
{

}

void Player::events()
{

}

void playerWalkCallback(SDL_Rect * dim, vect * s ) 
{

}