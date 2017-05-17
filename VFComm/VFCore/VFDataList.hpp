// -------------------------------------------------------------------------
//    @FileName         :    VFDataList.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    VFDataList
//
// -------------------------------------------------------------------------

#ifndef VF_DATALIST_H
#define VF_DATALIST_H

#include <cstdarg>
#include <string>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <assert.h>
#include <map>
#include <typeinfo>
#include <iomanip>
#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
#include "VFMemoryCounter.hpp"
#include "common/variant.hpp"
#include "VFComm/VFPluginModule/VFGUID.h"
#include "VFComm/VFPluginModule/VFPlatform.h"


enum VFDATA_TYPE
{
    TDATA_UNKNOWN,  
    TDATA_INT,      
    TDATA_FLOAT,    
    TDATA_STRING,   
    TDATA_OBJECT,   
	TDATA_VECTOR2,
	TDATA_VECTOR3,
    TDATA_MAX,
};

const static std::string NULL_STR = "";
const static VFGUID NULL_OBJECT = VFGUID();
const static double NULL_FLOAT = 0.0;
const static VFINT64 NULL_INT = 0;
//const static VFVector2 NULL_VECTOR2 = VFVector2();
//const static VFVector3 NULL_VECTOR3 = VFVector3();

struct VFData
{
public:
	VFData()
	{
		nType = TDATA_UNKNOWN;
	}

	VFData(VFDATA_TYPE eType)
	{
		nType = eType;
	}

	VFData(const VFData& value)
	{
		nType = value.nType;
		variantData = value.variantData;
	}

	~VFData()
	{
		nType = TDATA_UNKNOWN;
	}

	inline bool operator==(const VFData& src) const
	{
		//&& src.variantData == variantData
		if (src.GetType() == GetType())
		{
			switch (GetType())
			{
			case TDATA_INT:
			{
				if (src.GetInt() == GetInt())
				{
					return true;
				}
			}
			break;
			case TDATA_FLOAT:
			{
				double fValue = GetFloat() - src.GetFloat();
				if (fValue < 0.001  && fValue > -0.001)
				{
					return true;
				}
			}
			break;
			case TDATA_STRING:
			{
				if (src.GetString() == GetString())
				{
					return true;
				}
			}
			break;
			case TDATA_OBJECT:
			{
				if (src.GetObject() == GetObject())
				{
					return true;
				}
			}
			break;
			//case TDATA_VECTOR2:
			//{
			//	if (src.GetVector2() == GetVector2())
			//	{
			//		return true;
			//	}
			//}
			//break;
			//case TDATA_VECTOR3:
			//{
			//	if (src.GetVector3() == GetVector3())
			//	{
			//		return true;
			//	}
			//}
			break;
			default:
				break;
			}
		}

		return false;
	}

	void Reset()
	{
		nType = TDATA_UNKNOWN;
	}

	bool IsNullValue() const
	{
		bool bChanged = false;

		switch (GetType())
		{
		case TDATA_INT:
		{
			if (0 != GetInt())
			{
				bChanged = true;
			}
		}
		break;
		case TDATA_FLOAT:
		{
			double fValue = GetFloat();
			if (fValue > 0.001 || fValue < -0.001)
			{
				bChanged = true;
			}
		}
		break;
		case TDATA_STRING:
		{
			const std::string& strData = GetString();
			if (!strData.empty())
			{
				bChanged = true;
			}
		}
		break;
		case TDATA_OBJECT:
		{
			if (!GetObject().IsNull())
			{
				bChanged = true;
			}
		}
		break;
		//case TDATA_VECTOR2:
		//{
		//	if (!GetVector2().IsZero())
		//	{
		//		bChanged = true;
		//	}
		//}
		//break;
		//case TDATA_VECTOR3:
		//{
		//	if (!GetVector3().IsZero())
		//	{
		//		bChanged = true;
		//	}
		//}
		break;
		default:
			break;
		}

		return !bChanged;
	}

	VFDATA_TYPE GetType() const
	{
		return nType;
	}


