#include "Game.h"

/*
 *	Les deux méthodes suivantes permettent la gestion de la
 *	fréquence d'actualisation du moteur.
 *
 *	'tick_start': cette méthode est appelée au début de chaque image.
 *	Elle permet de récupérer puis de stocker la valeur de temps, en ticks
 *	processeur, au moment où elle a été appelée. Elle permet également de
 *	récupérer les coordonnées de la souris dans des membres de l'objet
 *	global via la fonction 'SDL_GetMouseState'
 *
 *	'tick_end': cette méthode est appelée à la fin de chaque image. Elle
 *	calcule le temps écoulé entre son appel et celui de 'tick_start' puis
 *	interromp le programme pendant un temps calculé à partir du temps écoulé.
 *	Ceci permettant aux image de s'actualiser à une fréquence stable.
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
 *	Méthode 'get_current_framerate':
 *
 *	Cette méthode renvoie le nombre actuel d'image par secondes. Calculé
 *	à partir du temps écoulé à la dernière image.
 *
 */
int Game::get_current_framerate() const
{
	return (int)((float)freq / (last - old_start));
}

/*
 *	Méthode 'get_frame_length':
 *
 *	Renvoie la durée théorique d'une image en milisecondes.
 *
 */
int Game::get_frame_length() const
{
	return (int)(((float)tick_target / freq) * 1000);
}

/*
 *	Méthode 'set_frame_rate':
 *
 *	Cette méthode permet de redéfinir le nombre d'images par secondes.
 *	Ainsi modifiant directement la fréquence d'actualisation.
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
 *	la fenêtre ainsi que de outil de rendu. Certains paramètres du jeu sont
 *	alors directement définis à l'intérieur du fichier de chemin 'cfgPath',
 *	contenant des propriétés telles que les largeur et hauteur de la fenêtre,
 *	etc. (chargement via un fichier de configuration crée par Amayun).
 *
 */
Game::Game(std::string title, std::string cfgPath, SDL_WindowFlags wflags, SDL_RendererFlags rflags
/*, FileConfig texturePath */)
	: title(title), freq(SDL_GetPerformanceFrequency()), wflags(wflags), rflags(rflags), config(cfgPath)
{
	/*
	 *	Initialisation du complément de SDL permettant la
	 *	lecture de fichiers .ttf (True Type Font) et .otf
	 *	(Open Type Font), permettant entre autre la création
	 *	d'objets graphiques type texte.
	 *
	 */
	if (TTF_Init())
	{
		Log::toSdlError("error.log", "TTF_Init: ", __FILE__, __LINE__);
	}

	int wx, wy;
	int ww, wh;

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
	 *	Initialisation de la fenêtre via 'SDL_CreateWindow', ainsi
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
	 *	Initialisation de la caméra, soit un objet théorique
	 *	sur lequel se base l'affichage des objets graphiques,
	 *	puis démarrage du flux audio principal.
	 *
	 */
	camera.x = camera.y = 0;

	/*player = new Player(this, { IMG_LoadTexture(r, texturePath.getPath("player").c_str())}, { 1.0, 0.0, 150, 300 });*/

	a.startStream();
}

/*
 *	Méthode 'moveCamera':
 *
 *	Déplace la caméra le long des axes x et y.
 *
 *	Note: La soustraction de la coordonnée en y vient du fait que
 *	le sens positif d'incrémentation des coordonnée en y se fait de
 *	haut en bas (et non de bas en haut).
 *
 */
void Game::moveCamera(int xShift, int yShift)
{
	camera.x += xShift;
	camera.y -= yShift;
}

/*
 *	Méthode 'restoreCamera':
 *
 *	Réinitialise les coordonnées de la caméra.
 *
 */
void Game::restoreCamera()
{
	camera.x = camera.y = 0;
}

