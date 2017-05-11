// -------------------------------------------------------------------------
//    @FileName         :    VFINetClientModule.h
//    @Author           :    WenZheng.Xu
//    @Date             :    2015-01-4
//    @Module           :    VFINetClientModule
//
// -------------------------------------------------------------------------

#ifndef VFI_CLUSTER_CLIENT_MODULE_H
#define VFI_CLUSTER_CLIENT_MODULE_H

#include <iostream>
#include "VFIModule.h"
#include "VFINetModule.h"
#include "VFComm/VFCore/VFCConsistentHash.hpp"

enum ConnectDataState
{
	DISCONNECT,
    CONNECTING,
	NORMAL,
    RECONNECT,
};

struct ConnectData
{
	ConnectData()
	{
		nGameID = 0;
		nPort = 0;
		strName = "";
		strIP = "";
		eServerType = VF_ST_NONE;
		eState = ConnectDataState::DISCONNECT;
		mnLastActionTime = 0;
	}

	int nGameID;
	VF_SERVER_TYPES eServerType;
	std::string strIP;
	int nPort;
	std::string strName;
	ConnectDataState eState;
	VFINT64 mnLastActionTime;

	VF_SHARE_PTR<VFINetModule> mxNetModule;
};

class VFINetClientModule : public VFIModule
{
public:
    enum EConstDefine
    {
        EConstDefine_DefaultWeith = 500,
    };

	template<typename BaseType>
	bool AddReceiveCallBack(const VF_SERVER_TYPES eType, const int nMsgID, BaseType* pBase, void (BaseType::*handleRecieve)(const int, const int, const char*, const uint32_t))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		NET_RECEIVE_FUNCTOR_PTR functorPtr(new NET_RECEIVE_FUNCTOR(functor));

		AddReceiveCallBack(eType, nMsgID, functorPtr);

		return true;
	}

	template<typename BaseType>
	int AddReceiveCallBack(const VF_SERVER_TYPES eType, BaseType* pBase, void (BaseType::*handleRecieve)(const int, const int, const char*, const uint32_t))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		NET_RECEIVE_FUNCTOR_PTR functorPtr(new NET_RECEIVE_FUNCTOR(functor));

		AddReceiveCallBack(eType, functorPtr);

		return true;
	}

	template<typename BaseType>
	bool AddEventCallBack(const VF_SERVER_TYPES eType, BaseType* pBase, void (BaseType::*handler)(const int, const VF_NET_EVENT, VFINet*))
	{
		NET_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NET_EVENT_FUNCTOR_PTR functorPtr(new NET_EVENT_FUNCTOR(functor));

		AddEventCallBack(eType, functorPtr);

		return true;
	}

	virtual int AddReceiveCallBack(const VF_SERVER_TYPES eType, NET_RECEIVE_FUNCTOR_PTR functorPtr) = 0;
	virtual int AddReceiveCallBack(const VF_SERVER_TYPES eType, const int nMsgID, NET_RECEIVE_FUNCTOR_PTR functorPtr) = 0;
	virtual int AddEventCallBack(const VF_SERVER_TYPES eType, NET_EVENT_FUNCTOR_PTR functorPtr) = 0;

	virtual void RemoveReceiveCallBack(const VF_SERVER_TYPES eType, const int nMsgID) = 0;
	////////////////////////////////////////////////////////////////////////////////

	virtual void AddServer(const ConnectData& xInfo) = 0;
	virtual int ExpandBufferSize(const unsigned int size = 1024 * 1024 * 20) = 0;

	virtual void SendByServerID(const int nServerID, const int nMsgID, const std::string& strData) = 0;
	virtual void SendByServerID(const int nServerID, const int nMsgID, const char* msg, const uint32_t nLen) = 0;

	virtual void SendToAllServer(const int nMsgID, const std::string& strData) = 0;
	virtual void SendToAllServer(const VF_SERVER_TYPES eType, const int nMsgID, const std::string& strData) = 0;

	virtual void SendToServerByPB(const int nServerID, const uint16_t nMsgID, google::protobuf::Message& xData) = 0;

	virtual void SendToAllServerByPB(const uint16_t nMsgID, google::protobuf::Message& xData) = 0;
	virtual void SendToAllServerByPB(const VF_SERVER_TYPES eType, const uint16_t nMsgID, google::protobuf::Message& xData) = 0;

	////////////////////////////////////////////////////////////////////////////////

	virtual void SendBySuit(const VF_SERVER_TYPES eType, const std::string& strHashKey, const int nMsgID, const std::string& strData) = 0;
	virtual void SendBySuit(const VF_SERVER_TYPES eType, const std::string& strHashKey, const int nMsgID, const char* msg, const uint32_t nLen) = 0;
	virtual void SendBySuit(const VF_SERVER_TYPES eType, const int& nHashKey, const int nMsgID, const std::string& strData) = 0;
	virtual void SendBySuit(const VF_SERVER_TYPES eType, const int& nHashKey, const int nMsgID, const char* msg, const uint32_t nLen) = 0;
	virtual void SendSuitByPB(const VF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, google::protobuf::Message& xData) = 0;
	virtual void SendSuitByPB(const VF_SERVER_TYPES eType, const int& nHashKey, const uint16_t nMsgID, google::protobuf::Message& xData) = 0;

	////////////////////////////////////////////////////////////////////////////////

	virtual VFMapEx<int, ConnectData>& GetServerList() = 0;
	virtual VF_SHARE_PTR<ConnectData> GetServerNetInfo(const VF_SERVER_TYPES eType) = 0;
	virtual VF_SHARE_PTR<ConnectData> GetServerNetInfo(const int nServerID) = 0;
	virtual VF_SHARE_PTR<ConnectData> GetServerNetInfo(const VFINet* pNet) = 0;
};
#endif
