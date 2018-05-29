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
	floating_rect dim;

	ParentObject();
	ParentObject(floating_rect);
	~ParentObject();

	floating_rect get_dim() const;
	void set_dim(floating_rect);
};
