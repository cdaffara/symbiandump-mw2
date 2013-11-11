/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  class of call objects
*
*/


//  INCLUDE FILES
#include "cccecallcontainer.h"
#include "mccpcall.h"
#include "mccpcscall.h"
#include "mcceobserver.h"
#include "mccessobserver.h"
#include "cccecall.h" 
#include "cccelogger.h"
#include "ccceutility.h"
#include "ccceconferencecall.h"
#include "ccceemergencycall.h"
#include "cccedtmf.h"
#include "cccetransfercontroller.h"
#include "cconvergedcallprovider.h"
#include "ccceextension.h"
#include "cccepluginmanager.h"
#include "cccedurationtimer.h"
#include "cccecallinfomediator.h"

#include <featmgr.h>

//CONSTANTS
const TInt KCSServiceId = 1;

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CCCECallContainer::CCCECallContainer( MCCEObserver& aObserver ): iMCCEObserver(&aObserver)
    {
    // Initialize callinfo call ids
    for ( TInt i = 0; i < KCCECallObjects; i++ )
        {
        iCallInfoCallId[ i ] = KErrNotFound;
        }
    
    }

// ---------------------------------------------------------------------------
// Construct object
// ---------------------------------------------------------------------------
//
void CCCECallContainer::ConstructL()
    {
    FeatureManager::InitializeLibL();
    iCallStateMediator = CCCECallInfoMediator::NewL( *this );
    
    CreateCallDataArraysL();
 
    iTransferController = CCCETransferController::NewL( *this );
    iPluginManager = CCCEPluginManager::NewL( *this, *iTransferController );
    iEmergencyCall =  CCCEEmergencyCall::NewL(*this, *iPluginManager);
    iPluginManager->SetEmergencyCallObserver(*iEmergencyCall);
    iExtension = CCCEExtension::NewL(*iPluginManager );
    iConferenceCall = CCCEConferenceCall::NewL( *this, *iPluginManager );
    if( iMCCEObserver )
        {
        iPluginManager->SetObserver(*iMCCEObserver);
        }
 
    // AirTime Timer
    iCSDuration = CCCEDurationTimer::NewL();
    iPSDuration = CCCEDurationTimer::NewL();
    
    iIdle = CIdle::NewL( CActive::EPriorityIdle );
    }
    