	void SetInt(const VFINT64 var)
	{
		if (nType == TDATA_INT || TDATA_UNKNOWN == nType)
		{
			nType = TDATA_INT;
			variantData = (VFINT64)var;
		}
	}

	void SetFloat(const double var)
	{
		if (nType == TDATA_FLOAT || TDATA_UNKNOWN == nType)
		{
			nType = TDATA_FLOAT;
			variantData = (double)var;
		}
	}

	void SetString(const std::string& var)
	{
		if (nType == TDATA_STRING || TDATA_UNKNOWN == nType)
		{
			nType = TDATA_STRING;
			variantData = (std::string)var;
		}
	}

	void SetObject(const VFGUID var)
	{
		if (nType == TDATA_OBJECT || TDATA_UNKNOWN == nType)
		{
			nType = TDATA_OBJECT;
			variantData = (VFGUID)var;
		}
	}

	//void SetVector2(const VFVector2 var)
	//{
	//	if (nType == TDATA_VECTOR2 || TDATA_UNKNOWN == nType)
	//	{
	//		nType = TDATA_VECTOR2;
	//		variantData = (VFVector2)var;
	//	}
	//}

	//void SetVector3(const VFVector3 var)
	//{
	//	if (nType == TDATA_VECTOR3 || TDATA_UNKNOWN == nType)
	//	{
	//		nType = TDATA_VECTOR3;
	//		variantData = (VFVector3)var;
	//	}
	//}

	VFINT64 GetInt() const
	{
		if (TDATA_INT == nType)
		{
			//return boost::get<VFINT64>(variantData);
			return variantData.get<VFINT64>();
		}

		return NULL_INT;
	}

	double GetFloat() const
	{
		if (TDATA_FLOAT == nType)
		{
			//return boost::get<double>(variantData);
			return variantData.get<double>();
		}

		return NULL_FLOAT;
	}
	const std::string& GetString() const
	{
		if (TDATA_STRING == nType)
		{
			//return boost::get<const std::string&>(variantData);
			return variantData.get<std::string>();
		}

		return NULL_STR;
	}

	const char* GetCharArr() const
	{
		if (TDATA_STRING == nType)
		{
			//return boost::get<const std::string&>(variantData);
			return variantData.get<std::string>().c_str();
		}

		return NULL_STR.c_str();
	}

	const VFGUID& GetObject() const
	{
		if (TDATA_OBJECT == nType)
		{
			//return boost::get<const VFGUID&>(variantData);
			return variantData.get<VFGUID>();
		}

		return NULL_OBJECT;
	}

	//const VFVector2& GetVector2() const
	//{
	//	if (TDATA_VECTOR2 == nType)
	//	{
	//		return variantData.get<VFVector2>();
	//	}

	//	return NULL_VECTOR2;
	//}

	//const VFVector3& GetVector3() const
	//{
	//	if (TDATA_VECTOR3 == nType)
	//	{
	//		return variantData.get<VFVector3>();
	//	}

	//	return NULL_VECTOR3;
	//}

	std::string StringValEx() const
	{
		std::string strData;

		switch (nType)
		{
		case TDATA_INT:
			strData = lexical_cast<std::string> (GetInt());
			break;

		case TDATA_FLOAT:
			strData = lexical_cast<std::string> (GetFloat());
			break;

		case TDATA_STRING:
			strData = GetString();
			break;

		case TDATA_OBJECT:
			strData = GetObject().ToString();
			break;

		//case TDATA_VECTOR2:
		//	strData = GetVector2().ToString();
		//	break;

		//case TDATA_VECTOR3:
		//	strData = GetVector3().ToString();
		//	break;

		default:
			strData = NULL_STR;
			break;
		}
		return strData;
	}

private:

	VFDATA_TYPE nType;

public:
	//std::variant
	mapbox::util::variant<VFINT64, double, std::string, VFGUID> variantData;
};

class VFDataList :public VFMemoryCounter<VFDataList>
{
public:
    VFDataList() : VFMemoryCounter(GET_CLASS_NAME(VFDataList))
    {
        mnUseSize = 0;
        mvList.reserve(STACK_SIZE);
        for (int i = 0; i < STACK_SIZE; ++i)
        {
            mvList.push_back(VF_SHARE_PTR<VFData>(VF_NEW VFData()));
        }
    }

