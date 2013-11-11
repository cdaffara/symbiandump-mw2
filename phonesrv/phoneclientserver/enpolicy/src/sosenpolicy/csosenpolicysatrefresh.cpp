/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CSosEnPolicySatRefresh
*                class member functions.
*
*/



// INCLUDE FILES
#include  <e32std.h>
#include "csosemergencynumberpolicyhandler.h" 
#include "csosenpolicysatrefresh.h" 

#include <rsatrefresh.h> 
#include <rsatsession.h> 

#include  <startupdomainpskeys.h> // Property values

// Constants
_LIT( KENPolicyPanicCategory, "ENPolicyFault" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSosEnPolicySatRefresh::CSosEnPolicySatRefresh
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSosEnPolicySatRefresh::CSosEnPolicySatRefresh( CSosEmergencyNumberPolicyHandler& aCSosEmergencyNumberPolicyHandler ): 
    CActive( EPriorityStandard ), iCSosEmergencyNumberPolicyHandler ( aCSosEmergencyNumberPolicyHandler )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CSosEnPolicySatRefresh::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSosEnPolicySatRefresh::ConstructL()
    {
    TInt err = KErrNotSupported;
    iSatSession = new ( ELeave ) RSatSession;
    iSatRefresh = new ( ELeave ) RSatRefresh( *this );
 
    err = iProperty.Attach( KPSUidStartup, KPSGlobalSystemState );
    __ASSERT_ALWAYS( err == KErrNone, User::Panic( 
        KENPolicyPanicCategory, EEnPolicyPanicCentralRepositoryConnectionFailure ) );
    IssueRequest();
    }

// -----------------------------------------------------------------------------
// CSosEnPolicySatRefresh::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSosEnPolicySatRefresh* CSosEnPolicySatRefresh::NewL( CSosEmergencyNumberPolicyHandler& aCSosEmergencyNumberPolicyHandler )
    {    
    CSosEnPolicySatRefresh* self = new (ELeave) CSosEnPolicySatRefresh( aCSosEmergencyNumberPolicyHandler );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CSosEnPolicySatRefresh::~CSosEnPolicySatRefresh()
    {
    if ( iSatRefresh )
        {
        // Cancel notifications.
        iSatRefresh->Cancel();  
        // Close SubSession.
        iSatRefresh->Close();
        }
    delete iSatRefresh;
    
    if ( iSatSession )
        {
        iSatSession->Close();
        }
      delete iSatSession;

    Cancel();
    iProperty.Close();
    }

// -----------------------------------------------------------------------------
// CSosEnPolicySatRefresh::AllowRefresh
// Refresh query. Determines whether it allow the refresh to happen. 
// -----------------------------------------------------------------------------
//
TBool CSosEnPolicySatRefresh::AllowRefresh( TSatRefreshType /*aType*/, const TSatRefreshFiles& /*aFiles*/ )
    {
    // Refresh is always allowed
    return ETrue;
    }
 
// -----------------------------------------------------------------------------
// CSosEnPolicySatRefresh::Refresh
// Notification of refresh. In this method the client should not close 
// or cancel the RSatRefresh subsession. 
// -----------------------------------------------------------------------------
//     
void CSosEnPolicySatRefresh::Refresh( TSatRefreshType /*aType*/, const TSatRefreshFiles& /*aFiles*/ )
    {
    // Reload SIM Topics in case of refresh             
    iCSosEmergencyNumberPolicyHandler.CpsssmHandleSimStatusEvent( CSosEmergencyNumberPolicyHandler::ESosEnPolicySimEmergencyNumberRead );
    iSatRefresh->RefreshEFRead( EFalse );
    }
    
    
// -----------------------------------------------------------------------------
// CSosEnPolicySimStatusMonitor::IssueRequest
// 
// Issue notify request to System agent.
// -----------------------------------------------------------------------------
//        
void CSosEnPolicySatRefresh::IssueRequest()
    {
    iProperty.Subscribe( iStatus );
    SetActive();
    }
    
// -----------------------------------------------------------------------------
// CSosEnPolicySatRefresh::RunL
// 
// Property event occurred.
// -----------------------------------------------------------------------------
//
void CSosEnPolicySatRefresh::RunL()
    {
    TInt value;
    iProperty.Get( KPSUidStartup, KPSGlobalSystemState, value );
    if ( ESwStateNormalRfOn == value || ESwStateNormalRfOff == value  ) 
        {
        switch ( iStatus.Int() )
            {
            case KErrNotSupported:
            case KErrCancel:
                return;
            case KErrNone:
                {
                iSatSession->ConnectL();
                iSatRefresh->OpenL( *iSatSession );     
                TSatRefreshFiles file;
                file.Append( KEccEf );
                iSatRefresh->NotifyFileChangeL( file );
                break;  
                }
            default:
                break;
            }
        }
    else
        {
        IssueRequest(); 
        }
    }

// -----------------------------------------------------------------------------
//CSosEnPolicySatRefresh::DoCancel
// 
//  Cancellation of notify.
// -----------------------------------------------------------------------------
//
void CSosEnPolicySatRefresh::DoCancel()
    {
    iProperty.Cancel();
    }
    

//  End of File  
