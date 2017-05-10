// -------------------------------------------------------------------------
//    @FileName			:    HelloWorld1.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   HelloWorld1
//
// -------------------------------------------------------------------------

#ifndef VFC_HELLO_WORLD1_H
#define VFC_HELLO_WORLD1_H

#include "VFComm/VFPluginModule/VFIPlugin.h"
#include "VFComm/VFPluginModule/VFIPluginManager.h"

class VFIHelloWorld1
	: public VFIModule
{
};

class VFCHelloWorld1
    : public VFIHelloWorld1
{
public:
    VFCHelloWorld1(VFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

protected:

};

#endif
