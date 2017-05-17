// -------------------------------------------------------------------------
//    @FileName			:    VFCEventModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    VFCEventModule
//
// -------------------------------------------------------------------------

#include "VFCEventModule.h"

bool VFCEventModule::Init()
{

    return true;
}

bool VFCEventModule::AfterInit()
{
    return true;
}

bool VFCEventModule::BeforeShut()
{	
	mModuleEventInfoMapEx.ClearAll();
	mObjectEventInfoMapEx.ClearAll();

    return true;
}

bool VFCEventModule::Shut()
{
    return true;
}

bool VFCEventModule::Execute()
{
	//remove
	if (mModuleRemoveListEx.Count() > 0)
	{
		VFEventDefine nEventID = VFEventDefine::VFED_UNKNOW;;
		bool bRet = mModuleRemoveListEx.First(nEventID);
		while (bRet)
		{
			mModuleEventInfoMapEx.RemoveElement(nEventID);

			bRet = mModuleRemoveListEx.Next(nEventID);
		}
	}

	//////
	if (mObjectRemoveListEx.Count() > 0)
	{
		VFGUID xObject;
		bool bRet = mObjectRemoveListEx.First(xObject);
		while (bRet)
		{
			mObjectEventInfoMapEx.RemoveElement(xObject);

			bRet = mObjectRemoveListEx.Next(xObject);
		}
	}

    return true;

}

bool VFCEventModule::DoEvent(const VFEventDefine nEventID, const VFDataList & valueList)
{
	bool bRet = false;

	VF_SHARE_PTR<VFList<MODULE_EVENT_FUNCTOR_PTR>> xEventListPtr = mModuleEventInfoMapEx.GetElement(nEventID);
	if (xEventListPtr)
	{
		MODULE_EVENT_FUNCTOR_PTR pFunPtr;
		bool bRet = xEventListPtr->First(pFunPtr);
		while (bRet)
		{
			MODULE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
			pFunc->operator()(nEventID, valueList);

			bRet = xEventListPtr->Next(pFunPtr);
		}

		bRet = true;
	}

	return bRet;
}

bool VFCEventModule::ExistEventCallBack(const VFEventDefine nEventID)
{
	return mModuleEventInfoMapEx.ExistElement(nEventID);
}

bool VFCEventModule::RemoveEventCallBack(const VFEventDefine nEventID)
{
	return mModuleEventInfoMapEx.RemoveElement(nEventID);
}

bool VFCEventModule::DoEvent(const VFGUID self, const VFEventDefine nEventID, const VFDataList & valueList)
{
	bool bRet = false;

	//if (!m_pKernelodule->ExistObject(self))
	//{
	//	return bRet;
	//}

	VF_SHARE_PTR<VFMapEx<VFEventDefine, VFList<OBJECT_EVENT_FUNCTOR_PTR>>> xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		return bRet;
	}

	VF_SHARE_PTR<VFList<OBJECT_EVENT_FUNCTOR_PTR>> xEventListPtr = xEventMapPtr->GetElement(nEventID);
	if (!xEventListPtr)
	{
		return bRet;
	}

	OBJECT_EVENT_FUNCTOR_PTR pFunPtr;
	bool bFunRet = xEventListPtr->First(pFunPtr);
	while (bFunRet)
	{
		OBJECT_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, nEventID, valueList);

		bFunRet = xEventListPtr->Next(pFunPtr);
	}

	return bRet;
}

bool VFCEventModule::ExistEventCallBack(const VFGUID self, const VFEventDefine nEventID)
{
	VF_SHARE_PTR<VFMapEx<VFEventDefine, VFList<OBJECT_EVENT_FUNCTOR_PTR>>> xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		return false;
	}

	return xEventMapPtr->ExistElement(nEventID);
}

bool VFCEventModule::RemoveEventCallBack(const VFGUID self, const VFEventDefine nEventID)
{
	VF_SHARE_PTR<VFMapEx<VFEventDefine, VFList<OBJECT_EVENT_FUNCTOR_PTR>>> xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		return false;
	}

	return xEventMapPtr->RemoveElement(nEventID);
}

bool VFCEventModule::RemoveEventCallBack(const VFGUID self)
{
	return mObjectEventInfoMapEx.RemoveElement(self);
}

bool VFCEventModule::AddEventCallBack(const VFEventDefine nEventID, const MODULE_EVENT_FUNCTOR_PTR cb)
{
	VF_SHARE_PTR<VFList<MODULE_EVENT_FUNCTOR_PTR>> xEventListPtr = mModuleEventInfoMapEx.GetElement(nEventID);
	if (!xEventListPtr)
	{
		xEventListPtr = VF_SHARE_PTR<VFList<MODULE_EVENT_FUNCTOR_PTR>>(VF_NEW VFList<MODULE_EVENT_FUNCTOR_PTR>());
		mModuleEventInfoMapEx.AddElement(nEventID, xEventListPtr);
	}

	xEventListPtr->Add(cb);

	return false;
}

bool VFCEventModule::AddEventCallBack(const VFGUID self, const VFEventDefine nEventID, const OBJECT_EVENT_FUNCTOR_PTR cb)
{
	//if (!m_pKernelodule->ExistObject(self))
	//{
	//	return false;
	//}
	//
	VF_SHARE_PTR<VFMapEx<VFEventDefine, VFList<OBJECT_EVENT_FUNCTOR_PTR>>> xEventMapPtr = mObjectEventInfoMapEx.GetElement(self);
	if (!xEventMapPtr)
	{
		xEventMapPtr = VF_SHARE_PTR<VFMapEx<VFEventDefine, VFList<OBJECT_EVENT_FUNCTOR_PTR>>>(VF_NEW VFMapEx<VFEventDefine, VFList<OBJECT_EVENT_FUNCTOR_PTR>>());
		mObjectEventInfoMapEx.AddElement(self, xEventMapPtr);
	}

	VF_SHARE_PTR<VFList<OBJECT_EVENT_FUNCTOR_PTR>> xEventListPtr =  xEventMapPtr->GetElement(nEventID);
	if (!xEventListPtr)
	{
		xEventListPtr = VF_SHARE_PTR<VFList<OBJECT_EVENT_FUNCTOR_PTR>>(VF_NEW VFList<OBJECT_EVENT_FUNCTOR_PTR>());
		xEventMapPtr->AddElement(nEventID, xEventListPtr);
	}

	xEventListPtr->Add(cb);

	return true;
}
