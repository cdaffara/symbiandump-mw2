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
* Description:  Asynchronises cce call observer calls 
*
*/


//  INCLUDE FILES

#include "cccecallobserverasynchroniser.h"
#include "cccelogger.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// NewL()
// Two-phased constructor
// -----------------------------------------------------------------------------

CCCECallObserverAsynchroniser* CCCECallObserverAsynchroniser::NewL()
    {
    CCCECallObserverAsynchroniser* self = CCCECallObserverAsynchroniser::NewLC();
    CleanupStack::Pop( self );
    return self; 
    }

// -----------------------------------------------------------------------------
// NewLC()
// Two-phased constructor
// Leaves pointer in cleanup stack
// -----------------------------------------------------------------------------

CCCECallObserverAsynchroniser* CCCECallObserverAsynchroniser::NewLC()
    {
    CCCECallObserverAsynchroniser* self = new( ELeave ) CCCECallObserverAsynchroniser();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;    
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
CCCECallObserverAsynchroniser::~CCCECallObserverAsynchroniser()
    {
    Cancel();
    iEvents.Close();  
    }

// -----------------------------------------------------------------------------
// Set call observer
// -----------------------------------------------------------------------------
void CCCECallObserverAsynchroniser::SetMCCECallObserver( MCCECallObserver* aObserver )
    {
    CCELOGSTRING( "CCCECallObserverAsynchroniser::SetMCCECallObserver()" );
    iCCECallObserver = aObserver;
    if ( iIsRuning && aObserver )
        {
        if( !IsActive() )
            {
            SetActive();
            TRequestStatus* status = &iStatus;
            User::RequestComplete( status, KErrNone );
            }
        }
     else
        {
        Cancel();
        iEvents.Reset();
        }
    }

// -----------------------------------------------------------------------------
// get call observer
// -----------------------------------------------------------------------------    
MCCECallObserver* CCCECallObserverAsynchroniser::GetMCCECallObserver()
    {
    return iCCECallObserver;
    }

// -----------------------------------------------------------------------------
// CActive RunL
// -----------------------------------------------------------------------------
void CCCECallObserverAsynchroniser::RunL()
    {
    CCELOGSTRING2( "CCCECallObserverAsynchroniser::RunL() events: %d", iEvents.Count() );
 
    if ( !iCCECallObserver )
        {
        CCELOGSTRING( "CCCECallObserverAsynchroniser::RunL() No observer!");
        return;
        }
        
    while (iEvents.Count() > 0)
        {
        switch ( iEvents[0].iEventType )
            {
            case EErrorOccurred:
                {
                CCELOGSTRING( "CCCECallObserverAsynchroniser::RunL() Event: EErrorOccurred");
                iCCECallObserver->ErrorOccurred(
                    (TCCPError)iEvents[0].iInt1 );
                break;    
                }           
            
            case ECallStateChanged:
                {
                CCELOGSTRING( "CCCECallObserverAsynchroniser::RunL() Event: ECallStateChanged");
                iCCECallObserver->CallStateChanged(
                    (CCPCall::TCallState)iEvents[0].iInt1 );
                break;    
                }
                
            case ECallStateChangedWithInband:
                {
                CCELOGSTRING( "CCCECallObserverAsynchroniser::RunL() Event: ECallStateChangedWithInband");
                iCCECallObserver->CallStateChangedWithInband(
                    (CCPCall::TCallState)iEvents[0].iInt1 );
                break;    
                }
                
            case ECallEventOccurred:
                {
                CCELOGSTRING( "CCCECallObserverAsynchroniser::RunL() Event: ECallEventOccurredanged");
                iCCECallObserver->CallEventOccurred(
                    (MCCECallObserver::TCCECallEvent)iEvents[0].iInt1 );
                break;    
                }
                
            case ECallDurationChanged:
                {
                CCELOGSTRING( "CCCECallObserverAsynchroniser::RunL() Event: ECallDurationChanged");
                iCCECallObserver->CallDurationChanged(iEvents[0].iDuration );
                break;    
                }
                
            case ECallCapsChanged:
                {
                CCELOGSTRING( "CCCECallObserverAsynchroniser::RunL() Event: ECallCapsChanged");
                iCCECallObserver->CallCapsChanged(
                    (MCCECallObserver::TCCECallControlCaps)iEvents[0].iInt1 );
                break;    
                }
                
            case EHandleTransfer:
                {
                CCELOGSTRING( "CCCECallObserverAsynchroniser::RunL() Event: EHandleTransfer");
                iCCECallObserver->HandleTransfer(iEvents[0].iBoolean, *iEvents[0].iDescriptor );
                break;    
                }
            }
            
        if( iEvents.Count() )
            {
            iEvents.Remove( 0 );
            iEvents.Compress();
            }
        }
    
    iIsRuning = EFalse;
    }

// -----------------------------------------------------------------------------
// CActive RunError
// -----------------------------------------------------------------------------
TInt CCCECallObserverAsynchroniser::RunError(TInt aError)
    {
    return aError;
    }

// -----------------------------------------------------------------------------
// CActive DoCancel
// -----------------------------------------------------------------------------
void CCCECallObserverAsynchroniser::DoCancel()   
    {
    iEvents.Reset();
    iIsRuning = EFalse;
    }

// -----------------------------------------------------------------------------
// MCCECallObserver ErrorOccurred
// -----------------------------------------------------------------------------
void CCCECallObserverAsynchroniser::ErrorOccurred( const TCCPError aError )
    {
    CCELOGSTRING( "CCCECallObserverAsynchroniser::ErrorOccurred");
    TEventStorage event;
    event.iEventType = EErrorOccurred;
    event.iInt1 = aError;
    AddEvent(event);
        
    }

// -----------------------------------------------------------------------------
// MCCECallObserver CallStateChanged
// -----------------------------------------------------------------------------
void CCCECallObserverAsynchroniser::CallStateChanged(
    const CCPCall::TCallState aState )
    {
    CCELOGSTRING( "CCCECallObserverAsynchroniser::CallStateChanged");
    TEventStorage event;
    event.iEventType = ECallStateChanged;
    event.iInt1 = aState;
    AddEvent(event);
        
    }

// -----------------------------------------------------------------------------
// MCCECallObserver CallStateChangedWithInband
// -----------------------------------------------------------------------------
void CCCECallObserverAsynchroniser::CallStateChangedWithInband(
    const CCPCall::TCallState aState )
    {
    CCELOGSTRING( "CCCECallObserverAsynchroniser::CallStateChangedWithInband");
    TEventStorage event;
    event.iEventType = ECallStateChangedWithInband;
    event.iInt1 = aState;
    AddEvent(event);
        
    }

// -----------------------------------------------------------------------------
// MCCECallObserver CallEventOccurred
// -----------------------------------------------------------------------------
void CCCECallObserverAsynchroniser::CallEventOccurred( 
    const MCCECallObserver::TCCECallEvent aEvent )
    {
    CCELOGSTRING( "CCCECallObserverAsynchroniser::CallEventOccurred");
    TEventStorage event;
    event.iEventType = ECallEventOccurred;
    event.iInt1 = aEvent;
    AddEvent(event);
        
    }

// -----------------------------------------------------------------------------
// MCCECallObserver CallDurationChanged
// -----------------------------------------------------------------------------
void CCCECallObserverAsynchroniser::CallDurationChanged( 
    const TTimeIntervalSeconds aDuration )
    {
    CCELOGSTRING( "CCCECallObserverAsynchroniser::CallDurationChanged");
    TEventStorage event;
    event.iEventType = ECallDurationChanged;
    event.iDuration = aDuration;
    AddEvent(event);
    }

// -----------------------------------------------------------------------------
// MCCECallObserver CallCapsChanged
// -----------------------------------------------------------------------------
void CCCECallObserverAsynchroniser::CallCapsChanged( 
    const MCCECallObserver::TCCECallControlCaps aCaps )
    {
    CCELOGSTRING( "CCCECallObserverAsynchroniser::CallCapsChanged");
    TEventStorage event;
    event.iEventType = ECallCapsChanged;
    event.iInt1 = aCaps;
    AddEvent(event);
    }

// -----------------------------------------------------------------------------
// MCCECallObserver HandleTransfer
// -----------------------------------------------------------------------------
void CCCECallObserverAsynchroniser::HandleTransfer( const TBool aAttended, 
                             const TDesC& aDestination )
    {
    CCELOGSTRING( "CCCECallObserverAsynchroniser::HandleTransfer");
    TEventStorage event;
    event.iEventType = EHandleTransfer;
    event.iBoolean = aAttended;
    event.iDescriptor = &aDestination;
    AddEvent(event);
    }

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
CCCECallObserverAsynchroniser::CCCECallObserverAsynchroniser() 
    :CActive( EPriorityStandard ), iIsRuning( EFalse )
    { 
    }


// -----------------------------------------------------------------------------
// Leaving stuff in constructor
// -----------------------------------------------------------------------------
void CCCECallObserverAsynchroniser::ConstructL()
    {
    CActiveScheduler::Add( this );
    }
    
// -----------------------------------------------------------------------------
// Adding event to queue
// -----------------------------------------------------------------------------
void CCCECallObserverAsynchroniser::AddEvent( TEventStorage aEvent )
    {
    iEvents.Append(aEvent);
    CCELOGSTRING( "CCCECallObserverAsynchroniser::AddEvent");
    if (!iIsRuning)
        {
        CCELOGSTRING( "CCCECallObserverAsynchroniser::AddEvent Activating...");
        iIsRuning = ETrue;
        if (iCCECallObserver)
            {
            SetActive();
            TRequestStatus* status = &iStatus;
            User::RequestComplete(status, KErrNone);    
            }
        }
    }
// end of file
