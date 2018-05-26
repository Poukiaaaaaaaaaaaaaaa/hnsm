#pragma once

#include <iostream>
#include <vector>

//libsndfile headers
#include <libsndfile\sndfile.h>

#include "Log.h"

/*
 *	Classe définissant le type des objets contenant les
 *	données qui seront directement lues par l'appareil audio
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
	 *	'audioFile': Membre pointant vers l'emplacement mémoire des
	 *	données audio du fichier (cet emplacement étant vide si le
	 *	fichier n'a pas encore été chargé)
	 *
	 *	'audioPath': Membre contenant le chemin vers le fichier audio lié
	 *
	 *	'exists': Membre permettant de tester "l'existence" du fichier.
	 *	`true` si le fichier a déjà été chargé, `false` dans le cas contraire
	 *
	 *
	 *	'reloaded': `false` si le fichier audio est en train d'être rechargé,
	 *	`true` dans le cas contraire
	 *
	 */
	SF_INFO info;
	SNDFILE *audioFile;
	const char *audioPath;
	bool exists;

	/*
	 *	Méthode et constructeur: voir AudioData.cpp
	 *
	 */
	AudioData();
	AudioData(const char* filename);
	void reload();
	bool reloaded = false;
	~AudioData();
};
