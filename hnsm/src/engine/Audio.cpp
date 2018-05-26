#include "Audio.h"

/*
 *	'streamCallback': fonction appelée automatiquement à une certaine fréquence
 *	(définie lors de l'initialisation du flux audio)
 *
 *	Paramètres:
 *	- 'input': pointeur utilisé pour l'enregistrement audio, désigne un emplacement
 *	mémoire dans lequel sont stockées les données enregistrées (membre inutilisé
 *	dans notre cas)
 *
 *	- 'output': poiteur désignant un emplacement mémoire directement lu par l'appareil
 *	audio, c'est à cet emplacement mémoire que les données audio seront placées
 *
 *	- 'frameCount': membre géré par l'appareil audio, contenant le nombre d'échantillons
 *	audio pouvant êtres placés à l'emplacement pointé par 'output'
 *
 *	- 'tInfo': informations de temps concernant les buffers 'input' et 'output' (membre
 *	non utilisé dans notre cas)
 *
 *	- 'statusFlags': informations concernant le callback audio, permet de savoir si
 *	les buffers 'input' et 'output' manquent ou débordent de données (non utilisé)
 *
 *	- 'userData': membre contenant la totalité des données audio, placées à l'adresse
 *	pointée par 'output' dans notre cas.
 *
 */
static int streamCallback(const void *input, void *output, unsigned long frameCount,
	const PaStreamCallbackTimeInfo *tInfo, PaStreamCallbackFlags statusFlags,
	void *userData)
{
	/*
	 *	Récupération des données audio dans un conteneur utilisable (autre que `void*`).
	 *	Même procédé pour 'output': un membre de type `void*` est inutilisable
	 *
	 */
	AudioCallbackData *data = (AudioCallbackData*)userData;
	float *out = (float*)output;

	/*
	 *	Variables permettant de récupérer le nombre d'échantillons chargés depuis
	 *	les fichiers audio (séparés en deux catégories: "loop" et "sound")
	 *
	 */
	sf_count_t loop_item_count;
	sf_count_t sound_item_count;

	/*
	 *	Paramètres unutilisés, convertis en `void` afin de libérer de la mémoire
	 *
	 */
	(void)tInfo;
	(void)statusFlags;
	(void)input;

	/*
	 *	Buffer temporaire, dans lequel sont placées les données audio non
	 *	traitées. Ce dernier est ensuite directement placé à l'emplacement pointé
	 *	par 'out' une fois les données audio traitées (c.f. gestion du volume, etc)
	 *
	 */
	float *temp = (float*)malloc(sizeof(float) * frameCount * CHANNELS);

	/*
	 *	L'emplacement pointé par 'out' est vidé à chaque appel.
	 *	Est possible car les données audio placées à 'out' crrespondent
	 *	exactement aux données audio lues par l'appareil entre deux
	 *	appels du callback
	 *
	 */
	memset(out, 0, sizeof(float) * frameCount * CHANNELS);

	/*
	 *	Chargement des données audio présentes dans les fichiers audio.
	 *	Les données chargées font au final une taille correspondant
	 *	au nombre d'échantillons lus par l'appareil entre deux appels
	 *	(ce nombre étant contenu par 'frameCount')
	 *
	 */
	if (data->loop.exists == true)
	{
		/*
		 *	Les données ne seront pas chargées si le fichier audio est
		 *	en train d'être chargé, le contraire entraînant une fuite mémoire
		 *
		 */
		if (data->loop.reloaded == true)
			loop_item_count = sf_read_float(data->loop.audioFile, temp, frameCount *
				data->loop.info.channels);
		else loop_item_count = frameCount;

		/*
		 *	Placement des données audio dans 'out' une fois traitées
		 *	Le traitement, dans notre cas, consiste uniquement en la
		 *	gestion du volume
		 *
		 */
		for (unsigned i = 0; i < frameCount * CHANNELS; i++)
		{
			out[i] += temp[i] * data->getLoopVol();
		}

		/*
		 *	Une fois arrivé à la fin du fichier audio, celui-ci et rechargé.
		 *	Se lecture sera alors impossible jusqu'à ce qu'il le soit
		 *	Ce procédé est unique à la "loop" qui, contrairement aux "sound",
		 *	se répète indéfiniment
		 *
		 */
		if (loop_item_count < frameCount)
		{
			data->loop.reload();
		}
	}

	/*
	 *	Les sons "loop" et "sound" sont procédés différemment, donc séparément:
	 *	La principale différence étant qu'une seule "loop" peut être lue à la fois,
	 *	contrairement aux "sound" pour lesquels le nombre des lectures simultanées est
	 *	limité à `MAX_SOUNDS` (ici 32)
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
				 *	Une fois un son terminé, l'objet du type 'AudioData' le
				 *	contenant est réinitialisé
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
	 *	Le buffer temporaire est libéré après chaque appel, permet
	 *	une utilisation fixe de la mémoire
	 *
	 */
	free(temp);

	/*
	 *	'paContinue' est un tag utilisé par PortAudio, indiquant que le
	 *	callback devrait être appelé normalement après celui-ci
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
	 *	Récupération des informations concernant l'appareil de
	 *	sortie par défaut
	 *
	 */
	PaDeviceIndex index = Pa_GetDefaultOutputDevice();
	const PaDeviceInfo *info = Pa_GetDeviceInfo(index);

	/*
	 *	Initialisation du flux audio à partir des paramètres audio
	 *	de l'appareil de lecture  par défaut
	 *
	 */
	if (info == nullptr)
	{
		Log::toFile("error.log", "[ERROR]:Couldn't retrieve device information");
	}
	else
	{
		/*
		 *	Pa_OpenDefaultStream initialise le flux audio puis définit le
		 *	callback lui étant lié (ici 'streamCallback')
		 *
		 */
		Pa_OpenDefaultStream(&audioStream, 0, 2, paFloat32, info->defaultSampleRate,
			paFramesPerBufferUnspecified, streamCallback, &userData
		);
	}
}

