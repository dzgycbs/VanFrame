// -------------------------------------------------------------------------
//    @FileName			:		VFINet.h
//    @Author			:		WenZheng.Xu
//    @Date				:		2012-12-15
//    @Module			:		VFINet
// -------------------------------------------------------------------------

#ifndef VFI_NET_H
#define VFI_NET_H

#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdint.h>
#include <iostream>
#include <map>

#include "VFComm/VFPluginModule/VFGUID.h"

#ifndef _MSC_VER
#include <netinet/in.h>
#ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include <vector>
#include <functional>
#include <memory>
#include <list>
#include <vector>
#include <assert.h>

#if VF_PLATFORM == VF_PLATFORM_WIN
//#ifdef _MSC_VER
#include <windows.h>
#include <WinSock2.h>
//#elseifdef _APPLE_
#elif VF_PLATFORM == VF_PLATFORM_APPLE || VF_PLATFORM == VF_PLATFORM_APPLE_IOS
#include <libkern/OSByteOrder.h>
#else
#include <unistd.h>
#endif

#pragma pack(push, 1)

enum VF_NET_EVENT
{
    VF_NET_EVENT_EOF = 0x10,        
    VF_NET_EVENT_ERROR = 0x20,      
    VF_NET_EVENT_TIMEOUT = 0x40,    
    VF_NET_EVENT_CONNECTED = 0x80,  
};


struct  VFIMsgHead
{
    enum VF_Head
    {
        VF_HEAD_LENGTH = 6,
    };

    virtual int EnCode(char* strData) = 0;
    virtual int DeCode(const char* strData) = 0;

    virtual uint16_t GetMsgID() const = 0;
    virtual void SetMsgID(uint16_t nMsgID) = 0;

    virtual uint32_t GetBodyLength() const = 0;
    virtual void SetBodyLength(uint32_t nLength) = 0;

    int64_t VF_HTONLL(int64_t nData)
    {
#if VF_PLATFORM == VF_PLATFORM_WIN
//#ifdef _MSC_VER
        return htonll(nData);
#elif VF_PLATFORM == VF_PLATFORM_APPLE || VF_PLATFORM == VF_PLATFORM_APPLE_IOS
//#elseifdef __APPLE_CC__
        return OSSwapHostToBigInt64(nData);
#else
        return htobe64(nData);
#endif
    }

    int64_t VF_NTOHLL(int64_t nData)
    {
#if VF_PLATFORM == VF_PLATFORM_WIN
//#ifdef _MSC_VER
        return ntohll(nData);
#elif VF_PLATFORM == VF_PLATFORM_APPLE || VF_PLATFORM == VF_PLATFORM_APPLE_IOS
//#elseifdef __APPLE__
        return OSSwapBigToHostInt64(nData);
#else
        return be64toh(nData);
#endif
    }

    int32_t VF_HTONL(int32_t nData)
    {
#if VF_PLATFORM == VF_PLATFORM_WIN
//#ifdef _MSC_VER
        return htonl(nData);
#elif VF_PLATFORM == VF_PLATFORM_APPLE || VF_PLATFORM == VF_PLATFORM_APPLE_IOS
//#elseifdef __APPLE__
        return OSSwapHostToBigInt32(nData);
#else
        return htobe32(nData);
#endif
    }

    int32_t VF_NTOHL(int32_t nData)
    {
#if VF_PLATFORM == VF_PLATFORM_WIN
//#ifdef _MSC_VER
        return ntohl(nData);
#elif VF_PLATFORM == VF_PLATFORM_APPLE || VF_PLATFORM == VF_PLATFORM_APPLE_IOS
//#elseifdef __APPLE__
        return OSSwapBigToHostInt32(nData);
#else
        return be32toh(nData);
#endif
    }

