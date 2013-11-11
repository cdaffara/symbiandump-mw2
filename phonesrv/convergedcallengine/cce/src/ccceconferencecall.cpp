/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Base class of conference call object
*
*/


//  INCLUDE FILES
#include "cconvergedcallprovider.h"
#include "ccceconferencecall.h"
#include "mccpconferencecall.h"
#include "mcceconferencecallobserver.h"
#include "cccepluginmanager.h"
#include "cccecallcontainer.h"
#include "cccecall.h"
#include "cccelogger.h"
#include "cccedurationtimer.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CCCEConferenceCall::CCCEConferenceCall(
        CCCECallContainer& aCallContainer,
        CCCEPluginManager& aPluginManager ):
        iPluginManager( aPluginManager ),
        iCallContainer( aCallContainer ),
        iCCEDurationTimer( NULL )
    {
    }

// ---------------------------------------------------------------------------
// Construct object
// ---------------------------------------------------------------------------
//
void CCCEConferenceCall::ConstructL()
    {
    iCallParameters = CCCECallParameters::NewL();
    iCCEDurationTimer = CCCEDurationTimer::NewL();
    Reset();
    }
    
// ---------------------------------------------------------------------------
// Default Symbian constructor
// ---------------------------------------------------------------------------
//
CCCEConferenceCall* CCCEConferenceCall::NewL(
        CCCECallContainer& aCallContainer,
        CCCEPluginManager& aPluginManager )
    {
    CCCEConferenceCall* self = new( ELeave ) CCCEConferenceCall(
        aCallContainer,
        aPluginManager );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCCEConferenceCall::~CCCEConferenceCall()
    {
    Reset();
    delete iCCEDurationTimer;
    delete iCallParameters;
    }

// ---------------------------------------------------------------------------
// SetObserver
// ---------------------------------------------------------------------------
//
void CCCEConferenceCall::SetObserver(
    const MCCEConferenceCallObserver& aObserver )
    {
    CCELOGSTRING("CCCEConferenceCall::SetObserver()");
    iObserver = const_cast<MCCEConferenceCallObserver*>( &aObserver );

    if (iCCEDurationTimer)
        {
        iCCEDurationTimer->SetConferenceObserver(iObserver);
        }
    }

// ---------------------------------------------------------------------------
// Release()
//
// Closes conference call
// ---------------------------------------------------------------------------
//
TInt CCCEConferenceCall::Release()
    {
    CCELOGSTRING("CCCEConferenceCall::Release()");

    // Reset timer observer
    iCCEDurationTimer->SetObserver( NULL );
    
    Reset();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Reset()
//
// Resets conference call
// ---------------------------------------------------------------------------
//
void CCCEConferenceCall::Reset()
    {
    CCELOGSTRING("CCCEConferenceCall::Reset()");
    iConferenceCaps = 1;
    
    iCallArray.Close();
    iState = MCCEConferenceCallObserver::ECCEConferenceIdle;
    
    if( iConferenceCall )
        {
        TRAP_IGNORE(iPluginManager.GetPluginL(
            iImplementationUid )->ReleaseConferenceCall( *iConferenceCall ));
        iConferenceCall = NULL;
        }
        
    iImplementationUid.iUid = 0; 
    
    iCallParameters->SetServiceId( 0 );
    
    if( iCCEDurationTimer )
        {
        iCCEDurationTimer->Reset();     
        }
    }

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------
//
MCCEConferenceCallObserver::TCCEConferenceCallState 
    CCCEConferenceCall::State() const
    {
    return iState;
    }

// ---------------------------------------------------------------------------
// ServiceId
// ---------------------------------------------------------------------------
//
TUint32 CCCEConferenceCall::ServiceId() const 
    {
    return iCallParameters->ServiceId();
    }

// ---------------------------------------------------------------------------
// ServiceId
// ---------------------------------------------------------------------------
//
TUid CCCEConferenceCall::ImplementationUid() const 
    {
    return iImplementationUid;
    }

// ---------------------------------------------------------------------------
// AddCallL( const TName& aCallName )
//
// Adds the single voice call to an existing conference call. 
// ---------------------------------------------------------------------------
//
void CCCEConferenceCall::AddCallL( MCCECall& aCall )
    {
    CCELOGSTRING("CCCEConferenceCall::AddCall()");
    CCCECall* call = static_cast<CCCECall*>( &aCall );
    
    if( !iConferenceCall )
        {
        iCallParameters->SetServiceId( call->ServiceId() );
        
        //This is needed for ccecallcontainer to check conf call type
        iCallParameters->SetCallType( call->CallType() );
        
        iImplementationUid = call->ImplementationUid();

        iConferenceCall = iPluginManager.GetPluginL(
            call->ImplementationUid() )->NewConferenceL( ServiceId(), *this );
            
        if(iConferenceCall)
            {
            iConferenceCall->AddCallL( &call->GetCCPCall() );
            }
        else
            {
            User::Leave( KErrBadHandle );
            }
            
        }
    else // Add call to current conferencecall if possible
        {
        iConferenceCall->AddCallL( &call->GetCCPCall() );
        }
        
    }

// ---------------------------------------------------------------------------
// RemoveCallL( const TName& aCallName )
//
// Removes the single voice call to an existing conference call. 
// ---------------------------------------------------------------------------
//
void CCCEConferenceCall::RemoveCallL( MCCECall& aCall )
    {
    CCELOGSTRING("CCCEConferenceCall::RemoveCallL()");
    CCCECall* call = static_cast<CCCECall*>( &aCall );
    
    if( iConferenceCall )
        {
        iConferenceCall->RemoveCallL( &call->GetCCPCall() );
        }
    else
        {
        User::Leave( KErrBadHandle );
        }
        
    }
// ---------------------------------------------------------------------------
// IsPartOfConference()
//
// Returns ETrue if aCall is part of conference 
// ---------------------------------------------------------------------------
//

TBool CCCEConferenceCall::IsPartOfConference( CCCECall& aCall )
    {
    CCELOGSTRING("CCCEConferenceCall::IsPartOfConference()");
    TBool ret = EFalse;
    TInt index = iCallArray.FindInAddressOrder( &aCall );
    
    if( index != KErrNotFound )
        {
        ret = ETrue;
        }
        
    return ret;
    }

// ---------------------------------------------------------------------------
// SetOngoingConferenceCall()
//
// Sets plugin created conference call
// ---------------------------------------------------------------------------
//
TInt CCCEConferenceCall::SetOngoingConferenceCall( 
    MCCPConferenceCall& aConferenceCall, 
    TUid aImplementationUid  )
    {
    CCELOGSTRING("CCCEConferenceCall::SetOngoingConferenceCall() IN");
    
    TInt err(KErrInUse);

    if( !iConferenceCall )
        {
        iConferenceCall = &aConferenceCall;
        TRAP_IGNORE( iConferenceCall->AddObserverL( *this ) );
        
        iConferenceCaps = 
            MCCEConferenceCallObserver::KCCECapsHangUp |
            MCCEConferenceCallObserver::KCCECapsSwap;
        
        iImplementationUid = aImplementationUid;

        err = KErrNone;
        
        RPointerArray<MCCPCall> array;
        iConferenceCall->GetCallArray( array );
        
        for( TInt i = 0; i < array.Count(); i++ )
            {
            // *call ownership is not transfered.
            const CCCECall* call = iCallContainer.GetCall( array[i] );
            
            // Check if cce call matching to ccp call is available
            iCallArray.Append( call ); // Ignore return value;
            iCallParameters->SetServiceId( call->ServiceId() );
                
            CCELOGSTRING("CCCEConferenceCall:: Call added to conferencecall");
            }

        TInt offset(0);
        for (TInt a = 0; a < iCallArray.Count(); a++)
            {
            TInt beats = iCallArray[a]->CallDuration().Int();
            if (beats > offset)   
                {
                offset = beats;   
                }
            } 
        iCCEDurationTimer->Start(offset);

        array.Close();
        }
    
    CCELOGSTRING("CCCEConferenceCall::SetOngoingConferenceCall() OUT");
    return err;
    }

// ---------------------------------------------------------------------------
// EnumerateCalls()
//
// Returns the member count of the conference call. 
// ---------------------------------------------------------------------------
//
TInt CCCEConferenceCall::EnumerateCalls()
    {
    CCELOGSTRING("CCCEConferenceCall::EnumerateCalls()");
    return iCallArray.Count();
    }

// ---------------------------------------------------------------------------
// GetCaps()
//
// Gets the current capabilities of the conference call.
// ---------------------------------------------------------------------------
//
MCCEConferenceCallObserver::TCCEConferenceCallCaps CCCEConferenceCall::Caps() const
    {
    CCELOGSTRING("CCCEConferenceCall::GetCaps()");
    return (MCCEConferenceCallObserver::TCCEConferenceCallCaps)iConferenceCaps;
    }

// ---------------------------------------------------------------------------
// Parameters() const
// ---------------------------------------------------------------------------
//
const CCCPCallParameters& CCCEConferenceCall::Parameters() const
    {
    return *iCallParameters;
    }

// ---------------------------------------------------------------------------
// HangUp()
//
// Terminates the conference.
// ---------------------------------------------------------------------------
//
TInt CCCEConferenceCall::HangUp()
    {
    CCELOGSTRING("CCCEConferenceCall::HangUp()");
    iCCEDurationTimer->Stop();
    return iConferenceCall->HangUp();
    }

// ---------------------------------------------------------------------------
// Swap()
//
// Allows a client to switch a conference call between "Active" 
// and "Hold" states.
// ---------------------------------------------------------------------------
//
TInt CCCEConferenceCall::Swap()
    {
    CCELOGSTRING("CCCEConferenceCall::Swap()");
    return iConferenceCall->Swap();
    }

// ---------------------------------------------------------------------------
// GoOneToOneL()
// ---------------------------------------------------------------------------
//
void CCCEConferenceCall::GoOneToOneL( MCCECall& aCall )
    {
    CCELOGSTRING("CCCEConferenceCall::GoOneToOne()");
    CCCECall* call = static_cast<CCCECall*>( &aCall );
    iConferenceCall->GoOneToOneL( call->GetCCPCall() );
    }

// ---------------------------------------------------------------------------
// GetCallArray()
// ---------------------------------------------------------------------------
//
TInt CCCEConferenceCall::GetCallArray( RPointerArray<MCCECall>& aCallArray )
    {
    TInt err( KErrNotFound );
    TInt count = iCallArray.Count();
    aCallArray.Reserve(count);
    for (TInt a = 0; a < count; a++)
        {
        err= aCallArray.Append( iCallArray[a] );
        }
    return err;
    }

// ---------------------------------------------------------------------------
// ConferenceCallEventOccurred()
// ---------------------------------------------------------------------------
//
void CCCEConferenceCall::ErrorOccurred( const TCCPConferenceCallError aError )
    {
    CCELOGSTRING("CCCEConferenceCall::ErrorOccurred()");
    
    iCallContainer.ErrorOccurred( NULL, ECCPErrorGeneral );
        
    iObserver->ErrorOccurred( aError );
    }

// ---------------------------------------------------------------------------
// ConferenceCallCapsChanged()
// ---------------------------------------------------------------------------
//
void CCCEConferenceCall::ConferenceCallCapsChanged( 
    const TCCPConferenceCallCaps aCaps )
    {
    CCELOGSTRING2(
        "CCCEConferenceCall::ConferenceCallStateChanged() Caps = %d", aCaps );
        
    CapsChanged( aCaps );
    }
    
// ---------------------------------------------------------------------------
// ConferenceCallEventOccurred()
// ---------------------------------------------------------------------------
//
void CCCEConferenceCall::ConferenceCallStateChanged( 
    const TCCPConferenceCallState aState )
    {
    CCELOGSTRING2(
        "CCCEConferenceCall::ConferenceCallStateChanged() State = %d", aState );
    TInt offset(0);
    switch( aState )
        {
        /** The conference call is in the idle state. */
        case ECCPConferenceIdle:
            CCELOGSTRING(
                "CCCEConferenceCall::ConferenceCallStateChanged() ECCPConferenceIdle");
            iState = MCCEConferenceCallObserver::ECCEConferenceIdle;
            DeActivate();
            iObserver->CallStateChanged( 
                MCCEConferenceCallObserver::ECCEConferenceIdle );
              
            iCallContainer.HandleConferenceStatusChange( 
                MCCEConferenceCallObserver::ECCEConferenceIdle );

            // Stop duration timer
            iCCEDurationTimer->Stop();
          
            break;
        /** The conference call is in the active, connected state. */
        case ECCPConferenceActive:
            CCELOGSTRING(
                "CCCEConferenceCall::ConferenceCallStateChanged() ECCPConferenceActive");
            iState = MCCEConferenceCallObserver::ECCEConferenceActive;
            iObserver->CallStateChanged( 
                MCCEConferenceCallObserver::ECCEConferenceActive );
            iCallContainer.HandleConferenceStatusChange( 
                MCCEConferenceCallObserver::ECCEConferenceActive );

            // Start call duration timer
            
            for (TInt a = 0; a < iCallArray.Count(); a++)
                {
                 TInt beats = iCallArray[a]->CallDuration().Int();
                 if (beats > offset)   
                    {
                     offset = beats;   
                    }
                } 
            iCCEDurationTimer->Start(offset);

            break;
        /** The conference call is in the held, connected state. */
        case ECCPConferenceHold:
            CCELOGSTRING(
                "CCCEConferenceCall::ConferenceCallStateChanged() ECCPConferenceHold");
            iState = MCCEConferenceCallObserver::ECCEConferenceHold;
            iObserver->CallStateChanged( 
                MCCEConferenceCallObserver::ECCEConferenceHold );
            iCallContainer.HandleConferenceStatusChange( 
                MCCEConferenceCallObserver::ECCEConferenceHold );
            break;
        default:
            break;
        }
        
    }

// ---------------------------------------------------------------------------
// ConferenceCallEventOccurred()
// ---------------------------------------------------------------------------
//
void CCCEConferenceCall::ConferenceCallEventOccurred( 
     const TCCPConferenceCallEvent aEvent,
     MCCPCall* aReferredCall )
    {
    CCELOGSTRING2(
        "CCCEConferenceCall::ConferenceCallEventOccurred() aEvent = %d", aEvent );
    CCCECall* call = NULL;
    
    if( aReferredCall )
        {
        call = iCallContainer.GetCall( aReferredCall );
        }

    switch( aEvent )
        {
        /** Conference call added. TODO, do we need call pointer with this 
            notification? and a existing call pointer list of the conference?*/
        case ECCPConferenceCallAdded:
        
            if( call )
                {
                CCELOGSTRING(
                    "CCCEConferenceCall::ConferenceCallEventOccurred():CallAdded");
                iCallArray.InsertInAddressOrder( call );
                iObserver->CallEventOccurred( 
                    MCCEConferenceCallObserver::ECCEConferenceCallAdded, call);
                
                iCallParameters->SetServiceId( call->ServiceId() );
                iImplementationUid = call->ImplementationUid();
                }
                
            break;
        /** Conference call removed */
        case ECCPConferenceCallRemoved:
        
            if( call )
                {
                TInt index = iCallArray.FindInAddressOrder( call );
                
                if( index != KErrNotFound )
                    {
                    CCELOGSTRING(
                        "CCCEConferenceCall::ConferenceCallEventOccurred():CallRemoved");

                    iCallArray.Remove( index );
                    iCallArray.Compress();

                    iObserver->CallEventOccurred( 
                        MCCEConferenceCallObserver::ECCEConferenceCallRemoved, call);
                    iObserver->ErrorOccurred((TCCPConferenceCallError)KErrNone);
                         
                    }
                else
                    {
                    CCELOGSTRING(
                        "CCCEConferenceCall::ConferenceCallEventOccurred(): Call not found");
                    }
                    
                }
                
            break;
        /** Names of the two calls used to build the conference call 
            returned in aCallName. */
        case ECCPConferenceBuilt:
            CCELOGSTRING(
                "CCCEConferenceCall::ConferenceCallEventOccurred():ECCPConferenceBuilt");
            iObserver->CallEventOccurred( 
                MCCEConferenceCallObserver::ECCEConferenceBuilt, call); 
            break;
        /** No name provided */
        case ECCPConferenceTerminated:
            CCELOGSTRING(
                "CCCEConferenceCall::ConferenceCallEventOccurred():ECCPConferenceTerminated");
                   
            iObserver->CallEventOccurred( 
                MCCEConferenceCallObserver::ECCEConferenceTerminated, call);
            break;
        /** No name provided. */
        case ECCPConferenceSwapped:
            CCELOGSTRING(
                "CCCEConferenceCall::ConferenceCallEventOccurred():ECCPConferenceSwapped");
            iObserver->CallEventOccurred( 
                MCCEConferenceCallObserver::ECCEConferenceSwapped, call); 
            break;
        /** Name of the call being seperated. */
        case ECCPConferenceSplit:
            CCELOGSTRING(
                "CCCEConferenceCall::ConferenceCallEventOccurred():ECCPConferenceSplit");
            iObserver->CallEventOccurred( 
                MCCEConferenceCallObserver::ECCEConferenceSplit, call); 
            break;
        default:
            break;

        }
    }

// ---------------------------------------------------------------------------
// CapsChanged()
// ---------------------------------------------------------------------------
//
void CCCEConferenceCall::CapsChanged( TInt aCaps )
    {
    if( aCaps != iConferenceCaps )
        {
        CCELOGSTRING2("CCCEConferenceCall::CapsChanged() aCaps = %d", aCaps);
        iConferenceCaps = aCaps;

        iObserver->CallCapsChanged(
            (MCCEConferenceCallObserver::TCCEConferenceCallCaps)iConferenceCaps );
        }
    }


// ---------------------------------------------------------------------------
// DeActivate()
// ---------------------------------------------------------------------------
// 
void CCCEConferenceCall::DeActivate()
    {
    CCELOGSTRING("CCCEConferenceCall::DeActivate():Conference idle");
        
    iCallArray.Reset();
    
    const TInt caps = MCCEConferenceCallObserver::KCCECapsCreate; // Create
    CapsChanged( caps );
    }

// ---------------------------------------------------------------------------
// CallDuration()
// ---------------------------------------------------------------------------
// 
TTimeIntervalSeconds CCCEConferenceCall::CallDuration() const
    {
    return iCCEDurationTimer->NumberOfBeats();
    }
    
// End of file
