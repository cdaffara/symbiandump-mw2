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


#ifndef CCCECALLOBSERVERASYNCHRONISER_H
#define CCCECALLOBSERVERASYNCHRONISER_H


#include <e32base.h>
#include "mccecallobserver.h"


NONSHARABLE_CLASS(CCCECallObserverAsynchroniser)  : 
    public CActive,  
    public MCCECallObserver
    {
    
public:
    
    /**
    * Two-phased constructor
    * 
    */
    static CCCECallObserverAsynchroniser* NewL();
   
    /**
    * Two-phased constructor
    * Leaves pointer in cleanup stack
    */
    static CCCECallObserverAsynchroniser* NewLC();

    /**
     * Destructor
     * 
     */
    ~CCCECallObserverAsynchroniser();
    

    void SetMCCECallObserver(MCCECallObserver* aObserver); 
    
    MCCECallObserver* GetMCCECallObserver();
  
   
   
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
    
    
    void ErrorOccurred( const TCCPError aError );
    
    /**
    * From MCCECallObserver
    */
    void CallStateChanged( const CCPCall::TCallState aState );

    /**
    * From MCCECallObserver
    */
    void CallStateChangedWithInband( const CCPCall::TCallState aState );

    /**
    * From MCCECallObserver
    */
    void CallEventOccurred( const MCCECallObserver::TCCECallEvent aEvent );

    /**
    * From MCCECallObserver
    */
    void CallDurationChanged( const TTimeIntervalSeconds aDuration );
    
    /**
    * From MCCECallObserver
    */
    void CallCapsChanged( const MCCECallObserver::TCCECallControlCaps aCaps );
    
    /**
    * From MCCECallObserver
    */
    void HandleTransfer( const TBool aAttended, 
                                 const TDesC& aDestination );
                                 
private:
  
   enum TEventType
        {
        EErrorOccurred = 0,
        ECallStateChanged,
        ECallStateChangedWithInband,
        ECallEventOccurred,
        ECallDurationChanged,
        ECallCapsChanged,
        EHandleTransfer         
        };
        
    class TEventStorage
    {
    public:
        TEventType iEventType;
        const TDesC* iDescriptor;
        TInt iInt1;
        TBool iBoolean;
        TTimeIntervalSeconds iDuration;
    };
    
    /**
    * Constructor
    */
    CCCECallObserverAsynchroniser();
 
    /** 
    * Leaving stuff in constructor 
    */
    void ConstructL();
    
    void AddEvent(TEventStorage aEvent);
    
    RArray<TEventStorage> iEvents;
   
    TBool iIsRuning;
    
    MCCECallObserver* iCCECallObserver;
    };
    
    
#endif //CCCECALLOBSERVERASYNCHRONISER_H
