#pragma once

#include <iostream>
#include <vector>
#include <functional>

//SDL headers
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <SDL2\SDL_ttf.h>
#include <SDL2\SDL_mixer.h>

//engine headers
#include "Log.h"
#include "Pair.h"
#include "Animation.h"
#include "ParentObject.h"

#undef _main

/*
 *	Différents types d'objets graphiques
 * 
 */
enum GObjectType
{
	STATIC, ANIMATED, TEXT
};

/*
 *	Différentes propriétés de texte
 *
 */
enum TextTag
{
	BLENDED, SHADED
};

/*
 *	La classe 'GObject' est fondamentale. Elle permet la création
 *	d'objets graphiques à partir de textures SDL. À leur création,
 *	les objets graphiques seront ajoutés au membre 'layers' de l'objet
 *	global (voir Game.h) puis seront affichés à chaque actualisation
 *	du jeu.
 *
 */
class GObject
{
	friend class Animation;

private:
	/*
	 *	Type d'objet graphique (statique, animé ou texte).
	 *
	 */
	GObjectType type;

	/*
	 *	Membres propres aux objets de type texte.
	 *
	 */
	TextTag TxtTag;
	std::string text;
	int size;
	TTF_Font *f;
	SDL_Color c;
	SDL_Color bg;

	/*
	 *	Membres propres aux objets graphiques animés.
	 *
	 */
	SDL_Rect clipSize;
	std::vector<Animation> textures;
	unsigned currentTexture;
	unsigned textureCount;

	int alpha;
	bool shouldDraw;

public:
	/*
	 *	Méthodes et constructeurs: voir GObject.cpp
	 *
	 */
	ParentObject * linked;

	GObject();
	GObject(ParentObject * lnk, std::vector<SDL_Texture*>, bool is_static, int delay = NULL,
		unsigned clipc = NULL, SDL_Rect clipSize = {});
	GObject(ParentObject * lnk, SDL_Renderer*, std::string text, SDL_Color, std::string fontPath,
		int fontSize, bool is_static, TextTag = BLENDED, SDL_Color bg = {}
	);
	~GObject();

	int get_alpha() const;
	SDL_Texture* get_texture(unsigned) const;
	int get_clipc(unsigned) const;
	std::vector<SDL_Rect> get_clips(unsigned) const;
	int get_current_clip(unsigned) const;
	SDL_Rect get_clip_size(unsigned) const;
	TextTag get_text_tag() const;
	SDL_Color get_text_bg_color() const;
	unsigned get_current_texture() const;

	void set_alpha(int);
	void set_should_draw(bool);
	void set_current_clip(unsigned);
	void set_current_texture(unsigned);
	void set_texture(SDL_Texture*, unsigned index);
	void set_text(SDL_Renderer *r, std::string txt);

	bool is_static;
	void draw(SDL_Renderer *r, Pair);
};
