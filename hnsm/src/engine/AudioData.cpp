#include "AudioData.h"

/*
 *	Constructeur par d�faut: construit un objet n'�tant
 *	li� � aucun fichier audio.
 *	Ne pouvant ainsi pas �tre lu, son membre 'exists' doit �tre
 *	�gal � `false`
 *
 */
AudioData::AudioData()
	: exists(false)
{

}

/*
 *	Constructeur: ouvre et charge un fichier audio � partir du
 *	chemin pass� en param�tre ('filename')
 *
 */
AudioData::AudioData(const char* filename)
	: exists(true), audioPath(filename)
{
	info.format = NULL;

	/*
	 *	Fonction 'sf_open': ouvre le fichier audio sp�cifi� par 'filename'
	 *	puis place les informations lui correspondant dans 'info'
	 *
	 *	`SFM_READ`: ouvre le fichier audio en mode "lecture"
	 *	(pourrait �galement �tre ouvert en mode "�criture" avec `SFM_WRITE`)
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
 *	M�thode 'reload': recharge le fichier audio.
 *	La valeur `false` est affect�e au membre 'reloaded' pendant la recharge
 *	pour emp�cher le callback audio d'acc�der aux donn�es
 *
 */
void AudioData::reload()
{
	reloaded = false;
	audioFile = sf_open(audioPath, SFM_READ, &info);
	reloaded = true;
}
