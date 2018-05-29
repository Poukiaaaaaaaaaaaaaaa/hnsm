#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <sstream>

//SDL headers
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <SDL2\SDL_ttf.h>
#include <SDL2\SDL_mixer.h>

//libsndfile headers
#include <libsndfile\sndfile.h>

//engine headers
#include "Log.h"
#include "GObject.h"
#include "Audio.h"
#include "Pair.h"
#include "UI.h"
#include "PObject.h"
#include "FileConfig/FileConfig.h"
#include "Block/Block.h"

#undef _main

/*
 *	Diff�rents modes de r�solution (d�finis par SDL)
 *
 */
enum FullscreenMode
{
	FULLSCREEN, DESKTOP, WINDOWED
};

/*
 *	Classe principale: contient l'int�gralit� des membres utiles
 *	� la gestion du jeu.
 *
 */
class Game
{
private:
	/*
	 *	Membres utiles � la fen�tre:
	 *
	 *	'title': Titre de la fen�tre
	 *	'wflags': param�tres de la fen�tre
	 *	'rflags': param�tres de rendu
	 *
	 */
	std::string title;
	SDL_WindowFlags wflags;
	SDL_RendererFlags rflags;

	/*
	 *	Membres utiles � la gestion de la fr�quence d'actualisation
	 *	du jeu (nombre d'images par seconde)
	 *
	 */
	Uint64 freq;
	Uint64 start = 0;
	Uint64 old_start = 0;
	Uint64 end = 0;
	Uint64 last = 0;

public:
	/*
	 *	'fps_target': nombre voulu d'images par secondes
	 *	'tick_target': nombre voulu de ticks processeur entre chaque image
	 *
	 */
	int fps_target;
	Uint64 tick_target;

	/*
	 *	'shouldQuit': condition de sortie du jeu
	 *	'isPaused': `true` si le jeu est en pause
	 *
	 */
	bool shouldQuit = false;
	bool isPaused = false;

	/*
	 *	Membres permettant la gestion des �v�nements:
	 *
	 *	'e': 
	 *	'mouse_x', 'mouse_y', etc: gestion des coordonn�es de la souris:
	 *	- 'mouse_x' et 'mouse_y': coordonn�es actuelles de la souris
	 *	- 'pmouse_x' et 'pmouse_y': coordonn�es de la souris � l'image pr�c�dente
	 *
	 */
	SDL_Event e;
	int mouse_x, mouse_y;
	int pmouse_x, pmouse_y;

	/*
	 *	'gui': vecteur contenant les objets de l'interface utilisateur (voir UI.h)
	 *	'game': vecteur contenant les objets du jeu (voir UI.h)
	 *
	 */
	std::vector<UI*> gui;
	std::vector<UI*> game;

	/*
	 *	Membres utiles � la gestion des graphismes:
	 *
	 *	'w': membre pointant vers les donn�es de la fen�tre
	 *	'dim': membre contenant les dimensions de la fen�tre
	 *	'r': membre pointant vers l'outil de rendu
	 *	't': vecteur contenant toutes les textures charg�es dans le
	 *	constructeur (voir Game.cpp)
	 *	'layers': vecteur contenant chaque couche de l'affichage (chaque couche
	 *	�tant compos�e des objets graphiques � afficher)
	 *	'camera': membre correspondant � la "cam�ra", utilis� pour le rendu de
	 *	certains objets graphiques
	 *
	 */
	SDL_Window *w;
	SDL_Rect dim;
	SDL_Renderer *r;
	std::vector<std::vector<GObject>> layers;
	std::vector<PObject> physicalEngine;
	Pair camera;

	/*
	 *	Membres permettant la gestion de l'audio:
	 *
	 *	'a': membre principal, contenant toutes les m�thodes li�es � l'audio (voir Audio.cpp)
	 *	'sound': vecteur contenant des donn�es audio charg�es dans le constructeur (voir Game.cpp)
	 *	'loop': vecteur contenant des donn�es audio concernant les "loop", �galement charg�es
	 *	dans le constructeur (voir Game.cpp)
	 *
	 */
	Audio a;
	std::vector<AudioData> sound;
	std::vector<AudioData> loop;

	FileConfig config;

public:
	/*
	 *	M�thodes et construteur: voir Game.cpp
	 *
	 */
	void tick_start();
	void tick_end();
	int get_current_framerate() const;
	int get_frame_length() const;
	void set_frame_rate(unsigned fps);

	Game(std::string title,
		std::string cfgPath,
		SDL_WindowFlags wflags,
		SDL_RendererFlags rflags
	);

	void resize(int w, int h);
	void setFullscreenMode(FullscreenMode);

	void moveCamera(int x, int y);
	void restoreCamera();
	//void setCamera(int x, int y);

	void loadLevel(std::string lvlPath);

	int get_w() const;
	int get_h() const;
	Uint64 get_freq() const;
	SDL_WindowFlags getWindowFlags() const;
	SDL_RendererFlags getRendererFlags() const;
	void setRendererFlags(SDL_RendererFlags flags);

	void events();
	void process();
	void render();

	~Game();
	void cleanup();
};
