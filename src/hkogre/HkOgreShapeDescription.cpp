#include "HkOgreShapeDescription.h"
#include <Common/Base/hkBase.h>
#include <Physics2012/Collide/Shape/Convex/Box/hkpBoxShape.h>					
#include <Physics2012/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics2012/Collide/Shape/Convex/hkpConvexShape.h>
#include <Physics2012/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>
#include <XkDebugStr.h>
#include "HkOgreUtil.h"

namespace HkOgre
{

ShapeDescription::ShapeDescription()
{
}

ShapeDescription::~ShapeDescription()
{
}

hkpConvexShape* ShapeDescription::createShape()  const
{
    XkDebugString("ShapeDescription::createShape");
    return 0;
}

Ogre::ManualObject* ShapeDescription::createDebugEntity(const Ogre::String& strName) const
{
    XkDebugString("ShapeDescription::createDebugEntity");
    return 0;
}

hkMassProperties ShapeDescription::calcMassProperties(float fMass) const
{
    XkDebugString("ShapeDescription::calcMassProperties");
    return hkMassProperties();
}

BoxShapeDescription::BoxShapeDescription(const Ogre::Vector3& vecSize)
{
    m_vecSize = vecSize;
}

hkpConvexShape* BoxShapeDescription::createShape() const
{
    hkVector4 halfSize = tohkVector4(m_vecSize);

    return new hkpBoxShape(halfSize);
}

Ogre::ManualObject* BoxShapeDescription::createDebugEntity(const Ogre::String& strName) const
{
    return createDebugBox(strName, m_vecSize.x, m_vecSize.y, m_vecSize.z);
}

hkMassProperties BoxShapeDescription::calcMassProperties(float fMass) const
{
    hkMassProperties massProp;
    hkVector4 halfSize = tohkVector4(m_vecSize);
    hkpInertiaTensorComputer::computeBoxVolumeMassProperties(
            halfSize, fMass, massProp);

    return massProp;
}

BoxShapeDescription::~BoxShapeDescription()
{
}

SphereShapeDescription::SphereShapeDescription(Ogre::Real radius)
{
    m_radius = radius;
}

SphereShapeDescription::~SphereShapeDescription()
{
}

hkpConvexShape* SphereShapeDescription::createShape() const
{
    return new hkpSphereShape(m_radius);
}

Ogre::ManualObject* SphereShapeDescription::createDebugEntity(const Ogre::String& strName) const
{
    return createDebugSphere(strName, m_radius);
}

hkMassProperties SphereShapeDescription::calcMassProperties(float fMass) const
{
    hkMassProperties massProp;
    hkpInertiaTensorComputer::computeSphereVolumeMassProperties(
           m_radius, fMass, massProp);

    return massProp;
}

}//namespace HkOgre