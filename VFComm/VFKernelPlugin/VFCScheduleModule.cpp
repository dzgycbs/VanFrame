// -------------------------------------------------------------------------
//    @FileName			:    VFCScheduleModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-05
//    @Module           :    VFCScheduleModule
//
// -------------------------------------------------------------------------

#include "VFCScheduleModule.h"

void VFCScheduleElement::DoHeartBeatEvent()
{
	if (self.IsNull())
	{
		MODULE_SCHEDULE_FUNCTOR_PTR cb;
		bool bRet = this->mxModuleFunctor.First(cb);
		while (bRet)
		{
			cb.get()->operator()(mstrScheduleName, mfIntervalTime, mnRemainCount);
			bRet = this->mxModuleFunctor.Next(cb);
		}
	}
	else
	{
		OBJECT_SCHEDULE_FUNCTOR_PTR cb;
		bool bRet = this->mxObjectFunctor.First(cb);
		while (bRet)
		{
			cb.get()->operator()(self, mstrScheduleName, mfIntervalTime, mnRemainCount);
			bRet = this->mxObjectFunctor.Next(cb);
		}
	}
}

VFCScheduleModule::VFCScheduleModule(VFIPluginManager* p)
{
	pPluginManager = p;
}

VFCScheduleModule::~VFCScheduleModule()
{
	mObjectScheduleMap.ClearAll();
}

bool VFCScheduleModule::Init()
{
	return true;
}
bool VFCScheduleModule::Execute()
{
	//execute every schedule
	VF_SHARE_PTR<VFMapEx <std::string, VFCScheduleElement >> xObjectSchedule = mObjectScheduleMap.First();
	while (xObjectSchedule)
	{
		std::string str;
		VF_SHARE_PTR<VFCScheduleElement> pSchedule = xObjectSchedule->First();
		while (pSchedule)
		{
			VFINT64 nNow = VFGetTime();
			if (nNow > pSchedule->mnNextTriggerTime)
			{
				std::map<VFGUID, std::string>::iterator itRet = mObjectRemoveList.find(pSchedule->self);
				if (itRet == mObjectRemoveList.end())
				{
					if (itRet->second != pSchedule->mstrScheduleName)
					{
						if (pSchedule->mnRemainCount > 0 || pSchedule->mbForever == true)
						{
							pSchedule->mnRemainCount--;
							pSchedule->DoHeartBeatEvent();

							if (pSchedule->mnRemainCount <= 0 && pSchedule->mbForever == false)
							{
								mObjectRemoveList.insert(std::map<VFGUID, std::string>::value_type(pSchedule->self, pSchedule->mstrScheduleName));
							}
							else
							{
								VFINT64 nNextCostTime = VFINT64(pSchedule->mfIntervalTime * 1000) * (pSchedule->mnAllCount - pSchedule->mnRemainCount);
								pSchedule->mnNextTriggerTime = pSchedule->mnStartTime + nNextCostTime;
							}
						}
					}
				}
			}

			pSchedule = xObjectSchedule->Next();
		}

		xObjectSchedule = mObjectScheduleMap.Next();
	}

	//remove schedule
	for (std::map<VFGUID, std::string>::iterator it = mObjectRemoveList.begin(); it != mObjectRemoveList.end(); ++it)
	{
		VFGUID self = it->first;
		std::string scheduleName = it->second;
		auto findIter = mObjectScheduleMap.GetElement(self);
		if (NULL != findIter)
		{
			findIter->RemoveElement(scheduleName);
			if (findIter->Count() == 0)
			{
				mObjectScheduleMap.RemoveElement(self);
			}
		}
	}
	mObjectRemoveList.clear();

	//add schedule
	for (std::list<VFCScheduleElement>::iterator iter = mObjectAddList.begin(); iter != mObjectAddList.end(); ++iter)
	{
		VF_SHARE_PTR< VFMapEx <std::string, VFCScheduleElement >> xObjectScheduleMap = mObjectScheduleMap.GetElement(iter->self);
		if (NULL == xObjectScheduleMap)
		{
			xObjectScheduleMap = VF_SHARE_PTR< VFMapEx <std::string, VFCScheduleElement >>(VF_NEW VFMapEx <std::string, VFCScheduleElement >());
			mObjectScheduleMap.AddElement(iter->self, xObjectScheduleMap);
		}

		VF_SHARE_PTR<VFCScheduleElement> xScheduleElement = xObjectScheduleMap->GetElement(iter->mstrScheduleName);
		if (NULL == xScheduleElement)
		{
			xScheduleElement = VF_SHARE_PTR<VFCScheduleElement>(VF_NEW VFCScheduleElement());
			*xScheduleElement = *iter;

			xObjectScheduleMap->AddElement(iter->mstrScheduleName, xScheduleElement);
		}
	}

	mObjectAddList.clear();

	////////////////////////////////////////////
	//execute every schedule
	VF_SHARE_PTR< VFCScheduleElement > xModuleSchedule = mModuleScheduleMap.First();
	while (xModuleSchedule)
	{
		VFINT64 nNow = VFGetTime();
		if (nNow > xModuleSchedule->mnNextTriggerTime)
		{
			if (xModuleSchedule->mnRemainCount > 0 || xModuleSchedule->mbForever == true)
			{
				xModuleSchedule->mnRemainCount--;
				xModuleSchedule->DoHeartBeatEvent();

				if (xModuleSchedule->mnRemainCount <= 0 && xModuleSchedule->mbForever == false)
				{
					mModuleRemoveList.push_back(xModuleSchedule->mstrScheduleName);
				}
				else
				{
					VFINT64 nNextCostTime = VFINT64(xModuleSchedule->mfIntervalTime * 1000) * (xModuleSchedule->mnAllCount - xModuleSchedule->mnRemainCount);
					xModuleSchedule->mnNextTriggerTime = xModuleSchedule->mnStartTime + nNextCostTime;
				}
			}
		}

		xModuleSchedule = mModuleScheduleMap.Next();
	}

	//remove schedule
	for (std::list<std::string>::iterator it = mModuleRemoveList.begin(); it != mModuleRemoveList.end(); ++it)
	{
		const std::string& strSheduleName = *it;;
		auto findIter = mModuleScheduleMap.GetElement(strSheduleName);
		if (NULL != findIter)
		{
			mModuleScheduleMap.RemoveElement(strSheduleName);
		}
	}
	mModuleRemoveList.clear();

	//add schedule
	for (std::list<VFCScheduleElement>::iterator iter = mModuleAddList.begin(); iter != mModuleAddList.end(); ++iter)
	{
		VF_SHARE_PTR< VFCScheduleElement > xModuleScheduleMap = mModuleScheduleMap.GetElement(iter->mstrScheduleName);
		if (NULL == xModuleScheduleMap)
		{
			xModuleScheduleMap = VF_SHARE_PTR< VFCScheduleElement >(VF_NEW VFCScheduleElement());
			mModuleScheduleMap.AddElement(iter->mstrScheduleName, xModuleScheduleMap);
		}

		*xModuleScheduleMap = *iter;
	}

	mModuleAddList.clear();
	return true;
}

