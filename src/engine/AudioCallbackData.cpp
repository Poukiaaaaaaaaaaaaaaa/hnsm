#include "AudioCallbackData.h"

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

void AudioCallbackData::setLoop(AudioData data)
{
	loop.audioPath = data.audioPath;
	loop.exists = true;
	loop.reload();
}

void AudioCallbackData::setSound(unsigned index, AudioData data)
{
	if (index < MAX_SOUNDS)
	{
		sounds[index].audioPath = data.audioPath;
		sounds[index].exists = true;
		sounds[index].reload();
	}
}

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

void AudioCallbackData::clearSounds()
{
	for (unsigned i = 0; i < MAX_SOUNDS; i++)
	{
		sounds[i] = AudioData();
	}
}

void AudioCallbackData::clearLoop()
{
	loop = AudioData();
}

void AudioCallbackData::setLoopVol(float vol)
{
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

float AudioCallbackData::getLoopVol() const
{
	return loop_vol;
}

float AudioCallbackData::getSoundsVol() const
{
	return sounds_vol;
}