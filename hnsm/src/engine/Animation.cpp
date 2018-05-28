#include "Animation.h"

Animation::Animation(SDL_Texture *tex, unsigned clipCount, SDL_Rect clipSize, Uint32 delay)
	: t(tex), clipc(clipCount), clipSize(clipSize)
{
	/*
	 *	D�termine le nombre de clips par ligne et calcule les coordonn�es de chaque
	 *	clip (uniquement si un nombre de clips a �t� sp�cifi�)
	 *	Le nombre de clips correspond au nombre d'images de l'animation sur une
	 *	ligne de la texture
	 *
	 */
	if (clipc)
	{
		/*
		 *	R�cup�ration des dimenstions de la texture puis calcule du nombre de
		 *	clips par ligne
		 *
		 */
		int w, h;
		SDL_QueryTexture(tex, NULL, NULL, &w, &h);
		unsigned clipsPerLine = w / clipSize.w;

		/*
		 *	Calcul des coordonn�es de chaque clip puis stockage de ces derni�res
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
	 *	delay: correspond au d�lai entre chaque image de l'animation (en ms)
	 *	Si sp�cifi�, ce d�lai est converti en nombre de ticks processeur,
	 *	tick_delay: ensuite utilis� pour la gestion du temps entre chaque image
	 *
	 */
	if (delay)
		tick_delay = (Uint64)(((float)delay / 1000) * SDL_GetPerformanceFrequency());
}

Animation::~Animation()
{
}

/*
 *	La m�thode 'animate' calcule le temps �coul� depuis le dernier changement
 *	de clip. Si ce temps correspond au d�lai entre chaque image de l'animation,
 *	le clip actuel (ici 'currentClip') est incr�ment�
 *	
 *	Retourne: 'currentClip', soit le clip actuel affich�
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