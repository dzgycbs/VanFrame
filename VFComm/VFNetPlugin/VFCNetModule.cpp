#include "VFCNetModule.h"
// -------------------------------------------------------------------------
//    @FileName         :    VFCNetModule.cpp
//    @Author           :    WenZheng.Xu
//    @Date             :    2017-02-15
//    @Module           :    VFCNetModule
//
// -------------------------------------------------------------------------

VFCNetModule::VFCNetModule(VFIPluginManager * p)
{
	pPluginManager = p;

	mnBufferSize = 0;
	nLastTime = GetPluginManager()->GetNowTime();
	m_pNet = NULL;
}

VFCNetModule::~VFCNetModule()
{
	if (m_pNet)
	{
		m_pNet->Final();
	}

	delete m_pNet;
	m_pNet = NULL;
}
void VFCNetModule::Initialization(const char * strIP, const unsigned short nPort)
{
	m_pNet = VF_NEW VFCNet(this, &VFCNetModule::OnReceiveNetPack, &VFCNetModule::OnSocketNetEvent);
	m_pNet->ExpandBufferSize(mnBufferSize);
	m_pNet->Initialization(strIP, nPort);
}

int VFCNetModule::Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount)
{
	m_pNet = VF_NEW VFCNet(this, &VFCNetModule::OnReceiveNetPack, &VFCNetModule::OnSocketNetEvent);
	m_pNet->ExpandBufferSize(mnBufferSize);
	return m_pNet->Initialization(nMaxClient, nPort, nCpuCount);
}

int VFCNetModule::ExpandBufferSize(const unsigned int size)
{
	if (size > 0)
	{
		mnBufferSize = size;
		if (m_pNet)
		{
			m_pNet->ExpandBufferSize(mnBufferSize);
		}
	}

	return mnBufferSize;
}

void VFCNetModule::RemoveReceiveCallBack(const int nMsgID)
{
	std::map<int, NET_RECEIVE_FUNCTOR_PTR>::iterator it = mxReceiveCallBack.find(nMsgID);
	if (mxReceiveCallBack.end() != it)
	{
		mxReceiveCallBack.erase(it);
	}
}

bool VFCNetModule::AddReceiveCallBack(const int nMsgID, const NET_RECEIVE_FUNCTOR_PTR & cb)
{
	if (mxReceiveCallBack.find(nMsgID) != mxReceiveCallBack.end())
	{
		return false;
	}

	mxReceiveCallBack.insert(std::map<int, NET_RECEIVE_FUNCTOR_PTR>::value_type(nMsgID, cb));

	return true;
}

bool VFCNetModule::AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR & cb)
{
	mxCallBackList.push_back(cb);

	return true;
}

bool VFCNetModule::AddEventCallBack(const NET_EVENT_FUNCTOR_PTR & cb)
{
	mxEventCallBackList.push_back(cb);

	return true;
}

bool VFCNetModule::Execute()
{
	if (!m_pNet)
	{
		return false;
	}


	KeepAlive();

	return m_pNet->Execute();
}

bool VFCNetModule::SendMsgWithOutHead(const int nMsgID, const std::string & msg, const int nSockIndex)
{
	return m_pNet->SendMsgWithOutHead(nMsgID, msg.c_str(), msg.length(), nSockIndex);
}

bool VFCNetModule::SendMsgToAllClientWithOutHead(const int nMsgID, const std::string & msg)
{
	return m_pNet->SendMsgToAllClientWithOutHead(nMsgID, msg.c_str(), msg.length());
}

bool VFCNetModule::SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message & xData, const uint32_t nSockIndex)
{
	VFMsg::MsgBase xMsg;
	if (!xData.SerializeToString(xMsg.mutable_msg_data()))
	{
		char szData[MAX_PATH] = { 0 };
		VFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgData, MessageID: %d\n", nSockIndex, nMsgID);

		return false;
	}

	VFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
	*pPlayerID = VFToPB(VFGUID());

	std::string strMsg;
	if (!xMsg.SerializeToString(&strMsg))
	{
		char szData[MAX_PATH] = { 0 };
		VFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgBase, MessageID: %d\n", nSockIndex, nMsgID);

		return false;
	}

	SendMsgWithOutHead(nMsgID, strMsg, nSockIndex);

	return true;
}

bool VFCNetModule::SendMsgPBToAllClient(const uint16_t nMsgID, const google::protobuf::Message & xData)
{
	VFMsg::MsgBase xMsg;
	if (!xData.SerializeToString(xMsg.mutable_msg_data()))
	{
		char szData[MAX_PATH] = { 0 };
		VFSPRINTF(szData, MAX_PATH, "Send Message to all Failed For Serialize of MsgData, MessageID: %d\n", nMsgID);

		return false;
	}

	VFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
	*pPlayerID = VFToPB(VFGUID());

	std::string strMsg;
	if (!xMsg.SerializeToString(&strMsg))
	{
		char szData[MAX_PATH] = { 0 };
		VFSPRINTF(szData, MAX_PATH, "Send Message to all Failed For Serialize of MsgBase, MessageID: %d\n", nMsgID);

		return false;
	}

	return SendMsgToAllClientWithOutHead(nMsgID, strMsg);
}

