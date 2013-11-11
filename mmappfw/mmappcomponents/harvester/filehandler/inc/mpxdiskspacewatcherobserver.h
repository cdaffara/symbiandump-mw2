/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Disk Space watcher observer
*
*/



#ifndef MMPXDISKSPACEOBSERVER_H
#define MMPXDISKSPACEOBSERVER_H

/**
* Observer to the disk space watcher
* @lib mpxfilehandler.lib
*/
NONSHARABLE_CLASS( MMPXDiskSpaceObserver )
    {
public:
    /**
    * Callback to the observer 
    * @param aDrive, drive to handle low disk space
    */
    virtual void HandleLowDiskEvent( TInt aDrive ) = 0;
    };


#endif // MMPXDISKSPACEOBSERVER_H
