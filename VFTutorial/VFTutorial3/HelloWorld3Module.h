// -------------------------------------------------------------------------
//    @FileName			:    HelloWorld3.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   HelloWorld3
//
// -------------------------------------------------------------------------

#ifndef VFC_HELLO_WORLD3_H
#define VFC_HELLO_WORLD3_H

#include "VFComm/VFPluginModule/VFIEventModule.h"
#include "VFComm/VFPluginModule/VFIScheduleModule.h"

class VFIHelloWorld3Module
	: public VFIModule
{

};

class VFCHelloWorld3Module
    : public VFIHelloWorld3Module
{
public:
    VFCHelloWorld3Module(VFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
    int OnEvent(const VFGUID& self, const VFEventDefine event, const VFDataList& arg);
    int OnHeartBeat(const VFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);

protected:
    unsigned long mLastTime;
protected:
	VFIEventModule* m_pEventModule;
	VFIScheduleModule* m_pScheduleModule;
};

#endif
