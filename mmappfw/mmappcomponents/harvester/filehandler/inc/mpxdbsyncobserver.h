/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  db synchronization process observer
*
*/


#ifndef MPXDBSYNCOBSERVER_H
#define MPXDBSYNCOBSERVER_H

//INCLUDES
#include <e32def.h>

/**
 *  MMPXDbSyncObserver 
 *
 *  Interface class to handle synchronization messages
 *
 *  @lib mpxfilehandler.lib
 *  @since S60 3.0
 */
class MMPXDbSyncObserver 
    {
public:
    /**
     * Handle a synchronization notification message
     *
     * @since S60 3.0
     * @param aErr, any error
     */
    virtual void HandleSynchronizationComplete( TInt aErr ) = 0;
    };


#endif // MPXDBSYNCOBSERVER_H
