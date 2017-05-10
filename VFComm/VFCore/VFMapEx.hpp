// -------------------------------------------------------------------------
//    @FileName         :    VFMapEx.h
//    @Author           :    Wenzheng.Xu
//    @Date             :    2017-05-10
//
// -------------------------------------------------------------------------

#ifndef VF_MAPEX_H
#define VF_MAPEX_H

#include <map>
#include <list>
#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>
#include "VFCConsistentHash.hpp"
#include "VFComm/VFPluginModule/VFPlatform.h"

template <typename T , typename TD>
class VFMapEx
{
public:
    typedef std::map<T, VF_SHARE_PTR<TD> > VFMapOBJECT;

    VFMapEx()
	{
	};
    virtual ~VFMapEx()
    {
    };

	virtual bool ExistElement(const T& name)
	{
		typename VFMapOBJECT::iterator itr = mObjectList.find(name);
		if (itr != mObjectList.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
/*
	virtual VF_SHARE_PTR<TD> AddElement(const T& name)
	{
		typename VFMapOBJECT::iterator itr = mObjectList.find(name);
		if (itr == mObjectList.end())
		{
			VF_SHARE_PTR<TD> data(VF_NEW TD());
			mObjectList.insert(typename VFMapOBJECT::value_type(name, data));
			return data;
		}

		return VF_SHARE_PTR<TD>();
	}
	*/
    virtual bool AddElement(const T& name, const VF_SHARE_PTR<TD> data)
    {
		if (data == nullptr)
		{
			return false;
		}

        typename VFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr == mObjectList.end())
        {
            mObjectList.insert(typename VFMapOBJECT::value_type(name, data));

            return true;
        }

        return false;
    }

    virtual bool RemoveElement(const T& name)
    {
        typename VFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            mObjectList.erase(itr);

            return true;
        }

        return false;
    }

    virtual TD* GetElementNude(const T& name)
    {
        typename VFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            return itr->second.get();
        }
        else
        {
            return NULL;
        }
    }

    virtual VF_SHARE_PTR<TD> GetElement(const T& name)
    {
        typename VFMapOBJECT::iterator itr = mObjectList.find(name);
        if (itr != mObjectList.end())
        {
            return itr->second;
        }
        else
        {
            return nullptr;
        }
    }

    virtual TD* FirstNude(T& name)
    {
        if (mObjectList.size() <= 0)
        {
            return NULL;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second.get();
        }
        else
        {
            return NULL;
        }
    }

    virtual TD* NextNude(T& name)
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NULL;
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second.get();
        }
        else
        {
            return NULL;
        }
    }
    virtual TD* FirstNude()
    {
        if (mObjectList.size() <= 0)
        {
            return NULL;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second.get();
        }
        else
        {
            return NULL;
        }
    }
    virtual TD* NextNude()
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return NULL;
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second.get();
        }
        else
        {
            return NULL;
        }
    }

    virtual VF_SHARE_PTR<TD> First()
    {
        if (mObjectList.size() <= 0)
        {
            return nullptr;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return nullptr;
        }
    }

    virtual VF_SHARE_PTR<TD> Next()
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return nullptr;
        }

        ++mObjectCurIter;
        if (mObjectCurIter != mObjectList.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return nullptr;
        }
    }

    virtual VF_SHARE_PTR<TD> First(T& name)
    {
        if (mObjectList.size() <= 0)
        {
            return nullptr;
        }

        mObjectCurIter = mObjectList.begin();
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return nullptr;
        }
    }

    virtual VF_SHARE_PTR<TD> Next(T& name)
    {
        if (mObjectCurIter == mObjectList.end())
        {
            return nullptr;
        }

        mObjectCurIter++;
        if (mObjectCurIter != mObjectList.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return nullptr;
        }
    }

    int Count()
    {
        return (int)mObjectList.size();
    }

    bool ClearAll()
    {
        mObjectList.clear();
        return true;
    }

protected:
    VFMapOBJECT     mObjectList;
    typename VFMapOBJECT::iterator mObjectCurIter;
};

template <typename T, typename TD>
class VFCConsistentHashMapEx : public VFMapEx<T, TD>
{
public:
	virtual void InitHashNodeWeith(const int nWeigh = 500)
	{
		mnWeigh = nWeigh;
	}

	virtual VF_SHARE_PTR<TD> GetElementBySuitRandom()
	{
		VFCVirtualNode<T> vNode;
		if (mxConsistentHash.GetSuitNodeRandom(vNode))
		{
			typename VFMapEx<T, TD>::VFMapOBJECT::iterator itr = VFMapEx<T, TD>::mObjectList.find(vNode.mxData);
			if (itr != VFMapEx<T, TD>::mObjectList.end())
			{
				return itr->second;
			}
		}

		return NULL;
	}

	virtual VF_SHARE_PTR<TD> GetElementBySuitConsistent()
	{
		VFCVirtualNode<T> vNode;
		if (mxConsistentHash.GetSuitNodeConsistent(vNode))
		{
			typename VFMapEx<T, TD>::VFMapOBJECT::iterator itr = VFMapEx<T, TD>::mObjectList.find(vNode.mxData);
			if (itr != VFMapEx<T, TD>::mObjectList.end())
			{
				return itr->second;
			}
		}

		return NULL;
	}

	virtual VF_SHARE_PTR<TD> GetElementBySuit(const T& name)
	{
		VFCVirtualNode<T> vNode;
		if (mxConsistentHash.GetSuitNode(name, vNode))
		{
			typename VFMapEx<T, TD>::VFMapOBJECT::iterator itr = VFMapEx<T, TD>::mObjectList.find(vNode.mxData);
			if (itr != VFMapEx<T, TD>::mObjectList.end())
			{
				return itr->second;
			}
		}

		return NULL;
	}

	virtual bool AddElement(const T& name, const VF_SHARE_PTR<TD> data)
	{
		if (data == nullptr)
		{
			return false;
		}

		typename VFMapEx<T, TD>::VFMapOBJECT::iterator itr = VFMapEx<T, TD>::mObjectList.find(name);
		if (itr == VFMapEx<T, TD>::mObjectList.end())
		{
			VFMapEx<T, TD>::mObjectList.insert(typename VFMapEx<T, TD>::VFMapOBJECT::value_type(name, data));

			mxConsistentHash.Insert(name);

			return true;
		}

		return false;
	}

	virtual bool RemoveElement(const T& name)
	{
		typename VFMapEx<T, TD>::VFMapOBJECT::iterator itr = VFMapEx<T, TD>::mObjectList.find(name);
		if (itr != VFMapEx<T, TD>::mObjectList.end())
		{
			VFMapEx<T, TD>::mObjectList.erase(itr);
			mxConsistentHash.Erase(name);

			return true;
		}

		return false;
	}

	bool ClearAll()
	{
		VFMapEx<T, TD>::mObjectList.clear();
		mxConsistentHash.ClearAll();
		return true;
	}

private:
	int mnWeigh = 0;
	VFCConsistentHash<T> mxConsistentHash;
};
#endif