#pragma once

#include <iostream>
#include <vector>

//engine headers
#include "AudioData.h"

/*
 *	Constantes utiles � la gestion des sons
 *
 */
#define MAX_SOUNDS 32
#define CHANNELS 2	/* Constantes correspondant au nombres de canaux audio (2 = st�r�o) */
#define MAX_VOLUME 2.0f
#define MIN_VOLUME 0.0f

/*
 *	Classe d�finissant le type de l'objet qui sera pass� en param�tre
 *	du callback audio. Cette classe contient les donn�es des fichier audio
 *	qui y sont charg�s (membres 'loop' et 'sounds')
 *
 */
class AudioCallbackData
{
	/*
	 *	Volume des �l�ments "loop" et "sounds"
	 *
	 */
	float loop_vol;
	float sounds_vol;

public:
	/*
	 *	Membres contenant les donn�es audio des �l�ments "loop" et "sounds"
	 *
	 */
	AudioData loop;
	AudioData sounds[MAX_SOUNDS];

	/*
	 *	M�thodes et constructeur: voir AudioCallbackData.cpp
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