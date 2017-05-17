#include "VFComm/VFPluginModule/VFPlatform.h"

#if VF_PLATFORM == VF_PLATFORM_WIN
#pragma comment( lib, "Dbghelp.lib" )
#endif

#ifdef VF_DEBUG_MODE

#if VF_PLATFORM == VF_PLATFORM_WIN

#ifdef VF_DYNAMIC_PLUGIN
//#pragma comment( lib, "VFCore_d.lib" )

#else
//#pragma comment( lib, "VFCore_Static_d.lib" )
#endif

//
//#pragma comment( lib, "libglog_d.lib" )
#elif VF_PLATFORM == VF_PLATFORM_LINUX || VF_PLATFORM == VF_PLATFORM_ANDROID
#pragma comment( lib, "VFCore_Static_d.a" )
//#pragma comment( lib, "libglog_d.a" )
#elif VF_PLATFORM == VF_PLATFORM_APPLE || VF_PLATFORM == VF_PLATFORM_APPLE_IOS
#endif

#else

#if VF_PLATFORM == VF_PLATFORM_WIN
#ifdef VF_DYNAMIC_PLUGIN
#pragma comment( lib, "VFCore.lib" )
#else
#pragma comment( lib, "VFCore_Static.lib" )
#endif

//
//#pragma comment( lib, "libglog.lib" )
#elif VF_PLATFORM == VF_PLATFORM_LINUX || VF_PLATFORM == VF_PLATFORM_ANDROID
#pragma comment( lib, "VFCore_Static.a" )
//#pragma comment( lib, "libglog.a" )
#elif VF_PLATFORM == VF_PLATFORM_APPLE || VF_PLATFORM == VF_PLATFORM_APPLE_IOS
#endif

#endif