	virtual ~VFDataList()
	{
	}

	virtual std::string StringValEx(const int index) const
	{

		if (ValidIndex(index))
		{
			std::string strData;

			const VFDATA_TYPE eType = Type(index);
			switch (eType)
			{
			case TDATA_INT:
				strData = lexical_cast<std::string> (Int(index));
				break;

			case TDATA_FLOAT:
				strData = lexical_cast<std::string> (Float(index));
				break;

			case TDATA_STRING:
				strData = String(index);
				break;

			case TDATA_OBJECT:
				strData = Object(index).ToString();
				break;

			//case TDATA_VECTOR2:
			//	strData = Vector2(index).ToString();
			//	break;

			//case TDATA_VECTOR3:
			//	strData = Vector3(index).ToString();
			//	break;

			default:
				strData = NULL_STR;
				break;
			}
		}

		return NULL_STR;
	}

	virtual bool ToString(std::string& str, const std::string& strSplit) const
	{
		for (int i = 0; i < GetCount(); ++i)
		{
			std::string strVal = StringValEx(i);
			str += strVal;
			str += strSplit;
		}

		std::string strTempSplit(strSplit);
		std::string::size_type nPos = str.rfind(strSplit);
		if (nPos == str.length() - strTempSplit.length())
		{
			str = str.substr(0, nPos);
		}

		return true;
	}

public:

	virtual const VF_SHARE_PTR<VFData> GetStack(const int index) const
	{
		if (index < mvList.size())
		{
			return mvList[index];
		}

		return VF_SHARE_PTR<VFData>();
	}

	virtual bool Concat(const VFDataList& src)
	{
		InnerAppendEx(src, 0, src.GetCount());
		return true;
	}
    
	virtual bool Append(const VFDataList& src)
	{
		return Append(src, 0, src.GetCount());
	}

	virtual bool Append(const VFDataList& src, const int start, const int count)
	{
		if (start >= src.GetCount())
		{
			return false;
		}

		int end = start + count;

		if (end > src.GetCount())
		{
			return false;
		}

		InnerAppendEx(src, start, end);

		return true;
	}
    
	virtual bool Append(const VFData& xData)
	{
		if (xData.GetType() <= TDATA_UNKNOWN
			|| xData.GetType() >= TDATA_MAX)
		{
			return false;
		}

		switch (xData.GetType())
		{
		case TDATA_INT:
			AddInt(xData.GetInt());
			break;
		case TDATA_FLOAT:
			AddFloat(xData.GetFloat());
			break;
		case TDATA_OBJECT:
			AddObject(xData.GetObject());
			break;
		case TDATA_STRING:
			AddString(xData.GetString());
			break;
		//case TDATA_VECTOR2:
		//	AddVector2(xData.GetVector2());
		//	break;
		//case TDATA_VECTOR3:
		//	AddVector3(xData.GetVector3());
			break;
		default:
			break;
		}

		return true;
	}
    
	virtual void Clear()
	{
		mnUseSize = 0;

		if (mvList.size() > STACK_SIZE)
		{
			for (int i = 0; i < STACK_SIZE; ++i)
			{
				mvList[i]->Reset();
			}

			mvList.erase(mvList.begin() + 8, mvList.end());
		}
	}
    
	virtual bool IsEmpty() const
	{
		return (0 == mnUseSize);
	}
    
	virtual int GetCount() const
	{
		return mnUseSize;
	}
    
	virtual VFDATA_TYPE Type(const int index) const
	{
		if (!ValidIndex(index))
		{
			return TDATA_UNKNOWN;
		}

		if (index < STACK_SIZE)
		{
			return mvList[index]->GetType();
		}
		else
		{
			const VF_SHARE_PTR<VFData> pData = GetStack(index);
			if (pData)
			{
				return pData->GetType();
			}
		}

		return TDATA_UNKNOWN;
	}
    
