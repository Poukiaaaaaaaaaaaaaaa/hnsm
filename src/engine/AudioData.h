#pragma once

#include <iostream>
#include <vector>

//libsndfile headers
#include <libsndfile\sndfile.h>

#include "Log.h"

class AudioData
{
public:
	SF_INFO info;
	SNDFILE *audioFile;
	const char *audioPath;
	bool exists;

	AudioData();
	AudioData(const char* filename);
	void reload();
	bool reloaded = false;
	~AudioData();
};
