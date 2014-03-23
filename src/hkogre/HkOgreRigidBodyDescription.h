#ifndef HKOGRE_RIGIDBODYDESCRIPTION_H
#define HKOGRE_RIGIDBODYDESCRIPTION_H

#include <Ogre.h>
#include "HkOgreHavokPrototypes.h"

namespace HkOgre
{

class HkOgrePublicClass RigidBodyDescription
{
public:
    enum MONTION_TYPE_ENUM{
        MOTION_INVALID,
        MOTION_DYNAMIC,
        MOTION_SPHERE_INERTIA,
        MOTION_BOX_INERTIA,
        MOTION_KEYFRAMED,
        MOTION_FIXED,
        MOTION_THIN_BOX_INERTIA,
        MOTION_CHARACTER,
        MOTION_MAX_ID
    };

    RigidBodyDescription();
    void to_hkCinfo(hkpRigidBodyCinfo* info) const;

public:
    Ogre::String m_strName;
    Ogre::String m_strEntity;
    Ogre::Vector3 m_vecPos;
    Ogre::Quaternion m_rotation;
    Ogre::Vector3 m_linearVelocity;
    Ogre::Vector3 m_angularVelocity;
    Ogre::Real m_mass;
    Ogre::Real m_friction;
    Ogre::Real m_restitution;
    Ogre::ulong m_collisionFilterInfo;
    MONTION_TYPE_ENUM m_nMotionType;
};

}//namespace HkOgre

#endif