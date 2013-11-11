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


#ifndef CCCEOBSERVERASYNCHRONISER_H
#define CCCEOBSERVERASYNCHRONISER_H

#include <e32base.h>
#include "mcceobserver.h"
#include "mccessobserver.h"
#include "mcceconferencecallobserver.h"
#include "mccedtmfobserver.h"
#include "mcceextensionobserver.h"


NONSHARABLE_CLASS(CCCEObserverAsynchroniser) : public CActive,  
                                    MCCEObserver,  
                                    MCCESsObserver, 
                                    MCCEConferenceCallObserver, 
                                    MCCEDtmfObserver, 
                                    MCCEExtensionObserver
    {
    
public:
    
    /**
    * Two-phased constructor
    * 
    */
    static CCCEObserverAsynchroniser* NewL();
   
    /**
    * Two-phased constructor
    * Leaves pointer in cleanup stack
    */
    static CCCEObserverAsynchroniser* NewLC();

    /**
     * Destructor
     * 
     */
    ~CCCEObserverAsynchroniser();
    
    void SetMCCEObserver(MCCEObserver* aObserver);
    
    void SetMCCESsObserver(MCCESsObserver* aObserver);
    
    void SetMCCEConferenceCallObserver(const MCCEConferenceCallObserver* aObserver);
    
    void SetMCCEDtmfObserver(const MCCEDtmfObserver* aObserver);
    
    void SetMCCEExtensionObserver(const MCCEExtensionObserver* aObserver);
   
   
    MCCEObserver& GetMCCEObserver();
    
    MCCESsObserver& GetMCCESsObserver();
    
    MCCEConferenceCallObserver& GetMCCEConferenceCallObserver();
    
    MCCEDtmfObserver& GetMCCEDtmfObserver();
    
    MCCEExtensionObserver& GetMCCEExtensionObserver();
    
    /**
    * From CActive
    */
    void RunL();
   
    /**
    * From CActive
    */
    TInt RunError(TInt aError);
   
    /**
    * From CActive
    */
    void DoCancel();      
   
    /**
    * From MCCEObserver
    */
    void IncomingCall( MCCECall& aCall );

    /**
    * From MCCEObserver
    */
    void MOCallCreated( MCCECall& aCall );

    /**
    * From MCCEObserver
    */
    void DataPortName( TName& aPortName );

    /**
    * From MCCEObserver
    */
    void ConferenceCallCreated( MCCEConferenceCall& aConferenceCall );

    /**
    * From MCCESsObserver
    */
    void BarringEventOccurred( const MCCESsObserver::TCCESsBarringEvent aBarringEvent );
                                  
    /**
    * From MCCESsObserver
    */
    void CLIEventOccurred( const MCCESsObserver::TCCESsCLIEvent aCallLineEvent );
                                  

    /**
    * From MCCESsObserver
    */
    void CallForwardEventOccurred( 
                        const MCCESsObserver::TCCESsCallForwardEvent aCallForwardEvent,
                        const TDesC& aRemoteAddress );

    /**
    * From MCCESsObserver
    */
    void CallCugEventOccurred( const MCCESsObserver::TCCESsCugEvent aCugEvent );

    /**
    * From MCCESsObserver
    */
    void NotifyCurrentActiveALSLine( TInt aLine );

    /**
    * From MCCEConferenceCallObserver
    */
    void CallEventOccurred( 
                 const MCCEConferenceCallObserver::TCCEConferenceCallEvent aEvent,
                 MCCECall* aReferredCall=NULL  );

    /**
    * From MCCEConferenceCallObserver
    */
    void CallStateChanged( 
                 const MCCEConferenceCallObserver::TCCEConferenceCallState aState );


    /**
    * From MCCEConferenceCallObserver
    */
    void CallCapsChanged( 
                 const MCCEConferenceCallObserver::TCCEConferenceCallCaps aCaps );
     
    /**
    * From MCCEConferenceCallObserver
    */
    void ErrorOccurred( TCCPConferenceCallError aError  );

    /**
    * From MCCEConferenceCallObserver
    */
    void CallDurationChanged( const TTimeIntervalSeconds aDuration );
   
    /**
    * From MCCEDtmfObserver
    */  
    void HandleDTMFEvent( const MCCEDtmfObserver::TCCEDtmfEvent aEvent, 
                                  const TInt aError, 
                                  const TChar aTone );
    /**
    * From MCCEExtensionObserver
    */                                  
    void HandleExtensionEvents( TUint32 aServiceId,
        TInt aEvent,
        TInt aStatus );
        
private:
    
    enum TEventType
        {
        EIncomingCall = 0,
        EMOCallCreated,
        EDataPortName,
        EConferenceCallCreated,
        EBarringEventOccurred,
        ECLIEventOccurred,
        ECallForwardEventOccurred,
        ECallCugEventOccurred,
        ENotifyCurrentActiveALSLine,
        ECallEventOccurred,
        ECallStateChanged,
        ECallCapsChanged,
        EErrorOccurred,
        ECallDurationChanged,
        EHandleDTMFEvent,
        EHandleExtensionEvents           
        };
        
    class TEventStorage
    {
    public:
        TEventType iEventType;
        MCCECall* iCall;
        MCCEConferenceCall* iConferenceCall;
        const TDesC* iDescriptor;
        TInt iInt1;
        TInt iInt2;
        TChar iChar;
        TUint32 iServiceId;
        TTimeIntervalSeconds iDuration;
        TName iTName;
    };
    
    /**
    * Constructor
    */
    CCCEObserverAsynchroniser();
 
    /** 
    * Leaving stuff in constructor 
    */
    void ConstructL();
    
    void AddEvent(TEventStorage aEvent);
    
    RArray<TEventStorage> iEvents;
   
    TBool iIsRuning;
    
    MCCEObserver* iCCEObserver;
   
    MCCESsObserver* iCCESsObserver;
   
    MCCEConferenceCallObserver* iCCEConferenceCallObserver;
  
    MCCEDtmfObserver* iCCEDtmfObserver;
   
    MCCEExtensionObserver* iCCEExtensionObserver;
    
    };

#endif // CCCEOBSERVERASYNCHRONISER_H