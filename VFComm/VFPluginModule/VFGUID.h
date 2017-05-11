// -------------------------------------------------------------------------
//    @FileName         :    VFGUID.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-11
//    @Module           :    VFGUID
//
// -------------------------------------------------------------------------

#ifndef VF_IDENTID_H
#define VF_IDENTID_H

#include "VFPlatform.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

struct VFGUID
{
    VFINT64 nData64;
    VFINT64 nHead64;

    VFGUID()
    {
        nData64 = 0;
        nHead64 = 0;
    }

    VFGUID(VFINT64 nHeadData, VFINT64 nData)
    {
        nHead64 = nHeadData;
        nData64 = nData;
    }

    VFGUID(const VFGUID& xData)
    {
        nHead64 = xData.nHead64;
        nData64 = xData.nData64;
    }

    VFGUID& operator=(const VFGUID& xData)
    {
        nHead64 = xData.nHead64;
        nData64 = xData.nData64;

        return *this;
    }

    const VFINT64 GetData() const
    {
        return nData64;
    }

    const VFINT64 GetHead() const
    {
        return nHead64;
    }

    void SetData(const VFINT64 nData)
    {
        nData64 = nData;
    }

    void SetHead(const VFINT64 nData)
    {
        nHead64 = nData;
    }

    bool IsNull() const
    {
        return 0 == nData64 && 0 == nHead64;
    }

    bool operator == (const VFGUID& id) const
    {
        return this->nData64 == id.nData64 && this->nHead64 == id.nHead64;
    }

    bool operator != (const VFGUID& id) const
    {
        return this->nData64 != id.nData64 || this->nHead64 != id.nHead64;
    }

    bool operator < (const VFGUID& id) const
    {
        if (this->nHead64 == id.nHead64)
        {
            return this->nData64 < id.nData64;
        }

        return this->nHead64 < id.nHead64;
    }

    std::string ToString() const
    {
        return lexical_cast<std::string>(nHead64) + "-" + lexical_cast<std::string>(nData64);
    }

    bool FromString(const std::string& strID)
    {
        size_t nStrLength = strID.length();
        size_t nPos = strID.find('-');
        if (nPos == std::string::npos)
        {
            return false;
        }

        std::string strHead = strID.substr(0, nPos);
        std::string strData = "";
        if (nPos + 1 < nStrLength)
        {
            strData = strID.substr(nPos + 1, nStrLength - nPos);
        }

        try
        {
            nHead64 = lexical_cast<VFINT64>(strHead);
            nData64 = lexical_cast<VFINT64>(strData);

            return true;
        }
        catch (...)
        {
            return false;
        }
    }
};

#endif
