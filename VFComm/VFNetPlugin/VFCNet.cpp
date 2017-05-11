// -------------------------------------------------------------------------
//    @FileName			:		VFCNet.cpp
//    @Author			:		WenZheng.Xu
//    @Date				:		2012-12-15
//    @Module			:		VFCNet
// -------------------------------------------------------------------------

#include "VFCNet.h"
#include <string.h>

#if VF_PLATFORM == VF_PLATFORM_WIN
#include <WS2tcpip.h>
#include <winsock2.h>
#pragma  comment(lib,"Ws2_32.lib")
#ifndef LIBEVENT_SRC
#pragma  comment(lib,"libevent.lib")
#pragma  comment(lib,"libevent_core.lib")
#endif
#elif VF_PLATFORM == VF_PLATFORM_APPLE
#include <arpa/inet.h>
#endif

#include "event2/bufferevent_struct.h"
#include "event2/event.h"
#include <atomic>

void VFCNet::conn_writecb(struct bufferevent* bev, void* user_data)
{
    
    //  struct evbuffer *output = bufferevent_get_output(bev);
}

void VFCNet::conn_eventcb(struct bufferevent* bev, short events, void* user_data)
{
    NetObject* pObject = (NetObject*)user_data;
    VFCNet* pNet = (VFCNet*)pObject->GetNet();

    if (events & BEV_EVENT_CONNECTED)
    {
        //must to set it's state before the "EventCB" functional be called[maybe user will send msg in the callback function]
        pNet->mbWorking = true;
    }
    else
    {
        if (!pNet->mbServer)
        {
            pNet->mbWorking = false;
        }
    }

    if (pNet->mEventCB)
    {
        pNet->mEventCB(pObject->GetRealFD(), VF_NET_EVENT(events), pNet);
    }

    if (events & BEV_EVENT_CONNECTED)
    {
		struct evbuffer* input = bufferevent_get_input(bev);
		struct evbuffer* output = bufferevent_get_output(bev);
		if (pNet->mnBufferSize > 0)
		{
			evbuffer_expand(input, pNet->mnBufferSize);
			evbuffer_expand(output, pNet->mnBufferSize);
		}
        //printf("%d Connection successed\n", pObject->GetFd());/*XXX win32*/
    }
    else
    {
        pNet->CloseNetObject(pObject->GetRealFD());
    }
}

void VFCNet::listener_cb(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sa, int socklen, void* user_data)
{
    
    VFCNet* pNet = (VFCNet*)user_data;
    bool bClose = pNet->CloseNetObject(fd);
    if (bClose)
    {
        return;
    }

    if (pNet->mmObject.size() >= pNet->mnMaxConnect)
    {
        
        return;
    }

    struct event_base* base = pNet->base;
    
    struct bufferevent* bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev)
    {
        
        fprintf(stderr, "Error constructing bufferevent!");
        return;
    }


    struct sockaddr_in* pSin = (sockaddr_in*)sa;

    NetObject* pObject = new NetObject(pNet, fd, *pSin, bev);
    pObject->GetNet()->AddNetObject(fd, pObject);

    
    bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, (void*)pObject);

    
    bufferevent_enable(bev, EV_READ | EV_WRITE);

    
    conn_eventcb(bev, BEV_EVENT_CONNECTED, (void*)pObject);
}


void VFCNet::conn_readcb(struct bufferevent* bev, void* user_data)
{
    
    NetObject* pObject = (NetObject*)user_data;
    if (!pObject)
    {
        return;
    }

    VFCNet* pNet = (VFCNet*)pObject->GetNet();
    if (!pNet)
    {
        return;
    }

    if (pObject->NeedRemove())
    {
        return;
    }

    struct evbuffer* input = bufferevent_get_input(bev);
    if (!input)
    {
        return;
    }

    size_t len = evbuffer_get_length(input);

    //////////////////////////////////////////////////////////////////////////

    
    char* strMsg = new char[len];

    if (evbuffer_remove(input, strMsg, len) > 0)
    {
        pObject->AddBuff(strMsg, len);
    }

    delete[] strMsg;

    while (1)
    {
        if (!pNet->Dismantle(pObject))
        {
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

bool VFCNet::Execute()
{
    ExecuteClose();

    if (base)
    {
        event_base_loop(base, EVLOOP_ONCE | EVLOOP_NONBLOCK);
    }

    return true;
}


void VFCNet::Initialization(const char* strIP, const unsigned short nPort)
{
    mstrIP = strIP;
    mnPort = nPort;

    InitClientNet();
}

int VFCNet::Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount)
{
    mnMaxConnect = nMaxClient;
    mnPort = nPort;
    mnCpuCount = nCpuCount;

    return InitServerNet();
}

int VFCNet::ExpandBufferSize(const unsigned int size)
{
	if (size > 0)
	{
		mnBufferSize = size;
	}
	return mnBufferSize;
}

bool VFCNet::Final()
{

    CloseSocketAll();

    if (listener)
    {
        evconnlistener_free(listener);
        listener = NULL;
    }

    if (!mbServer)
    {
        if (base)
        {
            event_base_free(base);
            base = NULL;
        }
    }

    return true;
}

bool VFCNet::SendMsgToAllClient(const char* msg, const uint32_t nLen)
{
    if (nLen <= 0)
    {
        return false;
    }

    std::map<int, NetObject*>::iterator it = mmObject.begin();
    for (; it != mmObject.end(); ++it)
    {
        NetObject* pNetObject = (NetObject*)it->second;
        if (pNetObject && !pNetObject->NeedRemove())
        {
            bufferevent* bev = pNetObject->GetBuffEvent();
            if (NULL != bev && mbWorking )
            {
                bufferevent_write(bev, msg, nLen);

                mnSendMsgTotal++;
            }
        }
    }

    return true;
}


bool VFCNet::SendMsg(const char* msg, const uint32_t nLen, const int nSockIndex)
{
    if (nLen <= 0)
    {
        return false;
    }

    std::map<int, NetObject*>::iterator it = mmObject.find(nSockIndex);
    if (it != mmObject.end())
    {
        NetObject* pNetObject = (NetObject*)it->second;
        if (pNetObject)
        {
            bufferevent* bev = pNetObject->GetBuffEvent();
            if (NULL != bev && mbWorking)
            {
                bufferevent_write(bev, msg, nLen);

                mnSendMsgTotal++;
                return true;
            }
        }
    }

    return false;
}

bool VFCNet::SendMsg(const char* msg, const uint32_t nLen, const std::list<int>& fdList)
{
    std::list<int>::const_iterator it = fdList.begin();
    for (; it != fdList.end(); ++it)
    {
        SendMsg(msg, nLen, *it);
    }

    return true;
}

bool VFCNet::CloseNetObject(const int nSockIndex)
{
    std::map<int, NetObject*>::iterator it = mmObject.find(nSockIndex);
    if (it != mmObject.end())
    {
        NetObject* pObject = it->second;

        pObject->SetNeedRemove(true);
        mvRemoveObject.push_back(nSockIndex);

        return true;
    }

    return false;
}

bool VFCNet::Dismantle(NetObject* pObject)
{
    bool bNeedDismantle = false;

    int len = pObject->GetBuffLen();
    if (len > VFIMsgHead::VF_Head::VF_HEAD_LENGTH)
    {
        VFCMsgHead xHead;
        int nMsgBodyLength = DeCode(pObject->GetBuff(), len, xHead);
        if (nMsgBodyLength > 0 && xHead.GetMsgID() > 0)
        {
            int nRet = 0;

            if (mRecvCB)
            {
                mRecvCB(pObject->GetRealFD(), xHead.GetMsgID(), pObject->GetBuff() + VFIMsgHead::VF_Head::VF_HEAD_LENGTH, nMsgBodyLength);

                mnReceiveMsgTotal++;
            }

			pObject->RemoveBuff(0, nMsgBodyLength + VFIMsgHead::VF_Head::VF_HEAD_LENGTH);

            bNeedDismantle = true;
        }
        else if (0 == nMsgBodyLength)
        {
            

            bNeedDismantle = false;
        }
        else
        {
            
            //pObject->IncreaseError();

            bNeedDismantle = false;

        }
    }

    return bNeedDismantle;
}

bool VFCNet::AddNetObject(const int nSockIndex, NetObject* pObject)
{
    //lock
    return mmObject.insert(std::map<int, NetObject*>::value_type(nSockIndex, pObject)).second;
}

int VFCNet::InitClientNet()
{
    std::string strIP = mstrIP;
    int nPort = mnPort;

    struct sockaddr_in addr;
    struct bufferevent* bev = NULL;

#ifdef _MSC_VER
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
#endif

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(nPort);

    if (evutil_inet_pton(AF_INET, strIP.c_str(), &addr.sin_addr) <= 0)
    {
        printf("inet_pton");
        return -1;
    }

    base = event_base_new();
    if (base == NULL)
    {
        printf("event_base_new ");
        return -1;
    }

    bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
    if (bev == NULL)
    {
        printf("bufferevent_socket_new ");
        return -1;
    }

    int bRet = bufferevent_socket_connect(bev, (struct sockaddr*)&addr, sizeof(addr));
    if (0 != bRet)
    {
        //int nError = GetLastError();
        printf("bufferevent_socket_connect error");
        return -1;
    }

    int sockfd = bufferevent_getfd(bev);
    NetObject* pObject = new NetObject(this, sockfd, addr, bev);
    if (!AddNetObject(0, pObject))
    {
        assert(0);
        return -1;
    }

    mbServer = false;

    bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, (void*)pObject);
    bufferevent_enable(bev, EV_READ | EV_WRITE);

    event_set_log_callback(&VFCNet::log_cb);

	int nSizeRead = bufferevent_get_max_to_read(bev);
	int nSizeWrite = bufferevent_get_max_to_write(bev);

	std::cout << "SizeRead: " << nSizeRead << std::endl;
	std::cout << "SizeWrite: " << nSizeWrite << std::endl;

	//bufferevent_set_max_single_read(bev, 0);
	//bufferevent_set_max_single_write(bev, 0);

    return sockfd;
}

int VFCNet::InitServerNet()
{
    int nMaxClient = mnMaxConnect;
    int nCpuCount = mnCpuCount;
    int nPort = mnPort;

    struct sockaddr_in sin;

#if VF_PLATFORM == VF_PLATFORM_WIN
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);

#endif
    //////////////////////////////////////////////////////////////////////////

    struct event_config* cfg = event_config_new();

#if VF_PLATFORM == VF_PLATFORM_WIN

    base = event_base_new_with_config(cfg);

#else

    //event_config_avoid_method(cfg, "epoll");
    if (event_config_set_flag(cfg, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST) < 0)
    {
        
        return -1;
    }

    if (event_config_set_num_cpus_hint(cfg, nCpuCount) < 0)
    {
        return -1;
    }

    base = event_base_new_with_config(cfg);//event_base_new()

#endif
    event_config_free(cfg);

    //////////////////////////////////////////////////////////////////////////

    if (!base)
    {
        fprintf(stderr, "Could not initialize libevent!\n");
        Final();

        return -1;
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(nPort);

    printf("server started with %d\n", nPort);

    listener = evconnlistener_new_bind(base, listener_cb, (void*)this,
                                       LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
                                       (struct sockaddr*)&sin,
                                       sizeof(sin));

    if (!listener)
    {
        fprintf(stderr, "Could not create a listener!\n");
        Final();

        return -1;
    }

    mbServer = true;

    event_set_log_callback(&VFCNet::log_cb);

    return mnMaxConnect;
}

bool VFCNet::CloseSocketAll()
{
    std::map<int, NetObject*>::iterator it = mmObject.begin();
    for (it; it != mmObject.end(); ++it)
    {
        int nFD = it->first;
        mvRemoveObject.push_back(nFD);
    }

    ExecuteClose();

    mmObject.clear();

    return true;
}

NetObject* VFCNet::GetNetObject(const int nSockIndex)
{
    std::map<int, NetObject*>::iterator it = mmObject.find(nSockIndex);
    if (it != mmObject.end())
    {
        return it->second;
    }

    return NULL;
}

void VFCNet::CloseObject(const int nSockIndex)
{
    std::map<int, NetObject*>::iterator it = mmObject.find(nSockIndex);
    if (it != mmObject.end())
    {
        NetObject* pObject = it->second;

        struct bufferevent* bev = pObject->GetBuffEvent();

        bufferevent_free(bev);

        mmObject.erase(it);

        delete pObject;
        pObject = NULL;
    }
}

void VFCNet::ExecuteClose()
{
    for (int i = 0; i < mvRemoveObject.size(); ++i)
    {
        int nSocketIndex = mvRemoveObject[i];
        CloseObject(nSocketIndex);
    }

    mvRemoveObject.clear();
}

void VFCNet::log_cb(int severity, const char* msg)
{

}

bool VFCNet::IsServer()
{
    return mbServer;
}

bool VFCNet::Log(int severity, const char* msg)
{
    log_cb(severity, msg);
    return true;
}

bool VFCNet::SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const int nSockIndex /*= 0*/)
{
    std::string strOutData;
    int nAllLen = EnCode(nMsgID, msg, nLen, strOutData);
    if (nAllLen == nLen + VFIMsgHead::VF_Head::VF_HEAD_LENGTH)
    {
        
        return SendMsg(strOutData.c_str(), strOutData.length(), nSockIndex);
    }

    return false;
}

