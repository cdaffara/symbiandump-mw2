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
* Description: Provides functions for finding resource files.
*
*
*/


// INCLUDE FILES
#include <bautils.h>
#include "EPos_PosLmFileFinder.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// PosLmFileFinder::ResourceFileL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt PosLmFileFinder::ResourceFileL(
    TFileName* aFilename,
    RFs& aFileSession,
    const TDesC& aResourceFileName)
    {
    TInt ret = KErrNone;
    TBool fileFound = EFalse;
    
    
    // Check system drive first
    TInt sysDrive = KErrNotFound;
    if ( BaflUtils::GetSystemDrive( (TDriveNumber&) sysDrive  ) == KErrNone )
        {
        fileFound = IsResourceOnDriveL( sysDrive, aFilename, aFileSession, aResourceFileName );
        }

    if (!fileFound)
        {
        TDriveList driveList;
        User::LeaveIfError(aFileSession.DriveList(driveList));

        for ( TInt driveNumber = EDriveA; driveNumber <= EDriveZ && !fileFound; driveNumber++ )
            {
            // Skip system drive because it has already been checked.
            if ( driveNumber == sysDrive )
                {
                driveNumber++;
                }

            if ( driveList[driveNumber] && !( driveList[driveNumber] & KDriveAttRemote ) ) // avoid remote drives
                {
                TDriveInfo drvInfo;
                TInt err = aFileSession.Drive( drvInfo, driveNumber );

                if ( !err && drvInfo.iType != EMediaNotPresent )
                    {
                    fileFound = IsResourceOnDriveL(driveNumber, aFilename, aFileSession, aResourceFileName);
                    }
                }
            }
        }

	// If no *.R0* file is found, look for the .rsc file
    if (!fileFound)
        {
        TParse parse;
        parse.Set( aResourceFileName, NULL, NULL );
    
        TFindFile finder( aFileSession );
        if ( finder.FindByDir( parse.NameAndExt(), parse.Path() ) == KErrNone )
            {
        	fileFound = ETrue;
        	aFilename->Zero();
        	aFilename->Append( finder.File() );
            }
        }

    if (!fileFound)
        {
        // Cannot find file on any drive
        ret = KErrNotFound;
        }

    return ret;
    }

// -----------------------------------------------------------------------------
// PosLmFileFinder::IsResourceOnDriveL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool PosLmFileFinder::IsResourceOnDriveL(
    TInt aDrive,
    TFileName* aFilename,
    RFs& aFileSession,
    const TDesC& aResourceFileName)
    {
    TFileName* oldFilename = new (ELeave) TFileName();
    CleanupStack::PushL(oldFilename);

    TDriveUnit unit(aDrive);
    aFilename->Copy(unit.Name());
    aFilename->Append(aResourceFileName);
    oldFilename->Copy(*aFilename);
    BaflUtils::NearestLanguageFile(aFileSession, *aFilename);

    TBool fileFound = (aFilename->Compare(*oldFilename) != 0);

    CleanupStack::PopAndDestroy(oldFilename);

    return fileFound;
    }

//  End of File
