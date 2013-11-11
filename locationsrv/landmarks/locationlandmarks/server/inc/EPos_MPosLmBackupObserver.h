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
* Description: 
*       Interface for observing backup events published by system backup.
*
*/


#ifndef MPOSLMBACKUPOBSERVER_H
#define MPOSLMBACKUPOBSERVER_H

//  INCLUDES
#include <e32base.h>

/**
*  Interface for observing backup events published by system backup.
*
*  This class is subclassed by clients who wants to listen for system
*  backup events. Only 'start backup' events will be issued.
*/
class MPosLmBackupObserver
    {
     public:  // New functions

        /**
        * Called to report that a backup has started.
        *
        *
        */
        virtual void HandleBackupEvent() = 0;

    };

#endif      // MPOSLMBACKUPOBSERVER_H

// End of File
