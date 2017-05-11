//------------------------------------------------------------------------ -
//    @FileName			:    VFNetPlugin.cpp
//    @Author           :    WenZheng.Xu
//    @Date             :    2017-02-15
//    @Module           :    VFNetPlugin
//
// -------------------------------------------------------------------------

#include "VFNetPlugin.h"
#include "VFCNetModule.h"
#include "VFCNetClientModule.h"
#include "VFCHttpServerModule.h"
#include "VFCWebsocketModule.h"
#include "VFComm/VFPluginModule/VFIPluginManager.h"

#ifdef VF_DYNAMIC_PLUGIN

VF_EXPORT void DllStartPlugin(VFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, VFNetPlugin)

};

VF_EXPORT void DllStopPlugin(VFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, VFNetPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

const int VFNetPlugin::GetPluginVersion()
{
    return 0;
}

const std::string VFNetPlugin::GetPluginName()
{
	return GET_CLASS_NAME(VFNetPlugin);
}

void VFNetPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, VFINetModule, VFCNetModule)
    REGISTER_MODULE(pPluginManager, VFINetClientModule, VFCNetClientModule)
	REGISTER_MODULE(pPluginManager, VFIHttpServerModule, VFCHttpServerModule)
	REGISTER_MODULE(pPluginManager, VFIWebsocketModule, VFCWebsocketModule)
}

void VFNetPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, VFIWebsocketModule, VFCWebsocketModule)
    UNREGISTER_MODULE(pPluginManager, VFINetClientModule, VFCNetClientModule)
    UNREGISTER_MODULE(pPluginManager, VFINetModule, VFCNetModule)
	UNREGISTER_MODULE(pPluginManager, VFIHttpServerModule, VFCHttpServerModule)
}