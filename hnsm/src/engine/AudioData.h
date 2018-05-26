#pragma once

#include <iostream>
#include <vector>

//libsndfile headers
#include <libsndfile\sndfile.h>

#include "Log.h"

/*
 *	Classe d�finissant le type des objets contenant les
 *	donn�es qui seront directement lues par l'appareil audio
 *
 */
class AudioData
{
public:
	/*
	 *	'info': Informations sur le fichier audio contenu
	 *	Contient des informations telles que le format, le
	 *	nombre de canaux du fichier audio, etc.
	 *
	 *	'audioFile': Membre pointant vers l'emplacement m�moire des
	 *	donn�es audio du fichier (cet emplacement �tant vide si le
	 *	fichier n'a pas encore �t� charg�)
	 *
	 *	'audioPath': Membre contenant le chemin vers le fichier audio li�
	 *
	 *	'exists': Membre permettant de tester "l'existence" du fichier.
	 *	`true` si le fichier a d�j� �t� charg�, `false` dans le cas contraire
	 *
	 *
	 *	'reloaded': `false` si le fichier audio est en train d'�tre recharg�,
	 *	`true` dans le cas contraire
	 *
	 */
	SF_INFO info;
	SNDFILE *audioFile;
	const char *audioPath;
	bool exists;

	/*
	 *	M�thode et constructeur: voir AudioData.cpp
	 *
	 */
	AudioData();
	AudioData(const char* filename);
	void reload();
	bool reloaded = false;
	~AudioData();
};
