// -------------------------------------------------------------------------
//    @FileName			:    VFCWebsocketModule.cpp
//    @Author           :    Stone.xin<xinxinst@163.com>
//    @Date             :    2016-12-22
//    @Module           :    VFCWebsocketModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "VFCWebsocketModule.h"

VFCWebsocketModule::VFCWebsocketModule(VFIPluginManager * p)
{
	pPluginManager = p;
}

bool VFCWebsocketModule::Init()
{
	m_pWSServer = new VFCWS(this, &VFCWebsocketModule::OnWebsocketMessage, &VFCWebsocketModule::OnWebsocketEvent);

	return true;
}

bool VFCWebsocketModule::AfterInit()
{
	return true;
}

bool VFCWebsocketModule::Shut()
{
	return true;
}

bool VFCWebsocketModule::BeforeShut()
{
	return true;
}

bool VFCWebsocketModule::Execute()
{
	m_pWSServer->Execute();
	return true;
}

void VFCWebsocketModule::SetReceiveCallBack(VF_WS_MSG_CALL_BACK_PTR functorPtr)
{
	m_pRecvMsgCB = functorPtr;
}

void VFCWebsocketModule::SetEventCallBack(VF_WS_EVENT_CALL_BACK_PTR functorPtr)
{
	m_pEvtCB = functorPtr;
}

int VFCWebsocketModule::Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount /*= 4*/)
{
	return m_pWSServer->Initialization(nMaxClient,nPort,nCpuCount);
}

void VFCWebsocketModule::OnWebsocketMessage(websocketpp::connection_hdl hd, const std::string & strPayload)
{
	if (m_pRecvMsgCB.get())
	{
		m_pRecvMsgCB->operator()(hd, strPayload);
	}
}

void VFCWebsocketModule::OnWebsocketEvent(websocketpp::connection_hdl hd, VF_WS_EVENT evt)
{
	if (m_pEvtCB.get())
	{
		m_pEvtCB->operator()(hd, evt);
	}
}