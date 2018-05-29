#include "GObject.h"

GObject::GObject()
{

}

/*
 *	Constructeur permettant la création d'objets graphiques animés
 *	ou statiques. Un objet statique étant, entre autres, un objet animé
 *	dont l'animation n'aurait qu'une seule image.
 *
 */
GObject::GObject(ParentObject * lnk, std::vector<SDL_Texture*> animations, bool is_static,
	int delay, unsigned clipc, SDL_Rect clipSize)
	: clipSize(clipSize), is_static(is_static), type(clipc > 0 ? ANIMATED : STATIC), 
	textureCount(animations.size()), alpha(255), shouldDraw(true), linked(lnk)
{
	/*
	 *	Chaque 'GObject' a la possibilité de stocker plusieurs Animations.
	 *	Chaque Animation représente une apparence différente. La totalité des
	 *	animations sont ainsi chargées pendant le constructeur. Il sera ensuite
	 *	possible de passer de l'une à l'autre (voir méthode 'set_current_texture').
	 *
	 */
	for (unsigned i = 0; i < animations.size(); i++)
	{
		textures.push_back(Animation(animations[i], clipc, clipSize, delay));
	}

	/*
	 *	Les dimensions de chaque 'GObject' sont stockées à l'intérieur d'un
	 *	objet parent. Cet objet parent peut être, entre autres, nimporte quel
	 *	élément du jeu ou de l'interface graphique, la totalité d'entre eux
	 *	héritant de la classe 'ParentObject' (voir ParentObject.h).
	 *
	 */
	floating_rect temp = linked->get_dim();
	if (!temp.w || !temp.h)
	{
		SDL_QueryTexture(textures[0].t, NULL, NULL, &temp.w, &temp.h);
		linked->set_dim({ temp.x, temp.y, temp.w, temp.h });
	}

	currentTexture = 0u;
}

/*
 *	Constructeur propre aux 'GObject' de type texte.
 *
 */
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
	textures.push_back(Animation(SDL_CreateTextureFromSurface(r, s), 0u, (SDL_Rect)linked->get_dim(), 0));

	/*
	 *	La création d'un objet graphique de type texte consiste en
	 *	la génération d'une texture à partir d'une chaîne de caractères,
	 *	d'une taille de police et d'un fichier .ttf ou .otf via les
	 *	fonctions 'TTF_RenderText_(Blended/Shaded)' ci-dessus.
	 *
	 */
	textures.push_back(Animation(SDL_CreateTextureFromSurface(r, s), 0u, linked->get_dim(), 0));

	/*
	 *	La taille de l'objet est calculée à partir de la taille de la
	 *	texture si aucune taille n'est fournie par le 'ParentObject'.
	 *
	 */
	floating_rect temp = linked->get_dim();
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

/*
 *	'clipc' correspond au nombre d'images de l'animation.
 *
 */
int GObject::get_clipc(unsigned index) const
{
	if (index < textures.size())
		return textures[index].clipc;
	else
		return NULL;
}

/*
 *	'clips' est un vecteur contenant les coordonnées des images
 *	de l'animation sur l'image complète (voir Animation.h et .cpp).
 *
 */
std::vector<SDL_Rect> GObject::get_clips(unsigned index) const
{
	if (index < textures.size())
		return textures[index].clips;
	else
		return {};
}

/*
 *	Renvoie le numéro de l'image actuelle de l'animation.
 *
 */
int GObject::get_current_clip(unsigned index) const
{
	if (index < textures.size())
		return textures[index].currentClip;
	else
		return NULL;
}

/*
 *	Renvoie la taille des images de l'animation.
 *
 */
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

/*
 *	Permet la gestion de l'affichage des objets graphiques.
 *	'shouldDraw' constitue la condition de dessin principale.
 *
 */
void GObject::set_should_draw(bool should)
{
	shouldDraw = should;
}

/*
 *	Change l'image actuelle de l'animation.
 *
 */
void GObject::set_current_clip(unsigned clip)
{
	if (clip < textures[currentTexture].clipc)
		textures[currentTexture].currentClip = clip;
}

/*
 *	Modifie une texture dans le tableau de textures du 'GObject'.
 *
 */
void GObject::set_texture(SDL_Texture *t, unsigned index)
{
	if (index < textures.size())
	{
		textures[index].t = t;
	}
}

/*
 *	Modifie la texture/animation actuellement affichée.
 *
 */
void GObject::set_current_texture(unsigned tIndex)
{
	if ((type == ANIMATED || type == STATIC) && tIndex < textures.size())
		currentTexture = tIndex;
}

/*
 *	Modifie le texte d'un objet graphique de type texte.
 *	Fénère entre autres une nouvelle textures avec les nouvelles
 *	propriétés de texte précisées en paramètre.
 *
 */
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
	linked->set_dim({ (float)temp.x, (float)temp.y, w, h });

	SDL_FreeSurface(s);
}

/*
 *	Modifie la valeur du canal alpha du 'GObject'.
 *	Dans le cadre de SDL, cette valeur est comprise
 *	entre 0 et 255.
 *
 */
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

/*
 *	Méthode fondamentale du 'GObject':
 *
 *	Affiche la texture à l'outil de rendu précisé en paramètre.
 *
 */
void GObject::draw(SDL_Renderer *r, Pair cam)
{
	/*
	 *	Condition principale de dessin.
	 *
	 */
	if (shouldDraw)
	{
		/*
		 *	Calcul des coordonnées finales en fonction
		 *	de la caméra si l'objet en dépend (soit is_static = false)
		 *
		 */
		SDL_Rect finalDim;
		if (!is_static)
		{
			SDL_Rect tempDim = (SDL_Rect)linked->get_dim();

			finalDim = {
				tempDim.x - cam.x,
				tempDim.y - cam.y,
				tempDim.w, tempDim.h
			};
		}
		else
		{
			finalDim = (SDL_Rect)linked->get_dim();
		}

		/*
		 *	Affiche le 'GObject' en fonction de son type.
		 *	L'affichage d'une texture se fait via la fonction
		 *	'SDL_RenderCopy', prenant entre autres en paramètres
		*	un outil de rendu et une texture.
		 *
		 */
		switch (type)
		{
		case ANIMATED:
			SDL_SetTextureAlphaMod(textures[currentTexture].t, alpha);
			SDL_RenderCopy(r, textures[currentTexture].t,
				&textures[currentTexture].clips[textures[currentTexture].animate()],
				&finalDim);
			break;

		case STATIC:
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
