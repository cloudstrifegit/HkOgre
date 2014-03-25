cmake_minimum_required(VERSION 2.8)

PROJECT(hkogre)

IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	SET(HAVOK_LIB_TYPE "dev")
ELSE(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	SET(HAVOK_LIB_TYPE "release")
ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Debug")

SET(raknetinc $ENV{SNH_HOME}/RakNet4.0802/Source)
SET(pocoinc $ENV{SNH_HOME}/poco/include)
SET(raknetlib $ENV{SNH_HOME}/RakNet4.0802/Lib)
SET(pocolib $ENV{SNH_HOME}/poco/lib)
SET(havokinc $ENV{SNH_HOME}/hk2013_1_0_r1/Source)
SET(havoklib $ENV{SNH_HOME}/hk2013_1_0_r1/Lib/win32_vs2010/${HAVOK_LIB_TYPE})
SET(ogreinc $ENV{SNH_HOME}/cloud-ogresdk-v1-9-0/include/OGRE)
SET(ogrelib $ENV{SNH_HOME}/cloud-ogresdk-v1-9-0/lib/${CMAKE_BUILD_TYPE})
SET(ogrelibopt $ENV{SNH_HOME}/cloud-ogresdk-v1-9-0/lib/${CMAKE_BUILD_TYPE}/opt)

MESSAGE(STATUS ${havoklib})

INCLUDE_DIRECTORIES(
	${raknetinc}
	${pocoinc}
	${ogreinc}
	$ENV{SNH_HOME}/cloud-ogresdk-v1-9-0/include/OGRE/Terrain
	$ENV{SNH_HOME}/cloud-ogresdk-v1-9-0/include/OGRE/Paging
	${havokinc}
	../include
)

LINK_DIRECTORIES(
	${raknetlib}
	${pocolib}
	${havoklib}
	${ogrelib}
	${ogrelibopt}
)

IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	SET(ogrelibs OgreMain_d OgreOverlay_d  OgrePaging_d OgreRTShaderSystem_d
		OgreTerrain_d OgreVolume_d OIS_d Plugin_BSPSceneManager_d
		Plugin_CgProgramManager_d Plugin_OctreeSceneManager_d
		Plugin_OctreeZone_d Plugin_ParticleFX_d Plugin_PCZSceneManager_d
		RenderSystem_Direct3D9_d RenderSystem_Direct3D11_d )
ELSE(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	SET(ogrelibs OgreMain OgreOverlay  OgrePaging OgreRTShaderSystem
		OgreTerrain OgreVolume OIS Plugin_BSPSceneManager
		Plugin_CgProgramManager Plugin_OctreeSceneManager
		Plugin_OctreeZone Plugin_ParticleFX Plugin_PCZSceneManager
		RenderSystem_Direct3D9 RenderSystem_Direct3D11 )
ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Debug")

SET(havoklibs hkBase hkCompat hkSceneData hkSerialize hkInternal
		hkGeometryUtilities hkVisualize hkcdInternal hkcdCollide
		hkpCollide hkpConstraint hkpConstraintSolver hkpDynamics
		hkpInternal hkpUtilities hkpVehicle)

AUX_SOURCE_DIRECTORY( ${PROJECT_SOURCE_DIR} src)
SET(source ${src})

ADD_DEFINITIONS(-DPOCO_STATIC -DHKOGRE_DYNAMIC_LIBRARY)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../lib)

ADD_LIBRARY(hkogre SHARED ${source})

FOREACH(_var ${ogrelibs})
	TARGET_LINK_LIBRARIES(hkogre ${_var})
ENDFOREACH()

FOREACH(_var ${havoklibs})
	TARGET_LINK_LIBRARIES(hkogre ${_var})
ENDFOREACH()

MESSAGE(STATUS ${PROJECT_BINARY_DIR})

INSTALL(TARGETS hkogre DESTINATION ${PROJECT_SOURCE_DIR}/../../bin/${CMAKE_BUILD_TYPE})