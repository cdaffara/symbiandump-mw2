/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Resolves sending service information and stores it to 
*                Central Repository.    
*
*/



#include <e32property.h>
#include <e32cmn.h>
#include <apgwgnam.h>
#include <mtclreg.h>                         // CClientMtmRegistry
#include <SendUiConsts.h>
#include <CSendingService.h>
#include <CSendingServiceInfo.h>
#include <SendingServiceInterfaceUids.hrh>

#include <CSendingServiceInfo.h>
#include <TSendingCapabilities.h>
#include "centralrepository.h"
#include "senduisrappui.h"
#include "SendUiPrivateCRKeys.h"
#include "senduiproperty.h"
#include "senduiserviceslog.h"              // Logging
#include <senduisingleton.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSendUiSrAppUi::CSendUiSrAppUi() :
    iCoeEnv( CCoeEnv::Static() )                             
    {
    SetFullScreenApp(EFalse); // disable possible transition effects
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CSendUiSrAppUi::ConstructL()
    {
    BaseConstructL(ENoAppResourceFile);

    HideApplicationFromFSW( ETrue ); // hide from Fast Swap Window
    
    TApaTask task(iEikonEnv->WsSession());
    task.SetWgId(iEikonEnv->RootWin().Identifier());
    task.SendToBackground(); 

    iIdle = CIdle::NewL( CActive::EPriorityIdle );
    iIdle->Start( TCallBack(UpdateTask, this ));
    }

TInt CSendUiSrAppUi::UpdateTask( TAny* aThis  )
    {
    CSendUiSrAppUi* appUi = static_cast<CSendUiSrAppUi*>( aThis );
    
    TRAPD( err, appUi->HandleUpdateSendListL() );

    if ( err ) 
        {
#ifdef _DEBUG
        TBuf<256> buf;
        buf.Format(_L("SendUi err"));
        User::InfoPrint(buf);
        LOGTEXT2(_L("SendUiServiceResolver: Error %d while loading CenRep."), err );
#endif
        RProperty::Set( 
        KPSUidSendUiProperty, 
        KSendUiUpdateOperation, 
        KSendUiUpdateOperationDone ); 

        RProperty::Set( 
        KPSUidSendUiProperty, 
        KSendUiUpdateRequest, 
        KSendUiNotInitialized  ); 
        }
        
    appUi->Exit();
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSendUiSrAppUi::~CSendUiSrAppUi()
    {
    delete iIdle;
    }

// ---------------------------------------------------------------------------
// HandleUpdateSendListL
// ---------------------------------------------------------------------------
//
void CSendUiSrAppUi::HandleUpdateSendListL()
    {
    RProperty::Set( 
        KPSUidSendUiProperty, 
        KSendUiUpdateOperation, 
        KSendUiUpdateOperationRunning ); 
    
    CRepository* myRep = CRepository::NewLC( KCRUidSendUi );
    LOGTEXT(_L("SendUiSr: Repository created."));
   
    // KErrNotFound indicates undefined repository state and services are re-read by Watcher
    // in boot, if this update fails. The correct language value is set when update completes 
    // succesfully.
    TInt result = myRep->Set( KKeySendUiServiceLanguage, KErrNotFound );
    
    LOGTEXT(_L("SendUiSr: Service Manager created."));
    RPointerArray<CSendingServiceInfo> servicesArray;
    RPointerArray<CSendingService> serviceProviders;
   
    PopulateServicesListL( servicesArray, serviceProviders, iCoeEnv );
    
    TSendingCapabilities serviceCapabilities;

    // cleaning old and storing new sending service values to Central Repository
    // is done in one transaction.
    result += myRep->StartTransaction( CRepository::EConcurrentReadWriteTransaction );

    if ( result != KErrNone )
        {
        servicesArray.Close();
        CleanupStack::PopAndDestroy( myRep );
        return;
        }
    else
        {
        ClearPluginListL( myRep );
        }

    TInt i(0);
    TInt err(0);
    for ( i = 0; i < servicesArray.Count(); i++)
        {
        // Store to Central Repository: uid, providerId, name, menuname, capabilities, 
        // technologytype, messagesize, flags and serviceFeatures
        err += AddSendingService( 
            myRep,
            i, 
            servicesArray[i]->ServiceId(), 
            servicesArray[i]->ServiceProviderId(), 
            servicesArray[i]->TechnologyTypeId(),
            servicesArray[i]->ServiceFeatures(),
            servicesArray[i]->ServiceCapabilities(), 
            servicesArray[i]->ServiceName().Left( KSendUiServiceMaxNameLength ),
            servicesArray[i]->ServiceMenuName().Left( KSendUiServiceMaxNameLength ),
            servicesArray[i]->ServiceAddress().Left( KSendUiServiceMaxNameLength ) );
        }

    err += myRep->Set( KKeySendUiServiceCount, servicesArray.Count() );
    err += myRep->Set( KKeySendUiServiceLanguage, (TInt)User::Language() );
    
    if ( err == KErrNone )
    	{
        TUint32 keyInfo;
        myRep->CommitTransaction( keyInfo );
    	}
    
    servicesArray.Close();
    serviceProviders.ResetAndDestroy();
    CleanupStack::PopAndDestroy( myRep ); // myRep
    
    RProperty::Set( 
        KPSUidSendUiProperty, 
        KSendUiUpdateOperation, 
        KSendUiUpdateOperationDone ); 

    RProperty::Set( 
        KPSUidSendUiProperty, 
        KSendUiUpdateRequest, 
        KSendUiUpdated ); 
}

// ---------------------------------------------------------------------------
// AddSendingService
// ---------------------------------------------------------------------------
//
TInt CSendUiSrAppUi::AddSendingService( 
        CRepository* aRep,
        TUint aIndex, 
        TUid aMtmUid,
        TUid aServiceProviderId,
        TUid aTechnologyType,
        TInt aFeatures,
        TSendingCapabilities aCapabilities,
        TPtrC aServiceName,
        TPtrC aServiceMenuName,
        TPtrC aServiceAddress )
    {
    // Cenrep stays in readble state during transaction
    
    aIndex = aIndex * KSendUiServiceIndex;
    // "collect" errors. Focus is in the existance of errors, not on any specific error.
    TInt err = aRep->Create( aIndex | KSendUiServiceId, (TInt)aMtmUid.iUid );
    err += aRep->Create( aIndex | KSendUiServiceProviderId, (TInt)aServiceProviderId.iUid );
    
    err += aRep->Create( aIndex | KSendUiServiceName, aServiceName );
    err += aRep->Create( aIndex | KSendUiServiceMenuName, aServiceMenuName );
    err += aRep->Create( aIndex | KSendUiServiceAddress, aServiceAddress );
    
    err += aRep->Create( aIndex | KSendUiServiceFlags, aCapabilities.iFlags );
    err += aRep->Create( aIndex | KSendUiServiceBodySize, aCapabilities.iBodySize );
    err += aRep->Create( aIndex | KSendUiServiceMessageSize, aCapabilities.iMessageSize );
    err += aRep->Create( aIndex | KSendUiServiceTechnologyType, (TInt)aTechnologyType.iUid );
    err += aRep->Create( aIndex | KSendUiServiceFeatures, aFeatures );
    LOGTEXT3(_L("SendUiServiceResolver: index %d, service %d"), aIndex, aMtmUid.iUid );
    if ( err != KErrNone )
    	{
    	LOGTEXT(_L("Error while creating Central Repository data.") );
    	}
    return ( err != KErrNone ); 
    }

// ---------------------------------------------------------------------------
// ClearPluginListL
// Deletes all service detail keys from the repository.
// ---------------------------------------------------------------------------
//
void CSendUiSrAppUi::ClearPluginListL( CRepository* aRep )
    {
    TUint32 partialKey = 0x00000000;
    TUint32 keyMask =    0xFFFF0000; // 0, don't care mask
    RArray<TUint32> indexKeys;
    CleanupClosePushL( indexKeys );
  
    aRep->FindL( partialKey, keyMask, indexKeys );

    for (int i(0) ; i < indexKeys.Count(); i++)
        {
        aRep->Delete( indexKeys[ i ] );
        }
    
    CleanupStack::PopAndDestroy( &indexKeys );
    }

// ---------------------------------------------------------------------------
// From class CAknAppUi.
// ---------------------------------------------------------------------------
//
void CSendUiSrAppUi::HandleCommandL(TInt /*aCommand*/)
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
CSendingService* CSendUiSrAppUi::LoadServiceL( 
    TUid aServiceProviderUid, 
    CCoeEnv* aCoeEnv, 
    CSendUiSingleton* aSingleton,
    RPointerArray<CSendingServiceInfo>& aServiceList )
    {
    CSendingService* sendingService = CSendingService::NewL( 
        aServiceProviderUid, 
        *aCoeEnv, 
        *aSingleton );        
        
    if( sendingService )
        {
        CleanupStack::PushL( sendingService );
        sendingService->PopulateServicesListL( aServiceList );
        // if everything goes well, aServiceProviders will own this object soon enough
        CleanupStack::Pop( sendingService ); 
        }
    return sendingService;
    }
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CSendUiSrAppUi::PopulateServicesListL(
    RPointerArray<CSendingServiceInfo>& aServiceList,
    RPointerArray<CSendingService>&      aServiceProviders,
    CCoeEnv* aCoeEnv )
    {
    CSendUiSingleton* singleton = CSendUiSingleton::NewL();
    CleanupStack::PushL( singleton );
    aServiceList.Reset();

    RImplInfoPtrArray implInfoArray;
    RPointerArray<CSendingServiceInfo> services;

    REComSession::ListImplementationsL(
        TUid::Uid( KSendUiSendingServiceInterface ),implInfoArray);

    TInt serviceIndex(0);
    CSendingService* sendingService;
    while ( serviceIndex < implInfoArray.Count() )
        {
        sendingService = NULL;
        TUid serviceUid = (*implInfoArray[serviceIndex]).ImplementationUid(); 
        // If Ecom service loading fails, then just move on
        TRAPD( error, sendingService = LoadServiceL( serviceUid, aCoeEnv, singleton, services ) );        
        
        if( sendingService && error == KErrNone )
            {
            aServiceProviders.AppendL( sendingService );
            
            for ( TInt index = 0; index < services.Count(); index++ )
                {
                aServiceList.Append( services[ index ] );
                }
            services.Reset();
            
            LOGTEXT2( _L("Provider %d creation SUCCESS."), serviceUid );
            }
        else
            {
            LOGTEXT3( _L("Provider %d creation FAILED. Error code: %d"), serviceUid, error );
            }

        serviceIndex++;
        }
    services.Close();
    implInfoArray.ResetAndDestroy();
    REComSession::FinalClose();
    CleanupStack::PopAndDestroy( singleton );
    }

// end of file