bool VFCNetModule::SendMsgPB(const uint16_t nMsgID, const google::protobuf::Message & xData, const uint32_t nSockIndex, const VFGUID nPlayer, const std::vector<VFGUID>* pClientIDList)
{
	if (!m_pNet)
	{
		char szData[MAX_PATH] = { 0 };
		VFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For NULL Of Net, MessageID: %d\n", nSockIndex, nMsgID);

		return false;
	}

	VFMsg::MsgBase xMsg;
	if (!xData.SerializeToString(xMsg.mutable_msg_data()))
	{
		char szData[MAX_PATH] = { 0 };
		VFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgData, MessageID: %d\n", nSockIndex, nMsgID);

		return false;
	}


	VFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
	*pPlayerID = VFToPB(nPlayer);
	if (pClientIDList)
	{
		for (int i = 0; i < pClientIDList->size(); ++i)
		{
			const VFGUID& ClientID = (*pClientIDList)[i];

			VFMsg::Ident* pData = xMsg.add_player_client_list();
			if (pData)
			{
				*pData = VFToPB(ClientID);
			}
		}
	}

	std::string strMsg;
	if (!xMsg.SerializeToString(&strMsg))
	{
		char szData[MAX_PATH] = { 0 };
		VFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgBase, MessageID: %d\n", nSockIndex, nMsgID);

		return false;
	}

	return SendMsgWithOutHead(nMsgID, strMsg, nSockIndex);
}

bool VFCNetModule::SendMsgPB(const uint16_t nMsgID, const std::string & strData, const uint32_t nSockIndex, const VFGUID nPlayer, const std::vector<VFGUID>* pClientIDList)
{
	if (!m_pNet)
	{
		char szData[MAX_PATH] = { 0 };
		VFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For NULL Of Net, MessageID: %d\n", nSockIndex, nMsgID);
		//LogSend(szData);

		return false;
	}

	VFMsg::MsgBase xMsg;
	xMsg.set_msg_data(strData.data(), strData.length());


	VFMsg::Ident* pPlayerID = xMsg.mutable_player_id();
	*pPlayerID = VFToPB(nPlayer);
	if (pClientIDList)
	{
		for (int i = 0; i < pClientIDList->size(); ++i)
		{
			const VFGUID& ClientID = (*pClientIDList)[i];

			VFMsg::Ident* pData = xMsg.add_player_client_list();
			if (pData)
			{
				*pData = VFToPB(ClientID);
			}
		}
	}

	std::string strMsg;
	if (!xMsg.SerializeToString(&strMsg))
	{
		char szData[MAX_PATH] = { 0 };
		VFSPRINTF(szData, MAX_PATH, "Send Message to %d Failed For Serialize of MsgBase, MessageID: %d\n", nSockIndex, nMsgID);
		//LogSend(szData);

		return false;
	}

	return SendMsgWithOutHead(nMsgID, strMsg, nSockIndex);
}

VFINet * VFCNetModule::GetNet()
{
	return m_pNet;
}

void VFCNetModule::OnReceiveNetPack(const int nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	std::map<int, NET_RECEIVE_FUNCTOR_PTR>::iterator it = mxReceiveCallBack.find(nMsgID);
	if (mxReceiveCallBack.end() != it)
	{
		NET_RECEIVE_FUNCTOR_PTR& pFunPtr = it->second;
		NET_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(nSockIndex, nMsgID, msg, nLen);
	}
	else
	{
		for (std::list<NET_RECEIVE_FUNCTOR_PTR>::iterator it = mxCallBackList.begin(); it != mxCallBackList.end(); ++it)
		{
			NET_RECEIVE_FUNCTOR_PTR& pFunPtr = *it;
			NET_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
			pFunc->operator()(nSockIndex, nMsgID, msg, nLen);
		}
	}
}
void VFCNetModule::OnSocketNetEvent(const int nSockIndex, const VF_NET_EVENT eEvent, VFINet * pNet)
{
	for (std::list<NET_EVENT_FUNCTOR_PTR>::iterator it = mxEventCallBackList.begin(); it != mxEventCallBackList.end(); ++it)
	{
		NET_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		NET_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(nSockIndex, eEvent, pNet);
	}
}

void VFCNetModule::KeepAlive()
{
	if (!m_pNet)
	{
		return;
	}

	if (m_pNet->IsServer())
	{
		return;
	}

	if (nLastTime + 10 > GetPluginManager()->GetNowTime())
	{
		return;
	}

	nLastTime = GetPluginManager()->GetNowTime();

	VFMsg::ServerHeartBeat xMsg;
	xMsg.set_count(0);

	SendMsgPB(VFMsg::EGameMsgID::EGMI_STS_HEART_BEAT, xMsg, 0);

}