
// -------------------------------------------------------------------------
//    @FileName         :    VFCNetClientModule.cpp
//    @Author           :    WenZheng.Xu
//    @Date             :    2017-02-15
//    @Module           :    VFCNetClientModule
//
// -------------------------------------------------------------------------

#include "VFCNetModule.h"
#include "VFCNetClientModule.h"

VFCNetClientModule::VFCNetClientModule(VFIPluginManager * p)
{
	mnBufferSize = 0;
	pPluginManager = p;
}

bool VFCNetClientModule::Init()
{
	for (int i = 0; i < VF_SERVER_TYPES::VF_ST_MAX; ++i)
	{
		VFINetClientModule::AddEventCallBack((VF_SERVER_TYPES)i, this, &VFCNetClientModule::OnSocketEvent);
	}

	return true;
}

bool VFCNetClientModule::AfterInit()
{
	return true;
}

bool VFCNetClientModule::BeforeShut()
{
	return true;
}

bool VFCNetClientModule::Shut()
{
	return true;
}

bool VFCNetClientModule::Execute()
{
	ProcessExecute();
	ProcessAddNetConnect();

	return true;
}

void VFCNetClientModule::RemoveReceiveCallBack(const VF_SERVER_TYPES eType, const int nMsgID)
{
	VF_SHARE_PTR<CallBack> xCallBack = mxCallBack.GetElement(eType);
	if (xCallBack)
	{
		std::map<int, NET_RECEIVE_FUNCTOR_PTR>::iterator it = xCallBack->mxReceiveCallBack.find(nMsgID);
		if (xCallBack->mxReceiveCallBack.end() == it)
		{
			xCallBack->mxReceiveCallBack.erase(it);
		}
	}
}

void VFCNetClientModule::AddServer(const ConnectData & xInfo)
{
	mxTempNetList.push_back(xInfo);
}

int VFCNetClientModule::ExpandBufferSize(const unsigned int size)
{
	if (size > 0)
	{
		mnBufferSize = size;
	}
	return mnBufferSize;
}

int VFCNetClientModule::AddReceiveCallBack(const VF_SERVER_TYPES eType, const int nMsgID, NET_RECEIVE_FUNCTOR_PTR functorPtr)
{
	VF_SHARE_PTR<CallBack> xCallBack = mxCallBack.GetElement(eType);
	if (!xCallBack)
	{
		xCallBack = VF_SHARE_PTR<CallBack>(VF_NEW CallBack);
		mxCallBack.AddElement(eType, xCallBack);
	}

	xCallBack->mxReceiveCallBack.insert(std::map<int, NET_RECEIVE_FUNCTOR_PTR>::value_type(nMsgID, functorPtr));

	return 0;
}

int VFCNetClientModule::AddReceiveCallBack(const VF_SERVER_TYPES eType, NET_RECEIVE_FUNCTOR_PTR functorPtr)
{
	VF_SHARE_PTR<CallBack> xCallBack = mxCallBack.GetElement(eType);
	if (!xCallBack)
	{
		xCallBack = VF_SHARE_PTR<CallBack>(VF_NEW CallBack);
		mxCallBack.AddElement(eType, xCallBack);
	}

	xCallBack->mxCallBackList.push_back(functorPtr);

	return 0;
}


int VFCNetClientModule::AddEventCallBack(const VF_SERVER_TYPES eType, NET_EVENT_FUNCTOR_PTR functorPtr)
{
	VF_SHARE_PTR<CallBack> xCallBack = mxCallBack.GetElement(eType);
	if (!xCallBack)
	{
		xCallBack = VF_SHARE_PTR<CallBack>(VF_NEW CallBack);
		mxCallBack.AddElement(eType, xCallBack);
	}

	xCallBack->mxEventCallBack.push_back(functorPtr);
	return 0;
}

void VFCNetClientModule::SendByServerID(const int nServerID, const int nMsgID, const std::string & strData)
{
	SendByServerID(nServerID, nMsgID, strData.c_str(), strData.length());
}

void VFCNetClientModule::SendByServerID(const int nServerID, const int nMsgID, const char * msg, const uint32_t nLen)
{
	VF_SHARE_PTR<ConnectData> pServer = mxServerMap.GetElement(nServerID);
	if (pServer)
	{
		VF_SHARE_PTR<VFINetModule> pNetModule = pServer->mxNetModule;
		if (pNetModule.get())
		{
			pNetModule->GetNet()->SendMsgWithOutHead(nMsgID, msg, nLen, 0);
		}
	}
}

