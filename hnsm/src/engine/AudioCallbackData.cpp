#include "AudioCallbackData.h"

/*
 *	Constructeur: initialise les sons en utilisant le constructeur
 *	par d�faut de 'AudioData' (voir AudioData.cpp)
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
 *	M�thode 'setLoop': remplace les donn�es du membre 'loop' par
 *	celles du param�tre 'data', puis recharge le fichier audio
 *	contenu par 'loop' (m�thode 'reload', voir AudioData.cpp)
 *
 */
void AudioCallbackData::setLoop(AudioData data)
{
	loop.audioPath = data.audioPath;
	loop.exists = true;
	loop.reload();
}

/*
 *	Remplace les donn�es audio du membre 'sounds' � l'index 'index' par 
 *	celles du param�tre 'data'
 *	Comme pour 'setLoop', le fichier audio contenu par 'sounds[index]'
 *	est ensuite recharg�.
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
 *	M�thode 'addSound': place les donn�es du param�tre 'data' au
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
 *	M�thode 'clearSounds': r�initialise toutes les cases du
 *	tableau 'sounds' (constructeur par d�faut de AudioData)
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
 *	M�thode 'clearLoop': r�initialise le membre 'loop'
 *
 */
void AudioCallbackData::clearLoop()
{
	loop = AudioData();
}

/*
 *	Les m�thodes suivantes s'occupent de la gestion du volume.
 *	Le volume de la "loop" et celui des "sounds" sont g�r�s s�par�ment.
 *
 */
void AudioCallbackData::setLoopVol(float vol)
{
	/*
	 *	V�rifie si le volume se trouve entre le volume maximal
	 *	et le volume minimal, si oui remplace directement la valeur de
	 *	'loop_vol' par celle de 'vol', si non la valeur de 'loop_vol'
	 *	est remplac�e soit `MIN_VOLUME` soit par `MAX_VOLUME`
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
 *	M�thode 'setSoundsVol': m�me principe que 'setLoopVol'
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
 *	M�thodes 'getLoopVol' et 'getSoundsVool': renvoient le volume de
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