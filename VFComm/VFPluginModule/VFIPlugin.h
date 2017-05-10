// -------------------------------------------------------------------------
//    @FileName         :    VFIPlugin.h
//    @Author           :    Wenzheng.Xu
//    @Date             :    2017-05-10
//    @Module           :    VFIPlugin
//
// -------------------------------------------------------------------------

#ifndef VFI_PLUGIN_H
#define VFI_PLUGIN_H

#include <iostream>
#include <assert.h>
#include "VFComm/VFCore/VFMap.hpp"
#include "VFComm/VFPluginModule/VFIModule.h"
#include "VFComm/VFPluginModule/VFPlatform.h"
#include "VFComm/VFPluginModule/VFIPluginManager.h"

#define REGISTER_MODULE(pManager, classBaseName, className)  \
	assert((TIsDerived<classBaseName, VFIModule>::Result));	\
	assert((TIsDerived<className, classBaseName>::Result));	\
	VFIModule* pRegisterModule##className= new className(pManager); \
    pRegisterModule##className->strName = (#className); \
    pManager->AddModule( #classBaseName, pRegisterModule##className );AddElement( #classBaseName, pRegisterModule##className );

#define UNREGISTER_MODULE(pManager, classBaseName, className) VFIModule* pUnRegisterModule##className =  \
	dynamic_cast<VFIModule*>( pManager->FindModule( #classBaseName )); \
	pManager->RemoveModule( #classBaseName ); RemoveElement( #classBaseName ); delete pUnRegisterModule##className;


#define CREATE_PLUGIN(pManager, className)  VFIPlugin* pCreatePlugin##className = new className(pManager); pManager->Registered( pCreatePlugin##className );

#define DESTROY_PLUGIN(pManager, className) pManager->UnRegistered( pManager->FindPlugin((#className)) );

/*
#define REGISTER_COMPONENT(pManager, className)  VFIComponent* pRegisterComponent##className= new className(pManager); \
    pRegisterComponent##className->strName = (#className); \
    pManager->AddComponent( (#className), pRegisterComponent##className );

#define UNREGISTER_COMPONENT(pManager, className) VFIComponent* pRegisterComponent##className =  \
        dynamic_cast<VFIComponent*>( pManager->FindComponent( (#className) ) ); pManager->RemoveComponent( (#className) ); delete pRegisterComponent##className;
*/

class VFIPluginManager;

class VFIPlugin : public VFIModule,
    public VFMap<std::string, VFIModule>
{

public:
	VFIPlugin()
	{
	}

    virtual const int GetPluginVersion() = 0;

    virtual const std::string GetPluginName() = 0;

    virtual void Install() = 0;

	virtual bool Awake()
	{
		VFIModule* pModule = First();
		while (pModule)
		{
			bool bRet = pModule->Awake();
			if (!bRet)
			{
				std::cout << pModule->strName << std::endl;
				assert(0);
			}

			pModule = Next();
		}
		return true;
	}

    virtual bool Init()
    {
        VFIModule* pModule = First();
        while (pModule)
        {
            bool bRet = pModule->Init();
            if (!bRet)
            {
				std::cout << pModule->strName << std::endl;
                assert(0);
            }

            pModule = Next();
        }
        return true;
    }

    virtual bool AfterInit()
    {
        VFIModule* pModule = First();
        while (pModule)
        {
            bool bRet = pModule->AfterInit();
            if (!bRet)
            {
				std::cout << pModule ->strName << std::endl;
                assert(0);
            }

            pModule = Next();
        }
        return true;
    }

    virtual bool CheckConfig()
    {
        VFIModule* pModule = First();
        while (pModule)
        {
            pModule->CheckConfig();

            pModule = Next();
        }

        return true;
    }

	virtual bool ReadyExecute()
	{
		VFIModule* pModule = First();
		while (pModule)
		{
			pModule->ReadyExecute();

			pModule = Next();
		}

		return true;
	}

    virtual bool Execute()
    {
        VFIModule* pModule = First();
        while (pModule)
        {
            pModule->Execute();

            pModule = Next();
        }

        return true;
    }

    virtual bool BeforeShut()
    {
        VFIModule* pModule = First();
        while (pModule)
        {
            pModule->BeforeShut();

            pModule = Next();
        }
        return true;
    }

    virtual bool Shut()
    {
        VFIModule* pModule = First();
        while (pModule)
        {
            pModule->Shut();

            pModule = Next();
        }

        return true;
    }

	virtual bool OnReloadPlugin()
	{
		VFIModule* pModule = First();
		while (pModule)
		{
			pModule->OnReloadPlugin();

			pModule = Next();
		}

		return true;
	}
    virtual void Uninstall() = 0;
};

#endif
