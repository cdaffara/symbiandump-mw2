/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:
*
*/
#ifndef CPEMESSAGEWAITER_H
#define CPEMESSAGEWAITER_H

#include <e32base.h>
#include <PEVirtualEngine.h>

/**
 * Synchorises the waiting for PhoneEngine messages.
 */
NONSHARABLE_CLASS( CPEMessageWaiter ) : public CTimer, public MEngineMonitor
    {
public:
    
    static CPEMessageWaiter* NewL();
    
    ~CPEMessageWaiter();
    
protected:

    CPEMessageWaiter();
    
    void ConstructL();
    
public: // New methods

    /**
     * Starts waiting for message. The call returns when phoneengine
     * sends the message. The waiting will fail if message does not
     * arrive in timeout given.
     */
    void WaitForMsg( 
        MEngineMonitor::TPEMessagesFromPhoneEngine aMsg,
        TInt aTimeOutInSeconds = 5);
    
    /**
     * Starts waiting for error. The call returns when phoneengine
     * sends the error.
     */
    void WaitForError( TInt aError );
    
    /**
     * Waits for timeout.
     */
    void WaitL( TInt aTimeoutInSeconds );
    
    /**
     * To check if certain phone engine message has arrived.
     */
    TBool HasMsgArrived( MEngineMonitor::TPEMessagesFromPhoneEngine aMsg );
    
    /**
     * Last Error Code.
     */
    TInt LastErrorCode();
     
public: // From MEngineMonitor

    void HandleError( const TPEErrorInfo& aErrorInfo );

    void HandleMessage( const TInt aMessage, const TInt aCallId );
    
protected: // From CTimer

    void RunL();
    
protected:

    CActiveSchedulerWait* iWait;
    
    MEngineMonitor::TPEMessagesFromPhoneEngine iWaitedMsg;
    
    RArray<MEngineMonitor::TPEMessagesFromPhoneEngine> iArrivedMsgs;
    
    TBool iWaitedMsgArrived;
    
    TInt iLastErrorCode;
    TInt iWaitedErrorCode;
    TBool iWaitedErrorCodeArrived;
    
    };

#endif // CPEMESSAGEWAITER_H
