#include "GObject.h"

GObject::GObject()
{

}

GObject::GObject(ParentObject * lnk, std::vector<SDL_Texture*> animations, bool is_static,
	int delay, unsigned clipc, SDL_Rect clipSize)
	: clipSize(clipSize), is_static(is_static), type(clipc > 0 ? ANIMATED : STATIC), 
	textureCount(animations.size()), alpha(255), shouldDraw(true), linked(lnk)
{
	for (unsigned i = 0; i < animations.size(); i++)
	{
		textures.push_back(Animation(animations[i], clipc, clipSize, delay));
	}

	SDL_Rect temp = linked->get_dim();
	if (!temp.w || !temp.h)
	{
		SDL_QueryTexture(textures[0].t, NULL, NULL, &temp.w, &temp.h);
		linked->set_dim({ temp.x, temp.y, temp.w, temp.h });
	}

	currentTexture = 0u;
}

GObject::GObject(ParentObject * lnk, SDL_Renderer *r, std::string txt, SDL_Color c,
	std::string fontPath, int size, bool is_static, TextTag tTag, SDL_Color b
)	: text(txt), c(c), size(size), f(TTF_OpenFont(fontPath.c_str(), size)), TxtTag(tTag),
	bg(b), type(TEXT), textureCount(1u), alpha(255), shouldDraw(true), is_static(is_static), linked(lnk)
{
	SDL_Surface *s = nullptr;

	if (TxtTag == BLENDED)
		s = TTF_RenderText_Blended(f, text.c_str(), c);

	if (TxtTag == SHADED)
		s = TTF_RenderText_Shaded(f, text.c_str(), c, bg);

	textures.push_back(Animation(SDL_CreateTextureFromSurface(r, s), 0u, linked->get_dim(), 0));

	SDL_Rect temp = linked->get_dim();
	if (!temp.w || !temp.h)
	{
		SDL_QueryTexture(textures[0].t, NULL, NULL, &temp.w, &temp.h);
		linked->set_dim({ temp.x, temp.y, temp.w, temp.h });
	}

	if (s != nullptr)
	{
		SDL_FreeSurface(s);
	}
}

GObject::~GObject()
{
}

SDL_Texture* GObject::get_texture(unsigned index) const
{
	if (index < textures.size())
		return textures[index].t;
	else
		return nullptr;
}

int GObject::get_clipc(unsigned index) const
{
	if (index < textures.size())
		return textures[index].clipc;
	else
		return NULL;
}

std::vector<SDL_Rect> GObject::get_clips(unsigned index) const
{
	if (index < textures.size())
		return textures[index].clips;
	else
		return {};
}

int GObject::get_current_clip(unsigned index) const
{
	if (index < textures.size())
		return textures[index].currentClip;
	else
		return NULL;
}

SDL_Rect GObject::get_clip_size(unsigned index) const
{
	if (index < textures.size())
		return textures[index].clipSize;
	else
		return {};
}

TextTag GObject::get_text_tag() const
{
	return TxtTag;
}

SDL_Color GObject::get_text_bg_color() const
{
	return bg;
}

unsigned GObject::get_current_texture() const
{
	return currentTexture;
}

int GObject::get_alpha() const
{
	return alpha;
}

void GObject::set_should_draw(bool should)
{
	shouldDraw = should;
}

void GObject::set_current_clip(unsigned clip)
{
	if (clip < textures[currentTexture].clipc)
		textures[currentTexture].currentClip = clip;
}

void GObject::set_texture(SDL_Texture *t, unsigned index)
{
	if (index < textures.size())
	{
		textures[index].t = t;
	}
}

void GObject::set_current_texture(unsigned tIndex)
{
	if ((type == ANIMATED || type == STATIC) && tIndex < textures.size())
		currentTexture = tIndex;
}

void GObject::set_text(SDL_Renderer *r, std::string txt)
{
	text = txt;

	SDL_Surface *s = nullptr;
	if (TxtTag == BLENDED)
		s = TTF_RenderText_Blended(f, text.c_str(), c);

	if (TxtTag == SHADED)
		s = TTF_RenderText_Shaded(f, text.c_str(), c, bg);

	SDL_DestroyTexture(textures[0].t);

	int w, h;
	textures[0].t = SDL_CreateTextureFromSurface(r, s);
	SDL_QueryTexture(textures[0].t, NULL, NULL, &w, &h);
	SDL_Rect temp = linked->get_dim();
	linked->set_dim({ temp.x, temp.y, w, h });

	SDL_FreeSurface(s);
}

void GObject::set_alpha(int val)
{
	if (val >= 0 && val <= 255)
	{
		alpha = val;
	}
	else
	{
		if (val < 0)
		{
			alpha = 0;
		}
		if (val > 255)
		{
			alpha = 255;
		}
	}
}

void GObject::draw(SDL_Renderer *r, Pair cam)
{
	if (shouldDraw)
	{
		SDL_Rect finalDim;
		if (!is_static)
		{
			SDL_Rect tempDim = linked->get_dim();

			finalDim = {
				tempDim.x - cam.x,
				tempDim.y - cam.y,
				tempDim.w, tempDim.h
			};
		}
		else
		{
			finalDim = linked->get_dim();
		}

		switch (type)
		{
		case ANIMATED:
			SDL_SetTextureAlphaMod(textures[currentTexture].t, alpha);
			SDL_RenderCopy(r, textures[currentTexture].t,
				&textures[currentTexture].clips[textures[currentTexture].animate()],
				&finalDim);
			break;

		case STATIC:
			Log::toFile("debug.txt", std::to_string(finalDim.x));
			SDL_SetTextureAlphaMod(textures[currentTexture].t, alpha);
			SDL_RenderCopy(r, textures[currentTexture].t, NULL, &finalDim);
			break;

		case TEXT:
			SDL_SetTextureAlphaMod(textures[0].t, alpha);
			SDL_RenderCopy(r, textures[0].t, NULL, &finalDim);
			break;
		}
	}
}
