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
* Description:  Abstract interface class to observe system events
*
*/


#ifndef M_MPXSYSTEMEVENTOBSERVER_H
#define M_MPXSYSTEMEVENTOBSERVER_H

#include "mpxharvestercommon.h"

/**
 *  Abstract interface for observing system events
 *
 *  @lib mpxharvesterserver.exe
 *  @since S60 3.0
 */
NONSHARABLE_CLASS( MMPXSystemEventObserver )
    {

public:

    /**
     * HandleSystemEvent
     *
     * @since S60 3.0
     * @param aEvent event to handle
     * @param aData additional data for the event (ie what drive number)
     */
     virtual void HandleSystemEventL( TSystemEvent aEvent, TInt aData ) = 0;

    };

#endif // M_MPXSYSTEMEVENTOBSERVER_H
