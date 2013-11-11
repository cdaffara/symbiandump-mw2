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
* Description:  CCP observer, Main plugin observer class
*
*/


#ifndef MCCPOBSERVER_H
#define MCCPOBSERVER_H

// INCLUDES
#include <e32base.h>

/**
CCP error base definition
*/
const TInt KCCPErrorBase = -30391;

// FORWARD DECLARATIONS
class MCCPCall;

/**
* CCP observer class.
* Observer for CCP events, errors and statuses etc. 
*
*  @since S60 3.2
*/
class MCCPObserver
    {
public: 
    enum TCCPError
        {
        /** Registration failed */
        ECCPRegistrationFailed = KCCPErrorBase - 1,
        /** Unregistration failed */
        ECCPUnregistrationFailed = KCCPErrorBase - 2,
        /** Not implemented */
        ECCPNotImplemented = KCCPErrorBase - 3,
        /** Error occured */
        ECCPError = KCCPErrorBase - 4,
        /** Plugin not able to handle incoming call */
        ECCPIncomingCallFailed = KCCPErrorBase - 5
        };

protected:
    /** 
    * Protects the observer being deleted through the observer interface 
    * @since S60 3.2
    */
    virtual inline ~MCCPObserver() {};

public:

    /**
    * An error has occurred concerning the connection.
    * @since S60 3.2
    * @param aError Error code.
    * @return none
    */
    virtual void ErrorOccurred( MCCPObserver::TCCPError aError ) = 0;

    /**
    * Incoming call invitation in MT call case. 
    * @since S60 3.2
    * @param aCall Pointer to the newly created incoming call.
    * @return none
    */
    virtual void IncomingCall( MCCPCall* aCall ) = 0;

    /**
    * Incoming call invitation in attended transfer case. Occurs at transfer target end.
    * For incoming transfer at transferor end see MCCPObserver::CallCreated.
    * @since S60 3.2
    * @param aCall IncomingCall transfer call.
    * @param aTempCall Pointer to the held call.
    */
    virtual void IncomingCall( MCCPCall* aCall,
                               MCCPCall& aTempCall ) = 0;

    /**
    * Indicate an arrived transfer request. 
    * Called at the transfer request receiver end, after receiving a transfer request .     
    * For incoming transfer at transfer target end see MCCPObserver::IncomingCall with two call 
     objects as parameters.
    * @since S60 3.2
    * @param aNewTransferCall The call that has been created with the new transfer target.
    * @param aOriginator Pointer to originator of the transfer request.
    * @param aAttended Transfer requested was attented (ETrue) or un-attented(EFalse)
    */
    virtual void CallCreated( MCCPCall* aNewTransferCall,
                              MCCPCall* aOriginator,
                              TBool aAttended ) = 0;
    };

#endif // MCCPOBSERVER_H

// End of File
