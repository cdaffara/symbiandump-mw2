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
* Description: Landmarks Server panic codes used for panicking a client.
*
*
*/


#ifndef LANDMARKSSERVERPANICS_H
#define LANDMARKSSERVERPANICS_H

// INCLUDES
#include <e32base.h>
#include <e32def.h>

// CONSTANTS
_LIT(KPosLandmarksServerPanic, "Landmarks Server");

/**
 * Landmarks Server panic codes used for panicking a client.
 */
enum TPosLmSrvClientPanic
    {
    EPosUnableToReadOrWriteDataToClient                     = 1
    /** The Landmarks Server cannot read or write data to a client. The client
        thread will be panicked. */,
    EPosEventNotifierAlreadyHasOutstandingRequest
    /** The event notifier already has an outstanding request. */,
    EPosClientHasNotRegistered
    /** The client has not registered itself. A register message must always
        be sent to the server first. */,
    EPosLmDbNotLocked
    /** A client has specified to unlock a landmark database that is not
        locked. */,
    EPosClientHasAlreadyRegistered
    /** The client has already registered itself. A register message can not be
        sent twice for the same sub-session instance. */
    };

/**
 * Landmarks Server panic codes used for panicking the server.
 */
enum TPosLmSrvPanic
    {
    EPosLmServerPanicSessionsCountInconsistency             = 1000
    /** Internal server error. The number of sessions is invalid. */
    };

// FUNCTION PROTOTYPES
GLDEF_C void PanicClient(const RMessagePtr2& aMessagePtr, TInt aReason);
GLDEF_C void PanicServer(TInt aReason);

#endif      // LANDMARKSSERVERPANICS_H

// End of File
