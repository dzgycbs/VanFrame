#include "HelloWorld3Module.h"
#include "Tutorial3Plugin.h"
#include "VFComm/VFMessageDefine/VFProtocolDefine.hpp"
#include "VFComm/VFPluginModule/VFIEventModule.h"

bool VFCHelloWorld3Module::Init()
{
	
	std::cout << "Hello, world3, Init" << std::endl;

	return true;
}

int VFCHelloWorld3Module::OnEvent(const VFGUID& self, const VFEventDefine event, const VFDataList& arg)
{
	
	std::cout << "OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg.Int(0) << " " << " " << arg.String(1) << std::endl;


	return 0;
}

int VFCHelloWorld3Module::OnHeartBeat(const VFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount)
{

	unsigned long unNowTime = VFGetTime();

	std::cout << "strHeartBeat: " << fTime << " Count: " << nCount << "  TimeDis: " << unNowTime - mLastTime << std::endl;

	mLastTime = unNowTime;

	return 0;
}



bool VFCHelloWorld3Module::AfterInit()
{
	
	std::cout << "Hello, world3, AfterInit" << std::endl;

	m_pEventModule = pPluginManager->FindModule<VFIEventModule>();
	m_pScheduleModule = pPluginManager->FindModule<VFIScheduleModule>();
	
	m_pEventModule->AddEventCallBack(VFGUID(0, 1), VFEventDefine(1), this, &VFCHelloWorld3Module::OnEvent);

	m_pScheduleModule->AddSchedule(VFGUID(0, 1), "OnHeartBeat", this, &VFCHelloWorld3Module::OnHeartBeat, 5.0f, 10);

	mLastTime = VFGetTime();


	m_pEventModule->DoEvent(VFGUID(0,1), VFEventDefine(1), VFDataList() << int(100) << "200");

	return true;
}

bool VFCHelloWorld3Module::Execute()
{
	
	//std::cout << "Hello, world3, Execute" << std::endl;

	return true;
}

bool VFCHelloWorld3Module::BeforeShut()
{
	
	std::cout << "Hello, world3, BeforeShut" << std::endl;

//	m_pKernelModule->DestroyAll();

	return true;
}

bool VFCHelloWorld3Module::Shut()
{
	
	std::cout << "Hello, world3, Shut" << std::endl;

	return true;
}
