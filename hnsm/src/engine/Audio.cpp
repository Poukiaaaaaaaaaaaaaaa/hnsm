#include "Audio.h"

/*
 *	'streamCallback': fonction appel�e automatiquement � une certaine fr�quence
 *	(d�finie lors de l'initialisation du flux audio)
 *
 *	Param�tres:
 *	- 'input': pointeur utilis� pour l'enregistrement audio, d�signe un emplacement
 *	m�moire dans lequel sont stock�es les donn�es enregistr�es (membre inutilis�
 *	dans notre cas)
 *
 *	- 'output': poiteur d�signant un emplacement m�moire directement lu par l'appareil
 *	audio, c'est � cet emplacement m�moire que les donn�es audio seront plac�es
 *
 *	- 'frameCount': membre g�r� par l'appareil audio, contenant le nombre d'�chantillons
 *	audio pouvant �tres plac�s � l'emplacement point� par 'output'
 *
 *	- 'tInfo': informations de temps concernant les buffers 'input' et 'output' (membre
 *	non utilis� dans notre cas)
 *
 *	- 'statusFlags': informations concernant le callback audio, permet de savoir si
 *	les buffers 'input' et 'output' manquent ou d�bordent de donn�es (non utilis�)
 *
 *	- 'userData': membre contenant la totalit� des donn�es audio, plac�es � l'adresse
 *	point�e par 'output' dans notre cas.
 *
 */
static int streamCallback(const void *input, void *output, unsigned long frameCount,
	const PaStreamCallbackTimeInfo *tInfo, PaStreamCallbackFlags statusFlags,
	void *userData)
{
	/*
	 *	R�cup�ration des donn�es audio dans un conteneur utilisable (autre que `void*`).
	 *	M�me proc�d� pour 'output': un membre de type `void*` est inutilisable
	 *
	 */
	AudioCallbackData *data = (AudioCallbackData*)userData;
	float *out = (float*)output;

	/*
	 *	Variables permettant de r�cup�rer le nombre d'�chantillons charg�s depuis
	 *	les fichiers audio (s�par�s en deux cat�gories: "loop" et "sound")
	 *
	 */
	sf_count_t loop_item_count;
	sf_count_t sound_item_count;

	/*
	 *	Param�tres unutilis�s, convertis en `void` afin de lib�rer de la m�moire
	 *
	 */
	(void)tInfo;
	(void)statusFlags;
	(void)input;

	/*
	 *	Buffer temporaire, dans lequel sont plac�es les donn�es audio non
	 *	trait�es. Ce dernier est ensuite directement plac� � l'emplacement point�
	 *	par 'out' une fois les donn�es audio trait�es (c.f. gestion du volume, etc)
	 *
	 */
	float *temp = (float*)malloc(sizeof(float) * frameCount * CHANNELS);

	/*
	 *	L'emplacement point� par 'out' est vid� � chaque appel.
	 *	Est possible car les donn�es audio plac�es � 'out' crrespondent
	 *	exactement aux donn�es audio lues par l'appareil entre deux
	 *	appels du callback
	 *
	 */
	memset(out, 0, sizeof(float) * frameCount * CHANNELS);

	/*
	 *	Chargement des donn�es audio pr�sentes dans les fichiers audio.
	 *	Les donn�es charg�es font au final une taille correspondant
	 *	au nombre d'�chantillons lus par l'appareil entre deux appels
	 *	(ce nombre �tant contenu par 'frameCount')
	 *
	 */
	if (data->loop.exists == true)
	{
		/*
		 *	Les donn�es ne seront pas charg�es si le fichier audio est
		 *	en train d'�tre charg�, le contraire entra�nant une fuite m�moire
		 *
		 */
		if (data->loop.reloaded == true)
			loop_item_count = sf_read_float(data->loop.audioFile, temp, frameCount *
				data->loop.info.channels);
		else loop_item_count = frameCount;

		/*
		 *	Placement des donn�es audio dans 'out' une fois trait�es
		 *	Le traitement, dans notre cas, consiste uniquement en la
		 *	gestion du volume
		 *
		 */
		for (unsigned i = 0; i < frameCount * CHANNELS; i++)
		{
			out[i] += temp[i] * data->getLoopVol();
		}

		/*
		 *	Une fois arriv� � la fin du fichier audio, celui-ci et recharg�.
		 *	Se lecture sera alors impossible jusqu'� ce qu'il le soit
		 *	Ce proc�d� est unique � la "loop" qui, contrairement aux "sound",
		 *	se r�p�te ind�finiment
		 *
		 */
		if (loop_item_count < frameCount)
		{
			data->loop.reload();
		}
	}

	/*
	 *	Les sons "loop" et "sound" sont proc�d�s diff�remment, donc s�par�ment:
	 *	La principale diff�rence �tant qu'une seule "loop" peut �tre lue � la fois,
	 *	contrairement aux "sound" pour lesquels le nombre des lectures simultan�es est
	 *	limit� � `MAX_SOUNDS` (ici 32)
	 *
	 */
	for (unsigned i = 0; i < MAX_SOUNDS; i++)
	{
		if (data->sounds[i].exists == true)
		{
			if (data->sounds[i].reloaded == true)
				sound_item_count = sf_read_float(data->sounds[i].audioFile, temp, frameCount *
					data->sounds[i].info.channels);
			else sound_item_count = frameCount;
			
			for (unsigned i = 0; i < frameCount * CHANNELS; i++)
			{
				out[i] += temp[i] * data->getSoundsVol();
			}

			if (sound_item_count < frameCount)
			{
				/*
				 *	Une fois un son termin�, l'objet du type 'AudioData' le
				 *	contenant est r�initialis�
				 *
				 */
				for (unsigned j = i; j < MAX_SOUNDS - 1; j++)
				{
					data->sounds[j] = data->sounds[j + 1];
				}

				data->sounds[MAX_SOUNDS - 1] = AudioData();
			}
		}
	}

	/*
	 *	Le buffer temporaire est lib�r� apr�s chaque appel, permet
	 *	une utilisation fixe de la m�moire
	 *
	 */
	free(temp);

	/*
	 *	'paContinue' est un tag utilis� par PortAudio, indiquant que le
	 *	callback devrait �tre appel� normalement apr�s celui-ci
	 *
	 */
	return paContinue;
}

