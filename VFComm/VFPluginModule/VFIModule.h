// -------------------------------------------------------------------------
//    @FileName			:    VFIModule.h
//    @Author           :    Wenzheng.Xu
//    @Date             :    2017-05-10
//    @Module           :    VFIModule
//
// -------------------------------------------------------------------------

#ifndef VFI_LOGIC_MODULE_H
#define VFI_LOGIC_MODULE_H

#include <string>
#include "VFComm/VFCore/VFMap.hpp"
#include "VFComm/VFCore/VFList.hpp"
#include "VFGUID.h"
//#include "VFComm/VFCore/VFDataList.hpp"
//#include "VFComm/VFCore/VFIRecord.h"

template<typename DerivedType, typename BaseType>
class TIsDerived
{
public:
    static int AnyFunction(BaseType* base)
    {
        return 1;
    }

    static  char AnyFunction(void* t2)
    {
        return 0;
    }

    enum
    {
        Result = (sizeof(int) == sizeof(AnyFunction((DerivedType*)NULL))),
    };
};

class VFIPluginManager;

class VFIModule
{

public:
    VFIModule()
    {
    }

    virtual ~VFIModule() {}

    virtual bool Awake()
    {
        return true;
    }

    virtual bool Init()
    {

        return true;
    }

    virtual bool AfterInit()
    {
        return true;
    }

    virtual bool CheckConfig()
    {
        return true;
    }

    virtual bool ReadyExecute()
    {
        return true;
    }

    virtual bool Execute()
    {
        return true;
    }

    virtual bool BeforeShut()
    {
        return true;
    }

    virtual bool Shut()
    {
        return true;
    }

    virtual bool Finalize()
    {
        return true;
    }

	virtual bool OnReloadPlugin()
	{
		return true;
	}

    virtual VFIPluginManager* GetPluginManager() const
    {
        return pPluginManager;
    }

    std::string strName;

protected:
	VFIPluginManager* pPluginManager = NULL;
};
#endif
