#ifndef HKOGRE_GRAPHIC_H
#define HKOGRE_GRAPHIC_H

#include <Ogre.h>
#include "HkOgreConfig.h"

namespace HkOgre
{

class HkOgrePublicClass Graphic
{
public:
    Graphic();
    ~Graphic();

    static Graphic& instance();
    static void destroy();

public:
    bool setupResources();
    bool setupRenderSystem();
    bool setupSceneManager();

    Ogre::SceneManager* getSceneManager()
    { return m_pSceneMgr;}

    Ogre::Root* getRoot()
    { return m_pRoot; }

    void updateBegin()
    { m_pRoot->_fireFrameStarted();}

    void updateEnd()
    { m_pRoot->_fireFrameEnded();}

public:
    static Ogre::ManualObject* createDebugBox(
        const Ogre::String& strName,
        const Ogre::String& strMaterial,
        Ogre::Real x, Ogre::Real y, Ogre::Real z);

    static Ogre::ManualObject* createDebugSphere(
        const Ogre::String& strName,
        const Ogre::String& strMaterial,
        Ogre::Real radius, int nRings = 6, int nSeg = 6);

private:
    Ogre::Root* m_pRoot;
    Ogre::RenderSystem* m_pRenderSystem;
    Ogre::SceneManager* m_pSceneMgr;

    static Graphic* m_pInstance;
};

}//namespace HkOgre

#define gGraphic HkOgre::Graphic::instance

#endif