// -------------------------------------------------------------------------
//    @FileName			:    VFIWebsocketModule.h
//    @Author           :    Stone.xin<xinxinst@163.com>
//    @Date             :    2016-12-22
//    @Module           :    VFIWebsocketModule
//    @Desc             :
// -------------------------------------------------------------------------
#ifndef VFI_WEBSOCKET_MODULE_H__
#define VFI_WEBSOCKET_MODULE_H__

#include "VFComm/VFNetPlugin/VFIWS.h"
#include "VFIModule.h"

class VFIWebsocketModule : public VFIModule
{
public:
	template<typename BaseType>
	void SetReceiveCallBack(BaseType* pBaseType, void (BaseType::*handleRecieve)(websocketpp::connection_hdl, const std::string&))
	{
		VF_WS_MSG_CALL_BACK functor = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2);
		VF_WS_MSG_CALL_BACK_PTR functorPtr(new VF_WS_MSG_CALL_BACK(functor));

		SetReceiveCallBack(functorPtr);
	}

	template<typename BaseType>
	void SetEventCallBack(BaseType* pBaseType, void (BaseType::*handleEvent)(websocketpp::connection_hdl, VF_WS_EVENT))
	{
		VF_WS_EVENT_CALL_BACK functor = std::bind(handleEvent, pBaseType, std::placeholders::_1, std::placeholders::_2);
		VF_WS_EVENT_CALL_BACK_PTR functorPtr(new VF_WS_EVENT_CALL_BACK(functor));

		SetEventCallBack(functorPtr);
	}
	
	virtual int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4) = 0;

	virtual void SetReceiveCallBack(VF_WS_MSG_CALL_BACK_PTR functorPtr) = 0;
	virtual void SetEventCallBack(VF_WS_EVENT_CALL_BACK_PTR functorPtr) = 0;
};

#endif // VFI_WEBSOCKET_MODULE_H__