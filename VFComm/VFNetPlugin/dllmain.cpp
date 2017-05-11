#include "VFComm/VFPluginModule/VFPlatform.h"

#ifdef VF_DEBUG_MODE

#if VF_PLATFORM == VF_PLATFORM_WIN
#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "VFMessageDefine_d.lib" )
#pragma comment( lib, "libprotobuf_d.lib" )
#pragma comment( lib, "libevent_core.lib" )

#elif VF_PLATFORM == VF_PLATFORM_LINUX || VF_PLATFORM == VF_PLATFORM_ANDROID
#pragma comment( lib, "libevent_core.a" )
#pragma comment( lib, "libevent.a" )
#pragma comment( lib, "VFMessageDefine_d.a" )
#elif VF_PLATFORM == VF_PLATFORM_APPLE || VF_PLATFORM == VF_PLATFORM_APPLE_IOS
#endif

#else

#if VF_PLATFORM == VF_PLATFORM_WIN
#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "VFMessageDefine.lib" )
#pragma comment( lib, "libprotobuf.lib" )
#pragma comment( lib, "libevent_core.lib" )

#elif VF_PLATFORM == VF_PLATFORM_LINUX || VF_PLATFORM == VF_PLATFORM_ANDROID
#pragma comment( lib, "libevent.a" )
#pragma comment( lib, "libevent_core.a" )
#pragma comment( lib, "VFMessageDefine.a" )
#elif VF_PLATFORM == VF_PLATFORM_APPLE || VF_PLATFORM == VF_PLATFORM_APPLE_IOS
#endif

#endif
