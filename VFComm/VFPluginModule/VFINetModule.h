// -------------------------------------------------------------------------
//    @FileName         :    VFINetModule.hpp
//    @Author           :    WenZheng.Xu
//    @Date             :    2015-01-4
//    @Module           :    VFINetModule
//
// -------------------------------------------------------------------------

#ifndef VFI_NET_MODULE_H
#define VFI_NET_MODULE_H

#include <iostream>
#include <iosfwd>
#include "VFGUID.h"
#include "VFIModule.h"
#include "VFIPluginManager.h"
#include "VFComm/VFNetPlugin/VFCNet.h"
#include "VFComm/VFCore/VFQueue.hpp"
#include "VFComm/VFMessageDefine/VFMsgDefine.h"
#include "VFComm/VFMessageDefine/VFDefine.pb.h"

enum VF_SERVER_TYPES
{
    VF_ST_NONE          = 0,    // NONE
    VF_ST_REDIS         = 1,    //
    VF_ST_MYSQL         = 2,    //
    VF_ST_MASTER        = 3,    //
    VF_ST_LOGIN         = 4,    //
    VF_ST_PROXY         = 5,    //
    VF_ST_GAME          = 6,    //
    VF_ST_WORLD         = 7,    //
	VF_ST_MAX			= 8,    //

};


////////////////////////////////////////////////////////////////////////////

//only use this macro when u has entered game server
#define CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msgData, nLen, msg)                 \
    VFGUID nPlayerID;                                \
    msg xMsg;                                           \
    if (!VFINetModule::ReceivePB(nSockIndex, nMsgID, msgData, nLen, xMsg, nPlayerID))             \
    {                                                   \
        /*m_pLogModule->LogNormal(VFILogModule::NLL_ERROR_NORMAL, VFGUID(), "", "Parse msg error", __FUNCTION__, __LINE__);*/ \
        return;                                         \
    }                                                   \
    \
    VF_SHARE_PTR<VFIObject> pObject = m_pKernelModule->GetObject(nPlayerID); \
    if ( NULL == pObject.get() )                        \
    {                                                   \
        /*m_pLogModule->LogNormal(VFILogModule::NLL_ERROR_NORMAL, nPlayerID, "FromClient Object do not Exist", "", __FUNCTION__, __LINE__);*/ \
        return;                                         \
    }

#define CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msgData, nLen, msg)                 \
    VFGUID nPlayerID;                                \
    msg xMsg;                                           \
    if (!VFINetModule::ReceivePB(nSockIndex, nMsgID, msgData, nLen, xMsg, nPlayerID))             \
    {                                                   \
        m_pLogModule->LogNormal(VFILogModule::NLL_ERROR_NORMAL, VFGUID(), "", "Parse msg error", __FUNCTION__, __LINE__); \
        return;                                         \
    }

//////////////////////////////////////////////////////////////////////////
struct ServerData
{
    ServerData()
    {
        pData = VF_SHARE_PTR<VFMsg::ServerInfoReport>(VF_NEW VFMsg::ServerInfoReport());
        nFD = 0;
    }
    ~ServerData()
    {
        nFD = 0;
        pData = NULL;
    }

    int nFD;
    VF_SHARE_PTR<VFMsg::ServerInfoReport> pData;
};

