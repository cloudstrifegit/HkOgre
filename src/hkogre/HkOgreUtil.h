#ifndef XKOGRE_UTIL_H
#define XKOGRE_UTIL_H

#include <Ogre.h>
#include "HkOgreHavokPrototypes.h"

namespace HkOgre
{

HkOgrePublicFunction Ogre::Vector3 toVector3(const hkVector4f& vec);
HkOgrePublicFunction hkVector4f tohkVector4(const Ogre::Vector3& vec);
HkOgrePublicFunction hkQuaternionf tohkQuaternion(const Ogre::Quaternion& q);
HkOgrePublicFunction Ogre::Quaternion toQuaternion(const hkQuaternionf& q);

HkOgrePublicFunction Ogre::ManualObject* createDebugBox(const Ogre::String& strName,
    Ogre::Real x, Ogre::Real y, Ogre::Real z);

HkOgrePublicFunction Ogre::ManualObject* createDebugSphere(const Ogre::String& strName,
    Ogre::Real radius, int nRings = 12, int nSeg = 12);

HkOgrePublicFunction Ogre::ManualObject* createDebugCapsule(const Ogre::String& strName,
    const Ogre::Vector3& vecA,
    const Ogre::Vector3& vecB,
    Ogre::Real radius,
    int nRings = 12, int nSeg = 12);

HkOgrePublicFunction Ogre::ManualObject* createDebugTriangle(const Ogre::String& strName,
    const Ogre::Vector3& v0,
    const Ogre::Vector3& v1,
    const Ogre::Vector3& v2);

}//namespace HkOgre

#endif