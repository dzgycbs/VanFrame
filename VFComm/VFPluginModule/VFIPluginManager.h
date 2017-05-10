// -------------------------------------------------------------------------
//    @FileName			:    VFIPluginManager.h
//    @Author           :    Wenzheng.Xu
//    @Date             :    2017-05-10
//    @Module           :    VFIPluginManager
//
// -------------------------------------------------------------------------

#ifndef VFI_PLUGIN_MANAGER_H
#define VFI_PLUGIN_MANAGER_H

#include "VFIModule.h"

typedef std::function<bool (const std::string &strFileName, std::string &strContent)> GET_FILECONTENT_FUNCTOR;

#define FIND_MODULE(classBaseName, className)  \
	assert((TIsDerived<classBaseName, VFIModule>::Result));

class VFIPlugin;

class VFIPluginManager : public VFIModule
{
public:
    VFIPluginManager()
    {

    }

	template <typename T>
	T* FindModule()
	{
		VFIModule* pLogicModule = FindModule(typeid(T).name());
		if (pLogicModule)
		{
			if (!TIsDerived<T, VFIModule>::Result)
			{
				return NULL;
			}

			T* pT = dynamic_cast<T*>(pLogicModule);
			assert(NULL != pT);

			return pT;
		}
		assert(NULL);
		return NULL;
	}

	virtual bool ReLoadPlugin(const std::string& strPluginDLLName) = 0;

    virtual void Registered(VFIPlugin* plugin) = 0;

    virtual void UnRegistered(VFIPlugin* plugin) = 0;

    virtual VFIPlugin* FindPlugin(const std::string& strPluginName) = 0;

    virtual void AddModule(const std::string& strModuleName, VFIModule* pModule) = 0;

    virtual void RemoveModule(const std::string& strModuleName) = 0;

    virtual VFIModule* FindModule(const std::string& strModuleName) = 0;

    virtual int GetAppID() const = 0;
    virtual void SetAppID(const int nAppID) = 0;

    virtual VFINT64 GetInitTime() const = 0;
    virtual VFINT64 GetNowTime() const = 0;

	virtual const std::string& GetConfigPath() const = 0;
	virtual void SetConfigName(const std::string& strFileName) = 0;

	virtual const std::string& GetAppName() const = 0;
	virtual void SetAppName(const std::string& strAppName) = 0;

	virtual const std::string& GetLogConfigName() const = 0;
	virtual void SetLogConfigName(const std::string& strName) = 0;

	virtual void SetGetFileContentFunctor(GET_FILECONTENT_FUNCTOR fun) = 0;
	virtual bool GetFileContent(const std::string &strFileName, std::string &strContent) = 0;
};

#endif
