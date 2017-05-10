// -------------------------------------------------------------------------
//    @FileName			:    VFCPluginManager.cpp
//    @Author           :    Wenzheng.Xu
//    @Date             :    2017-05-10
//    @Module           :    VFCPluginManager
//
// -------------------------------------------------------------------------

#include "VFCPluginManager.h"
#include "Dependencies/RapidXML/rapidxml.hpp"
#include "Dependencies/RapidXML/rapidxml_iterators.hpp"
#include "Dependencies/RapidXML/rapidxml_print.hpp"
#include "Dependencies/RapidXML/rapidxml_utils.hpp"
#include "VFComm/VFPluginModule/VFIPlugin.h"
#include "VFComm/VFPluginModule/VFPlatform.h"

#if VF_PLATFORM == VF_PLATFORM_WIN
#pragma comment( lib, "ws2_32.lib" )
#endif

#ifdef VF_DEBUG_MODE

#if VF_PLATFORM == VF_PLATFORM_WIN

#elif VF_PLATFORM == VF_PLATFORM_LINUX || VF_PLATFORM == VF_PLATFORM_ANDROID
#pragma comment( lib, "libtherond.a" )
#elif VF_PLATFORM == VF_PLATFORM_APPLE || VF_PLATFORM == VF_PLATFORM_APPLE_IOS
#pragma comment( lib, "libtherond.a" )
#endif

#else

#if VF_PLATFORM == VF_PLATFORM_WIN

#elif VF_PLATFORM == VF_PLATFORM_LINUX || VF_PLATFORM == VF_PLATFORM_ANDROID
#pragma comment( lib, "libtheron.a" )
#elif VF_PLATFORM == VF_PLATFORM_APPLE || VF_PLATFORM == VF_PLATFORM_APPLE_IOS
#pragma comment( lib, "libtheron.a" )
#endif

#endif

VFCPluginManager::VFCPluginManager() : VFIPluginManager()
{
   mnAppID = 0;
   mnInitTime = time(NULL);
   mnNowTime = mnInitTime;

   mGetFileContentFunctor = nullptr;

   mstrConfigPath = "";
   mstrConfigName = "Plugin.xml";
}

VFCPluginManager::~VFCPluginManager()
{

}

bool VFCPluginManager::Awake()
{
	LoadPluginConfig();

	PluginNameMap::iterator it = mPluginNameMap.begin();
	for (it; it != mPluginNameMap.end(); ++it)
	{
#ifdef VF_DYNAMIC_PLUGIN
		LoadPluginLibrary(it->first);
#else
		LoadStaticPlugin(it->first);
#endif
	}


	PluginInstanceMap::iterator itAfterInstance = mPluginInstanceMap.begin();
	for (itAfterInstance; itAfterInstance != mPluginInstanceMap.end(); itAfterInstance++)
	{
		itAfterInstance->second->Awake();
	}

	return true;
}

inline bool VFCPluginManager::Init()
{
	PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
	for (itInstance; itInstance != mPluginInstanceMap.end(); itInstance++)
	{
		itInstance->second->Init();
	}

	return true;
}

bool VFCPluginManager::LoadPluginConfig()
{
	std::string strContent;
	GetFileContent(mstrConfigName, strContent);

	rapidxml::xml_document<> xDoc;
	xDoc.parse<0>((char*)strContent.c_str());

    rapidxml::xml_node<>* pRoot = xDoc.first_node();
    rapidxml::xml_node<>* pAppNameNode = pRoot->first_node(mstrAppName.c_str());
    if (!pAppNameNode)
    {
        VFASSERT(0, "There are no App ID", __FILE__, __FUNCTION__);
        return false;
    }

    for (rapidxml::xml_node<>* pPluginNode = pAppNameNode->first_node("Plugin"); pPluginNode; pPluginNode = pPluginNode->next_sibling("Plugin"))
    {
        const char* strPluginName = pPluginNode->first_attribute("Name")->value();

        mPluginNameMap.insert(PluginNameMap::value_type(strPluginName, true));

    }

/*
    rapidxml::xml_node<>* pPluginAppNode = pAppNameNode->first_node("APPID");
    if (!pPluginAppNode)
    {
        VFASSERT(0, "There are no App ID", __FILE__, __FUNCTION__);
        return false;
    }

    const char* strAppID = pPluginAppNode->first_attribute("Name")->value();
    if (!strAppID)
    {
        VFASSERT(0, "There are no App ID", __FILE__, __FUNCTION__);
        return false;
    }

    if (!VF_StrTo(strAppID, mnAppID))
    {
        VFASSERT(0, "App ID Convert Error", __FILE__, __FUNCTION__);
        return false;
    }
*/
    rapidxml::xml_node<>* pPluginConfigPathNode = pAppNameNode->first_node("ConfigPath");
    if (!pPluginConfigPathNode)
    {
		VFASSERT(0, "There are no ConfigPath", __FILE__, __FUNCTION__);
        return false;
    }

    if (NULL == pPluginConfigPathNode->first_attribute("Name"))
    {
		VFASSERT(0, "There are no ConfigPath.Name", __FILE__, __FUNCTION__);
        return false;
    }

    mstrConfigPath = pPluginConfigPathNode->first_attribute("Name")->value();

    return true;
}

