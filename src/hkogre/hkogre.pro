TEMPLATE = lib
#CONFIG -= qt core
Debug:CONFIG += debug

TARGET = hkogre

HEADERS += HkOgreAutoConfig.h \
           HkOgreCommon.h \
		   HkOgreConfig.h \
		   HkOgreHavokPrototypes.h \
		   HkOgreRigidBody.h \
		   HkOgreRigidBodyDescription.h \
		   HkOgreShapeDescription.h\
		   HkOgreWorld.h

SOURCES += HkOgreRigidBody.cpp\
		   HkOgreRigidBodyDescription.cpp\
		   HkOgreShapeDescription.cpp\
		   HkOgreWorld.cpp\
		   PlatformInitWin32.cpp

Debug:target.path = ../../lib/debug
Debug:target.files = debug/hkogre.lib

Release:target.path = ../../lib/release
Release:target.files = release/hkogre.lib

Debug:bin.path = ../../bin/debug
Debug:bin.files = debug/hkogre.dll
Release:bin.path = ../../bin/release
Release:bin.files = release/hkogre.dll

INSTALLS += target bin
		   
win32{
INCLUDEPATH += $$(SNH_HOME)/hk2013_1_0_r1/Source
INCLUDEPATH += $$(SNH_HOME)/cloud-ogresdk-v1-9-0/include/OGRE
INCLUDEPATH += $$(SNH_HOME)/poco/include
INCLUDEPATH += ../include

LIBS += -L$$(SNH_HOME)/poco/lib

Debug:LIBS += -L$$(SNH_HOME)/hk2013_1_0_r1/Lib/win32_vs2010/dev\
              -L$$(SNH_HOME)/cloud-ogresdk-v1-9-0/lib/debug\
              -L$$(SNH_HOME)/cloud-ogresdk-v1-9-0/lib/debug/opt
			  
Release:LIBS += -L$$(SNH_HOME)/hk2013_1_0_r1/Lib/win32_vs2010/release\
				-L$$(SNH_HOME)/cloud-ogresdk-v1-9-0/lib/release\
                -L$$(SNH_HOME)/cloud-ogresdk-v1-9-0/lib/release/opt
				
LIBS += -lhkBase -lhkCompat -lhkSceneData -lhkSerialize -lhkInternal\
		-lhkGeometryUtilities -lhkVisualize -lhkcdInternal -lhkcdCollide\
		-lhkpCollide -lhkpConstraint -lhkpConstraintSolver -lhkpDynamics\
		-lhkpInternal -lhkpUtilities -lhkpVehicle
Debug:LIBS += -lOgreMain_d -lOgreOverlay_d -lOgrePaging_d -lOgreRTShaderSystem_d\
              -lOgreTerrain_d -lOgreVolume_d -lOIS_d -lPlugin_BSPSceneManager_d\
			  -lPlugin_CgProgramManager_d -lPlugin_OctreeSceneManager_d\
			  -lPlugin_OctreeZone_d -lPlugin_ParticleFX_d -lPlugin_PCZSceneManager_d\
			  -lRenderSystem_Direct3D9_d -lRenderSystem_Direct3D11_d
Release:LIBS += -lOgreMain -lOgreOverlay -lOgrePaging -lOgreRTShaderSystem\
              -lOgreTerrain -lOgreVolume -lOIS -lPlugin_BSPSceneManager\
			  -lPlugin_CgProgramManager -lPlugin_OctreeSceneManager\
			  -lPlugin_OctreeZone -lPlugin_ParticleFX -lPlugin_PCZSceneManager\
			  -lRenderSystem_Direct3D9 -lRenderSystem_Direct3D11

DEFINES += POCO_STATIC HKOGRE_DYNAMIC_LIBRARY
}

