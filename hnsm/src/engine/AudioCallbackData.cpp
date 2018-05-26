#include "AudioCallbackData.h"

/*
 *	Constructeur: initialise les sons en utilisant le constructeur
 *	par défaut de 'AudioData' (voir AudioData.cpp)
 *
 */
AudioCallbackData::AudioCallbackData()
	: loop_vol(1.0f), sounds_vol(1.0f)
{
	for (unsigned i = 0; i < MAX_SOUNDS; i++)
	{
		sounds[i] = AudioData();
	}
}

AudioCallbackData::~AudioCallbackData()
{
}

/*
 *	Méthode 'setLoop': remplace les données du membre 'loop' par
 *	celles du paramètre 'data', puis recharge le fichier audio
 *	contenu par 'loop' (méthode 'reload', voir AudioData.cpp)
 *
 */
void AudioCallbackData::setLoop(AudioData data)
{
	loop.audioPath = data.audioPath;
	loop.exists = true;
	loop.reload();
}

/*
 *	Remplace les données audio du membre 'sounds' à l'index 'index' par 
 *	celles du paramètre 'data'
 *	Comme pour 'setLoop', le fichier audio contenu par 'sounds[index]'
 *	est ensuite rechargé.
 *
 */
void AudioCallbackData::setSound(unsigned index, AudioData data)
{
	if (index < MAX_SOUNDS)
	{
		sounds[index].audioPath = data.audioPath;
		sounds[index].exists = true;
		sounds[index].reload();
	}
}

/*
 *	Méthode 'addSound': place les données du paramètre 'data' au
 *	premier emplacement libre du tableau 'sounds'
 *
 */
void AudioCallbackData::addSound(AudioData data)
{
	for (unsigned i = 0; i < MAX_SOUNDS; i++)
	{
		if (sounds[i].exists == false)
		{
			setSound(i, data);
			break;
		}
	}
}

/*
 *	Méthode 'clearSounds': réinitialise toutes les cases du
 *	tableau 'sounds' (constructeur par défaut de AudioData)
 *
 */
void AudioCallbackData::clearSounds()
{
	for (unsigned i = 0; i < MAX_SOUNDS; i++)
	{
		sounds[i] = AudioData();
	}
}

/*
 *	Méthode 'clearLoop': réinitialise le membre 'loop'
 *
 */
void AudioCallbackData::clearLoop()
{
	loop = AudioData();
}

/*
 *	Les méthodes suivantes s'occupent de la gestion du volume.
 *	Le volume de la "loop" et celui des "sounds" sont gérés séparément.
 *
 */
void AudioCallbackData::setLoopVol(float vol)
{
	/*
	 *	Vérifie si le volume se trouve entre le volume maximal
	 *	et le volume minimal, si oui remplace directement la valeur de
	 *	'loop_vol' par celle de 'vol', si non la valeur de 'loop_vol'
	 *	est remplacée soit `MIN_VOLUME` soit par `MAX_VOLUME`
	 *
	 */
	if (vol >= MIN_VOLUME && vol <= MAX_VOLUME)
	{
		loop_vol = vol;
	}
	else
	{
		if (vol < MIN_VOLUME)
		{
			loop_vol = MIN_VOLUME;
		}
		if (vol > MAX_VOLUME)
		{
			loop_vol = MAX_VOLUME;
		}
	}
}

/*
 *	Méthode 'setSoundsVol': même principe que 'setLoopVol'
 *
 */
void AudioCallbackData::setSoundsVol(float vol)
{
	if (vol >= MIN_VOLUME && vol <= MAX_VOLUME)
	{
		sounds_vol = vol;
	}
	else
	{
		if (vol < MIN_VOLUME)
		{
			sounds_vol = MIN_VOLUME;
		}
		if (vol > MAX_VOLUME)
		{
			sounds_vol = MAX_VOLUME;
		}
	}
}

/*
 *	Méthodes 'getLoopVol' et 'getSoundsVool': renvoient le volume de
 *	la "loop" et celui des "sounds" respectivement
 *
 */
float AudioCallbackData::getLoopVol() const
{
	return loop_vol;
}

float AudioCallbackData::getSoundsVol() const
{
	return sounds_vol;
}