#include "HkOgreGraphic.h"
#include <XkDebugStr.h>

namespace HkOgre
{

Graphic* Graphic::m_pInstance = 0;

Graphic::Graphic() :
	m_pRoot(NULL), m_pRenderSystem(NULL), m_pSceneMgr(NULL)
{
	Ogre::String strPluginPath;

	strPluginPath = "plugins.cfg";

	m_pRoot = OGRE_NEW Ogre::Root(strPluginPath,
		"ogre.cfg", "Ogre.log");

    setupResources();
    setupRenderSystem();
    setupSceneManager();
}

Graphic::~Graphic()
{
    OGRE_DELETE m_pRoot;
}

Graphic& Graphic::instance()
{
    if(m_pInstance==0)
        m_pInstance = new Graphic();

    return *m_pInstance;
}

void Graphic::destroy()
{
    if(m_pInstance)
        delete m_pInstance;
}

bool Graphic::setupResources()
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

bool Graphic::setupRenderSystem()
{
	m_pRoot->showConfigDialog();

	//不创建窗口
	m_pRoot->initialise(false);

	return true;
}

bool Graphic::setupSceneManager()
{
    m_pSceneMgr = m_pRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
    XkDebugString("m_pSceneMgr: 0x%X", m_pSceneMgr);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	return true;
}

}//namespace HkOgre