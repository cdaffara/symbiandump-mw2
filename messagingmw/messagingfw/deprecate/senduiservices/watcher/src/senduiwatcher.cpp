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
* Description:   Launches SendUiServiceResolver in the events of:
*                At all times:
*                - mtm installation/removal
*                - Message Server events:
*                    * EMsvEntryCreated
*                    * EMsvEntryDeleted
*                    * EMsvCorruptedIndexRebuilt
*                    * EMsvMediaChanged
*                - Ecom service event arrives and SendUI Ecom service count has 
*                  changed since last boot.
*
*/




#include <implementationproxy.h>
#include <msvapi.h>
#include <data_caging_path_literals.hrh> 
#include <msvids.h>                     // for KMsvRootIndexEntryId
#include <coemain.h>
#include <apgcli.h>
#include <apacmdln.h>
#include <eikdll.h>
#include <apaflrec.h>
#include <bldvariant.hrh>
#include <e32property.h>
#include <e32base.h>
#include <centralrepository.h>
#include <mtudreg.h>
#include <featmgr.h>

#include "senduiwatcher.h"
#include "senduiecomobserver.h"
#include "senduipropertyobserver.h"
#include "senduiproperty.h"
#include "SendUiPrivateCRKeys.h"
#include "senduiserviceslog.h"

_LIT( KSendUiLauncherApp, "SendUiServiceResolver.exe");

const TImplementationProxy ImplementationTable[] = 
    {
    IMPLEMENTATION_PROXY_ENTRY( 0x102823B6, CSendUiWatcher::NewL )
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );
    return ImplementationTable;
    }


// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CSendUiWatcher* CSendUiWatcher::NewL( TAny* /*aWatcherParams */)
    {
    CSendUiWatcher* self=new (ELeave) CSendUiWatcher();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSendUiWatcher::~CSendUiWatcher()
    {
    delete iPropertyObserver;
    delete iTimer;
    }

// ---------------------------------------------------------------------------
// DoTask
// ---------------------------------------------------------------------------
//
TInt CSendUiWatcher::DoTaskL( TAny* aThis )
	{
    LOGTEXT(_L("CSendUiWatcher::DoTask >>"));
    CSendUiWatcher* watcher = static_cast<CSendUiWatcher*>( aThis );
    // We've waited long enough. Try to set to done. Ignore error.
    RProperty::Set(
        KPSUidSendUiProperty, 
        KSendUiUpdateOperation, 
        KSendUiUpdateOperationDone );
    watcher->HandleUpdateServicesL();
    LOGTEXT(_L("CSendUiWatcher::DoTask <<"));
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// HandleUpdateServicesL
// ---------------------------------------------------------------------------
//
void CSendUiWatcher::HandleUpdateServicesL()
    {
    LOGTEXT(_L("CSendUiWatcher::HandleUpdateServicesL >>"));
    TInt value;
    delete iTimer;
	iTimer = NULL;

    RProperty::Get( KPSUidSendUiProperty, KSendUiUpdateOperation, value );
    if ( value != KSendUiUpdateOperationDone )
        {
        // try again later
        SetTimerL();
        }
    else
        {
        // Run SendUiSr
        RApaLsSession ls;
        
        CApaCommandLine *cmd = CApaCommandLine::NewL();
        CleanupStack::PushL( cmd );

        cmd->SetExecutableNameL( KSendUiLauncherApp ); 
        cmd->SetCommandL( EApaCommandBackground );
        User::LeaveIfError( ls.StartApp( *cmd ) );
        CleanupStack::PopAndDestroy( cmd );
        }
    LOGTEXT(_L("CSendUiWatcher::HandleUpdateServicesL <<"));
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSendUiWatcher::CSendUiWatcher() // first-phase C++ constructor
    {
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CSendUiWatcher::ConstructL() // second-phase constructor
    {
    if ( !iSession )
        {
        iSession = CMsvSession::OpenSyncL( *this );
        }
    TInt result( KErrNone );        
    
    result = RProperty::Define(
        KPSUidSendUiProperty, 
        KSendUiUpdateRequest,
        RProperty::EInt,
        TSecurityPolicy( ECapabilityReadUserData ),
        TSecurityPolicy( ECapabilityWriteUserData ));
    
    if ( result && result != KErrAlreadyExists )
        {
        User::LeaveIfError( result ); 
        }
   
    result = RProperty::Define(
        KPSUidSendUiProperty, 
        KSendUiUpdateOperation,
        RProperty::EInt,
        TSecurityPolicy( ECapabilityReadUserData ),
        TSecurityPolicy( ECapabilityWriteUserData  ));
    
    if ( result && result != KErrAlreadyExists )
        {
        User::LeaveIfError( result ); 
        }
    
    User::LeaveIfError( 
        RProperty::Set(KPSUidSendUiProperty, KSendUiUpdateOperation, KSendUiUpdateOperationDone) );
                
    iPropertyObserver = CSendUiPropertyObserver::NewL( this );

    iEcomObserver = CSendUiEcomObserver::NewL();  
    
    CheckIfUpdateNeededL();
    }

// ---------------------------------------------------------------------------
// UpdateFeature
// ---------------------------------------------------------------------------
//
void CSendUiWatcher::FeatureStatus( TInt aFeature, TInt &aNewFeatures, TInt aFeatureFlag )
    {
    if ( FeatureManager::FeatureSupported( aFeature ) )
        {
        aNewFeatures |= aFeatureFlag;
        }
    }

// ---------------------------------------------------------------------------
// CheckIfUpdateNeededL
// ---------------------------------------------------------------------------
//
void CSendUiWatcher::CheckIfUpdateNeededL()
    {
    LOGTEXT(_L("CSendUiWatcher::CheckIfUpdateNeededL >>"));
    CRepository* repository = CRepository::NewLC( KCRUidSendUi );
    
    TInt storedValue(KErrNotFound);
    TBool updateNeeded( EFalse );
    
    // Request service list update if phone language has changed.
    // KErrNotFound language value also indicates unsuccesfull run of SendUiServiceResolver
    TInt result = repository->Get( KKeySendUiServiceLanguage, storedValue );
    if ( result != KErrNone || storedValue != User::Language() )
    	{
    	updateNeeded = ETrue;
    	}
    
    // Issue service list update if mtm count has changed
    CMtmUiDataRegistry* mtmUiDataRegistry = CMtmUiDataRegistry::NewL( *iSession );
    TInt newMtmCount( KErrNotFound );
    if ( mtmUiDataRegistry )
    	{
    	newMtmCount = mtmUiDataRegistry->NumRegisteredMtmDlls();
	    delete mtmUiDataRegistry;
    	}

    result = repository->Get( KKeySendUiServiceMtmCount, storedValue ) ;
    if ( result != KErrNone || storedValue != newMtmCount )
    	{
    	result = repository->Set( KKeySendUiServiceMtmCount, newMtmCount );
    	updateNeeded = ETrue;
    	}
   
    TInt oldFeatures(0);   
    TInt newFeatures(0);   
    if ( repository->Get( KKeySendUiFeatureManagerServices, oldFeatures ) != KErrNone )
    	{
    	updateNeeded = ETrue;
    	}
    FeatureManager::InitializeLibL();
    FeatureStatus( KFeatureIdMmsPostcard, newFeatures, KSendUiPostcard );
    FeatureStatus( KFeatureIdAudioMessaging, newFeatures, KSendUiAudioMessage );
    FeatureStatus( KFeatureIdSenduiMmsUpload, newFeatures, KSendUiMmsUpload );
    FeatureManager::UnInitializeLib();
    if ( newFeatures != oldFeatures )
        {
        repository->Set( KKeySendUiFeatureManagerServices, newFeatures );    
        updateNeeded = ETrue;
        }

    CleanupStack::PopAndDestroy( repository );

    if ( updateNeeded )
        {
        HandleUpdateServicesL();
        }
    LOGTEXT(_L("CSendUiWatcher::CheckIfUpdateNeededL <<"));
    }

// ---------------------------------------------------------------------------
// SetTimerL
// ---------------------------------------------------------------------------
//
void CSendUiWatcher::SetTimerL()
    {
    LOGTEXT(_L("CSendUiWatcher::SetTimerL >>"));
    if ( !iTimer ) 
        {
        iTimer = CPeriodic::NewL( CActive::EPriorityStandard );    
        iTimer->Start( KSendUiServiceListUpdateDelay,
            KSendUiServiceListUpdateDelay,
            TCallBack( DoTaskL, this));
        }
    LOGTEXT(_L("CSendUiWatcher::SetTimerL <<"));
    }
    
void CSendUiWatcher::HandleSessionEventL(
            TMsvSessionEvent aEvent,
            TAny* /*aArg1*/,
            TAny* aArg2,
            TAny* /*aArg3*/ )
    {   
    // TODO: Reconnect to message server
    switch ( aEvent )
        {
        case EMsvServerReady:
        	{
        	LOGTEXT(_L("Message Server is ready."));
        	break;
        	}
        case EMsvMtmGroupDeInstalled: 
        case EMsvMtmGroupInstalled:
            {
            SetTimerL();
            break;
            }
    
        case EMsvEntriesCreated:// fall through
        case EMsvEntriesDeleted:
            {
            TMsvId folderId = KMsvNullIndexEntryId;
            folderId = (*(TMsvId*) (aArg2));
            if (folderId != KMsvRootIndexEntryId)
                {
                break;
                }
            SetTimerL();
            break;
                
            }
        case EMsvCorruptedIndexRebuilt:
            LOGTEXT2(_L("Watcher event %d"), aEvent);
            HandleUpdateServicesL();
            break;
        case EMsvCloseSession:
        case EMsvServerTerminated:
            delete iSession;
            iSession = NULL;
            LOGTEXT(_L("SendUiWatcher session was terminated"));
        break;
        case EMsvMediaChanged:// Added to handle change of Msg center
            SetTimerL();
            break;        
        default:;
        }
    }
 
// end of file

