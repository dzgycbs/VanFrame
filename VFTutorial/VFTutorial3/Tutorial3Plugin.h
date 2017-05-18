// -------------------------------------------------------------------------
//    @FileName			:    VFTutorial3.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   VFTutorial3
//
// -------------------------------------------------------------------------

#ifndef VF_TUTORIAL3_H
#define VF_TUTORIAL3_H

///
#include "VFComm/VFPluginModule/VFIPlugin.h"
#include "VFComm/VFPluginModule/VFIPluginManager.h"

class Tutorial3Plugin : public VFIPlugin
{
public:
    Tutorial3Plugin(VFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif