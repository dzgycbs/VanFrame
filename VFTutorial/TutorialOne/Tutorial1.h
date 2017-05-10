// -------------------------------------------------------------------------
//    @FileName			:    Tutorial1.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   Tutorial1
//
// -------------------------------------------------------------------------

#ifndef VF_TUTORIAL1_H
#define VF_TUTORIAL1_H

#include "VFComm/VFPluginModule/VFIPlugin.h"
#include "VFComm/VFPluginModule/VFIPluginManager.h"

class Tutorial1 : public VFIPlugin
{
public:
    Tutorial1(VFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif