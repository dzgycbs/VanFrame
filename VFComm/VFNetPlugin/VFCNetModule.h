// -------------------------------------------------------------------------
//    @FileName         :    VFCNetModule.h
//    @Author           :    WenZheng.Xu
//    @Date             :    2017-02-15
//    @Module           :    VFCNetModule
//
// -------------------------------------------------------------------------

#ifndef VFC_NET_MODULE_H
#define VFC_NET_MODULE_H

#include <iostream>
#include <iosfwd>
#include "VFINet.h"
#include "VFComm/VFCore/VFQueue.hpp"
#include "VFComm/VFPluginModule/VFINetModule.h"
#include "VFComm/VFPluginModule/VFIPluginManager.h"
#include "VFComm/VFMessageDefine/VFMsgDefine.h"
#include "VFComm/VFMessageDefine/VFDefine.pb.h"

class VFCNetModule
	: public VFINetModule
{
public:
	VFCNetModule(VFIPluginManager* p);

	virtual ~VFCNetModule();

	//as client
	virtual void Initialization(const char* strIP, const unsigned short nPort);

	//as server
	virtual int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4);
	virtual int ExpandBufferSize(const unsigned int size = 1024 * 1024 * 20);

	virtual void RemoveReceiveCallBack(const int nMsgID);

	virtual bool AddReceiveCallBack(const int nMsgID, const NET_RECEIVE_FUNCTOR_PTR& cb);

	virtual bool AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR& cb);

	virtual bool AddEventCallBack(const NET_EVENT_FUNCTOR_PTR& cb);

	virtual bool Execute();

	
	virtual bool SendMsgWithOutHead(const int nMsgID, const std::string& msg, const int nSockIndex);

	virtual bool SendMsgToAllClientWithOutHead(const int nMsgID, const std::string& msg);

	virtual bool SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message& xData, const uint32_t nSockIndex);

	virtual bool SendMsgPBToAllClient(const uint16_t nMsgID, const google::protobuf::Message& xData);

	virtual bool SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message& xData, const uint32_t nSockIndex, const VFGUID nPlayer, const std::vector<VFGUID>* pClientIDList = NULL);

	virtual bool SendMsgPB(const uint16_t nMsgID, const std::string& strData, const uint32_t nSockIndex, const VFGUID nPlayer, const std::vector<VFGUID>* pClientIDList = NULL);

	virtual VFINet* GetNet();

protected:
	void OnReceiveNetPack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	void OnSocketNetEvent(const int nSockIndex, const VF_NET_EVENT eEvent, VFINet* pNet);

	void KeepAlive();

private:

	int mnBufferSize;
	VFINet* m_pNet;
	VFINT64 nLastTime;
	std::map<int, NET_RECEIVE_FUNCTOR_PTR> mxReceiveCallBack;
	std::list<NET_EVENT_FUNCTOR_PTR> mxEventCallBackList;
	std::list<NET_RECEIVE_FUNCTOR_PTR> mxCallBackList;
};
#endif