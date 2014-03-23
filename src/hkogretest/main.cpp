#include "HkOgreWorld.h"
#include <QApplication>
#include "OgreWidget.h"
#include "HkOgreGraphic.h"
#include "HkOgreShapeDescription.h"
#include "HkOgreRigidBodyDescription.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    HkOgre::World* pWorld = HkOgre::World::createWorld();
    HkOgre::Graphic::instance();

    OgreWidget* pWidget = new OgreWidget();
    pWidget->show();

    HkOgre::BoxShapeDescription boxShape(Ogre::Vector3(5, 5, 5));
    HkOgre::RigidBodyDescription desc;
    desc.m_strEntity = "";
    desc.m_strName = "ogrehead";
    desc.m_vecPos = Ogre::Vector3(0, 100.0f, 0);
    desc.m_mass = 1.0f;
    desc.m_friction = 0.5f;

    pWorld->createRigidBody(boxShape, desc);

    desc.m_strName = "ground";
    desc.m_vecPos = Ogre::Vector3(0, 0, 0);
    desc.m_nMotionType = HkOgre::RigidBodyDescription::MOTION_FIXED;
    HkOgre::BoxShapeDescription groundShape(Ogre::Vector3(100, 2, 100));
    pWorld->createRigidBody(groundShape, desc);

    HkOgre::Graphic::instance().getSceneManager()->setAmbientLight(Ogre::ColourValue(0,0,0));
    Ogre::Light* pLight = HkOgre::Graphic::instance().getSceneManager()->createLight();
	pLight->setPosition(0, 80, 120);

    app.exec();

    HkOgre::World::destroyWorld();
    HkOgre::Graphic::destroy();

    return 0;
}
