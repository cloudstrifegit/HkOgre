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
#include <Physics2012/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>
#include <Physics2012/Collide/Shape/Compound/Tree/hkpBvTreeShape.h>
#include <Physics2012/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics2012/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
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
        //hkReal fRet =  mTerrain->getHeightAtPoint(x, z);
        //XkDebugString("fRet: %f (%d, %d)", fRet, x, z);
        //return fRet;

        //hkReal fScale = mTerrain->getWorldSize()/ 513.0f;
        Ogre::Real fX = (float)x / mTerrain->getWorldSize();
        Ogre::Real fY = (float)z / mTerrain->getWorldSize();

        hkReal fRet = mTerrain->getHeightAtTerrainPosition(fY, fX);
        //XkDebugString("fScale: %f, (fX, fY) = (%f, %f) fHeight: %f", fScale, fX, fY, fRet);
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
    /*
    Ogre::SceneManager* pSceneMgr = ::Ogre::Root::getSingletonPtr()->getSceneManagerIterator().getNext();
    Ogre::ManualObject* pObj = pSceneMgr->createManualObject(strName);
	pObj->clear();
    pObj->begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  
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
    */
    return 0;
}

hkMassProperties SampledHeightFieldShapeDescription::calcMassProperties(float fMass) const
{
    hkMassProperties massProp;
    return massProp;
}

BvTreeShapeDescription::BvTreeShapeDescription(Ogre::TerrainGroup* terrainGroup) :
    m_terrainGroup(terrainGroup)
{

}

BvTreeShapeDescription::~BvTreeShapeDescription()
{

}