/*
 *	Le constructeur de la classe Audio s'occupe principalement de
 *	l'initialisation du flux audio.
 * 
 */
Audio::Audio()
{
	/*
	 *	R�cup�ration des informations concernant l'appareil de
	 *	sortie par d�faut
	 *
	 */
	PaDeviceIndex index = Pa_GetDefaultOutputDevice();
	const PaDeviceInfo *info = Pa_GetDeviceInfo(index);

	/*
	 *	Initialisation du flux audio � partir des param�tres audio
	 *	de l'appareil de lecture  par d�faut
	 *
	 */
	if (info == nullptr)
	{
		Log::toFile("error.log", "[ERROR]:Couldn't retrieve device information");
	}
	else
	{
		/*
		 *	Pa_OpenDefaultStream initialise le flux audio puis d�finit le
		 *	callback lui �tant li� (ici 'streamCallback')
		 *
		 */
		Pa_OpenDefaultStream(&audioStream, 0, 2, paFloat32, info->defaultSampleRate,
			paFramesPerBufferUnspecified, streamCallback, &userData
		);
	}
}

/*
 *	M�thode 'startStream': d�marre le flux audio, ne fonctionne
 *	que si ce dernier a correctement �t� initialis�
 *
 */
void Audio::startStream()
{
	Pa_StartStream(audioStream);
}