	virtual bool TypeEx(const  int nType, ...) const
	{

		bool bRet = true;

		if (TDATA_UNKNOWN == nType)
		{
			bRet = false;
			return bRet;
		}

		VFDATA_TYPE pareType = (VFDATA_TYPE)nType;
		va_list arg_ptr;
		va_start(arg_ptr, nType);
		int index = 0;

		while (pareType != TDATA_UNKNOWN)
		{

			VFDATA_TYPE varType = Type(index);
			if (varType != pareType)
			{
				bRet = false;
				break;
			}

			++index;
			pareType = (VFDATA_TYPE)va_arg(arg_ptr, int);
		}

		va_end(arg_ptr);

		return bRet;
	}
    
	virtual bool Split(const std::string& str, const std::string& strSplit)
	{

		Clear();

		std::string strData(str);
		if (strData.empty())
		{
			return true;
		}

		std::string temstrSplit(strSplit);
		std::string::size_type pos;
		strData += temstrSplit;
		std::string::size_type size = strData.length();

		for (std::string::size_type i = 0; i < size; i++)
		{
			pos = int(strData.find(temstrSplit, i));
			if (pos < size)
			{
				std::string strSub = strData.substr(i, pos - i);
				Add(strSub.c_str());

				i = pos + temstrSplit.size() - 1;
			}
		}

		return true;
	}

    
	virtual bool Add(const VFINT64 value)
	{
		if (GetCount() == mvList.size())
		{
			AddStatck();
		}

		VF_SHARE_PTR<VFData> var = GetStack(GetCount());
		if (var)
		{
			var->SetInt(value);
			mnUseSize++;

			return true;
		}

		return false;
	}

	virtual bool Add(const double value)
	{
		if (GetCount() == mvList.size())
		{
			AddStatck();
		}

		VF_SHARE_PTR<VFData> var = GetStack(GetCount());
		if (var)
		{
			var->SetFloat(value);
			mnUseSize++;

			return true;
		}

		return false;
	}

	virtual bool Add(const std::string& value)
	{
		if (GetCount() == mvList.size())
		{
			AddStatck();
		}

		VF_SHARE_PTR<VFData> var = GetStack(GetCount());
		if (var)
		{
			var->SetString(value);
			mnUseSize++;

			return true;
		}

		return false;
	}

	virtual bool Add(const VFGUID& value)
	{
		if (GetCount() == mvList.size())
		{
			AddStatck();
		}

		VF_SHARE_PTR<VFData> var = GetStack(GetCount());
		if (var)
		{
			var->SetObject(value);
			mnUseSize++;

			return true;
		}

		return false;
	}

	//virtual bool Add(const VFVector2& value)
	//{
	//	if (GetCount() == mvList.size())
	//	{
	//		AddStatck();
	//	}

	//	VF_SHARE_PTR<VFData> var = GetStack(GetCount());
	//	if (var)
	//	{
	//		var->SetVector2(value);
	//		mnUseSize++;

	//		return true;
	//	}

	//	return false;
	//}

	//virtual bool Add(const VFVector3& value)
	//{
	//	if (GetCount() == mvList.size())
	//	{
	//		AddStatck();
	//	}

	//	VF_SHARE_PTR<VFData> var = GetStack(GetCount());
	//	if (var)
	//	{
	//		var->SetVector3(value);
	//		mnUseSize++;

	//		return true;
	//	}

	//	return false;
	//}

	virtual bool Set(const int index, const VFINT64 value)
	{
		if (ValidIndex(index) && Type(index) == TDATA_INT)
		{
			VF_SHARE_PTR<VFData> var = GetStack(index);
			if (var)
			{
				var->SetInt(value);

				return true;
			}
		}

		return false;
	}

	virtual bool Set(const int index, const double value)
	{
		if (ValidIndex(index) && Type(index) == TDATA_FLOAT)
		{
			VF_SHARE_PTR<VFData> var = GetStack(index);
			if (var)
			{
				var->SetFloat(value);

				return true;
			}
		}

		return false;
	}

	virtual bool Set(const int index, const std::string& value)
	{
		if (ValidIndex(index) && Type(index) == TDATA_STRING)
		{
			VF_SHARE_PTR<VFData> var = GetStack(index);
			if (var)
			{
				var->SetString(value);

				return true;
			}
		}

		return false;
	}