class VFINetModule
    : public VFIModule
{
public:
	static VFGUID PBToVF(VFMsg::Ident xID)
	{
		VFGUID  xIdent;
		xIdent.nHead64 = xID.svrid();
		xIdent.nData64 = xID.index();

		return xIdent;
	}

	static VFVector2 PBToVF(VFMsg::Vector2 value)
	{
		VFVector2  vector;
		vector.SetX(value.x());
		vector.SetY(value.y());
		return vector;
	}

	static VFVector3 PBToVF(VFMsg::Vector3 value)
	{
		VFVector3  vector;
		vector.SetX(value.x());
		vector.SetY(value.y());
		vector.SetZ(value.z());
		return vector;
	}

	static VFMsg::Ident VFToPB(VFGUID xID)
	{
		VFMsg::Ident  xIdent;
		xIdent.set_svrid(xID.nHead64);
		xIdent.set_index(xID.nData64);

		return xIdent;
	}

	static VFMsg::Vector2 VFToPB(VFVector2 value)
	{
		VFMsg::Vector2  vector;
		vector.set_x(value.X());
		vector.set_y(value.Y());
		return vector;
	}

	static VFMsg::Vector3 VFToPB(VFVector3 value)
	{
		VFMsg::Vector3  vector;
		vector.set_x(value.X());
		vector.set_y(value.Y());
		vector.set_z(value.Z());
		return vector;
	}

	template<typename BaseType>
	bool AddReceiveCallBack(const int nMsgID, BaseType* pBase, void (BaseType::*handleRecieve)(const int, const int, const char*, const uint32_t))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		NET_RECEIVE_FUNCTOR_PTR functorPtr(new NET_RECEIVE_FUNCTOR(functor));

		return AddReceiveCallBack(nMsgID, functorPtr);
	}

	template<typename BaseType>
	bool AddReceiveCallBack(BaseType* pBase, void (BaseType::*handleRecieve)(const int, const int, const char*, const uint32_t))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		NET_RECEIVE_FUNCTOR_PTR functorPtr(new NET_RECEIVE_FUNCTOR(functor));

		return AddReceiveCallBack(functorPtr);
	}

	template<typename BaseType>
	bool AddEventCallBack(BaseType* pBase, void (BaseType::*handler)(const int, const VF_NET_EVENT, VFINet*))
	{
		NET_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NET_EVENT_FUNCTOR_PTR functorPtr(new NET_EVENT_FUNCTOR(functor));

		return AddEventCallBack(functorPtr);
	}

	static bool ReceivePB(const int nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen, std::string & strMsg, VFGUID & nPlayer)
	{
		VFMsg::MsgBase xMsg;
		if (!xMsg.ParseFromArray(msg, nLen))
		{
			char szData[MAX_PATH] = { 0 };
			VFSPRINTF(szData, MAX_PATH, "Parse Message Failed from Packet to MsgBase, MessageID: %d\n", nMsgID);
			//LogRecive(szData);

			return false;
		}

		strMsg.assign(xMsg.msg_data().data(), xMsg.msg_data().length());

		nPlayer = PBToVF(xMsg.player_id());

		return true;
	}

	static bool ReceivePB(const int nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen, google::protobuf::Message & xData, VFGUID & nPlayer)
	{
		VFMsg::MsgBase xMsg;
		if (!xMsg.ParseFromArray(msg, nLen))
		{
			char szData[MAX_PATH] = { 0 };
			VFSPRINTF(szData, MAX_PATH, "Parse Message Failed from Packet to MsgBase, MessageID: %d\n", nMsgID);
			//LogRecive(szData);

			return false;
		}

		if (!xData.ParseFromString(xMsg.msg_data()))
		{
			char szData[MAX_PATH] = { 0 };
			VFSPRINTF(szData, MAX_PATH, "Parse Message Failed from MsgData to ProtocolData, MessageID: %d\n", nMsgID);
			//LogRecive(szData);

			return false;
		}

		nPlayer = PBToVF(xMsg.player_id());

		return true;
	}

	/////////////////
	//as client
	virtual void Initialization(const char* strIP, const unsigned short nPort) = 0;

	//as server
	virtual int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4) = 0;
	virtual int ExpandBufferSize(const unsigned int size = 1024 * 1024 * 20) = 0;

	virtual void RemoveReceiveCallBack(const int nMsgID) = 0;

	virtual bool AddReceiveCallBack(const int nMsgID, const NET_RECEIVE_FUNCTOR_PTR& cb) = 0;

	virtual bool AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR& cb) = 0;

	virtual bool AddEventCallBack(const NET_EVENT_FUNCTOR_PTR& cb) = 0;

	virtual bool Execute() = 0;


	virtual bool SendMsgWithOutHead(const int nMsgID, const std::string& msg, const int nSockIndex) = 0;

	virtual bool SendMsgToAllClientWithOutHead(const int nMsgID, const std::string& msg) = 0;

	virtual bool SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message& xData, const uint32_t nSockIndex) = 0;

	virtual bool SendMsgPBToAllClient(const uint16_t nMsgID, const google::protobuf::Message& xData) = 0;

	virtual bool SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message& xData, const uint32_t nSockIndex, const VFGUID nPlayer, const std::vector<VFGUID>* pClientIDList = NULL) = 0;

	virtual bool SendMsgPB(const uint16_t nMsgID, const std::string& strData, const uint32_t nSockIndex, const VFGUID nPlayer, const std::vector<VFGUID>* pClientIDList = NULL) = 0;

	virtual VFINet* GetNet() = 0;
};

#endif