bool VFCPluginManager::LoadStaticPlugin(const std::string& strPluginDLLName)
{
	//     PluginNameList::iterator it = mPluginNameList.begin();
	//     for (it; it != mPluginNameList.end(); it++)
	//     {
	//         const std::string& strPluginName = *it;
	//         CREATE_PLUGIN( this, strPluginName );
	//     }

	//     CREATE_PLUGIN(this, VFKernelPlugin)
	//     CREATE_PLUGIN(this, VFEventProcessPlugin)
	//     CREATE_PLUGIN(this, VFConfigPlugin)

	return false;
}

void VFCPluginManager::Registered(VFIPlugin* plugin)
{
    std::string strPluginName = plugin->GetPluginName();
    if (!FindPlugin(strPluginName))
    {
		// dynamic add plugin no dlls
        //bool bFind = false;
        //PluginNameMap::iterator it = mPluginNameMap.begin();
        //for (it; it != mPluginNameMap.end(); ++it)
        //{
        //    if (strPluginName == it->first)
        //    {
        //        bFind = true;
        //        break;
        //    }
        //}

        //if (bFind)
        {
            mPluginInstanceMap.insert(PluginInstanceMap::value_type(strPluginName, plugin));
            plugin->Install();
        }
    }
}

void VFCPluginManager::UnRegistered(VFIPlugin* plugin)
{
    PluginInstanceMap::iterator it = mPluginInstanceMap.find(plugin->GetPluginName());
    if (it != mPluginInstanceMap.end())
    {
        it->second->Uninstall();
        delete it->second;
        it->second = NULL;
        mPluginInstanceMap.erase(it);
    }
}

bool VFCPluginManager::ReLoadPlugin(const std::string & strPluginDLLName)
{
	//1.shut all module of this plugin
	//2.unload this plugin
	//3.load new plugin
	//4.init new module
	//5.tell others who has been reloaded
	PluginInstanceMap::iterator itInstance = mPluginInstanceMap.find(strPluginDLLName);
	if (itInstance == mPluginInstanceMap.end())
	{
		return false;
	}
	//1
	VFIPlugin* pPlugin = itInstance->second;
	VFIModule* pModule = pPlugin->First();
	while (pModule)
	{
		pModule->BeforeShut();
		pModule->Shut();
		pModule->Finalize();
		
		pModule = pPlugin->Next();
	}

	//2
	PluginLibMap::iterator it = mPluginLibMap.find(strPluginDLLName);
	if (it != mPluginLibMap.end())
	{
		VFCDynLib* pLib = it->second;
		DLL_STOP_PLUGIN_FUNC pFunc = (DLL_STOP_PLUGIN_FUNC)pLib->GetSymbol("DllStopPlugin");

		if (pFunc)
		{
			pFunc(this);
		}

		pLib->UnLoad();

		delete pLib;
		pLib = NULL;
		mPluginLibMap.erase(it);
	}

	//3
	VFCDynLib* pLib = new VFCDynLib(strPluginDLLName);
	bool bLoad = pLib->Load();
	if (bLoad)
	{
		mPluginLibMap.insert(PluginLibMap::value_type(strPluginDLLName, pLib));

		DLL_START_PLUGIN_FUNC pFunc = (DLL_START_PLUGIN_FUNC)pLib->GetSymbol("DllStartPlugin");
		if (!pFunc)
		{
			std::cout << "Reload Find function DllStartPlugin Failed in [" << pLib->GetName() << "]" << std::endl;
			assert(0);
			return false;
		}

		pFunc(this);
	}
	else
	{
#if VF_PLATFORM == VF_PLATFORM_LINUX
		char* error = dlerror();
		if (error)
		{
			std::cout << stderr << " Reload shared lib[" << pLib->GetName() << "] failed, ErrorNo. = [" << error << "]" << std::endl;
			std::cout << "Reload [" << pLib->GetName() << "] failed" << std::endl;
			assert(0);
			return false;
		}
#elif VF_PLATFORM == VF_PLATFORM_WIN
		std::cout << stderr << " Reload DLL[" << pLib->GetName() << "] failed, ErrorNo. = [" << GetLastError() << "]" << std::endl;
		std::cout << "Reload [" << pLib->GetName() << "] failed" << std::endl;
		assert(0);
		return false;
#endif // VF_PLATFORM
	}

	//4
	PluginInstanceMap::iterator itReloadInstance = mPluginInstanceMap.begin();
	for (itReloadInstance; itReloadInstance != mPluginInstanceMap.end(); itReloadInstance++)
	{
		if (strPluginDLLName != itReloadInstance->first)
		{
			itReloadInstance->second->OnReloadPlugin();
		}
	}
	return true;
}

