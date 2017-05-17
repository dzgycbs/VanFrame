// -------------------------------------------------------------------------
//    @FileName         :    VFMemoryCounter.h
//    @Author           :    Yu.Tang
//    @Date             :    2016-11-26
//    @Module           :    VFMemoryCounter
//
// -------------------------------------------------------------------------

#ifndef VF_MEMORY_COUNTER_H
#define VF_MEMORY_COUNTER_H

#include <iostream>
#include <string>
#include <map>
#include "VFComm/VFPluginModule/VFPlatform.h"

template <class T>
class VFMemoryCounter
{
private:
	VFMemoryCounter() {}
public:
	VFMemoryCounter(const std::string& strClassName)
	{
		mstrClassName = strClassName;
		mxCounter[mstrClassName]++;
	}

	~VFMemoryCounter()
	{
		mxCounter[mstrClassName]--;
	}

	std::string mstrClassName;
	static std::map<std::string, int> mxCounter;
};

template <class T>
std::map<std::string, int> VFMemoryCounter<T>::mxCounter;

#endif