void VFCNetClientModule::SendToAllServer(const int nMsgID, const std::string & strData)
{
	VF_SHARE_PTR<ConnectData> pServer = mxServerMap.First();
	while (pServer)
	{
		VF_SHARE_PTR<VFINetModule> pNetModule = pServer->mxNetModule;
		if (pNetModule)
		{
			pNetModule->SendMsgWithOutHead(nMsgID, strData, 0);
		}

		pServer = mxServerMap.Next();
	}
}

void VFCNetClientModule::SendToAllServer(const VF_SERVER_TYPES eType, const int nMsgID, const std::string & strData)
{
	VF_SHARE_PTR<ConnectData> pServer = mxServerMap.First();
	while (pServer)
	{
		VF_SHARE_PTR<VFINetModule> pNetModule = pServer->mxNetModule;
		if (pNetModule && eType == pServer->eServerType)
		{
			pNetModule->SendMsgWithOutHead(nMsgID, strData, 0);
		}

		pServer = mxServerMap.Next();
	}
}

void VFCNetClientModule::SendToServerByPB(const int nServerID, const uint16_t nMsgID, google::protobuf::Message & xData)
{
	VF_SHARE_PTR<ConnectData> pServer = mxServerMap.GetElement(nServerID);
	if (pServer)
	{
		VF_SHARE_PTR<VFINetModule> pNetModule = pServer->mxNetModule;
		if (pNetModule.get())
		{
			pNetModule->SendMsgPB(nMsgID, xData, 0, VFGUID(), NULL);
		}
	}
}

void VFCNetClientModule::SendToAllServerByPB(const uint16_t nMsgID, google::protobuf::Message & xData)
{
	VF_SHARE_PTR<ConnectData> pServer = mxServerMap.First();
	while (pServer)
	{
		VF_SHARE_PTR<VFINetModule> pNetModule = pServer->mxNetModule;
		if (pNetModule)
		{
			pNetModule->SendMsgPB(nMsgID, xData, 0);
		}

		pServer = mxServerMap.Next();
	}
}

void VFCNetClientModule::SendToAllServerByPB(const VF_SERVER_TYPES eType, const uint16_t nMsgID, google::protobuf::Message & xData)
{
	VF_SHARE_PTR<ConnectData> pServer = mxServerMap.First();
	while (pServer)
	{
		VF_SHARE_PTR<VFINetModule> pNetModule = pServer->mxNetModule;
		if (pNetModule && eType == pServer->eServerType)
		{
			pNetModule->SendMsgPB(nMsgID, xData, 0);
		}

		pServer = mxServerMap.Next();
	}
}

void VFCNetClientModule::SendBySuit(const VF_SERVER_TYPES eType, const std::string & strHashKey, const int nMsgID, const std::string & strData)
{
	uint32_t nCRC32 = VFrame::CRC32(strHashKey);
	SendBySuit(eType, nCRC32, nMsgID, strData);
}

void VFCNetClientModule::SendBySuit(const VF_SERVER_TYPES eType, const std::string & strHashKey, const int nMsgID, const char * msg, const uint32_t nLen)
{
	uint32_t nCRC32 = VFrame::CRC32(strHashKey);
	SendBySuit(eType, nCRC32, nMsgID, msg, nLen);
}

void VFCNetClientModule::SendBySuit(const VF_SERVER_TYPES eType, const int & nHashKey, const int nMsgID, const std::string & strData)
{
	SendBySuit(eType, nHashKey, nMsgID, strData.c_str(), strData.length());
}

void VFCNetClientModule::SendBySuit(const VF_SERVER_TYPES eType, const int & nHashKey, const int nMsgID, const char * msg, const uint32_t nLen)
{
	VF_SHARE_PTR<VFCConsistentHashMapEx<int, ConnectData>> xConnectDataMap = mxServerTypeMap.GetElement(eType);
	if (xConnectDataMap)
	{
		VF_SHARE_PTR<ConnectData> pConnectData = xConnectDataMap->GetElementBySuit(nHashKey);
		if (pConnectData)
		{
			SendByServerID(pConnectData->nGameID, nMsgID, msg, nLen);
		}
	}
}

void VFCNetClientModule::SendSuitByPB(const VF_SERVER_TYPES eType, const std::string & strHashKey, const uint16_t nMsgID, google::protobuf::Message & xData)
{
	uint32_t nCRC32 = VFrame::CRC32(strHashKey);
	SendSuitByPB(eType, nCRC32, nMsgID, xData);
}

void VFCNetClientModule::SendSuitByPB(const VF_SERVER_TYPES eType, const int & nHashKey, const uint16_t nMsgID, google::protobuf::Message & xData)
{
	VF_SHARE_PTR<VFCConsistentHashMapEx<int, ConnectData>> xConnectDataMap = mxServerTypeMap.GetElement(eType);
	if (xConnectDataMap)
	{
		VF_SHARE_PTR<ConnectData> pConnectData = xConnectDataMap->GetElementBySuit(nHashKey);
		if (pConnectData)
		{
			SendToServerByPB(pConnectData->nGameID, nMsgID, xData);
		}
	}
}

