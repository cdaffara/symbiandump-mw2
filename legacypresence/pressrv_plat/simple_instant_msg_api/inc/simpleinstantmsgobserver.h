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
* Description: SIMPLE Engine instant message callback observer
*
*/

#ifndef M_SIMPLEINSTANTMSGOBSERVER_H
#define M_SIMPLEINSTANTMSGOBSERVER_H


#include <e32std.h>


/**
 *  MSimpleInstantMsgObserver
 *
 *  SIMPLE Instant Message callback observer
 *
 *  @lib simpleengine.lib
 *  @since S60 v5.1
 */
class MSimpleInstantMsgObserver
    {

public:

    /**
     * Callback for completion of Send Instant Message Request
     *
     * @since S60 v5.1
     * @param aOpid   Operation ID
     * @param aStatus Completion status
     */
    virtual void SendInstantMsgReqComplete( TInt aOpid, TInt aStatus ) = 0;
        
    /**
     * Callback for handling of received text message
     *
     * @since S60 v5.1
     * @param aFrom    Message received from
     * @param aContent Message content
     */
    virtual void HandleReceiveTextMessage( TPtrC8 aFrom,
        TPtrC8 aContent ) = 0;
    
    };


#endif // M_SIMPLEINSTANTMSGOBSERVER_H
