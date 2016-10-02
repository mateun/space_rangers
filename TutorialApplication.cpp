/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.10)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#include "TutorialApplication.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "audio/wave.h"

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

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


//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
    // Create your scene here :)
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Entity* ogreEntity = mSceneMgr->createEntity("tank-machine.mesh");
	Ogre::SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("cube");
	ogreNode->setScale(0.3f, 0.3f, 0.3f); 
	ogreNode->setPosition(0, 0, -30);
	ogreNode->attachObject(ogreEntity);
	Ogre::Light* light = mSceneMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);	

	// sound stuff
	_soundDevice = alcOpenDevice(NULL); // select the "preferred device" 
	if (_soundDevice) { 
		_soundContext=alcCreateContext(_soundDevice,NULL);      
		alcMakeContextCurrent(_soundContext);            
	} 	


	ALuint source;


	alGenSources((ALuint)1, &source);
	// check for errors

	alSourcef(source, AL_PITCH, 1);
	// check for errors
	alSourcef(source, AL_GAIN, 1);
	// check for errors
	alSource3f(source, AL_POSITION, 0, 0, 0);
	// check for errors
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	// check for errors
	alSourcei(source, AL_LOOPING, AL_FALSE);
	// check for errros

	ALuint buffer;

	alGenBuffers((ALuint)1, &buffer);
	// check for errors

	WaveInfo *wave;
	char *bufferData;
	int ret;
	//wave = WaveOpenFileForReading("../media/sounds/splat1a.wav");
	wave = WaveOpenFileForReading("/usr/lib/libreoffice/share/gallery/sounds/space3.wav");

	

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

	alBufferData(buffer, to_al_format(wave->channels, wave->bitsPerSample),
                bufferData, wave->dataSize, wave->sampleRate);

	alSourcei(source, AL_BUFFER, buffer);
	// check for errors	

	alSourcePlay(source);
	
}
void TutorialApplication::destroyScene() {
	std::cout << "in destroyScene()" << std::endl;
	//device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(_soundContext);
	alcCloseDevice(_soundDevice);
}


bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& fe) {
	rotY = 0.01f;
	mSceneMgr->getSceneNode("cube")->yaw(Ogre::Degree(rotY));
	BaseApplication::frameRenderingQueued(fe);	
}
//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch(Ogre::Exception& e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
