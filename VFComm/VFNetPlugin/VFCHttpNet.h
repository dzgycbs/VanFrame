#ifndef VFC_HTTP_NET_H
#define VFC_HTTP_NET_H

#include "VFIHttpNet.h"

class VFCHttpNet : public VFIHttpNet
{
public:
	template<typename BaseType>
	VFCHttpNet(BaseType* pBaseType, void (BaseType::*handleRecieve)(struct evhttp_request *req, const std::string& strCommand, const std::string& strUrl))
	{
		base = NULL;
		mRecvCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		mPort = 0;
	}
	VFCHttpNet()
	{

	}
	virtual ~VFCHttpNet() {};

	virtual bool Execute();
public:
	virtual int InitServer(const unsigned short nPort);
	static void listener_cb(struct evhttp_request *req, void *arg);
	virtual bool SendMsg(struct evhttp_request *req, const char* strMsg);
	virtual bool SendFile(evhttp_request * req, const int fd, struct stat st, const std::string& strType);
	virtual bool Final();
	static std::vector<std::string> Split(const std::string& str, std::string delim);
private:
	int mPort;
	struct event_base* base;

	HTTPNET_RECEIVE_FUNCTOR mRecvCB;

};

#endif
