// -------------------------------------------------------------------------
//    @FileName         :    VFMap.h
//    @Author           :    Wenzheng.Xu
//    @Date             :    2017-05-10
//
// -------------------------------------------------------------------------

#ifndef VF_MAP_H
#define VF_MAP_H

#include <map>
#include <list>
#include <string>
#include <iostream>
#include <typeinfo>
#include "VFMapEx.hpp"

template <typename T , typename TD>
class VFMap
{
public:
    typedef std::map<T, TD*> VFMapOBJECT;

    VFMap() {};
    virtual ~VFMap()
    {
        //mObjectList.clear();
        //DeleteAllElement();
    };

    virtual bool AddElement(const T& name, TD* data)
    {
        typename VFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr == mObjectList.end())
        {
            mObjectList.insert(typename VFMapOBJECT::value_type(name, data));
            // mObjectList[name] = data;
            return true;
        }

        return false;
    }

    virtual TD* RemoveElement(const T& name)
    {
        TD* pData = NULL;
        typename VFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            pData = itr->second;
            mObjectList.erase(itr);
        }

        return pData;
    }

    virtual TD* GetElement(const T& name)
    {
        typename VFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            return itr->second;
        }
        else
        {
            return NULL;
        }
    }

    virtual TD* First()
    {
        if (mObjectList.size() <= 0)
        {
            return NULL;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return NULL;
        }
    }

    virtual TD* Next()
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NULL;
        }

        ++mObjectCurIter;
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return NULL;
        }
    }

    virtual TD* First(T& name)
    {
        if (mObjectList.size() <= 0)
        {
            return NULL;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return NULL;
        }
    }

    virtual TD* Next(T& name)
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NULL;
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return NULL;
        }
    }

    int Count()
    {
        return mObjectList.size();
    }

    bool ClearAll()
    {
        mObjectList.clear();
        return true;
    }
private:
    VFMapOBJECT     mObjectList;
    typename VFMapOBJECT::iterator mObjectCurIter;
};

#endif
