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
* Description:    SIMPLE watcher info callback observer
*
*/




#ifndef M_simplewinfoobserver_H
#define M_simplewinfoobserver_H

#include <e32std.h>


// FORWARD DECLARATION
class MSimpleWinfo;

/**
 *  MSimpleWinfoObserver
 *
 *  SIMPLE watcher info (WINFO) callback observer
 *
 *  @lib simpleengine
 *  @since S60 v3.2
 */

class MSimpleWinfoObserver
    {
public:

    /**
     * Callback for WINFO request completion
     * @since S60 3.2
     * @param aOpid operation id
     * @param aStatus completion status.
     */
    virtual void WinfoReqCompleteL( TInt aOpid, TInt aStatus ) = 0;

    /**
     * Callback for WINFO termination
     * @since S60 3.2
     * @param aOpid operation id
     * @param aReason termination reason. Refer to simpleerrors.h
     */
    virtual void WinfoTerminatedL(
        TInt aOpid, TInt aReason ) = 0;

    /**
     * Callback for WINFO notification
     * @since S60 3.2
     * @param aWinfo WINFO
     */
    virtual void WinfoNotificationL(
        MSimpleWinfo& aWinfo ) = 0;
    };

#endif

// End of File
