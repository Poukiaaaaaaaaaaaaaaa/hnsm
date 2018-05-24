#include "Audio.h"

static int streamCallback(const void *input, void *output, unsigned long frameCount,
	const PaStreamCallbackTimeInfo *tInfo, PaStreamCallbackFlags statusFlags,
	void *userData)
{
	AudioCallbackData *data = (AudioCallbackData*)userData;
	float *out = (float*)output;
	sf_count_t loop_item_count;
	sf_count_t sound_item_count;

	//unused parameters
	(void)tInfo;
	(void)statusFlags;
	(void)input;

	//temporary buffer holding data to be sent to the output buffer
	float *temp = (float*)malloc(sizeof(float) * frameCount * CHANNELS);

	//clearing the output buffer
	memset(out, 0, sizeof(float) * frameCount * CHANNELS);

	//process loop
	if (data->loop.exists == true)
	{
		if (data->loop.reloaded == true)
			loop_item_count = sf_read_float(data->loop.audioFile, temp, frameCount *
				data->loop.info.channels);
		else loop_item_count = frameCount;

		for (unsigned i = 0; i < frameCount * CHANNELS; i++)
		{
			out[i] += temp[i] * data->getLoopVol();
		}

		if (loop_item_count < frameCount)
		{
			data->loop.reload();
		}
	}

	//process sounds
	for (unsigned i = 0; i < MAX_SOUNDS; i++)
	{
		if (data->sounds[i].exists == true)
		{
			if (data->sounds[i].reloaded == true)
				//false if the audio file is being reloaded
				sound_item_count = sf_read_float(data->sounds[i].audioFile, temp, frameCount *
					data->sounds[i].info.channels);
			else sound_item_count = frameCount;
			
			for (unsigned i = 0; i < frameCount * CHANNELS; i++)
			{
				out[i] += temp[i] * data->getSoundsVol();
			}

			if (sound_item_count < frameCount)
			{
				for (unsigned j = i; j < MAX_SOUNDS - 1; j++)
				{
					data->sounds[j] = data->sounds[j + 1];
				}

				data->sounds[MAX_SOUNDS - 1] = AudioData();
			}
		}
	}

	free(temp);

	return paContinue;
}

Audio::Audio()
{
	PaDeviceIndex index = Pa_GetDefaultOutputDevice();
	const PaDeviceInfo *info = Pa_GetDeviceInfo(index);

	if (info == nullptr)
	{
		Log::toFile("error.log", "[ERROR]:Couldn't retrieve device information");
	}
	else
	{
		Pa_OpenDefaultStream(&audioStream, 0, 2, paFloat32, info->defaultSampleRate,
			paFramesPerBufferUnspecified, streamCallback, &userData
		);
	}
}

void Audio::startStream()
{
	Pa_StartStream(audioStream);
}

void Audio::stopStream(StopTag tag)
{
	if (tag == STOP)
		Pa_StopStream(audioStream);
	else if (tag == ABORT)
		Pa_AbortStream(audioStream);
}

PaError Audio::get_streamStopped() const
{
	return Pa_IsStreamStopped(audioStream);
}

void Audio::setLoop(AudioData data)
{
	userData.setLoop(data);
}

void Audio::playSound(AudioData data)
{
	for (unsigned i = 0; i < MAX_SOUNDS; i++)
	{
		userData.sounds[i] = AudioData();
	}

	userData.setSound(0, data);
}

void Audio::addSound(AudioData data)
{
	userData.addSound(data);
}

void Audio::clearSounds()
{
	userData.clearSounds();
}

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

Audio::~Audio()
{
	Pa_CloseStream(audioStream);
}