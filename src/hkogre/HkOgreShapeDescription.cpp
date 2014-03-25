#include "HkOgreShapeDescription.h"
#include <Common/Base/hkBase.h>
#include <Physics2012/Collide/Shape/Convex/Box/hkpBoxShape.h>					
#include <Physics2012/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics2012/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics2012/Collide/Shape/Convex/hkpConvexShape.h>
#include <Physics2012/Collide/Shape/Convex/Triangle/hkpTriangleShape.h>
#include <Physics2012/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>
#include <Physics2012/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>
#include <Physics2012/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldBaseCinfo.h>
#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>
#include <XkDebugStr.h>
#include "HkOgreUtil.h"

namespace HkOgre
{

//////////////////////////////////////////////
class MySampledHeightFieldShape : public hkpSampledHeightFieldShape
{
public:
    MySampledHeightFieldShape( const hkpSampledHeightFieldBaseCinfo& ci, Ogre::Terrain* pTerrain )
			:	hkpSampledHeightFieldShape(ci), mTerrain(pTerrain)
    {

    }

    hkReal getHeightAtImpl( int x, int z ) const
    {
			// Lookup data and return a float
			// We scale the data artificially by 5 to make it look interesting
        hkReal fRet =  mTerrain->getHeightAtPoint(x, z);
        //XkDebugString("fRet: %f (%d, %d)", fRet, x, z);
        return fRet;
    }

    hkBool getTriangleFlipImpl() const
	{	
		return false;
	}

