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
* Description:Provides functions for finding resource files.
*
*/


#ifndef POSLMFILEFINDER_H
#define POSLMFILEFINDER_H

// INCLUDES
#include <f32file.h>

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* Provides functions for finding resource files.
*/
class PosLmFileFinder
    {
    public:  // New functions

        /* Searches all installed drives for a localized
        * resource file. The filename is set to the resource file.
        * Drive C: is searched first, then A: to Z: if they are available.
        * @param aFilename Path to the localized resource file.
        * @param aFileSession A file session.
        * @param aResourceFieName Name of resource file.
        * @return Returns KErrNotFound if the resource cannot be found on any
        *   drive, KErrNone otherwise.
        */
        IMPORT_C static TInt ResourceFileL(
        /* IN/OUT */       TFileName* aFilename,
        /* IN  */       RFs& aFileSession,
        /* IN  */       const TDesC& aResourceFileName
        );

    private:

        /**
        * Checks if the resource file can be found on the specified drive.
        * @param aDrive A drive to search.
        * @param aFilename File name (and path) to the resource file if found.
        * @param aFileSession A file session.
        * @param aResourceFieName Name of resource file.
        * @return ETrue if resource file is found on specified drive,
        *   EFalse otherwise.
        */
        static TBool IsResourceOnDriveL(
        /* IN */        TInt aDrive,
        /* IN/OUT */       TFileName* aFilename,
        /* IN  */       RFs& aFileSession,
        /* IN  */       const TDesC& aResourceFileName
        );

    };

#endif      // POSLMFILEFINDER_H

// End of File
