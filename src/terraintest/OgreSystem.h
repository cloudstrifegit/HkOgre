#ifndef OGRE_SYSTEM_H
#define OGRE_SYSTEM_H

#include <Ogre.h>

class OgreSystem
{
public:
	OgreSystem();
	~OgreSystem();

    static OgreSystem& instance();
	static void Destory();

	bool SetupResources();
	bool SetupRenderSystem();
	bool SetupSceneManager();

	Ogre::SceneManager* GetSceneManager();
	Ogre::Root* GetRoot();

	void UpdateBegin();
	void UpdateEnd();

private:
	Ogre::Root*	m_pRoot;
	Ogre::RenderSystem* m_pRenderSystem;
	Ogre::SceneManager* m_pSceneMgr;

    static OgreSystem* m_pInstance;
};

#endif