#ifndef HKOGRE_AUTOCONFIG_H
#define HKOGRE_AUTOCONFIG_H

#ifdef HKOGRE_DYNAMIC_LIBRARY
#define HkOgrePublicClass __declspec(dllexport)
#define HkOgrePublicFunction __declspec(dllexport)
#define HkOgrePublicTemplateClass
#else
#define HkOgrePublicClass __declspec(dllimport)
#define HkOgrePublicFunction __declspec(dllimport)
#define HkOgrePublicTemplateClass
#endif

#ifdef HKOGRE_STATIC_LIBRARY
#undef HkOgrePublicClass
#undef HkOgrePublicFunction
#undef HkOgrePublicTemplateClass
#define HkOgrePublicClass
#define HkOgrePublicFunction
#define HkOgrePublicTemplateClass
#endif

//#pragma warning (disable : 4099)

#endif