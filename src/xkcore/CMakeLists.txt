cmake_minimum_required(VERSION 2.8)

PROJECT(xkcore)
IF(WIN32)
    SET(raknetinc $ENV{SNH_HOME}/RakNet4.0802/Source)
    SET(pocoinc $ENV{SNH_HOME}/poco/include)
	SET(protobufinc $ENV{SNH_HOME}/protobuf/include)
    SET(raknetlib $ENV{SNH_HOME}/RakNet4.0802/Lib)
    SET(pocolib $ENV{SNH_HOME}/poco/lib)
	SET(protobuflib $ENV{SNH_HOME}/protobuf/lib)
ELSE(WIN32)
    SET(raknetinc /usr/local/include/raknet)
    SET(pocoinc /usr/local/include)
    SET(raknetlib /usr/local/lib)
    SET(pocolib /usr/local/lib)
ENDIF(WIN32)

INCLUDE_DIRECTORIES(
	${raknetinc}
	${pocoinc}
	${protobufinc}
	../include
)

LINK_DIRECTORIES(
	${raknetlib}
	${pocolib}
	${protobuflib}
)

SET(source	XkServer.cpp
			XkClient.cpp
			XkLog.cpp
			XkRemoteLogChannel.cpp
			XkTimer.cpp	
			XkGC.cpp
                        XkMessageCodec.cpp
                        XkTextCodec.cpp
)

ADD_DEFINITIONS(-DPOCO_STATIC)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../lib)
ADD_LIBRARY(xkcore STATIC ${source})