    int16_t VF_HTONS(int16_t nData)
    {
#if VF_PLATFORM == VF_PLATFORM_WIN
//#ifdef _MSC_VER
        return htons(nData);
#elif VF_PLATFORM == VF_PLATFORM_APPLE || VF_PLATFORM == VF_PLATFORM_APPLE_IOS
//#elseifdef __APPLE__
        return OSSwapHostToBigInt16(nData);
#else
        return htobe16(nData);
#endif
    }

    int16_t VF_NTOHS(int16_t nData)
    {
#if VF_PLATFORM == VF_PLATFORM_WIN
//#ifdef _MSC_VER
        return ntohs(nData);
#elif VF_PLATFORM == VF_PLATFORM_APPLE || VF_PLATFORM == VF_PLATFORM_APPLE_IOS
//#elseifdef __APPLE__
        return OSSwapBigToHostInt16(nData);
#else
        return be16toh(nData);
#endif
    }

};

class VFCMsgHead : public VFIMsgHead
{
public:
    VFCMsgHead()
    {
        munSize = 0;
        munMsgID = 0;
    }

    // Message Head[ MsgID(2) | MsgSize(4) ]
    virtual int EnCode(char* strData)
    {
        uint32_t nOffset = 0;

        uint16_t nMsgID = VF_HTONS(munMsgID);
        memcpy(strData + nOffset, (void*)(&nMsgID), sizeof(munMsgID));
        nOffset += sizeof(munMsgID);

        uint32_t nPackSize = munSize + VF_HEAD_LENGTH;
        uint32_t nSize = VF_HTONL(nPackSize);
        memcpy(strData + nOffset, (void*)(&nSize), sizeof(munSize));
        nOffset += sizeof(munSize);

        if (nOffset != VF_HEAD_LENGTH)
        {
            assert(0);
        }

        return nOffset;
    }

    // Message Head[ MsgID(2) | MsgSize(4) ]
    virtual int DeCode(const char* strData)
    {
        uint32_t nOffset = 0;

        uint16_t nMsgID = 0;
        memcpy(&nMsgID, strData + nOffset, sizeof(munMsgID));
        munMsgID = VF_NTOHS(nMsgID);
        nOffset += sizeof(munMsgID);

        uint32_t nPackSize = 0;
        memcpy(&nPackSize, strData + nOffset, sizeof(munSize));
        munSize = VF_NTOHL(nPackSize) - VF_HEAD_LENGTH;
        nOffset += sizeof(munSize);

        if (nOffset != VF_HEAD_LENGTH)
        {
            assert(0);
        }

        return nOffset;
    }

    virtual uint16_t GetMsgID() const
    {
        return munMsgID;
    }
    virtual void SetMsgID(uint16_t nMsgID)
    {
        munMsgID = nMsgID;
    }

    virtual uint32_t GetBodyLength() const
    {
        return munSize;
    }
    virtual void SetBodyLength(uint32_t nLength)
    {
        munSize = nLength;
    }
protected:
    uint32_t munSize;
    uint16_t munMsgID;
};

class VFINet;

typedef std::function<void(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)> NET_RECEIVE_FUNCTOR;
typedef std::shared_ptr<NET_RECEIVE_FUNCTOR> NET_RECEIVE_FUNCTOR_PTR;

typedef std::function<void(const int nSockIndex, const VF_NET_EVENT nEvent, VFINet* pNet)> NET_EVENT_FUNCTOR;
typedef std::shared_ptr<NET_EVENT_FUNCTOR> NET_EVENT_FUNCTOR_PTR;

typedef std::function<void(int severity, const char* msg)> NET_EVENT_LOG_FUNCTOR;
typedef std::shared_ptr<NET_EVENT_LOG_FUNCTOR> NET_EVENT_LOG_FUNCTOR_PTR;

class NetObject
{
public:
    NetObject(VFINet* pNet, int32_t fd, sockaddr_in& addr, struct bufferevent* pBev)
    {
        mnLogicState = 0;
        mnGameID = 0;
        nFD = fd;
        bNeedRemove = false;

        m_pNet = pNet;

        bev = pBev;
        memset(&sin, 0, sizeof(sin));
        sin = addr;
    }