VF_SHARE_PTR<ConnectData> VFCNetClientModule::GetServerNetInfo(const VF_SERVER_TYPES eType)
{
	VF_SHARE_PTR<VFCConsistentHashMapEx<int, ConnectData>> xConnectDataMap = mxServerTypeMap.GetElement(eType);
	if (xConnectDataMap)
	{
		return xConnectDataMap->GetElementBySuitRandom();
	}

	return nullptr;
}

VF_SHARE_PTR<ConnectData> VFCNetClientModule::GetServerNetInfo(const int nServerID)
{
	return mxServerMap.GetElement(nServerID);
}

VFMapEx<int, ConnectData>& VFCNetClientModule::GetServerList()
{
	return mxServerMap;
}

VF_SHARE_PTR<ConnectData> VFCNetClientModule::GetServerNetInfo(const VFINet * pNet)
{
	int nServerID = 0;
	for (VF_SHARE_PTR<ConnectData> pData = mxServerMap.First(nServerID); pData != NULL; pData = mxServerMap.Next(nServerID))
	{
		if (pData->mxNetModule && pNet == pData->mxNetModule->GetNet())
		{
			return pData;
		}
	}

	return VF_SHARE_PTR<ConnectData>(NULL);
}

void VFCNetClientModule::InitCallBacks(VF_SHARE_PTR<ConnectData> pServerData)
{
	VF_SHARE_PTR<CallBack> xCallBack = mxCallBack.GetElement(pServerData->eServerType);
	if (!xCallBack)
	{
		xCallBack = VF_SHARE_PTR<CallBack>(VF_NEW CallBack);
		mxCallBack.AddElement(pServerData->eServerType, xCallBack);
	}

	//add msg callback
	std::map<int, NET_RECEIVE_FUNCTOR_PTR>::iterator itReciveCB = xCallBack->mxReceiveCallBack.begin();
	for (; xCallBack->mxReceiveCallBack.end() != itReciveCB; ++itReciveCB)
	{
		pServerData->mxNetModule->AddReceiveCallBack(itReciveCB->first, itReciveCB->second);
	}

	//add event callback
	std::list<NET_EVENT_FUNCTOR_PTR>::iterator itEventCB = xCallBack->mxEventCallBack.begin();
	for (; xCallBack->mxEventCallBack.end() != itEventCB; ++itEventCB)
	{
		pServerData->mxNetModule->AddEventCallBack(*itEventCB);
	}

	std::list<NET_RECEIVE_FUNCTOR_PTR>::iterator itCB = xCallBack->mxCallBackList.begin();
	for (; xCallBack->mxCallBackList.end() != itCB; ++itCB)
	{
		pServerData->mxNetModule->AddReceiveCallBack(*itCB);
	}
}

void VFCNetClientModule::ProcessExecute()
{
	VF_SHARE_PTR<ConnectData> pServerData = mxServerMap.First();
	while (pServerData)
	{
		switch (pServerData->eState)
		{
		case ConnectDataState::DISCONNECT:
		{
			if (NULL != pServerData->mxNetModule)
			{
				pServerData->mxNetModule = nullptr;
				pServerData->eState = ConnectDataState::RECONNECT;
			}
		}
		break;
		case ConnectDataState::CONNECTING:
		{
			if (pServerData->mxNetModule)
			{
				pServerData->mxNetModule->Execute();
			}
		}
		break;
		case ConnectDataState::NORMAL:
		{
			if (pServerData->mxNetModule)
			{
				pServerData->mxNetModule->Execute();

				KeepState(pServerData);
			}
		}
		break;
		case ConnectDataState::RECONNECT:
		{
			if ((pServerData->mnLastActionTime + 10) >= GetPluginManager()->GetNowTime())
			{
				break;
			}

			if (nullptr != pServerData->mxNetModule)
			{
				pServerData->mxNetModule = nullptr;
			}

			pServerData->eState = ConnectDataState::CONNECTING;
			pServerData->mxNetModule = VF_SHARE_PTR<VFINetModule>(VF_NEW VFCNetModule(pPluginManager));
			pServerData->mxNetModule->Initialization(pServerData->strIP.c_str(), pServerData->nPort);

			InitCallBacks(pServerData);
		}
		break;
		default:
			break;
		}

		pServerData = mxServerMap.Next();
	}
}