// Fonction de chargement d'un niveau
void Game::loadLevel(std::string lvlPath)
{
	std::ifstream lvlFile(lvlPath, std::ios::in); // On ouvre le fichier du niveau en lecture seule
	std::string line = "";
	int tmW = 1, tmH = 1; // On initialise les variables pour la hauteur et la largeur de la tiledmap

	getline(lvlFile, line, '\n'); // On récupère la première ligne et on la stocke dans une chaîne de caractère

	size_t xPos = line.find('x'); // On récupère la position du caractère x dans la ligne

	if (xPos == std::string::npos) // Si cette position est négative, c'est que le caractère recherché n'existe pas
	{
		Log::toFile("loadLevel.error", "Une erreur est survenue lors du chargement du niveau: " + lvlPath);
		return; // On signale une erreur et on arrête la fonction ici
	}

	try // On essaie de convertir les valeurs avant et après le caractère x
	{
		tmW = std::stoi(line.substr(0, xPos));
		tmH = std::stoi(line.substr(xPos + 1));
	}
	catch (std::invalid_argument)
	{
		Log::toFile("loadLevel.error", "Une erreur est survenue lors de la lecture des dimensions du niveau: " + lvlPath);
		return; // Si il y a eu une erreur de conversion, on signale une erreur et on arrête la fonction ici
	}

	std::vector<GObject> blocks; // On crée la liste d'objets graphiques
	FileConfig blockPath(config.getPath("blocks")); // On charge le fichier contenant les chemins vers les images des blocs

	for (int n = 0; !lvlFile.eof() && n < tmH; n++) // Tant que l'on est pas à la fin du fichier ou à la fin des lignes 
	{
		getline(lvlFile, line, '\n'); // On récupère la ligne actuelle pour la mettre dans une chaîne de caractère
		std::istringstream strLine(line); // On fait de cette chaîne de caractère un flux de caractères
		std::string blockLabel = "";

		for (int m = 0; !strLine.eof() && m < tmW; m++) // Tant que l'on n'a pas atteint le bout de la ligne
		{
			getline(strLine, blockLabel, '|'); // On stocke dans une chaîne de caractère la valeur entre chaque |  

			Block block({ (float)(m * 10), (float)(n * 10), (m + 1) * 10, (n + 1) * 10 },
				        IMG_LoadTexture(r, blockPath.getPath(blockLabel).c_str())
			            ); // On crée un objet bloc
			// Ses coordonnées sont définies par sa position dans la tiledMap
			// Son image est récupérée depuis le fichier contenant les paths

			// this->game.push_back(&block);

			blocks.push_back(block.getGObject());
		}
	}

	this->layers[0] = blocks; // On met la liste des blocs dans une couche des objets graphiques
	lvlFile.close(); // On ferme le fichier du niveau
}

/*
*	Méthode 'events':
*
*	Cette méthode est appelée à chaque image. Son but est d'appeler la
*	méthode 'events' de chaque objet du jeu, permettant la gestion des
*	entrées du joueur (clavier, souris, etc).
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
	 *	'gui': vecteur contenant des pointeurs vers les éléments
	 *	de l'interface utilisateur (menus, boutons, etc). Ces éléments
	 *	ne sont pas affectés par l'état du jeu (en pause ou non).
	 *
	 *	'game': vecteur contenant des pointeurs vers les éléments du
	 *	jeu (joueur, entités, décorations etc), qui seront, eux, affectés
	 *	par l'état de pause du jeu.
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
 *	Méthode 'process':
 *
 *	Cette méthode va appeler la méthode 'update' de chaque objet du
 *	jeu. Leur méthode 'process' est principalement utilisée pour le
 *	calcul de positions ou de vitesses, de collisions, de modifications
 *	à l'objet graphique, etc.
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
 *	Méthode 'render':
 *
 *	Cette méthode permet l'affichage de la totalité des
 *	éléments graphiques présents dans le membre 'layers'.
 *
 */
void Game::render()
{
	SDL_RenderClear(r);
	for (unsigned i = 0; i < 10; i++)
	{
		for (unsigned j = 0; j < layers[i].size(); j++)
		{
			layers[i][j].draw(r, camera);
		}
	}
	SDL_RenderPresent(r);
}

/*
 *	Méthode 'resize':
 *
 *	Modifie la taille de la fenêtre.
 *
 */
void Game::resize(int width, int height)
{
	SDL_SetWindowSize(w, width, height);
	dim.w = width; dim.h = height;
}

/*
 *	Méthode 'setFullscreenMode':
 *
 *	Modifie le mode d'affichage de la fenêtre (plein écran, fenêtré, etc).
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
 *	Méthodes 'get_w' et 'get_h':
 *
 *	Retournent respectivement la largeur et la hauteur de la fenêtre.
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
 *	Méthode 'get_freq':
 *
 *	Retourne la fréquence processeur.
 *
 */
Uint64 Game::get_freq() const
{
	return freq;
}

/*
 *	Méthodes 'get(Window/Renderer)Flags':
 *
 *	Retournent les propriétés de la fenêtre/de l'outil de rendu.
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
 *	Méthode 'setRendererFlags':
 *
 *	Réinitialise l'outil de rendu avec des propriétés précisées
 *	en paramètre 'flags'.
 *
 */
void Game::setRendererFlags(SDL_RendererFlags flags)
{
	SDL_DestroyRenderer(r);
	r = SDL_CreateRenderer(w, -1, flags);
}

/*
 *	Méthode 'cleanup':
 *
 *	Appelée au déconstructeur de l'objet global, cette
 *	méthode permet le nettoyage mémoire des différents
 *	éléments (ici la fenêtre, l'outil de rendu ainsi que
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
