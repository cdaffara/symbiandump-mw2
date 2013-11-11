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
* Description:  Asynchronises cce observer calls 
*
*/



#include "ccceobserverasynchroniser.h"
#include "cccelogger.h"
 
// -----------------------------------------------------------------------------
// NewL()
// Two-phased constructor
// 
CCCEObserverAsynchroniser* CCCEObserverAsynchroniser::NewL()
    {
    CCCEObserverAsynchroniser* self = CCCEObserverAsynchroniser::NewLC();
    CleanupStack::Pop( self );
    return self;    
    }
   
// -----------------------------------------------------------------------------
// NewLC()
// Two-phased constructor
// Leaves pointer in cleanup stack
// -----------------------------------------------------------------------------
CCCEObserverAsynchroniser* CCCEObserverAsynchroniser::NewLC()
    {
    CCCEObserverAsynchroniser* self = new( ELeave ) CCCEObserverAsynchroniser();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;    
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
CCCEObserverAsynchroniser::~CCCEObserverAsynchroniser()
    {
    Cancel();
    iEvents.Close();    
    }
    
// -----------------------------------------------------------------------------
// Set provider observer
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::SetMCCEObserver( MCCEObserver* aObserver )
    {
    iCCEObserver = aObserver;
    }
    
// -----------------------------------------------------------------------------
// Set SS observer
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::SetMCCESsObserver(MCCESsObserver* aObserver)
    {
    iCCESsObserver = aObserver;
    }

// -----------------------------------------------------------------------------
// Set Conference call observer
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::SetMCCEConferenceCallObserver(
    const MCCEConferenceCallObserver* aObserver)
    {
    iCCEConferenceCallObserver = const_cast<MCCEConferenceCallObserver*>( aObserver );  
    }

// -----------------------------------------------------------------------------
// Set DTMF observer
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::SetMCCEDtmfObserver( const MCCEDtmfObserver* aObserver )
    {
    iCCEDtmfObserver = const_cast<MCCEDtmfObserver*>( aObserver );   
    }

// -----------------------------------------------------------------------------
// Set Extension observer
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::SetMCCEExtensionObserver(
    const MCCEExtensionObserver* aObserver)
    {
    iCCEExtensionObserver = const_cast<MCCEExtensionObserver*>( aObserver );   
    }

// -----------------------------------------------------------------------------
// Get provider observer
// -----------------------------------------------------------------------------    
MCCEObserver& CCCEObserverAsynchroniser::GetMCCEObserver()
    {
    return *this;    
    }

// -----------------------------------------------------------------------------
// Get SS observer
// -----------------------------------------------------------------------------
MCCESsObserver& CCCEObserverAsynchroniser::GetMCCESsObserver()
    {
    return *this;   
    }

// -----------------------------------------------------------------------------
// Get Conference call observer
// -----------------------------------------------------------------------------
MCCEConferenceCallObserver& CCCEObserverAsynchroniser::GetMCCEConferenceCallObserver()
    {
    return *this;   
    }

// -----------------------------------------------------------------------------
// Get DTMF observer
// -----------------------------------------------------------------------------
MCCEDtmfObserver& CCCEObserverAsynchroniser::GetMCCEDtmfObserver()
    {
    return *this;  
    }
    
// -----------------------------------------------------------------------------
// Get Extension observer
// -----------------------------------------------------------------------------
MCCEExtensionObserver& CCCEObserverAsynchroniser::GetMCCEExtensionObserver()
    {
    return *this;   
    }
    
// -----------------------------------------------------------------------------
// CActive RunL
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::RunL()
    {
    CCELOGSTRING2( "CCCEObserverAsynchroniser::RunL() events: %d", iEvents.Count() );
    while (iEvents.Count() > 0)
        {
        switch (iEvents[0].iEventType)
            {
            
            case EIncomingCall:
                {
                CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: EIncomingCall");
                if (iCCEObserver)
                    {
                    iCCEObserver->IncomingCall(*iEvents[0].iCall);
                    }
                break;    
                }           
            
            case EMOCallCreated:
                {
                CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: EPluginMOCallCreated");
                if (iCCEObserver)
                    {
                    iCCEObserver->MOCallCreated(*iEvents[0].iCall);
                    }
                break;    
                }
                
            case EDataPortName:
                {
                CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: EDataPortName");
                if (iCCEObserver)
                    {
                    iCCEObserver->DataPortName( iEvents[0].iTName );
                    }
                break;    
                }

            case EConferenceCallCreated:
                {
                CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: EConferenceCallCreated");
                if( iCCEObserver )
                    {
                    iCCEObserver->ConferenceCallCreated( *iEvents[0].iConferenceCall );
                    }
                break;    
                }                
            case EBarringEventOccurred:
                {
                CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: EBarringEventOccurred");
                if (iCCESsObserver)
                    {
                    iCCESsObserver->BarringEventOccurred(
                        (MCCESsObserver::TCCESsBarringEvent)iEvents[0].iInt1);
                    }
                break;    
                }
                
            case ECLIEventOccurred:
                {
                CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: ECLIEventOccurred");
                if (iCCESsObserver)
                    {
                    iCCESsObserver->CLIEventOccurred(
                        (MCCESsObserver::TCCESsCLIEvent)iEvents[0].iInt1);
                    }
                break;    
                }
                
            case ECallForwardEventOccurred:
                {
                CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: ECallForwardEventOccurred");
                if (iCCESsObserver)
                    {
                    if( iEvents[0].iDescriptor )
                        {
                        iCCESsObserver->CallForwardEventOccurred(
                            (MCCESsObserver::TCCESsCallForwardEvent)iEvents[0].iInt1, *iEvents[0].iDescriptor );
                        }
                    else
                        {
                        CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: ECallForwardEventOccurred, descr is NULL!");
                        }
                    }
                break;    
                }
                
            case ECallCugEventOccurred:
                {
                CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: ECallCugEventOccurred");
                if (iCCESsObserver)
                    {
                    iCCESsObserver->CallCugEventOccurred(
                        (MCCESsObserver::TCCESsCugEvent)iEvents[0].iInt1);
                    }
                break;    
                }
                
            case ENotifyCurrentActiveALSLine:
                {
                CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: ENotifyCurrentActiveALSLine");
                if (iCCESsObserver)
                    {
                    iCCESsObserver->NotifyCurrentActiveALSLine(iEvents[0].iInt1);
                    }
                break;    
                }
                
            case ECallEventOccurred:
                {
                CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: ECallEventOccurred");        
                if (iCCEConferenceCallObserver)
                    {
                    iCCEConferenceCallObserver->CallEventOccurred(
                        (MCCEConferenceCallObserver::TCCEConferenceCallEvent)iEvents[0].iInt1, iEvents[0].iCall);
                    }
                break;    
                }
                
            case ECallStateChanged:
                {
                CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: ECallStateChanged");    
                if (iCCEConferenceCallObserver)
                    {
                    iCCEConferenceCallObserver->CallStateChanged(
                        (MCCEConferenceCallObserver::TCCEConferenceCallState)iEvents[0].iInt1);
                    }
                break;    
                }
                
            case ECallCapsChanged:
                {
                CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: ECallCapsChanged");    
                if (iCCEConferenceCallObserver)
                    {
                    iCCEConferenceCallObserver->CallCapsChanged(
                        (MCCEConferenceCallObserver::TCCEConferenceCallCaps)iEvents[0].iInt1);
                    }
                break;    
                }
                
            case EErrorOccurred:
                {
                CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: EErrorOccurred");    
                if (iCCEConferenceCallObserver)
                    {
                    iCCEConferenceCallObserver->ErrorOccurred(
                        (TCCPConferenceCallError)iEvents[0].iInt1);
                    }
                break;    
                }
                
            case ECallDurationChanged:
                {
                CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: ECallDurationChanged");    
                if (iCCEConferenceCallObserver)
                    {
                    iCCEConferenceCallObserver->CallDurationChanged(iEvents[0].iDuration);
                    }
                break;    
                }
                
            case EHandleDTMFEvent:
                {
                CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: EHandleDTMFEvent");    
                if (iCCEDtmfObserver)
                    {
                    iCCEDtmfObserver->HandleDTMFEvent((MCCEDtmfObserver::TCCEDtmfEvent)iEvents[0].iInt1,
                        iEvents[0].iInt2, iEvents[0].iChar);
                    }
                break;    
                }
                
            case EHandleExtensionEvents:
                {
                CCELOGSTRING( "CCCEObserverAsynchroniser::RunL() Event: EHandleExtensionEvents");    
                if (iCCEExtensionObserver)
                    {
                    iCCEExtensionObserver->HandleExtensionEvents(iEvents[0].iServiceId,
                        iEvents[0].iInt1, iEvents[0].iInt2);
                    }
                break;    
                }
            }
            
            iEvents.Remove(0);
            iEvents.Compress();
        }
       iIsRuning = EFalse;   
    }

// -----------------------------------------------------------------------------
// CActive RunError
// -----------------------------------------------------------------------------
TInt CCCEObserverAsynchroniser::RunError( TInt aError )
    {
    return aError;
    }

// -----------------------------------------------------------------------------
// CActive DoCancel
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::DoCancel()   
    {
    iIsRuning = EFalse;   
    iEvents.Reset();
    }
// -----------------------------------------------------------------------------
// MCCEObserver IncomingCall
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::IncomingCall( MCCECall& aCall )
    {
    CCELOGSTRING( "CCCEObserverAsynchroniser::IncomingCall");
    TEventStorage event;
    event.iEventType = EIncomingCall;
    event.iCall = &aCall;
    AddEvent(event);
    }

// -----------------------------------------------------------------------------
// MCCEObserver MOCallCreated
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::MOCallCreated( MCCECall& aCall )
    {
    CCELOGSTRING( "CCCEObserverAsynchroniser::MOCallCreated");
    TEventStorage event;
    event.iEventType = EMOCallCreated;
    event.iCall = &aCall;
    AddEvent(event);   
    }

// -----------------------------------------------------------------------------
// MCCEObserver DataPortName
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::DataPortName( TName& aPortName )
    {
    CCELOGSTRING( "CCCEObserverAsynchroniser::DataPortName");
    TEventStorage event;
    event.iEventType = EDataPortName;
    event.iTName = aPortName;
    AddEvent(event);   
    }

// -----------------------------------------------------------------------------
// MCCEObserver ConferenceCallCreated
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::ConferenceCallCreated( MCCEConferenceCall& aConferenceCall )
    {
    CCELOGSTRING( "CCCEObserverAsynchroniser::ConferenceCallCreated");
    TEventStorage event;
    event.iEventType = EConferenceCallCreated;    
    event.iConferenceCall = &aConferenceCall;
    AddEvent(event);   
    }

// -----------------------------------------------------------------------------
// MCCESsObserver BarringEventOccurred
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::BarringEventOccurred( const MCCESsObserver::TCCESsBarringEvent aBarringEvent )
    {
    CCELOGSTRING( "CCCEObserverAsynchroniser::BarringEventOccurred");
    TEventStorage event;
    event.iEventType = EBarringEventOccurred;
    event.iInt1 = aBarringEvent;
    AddEvent(event);   
    }
                              
// -----------------------------------------------------------------------------
// MCCESsObserver CLIEventOccurred
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::CLIEventOccurred( const MCCESsObserver::TCCESsCLIEvent aCallLineEvent )
    {
    CCELOGSTRING( "CCCEObserverAsynchroniser::CLIEventOccurred");
    TEventStorage event;
    event.iEventType = ECLIEventOccurred;
    event.iInt1 = aCallLineEvent;
    AddEvent(event); 
        
    }
                              
// -----------------------------------------------------------------------------
// MCCESsObserver CallForwardEventOccurred
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::CallForwardEventOccurred( 
                    const MCCESsObserver::TCCESsCallForwardEvent aCallForwardEvent,
                    const TDesC& aRemoteAddress )
    {
    CCELOGSTRING( "CCCEObserverAsynchroniser::CallForwardEventOccurred");
    TEventStorage event;
    event.iEventType = ECallForwardEventOccurred;
    event.iInt1 = aCallForwardEvent;
    // In theory this could be invalidated before async forward
    // also in more remote theoretical case overwritten by plugin with new value.
    event.iDescriptor = &aRemoteAddress;
    AddEvent(event);
        
    }

// -----------------------------------------------------------------------------
// MCCESsObserver CallCugEventOccurred
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::CallCugEventOccurred( const MCCESsObserver::TCCESsCugEvent aCugEvent )
    {
    CCELOGSTRING( "CCCEObserverAsynchroniser::CallCugEventOccurred");
    TEventStorage event;
    event.iEventType = ECallCugEventOccurred;
    event.iInt1 = aCugEvent;
    AddEvent(event);
        
    }

// -----------------------------------------------------------------------------
// MCCESsObserver NotifyCurrentActiveALSLine
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::NotifyCurrentActiveALSLine( TInt aLine )
    {
    CCELOGSTRING( "CCCEObserverAsynchroniser::NotifyCurrentActiveALSLine");
    TEventStorage event;
    event.iEventType = ENotifyCurrentActiveALSLine;
    event.iInt1 = aLine;
    AddEvent(event);
    }

// -----------------------------------------------------------------------------
// MCCESsObserver NotifyCurrentActiveALSLine
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::CallEventOccurred( 
             const MCCEConferenceCallObserver::TCCEConferenceCallEvent aEvent,
             MCCECall* aReferredCall )
    {
    CCELOGSTRING( "CCCEObserverAsynchroniser::CallEventOccurred");
    TEventStorage event;
    event.iEventType = ECallEventOccurred;
    event.iInt1 = aEvent;
    event.iCall = aReferredCall;
    AddEvent(event);       
    }
    
// -----------------------------------------------------------------------------
// MCCEConferenceCallObserver CallStateChanged
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::CallStateChanged( 
             const MCCEConferenceCallObserver::TCCEConferenceCallState aState )
    {
    CCELOGSTRING( "CCCEObserverAsynchroniser::CallStateChanged");
    TEventStorage event;
    event.iEventType = ECallStateChanged;
    event.iInt1 = aState;
    AddEvent(event);
    }

// -----------------------------------------------------------------------------
// MCCEConferenceCallObserver CallCapsChanged
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::CallCapsChanged( 
             const MCCEConferenceCallObserver::TCCEConferenceCallCaps aCaps )
    {
    CCELOGSTRING( "CCCEObserverAsynchroniser::CallCapsChanged");
    TEventStorage event;
    event.iEventType = ECallCapsChanged;
    event.iInt1 = aCaps;
    AddEvent(event);
    }
 
// -----------------------------------------------------------------------------
// MCCEConferenceCallObserver ErrorOccurred
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::ErrorOccurred( TCCPConferenceCallError aError  )
    {
    CCELOGSTRING( "CCCEObserverAsynchroniser::ErrorOccurred");
    TEventStorage event;
    event.iEventType = EErrorOccurred;
    event.iInt1 = aError;
    AddEvent(event);
    }

// -----------------------------------------------------------------------------
// MCCEConferenceCallObserver CallDurationChanged
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::CallDurationChanged( const TTimeIntervalSeconds aDuration )
    {
    CCELOGSTRING( "CCCEObserverAsynchroniser::CallDurationChanged");
    TEventStorage event;
    event.iEventType = ECallDurationChanged;
    event.iDuration = aDuration;
    AddEvent(event);
    }

// -----------------------------------------------------------------------------
// MCCEDtmfObserver HandleDTMFEvent
// -----------------------------------------------------------------------------    
void CCCEObserverAsynchroniser::HandleDTMFEvent( const MCCEDtmfObserver::TCCEDtmfEvent aEvent, 
                              const TInt aError, 
                              const TChar aTone )
    {
    CCELOGSTRING( "CCCEObserverAsynchroniser::HandleDTMFEvent");
    TEventStorage event;
    event.iEventType = EHandleDTMFEvent;
    event.iInt1 = aEvent;
    event.iInt2 = aError;
    event.iChar = aTone;
    AddEvent(event);
    }
    
// -----------------------------------------------------------------------------
// MCCEExtensionObserver HandleExtensionEvents
// -----------------------------------------------------------------------------                                
void CCCEObserverAsynchroniser::HandleExtensionEvents( TUint32 aServiceId,
    TInt aEvent,
    TInt aStatus )
    {
    CCELOGSTRING( "CCCEObserverAsynchroniser::HandleExtensionEvents");
    TEventStorage event;
    event.iEventType = EHandleExtensionEvents;
    event.iServiceId = aServiceId;
    event.iInt1 = aEvent;
    event.iInt2 = aStatus;
    AddEvent(event);        
    }
    
// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------     
CCCEObserverAsynchroniser::CCCEObserverAsynchroniser():CActive(EPriorityStandard)
    {      
    }

// -----------------------------------------------------------------------------
// Leaving stuff in constructor
// -----------------------------------------------------------------------------
void CCCEObserverAsynchroniser::ConstructL()
    {
    CActiveScheduler::Add(this);   
    }

// -----------------------------------------------------------------------------
// Adding event to queue
// -----------------------------------------------------------------------------  
void CCCEObserverAsynchroniser::AddEvent(TEventStorage aEvent)
    {
    iEvents.Append(aEvent);
    CCELOGSTRING( "CCCEObserverAsynchroniser::AddEvent");
    if (!iIsRuning)
        {
        CCELOGSTRING( "CCCEObserverAsynchroniser::AddEvent Activating...");
        iIsRuning = ETrue;
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete(status, KErrNone);
        }
    }
    
// end of file