void VFCNetClientModule::LogServerInfo()
{
	LogServerInfo("This is a client, begin to print Server Info----------------------------------");

	ConnectData* pServerData = mxServerMap.FirstNude();
	while (nullptr != pServerData)
	{
		std::ostringstream stream;
		stream << "Type: " << pServerData->eServerType << " ProxyServer ID: " << pServerData->nGameID << " State: " << pServerData->eState << " IP: " << pServerData->strIP << " Port: " << pServerData->nPort;

		LogServerInfo(stream.str());

		pServerData = mxServerMap.NextNude();
	}

	LogServerInfo("This is a client, end to print Server Info----------------------------------");
}
void VFCNetClientModule::KeepState(VF_SHARE_PTR<ConnectData> pServerData)
{
	if (pServerData->mnLastActionTime + 10 > GetPluginManager()->GetNowTime())
	{
		return;
	}

	pServerData->mnLastActionTime = GetPluginManager()->GetNowTime();

	KeepReport(pServerData);
	LogServerInfo();
}

void VFCNetClientModule::OnSocketEvent(const int fd, const VF_NET_EVENT eEvent, VFINet * pNet)
{
	if (eEvent & BEV_EVENT_CONNECTED)
	{
		OnConnected(fd, pNet);
	}
	else
	{
		OnDisConnected(fd, pNet);
	}
}

int VFCNetClientModule::OnConnected(const int fd, VFINet * pNet)
{
	VF_SHARE_PTR<ConnectData> pServerInfo = GetServerNetInfo(pNet);
	if (pServerInfo)
	{
		/////////////////////////////////////////////////////////////////////////////////////
		//AddServerWeightData(pServerInfo);
		pServerInfo->eState = ConnectDataState::NORMAL;

		//for type--suit
		VF_SHARE_PTR<VFCConsistentHashMapEx<int, ConnectData>> xConnectDataMap = mxServerTypeMap.GetElement(pServerInfo->eServerType);
		if (!xConnectDataMap)
		{
			xConnectDataMap = VF_SHARE_PTR<VFCConsistentHashMapEx<int, ConnectData>>(VF_NEW VFCConsistentHashMapEx<int, ConnectData>());
			mxServerTypeMap.AddElement(pServerInfo->eServerType, xConnectDataMap);
		}

		xConnectDataMap->AddElement(pServerInfo->nGameID, pServerInfo);
	}

	return 0;
}

int VFCNetClientModule::OnDisConnected(const int fd, VFINet * pNet)
{
	VF_SHARE_PTR<ConnectData> pServerInfo = GetServerNetInfo(pNet);
	if (nullptr != pServerInfo)
	{
		/////////////////////////////////////////////////////////////////////////////////////
		//RemoveServerWeightData(pServerInfo);
		pServerInfo->eState = ConnectDataState::DISCONNECT;
		pServerInfo->mnLastActionTime = GetPluginManager()->GetNowTime();

		//for type--suit
		VF_SHARE_PTR<VFCConsistentHashMapEx<int, ConnectData>> xConnectDataMap = mxServerTypeMap.GetElement(pServerInfo->eServerType);
		if (!xConnectDataMap)
		{
			xConnectDataMap = VF_SHARE_PTR<VFCConsistentHashMapEx<int, ConnectData>>(VF_NEW VFCConsistentHashMapEx<int, ConnectData>());
			mxServerTypeMap.AddElement(pServerInfo->eServerType, xConnectDataMap);
		}

		xConnectDataMap->RemoveElement(pServerInfo->nGameID);
	}

	return 0;
}

void VFCNetClientModule::ProcessAddNetConnect()
{
	std::list<ConnectData>::iterator it = mxTempNetList.begin();
	for (; it != mxTempNetList.end(); ++it)
	{
		const ConnectData& xInfo = *it;
		VF_SHARE_PTR<ConnectData> xServerData = mxServerMap.GetElement(xInfo.nGameID);
		if (nullptr == xServerData)
		{
			xServerData = VF_SHARE_PTR<ConnectData>(VF_NEW ConnectData());

			xServerData->nGameID = xInfo.nGameID;
			xServerData->eServerType = xInfo.eServerType;
			xServerData->strIP = xInfo.strIP;
			xServerData->strName = xInfo.strName;
			xServerData->eState = ConnectDataState::CONNECTING;
			xServerData->nPort = xInfo.nPort;
			xServerData->mnLastActionTime = GetPluginManager()->GetNowTime();

			xServerData->mxNetModule = VF_SHARE_PTR<VFINetModule>(VF_NEW VFCNetModule(pPluginManager));
			xServerData->mxNetModule->Initialization(xServerData->strIP.c_str(), xServerData->nPort);
			xServerData->mxNetModule->ExpandBufferSize(mnBufferSize);

			InitCallBacks(xServerData);

			mxServerMap.AddElement(xInfo.nGameID, xServerData);
		}
	}

	mxTempNetList.clear();
}