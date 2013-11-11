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
* Description:   Implementation of COMASuplIMSIObserver
*
*/




#include <startupdomainpskeys.h>

#include "epos_comasuplimsiobserver.h"
#include "epos_csuplsettingparams.h"
#include "epos_csuplsettingsinternal.h"
#include "epos_msuplimsiobserver.h"

const TInt KSettingsParamLen = 512;

_LIT( KImsiObserver, "epos_comasuplimsiobserver.cpp" );


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
COMASuplIMSIObserver* COMASuplIMSIObserver::NewL(MHandleImsiObserver& aObserver)
    {
    COMASuplIMSIObserver* self = new ( ELeave ) COMASuplIMSIObserver(aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// ---------------------------------------------------------------------------
// Returns current state of the observer
// ---------------------------------------------------------------------------
//
TInt COMASuplIMSIObserver::State()
    {
    return iState;
    }
    

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplIMSIObserver::~COMASuplIMSIObserver() 
    {
    Deque();
    if(iSettings)
        delete iSettings;
    iStartupObserver.Close();
    }


// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
COMASuplIMSIObserver::COMASuplIMSIObserver(MHandleImsiObserver& aObserver):CActive( EPriorityStandard ),iImsiObserver(aObserver)
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// 2nd phase constuctor for instantiating member variables
// ---------------------------------------------------------------------------
//
void COMASuplIMSIObserver::ConstructL()
    {
    iSettings = CSuplSettingsInternal::NewL();
    InitializeL();
    }


// ---------------------------------------------------------------------------
// Checks the current system state to initiate further processing
// ---------------------------------------------------------------------------
//
void COMASuplIMSIObserver::InitializeL()
    {
    Trace( _L("InitializeL() Start"), KImsiObserver, __LINE__ );
    
    RProperty simObserver;
    CleanupClosePushL( simObserver );
    
    User::LeaveIfError( simObserver.Attach( KPSUidStartup, 
                                                    KPSSimChanged, EOwnerThread ) );
    
    TInt SIMChanged = 0;
    User::LeaveIfError( simObserver.Get( SIMChanged ) );
    

    if ( SIMChanged == ESimChanged)
        {
	        Trace( _L("Detected SIM Change"), KImsiObserver, __LINE__ );
	        CheckForBootCompletionL();
        }
    else
        {
   	        iState = COMASuplSmsListenerBase::EComplete;
	        Trace( _L("No SIM Change Detected"), KImsiObserver, __LINE__ );
			if(iSettings)
			    {
			    Trace( _L("Deleted Settings Object"), KImsiObserver, __LINE__ );
			    delete iSettings;
			    iSettings = NULL;
			    }
			else
			    Trace( _L("Settings Object is NULL"), KImsiObserver, __LINE__ );
        	
        }
    
    CleanupStack::PopAndDestroy();
    }


// ---------------------------------------------------------------------------
// Initializes the instance of CSuplSettings
// ---------------------------------------------------------------------------
//
void COMASuplIMSIObserver::InitializeSettings()
    {
    iState = COMASuplSmsListenerBase::ESettingsInitialized;
    if(iSettings)
    iSettings->Initialize( iStatus );
    SetActive();
    }


// ---------------------------------------------------------------------------
// Handles the event of a SIM change
// ---------------------------------------------------------------------------
//
void COMASuplIMSIObserver::HandleIMSIChangeL()
    {
    Trace( _L( "HandleIMSIChangeL() Start" ), KImsiObserver, __LINE__ );
        
    TBool isIMSIChanged = EFalse;
    if(iSettings)
        {
        TInt err = iSettings->IsImsiChanged( isIMSIChanged ) ;
        if(err != KErrNone)
            {
            Trace( _L("Deleted Settings Object"), KImsiObserver, __LINE__ );
            delete iSettings;
            iSettings = NULL;
            User::Leave( err);
            }
        }
    
    if ( isIMSIChanged )
        {
        // delete all servers which have the Sim Change Remove Flag set
        
        Trace( _L( "RemoveOnSimChange()" ), KImsiObserver, __LINE__ );
        iSettings->RemoveOnSimChange(); //Ignore the error
        RBuf hslpaddr;
        User::LeaveIfError( hslpaddr.Create( KSettingsParamLen ) );
        CleanupClosePushL( hslpaddr );
        
        RBuf iapn;
        User::LeaveIfError( iapn.Create( KSettingsParamLen ) );
        CleanupClosePushL( iapn );
        
        
        Trace( _L( "GenerateHslpAddressFromImsi()" ), KImsiObserver, __LINE__ );
        if(iSettings)
            {
            TInt err = iSettings->GenerateHslpAddressFromImsi( hslpaddr );
            if(err != KErrNone)
                {
                Trace( _L("Deleted Settings Object"), KImsiObserver, __LINE__ );
                delete iSettings;
                iSettings = NULL;
                CleanupStack::PopAndDestroy(2); //hslpaddr,iapn
                User::Leave( err);
                }
            err = iSettings->GetDefaultIAPName(iapn);
            if(err != KErrNone)
                {
                Trace( _L("error retrieving default IAP"), KImsiObserver, __LINE__ );
                delete iSettings;
                iSettings = NULL;
                CleanupStack::PopAndDestroy(2); //hslpaddr,iapn
                User::Leave( err);
                }
            Trace( hslpaddr, KImsiObserver, __LINE__ );
            Trace( iapn, KImsiObserver, __LINE__ );
            }
        
        
        //create param structure
        CServerParams* param=CServerParams::NewL();
        CleanupStack::PushL(param);
        User::LeaveIfError(param->Set( hslpaddr,iapn,ETrue,ETrue,ETrue,EFalse ));
        param->SetServerConfigurationType(EFalse);
        
        // add new server into settings
        TInt64 slpId;
        Trace( _L( "AddNewServer()" ), KImsiObserver, __LINE__ );
        TInt err=iSettings->AddNewServer( param,slpId ); //Ignore error
        Trace( _L( "AddNewServer() error code" ), KImsiObserver, err );
        
        
        CleanupStack::PopAndDestroy(3); //iapn,hslpaddr,param
        }
    if(iSettings)
        {
        Trace( _L("Deleted Settings Object"), KImsiObserver, __LINE__ );
        delete iSettings;
        iSettings = NULL;
        }
    Trace( _L( "HandleIMSIChangeL() End" ), KImsiObserver, __LINE__ );
    iImsiObserver.HandleImsiObserverCompleteL();
    }


// ---------------------------------------------------------------------------
// Request completion event handler
// ---------------------------------------------------------------------------
//
void COMASuplIMSIObserver::RunL()
    {
    if ( KErrNone != iStatus.Int() )
        {
        Trace( _L( "COMASuplIMSIObserver::RunL() - Unknown Error " ), 
                                        KImsiObserver, __LINE__ );
            
        iState = COMASuplSmsListenerBase::EComplete;
        }
        
    switch( iState )
        {
        case COMASuplSmsListenerBase::ESubscriptionActive:
            InitializeSettings();
            break;
        
        case COMASuplSmsListenerBase::ESettingsInitialized:
            iState = COMASuplSmsListenerBase::EComplete;
            TRAPD( err, HandleIMSIChangeL() );
            if ( KErrNone != err )
                {
                Trace( _L( "COMASuplIMSIObserver::RunL - Error Processing IMSI Change" ), 
                            KImsiObserver, __LINE__ );
                }
            break;
            
        default:
            break;
        }
    }


// ---------------------------------------------------------------------------
// Cancels any outstanding request
// ---------------------------------------------------------------------------
//
void COMASuplIMSIObserver::DoCancel()
    {
    if ( iState == COMASuplSmsListenerBase::ESettingsInitialized )
        {
        if(iSettings)
        iSettings->CancelInitialize();
        }

    if ( iState == COMASuplSmsListenerBase::ESubscriptionActive )
        {
        iStartupObserver.Cancel();
        }
    }
    
// ---------------------------------------------------------------------------
// CheckForBootCompletion
// Check boot is completed... if its then do initilization of settings
// ---------------------------------------------------------------------------
//
void COMASuplIMSIObserver::CheckForBootCompletionL()
	{
        User::LeaveIfError( iStartupObserver.Attach( KPSUidStartup, 
                                        KPSGlobalSystemState, EOwnerThread ) );

        TInt bootCompleteStatus = 0;
        User::LeaveIfError( iStartupObserver.Get( bootCompleteStatus ) );
        TBuf<64> buff;
        buff.Copy(_L("CheckForBootCompletionL :  "));
        buff.AppendNum(bootCompleteStatus);
        Trace(buff, KImsiObserver, __LINE__ );
          
        if(! ((bootCompleteStatus == ESwStateNormalRfOn ) ||
            (bootCompleteStatus == ESwStateNormalRfOff ) ||
            (bootCompleteStatus == ESwStateNormalBTSap ) )
          )
            {
            iStartupObserver.Subscribe( iStatus );
            iState = COMASuplSmsListenerBase::ESubscriptionActive;
            SetActive();
            }
        else
            {
            InitializeSettings();
            }
	}


// ---------------------------------------------------------------------------
// Wrapper function used for tracing
// ---------------------------------------------------------------------------
//   
void COMASuplIMSIObserver::Trace( const TDesC& aMessage, const TDesC& aFileName, TInt line )
    {
    COMASuplTrace* trace = NULL;
    TRAPD( err, trace = COMASuplTrace::NewL() );
    if ( KErrNone == err )
        {
        trace->Trace( aMessage, aFileName, line );
        delete trace;
        }
    }
