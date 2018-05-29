#pragma once

//SDL headers
#include <SDL2\SDL.h>

//engine headers
#include "UI.h"

/*
 *	Classe abstraite désignant un objet du jeu.
 *	Un objet instance de la classe 'ParentObjet'
 *	possède, contrairement à la classe 'UI', des
 *	dimensions sous la forme d'un rectangle ('SDL_Rect').
 *
 */
class ParentObject : public UI
{
public:
	SDL_Rect dim;

	ParentObject();
	ParentObject(SDL_Rect);
	~ParentObject();

	SDL_Rect get_dim() const;
	void set_dim(SDL_Rect);
};
