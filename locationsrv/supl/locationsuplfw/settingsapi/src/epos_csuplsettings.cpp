/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation of CSuplSettings..in turn calls CSuplSettingsEngine methods
*
*/



// INCLUDE FILES

#include "epos_csuplsettingsengine.h"
#include "epos_csuplsettings.h"
#include "epos_csuplsettingparams.h"
#include "epos_msuplsettingsobserver.h"
#include "epos_csuplsettingsnotifier.h"
#include "epos_csuplsettingsconstants.h"
#include "epos_suplsessionnotifier.h"

// CONSTANTS

// ==================== LOCAL FUNCTIONS ====================


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplSettings::CSuplSettings():
    iSettingsNotifier(NULL),iSettingsDBChangeNotifier(NULL),iSessionNotifier(NULL)
    {

    }

EXPORT_C CSuplSettings* CSuplSettings::NewL()
    {
    CSuplSettings* self = new (ELeave) CSuplSettings();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

EXPORT_C CSuplSettings* CSuplSettings::NewLC()
    {
    CSuplSettings* self = new (ELeave) CSuplSettings();

    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
// EPOC default constructor can leave.
void CSuplSettings::ConstructL()
    {
    iSettingsEngine = CSuplSettingsEngine::NewL();
    }

// Destructor
EXPORT_C CSuplSettings::~CSuplSettings()
    {
    delete iSettingsNotifier;
    delete iSettingsDBChangeNotifier;
    delete iSettingsEngine; 
    delete iSessionNotifier;
    }
//-------------------------------------------------------------------------------------
//CSuplSettings::SetObserverL()
//
//This method is used to start listening for SUPL setting changes. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::SetObserverL(MSuplSettingsObserver& aObserver)
    {
    if (&aObserver == NULL)
        return KErrArgument;
    
    if(iSettingsDBChangeNotifier)
        {
        return KErrAlreadyExists;
        }
    else
        {
        iSettingsDBChangeNotifier = CSuplSettingsNotifier::NewL(aObserver,ETrue);//create a notifier for DB changes
        }
    
    // If already observing return error to client, if not start observation
    if (iSettingsNotifier)
        {
        return KErrAlreadyExists; 
        }
    else
        {
        iSettingsNotifier = CSuplSettingsNotifier::NewL(aObserver);
        }
   
    return KErrNone;
    }
//-------------------------------------------------------------------------------------
//CSuplSettings::RemoveObserver()
//
//This method is used to stop listening for SUPL setting changes.
//--------------------------------------------------------------------------------------
EXPORT_C void CSuplSettings::RemoveObserver()
    {
    if (iSettingsNotifier)
        {           
        delete iSettingsNotifier;
        iSettingsNotifier = NULL;
        }
    if(iSettingsDBChangeNotifier)
        {
        delete iSettingsDBChangeNotifier;
        iSettingsDBChangeNotifier = NULL;
        }
    }

//-------------------------------------------------------------------------------------
//CSuplSettings::SetSessionObserverL()
//
//This method is used to start listening for SUPL session changes. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::SetSessionObserverL(MSuplSessionObserver& aObserver)
    {
       
    if(iSessionNotifier)
        {
        return KErrAlreadyExists;
        }
    else
        {
        iSessionNotifier = CSuplSessionNotifier::NewL(aObserver);//create a notifier for DB changes
        }
    
    return KErrNone;
    }
//-------------------------------------------------------------------------------------
//CSuplSettings::RemoveSessionObserver()
//
//This method is used to stop listening for SUPL session changes.
//--------------------------------------------------------------------------------------
EXPORT_C void CSuplSettings::RemoveSessionObserver()
    {
    delete iSessionNotifier;
    iSessionNotifier = NULL;
    }

//-------------------------------------------------------------------------------------
//CSuplSettings::Initialize()
//
//This method is invoked to initialize SUPL settings object.
//--------------------------------------------------------------------------------------
EXPORT_C void CSuplSettings::Initialize(TRequestStatus& aStatus)
    {
        iSettingsEngine->Initialize(aStatus);
    }        
//-------------------------------------------------------------------------------------
//CSuplSettings::CancelInitialize()
//
//This method is invoked to cancel Initialize()
//--------------------------------------------------------------------------------------
EXPORT_C void CSuplSettings::CancelInitialize()
    {
        iSettingsEngine->CancelInitialize();
    }    
//-------------------------------------------------------------------------------------
//CSuplSettings::GenerateHslpAddressFromImsi()
//
//This method can be used to generate HSLP address from the IMSI
// obtained during initialization.   
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GenerateHslpAddressFromImsi(TDes& aHslpAddress)
    {
    if (!iSettingsEngine->IsInitDone())
        return KErrNotReady;          

    if (!iSettingsEngine->IsInitPassed())
        return KErrNotReady;          
	
    if (aHslpAddress.MaxLength() == 0)
        return KErrArgument;

    if (aHslpAddress.MaxLength() < 40)
        return KErrOverflow;

    return iSettingsEngine->GetHslpAddressFromImsi(aHslpAddress);
    }
//-------------------------------------------------------------------------------------
//CSuplSettings::GetImsi()
//
//This method returns IMSI obtained during initialization of SUPL settings object. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GetImsi(TDes& aImsi)
    {
    if (!iSettingsEngine->IsInitDone())
        return KErrNotReady;          

    if (!iSettingsEngine->IsInitPassed())
        return KErrNotReady;          

    if (aImsi.MaxLength() == 0)
        return KErrArgument;

    if (aImsi.MaxLength() < KMaxIMSILen)
        return KErrOverflow;            

    return iSettingsEngine->GetImsi(aImsi);
    }        
//-------------------------------------------------------------------------------------
//CSuplSettings::IsImsiChanged()
//
//This method compares IMSI value currently stored in SUPL settings storage with
//IMSI value obtained during initialization.
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::IsImsiChanged(TBool& aChanged)
    {
    if (!iSettingsEngine->IsInitDone())
        return KErrNotReady;          
    if (!iSettingsEngine->IsInitPassed())
        return KErrNotReady;          

    return iSettingsEngine->IsImsiChanged(aChanged);
    }
//-------------------------------------------------------------------------------------
//CSuplSettings::GetSuplUsage()
//
//This method is used to retrieve SUPL usage from settings storage. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GetSuplUsage(TSuplSettingsUsage& /* aUsage */ ) const
    {
    return KErrNotSupported;            
    }            
//-------------------------------------------------------------------------------------
//CSuplSettings::SetSuplUsage()
//
//This method is used to change SUPL usage.
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::SetSuplUsage(const TSuplSettingsUsage /* aUsage */ ) 
    {
    return KErrNotSupported;   
    }          
//-------------------------------------------------------------------------------------
//CSuplSettings::AddNewServer()
//
//This method is used to add new server into settings. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::AddNewServer( const CServerParams* aParamValues, TInt64& aSlpId )
    {
    if(aParamValues == NULL)
        return KErrGeneral;
    return iSettingsEngine->AddNewServer(aParamValues,aSlpId );  
    }

//-------------------------------------------------------------------------------------
//CSuplSettings::SetServerAddress()
//
//This method is used to change server address parameter in server settings 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::SetServerAddress( const TInt64 aSlpId, const TDesC& aServerAddress)
    {
    TBuf<KMaxHSLPAddrLen> serveraddr;
    serveraddr.Copy(aServerAddress);
    serveraddr.TrimAll();
    if(serveraddr.Length() <= 0)
        return KErrArgument;
    return iSettingsEngine->SetServerAddress(aSlpId,serveraddr);          
    }            
//-------------------------------------------------------------------------------------
//CSuplSettings::GetServerAddress()
//
//This method is used to retrieve server address parameter from server settings.
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GetServerAddress(const TInt64 aSlpId, TDes& aServerAddress) const
    {
    return iSettingsEngine->GetServerAddress(aSlpId,aServerAddress);            
    }            
//-------------------------------------------------------------------------------------
//CSuplSettings::SetIapName()
//
//This method is used to change IAP Name in server parameters. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::SetIapName( const TInt64 aSlpId, const TDesC& aIapName)
    {
    TBuf<KMaxIAPLen> iapname;
    iapname.Copy(aIapName);
    iapname.TrimAll();
    if(iapname.Length() <= 0)
            return KErrArgument;
    return iSettingsEngine->SetIapName(aSlpId,iapname);             
    }            
//-------------------------------------------------------------------------------------
//CSuplSettings::GetIapName()
//
//This method is used to retrieve IAP Name in server parameters. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GetIapName( const TInt64 aSlpId, TDes& aIapName) const
    {
    return iSettingsEngine->GetIapName(aSlpId,aIapName);             
    }            
//-------------------------------------------------------------------------------------
//CSuplSettings::SetServerEnabledFlag()
//
//This method is used to change parameter which indicates whether server
// can be used for SUPL session or not. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::SetServerEnabledFlag( const TInt64 aSlpId, const TBool aEnable )
    {
    return iSettingsEngine->SetServerEnabledFlag(aSlpId,aEnable);             
    }            
//-------------------------------------------------------------------------------------
//CSuplSettings::GetServerEnabledFlag()
//
//This method is used to retrieve parameter which indicates whether server
// can be used for SUPL session or not. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GetServerEnabledFlag( const TInt64 aSlpId, TBool& aEnable ) const
    {
    return iSettingsEngine->GetServerEnabledFlag(aSlpId,aEnable );           
    }            
//-------------------------------------------------------------------------------------
//CSuplSettings::SetSimChangeRemoveFlag()
//
//This method is used to change parameter which indicates whether server
// details can be removed if SIM is changed. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::SetSimChangeRemoveFlag( const TInt64 aSlpId, const TBool aSimChangeFlag )
    {
    return iSettingsEngine->SetSimChangeRemoveFlag(aSlpId,aSimChangeFlag );             
    }            
//-------------------------------------------------------------------------------------
//CSuplSettings::GetSimChangeRemoveFlag()
//
//This method is used to retrieve parameter which indicates whether server
//details can be removed if SIM is changed. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GetSimChangeRemoveFlag( const TInt64 aSlpId, TBool& aSimChangeFlag ) const        
    {
    return iSettingsEngine->GetSimChangeRemoveFlag(aSlpId,aSimChangeFlag );             
    }            
//-------------------------------------------------------------------------------------
//CSuplSettings::SetUsageInHomwNwFlag()
//
//This method is used to change parameter which indicates whether server
// can be used out side home network or not.
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::SetUsageInHomwNwFlag( const TInt64 aSlpId, const TBool aHomeNwFlag )
    {
    return iSettingsEngine->SetUsageInHomwNwFlag(aSlpId,aHomeNwFlag );             
    }            
//-------------------------------------------------------------------------------------
//CSuplSettings::GetUsageInHomwNwFlag()
//
//This method is used to retrieve parameter which indicates whether server
//can be used out side home network or not. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GetUsageInHomwNwFlag( const TInt64 aSlpId, TBool& aHomeNwFlag ) const
    {
    return iSettingsEngine->GetUsageInHomwNwFlag(aSlpId,aHomeNwFlag );             
    }            

//-------------------------------------------------------------------------------------
//CSuplSettings::SetEditableFlag()
//
//This method is used to change parameter which indicates whether server
// details are editable or not.
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::SetEditableFlag( const TInt64 aSlpId, const TBool aEditFlag )const
    {
    return iSettingsEngine->SetEditableFlag(aSlpId,aEditFlag);             
    }

//-------------------------------------------------------------------------------------
//CSuplSettings::GetEditableFlag()
//
//This method is used to retrieve parameter which indicates whether server
//details are editable or not.
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GetEditableFlag( const TInt64 aSlpId, TBool& aEditFlag ) const        
    {
    return iSettingsEngine->GetEditableFlag(aSlpId,aEditFlag );             
    }            
//-------------------------------------------------------------------------------------
//CSuplSettings::SetAllParameter()
//
//This method is used to change all parameters of server in SUPL settings.
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::SetAllParameter( 
        const CServerParams* aParamValues)
    {
    return iSettingsEngine->SetAllParameter(aParamValues);             
    }
//-------------------------------------------------------------------------------------
//CSuplSettings::SetDefaultServer()
//
//This method is used to change all parameters of default server in SUPL settings. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::SetDefaultServer( 
                       const CServerParams* aParamValues)
    {
    return iSettingsEngine->SetDefaultServer(aParamValues);
    }
//-------------------------------------------------------------------------------------
//CSuplSettings::RemoveServer()
//
//This method is deletes server entry from SUPL settings storage.
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::RemoveServer(TInt64 aSlpId)
    {
    return iSettingsEngine->RemoveServer(aSlpId);             
    }
//-------------------------------------------------------------------------------------
//CSuplSettings::ChangePriority()
//
//This method is changes the priority of the server to the specified priority.
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::ChangePriority(TInt64 aSlpId,TInt aPriority,TBool aDirection)
    {
    return iSettingsEngine->ChangePriority(aSlpId,aPriority,aDirection);             
    }

//-------------------------------------------------------------------------------------
//CSuplSettings::GetSlpInfoFromId()
//
//This method is used to retrieve all properties of server from SUPL settings storage
//based on SLP Id. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GetSlpInfoFromId(
        const TInt64 aSlpId, 
        CServerParams* aParamValues) const
    {
    if(aParamValues == NULL)
        return KErrArgument;
    return iSettingsEngine->GetSlpInfoFromId(aSlpId,aParamValues);
    }