bool VFCNet::SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const std::list<int>& fdList)
{
    std::string strOutData;
    int nAllLen = EnCode(nMsgID, msg, nLen, strOutData);
    if (nAllLen == nLen + VFIMsgHead::VF_Head::VF_HEAD_LENGTH)
    {
        
        return SendMsg(strOutData.c_str(), strOutData.length(), fdList);
    }

    return false;
}

bool VFCNet::SendMsgToAllClientWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen)
{
    std::string strOutData;
    int nAllLen = EnCode(nMsgID, msg, nLen, strOutData);
    if (nAllLen == nLen + VFIMsgHead::VF_Head::VF_HEAD_LENGTH)
    {
        
        return SendMsgToAllClient(strOutData.c_str(), strOutData.length());
    }

    return false;
}

int VFCNet::EnCode(const uint16_t unMsgID, const char* strData, const uint32_t unDataLen, std::string& strOutData)
{
    VFCMsgHead xHead;
    xHead.SetMsgID(unMsgID);
    xHead.SetBodyLength(unDataLen);

    char szHead[VFIMsgHead::VF_Head::VF_HEAD_LENGTH] = { 0 };
    xHead.EnCode(szHead);

    strOutData.clear();
    strOutData.append(szHead, VFIMsgHead::VF_Head::VF_HEAD_LENGTH);
    strOutData.append(strData, unDataLen);

    return xHead.GetBodyLength() + VFIMsgHead::VF_Head::VF_HEAD_LENGTH;
}

int VFCNet::DeCode(const char* strData, const uint32_t unAllLen, VFCMsgHead& xHead)
{
    
    if (unAllLen < VFIMsgHead::VF_Head::VF_HEAD_LENGTH)
    {
        
        return -1;
    }

    if (VFIMsgHead::VF_Head::VF_HEAD_LENGTH != xHead.DeCode(strData))
    {
        
        return -2;
    }

    if (xHead.GetBodyLength() > (unAllLen - VFIMsgHead::VF_Head::VF_HEAD_LENGTH))
    {
        
        return -3;
    }
    
    return xHead.GetBodyLength();
}
