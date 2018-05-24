#include "Animation.h"

Animation::Animation(SDL_Texture *tex, unsigned clipCount, SDL_Rect clipSize, Uint32 delay)
	: t(tex), clipc(clipCount), clipSize(clipSize)
{
	if (clipc)
	{
		int w, h;
		SDL_QueryTexture(tex, NULL, NULL, &w, &h);
		unsigned clipsPerLine = w / clipSize.w;

		for (unsigned i = 0; i < clipc; i++)
		{
			clips.push_back({ (int)(i % clipsPerLine) * clipSize.w, (int)(i / clipsPerLine) * clipSize.h, clipSize.w, clipSize.h });
		}

		if (clipc)
			currentClip = 0;
	}

	if (delay)
		tick_delay = (Uint64)(((float)delay / 1000) * SDL_GetPerformanceFrequency());
}

Animation::~Animation()
{
}

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