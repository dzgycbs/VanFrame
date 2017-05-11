// -------------------------------------------------------------------------
//    @FileName         :    VFCNetClientModule.h
//    @Author           :    WenZheng.Xu
//    @Date             :    2017-02-15
//    @Module           :    VFCNetClientModule
//
// -------------------------------------------------------------------------

#ifndef VFC_NET_CLIENT_MODULE_H
#define VFC_NET_CLIENT_MODULE_H

#include <iostream>
#include <iosfwd>
#include "VFCNet.h"
#include "VFComm/VFCore/VFQueue.hpp"
#include "VFComm/VFCore/VFCConsistentHash.hpp"
#include "VFComm/VFMessageDefine/VFMsgDefine.h"
#include "VFComm/VFMessageDefine/VFDefine.pb.h"
#include "VFComm/VFPluginModule/VFINetClientModule.h"
#include "VFComm/VFPluginModule/VFINetModule.h"
#include "VFComm/VFPluginModule/VFIPluginManager.h"

class VFCNetClientModule : public VFINetClientModule
{
public:
	VFCNetClientModule(VFIPluginManager* p);

	virtual bool Init();
	virtual bool AfterInit();
	virtual bool BeforeShut();
	virtual bool Shut();
	virtual bool Execute();

	virtual void AddServer(const ConnectData& xInfo);
	virtual int ExpandBufferSize(const unsigned int size = 1024 * 1024 * 20);

	virtual int AddReceiveCallBack(const VF_SERVER_TYPES eType, NET_RECEIVE_FUNCTOR_PTR functorPtr);
	virtual int AddReceiveCallBack(const VF_SERVER_TYPES eType, const int nMsgID, NET_RECEIVE_FUNCTOR_PTR functorPtr);
	virtual int AddEventCallBack(const VF_SERVER_TYPES eType, NET_EVENT_FUNCTOR_PTR functorPtr);

	virtual void RemoveReceiveCallBack(const VF_SERVER_TYPES eType, const int nMsgID);

	////////////////////////////////////////////////////////////////////////////////
	virtual void SendByServerID(const int nServerID, const int nMsgID, const std::string& strData);
	virtual void SendByServerID(const int nServerID, const int nMsgID, const char* msg, const uint32_t nLen);

	virtual void SendToAllServer(const int nMsgID, const std::string& strData);
	virtual void SendToAllServer(const VF_SERVER_TYPES eType, const int nMsgID, const std::string& strData);

	virtual void SendToServerByPB(const int nServerID, const uint16_t nMsgID, google::protobuf::Message& xData);

	virtual void SendToAllServerByPB(const uint16_t nMsgID, google::protobuf::Message& xData);
	virtual void SendToAllServerByPB(const VF_SERVER_TYPES eType, const uint16_t nMsgID, google::protobuf::Message& xData);

	////////////////////////////////////////////////////////////////////////////////

	virtual void SendBySuit(const VF_SERVER_TYPES eType, const std::string& strHashKey, const int nMsgID, const std::string& strData);
	virtual void SendBySuit(const VF_SERVER_TYPES eType, const std::string& strHashKey, const int nMsgID, const char* msg, const uint32_t nLen);
	virtual void SendBySuit(const VF_SERVER_TYPES eType, const int& nHashKey, const int nMsgID, const std::string& strData);
	//
	virtual void SendBySuit(const VF_SERVER_TYPES eType, const int& nHashKey, const int nMsgID, const char* msg, const uint32_t nLen);
	virtual void SendSuitByPB(const VF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, google::protobuf::Message& xData);
	virtual void SendSuitByPB(const VF_SERVER_TYPES eType, const int& nHashKey, const uint16_t nMsgID, google::protobuf::Message& xData);

	////////////////////////////////////////////////////////////////////////////////

	virtual VFMapEx<int, ConnectData>& GetServerList();
	virtual VF_SHARE_PTR<ConnectData> GetServerNetInfo(const VF_SERVER_TYPES eType);
	virtual VF_SHARE_PTR<ConnectData> GetServerNetInfo(const int nServerID);
	virtual VF_SHARE_PTR<ConnectData> GetServerNetInfo(const VFINet* pNet);

protected:

	void InitCallBacks(VF_SHARE_PTR<ConnectData> pServerData);
	void ProcessExecute();

	void KeepReport(VF_SHARE_PTR<ConnectData> pServerData) {};
	void LogServerInfo(const std::string& strServerInfo) {};

private:
	void LogServerInfo();

	void KeepState(VF_SHARE_PTR<ConnectData> pServerData);

	void OnSocketEvent(const int fd, const VF_NET_EVENT eEvent, VFINet* pNet);

	int OnConnected(const int fd, VFINet* pNet);

	int OnDisConnected(const int fd, VFINet* pNet);

	void ProcessAddNetConnect();

private:
	int mnBufferSize;
	//server_id, server_data
	VFCConsistentHashMapEx<int, ConnectData> mxServerMap;
	//server_type, server_id, server_data
	VFMapEx<int, VFCConsistentHashMapEx<int, ConnectData>> mxServerTypeMap;

	std::list<ConnectData> mxTempNetList;

	struct CallBack
	{
		//call back
		std::map<int, NET_RECEIVE_FUNCTOR_PTR> mxReceiveCallBack;
		std::list<NET_EVENT_FUNCTOR_PTR> mxEventCallBack;
		std::list<NET_RECEIVE_FUNCTOR_PTR> mxCallBackList;
	};

	VFMapEx<int, CallBack> mxCallBack;
};
#endif
