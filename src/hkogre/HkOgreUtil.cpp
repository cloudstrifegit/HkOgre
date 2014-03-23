#include "HkOgreUtil.h"
#include <Common/Base/hkBase.h>

namespace HkOgre
{

Ogre::Vector3 toVector3(const hkVector4f& vec)
{
    return Ogre::Vector3(vec(0), vec(1), vec(2));
}

hkVector4f tohkVector4(const Ogre::Vector3& vec)
{
    return hkVector4(vec.x, vec.y, vec.z);
}

hkQuaternionf tohkQuaternion(const Ogre::Quaternion& q)
{
	hkQuaternion quat;
	quat.set(q.x, q.y, q.z, q.w);

	return quat;
}

Ogre::Quaternion toQuaternion(const hkQuaternionf& q)
{
	return Ogre::Quaternion(q(3), q(0), q(1), q(2));
}

Ogre::ManualObject* createDebugBox(const Ogre::String& strName,
    Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
    Ogre::SceneManager* pSceneMgr = ::Ogre::Root::getSingletonPtr()->getSceneManagerIterator().getNext();
    
    Ogre::ManualObject* pObj = pSceneMgr->createManualObject(strName);
		pObj->clear();
		pObj->begin("BaseWhite", Ogre::RenderOperation::OT_LINE_LIST);

		pObj->position(-x, y, -z);
		pObj->position(x, y, -z);
		
		pObj->position(x, y, -z);
		pObj->position(x, y, z);

		pObj->position(x, y, z);
		pObj->position(-x, y, z);

		pObj->position(-x, y, z);
		pObj->position(-x, y, -z);

		pObj->position(-x, -y, -z);
		pObj->position(x, -y, -z);
		
		pObj->position(x, -y, -z);
		pObj->position(x, -y, z);

		pObj->position(x, -y, z);
		pObj->position(-x, -y, z);

		pObj->position(-x, -y, z);
		pObj->position(-x, -y, -z);

		pObj->position(-x, y, -z);
		pObj->position(-x, -y, -z);
		
		pObj->position(x, y, -z);
		pObj->position(x, -y, -z);

		pObj->position(x, y, z);
		pObj->position(x, -y, z);
		
		pObj->position(-x, y, z);
		pObj->position(-x, -y, z);

		pObj->end();

		return pObj;
}

Ogre::ManualObject*  createDebugSphere(
        const Ogre::String& strName,
        Ogre::Real radius, int nRings, int nSeg)
{
    Ogre::SceneManager* pSceneMgr = ::Ogre::Root::getSingletonPtr()->getSceneManagerIterator().getNext();
    Ogre::ManualObject* pObj = pSceneMgr->createManualObject(strName);
	pObj->clear();
	pObj->begin("BaseWhite", Ogre::RenderOperation::OT_LINE_LIST);

    float fDeltaRingAngle = (Ogre::Math::PI / nRings);
    float fDeltaSegAngle = (2 * Ogre::Math::PI / nSeg);
    unsigned short wVerticeIndex = 0;
    for(int ring = 0; ring <= nRings; ring ++) {
        float r0 = radius * sinf(ring*fDeltaRingAngle);
        float y0 = radius * cosf(ring*fDeltaRingAngle);

        for(int seg = 0; seg <= nSeg; seg++) {
            float x0 = r0*sinf(seg * fDeltaSegAngle);
            float z0 = r0*cosf(seg * fDeltaSegAngle);

            pObj->position(x0, y0, z0);
            pObj->normal(Ogre::Vector3(x0, y0, z0).normalisedCopy());
            pObj->textureCoord((float)seg/ (float)nSeg);
            if(ring != nRings) {
                pObj->index(wVerticeIndex + nSeg + 1);
                pObj->index(wVerticeIndex);
                pObj->index(wVerticeIndex + nSeg);
                pObj->index(wVerticeIndex + nSeg + 1);
                pObj->index(wVerticeIndex + 1);
                pObj->index(wVerticeIndex);
                wVerticeIndex++;
            }
        }
    }
    pObj->end();

    return pObj;
}


}//namespace HkOgre