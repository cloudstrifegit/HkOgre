#include "OgreWidget.h"
#include "OgreSystem.h"
#include "HkOgreWorld.h"
#include "HkOgreShapeDescription.h"
#include "HkOgreRigidBodyDescription.h"
#include "HkOgreUtil.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QBoxLayout>

#pragma execution_character_set("utf-8")

OgreWidget::OgreWidget(QWidget* parent) : m_nLastMouseX(-1), m_nLastMouseY(-1), m_bFirstPressed(false), m_bCtrlPressed(false),
	m_bResizing(false), m_fTime(0.0f)
{
    OgreSystem::instance().SetupResources();
    OgreSystem::instance().SetupRenderSystem();
    OgreSystem::instance().SetupSceneManager();

    //创建窗口
    Ogre::Root* pRoot = OgreSystem::instance().GetRoot();
    Ogre::SceneManager* pSceneMgr = OgreSystem::instance().GetSceneManager();

    Ogre::NameValuePairList miscParams;
    miscParams["externalWindowHandle"] = Ogre::StringConverter::toString((size_t) this->winId());
	miscParams["vsync"] = "false";
	miscParams["FSAA"] = "8";
	miscParams["colourDepth"] = "32";

    m_pWindow = pRoot->createRenderWindow(
		Ogre::StringConverter::toString((unsigned long)this->winId()), this->width(), this->height(), false, &miscParams);

    m_pCamera = pSceneMgr->createCamera(Ogre::StringConverter::toString((size_t) this->winId()));
    m_pCamera->setPosition(Ogre::Vector3(200, 200, 400));
	m_pCamera->lookAt(Ogre::Vector3(0, 0, 0));
	m_pCamera->setNearClipDistance(1);
	m_pVp = m_pWindow->addViewport(m_pCamera);
	m_pVp->setBackgroundColour(Ogre::ColourValue(0.5,0.5,0.5, 1));
	m_pCamera->setAspectRatio(Ogre::Real(m_pVp->getActualWidth()) /
		Ogre::Real(m_pVp->getActualHeight()));

    //创建地形
    pSceneMgr->setAmbientLight(Ogre::ColourValue(0.8,0.8,0.8));
    m_pLight = pSceneMgr->createLight();
    m_pLight->setType(Ogre::Light::LT_DIRECTIONAL);
    m_pLight->setDiffuseColour(Ogre::ColourValue::White);
    m_pLight->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));

    Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();
    m_pLight->setDirection(lightdir);
    
    m_pLightNode = pSceneMgr->getRootSceneNode()->createChildSceneNode();
    m_pLightNode->attachObject(m_pLight);
    
    MyTerrain* pTerrain = new MyTerrain();
    pTerrain->setup(pSceneMgr, m_pLight);
    Ogre::Real fPos = pTerrain->getTerrain()->getWorldSize() / 2.0f;

    HkOgre::RigidBodyDescription desc;
    desc.m_rotation = Ogre::Quaternion(sqrt(0.5), 0, sqrt(0.5), 0);
    desc.m_strEntity = "";
    desc.m_strName = "terrainhavok";
    desc.m_vecPos = Ogre::Vector3(-fPos, 0.0f, fPos);
    desc.m_friction = 0.5f;
    desc.m_nMotionType = HkOgre::RigidBodyDescription::MOTION_FIXED;

    HkOgre::SampledHeightFieldShapeDescription terrainshape(pTerrain->getTerrain());

    m_rbTerrain = HkOgre::World::getWorld()->createRigidBody(
        terrainshape,
        desc);

    Ogre::ResourceGroupManager::getSingleton().createResourceGroup("MyTerrain");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("..\\Terrain", "FileSystem", "MyTerrain");
    Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("MyTerrain");
        
	startTimer(2);
    m_timer.Restart();
}

OgreWidget::~OgreWidget()
{

}

void OgreWidget::updatePhysics()
{
    m_fTime += m_timer.elapsedSecond();

    Ogre::Real timeStep = 1.f/60.f;
    if(m_fTime >= timeStep) {

        HkOgre::World::getWorld()->simulate(timeStep);
        m_fTime -= timeStep;
        m_timer.Restart();
    }
}

void OgreWidget::shootBox()
{
    static int nCount = 0;
    
    HkOgre::SphereShapeDescription sphereShape(1.0f);
    HkOgre::RigidBodyDescription desc;
    desc.m_strEntity = "sphere_r1.mesh";
    desc.m_strName = "sphere" + Ogre::StringConverter::toString(nCount++);
    desc.m_vecPos = m_pCamera->getPosition();
    desc.m_mass = 1.0f;
    desc.m_linearVelocity = m_pCamera->getDirection() * 50;
    desc.m_strMaterial = "HavokSphereMat";

    HkOgre::World::getWorld()->createRigidBody(sphereShape, desc);
}

