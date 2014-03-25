#ifndef HKOGRE_SHAPEDESCRIPTION_H
#define HKOGRE_SHAPEDESCRIPTION_H

#include "HkOgreConfig.h"
#include "HkOgreHavokPrototypes.h"
#include <Ogre.h>
#include <OgreTerrain.h>

namespace HkOgre
{

class HkOgrePublicClass ShapeDescription
{
public:
    ShapeDescription();
    virtual ~ShapeDescription();

    virtual hkpShape* createShape() const;
    virtual Ogre::ManualObject* createDebugEntity(const Ogre::String& strName)  const;
    virtual hkMassProperties calcMassProperties(float fMass) const;
};

class HkOgrePublicClass BoxShapeDescription : public ShapeDescription
{
public:
    BoxShapeDescription(const Ogre::Vector3& vecSize);
    ~BoxShapeDescription();

    virtual hkpShape* createShape()  const;
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

    virtual hkpShape* createShape()  const;
    virtual Ogre::ManualObject* createDebugEntity(const Ogre::String& strName) const;
    virtual hkMassProperties calcMassProperties(float fMass) const;
private:
    Ogre::Real m_radius;
};

class HkOgrePublicClass CapsuleShapeDescription : public ShapeDescription
{
public:
    CapsuleShapeDescription(const Ogre::Vector3& vecStart,
                        const Ogre::Vector3& vecEnd,
                        Ogre::Real fRadius);
    ~CapsuleShapeDescription();

    virtual hkpShape* createShape() const;
    virtual Ogre::ManualObject* createDebugEntity(const Ogre::String& strName) const;
    virtual hkMassProperties calcMassProperties(float fMass) const;

private:
    Ogre::Vector3 m_vecStart;
    Ogre::Vector3 m_vecEnd;
    Ogre::Real m_radius;
};

class HkOgrePublicClass TriangleShapeDescription : public ShapeDescription
{
public:
    TriangleShapeDescription(const Ogre::Vector3& v0,
        const Ogre::Vector3& v1,
        const Ogre::Vector3& v2);

    ~TriangleShapeDescription();

    virtual hkpShape* createShape() const;
    virtual Ogre::ManualObject* createDebugEntity(const Ogre::String& strName) const;
    virtual hkMassProperties calcMassProperties(float fMass) const;

private:
    Ogre::Vector3 m_v0;
    Ogre::Vector3 m_v1;
    Ogre::Vector3 m_v2;
};

class HkOgrePublicClass SampledHeightFieldShapeDescription : public ShapeDescription
{
public:
    SampledHeightFieldShapeDescription(Ogre::Terrain* pTerrain);

    ~SampledHeightFieldShapeDescription();

    virtual hkpShape* createShape() const;
    virtual Ogre::ManualObject* createDebugEntity(const Ogre::String& strName) const;
    virtual hkMassProperties calcMassProperties(float fMass) const;

private:
    Ogre::Terrain* m_pTerrain;
};

}//namespace HkOgre

#endif