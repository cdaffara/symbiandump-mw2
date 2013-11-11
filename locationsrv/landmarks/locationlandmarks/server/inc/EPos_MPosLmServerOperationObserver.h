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
* Description: Interface for observers of server operations.
*
*
*/


#ifndef MPOSLMSERVEROPERATIONOBSERVER_H
#define MPOSLMSERVEROPERATIONOBSERVER_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CPosLmServerOperation;

/**
*  Interface for observers of server operations.
*
*  This class is subclassed by clients who wants to listen for status on
*  server operations.
*/
class MPosLmServerOperationObserver
    {
     public:  // New functions

        /**
        * Called to report status of a server operation.
        *
        * Will be called when an operation is completed or if it has failed.
        *
        * @param aServerOperation The object which handles the server operation.
        * @param aStatus The status of the server operation.
        */
        virtual void HandleOperationStatus(
        /* IN  */       CPosLmServerOperation* aServerOperation,
        /* IN  */       TInt aStatus
        ) = 0;

    };

#endif      // MPOSLMSERVEROPERATIONOBSERVER_H

// End of File