/*
 *	Méthode 'startStream': démarre le flux audio, ne fonctionne
 *	que si ce dernier a correctement été initialisé
 *
 */
void Audio::startStream()
{
	Pa_StartStream(audioStream);
}

/*
 *	Méthode 'stopStream': stoppe le flux audio.
 *	L'arrêt possédant le tag `STOP` finira de lire les données
 *	audio avant d'arrêter le flux.
 *	Celui possédant le  tag `ABORT` forcera l'arrêt du flux.
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
 *	Méthode 'get_streamStopped': retourne l'état d'arrêt du flux
 *	sous la forme d'un booléen
 *
 */
PaError Audio::get_streamStopped() const
{
	return Pa_IsStreamStopped(audioStream);
}

/*
 *	Méthode 'setLoop': modifie la "loop" pour un autre élément du type 'AudioData'
 *
 */
void Audio::setLoop(AudioData data)
{
	userData.setLoop(data);
}

/*
 *	Méthode 'playSound': vide l'intégralité des "sound" actuels
 *	pour n'en jouer qu'un seul (correspondant au paramètre 'data')
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
 *	Les méthodes suivantes servent à la gestion des "sound" et "loop"
 *	ainsi qu'à la gestion de leur volume.
 *
 *	Celles-ci utilisent principalement les méthode de la classe 'AudioData'
 *	(voir AudioData.h et AudioData.cpp pour plus de détails), elles seront
 *	donc peu ou non commentées
 *
 */

/*
 *	Ajoute un son à l'array 'sounds' du membre 'userData'
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
 *	Réinitialise le membre 'loop' de 'userData'
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
 *	Déconstructeur de la classe Audio: ferme le flux audio
 *
 */
Audio::~Audio()
{
	Pa_CloseStream(audioStream);
}