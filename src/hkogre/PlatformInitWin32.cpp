/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */
#if defined( HK_ATOM )
typedef struct {unsigned long field[4];} ADP_APPLICATIONID;
extern "C" ADP_APPLICATIONID ADP_DEBUG_APPLICATIONID;
extern "C" int __cdecl ADP_Initialize( void );
extern "C" int __cdecl ADP_IsAuthorized( ADP_APPLICATIONID );

ADP_APPLICATIONID applicationID = ADP_DEBUG_APPLICATIONID;

#include <stdio.h>
#include <stdlib.h>
#endif

void PlatformInit()
{
// When deploying Havok via the Intel ADP platform, it is necessary to
// initialize the ADP server and authorize the application using the
// API calls below. For regular usage, these can be ignored.
#if defined( HK_ATOM )
	if ( ADP_Initialize() != 0 )
	{
		printf( "Could not initialize ADP licensing system.\n" );
		exit(1);
	}
	if ( ADP_IsAuthorized( applicationID ) != 5 )
	{
		printf( "ADP application could not be authorized.\n" );
		exit(1);
	}
#endif
}

void PlatformQuit()
{
#if defined( HK_ATOM )
	ADP_Close();
#endif
}

/*
 * Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20130718)
 * 
 * Confidential Information of Havok.  (C) Copyright 1999-2013
 * Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
 * Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
 * rights, and intellectual property rights in the Havok software remain in
 * Havok and/or its suppliers.
 * 
 * Use of this software for evaluation purposes is subject to and indicates
 * acceptance of the End User licence Agreement for this product. A copy of
 * the license is included with this software and is also available at www.havok.com/tryhavok.
 * 
 */
