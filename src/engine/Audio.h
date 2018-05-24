#pragma once

#include <iostream>

//PortAudio headers
#include <PortAudio\portaudio.h>

#include "AudioData.h"
#include "AudioCallbackData.h"

enum StopTag
{
	STOP, ABORT
};

class Audio
{
public:
	AudioCallbackData userData;
	PaStream *audioStream;
	bool loop;

public:
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