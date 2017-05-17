// -------------------------------------------------------------------------
//    @FileName         :    VFILogModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    VFILogModule
//
// -------------------------------------------------------------------------

#ifndef VFI_LOG_MODULE_H
#define VFI_LOG_MODULE_H

#include "VFIModule.h"

class VFILogModule
    : public VFIModule
{

public:

    enum VF_LOG_LEVEL
    {
        NLL_DEBUG_NORMAL,
		NLL_INFO_NORMAL,
        NLL_WARING_NORMAL,
        NLL_ERROR_NORMAL,
        NLL_FATAL_NORMAL,
    };

    virtual bool LogElement(const VF_LOG_LEVEL nll, const VFGUID ident, const std::string& strElement, const std::string& strDesc, const char* func = "", int line = 0) = 0;
    virtual bool LogProperty(const VF_LOG_LEVEL nll, const VFGUID ident, const std::string& strProperty, const std::string& strDesc, const char* func = "", int line = 0) = 0;
    virtual bool LogObject(const VF_LOG_LEVEL nll, const VFGUID ident, const std::string& strDesc, const char* func = "", int line = 0) = 0;
    virtual bool LogRecord(const VF_LOG_LEVEL nll, const VFGUID ident, const std::string& strRecord, const std::string& strDesc, const int nRow, const int nCol, const char* func = "", int line = 0) = 0;
    virtual bool LogRecord(const VF_LOG_LEVEL nll, const VFGUID ident, const std::string& strRecord, const std::string& strDesc, const char* func = "", int line = 0) = 0;

    virtual bool LogNormal(const VF_LOG_LEVEL nll, const VFGUID ident, const std::string& strInfo, const int nDesc, const char* func = "", int line = 0) = 0;
    virtual bool LogNormal(const VF_LOG_LEVEL nll, const VFGUID ident, const std::string& strInfo, const std::string& strDesc, const  char* func = "", int line = 0) = 0;
    virtual bool LogNormal(const VF_LOG_LEVEL nll, const VFGUID ident, const std::ostringstream& stream, const char* func = "", int line = 0) = 0;

};

#endif