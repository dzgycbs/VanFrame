#include "VFComm/VFPluginModule/VFPlatform.h"

#ifdef VF_DEBUG_MODE

#if VF_PLATFORM == VF_PLATFORM_WIN
//#pragma comment( lib, "VFCore_d.lib" )

#elif VF_PLATFORM == VF_PLATFORM_LINUX || VF_PLATFORM == VF_PLATFORM_ANDROID
#pragma comment( lib, "VFCore_d.a" )
#elif VF_PLATFORM == VF_PLATFORM_APPLE || VF_PLATFORM == VF_PLATFORM_APPLE_IOS
#endif

#else

#if VF_PLATFORM == VF_PLATFORM_WIN
#pragma comment( lib, "VFCore.lib" )
#elif VF_PLATFORM == VF_PLATFORM_LINUX || VF_PLATFORM == VF_PLATFORM_ANDROID
#pragma comment( lib, "VFCore.a" )
#elif VF_PLATFORM == VF_PLATFORM_APPLE || VF_PLATFORM == VF_PLATFORM_APPLE_IOS
#endif

#endif