    virtual ~NetObject()
    {
    }

    int AddBuff(const char* str, uint32_t nLen)
    {
        mstrBuff.append(str, nLen);

        return (int)mstrBuff.length();
    }

    int CopyBuffTo(char* str, uint32_t nStart, uint32_t nLen)
    {
        if (nStart + nLen > mstrBuff.length())
        {
            return 0;
        }

        memcpy(str, mstrBuff.data() + nStart, nLen);

        return nLen;
    }

    int RemoveBuff(uint32_t nStart, uint32_t nLen)
    {
        if (nStart < 0)
        {
            return 0;
        }

        if (nStart + nLen > mstrBuff.length())
        {
            return 0;
        }

        mstrBuff.erase(nStart, nLen);

        return mstrBuff.length();
    }

    const char* GetBuff()
    {
        return mstrBuff.data();
    }

    int GetBuffLen() const
    {
        return mstrBuff.length();
    }

    bufferevent* GetBuffEvent()
    {
        return bev;
    }

    VFINet* GetNet()
    {
        return m_pNet;
    }
    //////////////////////////////////////////////////////////////////////////
    int GetConnectKeyState() const
    {
        return mnLogicState;
    }

    void SetConnectKeyState(const int nState)
    {
        mnLogicState = nState;
    }
    bool NeedRemove()
    {
        return bNeedRemove;
    }
    void SetNeedRemove(bool b)
    {
        bNeedRemove = b;
    }
    const std::string& GetAccount() const
    {
        return mstrUserData;
    }

    void SetAccount(const std::string& strData)
    {
        mstrUserData = strData;
    }
    int GetGameID() const
    {
        return mnGameID;
    }

    void SetGameID(const int nData)
    {
        mnGameID = nData;
    }
    const VFGUID& GetUserID()
    {
        return mnUserID;
    }

    void SetUserID(const VFGUID& nUserID)
    {
        mnUserID = nUserID;
    }

    const VFGUID& GetClientID()
    {
        return mnClientID;
    }

    void SetClientID(const VFGUID& xClientID)
    {
        mnClientID = xClientID;
    }
    const VFGUID& GetHashIdentID()
    {
        return mnHashIdentID;
    }

    void SetHashIdentID(const VFGUID& xHashIdentID)
    {
        mnHashIdentID = xHashIdentID;
    }

    int GetRealFD()
    {
        return nFD;
    }

private:
    sockaddr_in sin;
    bufferevent* bev;
    std::string mstrBuff;
    std::string mstrUserData;

    int32_t mnLogicState;
    int32_t mnGameID;
    VFGUID mnUserID;//player id
    VFGUID mnClientID;//temporary client id
    VFGUID mnHashIdentID;//hash ident, special for distributed
    VFINet* m_pNet;
    //
    int nFD;
    bool bNeedRemove;
};

class VFINet
{
public:
	virtual ~VFINet() {}

    //need to call this function every frame to drive network library
    virtual bool Execute() = 0;

    virtual void Initialization(const char* strIP, const unsigned short nPort) = 0;
    virtual int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4) = 0;
	virtual int ExpandBufferSize(const unsigned int size) = 0;

    virtual bool Final() = 0;

    //send a message with out msg-head[auto add msg-head in this function]
    virtual bool SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const int nSockIndex = 0) = 0;

    //send a message to all client[need to add msg-head for this message by youself]
    virtual bool SendMsgToAllClient(const char* msg, const uint32_t nLen) = 0;

    //send a message with out msg-head to all client[auto add msg-head in this function]
    virtual bool SendMsgToAllClientWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen) = 0;

    virtual bool CloseNetObject(const int nSockIndex) = 0;
    virtual NetObject* GetNetObject(const int nSockIndex) = 0;
    virtual bool AddNetObject(const int nSockIndex, NetObject* pObject) = 0;

    virtual bool IsServer() = 0;

    virtual bool Log(int severity, const char* msg) = 0;
};

#pragma pack(pop)

#endif
