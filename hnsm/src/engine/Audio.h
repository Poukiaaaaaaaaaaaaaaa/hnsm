#pragma once

#include <iostream>

//PortAudio headers
#include <PortAudio\portaudio.h>

#include "AudioData.h"
#include "AudioCallbackData.h"

/*
 *	Tags utilis�s pour l'arr�t du flux audio (voir Audio.cpp)
 *
 */
enum StopTag
{
	STOP, ABORT
};

/*
 *	Classe contenant les membres n�cessaires � la gestion de l'audio.
 *	Permet la simplification du chargement des fichiers audio et de
 *	leur utilisation.
 *
 */
class Audio
{
public:
	/*
	 *	'userData': membre contenant les donn�s audio n�cessaires �
	 *	la lecture. Ce membre est pass� � la fonction 'streamCallback',
	 *	appel�e automatiquement tr�s fr�quement apr�s l'initialisation
	 *	du flux audio
	 *
	 */
	AudioCallbackData userData;

	/*
	 *	'audioStream': d�signe l'emplacement m�moire du flux audio une fois celui-ci initialis�
	 *
	 */
	PaStream *audioStream;

public:
	/*
	 *	M�thodes et constructeur: voir Audio.cpp
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