VFIPlugin* VFCPluginManager::FindPlugin(const std::string& strPluginName)
{
    PluginInstanceMap::iterator it = mPluginInstanceMap.find(strPluginName);
    if (it != mPluginInstanceMap.end())
    {
        return it->second;
    }

    return NULL;
}

bool VFCPluginManager::Execute()
{
    mnNowTime = time(NULL);

    bool bRet = true;

    PluginInstanceMap::iterator it = mPluginInstanceMap.begin();
    for (; it != mPluginInstanceMap.end(); ++it)
    {
        bool tembRet = it->second->Execute();
        bRet = bRet && tembRet;
    }

    return bRet;
}

inline int VFCPluginManager::GetAppID() const
{
	return mnAppID;
}

inline void VFCPluginManager::SetAppID(const int nAppID)
{
    mnAppID = nAppID;
}

inline VFINT64 VFCPluginManager::GetInitTime() const
{
	return mnInitTime;
}

inline VFINT64 VFCPluginManager::GetNowTime() const
{
	return mnNowTime;
}

inline const std::string & VFCPluginManager::GetConfigPath() const
{
	return mstrConfigPath;
}

void VFCPluginManager::SetConfigName(const std::string & strFileName)
{
	if (strFileName.empty())
	{
		return;
	}

	if (strFileName.find(".xml") == string::npos)
	{
		return;
	}

	mstrConfigName = strFileName;
}

const std::string& VFCPluginManager::GetAppName() const
{
	return mstrAppName;
}

void VFCPluginManager::SetAppName(const std::string& strAppName)
{
	if (!mstrAppName.empty())
	{
		return;
	}

	mstrAppName = strAppName;
}

const std::string & VFCPluginManager::GetLogConfigName() const
{
	return mstrLogConfigName;
}

void VFCPluginManager::SetLogConfigName(const std::string & strName)
{
	mstrLogConfigName = strName;
}

void VFCPluginManager::SetGetFileContentFunctor(GET_FILECONTENT_FUNCTOR fun)
{
	mGetFileContentFunctor = fun;
}

bool VFCPluginManager::GetFileContent(const std::string &strFileName, std::string &strContent)
{
	if (mGetFileContentFunctor)
	{
		return mGetFileContentFunctor(strFileName, strContent);
	}
	FILE *fp = new FILE;
	fopen_s(&fp,strFileName.c_str(), "rb");
	if (!fp)
	{
		return false;
	}

	fseek(fp, 0, SEEK_END);
	const long filelength = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	strContent.resize(filelength);
	fread((void*)strContent.data(), filelength, 1, fp);
	fclose(fp);
	delete fp;
	return true;
}

void VFCPluginManager::AddModule(const std::string& strModuleName, VFIModule* pModule)
{
    if (!FindModule(strModuleName))
    {
        mModuleInstanceMap.insert(ModuleInstanceMap::value_type(strModuleName, pModule));
    }
}

void VFCPluginManager::RemoveModule(const std::string& strModuleName)
{
    ModuleInstanceMap::iterator it = mModuleInstanceMap.find(strModuleName);
    if (it != mModuleInstanceMap.end())
    {
        mModuleInstanceMap.erase(it);
    }
}


VFIModule* VFCPluginManager::FindModule(const std::string& strModuleName)
{
	std::string strSubModuleName = strModuleName;

#if VF_PLATFORM == VF_PLATFORM_WIN
	std::size_t position = strSubModuleName.find(" ");
	if (string::npos != position)
	{
		strSubModuleName = strSubModuleName.substr(position + 1, strSubModuleName.length());
	}
#else
	for (int i = 0; i < strSubModuleName.length(); i++)
	{
		std::string s = strSubModuleName.substr(0, i + 1);
		int n = atof(s.c_str());
		if (strSubModuleName.length() == i + 1 + n)
		{
			strSubModuleName = strSubModuleName.substr(i + 1, strSubModuleName.length());
			break;
		}
	}
#endif

	ModuleInstanceMap::iterator it = mModuleInstanceMap.find(strSubModuleName);
	if (it != mModuleInstanceMap.end())
	{
		return it->second;
	}

    return NULL;
}

