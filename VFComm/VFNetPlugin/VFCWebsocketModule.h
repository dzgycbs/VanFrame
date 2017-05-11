// -------------------------------------------------------------------------
//    @FileName			:    VFCWebsocketModule.h
//    @Author           :    Stone.xin<xinxinst@163.com>
//    @Date             :    2016-12-22
//    @Module           :    VFCWebsocketModule
//    @Desc             :
// -------------------------------------------------------------------------
#ifndef VFC_WEBSOCKET_MODULE_H
#define VFC_WEBSOCKET_MODULE_H

#include "VFCWS.h"
#include "VFComm/VFPluginModule/VFIWebsocketModule.h"


class VFCWebsocketModule : public VFIWebsocketModule
{
public:
	VFCWebsocketModule(VFIPluginManager* p);

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Shut() override;

	virtual bool BeforeShut() override;

	virtual bool Execute() override;
	
	virtual void SetReceiveCallBack(VF_WS_MSG_CALL_BACK_PTR functorPtr) override;
	virtual void SetEventCallBack(VF_WS_EVENT_CALL_BACK_PTR functorPtr) override;

	virtual int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4) override;

private:
	void OnWebsocketMessage(websocketpp::connection_hdl, const std::string&);
	void OnWebsocketEvent(websocketpp::connection_hdl, VF_WS_EVENT);

private:
	VFCWS*					m_pWSServer;	
	VF_WS_MSG_CALL_BACK_PTR	m_pRecvMsgCB;
	VF_WS_EVENT_CALL_BACK_PTR m_pEvtCB;

};

#endif