	virtual bool Set(const int index, const VFGUID& value)
	{
		if (ValidIndex(index) && Type(index) == TDATA_OBJECT)
		{
			VF_SHARE_PTR<VFData> var = GetStack(index);
			if (var)
			{
				var->SetObject(value);

				return true;
			}
		}

		return false;
	}

	//virtual bool Set(const int index, const VFVector2& value)
	//{
	//	if (ValidIndex(index) && Type(index) == TDATA_VECTOR2)
	//	{
	//		VF_SHARE_PTR<VFData> var = GetStack(index);
	//		if (var)
	//		{
	//			var->SetVector2(value);

	//			return true;
	//		}
	//	}

	//	return false;
	//}

	//virtual bool Set(const int index, const VFVector3& value)
	//{
	//	if (ValidIndex(index) && Type(index) == TDATA_VECTOR3)
	//	{
	//		VF_SHARE_PTR<VFData> var = GetStack(index);
	//		if (var)
	//		{
	//			var->SetVector3(value);

	//			return true;
	//		}
	//	}

	//	return false;
	//}

    
	virtual VFINT64 Int(const int index) const
	{
		if (ValidIndex(index))
		{
			if (Type(index) == TDATA_INT)
			{
				const VF_SHARE_PTR<VFData> var = GetStack(index);
				return var->GetInt();
			}
		}

		return 0;
	}

	virtual double Float(const int index) const
	{
		if (ValidIndex(index))
		{
			const VF_SHARE_PTR<VFData> var = mvList[index];
			if (var && TDATA_FLOAT == var->GetType())
			{
				return var->GetFloat();
			}
		}

		return 0.0f;
	}

	virtual const std::string& String(const int index) const
	{
		if (ValidIndex(index))
		{
			const VF_SHARE_PTR<VFData> var = mvList[index];
			if (var && TDATA_STRING == var->GetType())
			{
				return var->GetString();
			}
		}

		return NULL_STR;
	}

	virtual const VFGUID& Object(const int index) const
	{
		if (ValidIndex(index))
		{
			VFDATA_TYPE type = Type(index);
			if (TDATA_OBJECT == type)
			{
				VF_SHARE_PTR<VFData> var = GetStack(index);
				if (var)
				{
					return var->GetObject();
				}
			}
		}

		return NULL_OBJECT;
	}

	//virtual const VFVector2& Vector2(const int index) const
	//{
	//	if (ValidIndex(index))
	//	{
	//		VFDATA_TYPE type = Type(index);
	//		if (TDATA_VECTOR2 == type)
	//		{
	//			VF_SHARE_PTR<VFData> var = GetStack(index);
	//			if (var)
	//			{
	//				return var->GetVector2();
	//			}
	//		}
	//	}

	//	return NULL_VECTOR2;
	//}

	//virtual const VFVector3& Vector3(const int index) const
	//{
	//	if (ValidIndex(index))
	//	{
	//		VFDATA_TYPE type = Type(index);
	//		if (TDATA_VECTOR3 == type)
	//		{
	//			VF_SHARE_PTR<VFData> var = GetStack(index);
	//			if (var)
	//			{
	//				return var->GetVector3();
	//			}
	//		}
	//	}

	//	return NULL_VECTOR3;
	//}

    bool AddInt(const VFINT64 value)
    {
        return Add(value);
    }
    bool AddFloat(const double value)
    {
        return Add(value);
    }
    bool AddString(const std::string& value)
    {
        return Add(value);
    }
    bool AddStringFromChar(const char* value)
    {
        return Add(value);
    }
    bool AddObject(const VFGUID& value)
    {
        return Add(value);
    }
	//bool AddVector2(const VFVector2& value)
	//{
	//	return Add(value);
	//}
	//bool AddVector3(const VFVector3& value)
	//{
	//	return Add(value);
	//}

