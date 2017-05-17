// -------------------------------------------------------------------------
//    @FileName         :    VFKernelPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2010-03-15
//    @Module           :    VFKernelPlugin
//
// -------------------------------------------------------------------------
///
#include "VFComm/VFPluginModule/VFIPlugin.h"
#include "VFComm/VFPluginModule/VFIPluginManager.h"

//////////////////////////////////////////////////////////////////////////
class VFKernelPlugin : public VFIPlugin
{
public:
    VFKernelPlugin(VFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
