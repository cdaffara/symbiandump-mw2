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
* Description: Handles database backup registration information
*
*
*/


#ifndef POSLMDBBACKUPHANDLER_H
#define POSLMDBBACKUPHANDLER_H

//  INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
*  This class contains static functions which handles database backup
*  registration information.
*
*/
class PosLmDbBackupHandler
    {
    public: // New functions

        /**
        * Notify db change.
        *
        * @param aFileName Filename, including drive, for the database that
        * has been changed.
        */
        static void NotifyDbChangeL(
        /* IN */    const TDesC& aFileName
        );

    };

#endif      // POSLMDBBACKUPHANDLER_H

// End of File

