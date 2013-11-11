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
* Description:  Observer class to the folder monitor
*
*/


#ifndef MMPXFOLDERMONITOROBSERVER_H
#define MMPXFOLDERMONITOROBSERVER_H


/** 
*   Interface class to monitor to get callbacks to folder changes
*/
NONSHARABLE_CLASS( MMPXFolderMonitorObserver )
    {
public:
    /**
     * Handle a file system change notification
     *
     * @param aPath path to the folder tha changed
     */
    virtual void HandleDirectoryChangedL( const TDesC& aPath ) = 0;
  
    };

#endif // MMPXFOLDERMONITOROBSERVER_H
