#include "Game.h"

/*
 *	Les deux m�thodes suivantes permettent la gestion de la
 *	fr�quence d'actualisation du moteur.
 *
 *	'tick_start': cette m�thode est appel�e au d�but de chaque image.
 *	Elle permet de r�cup�rer puis de stocker la valeur de temps, en ticks
 *	processeur, au moment o� elle a �t� appel�e. Elle permet �galement de
 *	r�cup�rer les coordonn�es de la souris dans des membres de l'objet
 *	global via la fonction 'SDL_GetMouseState'
 *
 *	'tick_end': cette m�thode est appel�e � la fin de chaque image. Elle
 *	calcule le temps �coul� entre son appel et celui de 'tick_start' puis
 *	interromp le programme pendant un temps calcul� � partir du temps �coul�.
 *	Ceci permettant aux image de s'actualiser � une fr�quence stable.
 *
 */
void Game::tick_start()
{
	start = SDL_GetPerformanceCounter();
	SDL_GetMouseState(&mouse_x, &mouse_y);
}

void Game::tick_end()
{
	end = SDL_GetPerformanceCounter();

	if (tick_target != 0 && end - start < tick_target)
		SDL_Delay((Uint32)((float)(tick_target - end + start) / freq * 1000));

	pmouse_x = mouse_x; pmouse_y = mouse_y;

	old_start = start;
	last = SDL_GetPerformanceCounter();
}

/*
 *	M�thode 'get_current_framerate':
 *
 *	Cette m�thode renvoie le nombre actuel d'image par secondes. Calcul�
 *	� partir du temps �coul� � la derni�re image.
 *
 */
int Game::get_current_framerate() const
{
	return (int)((float)freq / (last - old_start));
}

/*
 *	M�thode 'get_frame_length':
 *
 *	Renvoie la dur�e th�orique d'une image en milisecondes.
 *
 */
int Game::get_frame_length() const
{
	return (int)(((float)tick_target / freq) * 1000);
}

/*
 *	M�thode 'set_frame_rate':
 *
 *	Cette m�thode permet de red�finir le nombre d'images par secondes.
 *	Ainsi modifiant directement la fr�quence d'actualisation.
 *
 */
void Game::set_frame_rate(unsigned fps)
{
	fps_target = fps;
	tick_target = (fps > 0) ? freq / fps_target : 0;
}

/*
 *	Constructeur:
 *
 *	Le constructeur de la classe 'Game' permet l'initialisation de
 *	la fen�tre ainsi que de outil de rendu. Certains param�tres du jeu sont
 *	alors directement d�finis � l'int�rieur du fichier de chemin 'cfgPath',
 *	contenant des propri�t�s telles que les largeur et hauteur de la fen�tre,
 *	etc. (chargement via un fichier de configuration cr�e par Amayun).
 *
 */
Game::Game(std::string title, std::string cfgPath, SDL_WindowFlags wflags, SDL_RendererFlags rflags)
	: title(title), freq(SDL_GetPerformanceFrequency()), wflags(wflags), rflags(rflags)
{
	/*
	 *	Initialisation du compl�ment de SDL permettant la
	 *	lecture de fichiers .ttf (True Type Font) et .otf
	 *	(Open Type Font), permettant entre autre la cr�ation
	 *	d'objets graphiques type texte.
	 *
	 */
	if (TTF_Init())
	{
		Log::toSdlError("error.log", "TTF_Init: ", __FILE__, __LINE__);
	}

	float wx, wy;
	int ww, wh;
	FileConfig config(cfgPath);

	FileConfig dispCfg(config.getPath("display"));

	fps_target = dispCfg.getVal("framerate");
	ww = dispCfg.getVal("width");
	wh = dispCfg.getVal("height");

	tick_target = fps_target ? freq / fps_target : 0;

	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode(0, &dm);

	if (!ww || !wh)
	{
		ww = dm.w; wh = dm.h;
	}

	wx = dm.w / 2 - ww / 2;
	wy = dm.h / 2 - wh / 2;

	dim = { wx, wy, ww, wh };

	/*
	 *	Initialisation de la fen�tre via 'SDL_CreateWindow', ainsi
	 *	que de sont outil de rendu (ici r) via 'SDL_CreateRenderer'.
	 *
	 */
	w = SDL_CreateWindow(title.c_str(), wx, wy, ww, wh, wflags);
	if (w == nullptr)
	{
		Log::toSdlError("error.log", "SDL_CreateWindow: ", __FILE__, __LINE__);
	}

	r = SDL_CreateRenderer(w, -1, rflags);
	if (r == nullptr)
	{
		Log::toSdlError("error.log", "SDL_CreateRenderer: ", __FILE__, __LINE__);
	}

	/*
	 *	Initialisation de la cam�ra, soit un objet th�orique
	 *	sur lequel se base l'affichage des objets graphiques,
	 *	puis d�marrage du flux audio principal.
	 *
	 */
	camera.x = camera.y = 0;

	a.startStream();
}

/*
 *	M�thode 'moveCamera':
 *
 *	D�place la cam�ra le long des axes x et y.
 *
 *	Note: La soustraction de la coordonn�e en y vient du fait que
 *	le sens positif d'incr�mentation des coordonn�e en y se fait de
 *	haut en bas (et non de bas en haut).
 *
 */
