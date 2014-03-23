#ifndef HKOGRE_SHAPEDESCRIPTION_H
#define HKOGRE_SHAPEDESCRIPTION_H

#include "HkOgreConfig.h"
#include "HkOgreHavokPrototypes.h"
#include <Ogre.h>

namespace HkOgre
{

class HkOgrePublicClass ShapeDescription
{
public:
    ShapeDescription();
    virtual ~ShapeDescription();

    virtual hkpConvexShape* createShape() const;
    virtual Ogre::ManualObject* createDebugEntity(const Ogre::String& strName)  const;
    virtual hkMassProperties calcMassProperties(float fMass) const;
};

class HkOgrePublicClass BoxShapeDescription : public ShapeDescription
{
public:
    BoxShapeDescription(const Ogre::Vector3& vecSize);
    ~BoxShapeDescription();

    virtual hkpConvexShape* createShape()  const;
    virtual Ogre::ManualObject* createDebugEntity(const Ogre::String& strName) const;
    virtual hkMassProperties calcMassProperties(float fMass) const;
private:
    Ogre::Vector3 m_vecSize;
};

class HkOgrePublicClass SphereShapeDescription : public ShapeDescription
{
public:
    SphereShapeDescription(const Ogre::Real radius);
    ~SphereShapeDescription();

    virtual hkpConvexShape* createShape()  const;
    virtual Ogre::ManualObject* createDebugEntity(const Ogre::String& strName) const;
    virtual hkMassProperties calcMassProperties(float fMass) const;
private:
    Ogre::Real m_radius;
};

}//namespace HkOgre

#endif