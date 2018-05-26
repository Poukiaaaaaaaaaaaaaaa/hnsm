#include "AudioData.h"

/*
 *	Constructeur par défaut: construit un objet n'étant
 *	lié à aucun fichier audio.
 *	Ne pouvant ainsi pas être lu, son membre 'exists' doit être
 *	égal à `false`
 *
 */
AudioData::AudioData()
	: exists(false)
{

}

/*
 *	Constructeur: ouvre et charge un fichier audio à partir du
 *	chemin passé en paramètre ('filename')
 *
 */
AudioData::AudioData(const char* filename)
	: exists(true), audioPath(filename)
{
	info.format = NULL;

	/*
	 *	Fonction 'sf_open': ouvre le fichier audio spécifié par 'filename'
	 *	puis place les informations lui correspondant dans 'info'
	 *
	 *	`SFM_READ`: ouvre le fichier audio en mode "lecture"
	 *	(pourrait également être ouvert en mode "écriture" avec `SFM_WRITE`)
	 *
	 */
	audioFile = sf_open(filename, SFM_READ, &info);
	if (audioFile == nullptr)
	{
		Log::toSfError("error.log", "[INFO]:Sound File error status: ", audioFile, __FILE__, __LINE__);
	}
}

AudioData::~AudioData()
{
}

/*
 *	Méthode 'reload': recharge le fichier audio.
 *	La valeur `false` est affectée au membre 'reloaded' pendant la recharge
 *	pour empêcher le callback audio d'accéder aux données
 *
 */
void AudioData::reload()
{
	reloaded = false;
	audioFile = sf_open(audioPath, SFM_READ, &info);
	reloaded = true;
}
