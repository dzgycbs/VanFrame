// -------------------------------------------------------------------------
//    @FileName			:    VFCKernelModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    VFCKernelModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef VFC_KERNEL_MODULE_H
#define VFC_KERNEL_MODULE_H

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include "VFComm/VFCore/VFIObject.h"
#include "VFComm/VFCore/VFDataList.hpp"
#include "VFComm/VFCore/VFIRecord.h"
#include "VFComm/VFPluginModule/VFGUID.h"
#include "VFComm/VFPluginModule/VFILogModule.h"
#include "VFComm/VFPluginModule/VFIKernelModule.h"
#include "VFComm/VFPluginModule/VFIClassModule.h"
#include "VFComm/VFPluginModule/VFIElementModule.h"
#include "VFComm/VFPluginModule/VFISceneAOIModule.h"
#include "VFComm/VFPluginModule/VFIScheduleModule.h"
#include "VFComm/VFPluginModule/VFIEventModule.h"


class VFCKernelModule
    : public VFIKernelModule,
  public VFMapEx<VFGUID, VFIObject>
{
public:
    VFCKernelModule(VFIPluginManager* p);
    virtual ~VFCKernelModule();

    virtual bool Init();
    virtual bool Shut();

    virtual bool BeforeShut();
    virtual bool AfterInit();

    virtual bool Execute();

    ///////////////////////////////////////////////////////////////////////

    virtual bool ExistScene(const int nSceneID);

	virtual bool ExistObject(const VFGUID& ident);
	virtual bool ExistObject(const VFGUID& ident, const int nSceneID, const int nGroupID);

    virtual VF_SHARE_PTR<VFIObject> GetObject(const VFGUID& ident);
    virtual VF_SHARE_PTR<VFIObject> CreateObject(const VFGUID& self, const int nSceneID, const int nGroupID, const std::string& strClassName, const std::string& strConfigIndex, const VFDataList& arg);

    virtual bool DestroyAll();
    virtual bool DestroySelf(const VFGUID& self);
    virtual bool DestroyObject(const VFGUID& self);

    //////////////////////////////////////////////////////////////////////////
    virtual bool FindProperty(const VFGUID& self, const std::string& strPropertyName);

    virtual bool SetPropertyInt(const VFGUID& self, const std::string& strPropertyName, const VFINT64 nValue);
    virtual bool SetPropertyFloat(const VFGUID& self, const std::string& strPropertyName, const double dValue);
    virtual bool SetPropertyString(const VFGUID& self, const std::string& strPropertyName, const std::string& strValue);
    virtual bool SetPropertyObject(const VFGUID& self, const std::string& strPropertyName, const VFGUID& objectValue);
	virtual bool SetPropertyVector2(const VFGUID& self, const std::string& strPropertyName, const VFVector2& value);
	virtual bool SetPropertyVector3(const VFGUID& self, const std::string& strPropertyName, const VFVector3& value);

    virtual VFINT64 GetPropertyInt(const VFGUID& self, const std::string& strPropertyName);
    virtual double GetPropertyFloat(const VFGUID& self, const std::string& strPropertyName);
    virtual const std::string& GetPropertyString(const VFGUID& self, const std::string& strPropertyName);
    virtual const VFGUID& GetPropertyObject(const VFGUID& self, const std::string& strPropertyName);
	virtual const VFVector2& GetPropertyVector2(const VFGUID& self, const std::string& strPropertyName);
	virtual const VFVector3& GetPropertyVector3(const VFGUID& self, const std::string& strPropertyName);

    //////////////////////////////////////////////////////////////////////////
    virtual VF_SHARE_PTR<VFIRecord> FindRecord(const VFGUID& self, const std::string& strRecordName);
    virtual bool ClearRecord(const VFGUID& self, const std::string& strRecordName);

    virtual bool SetRecordInt(const VFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const VFINT64 nValue);
    virtual bool SetRecordFloat(const VFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue);
    virtual bool SetRecordString(const VFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue);
    virtual bool SetRecordObject(const VFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const VFGUID& objectValue);
	virtual bool SetRecordVector2(const VFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const VFVector2& value);
	virtual bool SetRecordVector3(const VFGUID& self, const std::string& strRecordName, const int nRow, const int nCol, const VFVector3& value);

    virtual bool SetRecordInt(const VFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const VFINT64 value);
    virtual bool SetRecordFloat(const VFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value);
    virtual bool SetRecordString(const VFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value);
    virtual bool SetRecordObject(const VFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const VFGUID& value);
	virtual bool SetRecordVector2(const VFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const VFVector2& value);
	virtual bool SetRecordVector3(const VFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const VFVector3& value);

    virtual VFINT64 GetRecordInt(const VFGUID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual double GetRecordFloat(const VFGUID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual const std::string& GetRecordString(const VFGUID& self, const std::string& strRecordName, const int nRow, const int nCol);
    virtual const VFGUID& GetRecordObject(const VFGUID& self, const std::string& strRecordName, const int nRow, const int nCol);
	virtual const VFVector2& GetRecordVector2(const VFGUID& self, const std::string& strRecordName, const int nRow, const int nCol);
	virtual const VFVector3& GetRecordVector3(const VFGUID& self, const std::string& strRecordName, const int nRow, const int nCol);

    virtual VFINT64 GetRecordInt(const VFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual double GetRecordFloat(const VFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual const std::string& GetRecordString(const VFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual const VFGUID& GetRecordObject(const VFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
	virtual const VFVector2& GetRecordVector2(const VFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
	virtual const VFVector3& GetRecordVector3(const VFGUID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);

    ////////////////////////////////////////////////////////////////
    virtual VFGUID CreateGUID();

    virtual bool CreateScene(const int nSceneID);
    virtual bool DestroyScene(const int nSceneID);

    virtual int GetOnLineCount();
    virtual int GetMaxOnLineCount();

    virtual int RequestGroupScene(const int nSceneID);
    virtual bool ReleaseGroupScene(const int nSceneID, const int nGroupID);
    virtual bool ExitGroupScene(const int nSceneID, const int nGroupID);

	virtual bool GetGroupObjectList(const int nSceneID, const int nGroupID, VFDataList& list);
	virtual bool GetGroupObjectList(const int nSceneID, const int nGroupID, VFDataList& list, const VFGUID& noSelf);
	virtual bool GetGroupObjectList(const int nSceneID, const int nGroupID, VFDataList& list, const bool bPlayer);
	virtual bool GetGroupObjectList(const int nSceneID, const int nGroupID, VFDataList& list, const bool bPlayer, const VFGUID& noSelf);
	virtual bool GetGroupObjectList(const int nSceneID, const int nGroupID, const std::string& strClassName, VFDataList& list);
	virtual bool GetGroupObjectList(const int nSceneID, const int nGroupID, const std::string& strClassName, const VFGUID& noSelf, VFDataList& list);
	
	virtual int GetObjectByProperty(const int nSceneID, const int nGroupID, const std::string& strPropertyName, const VFDataList& valueArgArg, VFDataList& list);

	virtual void Random(int nStart, int nEnd, int nCount, VFDataList& valueList);
	virtual int Random(int nStart, int nEnd);

    //////////////////////////////////////////////////////////////////////////
    virtual bool LogStack();
    virtual bool LogInfo(const VFGUID ident);
    virtual bool LogSelfInfo(const VFGUID ident);

    //////////////////////////////////////////////////////////////////////////

    virtual bool DoEvent(const VFGUID& self, const std::string& strClassName, CLASS_OBJECT_EVENT eEvent, const VFDataList& valueList);

protected:

    
    virtual bool RegisterCommonClassEvent(const CLASS_EVENT_FUNCTOR_PTR& cb);

    
    virtual bool RegisterCommonPropertyEvent(const PROPERTY_EVENT_FUNCTOR_PTR& cb);

    
    virtual bool RegisterCommonRecordEvent(const RECORD_EVENT_FUNCTOR_PTR& cb);

protected:

    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb);

    void InitRandom();

    int OnClassCommonEvent(const VFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const VFDataList& var);
    int OnPropertyCommonEvent(const VFGUID& self, const std::string& strPropertyName, const VFData& oldVar, const VFData& newVar);
    int OnRecordCommonEvent(const VFGUID& self, const RECORD_EVENT_DATA& xEventData, const VFData& oldVar, const VFData& newVar);

    void ProcessMemFree();

protected:

    std::list<VFGUID> mtDeleteSelfList;

    //////////////////////////////////////////////////////////////////////////
    
    std::list<CLASS_EVENT_FUNCTOR_PTR> mtCommonClassCallBackList;
    
    std::list<PROPERTY_EVENT_FUNCTOR_PTR> mtCommonPropertyCallBackList;
    
    std::list<RECORD_EVENT_FUNCTOR_PTR> mtCommonRecordCallBackList;

private:
    
    //     std::map<std::string,std::map<TData, VFList<VFGUID>>>
    //     std::map<"Scene", std::map<10, VFList<VFGUID>>>

private:
    std::vector<float> mvRandom;
	int nGUIDIndex;
    int mnRandomPos;

    VFGUID mnCurExeObject;
    VFINT64 nLastTime;

	VFISceneAOIModule* m_pSceneModule;
    VFILogModule* m_pLogModule;
    VFIClassModule* m_pClassModule;
    VFIElementModule* m_pElementModule;
	VFIScheduleModule* m_pScheduleModule;
	VFIEventModule* m_pEventModule;
};

#endif
