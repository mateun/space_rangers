#ifndef SOUND_H
#define SOUND_H

#include <stdio.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <audio/wave.h>
#include <string>

class SoundDevice {

public:
	SoundDevice();
	~SoundDevice();

private:
	ALCdevice* _soundDevice;
	ALCcontext* _soundContext;

};

// The sound itself
class SoundCue {
	
public:
	SoundCue(std::string wavName);
	~SoundCue();

	ALuint GetBuffer();

private:
	ALuint _buffer;

};

// A sound entity which 
// has a specific position
class SoundSource {

public:
	SoundSource(float x, float y, float z, SoundCue& cue);
	~SoundSource();
	void Play();

private:
	ALuint _source;
};

#endif