void OgreWidget::createStack()
{
    static int nCount = 0;
    
    Ogre::Vector3 startPos = m_pCamera->getPosition();
    HkOgre::RigidBodyDescription desc;
    desc.m_strEntity = "1x1x1box.mesh";
    
    HkOgre::BoxShapeDescription boxShape(Ogre::Vector3(1, 1, 1));

    for(int i=0; i<5; i++) {
        for(int j=0; j<5; j++) {
            Ogre::Vector3 pos(startPos.x + i*1, startPos.y + j*1, startPos.z - 30);
            desc.m_vecPos = pos;
            desc.m_strName = "stack"+Ogre::StringConverter::toString(nCount++);
            HkOgre::World::getWorld()->createRigidBody(boxShape, desc);
        }
    }
}

void OgreWidget::resizeEvent(QResizeEvent* evt)
{
	Q_UNUSED(evt);
    if(m_pWindow == NULL) return;

    m_pWindow->windowMovedOrResized();
    m_pCamera->setAspectRatio(
        Ogre::Real(m_pVp->getActualWidth()) /
        Ogre::Real(m_pVp->getActualHeight())
        );
}

void OgreWidget::timerEvent(QTimerEvent* evt)
{
	Q_UNUSED(evt);
    updatePhysics();
    OgreSystem::instance().UpdateBegin();
	OgreSystem::instance().UpdateEnd();
	m_pWindow->update();	
}

void OgreWidget::paintEvent(QPaintEvent* evt)
{
	Q_UNUSED(evt);
    updatePhysics();
	OgreSystem::instance().UpdateBegin();
	OgreSystem::instance().UpdateEnd();
	m_pWindow->update();	
}

void OgreWidget::keyPressEvent(QKeyEvent* evt)
{
    if(evt->key() == Qt::Key_F){
        Ogre::PolygonMode mode = m_pCamera->getPolygonMode();
        if(mode == Ogre::PM_WIREFRAME) {
            m_pCamera->setPolygonMode(Ogre::PM_SOLID);
        }else{
            m_pCamera->setPolygonMode(Ogre::PM_WIREFRAME);
        }
    }else if(evt->key() == Qt::Key_Control) {
        m_bCtrlPressed = true;
    }else if(evt->key() == Qt::Key_Space) {
        shootBox();
    }else if(evt->key() == Qt::Key_S) {
        createStack();
    }else if(evt->key() == Qt::Key_L) {
        Ogre::Vector3 vecPos = m_pCamera->getPosition();
        m_pLightNode->setPosition(vecPos);
        m_pLight->setDirection(m_pCamera->getDirection());
    }
}

void OgreWidget::keyReleaseEvent(QKeyEvent* evt)
{
    if(evt->key() == Qt::Key_Control){
        m_bCtrlPressed = false;
    }
}

void OgreWidget::mousePressEvent(QMouseEvent* evt)
{
    if(evt->buttons() & Qt::LeftButton ) {
        m_bFirstPressed = true;
    }
	if(evt->buttons() & Qt::RightButton) {
		m_bFirstPressed = true;
	}
}

void OgreWidget::mouseReleaseEvent(QMouseEvent* evt)
{
    if(evt->buttons() & Qt::LeftButton) {
        m_bFirstPressed = false;
    }
	if(evt->buttons() & Qt::RightButton) {
		m_bFirstPressed = false;
	}
}

void OgreWidget::mouseMoveEvent(QMouseEvent* evt)
{
    if(evt->buttons() & Qt::LeftButton) {
        int nRelX = evt->x() - m_nLastMouseX;
        int nRelY = evt->y() - m_nLastMouseY;
        if(!m_bFirstPressed) {
            Ogre::Radian rotX = Ogre::Degree(nRelX * 0.13);
            Ogre::Radian rotY = Ogre::Degree(nRelY * 0.13);
            if(m_bCtrlPressed) {
                m_pCamera->yaw(rotX);
                m_pCamera->pitch(rotY);
            }else{
                //m_pNode->yaw(rotX);
                //m_pNode->pitch(rotY);
            }
        }else{
            m_bFirstPressed = false;
        }
        m_nLastMouseX = evt->x();
        m_nLastMouseY = evt->y();   
    }

	if(evt->buttons() & Qt::RightButton) {
		int nRelX = evt->x() - m_nLastMouseX;
		int nRelY = evt->y() - m_nLastMouseY;

		if(!m_bFirstPressed) {
			Ogre::Vector3 transVec = Ogre::Vector3::ZERO;
			transVec.x -= nRelX * 0.05;
			transVec.y += nRelY * 0.05;
			m_pCamera->moveRelative(transVec);
		}else{
            m_bFirstPressed = false;
        }

		m_nLastMouseX = evt->x();
        m_nLastMouseY = evt->y();
	}
}
	
void OgreWidget::wheelEvent(QWheelEvent* evt)
{
    Ogre::Vector3 dir = m_pCamera->getDirection();
    dir.normalise();
    Ogre::Vector3 pos = m_pCamera->getPosition();
    Ogre::Vector3 newpos = pos + (evt->delta()/10.0f)*dir;
    m_pCamera->setPosition(newpos);
}