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
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));

	// Create some more lights here
	Ogre::Light* directionalLight = mSceneMgr->createLight("DirectionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);	
	directionalLight->setDirection(Ogre::Vector3(0.2f, -0.5f, -0.234f));
	directionalLight->setDiffuseColour(Ogre::ColourValue(.4, 0.2, 0));
	directionalLight->setSpecularColour(Ogre::ColourValue(.4, 0.2, 0));

	Ogre::Light* spotLight = mSceneMgr->createLight("SpotLight");
	spotLight->setDiffuseColour(0, 0, .92);
	spotLight->setSpecularColour(0, 0, .83);
	spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
	spotLight->setDirection(-0, -1, 0);
	spotLight->setPosition(Ogre::Vector3(0, 23, -5));
	spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));
	

	// Add the tank body
	Ogre::Entity* ogreEntity = mSceneMgr->createEntity("simple-tank-body.mesh");
	Ogre::SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("tank-body");
	ogreNode->setScale(1.3f, 1.3f, 1.3f); 
	ogreNode->setPosition(0, 0.5f, -5);
	ogreNode->attachObject(ogreEntity);

	// Add the tank turret
	Ogre::Entity* tankTurret = mSceneMgr->createEntity("simple-tank-turret.mesh");
	tankTurret->setCastShadows(true);
	Ogre::SceneNode* turretNode = mSceneMgr->getSceneNode("tank-body")->createChildSceneNode("tank-turret");
	turretNode->setScale(1.3f, 1.3f, 1.3f); 
	turretNode->attachObject(tankTurret);

	// Add the platform
	Ogre::Entity* platformEntity= mSceneMgr->createEntity("simple-platform.mesh");
	platformEntity->setCastShadows(false);
	Ogre::SceneNode* platformNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("platform");
	platformNode->setScale(2.3f, 2.3f, 2.3f); 
	platformNode->setPosition(0, -00, -20);
	platformNode->attachObject(platformEntity);

	//Ogre::Light* light = mSceneMgr->createLight("MainLight");
	//light->setPosition(20, 80, 50);	

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
	mSceneMgr->getSceneNode("tank-body")->yaw(Ogre::Degree(rotY));
	mSceneMgr->getSceneNode("tank-turret")->yaw(Ogre::Degree(-rotY*2));
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
