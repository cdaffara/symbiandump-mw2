/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   WAP Listener class definition
*
*/



#ifndef C_COMASUPLWAPLISTENER_H
#define C_COMASUPLWAPLISTENER_H

#include <e32base.h>
#include <push/cpushhandlerbase.h>

#include "epos_suplnetwork.h"
#include "epos_comasupltrace.h"
/**
 * COMASuplWapListener is an ECom plugin that registers with the WAP framework to
 * listen to SUPL INIT requests sent as WAP Push messages.
 * @since S60 v3.0
 */
class COMASuplWapListener : public CPushHandlerBase
    {
    
public:	// Methods

    /**
     * Factory function that instantiates a new object of COMASuplWapListener
     *
     * @since S60 v3.0
     */
    static COMASuplWapListener* NewL();

    /**
     * Destructor
     *
     * @since S60 v3.0
     */
    ~COMASuplWapListener();

public:	// From base class CPushHandlerBase

    /**
     * From CPushHandlerBase
     * Called by the Wap Push Framework to handle the message. Asynchronous method
     *
     * @since S60 v3.0
     * @param aPushMsg The push message delivered from the framework containing
     *	               the Push Message.  Ownership is taken over.
     * @param aStatus  when this function is complete, this status is returned 
     *				   with a completion code.
     */
    void HandleMessageL( CPushMessage* aPushMsg, TRequestStatus& aStatus );

    /**
     * From CPushHandlerBase
     * Called by the Wap Push Framework to handle the message. Synchronous method
     *
     * @since S60 v3.0
     * @param aPushMsg The push message delivered from the framework containing
     *	               the Push Message.  Ownership is taken over.
     */
    void HandleMessageL( CPushMessage* aPushMsg );

    /**
     * From CPushHandlerBase
     * Called by the Wap Push Framework to cancel outstanding events.
     *
     * @since S60 v3.0
     */
    void CancelHandleMessage();

    /**
     * From CPushHandlerBase
     * Reserved for ECOM for future expansion
     *
     * @since S60 v3.0
     */
     void CPushHandlerBase_Reserved1();

    /**
     * From CPushHandlerBase
     * Reserved for ECOM for future expansion
     *
     * @since S60 v3.0
     */
    void CPushHandlerBase_Reserved2();

private:	// From base class CActive

    /**
     * From CActive
     * Called to cancel outanding events
     *
     * @since S60 v3.0
     */
    void DoCancel();

    /**
     * From CActive
     * Request completion event handler
     *
     * @since S60 v3.0
     */
    void RunL();

    /**
     * From CActive
     * Handles the event of leave from the request completion event handler RunL()
     *
     * @since S60 v3.0
     * @param aError Argument specifying the error code
     */
    TInt RunError( TInt aError );

private:	// Methods

    /**
     * Default constructor
     *
     * @since S60 v3.0
     */
    COMASuplWapListener();

    /**
     * 2nd phase constuctor for instantiating member variables
     *
     * @since S60 v3.0
     */
    void ConstructL();

    /**
     * Completes the iRequestStatus with KErrNone 
     *
     * @since S60 v3.0
     */
    void IdleComplete();

    /**
     * Terminate execution
     *
     * @since S60 v3.0
     * @param aError - indicate to function an error	 
     */
    void Term( TInt aError );

    /**
     * Extracts the SUPL INIT request from the WAP Push message. Creates an IPC 
     * connection with the SUPL gateway, forwards the request and terminates 
     * the connection
     *
     * @since S60 v3.0
     */
    void  ProcessPushMsgL();

    /**
     * Creates an IPC connection with the SUPL gateway, forwards 
     *				   the request and terminates the connection
     * @since S60 v3.0
     */
    void SendL( TPtrC8& aMessage );
    
    TBool CheckBinaryContentType(CPushMessage* aPushMsg);
    void PrintHex(const TDesC8& aBuffer,TInt aLine);

private:	// Attributes

    /**
     * Data field to hold the error value
     */
    TInt				iErrCode;

    typedef enum TState {  EProcess, EComplete };  

    TState 				iState;

    /**
     * Push Message passed in
     */
    CPushMessage*		iMessage;

    /**
     * Flag to indicate if this was called asynchonously
     */
    TBool				iAsyncHandling;

    /**
     * Handle to an instance of COMASuplTrace used for tracing
     */ 
    COMASuplTrace*			iTrace;

    };

#endif	// C_COMASUPLWAPLISTENER_H
