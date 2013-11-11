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
* Description:  Handler for the SS Additional Info notification
*
*/


#ifndef CSATSENDSSADDITIONALINFOHANDLER_H
#define CSATSENDSSADDITIONALINFOHANDLER_H

#include <e32base.h>
#include <rmmcustomapi.h>

class CSendSSHandler;

/**
*  This is the handler for the SS Additional Info notification.
*  This active objects is registered with Custom ETel Api to receive notifications
*  about the SS Additional Info.
*
*  @lib SendSsCmd
*  @since S60 v3.1
*/
class CSatSendSsAdditionalInfoHandler : public CActive
    {

public:

    /**
     * Two-phased constructor.
     * @param aPhone A reference to the Custom ETel API.
     * @param aDispatcher Pointer to Ss handler
     * @return a pointer to the newly created active object.
     */
    static CSatSendSsAdditionalInfoHandler* NewL( RMmCustomAPI& aPhone,
        CSendSSHandler* aDispatcher );

    /**
     * Destructor.
     */
    virtual ~CSatSendSsAdditionalInfoHandler();

    /**
     * Starts listening for the Sat command. 
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

private: // data

    /**
     * C++ default constructor.
     * @param aPriority An active object priority value.
     * @param aDispatcher Pointer to Ss handler
     * @param aPhone A reference to the Custom ETel API.
     */
    CSatSendSsAdditionalInfoHandler( TInt aPriority, RMmCustomAPI& aPhone,
        CSendSSHandler* aDispatcher );

private:

    /**
     * Reference to the Custom API
     */
    RMmCustomAPI& iPhone;

    /**
     * Additional information received through Custom API
     */
    RMmCustomAPI::TSsAdditionalInfo iSsAdditionalInfo;

    /**
     * Pointer to SendSs command handler
     */
    CSendSSHandler* iDispatcher;

    };

#endif      // CSATUISSADDITIONALINFOHANDLER_H
