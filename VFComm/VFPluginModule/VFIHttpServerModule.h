#ifndef VFI_HTTPNET_MODULE_H
#define VFI_HTTPNET_MODULE_H

#include "VFComm/VFPluginModule/VFIModule.h"
#include "VFComm/VFNetPlugin/VFIHttpNet.h"

class VFIHttpServerModule
	: public VFIModule
{

public:
	virtual ~VFIHttpServerModule() {};	
	// register msg callback
	template<typename BaseType>
	bool AddReceiveCallBack(const std::string& strCommand, BaseType* pBase, void (BaseType::*handleRecieve)(struct evhttp_request *req, const std::string& strCommand, const std::string& strUrl))
	{
		HTTPNET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		HTTPNET_RECEIVE_FUNCTOR_PTR functorPtr(new HTTPNET_RECEIVE_FUNCTOR(functor));
		return AddMsgCB(strCommand, functorPtr);
	}
	template<typename BaseType>
	bool AddNetCommonReceiveCallBack(BaseType* pBase, void (BaseType::*handleRecieve)(struct evhttp_request *req, const std::string& strCommand, const std::string& strUrl))
	{
		HTTPNET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		HTTPNET_RECEIVE_FUNCTOR_PTR functorPtr(new HTTPNET_RECEIVE_FUNCTOR(functor));

		return AddComMsgCB(functorPtr);
	}
	
public:
	virtual bool Execute() = 0;
	virtual int InitServer(const unsigned short nPort) = 0;	
	virtual bool AddMsgCB(const std::string& strCommand, const HTTPNET_RECEIVE_FUNCTOR_PTR& cb) = 0;	
	virtual bool AddComMsgCB(const HTTPNET_RECEIVE_FUNCTOR_PTR& cb) = 0;	
	virtual VFIHttpNet* GetHttpNet() = 0;
};

#endif
