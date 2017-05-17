// -------------------------------------------------------------------------
//    @FileName         :    VFCEventModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    VFCEventModule
//
// -------------------------------------------------------------------------

#ifndef VFC_EVENT_MODULE_H
#define VFC_EVENT_MODULE_H

#include <iostream>
#include "VFComm/VFPluginModule/VFGUID.h"
#include "VFComm/VFPluginModule/VFIEventModule.h"

class VFCEventModule
    : public VFIEventModule
{
public:
    VFCEventModule(VFIPluginManager* p)
    {
        pPluginManager = p;
    }


    virtual ~VFCEventModule()
    {
    }

	virtual bool Init();
	virtual bool AfterInit();
	virtual bool BeforeShut();
	virtual bool Shut();
	virtual bool Execute();

	virtual bool DoEvent(const VFEventDefine nEventID, const VFDataList& valueList);

	virtual bool ExistEventCallBack(const VFEventDefine nEventID);

	virtual bool RemoveEventCallBack(const VFEventDefine nEventID);

	//////////////////////////////////////////////////////////
	virtual bool DoEvent(const VFGUID self, const VFEventDefine nEventID, const VFDataList& valueList);

	virtual bool ExistEventCallBack(const VFGUID self, const VFEventDefine nEventID);

	virtual bool RemoveEventCallBack(const VFGUID self, const VFEventDefine nEventID);
	virtual bool RemoveEventCallBack(const VFGUID self);

protected:

	virtual bool AddEventCallBack(const VFEventDefine nEventID, const MODULE_EVENT_FUNCTOR_PTR cb);
	virtual bool AddEventCallBack(const VFGUID self, const VFEventDefine nEventID, const OBJECT_EVENT_FUNCTOR_PTR cb);

private:


private:
	// for module
	VFList<VFEventDefine> mModuleRemoveListEx;
	VFMapEx<VFEventDefine, VFList<MODULE_EVENT_FUNCTOR_PTR>> mModuleEventInfoMapEx;

	//for object
	VFList<VFGUID> mObjectRemoveListEx;
	VFMapEx<VFGUID, VFMapEx<VFEventDefine, VFList<OBJECT_EVENT_FUNCTOR_PTR>>> mObjectEventInfoMapEx;
};

#endif