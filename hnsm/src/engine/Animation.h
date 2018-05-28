#pragma once
#include <iostream>
#include <vector>

//SDL headers
#include <SDL2\SDL.h>

#undef _main

/*
 *	Classe contenant l'int�gralit� des membres n�cessaires �
 *	l'animation des sprites
 *
 */
class Animation
{
public:
	/*
	 *	Texture contenant l'image enti�re, dans laquelle se trouve
	 *	toutes les diff�rentes images de l'animation
	 *
	 */
	SDL_Texture * t;

	/*
	 *	Membres n�cessaires � la gestion des clips:
	 *
	 *	'clipSize': membre contenant les dimensions des clips (largeur, hauteur)
	 *	'clipc': nombre total de clips/d'images de l'animation
	 *	'currentClip': clip actuel de l'animation
	 *	'clips': vecteur contenant les coordonn�es de chaque clip sur la texture 't'
	 *
	 */
	SDL_Rect clipSize;
	unsigned clipc;
	unsigned currentClip;
	std::vector<SDL_Rect> clips;

	/*
	 *	Membres n�cessaires � la gestion du temps entre chaque clip:
	 *
	 *	'delay': d�lai entre chaque image, en milisecondes
	 *	'tick_delay': delai entre chaque image, en nombre de ticks processeur
	 *	'start' et 'end': utilis�s dans la m�thode 'animate' pour la gestion du
	 *	temps entre chaque clip (plus de d�tails dans Animation.cpp)
	 *
	 */
	int delay;
	Uint64 tick_delay;
	Uint64 start;
	Uint64 end;
	
	/*
	 *	Constructeur et m�thode 'animate':
	 *
	 *	Voir Animation.cpp
	 *
	 */
	Animation(SDL_Texture *t, unsigned clipCount, SDL_Rect clipSize, Uint32 delay = 0);
	~Animation();
	unsigned animate();
};
