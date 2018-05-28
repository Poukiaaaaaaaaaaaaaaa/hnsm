#pragma once
#include <iostream>
#include <vector>

//SDL headers
#include <SDL2\SDL.h>

#undef _main

/*
 *	Classe contenant l'intégralité des membres nécessaires à
 *	l'animation des sprites
 *
 */
class Animation
{
public:
	/*
	 *	Texture contenant l'image entière, dans laquelle se trouve
	 *	toutes les différentes images de l'animation
	 *
	 */
	SDL_Texture * t;

	/*
	 *	Membres nécessaires à la gestion des clips:
	 *
	 *	'clipSize': membre contenant les dimensions des clips (largeur, hauteur)
	 *	'clipc': nombre total de clips/d'images de l'animation
	 *	'currentClip': clip actuel de l'animation
	 *	'clips': vecteur contenant les coordonnées de chaque clip sur la texture 't'
	 *
	 */
	SDL_Rect clipSize;
	unsigned clipc;
	unsigned currentClip;
	std::vector<SDL_Rect> clips;

	/*
	 *	Membres nécessaires à la gestion du temps entre chaque clip:
	 *
	 *	'delay': délai entre chaque image, en milisecondes
	 *	'tick_delay': delai entre chaque image, en nombre de ticks processeur
	 *	'start' et 'end': utilisés dans la méthode 'animate' pour la gestion du
	 *	temps entre chaque clip (plus de détails dans Animation.cpp)
	 *
	 */
	int delay;
	Uint64 tick_delay;
	Uint64 start;
	Uint64 end;
	
	/*
	 *	Constructeur et méthode 'animate':
	 *
	 *	Voir Animation.cpp
	 *
	 */
	Animation(SDL_Texture *t, unsigned clipCount, SDL_Rect clipSize, Uint32 delay = 0);
	~Animation();
	unsigned animate();
};
