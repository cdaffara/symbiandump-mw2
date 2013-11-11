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
* Description:    SIMPLE Engine watcher callback observer
*
*/




#ifndef M_simplewatcherobserver_H
#define M_simplewatcherobserver_H

#include <e32std.h>

// FORWARD DECLARATION
class MSimpleConnection;
class MSimplePresenceList;
class MSimpleDocument;

/**
 *  MSimpleWatcher
 *
 *  SIMPLE Engine watcher callback observer
 *
 *  @lib simpleengine
 *  @since S60 3.2
 */

class MSimpleWatcherObserver
    {

public:

    /**
     * Watcher request completion.
     *
     * @since S60 3.2
     * @param aOpId operation id
     * @param aStatus completion status. 
     */
    virtual void WatcherReqCompleteL( TInt aOpId, TInt aStatus ) = 0;

    /**
     * Watcher notification callback.
     *
     * @since S60 3.2
     * @param aDocument presence entity data
     */
    virtual void WatcherNotificationL( MSimpleDocument& aDocument ) = 0;

    /**
     * Watcher presence list notification callback.
     *
     * @since S60 3.2
     * @param aList presence list
     */
    virtual void WatcherListNotificationL( MSimplePresenceList& aList ) = 0;

    /**
     * Watcher request terminated
     *
     * @since S60 3.2
     * @param aOpId operation id
     * @param aReason termination reason. Refer to simpleerrors.h
     */
    virtual void WatcherTerminatedL(
        TInt aOpId, TInt aReason ) = 0;
    };

#endif

// End of File