    bool SetInt(const int index, const VFINT64 value)
    {
        return Set(index, value);
    }
    bool SetFloat(const int index, const double value)
    {
        return Set(index, value);
    }
    bool SetString(const int index, const std::string& value)
    {
        return Set(index, value);
    }
    bool SetObject(const int index, const VFGUID& value)
    {
        return Set(index, value);
    }
	//bool SetVector2(const int index, const VFVector2& value)
	//{
	//	return Set(index, value);
	//}
	//bool SetVector3(const int index, const VFVector3& value)
	//{
	//	return Set(index, value);
	//}



    inline bool Compare(const int nPos, const VFDataList& src) const
    {
        if (src.GetCount() > nPos
            && GetCount() > nPos
            && src.Type(nPos) == Type(nPos))
        {
            switch (src.Type(nPos))
            {
                case TDATA_INT:
                    return Int(nPos) == src.Int(nPos);
                    break;

                case TDATA_FLOAT:
                    return fabs(Float(nPos) - src.Float(nPos)) < 0.001f;
                    break;

                case TDATA_STRING:
                    return String(nPos) == src.String(nPos);
                    break;

                case TDATA_OBJECT:
                    return Object(nPos) == src.Object(nPos);
                    break;

				//case TDATA_VECTOR2:
				//	return Vector2(nPos) == src.Vector2(nPos);
				//	break;

				//case TDATA_VECTOR3:
				//	return Vector3(nPos) == src.Vector3(nPos);
				//	break;

                default:
                    return false;
                    break;
            }
        }

        return false;
    }


    inline bool operator==(const VFDataList& src) const
    {
        if (src.GetCount() == GetCount())
        {
            for (int i = 0; i < GetCount(); i++)
            {
                if (!Compare(i, src))
                {
                    return false;
                }
            }

            return true;
        }

        return false;
    }

    inline bool operator!=(const VFDataList& src)
    {
        return !(*this == src);
    }
    inline VFDataList& operator<<(const double value)
    {
        Add(value);
        return *this;
    }
    inline VFDataList& operator<<(const char* value)
    {
        Add(value);
        return *this;
    }
    inline VFDataList& operator<<(const std::string& value)
    {
        Add(value);
        return *this;
    }

    inline VFDataList& operator<<(const VFINT64& value)
    {
        Add(value);
        return *this;
    }
    inline VFDataList& operator<<(const int value)
    {
        Add((VFINT64)value);
        return *this;
    }
    inline VFDataList& operator<<(const VFGUID& value)
    {
        Add(value);
        return *this;
    }
	//inline VFDataList& operator<<(const VFVector2& value)
	//{
	//	Add(value);
	//	return *this;
	//}
	//inline VFDataList& operator<<(const VFVector3& value)
	//{
	//	Add(value);
	//	return *this;
	//}
    inline VFDataList& operator<<(const VFDataList& value)
    {
        Concat(value);
        return *this;
    }
    enum { STACK_SIZE = 8 };

protected:

	bool ValidIndex(int index) const
	{
		return (index < GetCount()) && (index >= 0);
	}

	void AddStatck()
	{
		for (int i = 0; i < STACK_SIZE; ++i)
		{
			VF_SHARE_PTR<VFData> pData(VF_NEW  VFData());
			mvList.push_back(pData);
		}
	}

	void InnerAppendEx(const VFDataList& src, const int start, const int end)
	{
		for (int i = start; i < end; ++i)
		{
			VFDATA_TYPE vType = src.Type(i);
			switch (vType)
			{
			case TDATA_INT:
				AddInt(src.Int(i));
				break;
			case TDATA_FLOAT:
				AddFloat(src.Float(i));
				break;
			case TDATA_STRING:
				AddString(src.String(i));
				break;
			case TDATA_OBJECT:
				AddObject(src.Object(i));
				break;
			//case TDATA_VECTOR2:
			//	AddVector2(src.Vector2(i));
			//	break;
			//case TDATA_VECTOR3:
			//	AddVector3(src.Vector3(i));
				break;
			default:
				break;
			}
		}
	}
protected:

    int mnUseSize;
    std::vector< VF_SHARE_PTR<VFData> > mvList;
    std::map<std::string, VF_SHARE_PTR<VFData> > mxMap;
};

const static VFData NULL_TDATA = VFData();

#endif