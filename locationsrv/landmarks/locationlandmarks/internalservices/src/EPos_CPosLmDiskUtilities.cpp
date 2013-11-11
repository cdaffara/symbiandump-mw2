/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Helper class for handling out of disk space situations.
*
*
*/


#include <lbs.h>
#include <centralrepository.h>
#include <UiklafInternalCRKeys.h>
#include "EPos_CPosLmDiskUtilities.h"
#include "EPos_CPosLmResourceReader.h"
#include "EPos_LandmarksInternalServicesPanic.h"

const TInt KEvaluatedMeanLmDiskSize = 1250;
const TInt KEvaluatedMeanLmCatDiskSize = 1250;
const TInt KEvaluatedMeanAddLmCatToLmDiskSize = 250;
const TInt KEvaluatedMeanRemoveLmCatFromLmDiskSize = 100;
const TInt KEmptyDatabaseSize = 8300;
const TInt KIdIndexStartSize = 1100;
const TInt KIdIndexGrowth = 9;
const TInt KFatClusterSize = 2048;

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmDiskUtilities::CPosLmDiskUtilities() :
    iCriticalDiskLevel(0)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmDiskUtilities::ConstructL()
    {
    User::LeaveIfError(iFs.Connect());

    CRepository* repository = CRepository::NewLC(KCRUidUiklaf);
    User::LeaveIfError(repository->Get(KUikOODDiskCriticalThreshold,
        iCriticalDiskLevel));

    CleanupStack::PopAndDestroy(repository);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmDiskUtilities* CPosLmDiskUtilities::NewL()
    {
    CPosLmDiskUtilities* self = new (ELeave) CPosLmDiskUtilities;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmDiskUtilities::~CPosLmDiskUtilities()
    {
    iFs.Close();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmDiskUtilities::DiskSpaceBelowCriticalLevelL(
    TInt aBytesToWrite, TChar aDrive )
    {
    // RFs allocates by clusters. Calculate the actual bytes to write according to this.
    TInt clusterSize = KFatClusterSize;

    TInt drive;
    User::LeaveIfError( iFs.CharToDrive( aDrive, drive ) );

    TVolumeIOParamInfo ioInfo;
    TInt err = iFs.VolumeIOParam( drive, ioInfo );
    if ( !err )
        {
        clusterSize = ioInfo.iClusterSize;
        }
    
    TInt bytesInLastCluster = aBytesToWrite % clusterSize;
    TInt paddedBytesToFillCluster = 0;
    if ( bytesInLastCluster > 0 )
        {
        paddedBytesToFillCluster = clusterSize - bytesInLastCluster;
        }

    TInt64 neededSpace( aBytesToWrite + paddedBytesToFillCluster );

    TVolumeInfo volumeInfo;
    User::LeaveIfError( iFs.Volume( volumeInfo, drive ) );
    TInt64 available( volumeInfo.iFree - TInt64( iCriticalDiskLevel ) );

    if ( available < neededSpace )
        {
        User::Leave( KErrDiskFull );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmDiskUtilities::EstimatedDiskSizeOfLmOperation(
    TLmOperation aOperation,
    const CPosLandmark& aLandmark)
    {
    __ASSERT_DEBUG(aOperation == EAddLandmarkOp ||
        aOperation == EUpdateLandmarkOp,
        DebugPanic(EPosLandmarkInternalServicesInvalidArgument));
    __ASSERT_DEBUG(&aLandmark,
        DebugPanic(EPosLandmarkInternalServicesInvalidArgument));

    if (aOperation == EAddLandmarkOp)
        {
        return CalculateLandmarkDisksize(aLandmark);
        }

    return Max(CalculateLandmarkDisksize(aLandmark) -
        KEvaluatedMeanLmDiskSize, 0);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmDiskUtilities::EstimatedDiskSizeOfLmOperation(
    TLmOperation aOperation,
    const CPosLandmarkCategory& aLmCategory)
    {
    __ASSERT_DEBUG(aOperation == EAddLmCategoryOp ||
        aOperation == EUpdateLmCategoryOp ||
        aOperation == ERemoveLmCategoryOp,
        DebugPanic(EPosLandmarkInternalServicesInvalidArgument));
    __ASSERT_DEBUG(&aLmCategory,
        DebugPanic(EPosLandmarkInternalServicesInvalidArgument));

    if (aOperation == EAddLmCategoryOp)
        {
        return CalculateLandmarkCategoryDisksize(aLmCategory);
        }

    // aOperation == EUpdateLmCategoryOp
    return Max(CalculateLandmarkCategoryDisksize(aLmCategory) -
        KEvaluatedMeanLmCatDiskSize, 0);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmDiskUtilities::EstimatedDiskSizeOfLmOperation(
    TLmOperation aOperation,
    const TPosLmItemId& /*aLandmarkId*/)
    {
    __ASSERT_DEBUG(aOperation == ERemoveLandmarkOp ||
        aOperation == ERemoveLmCategoryOp,
        DebugPanic(EPosLandmarkInternalServicesInvalidArgument));

    if (aOperation == ERemoveLandmarkOp)
        {
        return KEvaluatedMeanLmDiskSize;
        }

    // aOperation == ERemoveLmCategoryOp
    return KEvaluatedMeanLmCatDiskSize;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmDiskUtilities::EstimatedDiskSizeOfLmOperation(
    TLmOperation aOperation,
    const RArray<TPosLmItemId>& aLandmarkIdArray)
    {
    __ASSERT_DEBUG(aOperation == ERemoveLandmarksOp ||
        aOperation == ERemoveLmCategoriesOp,
        DebugPanic(EPosLandmarkInternalServicesInvalidArgument));

    if (aOperation == ERemoveLandmarksOp)
        {
        return aLandmarkIdArray.Count() * KEvaluatedMeanLmDiskSize;
        }

    // aOperation == ERemoveLmCategoriesOp
    return aLandmarkIdArray.Count() * KEvaluatedMeanLmCatDiskSize;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmDiskUtilities::EstimatedDiskSizeOfLmCatOperation(
    TLmOperation aOperation,
    TPosLmItemId& /*aLandmarkId*/,
    const RArray<TPosLmItemId>& aLandmarkIdArray)
    {
    __ASSERT_DEBUG(aOperation == EAddLmCategoryToLmOp ||
        aOperation == ERemoveLmCategoryFromLmOp,
        DebugPanic(EPosLandmarkInternalServicesInvalidArgument));

    if (aOperation == EAddLmCategoryToLmOp)
        {
        return aLandmarkIdArray.Count() * KEvaluatedMeanAddLmCatToLmDiskSize;
        }

    // aOperation == ERemoveLmCategoryFromLmOp
    return aLandmarkIdArray.Count() * KEvaluatedMeanRemoveLmCatFromLmDiskSize;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmDiskUtilities::EstimatedDiskSizeOfEmptyDatabase()
    {
    return KEmptyDatabaseSize;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmDiskUtilities::EstimatedDiskSizeOfIndex(
    TLmOperation aOperation,
    TInt aCountOfRecords )
    {
    __ASSERT_DEBUG( aOperation == ECreateFieldsLmIdIndex ||
        aOperation == ECreateCategoriesLmIdIndex,
        DebugPanic( EPosLandmarkInternalServicesInvalidArgument ) );

    return KIdIndexStartSize + aCountOfRecords * KIdIndexGrowth;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmDiskUtilities::CalculateLandmarkDisksize(
    const CPosLandmark& aLandmark)
    {
    // Constant for estimating disk size
    const TReal bytesToDisk = 4.4;
    // A bare landmark takes 44 bytes on disk
    const TInt bareLm = 44;
    // Adding 73 bytes when landmark have an icon.
    const TInt iconIncluded = 73;

    // Method variables
    TInt err = KErrNone;
    TReal result = 0;

    // Bare landmark
    result += bareLm * bytesToDisk;

    // Estimate location
    TLocality loc;
    err = aLandmark.GetPosition(loc);
    if (!err)
        {
        if (!TRealX(loc.Latitude()).IsNaN())
            {
            // TReal64
            result += bytesToDisk * bytesToDisk * 4;
            }
        if (!TRealX(loc.Longitude()).IsNaN())
            {
            // TReal64
            result += bytesToDisk * bytesToDisk * 4;
            }
        if (!TRealX(loc.Altitude()).IsNaN())
            {
            // TReal32
            result += bytesToDisk * bytesToDisk * 2;
            }
        if (!TRealX(loc.HorizontalAccuracy()).IsNaN())
            {
            // TReal32
            result += bytesToDisk * bytesToDisk * 2;
            }
        if (!TRealX(loc.VerticalAccuracy()).IsNaN())
            {
            // TReal32
            result += bytesToDisk * bytesToDisk * 2;
            }
        }

    // Calculate landmark coverage radius
    TReal32 real;
    err = aLandmark.GetCoverageRadius(real);
    if (!err)
        {
        result += bytesToDisk * bytesToDisk * 2;
        }

    // Calculate landmark name
    TPtrC des;
    err = aLandmark.GetLandmarkName(des);
    if (!err)
        {
        result += des.Length() * bytesToDisk * 2;
        }

    // Calculate landmark description
    err = aLandmark.GetLandmarkDescription(des);
    if (!err)
        {
        result += des.Length() * bytesToDisk * 2;
        }

    // Icon
    // Constants for adding a icon
    TInt iconIndex;
    TInt maskIndex;
    err = aLandmark.GetIcon(des, iconIndex, maskIndex);
    if (!err)
        {
        result += iconIncluded;
        }

    // Calculating position fields
    TInt nrFields = aLandmark.NumOfAvailablePositionFields();
    TPositionFieldId fid = aLandmark.FirstPositionFieldId();
    for (TInt j = 0; j < nrFields; j++)
        {
        err = aLandmark.GetPositionField(fid, des);
        if (!err)
            {
            result += des.Length() * bytesToDisk;
            }

        fid = aLandmark.NextPositionFieldId(fid);
        }

    return static_cast<TInt> (result);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmDiskUtilities::CalculateLandmarkCategoryDisksize(
    const CPosLandmarkCategory& /*aLandmark*/)
    {
    return KEvaluatedMeanLmCatDiskSize;
    }