    virtual void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const
	{
		hkSampledHeightFieldShape_collideSpheres(*this, input, outputArray);
	}

private:
    Ogre::Terrain* mTerrain;
};

namespace{
    MySampledHeightFieldShape* g_heightFieldShape = 0;
}

/////////////////////////////////////////////


ShapeDescription::ShapeDescription()
{
}

ShapeDescription::~ShapeDescription()
{
}

hkpShape* ShapeDescription::createShape()  const
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

hkpShape* BoxShapeDescription::createShape() const
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

hkpShape* SphereShapeDescription::createShape() const
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

CapsuleShapeDescription::CapsuleShapeDescription(const Ogre::Vector3& vecStart,
                        const Ogre::Vector3& vecEnd,
                        Ogre::Real fRadius):
    m_vecStart(vecStart),m_vecEnd(vecEnd), m_radius(fRadius)
{

}

CapsuleShapeDescription::~CapsuleShapeDescription()
{

}

hkpShape* CapsuleShapeDescription::createShape() const
{
    hkVector4 start = tohkVector4(m_vecStart);
    hkVector4 end = tohkVector4(m_vecEnd);

    return new hkpCapsuleShape(start, end, m_radius);
}

Ogre::ManualObject* CapsuleShapeDescription::createDebugEntity(const Ogre::String& strName) const
{
    return createDebugCapsule(strName,
        m_vecStart, 
        m_vecEnd,
        m_radius);
}

hkMassProperties CapsuleShapeDescription::calcMassProperties(float fMass) const
{
    hkMassProperties massProp;
    hkVector4 start = tohkVector4(m_vecStart);
    hkVector4 end = tohkVector4(m_vecEnd);

    hkpInertiaTensorComputer::computeCapsuleVolumeMassProperties(start, end, m_radius, fMass, massProp);
    return massProp;
}

TriangleShapeDescription::TriangleShapeDescription(const Ogre::Vector3& v0,
        const Ogre::Vector3& v1,
        const Ogre::Vector3& v2) :
    m_v0(v0), m_v1(v1), m_v2(v2)
{
    
}

TriangleShapeDescription::~TriangleShapeDescription()
{

}

hkpShape* TriangleShapeDescription::createShape() const
{
    hkVector4 v0 = tohkVector4(m_v0);
    hkVector4 v1 = tohkVector4(m_v1);
    hkVector4 v2 = tohkVector4(m_v2);

    return new hkpTriangleShape(v0, v1, v2);
}

Ogre::ManualObject* TriangleShapeDescription::createDebugEntity(const Ogre::String& strName) const
{
    return createDebugTriangle(strName, m_v0, m_v1, m_v2);
}

hkMassProperties TriangleShapeDescription::calcMassProperties(float fMass) const
{
    hkVector4 v0 = tohkVector4(m_v0);
    hkVector4 v1 = tohkVector4(m_v1);
    hkVector4 v2 = tohkVector4(m_v2);

    hkMassProperties massProp;

    hkpInertiaTensorComputer::computeTriangleSurfaceMassProperties(
        v0, v1, v2, fMass, 1.0f, massProp);

    return massProp;
}

SampledHeightFieldShapeDescription::SampledHeightFieldShapeDescription(Ogre::Terrain* pTerrain) :
    m_pTerrain(pTerrain)
{
    
}

SampledHeightFieldShapeDescription::~SampledHeightFieldShapeDescription()
{

}

hkpShape* SampledHeightFieldShapeDescription::createShape() const
{
    hkpSampledHeightFieldBaseCinfo ci;
    ci.m_xRes = m_pTerrain->getWorldSize();
    ci.m_zRes = m_pTerrain->getWorldSize();
    XkDebugString("m_xRes: %d, m_zRes: %d", ci.m_xRes, ci.m_zRes);

    MySampledHeightFieldShape* heightFieldShape = new MySampledHeightFieldShape(ci, m_pTerrain);
    
    XkDebugString("HeightField extends:(%f, %f, %f)", heightFieldShape->m_extents(0),
        heightFieldShape->m_extents(1),
        heightFieldShape->m_extents(2));
    XkDebugString("HeightField center:(%f)",
        heightFieldShape->m_heightCenter);

    g_heightFieldShape = heightFieldShape;

    return heightFieldShape;
}

Ogre::ManualObject* SampledHeightFieldShapeDescription::createDebugEntity(const Ogre::String& strName) const
{
    Ogre::SceneManager* pSceneMgr = ::Ogre::Root::getSingletonPtr()->getSceneManagerIterator().getNext();
    Ogre::ManualObject* pObj = pSceneMgr->createManualObject(strName);
	pObj->clear();
    pObj->begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    /*
    for(int i=0; i<m_pTerrain->getSize(); i++){
        for(int j=0; j<m_pTerrain->getSize(); j++) {
            Ogre::Real fHeight = m_pTerrain->getHeightAtPoint(i, j);
            pObj->position(i, fHeight, j);
        }
    }
    */
    MySampledHeightFieldShape* heightFieldShape = g_heightFieldShape;

    hkVector4 scale = heightFieldShape->m_intToFloatScale;
    for(int i=0; i<heightFieldShape->m_xRes; ++i) {
        for(int j=0; j<heightFieldShape->m_zRes; ++j) {
            hkVector4 p00;
            p00.set((hkReal)i, heightFieldShape->getHeightAt(i, j), (hkReal) j);
            p00.mul(scale);
            pObj->position(toVector3(p00));
        }
    }

    for(int i=0; i<heightFieldShape->m_xRes -1; ++i) {
        for(int j=0; j<heightFieldShape->m_zRes -1; ++j) {
            const int thisRowV = i * heightFieldShape->m_zRes;
            const int nextRowV = thisRowV + heightFieldShape->m_zRes; 
            const int thisRowI = i * ( heightFieldShape->m_zRes - 1) * 2;
            if(heightFieldShape->getTriangleFlip()) {
                pObj->triangle(thisRowV + j, thisRowV + j + 1, nextRowV +j + 1);
                pObj->triangle(thisRowV +j, nextRowV +j +1, nextRowV +j );
            }else{
                pObj->triangle(thisRowV +j, thisRowV +j +1, nextRowV +j);
                pObj->triangle(thisRowV +j +1, nextRowV +j +1, nextRowV +j);
            }
        }
    }

    pObj->end();
    return pObj;
}

hkMassProperties SampledHeightFieldShapeDescription::calcMassProperties(float fMass) const
{
    hkMassProperties massProp;
    return massProp;
}

}//namespace HkOgre