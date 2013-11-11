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
*       Interface for observers of operation managers.
*
*/


#ifndef MPOSLMSERVERMANAGEROBSERVER_H
#define MPOSLMSERVERMANAGEROBSERVER_H

//  INCLUDES
#include <e32base.h>

/**
*  Interface for observers of operation managers.
*
*  This class is subclassed by clients who wants to listen for an event from an
*  operation manager. The operation manager calls this function when it no
*  longer has any server operations running.
*/
class MPosLmServerManagerObserver
    {
     public:  // New functions

        /**
        * Called to report that a operation manager no longer has any server
        * operations running.
        *
        */
        virtual void HandleManagerFinished() = 0;

    };

#endif      // MPOSLMSERVERMANAGEROBSERVER_H

// End of File