bool VFCScheduleModule::AddSchedule(const std::string & strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR & cb, const float fTime, const int nCount)
{
	VFCScheduleElement xSchedule;
	xSchedule.mstrScheduleName = strScheduleName;
	xSchedule.mfIntervalTime = fTime;
	xSchedule.mnNextTriggerTime = VFGetTime() + (VFINT64)(fTime * 1000);
	xSchedule.mnStartTime = VFGetTime();
	xSchedule.mnRemainCount = nCount;
	xSchedule.mnAllCount = nCount;
	xSchedule.self = VFGUID();
	if (nCount < 0)
	{
		xSchedule.mbForever = true;
	}

	xSchedule.mxModuleFunctor.Add(cb);

	mModuleAddList.push_back(xSchedule);

	return true;
}

bool VFCScheduleModule::AddSchedule(const std::string & strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR & cb, const int nCount, const VFDateTime & date)
{
	return false;
}

bool VFCScheduleModule::RemoveSchedule(const std::string & strScheduleName)
{
	mModuleRemoveList.push_back(strScheduleName);

	return true;
}

bool VFCScheduleModule::ExistSchedule(const std::string & strScheduleName)
{
	return mModuleScheduleMap.ExistElement(strScheduleName);
}

bool VFCScheduleModule::AddSchedule(const VFGUID self, const std::string& strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const float fTime, const int nCount)
{
	VFCScheduleElement xSchedule;
	xSchedule.mstrScheduleName = strScheduleName;
	xSchedule.mfIntervalTime = fTime;
	xSchedule.mnNextTriggerTime = VFGetTime() + (VFINT64)(fTime * 1000);
	xSchedule.mnStartTime = VFGetTime();
	xSchedule.mnRemainCount = nCount;
	xSchedule.mnAllCount = nCount;
	xSchedule.self = self;
	if (nCount < 0)
	{
		xSchedule.mbForever = true;
	}

	xSchedule.mxObjectFunctor.Add(cb);

	mObjectAddList.push_back(xSchedule);

	return true;
}

bool VFCScheduleModule::AddSchedule(const VFGUID self, const std::string & strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR & cb, const int nCount, const VFDateTime & date)
{
	return false;
}

bool VFCScheduleModule::RemoveSchedule(const VFGUID self)
{
	return mObjectScheduleMap.RemoveElement(self);
}

bool VFCScheduleModule::RemoveSchedule(const VFGUID self, const std::string& strScheduleName)
{
	mObjectRemoveList.insert(std::map<VFGUID, std::string>::value_type(self, strScheduleName));
	return true;
}

bool VFCScheduleModule::ExistSchedule(const VFGUID self, const std::string& strScheduleName)
{
	VF_SHARE_PTR< VFMapEx <std::string, VFCScheduleElement >> xObjectScheduleMap = mObjectScheduleMap.GetElement(self);
	if (NULL == xObjectScheduleMap)
	{
		return false;
	}

	return xObjectScheduleMap->ExistElement(strScheduleName);
}
