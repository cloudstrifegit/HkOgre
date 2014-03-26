#include "HkOgreWorld.h"
#include <QApplication>
#include "OgreWidget.h"
#include "HkOgreGraphic.h"
#include "HkOgreShapeDescription.h"
#include "HkOgreRigidBodyDescription.h"
#include "MyTerrain.h"
#include <XkDebugStr.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    HkOgre::World* pWorld = HkOgre::World::createWorld();
   
    OgreWidget* pWidget = new OgreWidget();
    pWidget->show();

    /*
    Ogre::SceneManager* pSceneMgr = HkOgre::Graphic::instance().getSceneManager();

    HkOgre::Graphic::instance().getSceneManager()->setAmbientLight(Ogre::ColourValue(0.8,0.8,0.8));
    Ogre::Light* pLight = HkOgre::Graphic::instance().getSceneManager()->createLight();
	pLight->setPosition(0, 2000, 120);

    MyTerrain* pTerrain = new MyTerrain();
    pTerrain->setup(pSceneMgr, pLight);
    Ogre::Vector3 vecPos = pTerrain->getTerrain()->getPosition();
    DBGSTRING("vecPos(%f, %f, %f)", vecPos.x, vecPos.y, vecPos.z);

    XkDebugString("pTerrain->getWorldSize() = %f", pTerrain->getTerrain()->getWorldSize());

    Ogre::Real fPos = pTerrain->getTerrain()->getWorldSize() / 2.0f;
    //pTerrain->getTerrain()->setPosition(Ogre::Vector3(fPos, 0, -fPos));

    HkOgre::RigidBodyDescription desc;
    desc.m_rotation = Ogre::Quaternion(sqrt(0.5), 0, sqrt(0.5), 0);
    desc.m_strEntity = "";
    desc.m_strName = "terrainhavok";
    desc.m_vecPos = Ogre::Vector3(-fPos, 0.0f, fPos);
    desc.m_friction = 0.5f;
    desc.m_nMotionType = HkOgre::RigidBodyDescription::MOTION_FIXED;

    HkOgre::SampledHeightFieldShapeDescription terrainshape(pTerrain->getTerrain());
    HkOgre::BvTreeShapeDescription treeShape(pTerrain->getTerrainGroup());

    HkOgre::World::getWorld()->createRigidBody(
        terrainshape,
        desc);
        */
    app.exec();

    return 0;
}
