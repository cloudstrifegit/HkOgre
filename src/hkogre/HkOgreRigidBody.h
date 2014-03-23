#ifndef HKOGRE_RIGIDBODY_H
#define HKOGRE_RIGIDBODY_H

#include "HkOgreConfig.h"
#include "HkOgreHavokPrototypes.h"
#include <Ogre.h>
#include "HkOgreTimeListener.h"

namespace HkOgre
{
class World;
class RigidBodyDescription;
class ShapeDescription;

class HkOgrePublicClass RigidBody : public TimeListener
{
public:
    RigidBody(World* world);
    ~RigidBody();

    virtual void advance(float fTime);

    void createRigidBody(const ShapeDescription& shape,
        const RigidBodyDescription& desc);

private:
    hkpRigidBody* m_rb;

    World* m_world;

    Ogre::SceneNode* m_pNode;
    Ogre::Entity* m_pEntity;

#if HKOGRE_DEBUG_RIGIDBODY == 1
    Ogre::ManualObject* m_pDebugEntity;
#endif
};


}//namespace HkOgre

#endif