//-------------------------------------------------------------------------------------
//CSuplSettings::GetSlpInfoAddress()
//
//This method is used to retrieve all properties of server from SUPL settings storage
//based on SLP address. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GetSlpInfoAddress(const TDesC& aServerAddress,CServerParams* aParamValues) const
    {
    if(aParamValues == NULL)
        return KErrArgument;
    return iSettingsEngine->GetSlpInfoAddress(aServerAddress,aParamValues);
    }


//--------------------------------------------------------------------------------------
//CSuplSettings::GetDefaultServer()
//
//This method is used to retrieve all properties of default server from SUPL settings storage.
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GetDefaultServer(CServerParams* aParamValues) const
    {
    return iSettingsEngine->GetDefaultServer(aParamValues);
    }

//--------------------------------------------------------------------------------------
//CSuplSettings::SlpCount()
//
//This method is used to retrieve number of server entries present in SUPL settings. 
//--------------------------------------------------------------------------------------

EXPORT_C TInt CSuplSettings:: SlpCount(TInt& aCount )
    {
    return iSettingsEngine->SlpCount(aCount);
    }

//--------------------------------------------------------------------------------------
//CSuplSettings::IsSlpExists()
//
//This method is used check whether server entry exists or not based on SLP id
//--------------------------------------------------------------------------------------
EXPORT_C TBool CSuplSettings::IsSlpExists(const TInt64 aSlpId)
    {
    return iSettingsEngine->IsSlpExists(aSlpId);
    }
