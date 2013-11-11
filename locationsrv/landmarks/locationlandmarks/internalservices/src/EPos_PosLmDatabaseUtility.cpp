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


#include <f32file.h>
#include "EPos_CPosLmLocalDbAccess.h"
#include "EPos_LandmarkDatabaseStructure.h"
#include "EPos_PosLmDatabaseUtility.h"

const TUint KPosSingleQuoteCharacter = '\'';

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC* PosLmDatabaseUtility::CreateDatabaseUriL(
    const TDesC& aDbUri)
    {
    TPtrC dbUri = aDbUri;

    if (ProtocolPresent(aDbUri))
        {
        // Remove protocol if present first in the URI.
        dbUri.Set(UriWithoutProtocol(aDbUri));
        }

    TFileName* completePath = new (ELeave) TFileName;
    CleanupStack::PushL(completePath);

    TChar sysDriveChar = RFs::GetSystemDriveChar();
    TBuf<2> defaultDrive;
    defaultDrive.Append( sysDriveChar );
    defaultDrive.Append( KDriveDelimiter );
    User::LeaveIfError(
        VerifyUriIsValid( dbUri, &defaultDrive, completePath ) );

    // Create correct URI
    HBufC* dbUriToReturn = HBufC::NewL(
        KPosLocalDbFileProtocol().Length() + completePath->Length());
    dbUriToReturn->Des().Append(KPosLocalDbFileProtocol);
    dbUriToReturn->Des().Append(*completePath);

    CleanupStack::PopAndDestroy(completePath);

    return dbUriToReturn;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt PosLmDatabaseUtility::ValidateDatabaseUri(
    const TDesC& aDbUri)
    {
    if (!ProtocolPresent(aDbUri))
        {
        return KErrArgument;
        }

    return VerifyUriIsValid(UriWithoutProtocol(aDbUri), NULL);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmDatabaseUtility::RemoveProtocolFromUriL(
    TPtrC& aDbUri)
    {
    // Verify the file name is not too long to be placed into a TFileName
    __ASSERT_ALWAYS(aDbUri.Length() <= KMaxFileName +
        KPosLocalDbFileProtocol().Length(), User::Leave(KErrArgument));

    // Verify there is a protocol at the beginning
    __ASSERT_ALWAYS(ProtocolPresent(aDbUri), User::Leave(KErrArgument));

    // Assume the protocol is specified.
    aDbUri.Set(UriWithoutProtocol(aDbUri));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt PosLmDatabaseUtility::VerifyUriIsValid(
    const TDesC& aUriWoProtocol,
    const TDesC* aDefaultDrive,
    TFileName* aPath)
    {
    TParse parse;

    TInt err = parse.Set(aUriWoProtocol, NULL, aDefaultDrive);

    if (err == KErrBadName)
        {
        // File name too long (longer than KMaxFileName)
        err = KErrArgument;
        }
    else if (err == KErrNone)
        {
        if ((!aDefaultDrive && !parse.DrivePresent()) ||
            parse.PathPresent() || !parse.NamePresent() ||
            parse.Ext().CompareF(KPosDbFileExtension) != 0)
            {
            err = KErrArgument;
            }
        else if (aPath)
            {
            aPath->Copy(parse.FullName());
            }
        }

    return err;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPtrC PosLmDatabaseUtility::UriWithoutProtocol( const TDesC& aDbUri )
    {
    return aDbUri.Right(aDbUri.Length() - KPosLocalDbFileProtocol().Length());
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool PosLmDatabaseUtility::ProtocolPresent( const TDesC& aDbUri )
    {
    return aDbUri.Left(KPosLocalDbFileProtocol().Length()).CompareF(
        KPosLocalDbFileProtocol) == 0;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC* PosLmDatabaseUtility::EmbedSingleQuoteCharactersLC(
    const TDesC& aSearchText)
    {
    // Count the number of single quote characters.
    TInt noOfCharMatches = 0;
    TPtrC originalText(aSearchText);
    TInt pos = originalText.Locate(KPosSingleQuoteCharacter);
    while (pos != KErrNotFound)
        {
        noOfCharMatches++;

        originalText.Set(originalText.Right(originalText.Length() - pos - 1));
        pos = originalText.Locate(KPosSingleQuoteCharacter);
        }

    HBufC* embeddedText = HBufC::NewLC(aSearchText.Length() + noOfCharMatches);

    // Embed all single quote characters.
    originalText.Set(aSearchText);
    pos = originalText.Locate(KPosSingleQuoteCharacter);
    while (pos != KErrNotFound)
        {
        embeddedText->Des().Append(originalText.Left(pos + 1));
        embeddedText->Des().Append(KPosSingleQuoteCharacter);

        originalText.Set(originalText.Right(originalText.Length() - pos - 1));
        pos = originalText.Locate(KPosSingleQuoteCharacter);
        }

    embeddedText->Des().Append(originalText);

    return embeddedText;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TUint PosLmDatabaseUtility::GetTableRowCountL( 
    CPosLmLocalDbAccess& aDbAccess,
    const TDesC& aTableName )
    {
    RDbNamedDatabase db;
    aDbAccess.GetDatabase( db );
    
    RDbTable table;
    User::LeaveIfError( table.Open( db, aTableName, RDbRowSet::EReadOnly ) );
    CleanupClosePushL( table );
    TInt numRows = table.CountL();
    CleanupStack::PopAndDestroy( &table );

    if ( numRows < 0 )
        {
        User::Leave( KErrNotFound );
        }
    
    return numRows;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TBool PosLmDatabaseUtility::IndexExistsL(
    const RDbNamedDatabase& aDatabase,
    const TDesC& aTable,
    const TDesC& aIndex )
    {
    TBool found( EFalse );
    CDbIndexNames* indexes = aDatabase.IndexNamesL( aTable );
    for ( TInt i = 0; i < indexes->Count(); ++i ) 
        {
        if ( (*indexes)[i].Compare( aIndex ) == 0 )
            {
            found = ETrue;
            break;
            }
        }
    delete indexes;
    return found;
    }
