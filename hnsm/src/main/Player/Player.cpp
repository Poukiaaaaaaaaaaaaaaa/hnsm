#include "Player.h"
#include "Game.h"

Player::Player(Game * g, std::vector<SDL_Texture*> sprites, SDL_Rect dimn)
	: linked(g)
{
	dim = dimn;

	layerIndexA = 0;

	linked->layers[layerIndexA].push_back(GObject(
		this, sprites, false
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