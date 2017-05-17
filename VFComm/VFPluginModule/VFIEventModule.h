// -------------------------------------------------------------------------
//    @FileName         :    VFIEventModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    VFIEventModule
//
// -------------------------------------------------------------------------

#ifndef VFI_EVENT_MODULE_H
#define VFI_EVENT_MODULE_H

#include <iostream>
#include "VFIModule.h"
#include "VFComm/VFCore/VFDataList.hpp"

enum VFEventDefine
{
	VFED_UNKNOW = 0,
	////////////////Has Self---logic//////////////////////////////////////////////////////////
	//arg[0]:resultID[EGameErrorCode]
	VFED_ON_GENERAL_MESSAGE,

	////////////////core events for VF--start////////////////////////////////////////////////////////
	//scene swap
	//common property
	//common record

	////////////////core events for VF--end//////////////////////////////////////////////////////////
	//arg[0]:x,y,z
	VFED_ON_CLIENT_REQUIRE_MOVE,
	VFED_ON_CLIENT_MOVE_RESULT,

	//arg[0]:player_id key_value skillID target(1-N)
	VFED_ON_CLIENT_REQUIRE_USE_SKILL,
	VFED_ON_CLIENT_USE_SKILL_RESULT,

	//arg[0]:skillID, fx, fy, fz, TargetObjectList
	VFED_ON_CLIENT_REQUIRE_USE_SKILL_POS,
	VFED_ON_CLIENT_USE_SKILL_POS_RESULT,

	//arg[0]:itemID,TargetObject
	VFED_ON_CLIENT_REQUIRE_USE_ITEM,

	//arg[0]:itemID, fx, fy, fz, TargetObjectList
	VFED_ON_CLIENT_REQUIRE_USE_ITEM_POS,

	VFED_ON_OBJECT_BE_KILLED,


	VFED_ON_NOTICE_ECTYPE_AWARD,

};

class VFIEventModule
    : public VFIModule
{
public:
protected:

	typedef std::function<int(const VFGUID&, const VFEventDefine, const VFDataList&)> OBJECT_EVENT_FUNCTOR;
	typedef std::function<int(const VFEventDefine, const VFDataList&)> MODULE_EVENT_FUNCTOR;

	typedef VF_SHARE_PTR<OBJECT_EVENT_FUNCTOR> OBJECT_EVENT_FUNCTOR_PTR;//EVENT
	typedef VF_SHARE_PTR<MODULE_EVENT_FUNCTOR> MODULE_EVENT_FUNCTOR_PTR;//EVENT

public:
	// only be used in module
    virtual bool DoEvent(const VFEventDefine nEventID, const VFDataList& valueList) = 0;

    virtual bool ExistEventCallBack(const VFEventDefine nEventID) = 0;
    
    virtual bool RemoveEventCallBack(const VFEventDefine nEventID) = 0;

	template<typename BaseType>
	bool AddEventCallBack(const VFEventDefine nEventID, BaseType* pBase, int (BaseType::*handler)(const VFEventDefine, const VFDataList&))
	{
		MODULE_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2);
		MODULE_EVENT_FUNCTOR_PTR functorPtr(new MODULE_EVENT_FUNCTOR(functor));
		return AddEventCallBack(nEventID, functorPtr);
	}
    ///////////////////////////////////////////////////////////////////////////////////////////////
	// can be used for object
    virtual bool DoEvent(const VFGUID self, const VFEventDefine nEventID, const VFDataList& valueList) = 0;

    virtual bool ExistEventCallBack(const VFGUID self,const VFEventDefine nEventID) = 0;
    
    virtual bool RemoveEventCallBack(const VFGUID self,const VFEventDefine nEventID) = 0;
	virtual bool RemoveEventCallBack(const VFGUID self) = 0;
	
	template<typename BaseType>
	bool AddEventCallBack(const VFGUID& self, const VFEventDefine nEventID, BaseType* pBase, int (BaseType::*handler)(const VFGUID&, const VFEventDefine, const VFDataList&))
	{
		OBJECT_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		OBJECT_EVENT_FUNCTOR_PTR functorPtr(new OBJECT_EVENT_FUNCTOR(functor));
		return AddEventCallBack(self, nEventID, functorPtr);
	}

protected:

	virtual bool AddEventCallBack(const VFEventDefine nEventID, const MODULE_EVENT_FUNCTOR_PTR cb) = 0;
	virtual bool AddEventCallBack(const VFGUID self, const VFEventDefine nEventID, const OBJECT_EVENT_FUNCTOR_PTR cb) = 0;

};

#endif