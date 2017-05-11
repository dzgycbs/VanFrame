#include "VFCHttpServerModule.h"

VFCHttpServerModule::VFCHttpServerModule(VFIPluginManager* p)
{
	pPluginManager = p;
	mHttpNet = NULL;
}
VFCHttpServerModule::~VFCHttpServerModule()
{
	if (mHttpNet)
	{
		mHttpNet->Final();
		delete mHttpNet;
		mHttpNet = NULL;
	}
}

bool VFCHttpServerModule::Execute()
{
	if(mHttpNet)
		return mHttpNet->Execute();

	return true;
}

int VFCHttpServerModule::InitServer(const unsigned short nPort)
{
	mHttpNet = new VFCHttpNet(this, &VFCHttpServerModule::OnReceiveNetPack);
	std::cout << "Open http port:" << nPort << std::endl;
	return mHttpNet->InitServer(nPort);
}

void VFCHttpServerModule::OnReceiveNetPack(struct evhttp_request *req, const std::string& strCommand, const std::string& strUrl)
{
	auto it = mMsgCBMap.find(strCommand);
	if (mMsgCBMap.end() != it)
	{
		HTTPNET_RECEIVE_FUNCTOR_PTR& pFunPtr = it->second;
		HTTPNET_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(req, strCommand, strUrl);
	}
	else
	{
		if (mComMsgCBList.size() > 0)
		{
			for (std::list<HTTPNET_RECEIVE_FUNCTOR_PTR>::iterator it = mComMsgCBList.begin(); it != mComMsgCBList.end(); ++it)
			{
				HTTPNET_RECEIVE_FUNCTOR_PTR& pFunPtr = *it;
				HTTPNET_RECEIVE_FUNCTOR* pFunc = pFunPtr.get();
				pFunc->operator()(req, strCommand, strUrl);
			}
		}
		else
		{
			std::cout << "un support http Command:" << strCommand.c_str() << std::endl;
		}
	}
}

bool VFCHttpServerModule::AddMsgCB(const std::string& strCommand, const HTTPNET_RECEIVE_FUNCTOR_PTR& cb)
{
	if (mMsgCBMap.find(strCommand) == mMsgCBMap.end())
	{
		mMsgCBMap.insert(std::map<std::string, HTTPNET_RECEIVE_FUNCTOR_PTR>::value_type(strCommand, cb));
		return true;
	}
	return false;
}

bool VFCHttpServerModule::AddComMsgCB(const HTTPNET_RECEIVE_FUNCTOR_PTR& cb)
{
	mComMsgCBList.push_back(cb);
	return true;
}

VFIHttpNet* VFCHttpServerModule::GetHttpNet()
{
	return mHttpNet;
}