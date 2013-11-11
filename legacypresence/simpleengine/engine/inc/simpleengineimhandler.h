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
* Description:  SIMPLE Engine IM Handler
*
*/


#ifndef C_SIMPLEENGINEIMHANDLER_H
#define C_SIMPLEENGINEIMHANDLER_H


#include <e32std.h>

#include "simpleclient.h"
#include "simpleinstantmessage.h"

class MSimpleInstantMsgObserver;


/**
 *  Class for sending and receiving instant messages.
 *
 *  @code
 *   MSimpleInstantMessage* im = 
 *      TSimpleFactory::NewInstantMessageL( conn, connObs );
 *   TInt result = im->SendInstantMessage( text8, recipientsPtr8 );
 *  @endcode
 *
 *  @lib simpleengine.lib
 *  @since S60 v5.1
 */
class CSimpleEngineImHandler : public CSimpleClient,
                               public MSimpleInstantMessage
    {

public:

    /**
     * Two-phased constructor.
     * @param aConn connection
     * @param aObserver callback observer
     */
    static CSimpleEngineImHandler* NewL( MSimpleConnection& aConn,
        MSimpleInstantMsgObserver& aObserver );

    /**
     * Destructor.
     */
    virtual ~CSimpleEngineImHandler();

// from base class MSimpleInstantMessage

    /**
     * From MSimpleInstantMessage
     * Send instant message.
     *
     * @since S60 v5.1
     * @param aMsgContent message content
     * @param aRecipient  recipient ID
     * @return operation ID
     */
    TInt SendInstantMessage( TPtrC8 aMsgContent, TPtrC8 aRecipient );

    /**
     * From MSimpleInstantMessage
     * Destroy this entity.
     *
     * @since S60 v5.1
     */
    void Close();

// from base class CSimpleClient

    /**
     * From CSimpleClient
     * Complete the open client request.
     * 
     * @since S60 v5.1
     * @param aOpId operation id
     * @param aStatus result status
     * @param aReq request
     */
    void Complete( TInt aOpId, TInt aStatus, MSimpleEngineRequest& aReq );

private:

    CSimpleEngineImHandler( MSimpleConnection& aConn,
        MSimpleInstantMsgObserver& aObserver );

    void ConstructL();

    /**
     * Do Send Instant Message request.
     * 
     * @since S60 v5.1
     * @param aMsgContent message content
     * @param aRecipient  recipient ID
     */
    void DoSendInstantMessageRequestL( TPtrC8 aMsgContent,
        TPtrC8 aRecipient );

    /**
     * Receive instant message.
     * 
     * @since S60 v5.1
     */
    void ReceiveInstantMessageL();

private: // data

    /**
     * Instant Message observer for callbacks
     */
    MSimpleInstantMsgObserver& iObserver;

    };

#endif // C_SIMPLEENGINEIMHANDLER_H
