/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CbsUtils class 
*                 member functions.
*
*/



// INCLUDE FILES
#include <f32file.h>
#include <barsc.h>
#include <bautils.h>
#include <sysutil.h>            // FFS critical level check
#include "CbsUtils.h"
#include <hal.h>
#include <centralrepository.h>
#include <UiklafInternalCRKeys.h>
#include <data_caging_path_literals.hrh> 
#include "CbsLogger.h"


// ================= CONSTANTS =======================

const TInt KRamMemoryCriticalLevel = 1500000;   // KRAMLOWTHRESHOLD = 1500000


// ================= MEMBER FUNCTIONS =======================


// -----------------------------------------------------------------------------
// CbsUtils::FindAndOpenDefaultResourceFileLC
// Searches and opens the DLL default resource file.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CbsUtils::FindAndOpenDefaultResourceFileLC(
    RFs& aFs, 
    RResourceFile& aResFile )
    {
    // default resource file path

	_LIT(KDirAndFile,"z:CbsServer.rsc"); 

	TParse* fp = new(ELeave) TParse(); 
	fp->Set(KDirAndFile, &KDC_RESOURCE_FILES_DIR, NULL); 

    static const TInt KDefResFileSignature = 4;

    // Find the resource file
    TFileName fileName( fp->FullName() ); 
    BaflUtils::NearestLanguageFile( aFs, fileName );

    // Open the resource file
    aResFile.OpenL( aFs, fileName );
    // Push close operation in tbe cleanup stack
    CleanupClosePushL( aResFile );

    aResFile.ConfirmSignatureL( KDefResFileSignature );

	delete fp;
    }

// -----------------------------------------------------------------------------
// CbsUtils::FFSCriticalLevelCheckL
// Checks that there is space for writing data on FFS.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CbsUtils::FFSCriticalLevelCheckL( 
    const TInt aBytesToWrite, 
    RFs& aFs )
    {
    CBSLOGSTRING("CBSSERVER: >>> CbsUtils::FFSCriticalLevelCheckL()");
    
    // Checks RAM memory critical level.
    if (RamMemCriticalLevelCheckL( aBytesToWrite ))
        {
        User::Leave( KErrDiskFull );    // returns KErrDiskFull -> no changes to clients
        }

    // Checks RAM disk critical level
    if ( SysUtil::FFSSpaceBelowCriticalLevelL( &aFs, aBytesToWrite ) )
        {
        User::Leave( KErrDiskFull );
        }
        
    CBSLOGSTRING("CBSSERVER: <<< CbsUtils::FFSCriticalLevelCheckL()");
    }

// -----------------------------------------------------------------------------
// CbsUtils::RamMemCriticalLevelCheckL
// Checks that there is space for allocating data in memory.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CbsUtils::RamMemCriticalLevelCheckL( 
    const TInt aBytesToWrite)
    {
    
    // Read the critical threahold RAM memory level from the repository.
    TInt thresholdVal;
    CRepository* repository = CRepository::NewLC( KCRUidUiklaf );
    if (repository->Get( KUikOOMRamLowThreshold, thresholdVal ) != KErrNone)
        {
        thresholdVal = KRamMemoryCriticalLevel;
        }
    CleanupStack::PopAndDestroy( repository );

    TInt freeMem = 0;
    HAL::Get(HAL::EMemoryRAMFree, freeMem);
    if ( freeMem > (thresholdVal + aBytesToWrite) )
        {
        return EFalse;
        }
  
    return ETrue;
    }
// -----------------------------------------------------------------------------
// CbsUtils::VolumeCriticalLevelCheckL
// Leaves with KErrDiskFull if writing aBytesToWrite bytes 
// to the RAM File System would reduce the free space on RAM 
// under the critical level.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CbsUtils::VolumeCriticalLevelCheckL( 
    const TDesC& aFileName, 
    TInt aBytesToWrite, 
    const RFs& aFs )
    {
    // Convert file name to a volume number
    TInt volumeNumber;
    aFs.CharToDrive( aFileName[0], volumeNumber );

    // Find out if the volume has enough free space
    TVolumeInfo volume;
    User::LeaveIfError( aFs.Volume( volume, volumeNumber ) );
    if ( volume.iFree < aBytesToWrite )
        {
        User::Leave( KErrDiskFull );
        }
    }

// -----------------------------------------------------------------------------
// CbsUtils::ExistsL
// Returns ETrue, if the file aFile exists.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CbsUtils::ExistsL( 
    const RFs& aFs, 
    const TDesC& aFile )
    {
    TParse filename;
    aFs.Parse( aFile, filename );

    TBool returnCode( EFalse );
	TUint attributes;

    TInt result( aFs.Att( filename.FullName(), attributes ) );
    if ( result == KErrNone )
        {
        returnCode = ETrue;
        }
    else if ( result == KErrNotFound ) 
        {
        returnCode = EFalse;
        }
    else 
        {
        User::Leave( result );
        }
    return returnCode;  
    }

// -----------------------------------------------------------------------------
// CbsUtils::DeleteFileL
// Deletes the given file.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CbsUtils::DeleteFileL( 
    RFs& aFs, 
    const TDesC& aFile )
    {
    TInt result( aFs.Delete( aFile ) );
    if ( result != KErrNone && 
        result != KErrNotFound && 
        result != KErrPathNotFound )
        {
        User::Leave( result );
        }
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// End of File


