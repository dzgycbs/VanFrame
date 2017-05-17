// -------------------------------------------------------------------------
//    @FileName			:    VFCScheduleModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-05
//    @Module           :    VFCScheduleModule
//
// -------------------------------------------------------------------------

#ifndef VFC_SCHEDULE_MODULE_H
#define VFC_SCHEDULE_MODULE_H

#include "VFComm/VFCore/VFMap.hpp"
#include "VFComm/VFCore/VFList.hpp"
#include "VFComm/VFCore/VFDataList.hpp"
#include "VFComm/VFCore/VFDateTime.hpp"
#include "VFComm/VFPluginModule/VFIScheduleModule.h"

class  VFCScheduleElement
{
public:
	VFCScheduleElement()
	{
		mstrScheduleName = "";
		mfIntervalTime = 0.0f;
		mnNextTriggerTime = 0;
		mnStartTime = 0;
		mnRemainCount = 0;
		mnAllCount = 0;
		mbForever = false;
	};

	virtual ~VFCScheduleElement()
	{
	}

	void DoHeartBeatEvent();

	std::string mstrScheduleName;
	float mfIntervalTime;
	VFINT64 mnNextTriggerTime;
	VFINT64 mnStartTime;
	int mnRemainCount;
	int mnAllCount;
	bool mbForever;

	VFGUID self;

	VFList<OBJECT_SCHEDULE_FUNCTOR_PTR> mxObjectFunctor;
	VFList<MODULE_SCHEDULE_FUNCTOR_PTR> mxModuleFunctor;
};

class VFCScheduleModule : public VFIScheduleModule
{
public:
	VFCScheduleModule(VFIPluginManager* p);

	virtual ~VFCScheduleModule();

	virtual bool Init();
	virtual bool Execute();

	virtual bool AddSchedule(const std::string& strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR& cb, const float fTime, const int nCount);
	virtual bool AddSchedule(const std::string& strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR& cb, const int nCount, const VFDateTime& date);
	virtual bool RemoveSchedule(const std::string& strScheduleName);
	virtual bool ExistSchedule(const std::string& strScheduleName);
	
	
	virtual bool AddSchedule(const VFGUID self, const std::string& strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const float fTime, const int nCount);
	virtual bool AddSchedule(const VFGUID self, const std::string& strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const int nCount, const VFDateTime& date);
	virtual bool RemoveSchedule(const VFGUID self);
	virtual bool RemoveSchedule(const VFGUID self, const std::string& strScheduleName);
	virtual bool ExistSchedule(const VFGUID self, const std::string& strScheduleName);



protected:
	VFMapEx<VFGUID, VFMapEx <std::string, VFCScheduleElement >> mObjectScheduleMap;//guid_scheduleName_element
	std::list<VFCScheduleElement> mObjectAddList;
	std::map<VFGUID, std::string> mObjectRemoveList;

	VFMapEx <std::string, VFCScheduleElement > mModuleScheduleMap;//guid_scheduleName_element
	std::list<VFCScheduleElement> mModuleAddList;
	std::list<std::string> mModuleRemoveList;
	
};

#endif