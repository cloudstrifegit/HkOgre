#include "HkOgreWorld.h"
#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Memory/System/hkMemorySystem.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Common/Base/Container/String/hkStringBuf.h>

// Dynamics includes
#include <Physics2012/Collide/hkpCollide.h>										
#include <Physics2012/Collide/Agent/ConvexAgent/SphereBox/hkpSphereBoxAgent.h>	
#include <Physics2012/Collide/Shape/Convex/Box/hkpBoxShape.h>					
#include <Physics2012/Collide/Shape/Convex/Sphere/hkpSphereShape.h>				
#include <Physics2012/Collide/Dispatch/hkpAgentRegisterUtil.h>					

#include <Physics2012/Collide/Query/CastUtil/hkpWorldRayCastInput.h>			
#include <Physics2012/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>			

#include <Physics2012/Dynamics/World/hkpWorld.h>								
#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>							
#include <Physics2012/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

// Visual Debugger includes
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics2012/Utilities/VisualDebugger/hkpPhysicsContext.h>				

// Keycode
#include <Common/Base/keycode.cxx>

#if !defined HK_FEATURE_PRODUCT_PHYSICS_2012
#error Physics is needed to build this demo. It is included in the common package for reference only.
#endif

// This excludes libraries that are not going to be linked
// from the project configuration, even if the keycodes are
// present

#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION_2012
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR
#undef HK_FEATURE_PRODUCT_PHYSICS
#define HK_FEATURE_REFLECTION_PHYSICS_2012
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#define HK_EXCLUDE_FEATURE_MemoryTracker
#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches 
#define HK_EXCLUDE_LIBRARY_hkGeometryUtilities
#include <Common/Base/Config/hkProductFeatures.cxx>

extern void PlatformInit();
extern void PlatformQuit();

#include <XkDebugStr.h>
#include <stdlib.h>
#include "HkOgreRigidBody.h"
#include "HkOgreShapeDescription.h"
#include "HkOgreRigidBodyDescription.h"

namespace HkOgre
{

World* World::m_pInstance = 0;

World::World() : m_accumulator(0.0f), m_stepSize(1.0f/60.0f)
{

    if(!initialize()) {
        XkDebugString("initialize havok failed!");
        abort();
    }
}

World::~World()
{
    uninitialize();
}

RigidBody* World::createRigidBody(const ShapeDescription& shape,
        const RigidBodyDescription& desc)
{
    RigidBody* rb = new RigidBody(this);

    rb->createRigidBody(shape, desc);

    m_rigidBodys.push_back(rb);

    return rb;
}

void World::step(float fTime)
{
    //m_accumulator += fTime;
    //if(m_accumulator < m_stepSize) return;

    //m_accumulator -= m_stepSize;
    m_hkWorld->stepMultithreaded(m_jobQueue,
        m_jobThreadPool, fTime);

#if HKOGRE_ENABLE_VDB == 1
    m_physicsContext->syncTimers(m_jobThreadPool);
    m_vdb->step();
#endif

    hkMonitorStream::getInstance().reset();
    m_jobThreadPool->clearTimerData();
}

void World::simulate(float fTime)
{
    step(fTime);

    for(RigidBodys::const_iterator it = m_rigidBodys.begin();
        it != m_rigidBodys.end();
        it ++)
    {
        (*it)->advance(fTime);
    }
}

World* World::createWorld()
{
    if(!m_pInstance) {
        m_pInstance = new World();
    }

    return m_pInstance;
}

World* World::getWorld()
{
    return m_pInstance;
}

void World::destroyWorld()
{
    if(m_pInstance)
        delete m_pInstance;
}

void World::errorReport(const char* msg, void* userArgGivenToInit)
{
    XkDebugString("Havok Report: %s\n", msg);
}

bool World::initialize()
{
    m_memoryRouter = hkMemoryInitUtil::initDefault(
        hkMallocAllocator::m_defaultMallocAllocator,
        hkMemorySystem::FrameInfo(500*1024)
        );

    hkBaseSystem::init(m_memoryRouter, errorReport);
    hkHardwareInfo hwInfo;
    hkGetHardwareInfo(hwInfo);

    m_nThreadUsed = hwInfo.m_numThreads;
    DBGSTRING("m_nThreadUsed: %d\n", m_nThreadUsed);

    hkCpuJobThreadPoolCinfo threadPoolCinfo;
    threadPoolCinfo.m_numThreads = m_nThreadUsed - 1;

    threadPoolCinfo.m_timerBufferPerThreadAllocation = 200000;
    m_jobThreadPool = new hkCpuJobThreadPool(threadPoolCinfo);

    hkJobQueueCinfo info;
    info.m_jobQueueHwSetup.m_numCpuThreads = m_nThreadUsed;
    m_jobQueue = new hkJobQueue(info);

    hkMonitorStream::getInstance().resize(200000);
    
    return createHavokWorld();
}

void World::uninitialize()
{
    {
        m_hkWorld->markForWrite();
        m_hkWorld->removeReference();
    }

#if HKOGRE_ENABLE_VDB == 1
    m_vdb->removeReference();
    m_physicsContext->removeReference();
#endif

    delete m_jobQueue;
    m_jobThreadPool->removeReference();
    hkBaseSystem::quit();
    hkMemoryInitUtil::quit();
}

bool World::createHavokWorld()
{
    hkpWorldCinfo worldInfo;
    worldInfo.setBroadPhaseWorldSize(10000);
    worldInfo.m_collisionTolerance = 0.03f;
    worldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;
    worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_REMOVE_ENTITY;
    m_hkWorld = new hkpWorld(worldInfo);

#if HKOGRE_ENABLE_VDB == 1
    m_hkWorld->m_wantDeactivation = false;
#endif

    m_hkWorld->markForWrite();
    hkpAgentRegisterUtil::registerAllAgents(m_hkWorld->getCollisionDispatcher());
    m_hkWorld->registerWithJobQueue(m_jobQueue);

#if HKOGRE_ENABLE_VDB == 1
    hkArray<hkProcessContext*> processContexts;
    m_physicsContext = new hkpPhysicsContext();
    hkpPhysicsContext::registerAllPhysicsProcesses();
    m_physicsContext->addWorld(m_hkWorld);
    processContexts.pushBack(m_physicsContext);
#endif

    m_hkWorld->unmarkForWrite();

#if HKOGRE_ENABLE_VDB == 1
    m_vdb = new hkVisualDebugger(processContexts);
    m_vdb->serve();
#endif

    return true;
}

}//namespace HkOgre