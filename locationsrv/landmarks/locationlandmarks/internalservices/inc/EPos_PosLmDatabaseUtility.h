/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Utility function for landmark database.
*
*
*/


#ifndef POSLMDATABASEUTILITY_H
#define POSLMDATABASEUTILITY_H

#include <e32std.h>

class CPosLmLocalDbAccess;
class RDbNamedDatabase;

_LIT(KPosLocalDbFileProtocol, "file://");
_LIT(KPosDbFileExtension, ".ldb");

/**
*  Utility function for landmark database.
*/
class PosLmDatabaseUtility
    {

    public: // New functions

        /**
        * Creates a correct database URI,
        * using format file://[drive]:[filename].ldb.
        *
        * Check that there is at least a [filename].ldb in incoming URI.
        * Adds file:// if not present in incoming URI.
        * If [drive]: is not present in incoming URI c: is set in
        * created URI.
        *
        * Leaves with @p KErrArgument if URI has wrong format.
        *
        * @param aDbUri A database URI.
        * @return A correct URI.
        */
        IMPORT_C static HBufC* CreateDatabaseUriL(
        /* IN */        const TDesC& aDbUri
        );

        /**
        * Validates a database URI, using format file://drive:filename.ldb.
        *
        * Return @p KErrArgument if URI is not valid.
        *
        * @param aDbUri A database URI.
        * @return Error code.
        */
        IMPORT_C static TInt ValidateDatabaseUri(
        /* IN */        const TDesC& aDbUri
        );

        /**
        * Removes the protocol (i.e. "file://") from the URI. This function
        * assumes that the protocol is present first in the specified URI.
        *
        * @param aUri A URI. On out it contains the URI without the protocol.
        */
        IMPORT_C static void RemoveProtocolFromUriL(
        /* IN/OUT */    TPtrC& aDbUri
        );

        /**
        * Single quote characters (') in the search text will be embedded by
        * using two single quote characters.
        *
        * The caller takes ownership of the returned string.
        *
        * @param aSearchText The search text.
        * @return The search text with embedded single quote characters.
        */
        IMPORT_C static HBufC* EmbedSingleQuoteCharactersLC(
        /* IN  */       const TDesC& aSearchText
        );

        /** Returns number of rows in a table 
         * @param aTableName Name of the table which rowcount is needed */
        IMPORT_C static TUint GetTableRowCountL( 
            CPosLmLocalDbAccess& aDbAccess,
            const TDesC& aTableName );

        /** Checks whether given index exists on given table */
        IMPORT_C static TBool IndexExistsL(
            const RDbNamedDatabase& aDatabase,
            const TDesC& aTable,
            const TDesC& aIndex );

    private:

        /**
        * Verifies that a database URI is valid.
        *
        * @param aUriWoProtocol The database path set by a client, which
        *   is the same as the URI without the protocol.
        * @param aDefaultDrive The default drive, or NULL if no drive
        *   default drive should be set.
        * @param aPath Contains the full path including drive
        *   but without protocol. This path is only set if this function
        *   returns KErrNone.
        * @return KErrNone if valid, KErrArgument otherwise.
        */
        static TInt VerifyUriIsValid(
        /* IN */        const TDesC& aUriWoProtocol,
        /* IN/OUT */    const TDesC* aDefaultDrive,
        /* OUT */       TFileName* aPath = NULL
        );

        /**
        * Returns the URI without the protocol. Assumes there is a protocol
        * available.
        *
        * @param aDbUri A database URI.
        * @return Database URI without protocol.
        */
        static TPtrC UriWithoutProtocol(
        /* IN */        const TDesC& aDbUri
        );

        /**
        * Checks if the protocol is present first in the URI.
        *
        * @param aDbUri A database URI.
        * @return ETrue if the protocol is present. EFalse othwerwise.
        */
        static TBool ProtocolPresent(
        /* IN */        const TDesC& aDbUri
        );
    };

#endif      // POSLMDATABASEUTILITY_H

