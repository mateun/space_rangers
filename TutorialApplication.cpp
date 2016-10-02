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
#include "Sound.h"
#include <string>

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
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
	SoundDevice* device = new SoundDevice();
	SoundCue* startMusic = new SoundCue(std::string("../media/sounds/the_other.wav"));
	SoundSource* musicSource = new SoundSource(0, 0, 0, *startMusic);
	musicSource->Play();
	
	
	
}
void TutorialApplication::destroyScene() {
	std::cout << "in destroyScene()" << std::endl;
	//device = alcGetContextsDevice(context);
	//alcMakeContextCurrent(NULL);
	//alcDestroyContext(_soundContext);
	//alcCloseDevice(_soundDevice);
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