void Game::moveCamera(int xShift, int yShift)
{
	camera.x += xShift;
	camera.y -= yShift;
}

/*
 *	M�thode 'restoreCamera':
 *
 *	R�initialise les coordonn�es de la cam�ra.
 *
 */
void Game::restoreCamera()
{
	camera.x = camera.y = 0;
}

/*
 *	M�thode 'events':
 *
 *	Cette m�thode est appel�e � chaque image. Son but est d'appeler la
 *	m�thode 'events' de chaque objet du jeu, permettant la gestion des
 *	entr�es du joueur (clavier, souris, etc).
 *
 */
void Game::events()
{
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			shouldQuit = true;
		}
	}

	/*
	 *	'gui': vecteur contenant des pointeurs vers les �l�ments
	 *	de l'interface utilisateur (menus, boutons, etc). Ces �l�ments
	 *	ne sont pas affect�s par l'�tat du jeu (en pause ou non).
	 *
	 *	'game': vecteur contenant des pointeurs vers les �l�ments du
	 *	jeu (joueur, entit�s, d�corations etc), qui seront, eux, affect�s
	 *	par l'�tat de pause du jeu.
	 *
	 */
	for (unsigned i = 0; i < gui.size(); i++)
	{
		gui[i]->events();
	}

	if (!isPaused)
	{
		for (unsigned i = 0; i < game.size(); i++)
		{
			game[i]->events();
		}
	}
}

/*
 *	M�thode 'process':
 *
 *	Cette m�thode va appeler la m�thode 'update' de chaque objet du
 *	jeu. Leur m�thode 'process' est principalement utilis�e pour le
 *	calcul de positions ou de vitesses, de collisions, de modifications
 *	� l'objet graphique, etc.
 *
 */
void Game::process()
{
	for (unsigned i = 0; i < gui.size(); i++)
	{
		gui[i]->update();
	}

	if (!isPaused)
	{
		
		for (unsigned i = 0; i < physicalEngine.size(); i++)
		{
			physicalEngine[i].process(physicalEngine);	
		}
	
		for (unsigned i = 0; i < game.size(); i++)
		{
			game[i]->update();
		}
	}
}

/*
 *	M�thode 'render':
 *
 *	Cette m�thode permet l'affichage de la totalit� des
 *	�l�ments graphiques pr�sents dans le membre 'layers'.
 *
 */
void Game::render()
{
	SDL_RenderClear(r);
	for (unsigned i = 0; i < layers.size(); i++)
	{
		for (unsigned j = 0; j < layers[i].size(); j++)
		{
			layers[i][j].draw(r, camera);
		}
	}
	SDL_RenderPresent(r);
}

/*
 *	M�thode 'resize':
 *
 *	Modifie la taille de la fen�tre.
 *
 */
void Game::resize(int width, int height)
{
	SDL_SetWindowSize(w, width, height);
	dim.w = width; dim.h = height;
}

/*
 *	M�thode 'setFullscreenMode':
 *
 *	Modifie le mode d'affichage de la fen�tre (plein �cran, fen�tr�, etc).
 *
 */
void Game::setFullscreenMode(FullscreenMode mode)
{
	switch (mode)
	{
	case FULLSCREEN:
		SDL_SetWindowFullscreen(w, SDL_WINDOW_FULLSCREEN);
		break;

	case DESKTOP:
		SDL_SetWindowFullscreen(w, SDL_WINDOW_FULLSCREEN_DESKTOP);
		break;

	case WINDOWED:
		SDL_SetWindowFullscreen(w, NULL);
		break;
	}
}

/*
 *	M�thodes 'get_w' et 'get_h':
 *
 *	Retournent respectivement la largeur et la hauteur de la fen�tre.
 *
 */
int Game::get_w() const
{
	return dim.w;
}

int Game::get_h() const
{
	return dim.h;
}

/*
 *	M�thode 'get_freq':
 *
 *	Retourne la fr�quence processeur.
 *
 */
Uint64 Game::get_freq() const
{
	return freq;
}

/*
 *	M�thodes 'get(Window/Renderer)Flags':
 *
 *	Retournent les propri�t�s de la fen�tre/de l'outil de rendu.
 *
 */
SDL_WindowFlags Game::getWindowFlags() const
{
	return wflags;
}

SDL_RendererFlags Game::getRendererFlags() const
{
	return rflags;
}

/*
 *	M�thode 'setRendererFlags':
 *
 *	R�initialise l'outil de rendu avec des propri�t�s pr�cis�es
 *	en param�tre 'flags'.
 *
 */
void Game::setRendererFlags(SDL_RendererFlags flags)
{
	SDL_DestroyRenderer(r);
	r = SDL_CreateRenderer(w, -1, flags);
}

/*
 *	M�thode 'cleanup':
 *
 *	Appel�e au d�constructeur de l'objet global, cette
 *	m�thode permet le nettoyage m�moire des diff�rents
 *	�l�ments (ici la fen�tre, l'outil de rendu ainsi que
 *	le flux audio principal).
 *
 */
Game::~Game()
{
	cleanup();
}

void Game::cleanup()
{
	SDL_DestroyWindow(w);
	SDL_DestroyRenderer(r);
	a.stopStream(ABORT);
}
