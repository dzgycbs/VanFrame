// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef VF_MSGDEFINE_H
#define VF_MSGDEFINE_H
#include "VFComm/VFPluginModule/VFPlatform.h"
//#include "VFComm/VFCore/VFIRecordManager.h"
//#include "VFComm/VFCore/VFIPropertyManager.h"
#include "VFComm/VFCore/VFList.hpp"
#include "VFLimit.h"
#include "VFDefine.pb.h"
#include "VFMsgBase.pb.h"
//#include "VFMsgBaseEx.pb.h"
#include "VFMsgPreGame.pb.h"
//#include "VFMsgShare.pb.h"
//#include "VFMsgURl.pb.h"
//#include "VFMsgMysql.pb.h"
//#include "VFSLGDefine.pb.h"

// -------------------------------------------------------------------------
#pragma pack(push,1)

enum E_CHECK_TYPE
{
    ECT_SAVE        = 0, 
    ECT_PRIVATE     = 1, 
    ECT_PUBLIC      = 2, 
};

// bool RecordToString( VFIRecord* pRecord, VFMsg::ObjectRecordBase& recordBase, E_CHECK_TYPE nCheckType, const bool bCheck );
// bool RecordToString( VFIRecordManager* pRecordManager, VFMsg::ObjectRecordList& recordList, E_CHECK_TYPE nCheckType, const bool bCheck );
//
// bool RecordFormString( VFIRecord* pRecord, const VFMsg::ObjectRecordBase& recordBase );
// bool RecordFormString( VFIRecordManager* pRecordManager, const VFMsg::ObjectRecordList& recordList );
//
// bool PropertyToString( VFIPropertyManager* pProManager, VFMsg::ObjectPropertyList& propertyDB, E_CHECK_TYPE nCheckType, const bool bCheck );
// bool PropertyListToString( VFList<VFIPropertyManager*>& pProManagerList,VFMsg::MultiObjectPropertyList& propertyDB, E_CHECK_TYPE nCheckType, const bool bCheck );
//
// bool PropertyFormString( VFIPropertyManager* pProManager, const VFMsg::ObjectPropertyList& propertyDB );

#pragma pack(pop)
#endif
