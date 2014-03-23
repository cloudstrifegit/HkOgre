#ifndef HKOGRE_WORLD_H
#define HKOGRE_WORLD_H

#include "HkOgreConfig.h"
#include "HkOgreHavokPrototypes.h"
#include <string>
#include <vector>

namespace HkOgre
{
class RigidBody;
class RigidBodyDescription;
class ShapeDescription;

class HkOgrePublicClass World
{
public:
    typedef std::vector<RigidBody*> RigidBodys;

    World();
    ~World();

    RigidBody* createRigidBody(const ShapeDescription& shape,
        const RigidBodyDescription& desc);

    void step(float fTime);
    void simulate(float fTime);

    static World* createWorld();
    static void destroyWorld();
    static World* getWorld();

    static void errorReport(const char* msg, void* userArgGivenToInit);

    hkpWorld* gethkpWorld()
    {
        return m_hkWorld;
    }

protected:
    bool initialize();
    void uninitialize();
    bool createHavokWorld();

private:
    static World* m_pInstance;

    RigidBodys m_rigidBodys;

    hkpWorld* m_hkWorld;

    hkMemoryRouter* m_memoryRouter;
    hkJobThreadPool* m_jobThreadPool;
    hkJobQueue* m_jobQueue;
    int m_nThreadUsed;

#if HKOGRE_ENABLE_VDB == 1
    hkpPhysicsContext* m_physicsContext;
    hkVisualDebugger* m_vdb;
#endif

    float m_accumulator;
    float m_stepSize;
};


}//namespace HkOgre

#endif