// ---------------------------------------------------------------------------
// Default Symbian constructor
// ---------------------------------------------------------------------------
//
CCCECallContainer* CCCECallContainer::NewL( MCCEObserver& aObserver )
    {
    CCCECallContainer* self = new( ELeave ) CCCECallContainer(aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCCECallContainer::~CCCECallContainer()
    {
    delete iConferenceCall;
    delete iEmergencyCall;
    delete iDtmf;
    delete iExtension;
    delete iPluginManager;
    delete iTransferController;
        
    if( iCallArray )
        {
        iCallArray->ResetAndDestroy();
        delete iCallArray;
        }
    
    delete iCallStateMediator;
        
    iCallstoRelease.Reset();
    iCallstoRelease.Close();
    
    delete iCSDuration;
    delete iPSDuration;
    
    
    if( iIdle )
        {
        iIdle->Cancel();
        delete iIdle;
        }
    FeatureManager::UnInitializeLib();
    }
// -----------------------------------------------------------------------------
// SetObserver()
// -----------------------------------------------------------------------------
//
void CCCECallContainer::SetObserver( MCCEObserver& aObserver, MCCESsObserver& aSsObserver )
    {
    iMCCEObserver = &aObserver;
    iMCCESsObserver = &aSsObserver;
    iPluginManager->SetObserver(aObserver );
    }

// -----------------------------------------------------------------------------
// Create call objects
// CS objects are located in the beginning of array and VoIP objects after those
// -----------------------------------------------------------------------------
//
void CCCECallContainer::CreateCallDataArraysL()
    {
    CCCECall* call = NULL;
    TInt callIndex = 0;

    iCallArray = new ( ELeave ) CArrayPtrFlat<CCCECall>( KCCECallObjects );

    for( callIndex = 0; callIndex < KCCECallObjects; callIndex++ )
        {
        call = CCCECall::NewLC( *this, callIndex );
        iCallArray->AppendL( call );
        CleanupStack::Pop( call );
        }
    }

// -----------------------------------------------------------------------------
// Swap PS and CS air duration timers
// Is called after handover, when call type has changed
// -----------------------------------------------------------------------------
//
void CCCECallContainer::AirTimeDurationSwap(
                           const CCPCall::TCallType aNewCallType)
    {
    CCELOGSTRING("CCCECallContainer::AirTimeDurationSwap(): IN");
    
    CCCECall* call = NULL;
    TInt calls(0);
    for ( TInt i = 0; i < KCCECallObjects; i++ )
        {
        call = GetCall( i );
        if ( call->State() == CCPCall::EStateConnected ||
             call->State() == CCPCall::EStateHold  )
            {
            calls ++;
            }
        }

    if( CCPCall::EStateConnected == iEmergencyCall->State() )
        {
        calls ++;
        }
    
    if( 1 == calls )
        {
        // After handover we will have only one active call
        // If we have more or less, timer swapping will not happend
        if (aNewCallType == CCPCall::ECallTypePS )
            {
            CCELOGSTRING2("CCCECallContainer::AirTimeDurationSwap(): CS Stops, PS restarts from %d",
                    iCSDuration->NumberOfBeats() );
            iCSDuration->Stop();
            iPSDuration->RestartAirTimeDuration(iCSDuration->NumberOfBeats());
            iCSDuration->Reset();
            }
        else
            {
            CCELOGSTRING2("CCCECallContainer::AirTimeDurationSwap(): PS Stops, CS restarts from %d",
                    iPSDuration->NumberOfBeats() );
            iPSDuration->Stop();
            iCSDuration->RestartAirTimeDuration(iPSDuration->NumberOfBeats());
            iPSDuration->Reset();
            }
        }
    CCELOGSTRING("CCCECallContainer::AirTimeDurationSwap(): OUT");
    }
    
// ---------------------------------------------------------------------------
// AirTimeDurationStop()
// ---------------------------------------------------------------------------
//
void CCCECallContainer::AirTimeDurationStop( CCCECall* /*aCall*/ ) const
    {
    CCELOGSTRING("CCCECallContainer::AirTimeDurationStop(): IN");
    CCCECall* call = NULL;
    TInt csCalls=0;
    TInt psCalls=0;

    // Check if any other calls    
    for ( TInt i = 0; i < KCCECallObjects; i++ )
        {
        call = GetCall( i );
        if ( call->State() == CCPCall::EStateConnected ||
             call->State() == CCPCall::EStateHold  )
            {           
            if ( IsCsPlugin( call, EFalse ) )
                {
                csCalls++;
                CCELOGSTRING2("   AirTimeDurationStop() csCalls=%d ", csCalls);
                }
            // ps call found
            else
                {
                psCalls++;
                CCELOGSTRING2("   AirTimeDurationStop() psCalls=%d ", psCalls);
                }
            }
        }
        
    if( CCPCall::EStateConnected == iEmergencyCall->State() )
        {
        if(  CCPCall::ECallTypePS == iEmergencyCall->Parameters().CallType() )
            {
            psCalls++;
            }
        else
            {
            csCalls++;
            }
        }
        
           
    // No other cs calls found
    if( !csCalls )
        {
        CCELOGSTRING("   AirTimeDurationStop() stop CS timer");
        iCSDuration->Stop();
        }
           
    // No other ps calls found
    if( !psCalls )
        {
        CCELOGSTRING("   AirTimeDurationStop() stop PS timer");
        iPSDuration->Stop();
        }
 
        
    CCELOGSTRING("CCCECallContainer::AirTimeDurationStop(): IN");
    }

// ---------------------------------------------------------------------------
// AirTimeDurationStart()
// ---------------------------------------------------------------------------
//
void CCCECallContainer::AirTimeDurationStart( CCCECall* /*aCall*/ ) const
    {
    CCELOGSTRING("CCCECallContainer::AirTimeDurationStart(): IN");
    CCCECall* call = NULL;
    TInt csCalls=0;
    TInt psCalls=0;

    // Check if any other calls    
    for ( TInt i = 0; i < KCCECallObjects; i++ )
        {
        call = GetCall( i );
        if ( call->State() == CCPCall::EStateConnected ||
             call->State() == CCPCall::EStateHold  )
            {      
            if ( IsCsPlugin( call, EFalse ) )
                {
                csCalls++;
                CCELOGSTRING2("   AirTimeDurationStart() csCalls=%d ", csCalls);
                }
            // ps call found
            else
                {
                psCalls++;
                CCELOGSTRING2("   AirTimeDurationStart() pscalls=%d ", psCalls);
                }
            }
        }
        
    if( CCPCall::EStateConnected == iEmergencyCall->State() )
        {
        if(  CCPCall::ECallTypePS == iEmergencyCall->Parameters().CallType() )
            {
            psCalls++;
            }
        else
            {
            csCalls++;
            }
        }
        
    // No other cs calls found
    if( csCalls )
        {
        iCSDuration->StartAirTimeDuration();
        }
    // No other ps calls found
    if( psCalls )
        {
        iPSDuration->StartAirTimeDuration();
        }
    CCELOGSTRING("CCCECallContainer::AirTimeDurationStart(): OUT");
    }

// ---------------------------------------------------------------------------
// From base class MCCECallArray
// ---------------------------------------------------------------------------
//
MCCECallInfo* CCCECallContainer::EmergencyCallInfo() const
    {
    return iEmergencyCall;
    }

// ---------------------------------------------------------------------------
// From base class MCCECallArray
// ---------------------------------------------------------------------------
//
MCCECallInfo* CCCECallContainer::CallInfo( TInt aIndex ) const  
    {
    return iCallArray->At( aIndex );
    }

// ---------------------------------------------------------------------------
// From base class MCCECallArray
// ---------------------------------------------------------------------------
//  
TInt CCCECallContainer::MaxNumberOfCalls() const
    {
    return iCallArray->Count();
    }

// ---------------------------------------------------------------------------
// Add calls to be removed, and start releasing if not already running
// ---------------------------------------------------------------------------
//
void CCCECallContainer::ScheduleReleaseCall( MCCPCall& aCalltoRemove, const TUid aUid )
    {
    TCallReleaseEntry entry;
    entry.iCallToRelease = &aCalltoRemove;
    entry.iUid = aUid;
    iCallstoRelease.Append(entry); 
    if (!iIdle->IsActive())
        {
        iIdle->Start( TCallBack(ReleaseCalls,this) );
        }
    }

// ---------------------------------------------------------------------------
// Callback to CIdle class to actually release calls
// ---------------------------------------------------------------------------
// 
TInt CCCECallContainer::ReleaseCalls( TAny* aCallContainer )
    {
    CCCECallContainer* container = 
        static_cast< CCCECallContainer* >( aCallContainer );
        container->DoReleaseCalls();
   
    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// Callback to CIdle class to actually release calls
// ---------------------------------------------------------------------------
//     
void CCCECallContainer::DoReleaseCalls()
    {
    CCELOGSTRING("CCCECallContainer::DoReleaseCalls()")
    while( iCallstoRelease.Count() > 0 )
        {
        CConvergedCallProvider* plugin = NULL;
        TUid pluginUid = iCallstoRelease[0].iUid;
        TRAP_IGNORE( plugin = iPluginManager->GetPluginL( pluginUid ) );
        if( plugin )
            {
            // release call
            plugin->ReleaseCall( *iCallstoRelease[0].iCallToRelease );
            
            // first check can we indicate plugin release
            if( !iEmergencyCall->IsInProgress() &&
                iPluginManager->ReleaseWhenIdle( pluginUid ) &&
                pluginUid != iConferenceCall->ImplementationUid() )
                {
                // 2nd step
                if( 0 == CountCalls( pluginUid ) )
                    {
                    TBool releasePlugin = ETrue;
                    // and last test
                    if( 1 < iCallstoRelease.Count() )
                        {
                        for( TInt a = 1; a < iCallstoRelease.Count(); a++ )
                            {
                            if( iCallstoRelease[a].iUid == pluginUid )
                                {
                                releasePlugin = EFalse;
                                }
                            }
                        }
                    
                    if( releasePlugin )
                        {
                        iPluginManager->RemovePlugin( pluginUid );
                        }
                    }
                }
            }
        iCallstoRelease.Remove(0);
        }
  
    };

// ---------------------------------------------------------------------------
// ConferenceCallL()
// ---------------------------------------------------------------------------
//
CCCEConferenceCall& CCCECallContainer::ConferenceCallL( 
    const MCCEConferenceCallObserver& aObserver )
    {
    if( iConferenceCall )
        {
        iConferenceCall->SetObserver( 
            const_cast<MCCEConferenceCallObserver&>( aObserver ) );
        }
    else
        {
        iConferenceCall = CCCEConferenceCall::NewL( *this, *iPluginManager );
        iConferenceCall->SetObserver( 
            const_cast<MCCEConferenceCallObserver&>( aObserver ) );
        }
    return *iConferenceCall;
    }

// ---------------------------------------------------------------------------
// ConferenceCall()
// ---------------------------------------------------------------------------
//
CCCEConferenceCall& CCCECallContainer::ConferenceCall() const
    {
    return *iConferenceCall;
    }

// ---------------------------------------------------------------------------
// DtmfInterfaceL()
// ---------------------------------------------------------------------------
//
CCCEDtmf& CCCECallContainer::DtmfInterfaceL( 
    const MCCEDtmfObserver& aObserver )
    {
    if( iDtmf )
        {
        iDtmf->SetObserver( aObserver );
        }
    else
        {
        iDtmf = CCCEDtmf::NewL( *this, *iPluginManager );
        iDtmf->AddObserverL( aObserver );
        }
    return *iDtmf;
    }

// ---------------------------------------------------------------------------
// ExtensionInterfaceL()
// ---------------------------------------------------------------------------
// 

MCCEExtensionInterface& CCCECallContainer::ExtensionInterfaceL(
        const MCCEExtensionObserver& aObserver )
    {
    if( iExtension )
        {
        iExtension->AddObserverL( aObserver );
        }
    else
        {
        iExtension = CCCEExtension::NewL(*iPluginManager );
        iExtension->AddObserverL( aObserver ); 
        }
    return *iExtension;
    }
  
// ---------------------------------------------------------------------------
// GetNextFreeCall()
// ---------------------------------------------------------------------------
//
CCCECall* CCCECallContainer::GetNextFreeCall() const
    {
    CCCECall* call = NULL;
    if ( !iEmergencyCall->IsInProgress())
        {
        for (TInt i = 0; i < KCCECallObjects; i++ )
            {
            call = GetCall( i );

            if ( !call->IsInUse() )
                {
                CCELOGSTRING("CCCE::GetNextFreeCall: Call found");
                break;
                }
            }
        }
  
    return call;
    }
  
// ---------------------------------------------------------------------------
// GetCall()
// ---------------------------------------------------------------------------
//
CCCECall* CCCECallContainer::GetCall( CCPCall::TCallState aCallState, 
    CCCECall* aSkippedCall ) const
    {
    CCCECall* call = NULL;
    CCPCall::TCallState callState = CCPCall::EStateIdle;

    for (TInt i = 0; i < KCCECallObjects; i++ )
        {
        call = GetCall( i );

        // Skip given call
        if ( call == aSkippedCall )
            {
            continue;
            }


        callState = call->State();
        if ( callState == aCallState )
            {
            return call;
            }

        }

    return NULL;
    }

// -----------------------------------------------------------------------------
// Open new MCCP call and bind it to MCCECall
// -----------------------------------------------------------------------------
//
 MCCECall& CCCECallContainer::OpenNewCallL(
    const TDesC&                        aNumber,
    const CCCECallParameters& aCallParameters,
    const MCCECallObserver&             aCallObserver )
    {
    CCCECall* call = GetNextFreeCall();
    
    if(call)
        {
        CleanupReleasePushL( *call );
        
        MCCPCall* convergedCall = NULL;
        // This block could be put to own method in transfercontroller
        if( iTransferController->TransferCall() &&
            CCCETransferController::ECCETypeUnattended == 
            iTransferController->OngoingTransferType() && 
            aNumber == iTransferController->TransferCall()->RemoteParty() )
            {
            CCELOGSTRING("CCCE::OpenNewCallL: Use transfer call");
            convergedCall = iTransferController->TransferCall();        
            // For MO transfer call observer needs to be set 
            convergedCall->AddObserverL( *call );
            call->SetTransferController( iTransferController ); 
            
            convergedCall->SetParameters( aCallParameters );
            }
        else
            {
            CCELOGSTRING2("CCCE::OpenNewCallL: ServiceId = %d", aCallParameters.ServiceId() );
            CConvergedCallProvider* plugin = 
                iPluginManager->GetPluginL( aCallParameters.ServiceId() );
            convergedCall = plugin->NewCallL(aCallParameters, aNumber, *call);
            }
        
        if(convergedCall)
            {
            CCELOGSTRING("CCCE::OpenNewCallL: Set converged call to engine call");
            call->SetConvergedCallL( convergedCall, convergedCall->Uid() );
            call->SetObserver( aCallObserver );
            
            CCELOGSTRING("CCCE::OpenNewCallL():OUT");
            }
        else
            {
            CCELOGSTRING("CCCE::OpenNewCallL():No converged call:OUT");
            User::Leave( KErrGeneral );
            }
        CleanupStack::Pop( call );
        }
    else
        {
        CCELOGSTRING("CCCE::OpenNewCallL():No call:OUT");
        User::Leave( KErrGeneral );
        }

    return *call;
    }
// -----------------------------------------------------------------------------
// ReleaseCall
// -----------------------------------------------------------------------------
//
TInt CCCECallContainer::ReleaseCall( MCCPCall& aCalltoRelease, const TUid aUid )
    {
    
    CCELOGSTRING("CCCECallContainer::ReleaseCall( MCCPCall& aCalltoRemove, const TUid aUid )");
    ScheduleReleaseCall(aCalltoRelease,aUid);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// ReleaseCall
// -----------------------------------------------------------------------------
//
TInt CCCECallContainer::ReleaseCall( MCCPCall& aCalltoRelease)
    {
    CCELOGSTRING("CCCECallContainer::ReleaseCall( MCCPCall& aCalltoRelease )");
    TUid pluginUid = aCalltoRelease.Uid();
    return ReleaseCall(aCalltoRelease, pluginUid);
    }

// -----------------------------------------------------------------------------
// Get emergencycall
// -----------------------------------------------------------------------------
//
 MCCECall& CCCECallContainer::EmergencyCall( const MCCECallObserver& aCallObserver )
    {
    iEmergencyCall->SetObserver(aCallObserver);
    return *iEmergencyCall;
    }

// -----------------------------------------------------------------------------
// Get call object by call MCCPCall 
// -----------------------------------------------------------------------------
//
CCCECall* CCCECallContainer::GetCall( const MCCPCall* aMCCPCall ) const
    {
    CCCECall* call = NULL;
    
    for( TInt i = 0; i < iCallArray->Count(); i++ )
        {        
        call = iCallArray->At(i);
        
        if( aMCCPCall == &call->GetCCPCall() )
            {
            CCELOGSTRING("CCCECallContainer::GetCall: Call found");
            break;
            }
            
        call = NULL;
        }
    
    return call;
    }    

// ---------------------------------------------------------------------------
// GetCall()
// ---------------------------------------------------------------------------
//
TInt CCCECallContainer::GetCall( TUid aImplementationUid, 
    RPointerArray<CCCECall>& aCallArray ) const
    {
    CCCECall* call = NULL;

    for ( TInt i = 0; i < KCCECallObjects; i++ )
        {
        call = GetCall( i );
        if ( call->IsInUse() )
            {
            if( call->ImplementationUid() == aImplementationUid )
                {
                // return value ignored. Ownership of the call object is not 
                // transferred to array. 
                aCallArray.Append(call); 
                }
            }
        }

    return aCallArray.Count();
    }

// ---------------------------------------------------------------------------
// IsAnyActiveOrConnectingCall()
// ---------------------------------------------------------------------------
//
TInt CCCECallContainer::GetActiveOrConnectingCalls(
    RPointerArray<MCCECallInfo>& aCallArray ) const
    {
    CCPCall::TCallState callState = CCPCall::EStateIdle;
    CCCECall* call = NULL;
    TInt err = KErrNotFound;
    TInt i = 0;

    for ( i = 0; i < KCCECallObjects; i++ )
        {
        call = GetCall( i );
        if ( call->IsInUse() )
            {
            callState = call->State();
            if ( callState == CCPCall::EStateDialling ||
                 callState == CCPCall::EStateConnecting ||
                 callState == CCPCall::EStateConnected )
                {
                // Ownership of the call object is not 
                // transferred to array.
                err = aCallArray.Append( call );
                
                }
            }
        }
    
    if( iEmergencyCall->IsInProgress() )
        {
        callState = iEmergencyCall->State();
        if ( callState == CCPCall::EStateDialling ||
             callState == CCPCall::EStateConnecting ||
             callState == CCPCall::EStateConnected )
            {
            // Ownership of the iEmergencyCall object is not transfered.
            err = aCallArray.Append( iEmergencyCall );
            }
        }
        
    return err;
    }

// ---------------------------------------------------------------------------
// IsAnyActiveOrConnectingCall()
// ---------------------------------------------------------------------------
//
TInt CCCECallContainer::CountCalls( TUid aImplementationUid ) const
    {
    CCCECall* call = NULL;
    TInt i = 0;
    TInt count=0;
    
    for ( i = 0; i < KCCECallObjects; i++ )
        {
        call = GetCall( i );
        if ( call->IsInUse() )
            {
            if( call->ImplementationUid() == aImplementationUid )
                {
                count++;
                }
            }
        }

    if( iEmergencyCall->IsInProgress() )
        {
        if( iEmergencyCall->ImplementationUid() == aImplementationUid )
            {
            count++;
            }
        }
    
    return count;
    }

// ---------------------------------------------------------------------------
// CountCsCalls()
// ---------------------------------------------------------------------------
//
TInt CCCECallContainer::CountCsCalls( TUid aImplementationUid ) const
    {
    TInt count=0;
    CCCECall* call = NULL;
    TInt i = 0;
        
    for ( i = 0; i < KCCECallObjects; i++ )
        {
        call = GetCall( i );
        if ( call->IsInUse() )
            {
            if( call->ImplementationUid() == aImplementationUid && 
              ( call->Parameters().CallType() !=  CCPCall::ECallTypePS ) )
                {
                count++;
                }
            }
        }
    
    return count;
    }

// ---------------------------------------------------------------------------
// GetCallWithActiveRequest()
// ---------------------------------------------------------------------------
//
CCCECall* CCCECallContainer::GetCallWithActiveRequest( CCCECall* aCall ) const
    {
    CCCECall* call = NULL;
    for( TInt i = 0; i < iCallArray->Count(); i++ )
        {        

        if( CCCECall::ECCENone != iCallArray->At(i)->Request() )
            {
            CCELOGSTRING( "CCCECallContainer::GetCallWithRequest: Call found" );
            if( iCallArray->At(i) != aCall )
                {
                call = iCallArray->At(i);
                }
            }
        }

    return call;
    }

// ---------------------------------------------------------------------------
// GetNonConferenceCall()
// ---------------------------------------------------------------------------
//
CCCECall* CCCECallContainer::GetNonConferenceCall() const
    {
    CCCECall* call = NULL;
    for( TInt i = 0; i < iCallArray->Count(); i++ )
        {        
        if( iCallArray->At(i)->IsInUse() )
            {
            if( !iConferenceCall->IsPartOfConference( *iCallArray->At(i) ) )
                {
                CCELOGSTRING( "CCCECallContainer::GetNonConferenceCall: Call found" );
                call = iCallArray->At(i);
                break;
                }
            }
        }

    return call;
    }
    
// -----------------------------------------------------------------------------
// Handle Conference status changes.
// If dial was pending for hold and hold is received, continue dial.
// -----------------------------------------------------------------------------
//

void CCCECallContainer::HandleConferenceStatusChange( 
        MCCEConferenceCallObserver::TCCEConferenceCallState aStatus )
    {
  
    CCELOGSTRING2("CCCECallContainer::HandleConferenceStatusChange: %d", aStatus );    
    CCCECall* callWithRequest = GetCallWithActiveRequest( NULL );

    switch ( aStatus )
        {
        case MCCEConferenceCallObserver::ECCEConferenceIdle:
            if ( callWithRequest &&
                 callWithRequest->Request() == CCCECall::ECCEReplaceActive &&
                 !GetCall( CCPCall::EStateConnected, callWithRequest ) )
                {
                callWithRequest->DoPendingRequest();
                }
            break;
        
        case MCCEConferenceCallObserver::ECCEConferenceHold:
            if( iConferenceCall->EnumerateCalls() != 0 )
                {
                // Swap 1/2 point ( two calls in hold ) if there isnt connected call allready
                CCCECall* call = GetNonConferenceCall();
                if( call && 
                    call->State() == CCPCall::EStateHold )
                    {
                    if( call->ImplementationUid() == KCSProviderUid && 
                        iConferenceCall->ImplementationUid() == KCSProviderUid )
                        {
                        // If there is two cs calls do nothing
                        }
                    else
                        {
                        call->ResumeNoNotify();
                        }

                    break;
                    }
                
                // Answer new call when one(ringing) ongoing
                // Answer new call when one(queued) ongoing
                // Dial new call when one ongoing
                if( callWithRequest )
                    {
                    callWithRequest->DoPendingRequest();
                    }
                }
            break;
        case MCCEConferenceCallObserver::ECCEConferenceActive:
        
            // Swap completed
            if( callWithRequest )
                {
                callWithRequest->RequestComplete( ECCPErrorNone );
                }
        default:
            break;

        }       
    }

// -----------------------------------------------------------------------------
// Updates the call information API clients.
// -----------------------------------------------------------------------------
//
void CCCECallContainer::HandleEmergencyCallStatusChange(
    CCECallEnums::TNotifyType aNotify )
    {
    CCELOGSTRING("CCCECallContainer::HandleEmergencyCallStatusChange(): IN");
    if ( aNotify != CCECallEnums::ENoNotify )
        {
        iCallStateMediator->UpdateCallInfos();
        }
    CCELOGSTRING("CCCECallContainer::HandleEmergencyCallStatusChange(): OUT");
    }

// -----------------------------------------------------------------------------
// Handle call status changes.
// If dial was pending for hold and hold is received, continue dial.
// If swap is active, and hold was received for wanted object, start resume other call
// If swap is active, and resume was received for wanted call, complete swap request
// -----------------------------------------------------------------------------
//
void CCCECallContainer::HandleCallStatusChange( 
    CCPCall::TCallState aState, 
    CCCECall* aCall,
    CCECallEnums::TNotifyType aNotify )
    {
    CCELOGSTRING( "CCCECallContainer::HandleCallStatusChange(): IN" );
    CCCECall* call = NULL;
    TInt error = KErrNone;
    CCCECall* callWithRequest = GetCallWithActiveRequest( aCall );

    switch ( aState )
        {
        case CCPCall::EStateConnected:
            AirTimeDurationStart( aCall );
        
            // Swap completed
            if( callWithRequest )
                {
                callWithRequest->RequestComplete( ECCPErrorNone );
                }
            break;

        case CCPCall::EStateIdle:
            AirTimeDurationStop( aCall );
            
            if ( callWithRequest &&
                callWithRequest->Request() == CCCECall::ECCEReplaceActive &&
                !iConferenceCall->IsPartOfConference( *aCall ) &&
                !GetCall( CCPCall::EStateConnected, aCall ) )
                {
                error = callWithRequest->DoPendingRequest();
                }
            break;
            
        case CCPCall::EStateHold:
            // Check if there is conference call
            if( iConferenceCall->EnumerateCalls() != 0 )
                {
                // Do nothing when there is only cscalls

                if ( IsCsPlugin( aCall, ETrue ) )
                    {
                    
                    break;
                    }
                    
                // If call or conferencecall isn`t cscall then do conference swap
                if( iConferenceCall->State() == MCCEConferenceCallObserver::ECCEConferenceHold &&
                        !iConferenceCall->IsPartOfConference( *aCall ) )
                    {
                    iConferenceCall->Swap();
                    }
                break;
                }
        
            // Swap 1/2 point ( two calls in hold ) if there isnt connected call allready
            call = GetCall( CCPCall::EStateHold, aCall );
            if( call && !GetCall( CCPCall::EStateConnected, aCall ) )
                {
                
                if ( IsCsPlugin( call, EFalse ) && IsCsPlugin( aCall, EFalse ) )
                    {
                    // If there is two cs calls do nothing
                    }
                else
                    {
                    error = call->ResumeNoNotify();
                    }

                break;
                }

            // Answer new call when one(ringing) ongoing
            // Answer new call when one(queued) ongoing
            // Dial new call when one ongoing. NOTICE: Idle!
            if( callWithRequest )
                {
                error = callWithRequest->DoPendingRequest();
                }
            break;

        default:
            CCELOGSTRING2("CCCECallContainer::HandleCallStatusChange: \
                Unhandled state = %d", aState );
            break;
        }
        
    if( error != KErrNone )
        {
        // Do some error handling
        }
    if ( aNotify != CCECallEnums::ENoNotify )
        {
        iCallStateMediator->UpdateCallInfos();
        }
    CCELOGSTRING( "CCCECallContainer::HandleCallStatusChange(): OUT" );
    }

// -----------------------------------------------------------------------------
// contained call has failed
// -----------------------------------------------------------------------------
//
void CCCECallContainer::ErrorOccurred( CCCECall* aCall, TCCPError aError )
    {
    
    CCCECall* callWithRequest = GetCallWithActiveRequest( aCall );
    if( callWithRequest )
        {
        // When swap fails try to resume call with request
        if( callWithRequest->Request() == CCCECall::ECCESwap && 
            aCall != callWithRequest && 
            !GetCall( CCPCall::EStateConnected, NULL ) )
            {
            callWithRequest->ResumeNoNotify();
            }
        if( aError == ECCPLocalHoldFail && 
            callWithRequest->Request() == CCCECall::ECCEDial &&
            CCPCall::ECallTypePS != callWithRequest->Parameters().CallType() )
            {
            callWithRequest->RequestComplete( ECCPErrorAlreadyInUse );
            }
        else
            {
            callWithRequest->RequestComplete( aError );
            }
        }
    }

// -----------------------------------------------------------------------------
// Search for first available callinfo call id and save it to array.
// -----------------------------------------------------------------------------
//
void CCCECallContainer::ReserveCallInfoCallId( TInt aCallId )
    {
    for ( TInt i = 0; i < KCCECallObjects; i++ )
        {
        if ( KErrNotFound == iCallInfoCallId[ i ] )
            {
            iCallInfoCallId[ i ] = aCallId;
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// Release call's call id.
// -----------------------------------------------------------------------------
//
void CCCECallContainer::ReleaseCallInfoCallId( TInt aCallId )
    {
    // Clear CallInfoCallId from array.
    for ( TInt i = 0; i < KCCECallObjects; i++ )
        {
        if ( iCallInfoCallId[ i ] == aCallId )
            {
            iCallInfoCallId[ i ] = KErrNotFound;
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// Get call object by call id
// -----------------------------------------------------------------------------
//
CCCECall* CCCECallContainer::GetCall( TInt aCallId ) const
    {
    if(iCallArray->Count()>aCallId)
        {
        return iCallArray->At( aCallId );
        }
    else
        {
        return NULL;
        }
    }

// -----------------------------------------------------------------------------
// Get call durations
// -----------------------------------------------------------------------------
//
void CCCECallContainer::GetAirTimeDuration( TTimeIntervalSeconds& aCSDuration, 
        TTimeIntervalSeconds& aPSDuration)
    {
    CCELOGSTRING("CCCECallContainer::GetAirTimeDuration()");
    aCSDuration = iCSDuration->NumberOfBeats();
    aPSDuration = iPSDuration->NumberOfBeats();
    }
    
// -----------------------------------------------------------------------------
// TerminateAllButRingingAndQueuedCalls
// -----------------------------------------------------------------------------
//    
void CCCECallContainer::TerminateAllButRingingAndQueuedCalls()
    {
    CCELOGSTRING("CCCECallContainer::TerminateAllButRingingAndQueuedCalls():IN");
    
    for( TInt i = 0; i < KCCECallObjects; i++ )
        {
        CCCECall* call = GetCall( i );
        if( call->IsInUse() && 
            call->State() != CCPCall::EStateQueued && 
            call->State() != CCPCall::EStateRinging &&
            !iConferenceCall->IsPartOfConference( *call ) )
            {
            // Quiet hangup
            switch( call->State() )
                {
                case CCPCall::EStateDialling:
                    call->GetCCPCall().Cancel();
                    break;
                case CCPCall::EStateAnswering:
                    call->GetCCPCall().Reject();
                    break;
                default:
                    call->GetCCPCall().HangUp();
                    break;
                }
            
            }
        }

    if( iConferenceCall->EnumerateCalls() )
        {
        iConferenceCall->HangUp();
        }

        
    CCELOGSTRING("CCCECallContainer::TerminateAllButRingingAndQueuedCalls():OUT");
    }
// -----------------------------------------------------------------------------
// TerminateAllCalls
// -----------------------------------------------------------------------------
//    
void CCCECallContainer::TerminateAllCalls()
    {
    CCELOGSTRING("CCCECallContainer::TerminateAllCalls():IN");
    
    for( TInt i = 0; i < KCCECallObjects; i++ )
        {
        CCCECall* call = GetCall( i );
        
        if( call->IsInUse() && 
            !iConferenceCall->IsPartOfConference( *call ) )
            {
            // Quiet hangup
            switch( call->State() )
                {
                case CCPCall::EStateDialling:
                    call->GetCCPCall().Cancel();
                    break;
                case CCPCall::EStateAnswering:
                case CCPCall::EStateRinging:
                case CCPCall::EStateQueued:
                    call->GetCCPCall().Reject();
                    break;
                default:
                    call->GetCCPCall().HangUp();
                    break;
                }
            }
        
        }

    if( iConferenceCall->EnumerateCalls() )
        {
        iConferenceCall->HangUp();
        }

    CCELOGSTRING("CCCECallContainer::TerminateAllCalls():OUT");
    }

// -----------------------------------------------------------------------------
// TerminateAllVoIPCalls
// -----------------------------------------------------------------------------
//
void CCCECallContainer::TerminateAllVoIPCalls()
    {
    CCELOGSTRING("CCCECallContainer::TerminateAllVoIPCalls():IN");

    for( TInt i = 0; i < KCCECallObjects; i++ )
        {
        CCCECall* call = GetCall( i );
        
        //Only VoIP calls are terminated
        if( call->IsInUse() && 
                !iConferenceCall->IsPartOfConference( *call ) && 
                !IsCsPlugin( call, ETrue ) )
            {
            // Quiet hangup
            switch( call->State() )
                {
                case CCPCall::EStateDialling:
                    call->GetCCPCall().Cancel();
                    break;
                case CCPCall::EStateAnswering:
                case CCPCall::EStateRinging:
                case CCPCall::EStateQueued:
                    call->GetCCPCall().Reject();
                    break;
                default:
                    call->GetCCPCall().HangUp();
                    break;
                }
            }

        }
    
    //Hang up VoIP conference
    if( iConferenceCall->EnumerateCalls() &&
            iConferenceCall->Parameters().CallType() == 
            CCPCall::ECallTypePS )
        {
        iConferenceCall->HangUp();
        }

    CCELOGSTRING("CCCECallContainer::TerminateAllVoIPCalls():OUT");
    }

// -----------------------------------------------------------------------------
// Return CCE observer
// -----------------------------------------------------------------------------
//    
MCCEObserver* CCCECallContainer::CCEObserver()
    {
    return iMCCEObserver;
    }

// -----------------------------------------------------------------------------
// Return ss observer
// -----------------------------------------------------------------------------
//    
MCCESsObserver* CCCECallContainer::SsObserver()
    {
    return iMCCESsObserver;
    }



// -----------------------------------------------------------------------------
// GetLifeTime()
// -----------------------------------------------------------------------------
// 
TBool CCCECallContainer::GetLifeTime( TDes8& aLifeTimeInfo )
    {
    CCELOGSTRING("CCCE::GetLifeTime()");
    
    CConvergedCallProvider* plugin = NULL;
    
    TRAP_IGNORE( plugin = iPluginManager->GetPluginL(KCSServiceId));
    
    if( plugin )
        {
        plugin->GetLifeTime(aLifeTimeInfo);
        }
    else
        {
        CCELOGSTRING("CCCE::GetLifeTime() - CS plugin not found!");
        return EFalse;
        }

    return ETrue;
    }

// -----------------------------------------------------------------------------
// GetCSInfo()
// -----------------------------------------------------------------------------
// 
TBool CCCECallContainer::GetCSInfo( CSInfo& aCSInfo )
    {
    CCELOGSTRING("CCCE::GetCSInfo()");

    CConvergedCallProvider* plugin = NULL;
    
    TRAP_IGNORE( plugin = iPluginManager->GetPluginL(KCSServiceId));
        
    
    if( plugin )
        {
        plugin->GetCSInfo(aCSInfo);
        }
    else
        {
        CCELOGSTRING("CCCE::GetCSInfo() - CS plugin not found!");
        return EFalse;
        }

    return ETrue;
    }

// ---------------------------------------------------------------------------
// CCCECallContainer::IsCsPlugin
// ---------------------------------------------------------------------------
//
TBool CCCECallContainer::IsCsPlugin(CCCECall* aCall, TBool aConference ) const
    {
    TBool isCsPlugin( EFalse );
    if ( FeatureManager::FeatureSupported( KFeatureIdFfVoiceCallContinuity ) )
        {
        if ( aCall->Parameters().CallType() != 
                CCPCall::ECallTypePS )
            {
            isCsPlugin = ETrue;
            if( aConference &&
                iConferenceCall->Parameters().CallType() == 
                    CCPCall::ECallTypePS )
                {
                isCsPlugin = EFalse;
                }
            }
        }
    else
        {
        if( aCall->ImplementationUid() == KCSProviderUid )
            {
            isCsPlugin = ETrue;
            }
        }
    return isCsPlugin;
    }


//end of file
