// -------------------------------------------------------------------------
//    @FileName			:		VFCWS.cpp
//    @Author			:		Stone.xin
//    @Date				:		2016-12-22
//    @Module			:		VFCWS
// -------------------------------------------------------------------------

#include "VFCWS.h"
#include <string.h>
#include <atomic>

bool VFCWS::Execute()
{
	if (mbWorking)
	{
		ExecuteClose();
		m_EndPoint.poll_one();
	}
	return false;
}

int VFCWS::Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount)
{
	if (mbWorking)
	{
		return 1;
	}

	mnPort = nPort;
	mnMaxConnect = nMaxClient;
	mnCpuCount = nCpuCount;

	m_EndPoint.init_asio();

	m_EndPoint.set_message_handler(std::bind(
		&VFCWS::OnMessageHandler, this,
		std::placeholders::_1, std::placeholders::_2
		));

	m_EndPoint.set_open_handler(std::bind(
		&VFCWS::OnOpenHandler, this,
		std::placeholders::_1));

	m_EndPoint.set_close_handler(std::bind(
		&VFCWS::OnCloseHandler, this,
		std::placeholders::_1));

	m_EndPoint.set_fail_handler(std::bind(
		&VFCWS::OnFailHandler, this,
		std::placeholders::_1));

	m_EndPoint.set_pong_handler(std::bind(
		&VFCWS::OnPongHandler, this,
		std::placeholders::_1, std::placeholders::_2));

	m_EndPoint.set_interrupt_handler(std::bind(
		&VFCWS::OnInterruptHandler, this,
		std::placeholders::_1));
	
	m_EndPoint.set_pong_timeout_handler(std::bind(
		&VFCWS::OnPongTimeOutHandler, this,
		std::placeholders::_1, std::placeholders::_2));


	m_EndPoint.listen(nPort);
	m_EndPoint.start_accept();

	mbWorking = true;

	return 0;
}

bool VFCWS::Final()
{
	CloseSocketAll();
	m_EndPoint.stop_listening();

	return true;
}

bool VFCWS::SendMsgToAllClient(const char * msg, const uint32_t nLen)
{
	if (nLen <= 0)
	{
		return false;
	}

	session_list::iterator it = mmObject.begin();
	while (it!=mmObject.end())
	{
		WSObjectPtr pWSObject = it->second;
		if (pWSObject && !pWSObject->NeedRemove())
		{
			try
			{
				m_EndPoint.send(it->first, msg, nLen, websocketpp::frame::opcode::TEXT);
			}
			catch (websocketpp::exception& e)
			{
				std::cout<<"websocket exception: "<<e.what()<< " this conn will be removed." <<std::endl;
				RemoveConnection(it->first, VF_WS_EVENT_CLOSE);
			}
		}
		it++;
	}
	
	return true;
}

bool VFCWS::SendMsgToClient(const char * msg, const uint32_t nLen, const std::vector<websocketpp::connection_hdl>& vList)
{
	for (auto vIt:vList)
	{
		auto pWSObject = GetNetObject(vIt);
		if (pWSObject && !pWSObject->NeedRemove())
		{
			try
			{
				m_EndPoint.send(vIt, msg, nLen, websocketpp::frame::opcode::TEXT);
				return true;
			}
			catch (websocketpp::exception& e)
			{
				std::cout << "websocket exception: " << e.what()<<" this conn will be removed." << std::endl;	
				RemoveConnection(vIt, VF_WS_EVENT_CLOSE);
			}
		}
	}
	return false;
}

bool VFCWS::SendMsgToClient(const char * msg, const uint32_t nLen, websocketpp::connection_hdl hd)
{
	auto pWSObject = GetNetObject(hd);
	if (pWSObject && !pWSObject->NeedRemove())
	{
		try
		{
			m_EndPoint.send(hd, msg, nLen, websocketpp::frame::opcode::TEXT);
			return true;
		}
		catch (websocketpp::exception& e)
		{
			std::cout << "websocket exception: " << e.what()<<" this conn will be removed." << std::endl;
			RemoveConnection(hd, VF_WS_EVENT_CLOSE);
		}
	}
	return false;
}

bool VFCWS::AddNetObject(websocketpp::connection_hdl hd,WSObjectPtr pWSObject)
{
	auto pObject = GetNetObject(hd);
	if (pObject)
	{
		return false;
	}
	mmObject.emplace(session_list::value_type(hd,pWSObject));
	return true;
}

WSObjectPtr VFCWS::GetNetObject(websocketpp::connection_hdl hd)
{
	session_list::iterator it = mmObject.find(hd);
	if (it == mmObject.end())
	{
		return nullptr;
	}
	return it->second;
}

void VFCWS::ExecuteClose()
{
	for(auto vIt : mvRemoveObject)
	{
		CloseObject(vIt);
	}
	mvRemoveObject.clear();
}

bool VFCWS::CloseSocketAll()
{
	session_list::iterator it = mmObject.begin();
	while(it != mmObject.end())
	{
		mvRemoveObject.push_back(it->first);
		it++;
	}

	ExecuteClose();

	mmObject.clear();

	return true;
}

void VFCWS::CloseObject(websocketpp::connection_hdl hd, int nCloseCode/* =1000 */, const std::string& strCloseReason/* ="" */)
{
	m_EndPoint.close(hd, nCloseCode, strCloseReason);
}

void VFCWS::OnMessageHandler(websocketpp::connection_hdl hd, VFWebSockConf::message_ptr msg)
{
	auto pObject = GetNetObject(hd);
	if (!pObject)
	{
		return;
	}

	if (mRecvCB)
	{
		mRecvCB(hd,msg->get_payload());
	}
}

void VFCWS::OnOpenHandler(websocketpp::connection_hdl hd)
{
	WSObjectPtr pWSObject(VF_NEW(WSObject));
	if (AddNetObject(hd,pWSObject))
	{
		if (mEventCB)
		{
			mEventCB(hd, VF_WS_EVENT_OPEN);
		}
	}
}

bool VFCWS::RemoveConnection(websocketpp::connection_hdl hd, VF_WS_EVENT evt, int nCloseCode /* = 1000 */, const std::string& strCloseReason /* = "" */)
{
	session_list::iterator it = mmObject.find(hd);
	if (it != mmObject.end())
	{
		mvRemoveObject.push_back(hd);
		return true;
	}
	return false;
}

void VFCWS::OnCloseHandler(websocketpp::connection_hdl hd)
{
	RemoveConnection(hd, VF_WS_EVENT_CLOSE);
}

void VFCWS::OnFailHandler(websocketpp::connection_hdl hd)
{
	RemoveConnection(hd, VF_WS_EVENT_FAIL);
}

void VFCWS::OnInterruptHandler(websocketpp::connection_hdl hd)
{
	RemoveConnection(hd, VF_WS_EVENT_INTERRUPT);
}

bool VFCWS::OnPongHandler(websocketpp::connection_hdl hd, std::string str)
{
	return true;
}

void VFCWS::OnPongTimeOutHandler(websocketpp::connection_hdl hd, std::string str)
{
	RemoveConnection(hd, VF_WS_EVENT_PONG_TIMEOUT);
}
