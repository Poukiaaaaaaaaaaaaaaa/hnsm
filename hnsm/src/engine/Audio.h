#pragma once

#include <iostream>

//PortAudio headers
#include <PortAudio\portaudio.h>

#include "AudioData.h"
#include "AudioCallbackData.h"

/*
 *	Tags utilisés pour l'arrêt du flux audio (voir Audio.cpp)
 *
 */
enum StopTag
{
	STOP, ABORT
};

/*
 *	Classe contenant les membres nécessaires à la gestion de l'audio.
 *	Permet la simplification du chargement des fichiers audio et de
 *	leur utilisation.
 *
 */
class Audio
{
public:
	/*
	 *	'userData': membre contenant les donnés audio nécessaires à
	 *	la lecture. Ce membre est passé à la fonction 'streamCallback',
	 *	appelée automatiquement très fréquement après l'initialisation
	 *	du flux audio
	 *
	 */
	AudioCallbackData userData;

	/*
	 *	'audioStream': désigne l'emplacement mémoire du flux audio une fois celui-ci initialisé
	 *
	 */
	PaStream *audioStream;

public:
	/*
	 *	Méthodes et constructeur: voir Audio.cpp
	 *
	 */
	Audio();
	~Audio();

	void startStream();
	void stopStream(StopTag);

	void playSound(AudioData);
	void addSound(AudioData);
	void setLoop(AudioData);
	
	void clearSounds();
	void clearLoop();
	
	void setGlobalVolume(float vol);
	void globalVolumeUp(float inc);
	void globalVolumeDown(float inc);
	void setLoopVol(float vol);
	void loopVolUp(float inc);
	void loopVolDown(float inc);
	void setSoundsVol(float vol);
	void soundsVolUp(float inc);
	void soundsVolDown(float inc);

	int get_streamStopped() const;
};