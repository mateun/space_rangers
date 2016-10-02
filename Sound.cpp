#include "Sound.h"
#include <iostream>
#include <stdlib.h>

static inline ALenum to_al_format(short channels, short samples)
{
        bool stereo = (channels > 1);

        switch (samples) {
        case 16:
                if (stereo)
                        return AL_FORMAT_STEREO16;
                else
                        return AL_FORMAT_MONO16;
        case 8:
                if (stereo)
                        return AL_FORMAT_STEREO8;
                else
                        return AL_FORMAT_MONO8;
        default:
                return -1;
        }
}



ALuint SoundCue::GetBuffer() {
	return _buffer;
}

SoundCue::SoundCue(std::string wavName) {

	alGenBuffers((ALuint)1, &_buffer);
	ALenum err = alGetError();
	if (err != AL_NO_ERROR) {
		std::cout << "error creating sound buffer" << std::endl;
	}	

	WaveInfo *wave;
	char *bufferData;
	int ret;
	wave = WaveOpenFileForReading(wavName.c_str());

	if (!wave) {
		std::cout << "failed to read wave file" << std::endl;
		exit(1);
	}

	

	ret = WaveSeekFile(0, wave);
	if (ret) {
		std::cout << "failed to seek wave file" << std::endl;
		exit(1);
	}

	bufferData = (char*)malloc(wave->dataSize);
	if (!bufferData) {
		std::cout << "malloc error sound" << std::endl;
		exit(1);
	}

	ret = WaveReadFile(bufferData, wave->dataSize, wave);
	if (ret != wave->dataSize) {
		std::cout << "short read: " << ret << "want: " << wave->dataSize << std::endl;
		exit(1);
	}

	alBufferData(_buffer, to_al_format(wave->channels, wave->bitsPerSample),
                bufferData, wave->dataSize, wave->sampleRate);
}


SoundSource::SoundSource(float x, float y, float z, SoundCue& cue) {
	alGenSources((ALuint)1, &_source);
	// check for errors

	alSourcef(_source, AL_PITCH, 1);
	// check for errors
	alSourcef(_source, AL_GAIN, 1);
	// check for errors
	alSource3f(_source, AL_POSITION, x, y, z);
	// check for errors
	alSource3f(_source, AL_VELOCITY, 0, 0, 0);
	// check for errors
	alSourcei(_source, AL_LOOPING, AL_FALSE);
	// check for errros

	
	alSourcei(_source, AL_BUFFER, cue.GetBuffer());


}

void SoundSource::Play() {
	alSourcePlay(_source);
}


SoundDevice::SoundDevice() {

	_soundDevice = alcOpenDevice(NULL); // select the "preferred device" 
	if (_soundDevice) { 
		_soundContext=alcCreateContext(_soundDevice,NULL);      
		alcMakeContextCurrent(_soundContext);            
	} 	

	std::cout << "SoundDevice created" << std::endl;

	alGetError();


}



