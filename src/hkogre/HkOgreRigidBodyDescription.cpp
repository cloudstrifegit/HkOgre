#include "HkOgreRigidBodyDescription.h"
#include <Common/Base/hkBase.h>
#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>
#include "HkOgreUtil.h"

namespace HkOgre
{

RigidBodyDescription::RigidBodyDescription()
{
    m_collisionFilterInfo = 0;
    m_vecPos = Ogre::Vector3::ZERO;
    m_linearVelocity = Ogre::Vector3::ZERO;
    m_angularVelocity = Ogre::Vector3::ZERO;
    m_mass = 0.1f;
    m_friction = 0.4f;
    m_restitution = 0.0f;
    m_nMotionType = MOTION_DYNAMIC;
}

void RigidBodyDescription::to_hkCinfo(hkpRigidBodyCinfo* info) const
{
    info->m_collisionFilterInfo = m_collisionFilterInfo;
    info->m_position = tohkVector4(m_vecPos);
    info->m_rotation = tohkQuaternion(m_rotation);
    info->m_linearVelocity = tohkVector4(m_linearVelocity);
    info->m_angularVelocity = tohkVector4(m_angularVelocity);
    info->m_mass = m_mass;
    info->m_friction = m_friction;
    info->m_motionType = hkpMotion::MotionType(m_nMotionType);
}

}//namespace HkOgre