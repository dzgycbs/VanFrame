// -------------------------------------------------------------------------
//    @FileName         :    VFException.hpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-22
//    @Module           :    VFException
//
// -------------------------------------------------------------------------

#ifndef VF_EXCEPTION_HPP
#define VF_EXCEPTION_HPP

#include <string>
#include <cstring>
#include <stdio.h>
#include <stdarg.h>

class VFException
{
public:
	VFException(const char *format, ...)
	{
		char buf[1024] = {0};
		va_list args;
		va_start(args, format);
		vsprintf(buf, format, args);
		message = std::string(buf);
		va_end(args);
	}
	std::string GetMessage() { return message; }
protected:
	std::string message;
};

#endif