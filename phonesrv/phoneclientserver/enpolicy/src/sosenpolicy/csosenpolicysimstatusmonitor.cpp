/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SIM Status Monitor.
*
*/



// INCLUDES
#include    "csosenpolicysimstatusmonitor.h" 
#include    <startupdomainpskeys.h> // Property values
#include    <PSVariables.h> 



// CONSTANTS
const TUint KENPolicySAConnectionRetryTime = 2000000;  // 2s
const TInt KENPolicySAConnectionRetries = 3;

_LIT( KENPolicyPanicCategory, "ENPolicyFault" );


// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// CSosEnPolicySimStatusMonitor::CSosEnPolicySimStatusMonitor
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSosEnPolicySimStatusMonitor::CSosEnPolicySimStatusMonitor( 
    CSosEmergencyNumberPolicyHandler& aObserver ) :
    CActive( EPriorityStandard ),
    iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CSosEnPolicySimStatusMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSosEnPolicySimStatusMonitor* CSosEnPolicySimStatusMonitor::NewL( 
    CSosEmergencyNumberPolicyHandler& aObserver )
    {
    CSosEnPolicySimStatusMonitor* self = new ( ELeave ) 
        CSosEnPolicySimStatusMonitor( aObserver );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CSosEnPolicySimStatusMonitor::~CSosEnPolicySimStatusMonitor
// Destructor.
// -----------------------------------------------------------------------------
//
CSosEnPolicySimStatusMonitor::~CSosEnPolicySimStatusMonitor()
    {
    Cancel();

    iProperty.Close();

    }

// -----------------------------------------------------------------------------
// CSosEnPolicySimStatusMonitor::ConstructL
// 
// Construction. Creates also connection to system agent.
// -----------------------------------------------------------------------------
//        
void CSosEnPolicySimStatusMonitor::ConstructL()
    {
    TInt err = KErrNotSupported;
    for( TInt i = 0; i < KENPolicySAConnectionRetries; i++ )
        {
        err = iProperty.Attach( KPSUidStartup, KPSSimStatus );
        if( err == KErrNone )
            {
            i = KENPolicySAConnectionRetries;
            }
        else
            {
            // _DPRINT( 4, "ENPolicy.SimStatus.Attach.RETRY" );  
            User::After( KENPolicySAConnectionRetryTime );
            }
        }
    //if connection not established.
    __ASSERT_ALWAYS( err == KErrNone, User::Panic( 
        KENPolicyPanicCategory, EEnPolicyPanicCentralRepositoryConnectionFailure ) );
    IssueRequest();
    }

// -----------------------------------------------------------------------------
// CSosEnPolicySimStatusMonitor::DoCancel
// 
//  Cancellation of notify.
// -----------------------------------------------------------------------------
//
void CSosEnPolicySimStatusMonitor::DoCancel()
    {
    iProperty.Cancel();
    }

// -----------------------------------------------------------------------------
// CSosEnPolicySimStatusMonitor::RunL
// 
// SIM event occurred.
// Basically events make three kinds of responses.
// If there is an error in notify, nothing is made. Othervice, event
// may indicate that SIM is accessible or not. Depending on that, action is
// send for observer.
// -----------------------------------------------------------------------------
//
void CSosEnPolicySimStatusMonitor::RunL()
    {
    TInt state = KErrGeneral;

    iProperty.Get( state );

    // _DDPRINT( 4, "ENPolicy.SimStatus.status.e.", iStatus.Int() );    
    // _DDPRINT( 4, "ENPolicy.SimStatus.status.", state );
    CSosEmergencyNumberPolicyHandler::TSosEnPolicySimEmergencyNumberAction
        action = CSosEmergencyNumberPolicyHandler::
                    ESosEnPolicySimEmergencyNumberClear;
    switch ( iStatus.Int() )
        {
        case KErrNotSupported:
        case KErrCancel:
            return;
        case KErrNone:
            {
            if ( TranslateState( state ) )
                {
                action = 
                    CSosEmergencyNumberPolicyHandler::
                        ESosEnPolicySimEmergencyNumberRead;
                }
            iObserver.CpsssmHandleSimStatusEvent( action );
            }
        default:
            break;
        }
    IssueRequest();
    }

// -----------------------------------------------------------------------------
// CSosEnPolicySimStatusMonitor::IsReadingAllowed
// 
// Checks sim state from system agent
// -----------------------------------------------------------------------------
// 
TBool CSosEnPolicySimStatusMonitor::IsReadingAllowed()
    {
    TInt state = KErrGeneral;

    iProperty.Get( state );
    return TranslateState( state );
    }

// -----------------------------------------------------------------------------
// CSosEnPolicySimStatusMonitor::IssueRequest
// 
// Issue notify request to System agent.
// -----------------------------------------------------------------------------
//        
void CSosEnPolicySimStatusMonitor::IssueRequest()
    {
    iProperty.Subscribe( iStatus );

    SetActive();
    }

// -----------------------------------------------------------------------------
// CSosEnPolicySimStatusMonitor::TranslateState
// 
// Translate the state information.
// -----------------------------------------------------------------------------
// 
TBool CSosEnPolicySimStatusMonitor::TranslateState( TInt aState )
    {
    switch ( aState )
        {
        case ESimUsable:         
        case ESimReadable:
            return ETrue;
        case  ESimStatusUninitialized:
        case ESimNotReady:
        case ESimNotSupported:
        case ESimNotPresent:
            return EFalse;
        default:
            return EFalse;
        }
    }


//  End of File
