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
* Description:handles icon paths in the database.
*
*/


#ifndef POSLMICONHANDLER_H
#define POSLMICONHANDLER_H

//  INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class CPosLmLocalDbAccess;

// CLASS DECLARATION

/**
*  Class containing static functions that handles icon paths in the database.
*/
class PosLmIconHandler
    {

    public:  // New functions

        /**
        * Gets the icon path id from the landmark database for the specified
        * icon path. If the icon path does not exist it will be added to
        * the database.
        *
        * @param aDbAccess A database access.
        * @param aIconPath Icon path.
        * @param aIconPathId The icon id for the specified icon path.
        */
        static void GetIconPathIdL(
        /* IN */        CPosLmLocalDbAccess& aDbAccess,
        /* IN */        const TDesC& aIconPath,
        /* OUT */       TUint32& aIconPathId
        );

        /**
        * Gets the icon path from the landmark database with the specified
        * icon path id.
        *
        * @param aDbAccess A database access.
        * @param aIconPathId Icon path id.
        * @param aIconPath Icon path. Enough memory must be allocated by user
        *   to hold the icon path (preferably buffer of KMaxFileName size).
        * @return KErrNone if found. KErrNotFound if not found.
        */
        static TInt GetIconPathL(
        /* IN */        CPosLmLocalDbAccess& aDbAccess,
        /* IN */        TUint32 aIconPathId,
        /* OUT */       TDes& aIconPath
        );

        /**
        * Removes an icon path from the landmark database if it is not used by
        * any category or landmark.
        *
        * @param aDbAccess A database access.
        * @param aIconId Icon path id.
        */
        static void RemoveIconIfNotUsedL(
        /* IN */        CPosLmLocalDbAccess& aDbAccess,
        /* IN */        const TUint32& aIconPathId
        );

    private:

        /**
        * Adds an icon path to the landmark database.
        *
        * @param aDbAccess A database access.
        * @param aIconPath The icon path to add.
        * @param aIconPathid Icon path id of the added icon path.
        */
        static void AddIconPathL(
        /* IN */        CPosLmLocalDbAccess& aDbAccess,
        /* IN */        const TDesC& aIconPath,
        /* OUT */       TUint32& aIconPathId
        );

    };

#endif      // POSLMICONHANDLER_H

// End of File
