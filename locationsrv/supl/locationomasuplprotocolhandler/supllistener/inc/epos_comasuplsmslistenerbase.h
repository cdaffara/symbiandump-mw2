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
* Description:   Base class for OMA SUPL SMS Listener implementations
*
*/



#ifndef C_COMASUPLSMSLISTENERBASE_H
#define C_COMASUPLSMSLISTENERBASE_H

#include <e32base.h>
#include <e32std.h>
#include <es_sock.h>
#include <f32file.h>


/**  WDP Port on which to listen for incoming messages */
const TInt KWDPPort             = 7275;

/**
 *  Base class for OMA SUPL SMS Listener implementations
 *  @since S60 v3.1
 */
class COMASuplSmsListenerBase : public CActive
    {

public:

    /**  
     * States of SMS Listener component 
     */
    enum TState
        {
        EIdle = 0, 
        EListening, 
        EAcknowledging,
        ESubscriptionActive,
        ESettingsInitialized,
        EComplete
        };

public:

    /**
     * Destructor
     *
     */
    virtual ~COMASuplSmsListenerBase();
    
    /**
     * Listens for incoming SMS messages
     *
     * @since S60 v3.1
     */
    void ListenForSMSL();
    
	/**
	 * Wrapper function used for tracing
	 *
	 */
	void Trace( const TDesC& aMessage, const TDesC& aFileName, TInt line );
    
protected:

    /** 
     * Connection to Socket Server
     */
    RSocketServ 	            iSocketServer;

    /** 
     * Socket over which SMS is received
     */
    RSocket 		            iSocket;

    /**
     * Connection to File server required by CSMSMessage API
     */
    RFs 			            iFs;

    /** 
     * 
     */
    TPckgBuf<TUint>             iBuf;

    /** 
     * Current state transition
     */
    TState 			            iState;
    
protected:

    /**
     * Constructor
     *
     */
    COMASuplSmsListenerBase();
    
	/*
	 * Forwards message to SUPL gateway
	 *
	 * @since S60 v3.1
	 */
    void SendL( const TPtr8& aMessage );

    // from base class CActive    
    /**
     * Cancels outanding events
     *
     */	
	void DoCancel();
	
    /**
     * Event handling function
     *
     */	
	void RunL();
	
private:

    /**
     * Associates the socket with a specific address family 
     * based on the implementation and binds the socket
     *
     * @since S60 v3.1
     */
    virtual void InitializeL()		= 0;

    /**
     * Extracts the payload from the received SMS message
     *
     * @since S60 v3.1
     */	
    virtual void ProcessMessageL()	= 0;

    };


#include "epos_comasuplsmslistenerbase.inl"


#endif  // C_COMASUPLSMSLISTENERBASE_H