/*
 *	M�thode 'stopStream': stoppe le flux audio.
 *	L'arr�t poss�dant le tag `STOP` finira de lire les donn�es
 *	audio avant d'arr�ter le flux.
 *	Celui poss�dant le  tag `ABORT` forcera l'arr�t du flux.
 *
 */
void Audio::stopStream(StopTag tag)
{
	if (tag == STOP)
		Pa_StopStream(audioStream);
	else if (tag == ABORT)
		Pa_AbortStream(audioStream);
}

/*
 *	M�thode 'get_streamStopped': retourne l'�tat d'arr�t du flux
 *	sous la forme d'un bool�en
 *
 */
PaError Audio::get_streamStopped() const
{
	return Pa_IsStreamStopped(audioStream);
}

/*
 *	M�thode 'setLoop': modifie la "loop" pour un autre �l�ment du type 'AudioData'
 *
 */
void Audio::setLoop(AudioData data)
{
	userData.setLoop(data);
}

/*
 *	M�thode 'playSound': vide l'int�gralit� des "sound" actuels
 *	pour n'en jouer qu'un seul (correspondant au param�tre 'data')
 *
 */
void Audio::playSound(AudioData data)
{
	for (unsigned i = 0; i < MAX_SOUNDS; i++)
	{
		userData.sounds[i] = AudioData();
	}

	userData.setSound(0, data);
}

/*
 *	Les m�thodes suivantes servent � la gestion des "sound" et "loop"
 *	ainsi qu'� la gestion de leur volume.
 *
 *	Celles-ci utilisent principalement les m�thode de la classe 'AudioData'
 *	(voir AudioData.h et AudioData.cpp pour plus de d�tails), elles seront
 *	donc peu ou non comment�es
 *
 */

/*
 *	Ajoute un son � l'array 'sounds' du membre 'userData'
 *
 */
void Audio::addSound(AudioData data)
{
	userData.addSound(data);
}

/*
 *	Vide l'array 'sounds' de 'userData'
 *
 */
void Audio::clearSounds()
{
	userData.clearSounds();
}

/*
 *	R�initialise le membre 'loop' de 'userData'
 *
 */
void Audio::clearLoop()
{
	userData.clearLoop();
}

void Audio::setGlobalVolume(float vol)
{
	setLoopVol(vol);
	setSoundsVol(vol);
}

void Audio::globalVolumeUp(float inc)
{
	loopVolUp(inc);
	soundsVolUp(inc);
}

void Audio::globalVolumeDown(float dec)
{
	loopVolDown(dec);
	soundsVolDown(dec);
}

void Audio::setLoopVol(float vol)
{
	userData.setLoopVol(vol);
}

void Audio::setSoundsVol(float vol)
{
	userData.setSoundsVol(vol);
}

void Audio::loopVolUp(float inc)
{
	if (userData.getLoopVol() + inc <= 2.0f)
	{
		userData.setLoopVol(userData.getLoopVol() + inc);
	}
	else
	{
		userData.setLoopVol(2.0f);
	}
}

void Audio::loopVolDown(float dec)
{
	if (userData.getLoopVol() - dec >= 0.0f)
	{
		userData.setLoopVol(userData.getLoopVol() - dec);
	}
	else
	{
		userData.setLoopVol(0.0f);
	}
}

void Audio::soundsVolUp(float inc)
{
	if (userData.getSoundsVol() + inc <= 2.0f)
	{
		userData.setSoundsVol(userData.getSoundsVol() + inc);
	}
	else
	{
		userData.setSoundsVol(2.0f);
	}
}

void Audio::soundsVolDown(float inc)
{
	if (userData.getSoundsVol() - inc >= 0.0f)
	{
		userData.setSoundsVol(userData.getSoundsVol() - inc);
	}
	else
	{
		userData.setSoundsVol(0.0f);
	}
}

/*
 *	D�constructeur de la classe Audio: ferme le flux audio
 *
 */
Audio::~Audio()
{
	Pa_CloseStream(audioStream);
}