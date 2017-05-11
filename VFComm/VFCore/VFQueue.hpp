// -------------------------------------------------------------------------
//    @FileName			:    VFQueue.h
//    @Author           :    WenZheng.Xu
//    @Date             :    2011-01-21 21:49
//    @Module           :
//
// -------------------------------------------------------------------------

#ifndef VF_QUEUE_H
#define VF_QUEUE_H

#include <list>
#include <thread>
#include <mutex>
#include <atomic>
#include "VFComm/VFPluginModule/VFPlatform.h"

class VFLock
{
public:
    explicit VFLock()
    {
        flag.clear();
    }

    ~VFLock()
    {
    }
    void lock()
    {
        while (flag.test_and_set(std::memory_order_acquire));
    }

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }

protected:
    mutable std::atomic_flag flag;

private:
    VFLock& operator=(const VFLock& src);
};

template<typename T>
class VFQueue : public VFLock
{
public:
    VFQueue()
    {
    }

    virtual ~VFQueue()
    {
    }

    bool Push(const T& object)
    {
        lock();

        mList.push_back(object);

        unlock();

        return true;
    }

    bool Pop(T& object)
    {
        lock();

        if (mList.empty())
        {
            unlock();

            return false;
        }

        object = mList.front();
        mList.pop_front();

        unlock();

        return true;
    }

private:
    std::list<T> mList;
};

#endif