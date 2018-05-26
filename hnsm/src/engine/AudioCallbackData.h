#pragma once

#include <iostream>
#include <vector>

//engine headers
#include "AudioData.h"

/*
 *	Constantes utiles à la gestion des sons
 *
 */
#define MAX_SOUNDS 32
#define CHANNELS 2	/* Constantes correspondant au nombres de canaux audio (2 = stéréo) */
#define MAX_VOLUME 2.0f
#define MIN_VOLUME 0.0f

/*
 *	Classe définissant le type de l'objet qui sera passé en paramètre
 *	du callback audio. Cette classe contient les données des fichier audio
 *	qui y sont chargés (membres 'loop' et 'sounds')
 *
 */
class AudioCallbackData
{
	/*
	 *	Volume des éléments "loop" et "sounds"
	 *
	 */
	float loop_vol;
	float sounds_vol;

public:
	/*
	 *	Membres contenant les données audio des éléments "loop" et "sounds"
	 *
	 */
	AudioData loop;
	AudioData sounds[MAX_SOUNDS];

	/*
	 *	Méthodes et constructeur: voir AudioCallbackData.cpp
	 *
	 */
	AudioCallbackData();
	~AudioCallbackData();

	void setLoop(AudioData);
	void setSound(unsigned index, AudioData data);
	void addSound(AudioData data);

	void setLoopVol(float vol);
	void setSoundsVol(float vol);

	void clearSounds();
	void clearLoop();

	float getLoopVol() const;
	float getSoundsVol() const;
};