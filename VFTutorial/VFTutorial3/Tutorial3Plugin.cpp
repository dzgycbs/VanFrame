#include "Tutorial3Plugin.h"
#include "HelloWorld3Module.h"

#ifdef VF_DYNAMIC_PLUGIN

VF_EXPORT void DllStartPlugin(VFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, Tutorial3Plugin)
};

VF_EXPORT void DllStopPlugin(VFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, Tutorial3Plugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int Tutorial3Plugin::GetPluginVersion()
{
    return 0;
}

const std::string Tutorial3Plugin::GetPluginName()
{
	return GET_CLASS_NAME(Tutorial3Plugin);
}

void Tutorial3Plugin::Install()
{

    REGISTER_MODULE(pPluginManager, VFIHelloWorld3Module, VFCHelloWorld3Module)

}

void Tutorial3Plugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, VFIHelloWorld3Module, VFCHelloWorld3Module)
}