//--------------------------------------------------------------------------------------
//CSuplSettings::IsSlpExists()
//
//This method is used check whether server entry exists or not based on SLP address
//--------------------------------------------------------------------------------------
EXPORT_C TBool CSuplSettings::IsSlpExists(const TDesC& aServerAddress)
    {
    return iSettingsEngine->IsSlpExists(aServerAddress);
    }
//--------------------------------------------------------------------------------------
//CSuplSettings::GetAllSlp()
//
//This synchronous method is used to retrieve properties of all servers 
//from SUPL settings storage.
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GetAllSlp(RPointerArray<CServerParams>& aParamValues) const
    {
    return iSettingsEngine->GetAllSlp(aParamValues);
    }
//---------------------------------------------------------------------
// CSuplSettings::GetAllSlp()
//
//  This method is used to retrieve properties of all servers from SUPL 
//  settings storage.
//---------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GetAllSlp(RPointerArray<CServerParams>& aParamValues,TRequestStatus& aStatus) const
    {
    return iSettingsEngine->GetAllSlp(aParamValues,aStatus);
    }

//---------------------------------------------------------------------
// CSuplSettings::SetFallBack()
//
// This method enables or disables the fall back in SUPL settings.
//---------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::SetFallBack(TBool aFallBack)
    {
    return iSettingsEngine->SetFallBack(aFallBack);             
    }
