/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handler for the SS Request Complete notification.
*
*/


#ifndef CSATSENDSSREQUESTCOMPLETEHANDLER_H
#define CSATSENDSSREQUESTCOMPLETEHANDLER_H

#include <e32base.h>
#include <rmmcustomapi.h>

class CSendSSHandler;

/**
*  This is the handler for the SS Request Complete notification.
*  This active objects is registered with Custom ETel Api to receive
*  notifications about the SS Request Complete.
*
*  @lib SendSsCmd
*  @since S60 v3.1
*/

class CSatSendSsRequestCompleteHandler : public CActive
    {

public:

    /**
     * Two-phased constructor.
     * @param aPhone A reference to the Custom ETel API.
     * @param aDispatcher Pointer to Ss handler
     * @return a pointer to the newly created active object.
     */
    static CSatSendSsRequestCompleteHandler* NewL( RMmCustomAPI& aPhone,
        CSendSSHandler* aDispatcher );

    /**
     * Destructor.
     */
    virtual ~CSatSendSsRequestCompleteHandler();

    /**
     * Starts listening for the SS Request Complete notification.
     */
    void Start();

protected:

    /**
     * From CActive, handles the request completion.
     */
    void RunL();

    /**
     * From CActive, implements the cancel protocol.
     */
    void DoCancel();

private:

    /**
     * C++ default constructor.
     * @param aPriority An active object priority value.
     * @param aPhone A reference to the Custom ETel API.
     */
    CSatSendSsRequestCompleteHandler( TInt aPriority, RMmCustomAPI& aPhone,
        CSendSSHandler* aDispatcher );

private: // Data

    /**
     * Reference to the Custom API
     */
    RMmCustomAPI& iPhone;

    /**
     * Result of the Ss sending
     */
    TInt iSsStatus;

    /**
     * Pointer to SendSs command handler
     */
    CSendSSHandler* iDispatcher;
    };

#endif      // CSATSENDSSREQUESTCOMPLETEHANDLER_H
