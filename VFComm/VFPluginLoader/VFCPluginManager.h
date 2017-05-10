// -------------------------------------------------------------------------
//    @FileName			:    VFCPluginManager.h
//    @Author           :    Wenzheng.Xu
//    @Date             :    2017-05-10
//    @Module           :    VFCPluginManager
//
// -------------------------------------------------------------------------

#ifndef VFC_PLUGIN_MANAGER_H
#define VFC_PLUGIN_MANAGER_H

#include <map>
#include <string>
#include <time.h>
#include "VFCDynLib.h"
#include "VFComm/VFCore/VFSingleton.hpp"
#include "VFComm/VFPluginModule/VFIPluginManager.h"

class VFCPluginManager
    : public VFIPluginManager,
	public VFSingleton<VFCPluginManager>
{
public:
    VFCPluginManager();
    virtual ~VFCPluginManager();

	virtual bool Awake();

	virtual bool Init();

    virtual bool AfterInit();

    virtual bool CheckConfig();

    virtual bool ReadyExecute();

    virtual bool BeforeShut();

	virtual bool Shut();

	virtual bool Finalize();


    //////////////////////////////////////////////////////////////////////////

    virtual void Registered(VFIPlugin* pPlugin);

    virtual void UnRegistered(VFIPlugin* pPlugin);

    //////////////////////////////////////////////////////////////////////////

	virtual bool ReLoadPlugin(const std::string& strPluginDLLName);

    virtual VFIPlugin* FindPlugin(const std::string& strPluginName);

    virtual void AddModule(const std::string& strModuleName, VFIModule* pModule);

    virtual void RemoveModule(const std::string& strModuleName);

    virtual VFIModule* FindModule(const std::string& strModuleName);

    virtual bool Execute();

	virtual int GetAppID() const;

    virtual void SetAppID(const int nAppID);

	virtual VFINT64 GetInitTime() const;

	virtual VFINT64 GetNowTime() const;

	virtual const std::string& GetConfigPath() const;

	virtual void SetConfigName(const std::string& strFileName);

	virtual const std::string& GetAppName() const;

	virtual void SetAppName(const std::string& strAppName);

	virtual const std::string& GetLogConfigName() const;

	virtual void SetLogConfigName(const std::string& strName);

	virtual void SetGetFileContentFunctor(GET_FILECONTENT_FUNCTOR fun);

	virtual bool GetFileContent(const std::string &strFileName, std::string &strContent);

protected:
	bool LoadPluginConfig();

	bool LoadStaticPlugin(const std::string& strPluginDLLName);
    bool LoadPluginLibrary(const std::string& strPluginDLLName);
    bool UnLoadPluginLibrary(const std::string& strPluginDLLName);
	bool UnLoadStaticPlugin(const std::string& strPluginDLLName);

private:
    int mnAppID;
    VFINT64 mnInitTime;
    VFINT64 mnNowTime;
    std::string mstrConfigPath;
	std::string mstrConfigName;
	std::string mstrAppName;
	std::string mstrLogConfigName;

    typedef std::map<std::string, bool> PluginNameMap;
    typedef std::map<std::string, VFCDynLib*> PluginLibMap;
    typedef std::map<std::string, VFIPlugin*> PluginInstanceMap;
    typedef std::map<std::string, VFIModule*> ModuleInstanceMap;

    typedef void(* DLL_START_PLUGIN_FUNC)(VFIPluginManager* pm);
    typedef void(* DLL_STOP_PLUGIN_FUNC)(VFIPluginManager* pm);

    PluginNameMap mPluginNameMap;
    PluginLibMap mPluginLibMap;
    PluginInstanceMap mPluginInstanceMap;
    ModuleInstanceMap mModuleInstanceMap;

	GET_FILECONTENT_FUNCTOR mGetFileContentFunctor;
};

#endif