//---------------------------------------------------------------------
// CSuplSettings::GetFallBack()
//
// This method retrieves the fallback value from SUPL settings storage.
//---------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GetFallBack(TBool& aFallBack) const
    {
    return iSettingsEngine->GetFallBack(aFallBack);             
    }

//---------------------------------------------------------------------
// CSuplSettings::GetTriggerParams()
//
// This method is used to retrieve properties of all active triggers. 
//---------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GetTriggerParams( RPointerArray<CTriggerParams>& aParamValues ) const
	{
	return iSettingsEngine->GetTriggerParams(aParamValues);        
	}

//---------------------------------------------------------------------
// CSuplSettings::GetTriggerParams()
//
// This method is used to retrieve properties of a given session. 
//---------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::GetTriggerParams( TInt64 aSessionId,
                                                CTriggerParams*& aParamValues ) const
    {
    if( aParamValues )
        {
        return iSettingsEngine->GetTriggerParams(aSessionId,aParamValues);
        }
    return KErrArgument;
    
    }

//---------------------------------------------------------------------
// CSuplSettings::SetNotificationStatus()
//
// Sets Notification status of particular session
//---------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::SetNotificationStatus( TInt64 aSessionId,
    	           									TBool aTriggerNotificationStatus )
	{
	return iSettingsEngine->SetNotificationStatus(aSessionId,aTriggerNotificationStatus);    
	}
