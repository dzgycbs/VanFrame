#include "Tutorial1.h"
#include "HelloWorld1.h"

#ifdef VF_DYNAMIC_PLUGIN

VF_EXPORT void DllStartPlugin(VFIPluginManager* pm)
{

    CREATE_PLUGIN(pm, Tutorial1)

};

VF_EXPORT void DllStopPlugin(VFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, Tutorial1)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int Tutorial1::GetPluginVersion()
{
    return 0;
}

const std::string Tutorial1::GetPluginName()
{
	return GET_CLASS_NAME(Tutorial1);
}

void Tutorial1::Install()
{
    REGISTER_MODULE(pPluginManager, VFIHelloWorld1, VFCHelloWorld1)
}

void Tutorial1::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, VFIHelloWorld1, VFCHelloWorld1)
}