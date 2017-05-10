// -------------------------------------------------------------------------
//    @FileName			:    NFCDynLib.cpp
//    @Author           :    Wenzheng.Xu
//    @Date             :    2017-05-10
//    @Module           :    NFCDynLib
//
// -------------------------------------------------------------------------

#include "VFCDynLib.h"

bool VFCDynLib::Load()
{
    std::string strLibPath = "./";
    strLibPath += mstrName;
    mInst = (DYNLIB_HANDLE)DYNLIB_LOAD(strLibPath.c_str());

    return mInst != NULL;
}

bool VFCDynLib::UnLoad()
{
    DYNLIB_UNLOAD(mInst);
    return true;
}

void* VFCDynLib::GetSymbol(const char* szProcName)
{
    return (DYNLIB_HANDLE)DYNLIB_GETSYM(mInst, szProcName);
}
