#pragma once

#include <iostream>
#include <vector>

#include "AudioData.h"

#define MAX_SOUNDS 32
#define CHANNELS 2
#define MAX_VOLUME 2.0f
#define MIN_VOLUME 0.0f

class AudioCallbackData
{
	float loop_vol;
	float sounds_vol;

public:
	AudioData loop;

	AudioData sounds[MAX_SOUNDS];

	AudioCallbackData();
	~AudioCallbackData();

	void setLoop(AudioData);
	void setSound(unsigned index, AudioData data);
	void addSound(AudioData data);

	void setLoopVol(float vol);
	void setSoundsVol(float vol);

	void clearSounds();
	void clearLoop();

	float getLoopVol() const;
	float getSoundsVol() const;
};