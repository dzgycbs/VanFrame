#ifndef VFC_HTTPNET_MODULE_H
#define VFC_HTTPNET_MODULE_H

#include <iostream>
#include "VFComm/VFNetPlugin/VFCHttpNet.h"
#include "VFComm/VFPluginModule/VFIHttpServerModule.h"
#include "VFComm/VFPluginModule/VFILogModule.h"

class VFCHttpServerModule
	: public VFIHttpServerModule
{

public:
	VFCHttpServerModule(VFIPluginManager* p);
	virtual ~VFCHttpServerModule();
	
public:
	virtual bool Execute();

	virtual int InitServer(const unsigned short nPort);
	
	void OnReceiveNetPack(struct evhttp_request *req, const std::string& strCommand, const std::string& strUrl);
	
	virtual bool AddMsgCB(const std::string& strCommand, const HTTPNET_RECEIVE_FUNCTOR_PTR& cb);
	
	virtual bool AddComMsgCB(const HTTPNET_RECEIVE_FUNCTOR_PTR& cb);
	
	virtual VFIHttpNet* GetHttpNet();

private:
	VFILogModule* mLogModule;

	VFCHttpNet* mHttpNet;
	std::map<std::string, HTTPNET_RECEIVE_FUNCTOR_PTR> mMsgCBMap;
	std::list<HTTPNET_RECEIVE_FUNCTOR_PTR> mComMsgCBList;
};

#endif
