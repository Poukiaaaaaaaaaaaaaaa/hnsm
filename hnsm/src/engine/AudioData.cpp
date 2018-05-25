#include "AudioData.h"

AudioData::AudioData()
	: exists(false)
{

}

AudioData::AudioData(const char* filename)
	: exists(true), audioPath(filename)
{
	info.format = NULL;

	audioFile = sf_open(filename, SFM_READ, &info);
	if (audioFile == nullptr)
	{
		Log::toSfError("error.log", "[INFO]:Sound File error status: ", audioFile, __FILE__, __LINE__);
	}
}

AudioData::~AudioData()
{
}

void AudioData::reload()
{
	reloaded = false;
	audioFile = sf_open(audioPath, SFM_READ, &info);
	reloaded = true;
}
