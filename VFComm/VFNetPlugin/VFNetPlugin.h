//------------------------------------------------------------------------ -
//    @FileName			:    VFNetPlugin.h
//    @Author           :    WenZheng.Xu
//    @Date             :    2017-02-15
//    @Module           :    VFNetPlugin
//
// -------------------------------------------------------------------------

#include "VFComm/VFPluginModule/VFIPlugin.h"
#include "VFComm/VFPluginModule/VFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class VFNetPlugin : public VFIPlugin
{
public:
	VFNetPlugin(VFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