hkpShape* BvTreeShapeDescription::createShape() const
{
    // TODO We are only creating a navmesh from terrain at the moment
	// TODO look at PW's loadhouses method for entities loading


	// TODO check for no terrain
	//    if (srcMeshes.empty())
	//        return;

	// PARTS OF THE FOLLOWING CODE WERE TAKEN AND MODIFIED FROM AN OGRE3D FORUM POST
	//const int numNodes = srcMeshes.size();
	// TODO how to retrieve number of pages in terrain group? Hardcoded at the moment.
	int pagesTotal = 1;
	const int totalMeshes = pagesTotal /*+ numNodes*/;

	size_t nverts = 0;
	size_t ntris = 0;
	size_t *meshVertexCount = new size_t[totalMeshes];
	size_t *meshIndexCount = new size_t[totalMeshes];
	Ogre::Vector3 **meshVertices = new Ogre::Vector3*[totalMeshes];
	unsigned long **meshIndices = new unsigned long*[totalMeshes];

	// Calculate terrain bounds
	// TODO extract calculateTerrainExtents() ?
	Ogre::TerrainGroup::TerrainIterator ti = m_terrainGroup->getTerrainIterator();
	Ogre::Terrain* trn;
	// TODO we could calculate terrain count here
	size_t trnCount = 0;

	//---------------------------------------------------------------------------------
	// TERRAIN DATA BUILDING
	ti = m_terrainGroup->getTerrainIterator();
	trnCount = 0;
	while(ti.hasMoreElements())
	{
		trn = ti.getNext()->instance;
		// TODO do we want to set a query mask here? or does default suffice?
		//         trn->setQueryFlags(GEOMETRY_QUERY_MASK);

		// get height data, world size, map size
		float *mapptr = trn->getHeightData();
		float WorldSize = trn->getWorldSize();
		int MapSize = trn->getSize();
		// calculate where we need to move/place our vertices
		float DeltaPos = (WorldSize / 2.0f);

		float DeltaX = 0;
		float DeltaZ = 0;
		// TODO this hardcoded behaviour has to go! Supports only up to 4 terrain pages
		switch(trnCount)
		{
		case 0:
			DeltaX = -2000;
			DeltaZ = 2000;
			break;
		case 1:
			DeltaX = -2000;
			DeltaZ = -2000;
			break;
		case 2:
			DeltaX = 2000;
			DeltaZ = 2000;
			break;
		case 3:
			DeltaX = 2000;
			DeltaZ = -2000;
			break;
		default:
			DeltaX = 0;
			DeltaZ = 0;
		}

		std::cout << "Terrain: " << trnCount << std::endl;

		float Scale = WorldSize / (float)(MapSize - 1);

		//////////////////////////////
		// THIS CODE WAS TAKEN FROM
		// AN OGRE FORUMS THREAD IN THE
		// NEW TERRAIN SCREENSHOTS THREAD
		// IN THE SHOWCASE FORUMS - I ONLY MODIFIED IT
		// TO BE ABLE TO WORK FOR RECAST AND IN THE CONTEXT OF
		// THIS DEMO APPLICATION

		// build vertices
		meshVertices[trnCount] = new Ogre::Vector3[(MapSize*MapSize)];

		int i = 0;
		int u = 0;
		int max = MapSize; // i think i needed this for something before but now it's obviously redundant
		int z = 0;
		for(int x = 0;; ++x)
		{
			// if we've reached the right edge, start over on the next line
			if(x == max)
			{
				x = 0;
				++z;
			}
			// if we reached the bottom/end, we're done
			if(z == max)
				break;

			// Calculate world coordinates for terrain tile vertex. Terrain vertices are defined in tile-local coordinates.
			// add the vertex to the buffer
			meshVertices[trnCount][u] = Ogre::Vector3((Scale * x) + DeltaX, mapptr[(MapSize * z) + x], (Scale * -z) + DeltaZ);

			i += 3;
			++u;
		}


		size_t size = ((MapSize*MapSize)-(MapSize*2)) * 6;
		meshIndices[trnCount] = new unsigned long[size];
		// i will point to the 'indices' index to insert at, x points to the vertex # to use
		i = 0;
		for(int x = 0;;++x)
		{
			// skip rightmost vertices
			if((x+1)%MapSize == 0)
			{
				++x;
			}

			// make a square of 2 triangles
			meshIndices[trnCount][i] = x;
			meshIndices[trnCount][i+1] = x + 1;
			meshIndices[trnCount][i+2] = x + MapSize;

			meshIndices[trnCount][i+3] = x + 1;
			meshIndices[trnCount][i+4] = x + 1 + MapSize;
			meshIndices[trnCount][i+5] = x + MapSize;

			// if we just did the final square, we're done
			if(x+1+MapSize == (MapSize*MapSize)-1)
				break;

			i += 6;
		}

		meshVertexCount[trnCount] = trn->getSize()*trn->getSize();
		meshIndexCount[trnCount] = size;

		nverts += meshVertexCount[trnCount];
		ntris += meshIndexCount[trnCount];

		if(trnCount < pagesTotal)
			++trnCount;
	}
	ntris = ntris / 3;


	hkpSimpleMeshShape* meshStorage = new hkpSimpleMeshShape(hkConvexShapeDefaultRadius);
	meshStorage->m_vertices.setSize(nverts);
	meshStorage->m_triangles.setSize(ntris);

	for(unsigned int i = 0; i < meshVertexCount[0]; i++){
		hkVector4 vertex(meshVertices[0][i].x, meshVertices[0][i].y, meshVertices[0][i].z);
		meshStorage->m_vertices[i] = vertex;
	}
	for(unsigned int i = 0; i < ntris; i++){
		meshStorage->m_triangles[i].m_a = meshIndices[0][i];
		meshStorage->m_triangles[i].m_b = meshIndices[0][i+1];
		meshStorage->m_triangles[i].m_c = meshIndices[0][i+2];
	}

	hkpMoppCompilerInput mfr;
	mfr.setAbsoluteFitToleranceOfAxisAlignedTriangles( hkVector4( 0.1f, 0.1f, 0.1f ) );


	hkpMoppCode* pCode = hkpMoppUtility::buildCode( meshStorage, mfr );
	hkpBvTreeShape* shape = new hkpMoppBvTreeShape( meshStorage, pCode );
	pCode->removeReference();
	meshStorage->removeReference();

	delete [] meshVertices;
	delete [] meshVertexCount;
	delete [] meshIndices;
	delete [] meshIndexCount;

	return shape;
}

Ogre::ManualObject* BvTreeShapeDescription::createDebugEntity(const Ogre::String& strName) const
{
    return 0;
}

hkMassProperties BvTreeShapeDescription::calcMassProperties(float fMass) const
{
    hkMassProperties massProp;
    return massProp;
}

}//namespace HkOgre