// -------------------------------------------------------------------------
//    @FileName         :    VFKernelPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2010-03-15
//    @Module           :    VFKernelPlugin
//
// -------------------------------------------------------------------------

#include "VFKernelPlugin.h"
#include "VFCEventModule.h"
#include "VFCScheduleModule.h"

//
//
#ifdef VF_DYNAMIC_PLUGIN

VF_EXPORT void DllStartPlugin(VFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, VFKernelPlugin)

};

VF_EXPORT void DllStopPlugin(VFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, VFKernelPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

const int VFKernelPlugin::GetPluginVersion()
{
    return 0;
}

const std::string VFKernelPlugin::GetPluginName()
{
	return GET_CLASS_NAME(VFKernelPlugin);
}

void VFKernelPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, VFIEventModule, VFCEventModule)
	REGISTER_MODULE(pPluginManager, VFIScheduleModule, VFCScheduleModule)
}

void VFKernelPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, VFIEventModule, VFCEventModule)
	UNREGISTER_MODULE(pPluginManager, VFIScheduleModule, VFCScheduleModule)
}