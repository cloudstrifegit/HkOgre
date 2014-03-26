#include "OgreSystem.h"
#include "XkDebugStr.h"

OgreSystem* OgreSystem::m_pInstance = NULL;

OgreSystem::OgreSystem() :
	m_pRoot(NULL), m_pRenderSystem(NULL), m_pSceneMgr(NULL)
{
	Ogre::String strPluginPath;

	strPluginPath = "plugins.cfg";

	m_pRoot = OGRE_NEW Ogre::Root(strPluginPath,
		"ogre.cfg", "Ogre.log");
}

OgreSystem::~OgreSystem()
{
	OGRE_DELETE m_pRoot;
}

OgreSystem& OgreSystem::instance()
{
    if(m_pInstance == 0) {
        m_pInstance = new OgreSystem();
    }
    return *m_pInstance;
}

bool OgreSystem::SetupResources()
{
	Ogre::ConfigFile cf;
	cf.load("resources.cfg");

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String strSecName, strTypeName, strArchName;
	while(seci.hasMoreElements()) {
		strSecName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for(i = settings->begin(); i != settings->end(); ++i) {
			strTypeName = i->first;
			strArchName = i->second;

			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				strArchName, strTypeName, strSecName);
		}
	}

	return true;
}

bool OgreSystem::SetupRenderSystem()
{
	m_pRoot->showConfigDialog();

	//不创建窗口
	m_pRoot->initialise(false);

	return true;
}

bool OgreSystem::SetupSceneManager()
{

	m_pSceneMgr = m_pRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	
	return true;
}

Ogre::SceneManager* OgreSystem::GetSceneManager()
{
	return m_pSceneMgr;
}

Ogre::Root* OgreSystem::GetRoot()
{
    return m_pRoot;
}

void OgreSystem::UpdateBegin()
{
	m_pRoot->_fireFrameStarted();
}

void OgreSystem::UpdateEnd()
{
	m_pRoot->_fireFrameEnded();
}