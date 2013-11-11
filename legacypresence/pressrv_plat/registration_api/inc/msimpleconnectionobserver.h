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
* Description:    SIMPLE engine connection observer
*
*/




#ifndef M_simpleconnectionobserver_H
#define M_simpleconnectionobserver_H

#include <e32std.h>
#include <msimpleconnection.h>

/**
 *  MSimpleConnectionObserver
 *
 *  SIMPLE engine connection observer
 *
 *  @lib simpleengine
 *  @since S60 3.2
 */

class MSimpleConnectionObserver
    {

public:

    /**
     * Connection status change
     * @since S60 3.2
     * @param aState new state
     */
    virtual void ConnectionStatusL( MSimpleConnection::TSimpleState aState ) = 0;

    /**
     * Client's request completion.
     *
     * @since S60 3.2
     * @param aOpId operation id
     * @param aStatus completion status
     */
    virtual void RequestCompleteL( TInt aOpId, TInt aStatus ) = 0;

    };

#endif

// End of File
