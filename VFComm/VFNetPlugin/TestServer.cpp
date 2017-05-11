
#include "VFCNet.h"
#include <thread>
#include <string>
#include <processthreadsapi.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"VFNet_d.lib")

class TestServerClass
{
public:
    TestServerClass()
    {
        pNet = new VFCNet(this, &TestServerClass::ReciveHandler, &TestServerClass::EventHandler);
        pNet->Initialization(1000, 8088);
    }

    void ReciveHandler(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
    {
        std::string str;
        str.assign(msg, nLen);

        pNet->SendMsgWithOutHead(nMsgID, msg, nLen, nSockIndex);
        std::cout << " fd: " << nSockIndex << " msg_id: " << nMsgID /*<<  " data: " << str*/ << " thread_id: " << GetCurrentThreadId() << std::endl;
    }

    void EventHandler(const int nSockIndex, const VF_NET_EVENT e, VFINet* p)
    {
        std::cout << " fd: " << nSockIndex << " event_id: " << e << " thread_id: " << std::this_thread::get_id() << std::endl;
    }

    void Execute()
    {
        pNet->Execute();
    }

protected:
    VFINet* pNet;
};

int main(int argc, char** argv)
{
    TestServerClass x;

    while (1)
    {
        x.Execute();
    }

    return 0;
}
