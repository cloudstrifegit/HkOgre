#include "HkOgreRigidBody.h"
#include <Common/Base/hkBase.h>
#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>
#include <Physics2012/Collide/Shape/Convex/hkpConvexShape.h>
#include <Physics2012/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>
#include "HkOgreWorld.h"
#include "HkOgreShapeDescription.h"
#include "HkOgreRigidBodyDescription.h"
#include "HkOgreUtil.h"

namespace HkOgre
{

RigidBody::RigidBody(World* world) :
    m_world(world), m_pEntity(0), m_pNode(0)
{

}

RigidBody::~RigidBody()
{

}

void RigidBody::advance(float fTime)
{
    Ogre::Vector3 vecPos = toVector3(m_rb->getPosition());
    Ogre::Quaternion rot = toQuaternion(m_rb->getRotation());

    m_pNode->setPosition(vecPos);
    m_pNode->setOrientation(rot); 
}

void RigidBody::createRigidBody(const ShapeDescription& shape,
        const RigidBodyDescription& desc)
{
    hkpShape* pShape = shape.createShape();
    hkpRigidBodyCinfo ci;
    desc.to_hkCinfo(&ci);

    hkMassProperties massProp = shape.calcMassProperties(desc.m_mass);
    ci.m_mass = massProp.m_mass;
    ci.m_centerOfMass = massProp.m_centerOfMass;
    ci.m_solverDeactivation = ci.SOLVER_DEACTIVATION_MEDIUM;
    ci.m_inertiaTensor = massProp.m_inertiaTensor;

    ci.m_shape = pShape;

    m_rb = new hkpRigidBody(ci);
    pShape->removeReference();

    hkpWorld* pWorld = m_world->gethkpWorld();
    pWorld->markForWrite();
    pWorld->addEntity(m_rb);
    pWorld->unmarkForWrite();

    Ogre::SceneManager* pSceneMgr = ::Ogre::Root::getSingletonPtr()->getSceneManagerIterator().getNext();

    //创建场景节点等
    if(!desc.m_strEntity.empty()) {
        m_pEntity = pSceneMgr->createEntity(desc.m_strName, desc.m_strEntity);
    }

    if(desc.m_strName.empty()) {
        m_pNode = pSceneMgr->getRootSceneNode()->createChildSceneNode();
    }else{
        m_pNode = pSceneMgr->getRootSceneNode()->createChildSceneNode(desc.m_strName);
    }

    if(m_pEntity)
        m_pNode->attachObject(m_pEntity);

#if HKOGRE_DEBUG_RIGIDBODY == 1
    m_pDebugEntity = shape.createDebugEntity(desc.m_strName + "debug");
    if(m_pDebugEntity)
        m_pNode->attachObject(m_pDebugEntity);
#endif
}

}//namespace HkOgre