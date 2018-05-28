#include "Animation.h"

Animation::Animation(SDL_Texture *tex, unsigned clipCount, SDL_Rect clipSize, Uint32 delay)
	: t(tex), clipc(clipCount), clipSize(clipSize)
{
	/*
	 *	Détermine le nombre de clips par ligne et calcule les coordonnées de chaque
	 *	clip (uniquement si un nombre de clips a été spécifié)
	 *	Le nombre de clips correspond au nombre d'images de l'animation sur une
	 *	ligne de la texture
	 *
	 */
	if (clipc)
	{
		/*
		 *	Récupération des dimenstions de la texture puis calcule du nombre de
		 *	clips par ligne
		 *
		 */
		int w, h;
		SDL_QueryTexture(tex, NULL, NULL, &w, &h);
		unsigned clipsPerLine = w / clipSize.w;

		/*
		 *	Calcul des coordonnées de chaque clip puis stockage de ces dernières
		 *	dans le vecteur 'clips'
		 *
		 */
		for (unsigned i = 0; i < clipc; i++)
		{
			clips.push_back({ (float)(i % clipsPerLine) * clipSize.w, (float)(i / clipsPerLine) * clipSize.h, clipSize.w, clipSize.h });
		}

		if (clipc)
			currentClip = 0;
	}

	/*
	 *	delay: correspond au délai entre chaque image de l'animation (en ms)
	 *	Si spécifié, ce délai est converti en nombre de ticks processeur,
	 *	tick_delay: ensuite utilisé pour la gestion du temps entre chaque image
	 *
	 */
	if (delay)
		tick_delay = (Uint64)(((float)delay / 1000) * SDL_GetPerformanceFrequency());
}

Animation::~Animation()
{
}

/*
 *	La méthode 'animate' calcule le temps écoulé depuis le dernier changement
 *	de clip. Si ce temps correspond au délai entre chaque image de l'animation,
 *	le clip actuel (ici 'currentClip') est incrémenté
 *	
 *	Retourne: 'currentClip', soit le clip actuel affiché
 *
 */
unsigned Animation::animate()
{
	if (!start)
	{
		start = SDL_GetPerformanceCounter();
	}

	end = SDL_GetPerformanceCounter();
	

	if (end - start > tick_delay)
	{
		currentClip = (currentClip + 1) % clipc;
		start = 0;
	}

	return currentClip;
}