//---------------------------------------------------------------------
// CSuplSettings::CancelTriggerSession()
//
// Cancels ongoing triggering session
//---------------------------------------------------------------------
EXPORT_C TInt CSuplSettings::CancelTriggerSession( TInt64 aSessionId )
    {
    return iSettingsEngine->CancelTriggerSession(aSessionId);    
    }
//---------------------------------------------------------------------
// CSuplSettings::GetDefaultIAPName()
//
// Returns the default configured IAP name
//---------------------------------------------------------------------
EXPORT_C TInt  CSuplSettings::GetDefaultIAPName(TDes& aIapName)
    {
    return iSettingsEngine->GetDefaultIAPName(aIapName);    
    }
	
	    
//---------------------------------------------------------------------
// CSuplSettings::SetSuplTriggeredServiceStatus()
//
// Changes status of SUPL triggering service. 
//---------------------------------------------------------------------    
EXPORT_C TInt CSuplSettings::SetSuplTriggeredServiceStatus( const TSuplTriggerStatus& aSuplTriggerStatus )
	{
			return iSettingsEngine->SetSuplTriggeredServiceStatus(aSuplTriggerStatus);    
	}   
	 
//---------------------------------------------------------------------
// CSuplSettings::GetSuplTriggeredServiceStatus()
//
// Gets status of SUPL triggering service.SUPL Triggering service can be either ESuplTriggerOn or ESuplTriggerOff
//---------------------------------------------------------------------    
EXPORT_C TInt CSuplSettings::GetSuplTriggeredServiceStatus( TSuplTriggerStatus& aSuplTriggerStatus )
	{
			return iSettingsEngine->GetSuplTriggeredServiceStatus(aSuplTriggerStatus);    
	}   	 
	
//  End of File
