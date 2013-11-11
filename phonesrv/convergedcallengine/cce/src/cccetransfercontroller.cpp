/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contains transfer handling logic
*
*/

#include "cccelogger.h"
#include "cccetransfercontroller.h"
#include "cccecallcontainer.h"
#include "cccecall.h"
#include "mccpcall.h"
#include "mccpcscall.h"
#include "mccecallobserver.h"

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CCCETransferController::CCCETransferController( 
    CCCECallContainer& aCallContainer ) :
    iCallContainer( aCallContainer )
    {
    Reset();
    }

// ---------------------------------------------------------------------------
// CCCETransferController::NewL
// ---------------------------------------------------------------------------
//
CCCETransferController* CCCETransferController::NewL( 
    CCCECallContainer& aCallContainer )
    {
    CCCETransferController* self = new( ELeave ) CCCETransferController(
        aCallContainer );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCCETransferController::~CCCETransferController()
    {
    Reset();
    }

// ---------------------------------------------------------------------------
// CCCETransferController::ErrorOccurred
// ---------------------------------------------------------------------------
//
void CCCETransferController::ErrorOccurred( 
    const TCCPError aError, MCCPCall* /*aCall*/ ) 
    {
    CCELOGSTRING2("CCCETransferController::ErrorOccurred() aError=%d", aError);    
    (void) aError; // remove urel compile warning 
    }
        
// ---------------------------------------------------------------------------
// CCCETransferController::CallStateChanged
// ---------------------------------------------------------------------------
//   
void CCCETransferController::CallStateChanged( 
    const MCCPCallObserver::TCCPCallState aState, MCCPCall* /*aCall*/ ) 
    {
    CCELOGSTRING2("CCCETransferController::CallStateChanged():IN \
            aState=%d", aState);
    
    switch ( aState )
        {
        case ECCPStateIdle:
            {
            CCELOGSTRING("CCCETransferController::CallStateChanged: ECCPStateIdle");
            if ( iOriginatorCall && iObservedCall == iOriginatorCall )
                {
                CCELOGSTRING("CCCETransferController::CallStateChanged: Idle -> release original call");
                iCallContainer.ReleaseCall( *iOriginatorCall, iOriginatorCall->Uid() ); 
                iOriginatorCall = NULL;
                iObservedCall = NULL;
                }
            else if ( iTransferCall && iObservedCall == iTransferCall )
                {
                CCELOGSTRING("CCCETransferController::CallStateChanged: Idle -> release transfer call");
                iCallContainer.ReleaseCall( *iTransferCall, iTransferCall->Uid() ); 
                iTransferCall = NULL;
                iObservedCall = NULL;
                }
            }
            break;  
        case ECCPStateConnected:
            {
            CCELOGSTRING("CCCETransferController::CallStateChanged: ECCPStateConnected ");
            CCCECall* originator = iCallContainer.GetCall( iOriginatorCall );
            
            if ( !originator ||
                 ( originator->State() != CCPCall::EStateHold &&
                 originator->State() != CCPCall::EStateConnected ) )
                {
                // Originator disconnected or released.
                // Terminate transfer call.
                TInt err = iTransferCall->HangUp();
                if ( err )
                    {
                    ReleaseTransferCall();
                    }
                
                }
            else if (  ECCETypeAttendedMO == iOngoingTransferType )
                {
                // Transfer call was connected
                // Swap transfer call with original call and hangup&release
                // original call
                
                
                TRAPD( err, 
                       originator->SetConvergedCallL( iTransferCall, 
                           iTransferCall->Uid(), ETrue );
                       iTransferCall->RemoveObserver( *this );
                       iTransferCall->AddObserverL( *originator ); 
                       iOriginatorCall->RemoveObserver( *originator );
                       iOriginatorCall->AddObserverL( *this ); );

                
    
                iObservedCall = iOriginatorCall;
                
                if ( KErrNone == err )
                    {
                    // If original call was on hold, new call must be on hold also
                    if ( ECCPStateHold == originator->State() )
                        {
                        iTransferCall->Hold();
                        }
                   
                    // New call is no more on hold by remote end
                    static_cast<MCCPCallObserver*>(originator)->CallEventOccurred( ECCPRemoteResume, 
                                                                &originator->GetCCPCall() ); 
                    }
                else
                    {
                    CCELOGSTRING2("CCCETransferController::CallStateChanged():transfer error=%d",
                        err);
                    }
                }
            else if ( ECCETypeAttendedMT == iOngoingTransferType ) 
                {
                
                // Set new transferred call to original CCECall object
                TRAP_IGNORE( originator->SetConvergedCallL( iTransferCall, 
                    iTransferCall->Uid(), ETrue ) );
                
                
                // Release original call 
                iOriginatorCall->RemoveObserver( *originator );
                TRAP_IGNORE( iOriginatorCall->AddObserverL( *this ) );
                
                iObservedCall = iOriginatorCall;
                iOriginatorCall->HangUp();
                }
            }
            break;     
        default:
            CCELOGSTRING2("CCCETransferController::CallStateChanged: \
                    Unhandled state = %d",aState );
            break;
        }
    CCELOGSTRING("CCCETransferController::CallStateChanged():OUT");
    }

// ---------------------------------------------------------------------------
// CallStateChangedWithInband( TCCPCallState aState )
// ---------------------------------------------------------------------------
//
void CCCETransferController::CallStateChangedWithInband( TCCPCallState aState, 
                                                         MCCPCall* aCall ) 
    {
    CCELOGSTRING("CCCETransferController::CallStateChangedWithInband()");
    CallStateChanged( aState, aCall );    
    }

// ---------------------------------------------------------------------------
// CCCETransferController::CallEventOccurred
// ---------------------------------------------------------------------------
// 
void CCCETransferController::CallEventOccurred( 
    const MCCPCallObserver::TCCPCallEvent /*aEvent*/, MCCPCall* /*aCall*/ )
    {
    CCELOGSTRING("CCCETransferController::CallEventOccurred()");
    }

// ---------------------------------------------------------------------------
// CCCETransferController::CallCapsChanged
// ---------------------------------------------------------------------------
// 
void CCCETransferController::CallCapsChanged( const TUint32 /*aCapsFlags*/, 
                                              MCCPCall* /*aCall*/ ) 
    {
    CCELOGSTRING("CCCETransferController::CallCapsChanged():");
    }
    
// ---------------------------------------------------------------------------
// CCCETransferController::Reset
// ---------------------------------------------------------------------------
//
void CCCETransferController::Reset()
    {
    CCELOGSTRING("CCCETransferController::Reset()");
    iOriginatorCall = NULL;
    iOriginatorCallId = KErrNotFound;
    iTransferDialPending = EFalse;
    iTransferAccepted = EFalse;
    iOngoingTransferType = ECCETypeNone;
    iObservedCall = NULL;    
    }
    
// ---------------------------------------------------------------------------
// CCCETransferController::HandleRemoteTransferRequest
// ---------------------------------------------------------------------------
//
void CCCETransferController::HandleRemoteTransferRequest( 
    MCCPCall* aNewTransferCall,
    MCCPCall* aOriginator, 
    TBool aAttented )                            
    {
    CCELOGSTRING("CCCETransferController::HandleTransferRequest():IN");
    
    CCCECall* originator = iCallContainer.GetCall( aOriginator );
    if ( originator && 
        ( originator->State() == CCPCall::EStateHold ||
          originator->State() == CCPCall::EStateConnected ) )
        {
        iOriginatorCall = aOriginator;
        iTransferCall = aNewTransferCall;
        
        // Unattended case
        if ( !aAttented )
            {        
            CCELOGSTRING("CCCETransferController::HandleTransferRequest() unattended ");
            iOriginatorCallId = originator->CallId();       
            iOngoingTransferType = ECCETypeUnattended;     
            originator->HandleTransfer( aAttented, *this );
            }
        // Attended case
        else
            {
            CCELOGSTRING("CCCETransferController::HandleTransferRequest() attended ");
            originator->SetTransferController( this );
            iOngoingTransferType = ECCETypeAttendedMO; 
            TRAP_IGNORE( iTransferCall->AddObserverL( *this ) );
            
            iObservedCall = iTransferCall;
            TInt error = iTransferCall->Dial(); 
            
            if( error )
                {
                CCELOGSTRING2("CCCETransferController: \
                    dial error: %d", error);
                ReleaseTransferCall();    
                }
            }
        }
    else
        {
        CCELOGSTRING("CCCETransferController: Originator call not found!");
        iCallContainer.ReleaseCall( *aNewTransferCall );
        }
    
    CCELOGSTRING("CCCETransferController::HandleTransferRequest():OUT");
    }

// ---------------------------------------------------------------------------
// CCCETransferController::TransferCallIncoming
// ---------------------------------------------------------------------------
//
void CCCETransferController::TransferCallIncoming( MCCPCall* aNewTransferCall,
    MCCPCall* aOriginator )
    {
    CCELOGSTRING("CCCETransferController::TransferCallIncoming():IN");
    iTransferCall = aNewTransferCall;
    iOriginatorCall = aOriginator;
    
    CCCECall* originator = iCallContainer.GetCall( aOriginator );
    if ( originator )
        {
        originator->SetTransferController( this );
        
        iOngoingTransferType = ECCETypeAttendedMT;
         
        TInt err = KErrNone;
        TRAP( err, iTransferCall->AddObserverL( *this ) );
        if ( KErrNone == err )
            {
            iObservedCall = iTransferCall;
            // Answer new call when this gets connected original call will be terminated
            TInt err = iTransferCall->Answer(); 
            
            if ( KErrNone != err )
                {
                CCELOGSTRING2("CCCETransferController::TransferCallIncoming() Answering failed err=%d", err);
                ReleaseTransferCall();
                }
            }
        }
    else
        {
        CCELOGSTRING("CCCETransferController::TransferCallIncoming() originator not \
                found");
        ReleaseTransferCall();
        }
     
    CCELOGSTRING("CCCETransferController::TransferCallIncoming():OUT");
    }
    
// ---------------------------------------------------------------------------
// CCCETransferController::ReleaseTransferCall
// ---------------------------------------------------------------------------
//
 void CCCETransferController::ReleaseTransferCall()
    {
    CCELOGSTRING("CCCETransferController::ReleaseTransferCall()");
    if ( iTransferCall )
        {
        iCallContainer.ReleaseCall(*iTransferCall);
        iTransferCall = NULL;
        Reset();
        CCELOGSTRING("CCCETransferController: TransferCall queued \
         to be released");
        }
    }
    
// ---------------------------------------------------------------------------
// CCCETransferController::OngoingTransferType
// ---------------------------------------------------------------------------
// 
CCCETransferController::TTransferType CCCETransferController::OngoingTransferType() const
    {
    return iOngoingTransferType;    
    }
    
// ---------------------------------------------------------------------------
// CCCETransferController::TransferCall
// ---------------------------------------------------------------------------
// 
MCCPCall* CCCETransferController::TransferCall() const
    {
    return iTransferCall;
    }

// ---------------------------------------------------------------------------
// CCCETransferController::OriginatorCall
// ---------------------------------------------------------------------------
// 
MCCPCall* CCCETransferController::OriginatorCall() const
    {
    return iOriginatorCall;
    }
    
// ---------------------------------------------------------------------------
// CCCETransferController::SetTransferAccepted
// ---------------------------------------------------------------------------
//    
void CCCETransferController::SetTransferAccepted( TBool aAccepted )
    {
    iTransferAccepted = aAccepted;
    }

// ---------------------------------------------------------------------------
// CCCETransferController::TransferAccepted
// ---------------------------------------------------------------------------
//      
TBool CCCETransferController::TransferAccepted() const
    {
    return iTransferAccepted;
    }
    
// ---------------------------------------------------------------------------
// CCCETransferController::SetTransferDialPending
// ---------------------------------------------------------------------------
// 
void CCCETransferController::SetTransferDialPending( TBool aDialPending )
    {
    CCELOGSTRING3("CCCETransferController::SetTransferDialPending() \
    iDialPending=%d aDialPending=%d",iTransferDialPending, aDialPending);
    iTransferDialPending = aDialPending;
    }
    
// ---------------------------------------------------------------------------
// CCCETransferController::HandleCallStateChanged
// ---------------------------------------------------------------------------
// 
void CCCETransferController::HandleCallStateChanged( CCCECall* aCall,
        CCPCall::TCallState aStatus )
    {
    CCELOGSTRING2("CCCETransferController::HandleCallStatusChange():IN aStatus=%d",
            aStatus);
    
    switch ( aStatus )
        {         
        case CCPCall::EStateIdle:
        // flow through
        case CCPCall::EStateConnected:  
            {
            // When original or new call goes to idle/connected transfercontroller
            // can be resetted
            if( &aCall->GetCCPCall() == iOriginatorCall || 
                &aCall->GetCCPCall() == iTransferCall )
                {
                Reset();
                }
            }
            break;        
        case CCPCall::EStateDisconnecting:
            {
            // When original call goes to disconnecting state and there
            // is unattended transfer dial pending. Start dialing.
            if ( &aCall->GetCCPCall() == iOriginatorCall &&
                iTransferDialPending )
                {
                CCCECall* call =iCallContainer.GetCall( iTransferCall );
                if ( call )
                    {
                    iTransferDialPending = EFalse;
                    call->DoPendingRequest();
                    }
                }
            }
            break;  
        default:
            CCELOGSTRING2("CCCETransferController::HandleCallStatusChange: \
                    Unhandled state = %d", aStatus );
        }
    CCELOGSTRING("CCCETransferController::HandleCallStatusChange():OUT");
    }
// End of file