bool VFCPluginManager::AfterInit()
{
    PluginInstanceMap::iterator itAfterInstance = mPluginInstanceMap.begin();
    for (itAfterInstance; itAfterInstance != mPluginInstanceMap.end(); itAfterInstance++)
    {
        itAfterInstance->second->AfterInit();
    }

    return true;
}

bool VFCPluginManager::CheckConfig()
{
    PluginInstanceMap::iterator itCheckInstance = mPluginInstanceMap.begin();
    for (itCheckInstance; itCheckInstance != mPluginInstanceMap.end(); itCheckInstance++)
    {
        itCheckInstance->second->CheckConfig();
    }

    return true;
}

bool VFCPluginManager::ReadyExecute()
{
    PluginInstanceMap::iterator itCheckInstance = mPluginInstanceMap.begin();
    for (itCheckInstance; itCheckInstance != mPluginInstanceMap.end(); itCheckInstance++)
    {
        itCheckInstance->second->ReadyExecute();
    }

    return true;
}

bool VFCPluginManager::BeforeShut()
{
    PluginInstanceMap::iterator itBeforeInstance = mPluginInstanceMap.begin();
    for (itBeforeInstance; itBeforeInstance != mPluginInstanceMap.end(); itBeforeInstance++)
    {
        itBeforeInstance->second->BeforeShut();
    }

    return true;
}

bool VFCPluginManager::Shut()
{
    PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
    for (itInstance; itInstance != mPluginInstanceMap.end(); ++itInstance)
    {
        itInstance->second->Shut();
    }

    return true;
}

bool VFCPluginManager::Finalize()
{
	PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
	for (itInstance; itInstance != mPluginInstanceMap.end(); itInstance++)
	{
		itInstance->second->Finalize();
	}

	////////////////////////////////////////////////

	PluginNameMap::iterator it = mPluginNameMap.begin();
	for (it; it != mPluginNameMap.end(); it++)
	{
#ifdef VF_DYNAMIC_PLUGIN
		UnLoadPluginLibrary(it->first);
#else
		UnLoadStaticPlugin(it->first);
#endif
	}

	mPluginInstanceMap.clear();
	mPluginNameMap.clear();

	return true;
}

bool VFCPluginManager::LoadPluginLibrary(const std::string& strPluginDLLName)
{
    PluginLibMap::iterator it = mPluginLibMap.find(strPluginDLLName);
    if (it == mPluginLibMap.end())
    {
        VFCDynLib* pLib = new VFCDynLib(strPluginDLLName);
        bool bLoad = pLib->Load();

        if (bLoad)
        {
            mPluginLibMap.insert(PluginLibMap::value_type(strPluginDLLName, pLib));

            DLL_START_PLUGIN_FUNC pFunc = (DLL_START_PLUGIN_FUNC)pLib->GetSymbol("DllStartPlugin");
            if (!pFunc)
            {
                std::cout << "Find function DllStartPlugin Failed in [" << pLib->GetName() << "]" << std::endl;
                assert(0);
                return false;
            }

            pFunc(this);

            return true;
        }
        else
        {
#if VF_PLATFORM == VF_PLATFORM_LINUX
            char* error = dlerror();
            if (error)
            {
                std::cout << stderr << " Load shared lib[" << pLib->GetName() << "] failed, ErrorNo. = [" << error << "]" << std::endl;
                std::cout << "Load [" << pLib->GetName() << "] failed" << std::endl;
                assert(0);
                return false;
            }
#elif VF_PLATFORM == VF_PLATFORM_WIN
            std::cout << stderr << " Load DLL[" << pLib->GetName() << "] failed, ErrorNo. = [" << GetLastError() << "]" << std::endl;
            std::cout << "Load [" << pLib->GetName() << "] failed" << std::endl;
            assert(0);
            return false;
#endif // VF_PLATFORM
        }
    }

    return false;
}

bool VFCPluginManager::UnLoadPluginLibrary(const std::string& strPluginDLLName)
{
    PluginLibMap::iterator it = mPluginLibMap.find(strPluginDLLName);
    if (it != mPluginLibMap.end())
    {
        VFCDynLib* pLib = it->second;
        DLL_STOP_PLUGIN_FUNC pFunc = (DLL_STOP_PLUGIN_FUNC)pLib->GetSymbol("DllStopPlugin");

        if (pFunc)
        {
            pFunc(this);
        }

        pLib->UnLoad();

        delete pLib;
        pLib = NULL;
        mPluginLibMap.erase(it);

        return true;
    }

    return false;
}

bool VFCPluginManager::UnLoadStaticPlugin(const std::string & strPluginDLLName)
{
	//     DESTROY_PLUGIN(this, VFConfigPlugin)
	//     DESTROY_PLUGIN(this, VFEventProcessPlugin)
	//     DESTROY_PLUGIN(this, VFKernelPlugin)
	return false;
}