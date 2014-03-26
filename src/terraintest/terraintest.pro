TEMPLATE = app
QT += widgets

TARGET = terraintest

HEADERS += OgreWidget.h MyTerrain.h
SOURCES += main.cpp\
		   OgreWidget.cpp\
		   MyTerrain.cpp

Debug:DESTDIR = ../../../bin/debug
Release:DESTDIR = ../../../bin/release
		   
win32{
INCLUDEPATH += $$(SNH_HOME)/hk2013_1_0_r1/Source
INCLUDEPATH += $$(SNH_HOME)/cloud-ogresdk-v1-9-0/include/OGRE
INCLUDEPATH += $$(SNH_HOME)/cloud-ogresdk-v1-9-0/include/OGRE/Terrain
INCLUDEPATH += $$(SNH_HOME)/cloud-ogresdk-v1-9-0/include/OGRE/Paging
INCLUDEPATH += $$(SNH_HOME)/cloud-ogresdk-v1-9-0/include/OGRE/Valume
INCLUDEPATH += $$(SNH_HOME)/poco/include
INCLUDEPATH += ../include
INCLUDEPATH += ../hkogre

LIBS += -L$$(SNH_HOME)/poco/lib
Debug:LIBS += -L$$PWD/../../lib/debug \
				-L$$(SNH_HOME)/hk2013_1_0_r1/Lib/win32_vs2010/dev
Release:LIBS += -L$$PWD/../../lib/release\
				-L$$(SNH_HOME)/hk2013_1_0_r1/Lib/win32_vs2010/release

Debug:LIBS += -L$$(SNH_HOME)/cloud-ogresdk-v1-9-0/lib/debug\
              -L$$(SNH_HOME)/cloud-ogresdk-v1-9-0/lib/debug/opt
			  
Release:LIBS += -L$$(SNH_HOME)/cloud-ogresdk-v1-9-0/lib/release\
                -L$$(SNH_HOME)/cloud-ogresdk-v1-9-0/lib/release/opt
				


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

LIBS += -lhkogre

LIBS += -lhkBase -lhkCompat -lhkSceneData -lhkSerialize -lhkInternal\
		-lhkGeometryUtilities -lhkVisualize -lhkcdInternal -lhkcdCollide\
		-lhkpCollide -lhkpConstraint -lhkpConstraintSolver -lhkpDynamics\
		-lhkpInternal -lhkpUtilities -lhkpVehicle

DEFINES += POCO_STATIC
}

