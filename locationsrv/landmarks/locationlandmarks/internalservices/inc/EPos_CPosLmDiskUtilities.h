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
*/


#ifndef CPOSLMDISKUTILITIES_H
#define CPOSLMDISKUTILITIES_H

#include <e32base.h>
#include <f32file.h>
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkCategory.h>

/**
*
*   Helper class for handling out of disk space situations.
*
*/
class CPosLmDiskUtilities : public CBase
    {
    public:

        enum TLmOperation
            {
            EAddLandmarkOp,
            /**< Add landmark operation. */
            EUpdateLandmarkOp,
            /**< Update landmark operation. */
            ERemoveLandmarkOp,
            /**< Remove landmark operation. */
            ERemoveLandmarksOp,
            /**< Remove landmarks operation. */
            ERemoveAllLandmarksOp,
            /**< Remove all landmarks operation. */
            EAddLmCategoryOp,
            /**< Add landmark category operation. */
            EUpdateLmCategoryOp,
            /**< Update landmark category operation. */
            ERemoveLmCategoryOp,
            /**< Remove landmark category operation. */
            ERemoveLmCategoriesOp,
            /**< remove landmark categories operation. */
            EAddLmCategoryToLmOp,
            /**< Add landmark category to landmark operation. */
            ERemoveLmCategoryFromLmOp,
            /**< Remove landmark category from landmark operation. */
            ECreateFieldsLmIdIndex,
            /**< Create index in fields table on Lm ID field */
            ECreateCategoriesLmIdIndex,
            /**< Create index in categories table on Lm ID field */
            };

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CPosLmDiskUtilities* NewL();

        /**
        * Destructor.
        */
        virtual ~CPosLmDiskUtilities();

    public: // New functions

        /**
        * Method to check if free drive storage
        * space is or will fall below Critical Level (CL).
        * The CL is defined by this module.
        *
        * Method will leave with @p KErrDiskFull if disk size is/or will
        * decrease below critical level.
        *
        * @param[in] aBytesToWrite Number of bytes the caller is about to add.
        *   The value 0 checks if the current space is already below the CL.
        * @param[in] aDrive Character of the drive to be checked.
        */
        IMPORT_C void DiskSpaceBelowCriticalLevelL(
            TInt  aBytesToWrite,
            TChar aDrive );

        /**
        * Method to estimate disk size impact for a landmark operation.
        *
        * Method for operations, EAddLandmarkOp and EUpdateLandmarkOp.
        * Otherwise method will raise a debug panic.
        *
        * @param[in] aOperation Type of landmark operation.
        * @param[in] aLandmark Landmark to be estimated.
        * @return Estimated number of bytes the operation will
        *         take on disk.
        */
        IMPORT_C TInt EstimatedDiskSizeOfLmOperation(
            TLmOperation aOperation,
            const CPosLandmark& aLandmark );

        /**
        * Method to estimate disk size impact for a landmark operation.
        *
        * Method for operations, EAddLmCateogoryOp and EUpdateLmCategoryOp.
        * Otherwise method a will raise a debug panic.
        *
        * @param[in] aOperation Type of landmark operation.
        * @param[in] aLmCategory Landmark category to be estimated.
        * @return Estimated number of bytes the operation will
        *         take on disk.
        */
        IMPORT_C TInt EstimatedDiskSizeOfLmOperation(
            TLmOperation aOperation,
            const CPosLandmarkCategory& aLmCategory );

        /**
        * Method to estimate disk size impact for a landmark operation.
        *
        * Method for operations, ERemoveLandmarkOp and ERemoveLmCategoryOp.
        * Otherwise method a will raise a debug panic.
        *
        * @param[in] aOperation Type of landmark operation.
        * @param[in] aLandmarkId Landmark item id to be estimated.
        * @return Estimated number of bytes the operation will
        *         take on disk.
        */
        IMPORT_C TInt EstimatedDiskSizeOfLmOperation(
            TLmOperation aOperation,
            const TPosLmItemId& aLandmarkId );

        /**
        * Method to estimate disk size impact for a landmark operation.
        *
        * Method for operations, ERemoveLandmarksOp and ERemoveLmCategoriesOp.
        * Otherwise method a will raise a debug panic.
        *
        * @param[in] aOperation Type of landmark operation.
        * @param[in] aLandmarkIdArray Landmark item id's to be estimated.
        * @return Estimated number of bytes the operation will
        *         take on disk.
        */
        IMPORT_C TInt EstimatedDiskSizeOfLmOperation(
            TLmOperation aOperation,
            const RArray<TPosLmItemId>& aLandmarkIdArray );

        /**
        * Method to estimate disk size impact for a landmark operation.
        *
        * Method for operations, EAddLmCategoryToLmOp and
        *   ERemoveLmCategoryFromLmOp.
        * Otherwise method a will raise a debug panic.
        *
        * @param[in] aOperation Type of landmark operation.
        * @param[in] aLandmarkId Landmark item id's to be estimated.
        * @param[in] aLandmarkIdArray Landmark item id's to be estimated.
        * @return Estimated number of bytes the operation will
        *         take on disk.
        */
        IMPORT_C TInt EstimatedDiskSizeOfLmCatOperation(
            TLmOperation aOperation,
            TPosLmItemId& aLandmarkId,
            const RArray<TPosLmItemId>& aLandmarkIdArray );

        /**
        * Method to estimate disk size impact for an empty landmark database.
        *
        * @return Estimated number of bytes.
        */
        IMPORT_C TInt EstimatedDiskSizeOfEmptyDatabase();
        
        /**
        * Method to estimate disk size impact for creating
        * foreign index in a table.
        * @return Estimated number of bytes.
        */
        IMPORT_C TInt EstimatedDiskSizeOfIndex(
            TLmOperation aOperation,
            TInt aCountOfRecords );

    protected:

        /**
        * C++ default constructor.
        */
        CPosLmDiskUtilities();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Method for estimating disk size for a landmark.
        *
        * @param aLandmarkCategory Landmark category to estimate.
        * @return Number of bytes the estimated landmark would take.
        */
        TInt CalculateLandmarkDisksize( const CPosLandmark& aLandmark );

        /**
        * Method for estimating disk size for a landmark cagegory.
        *
        * @param aLandmarkCategory Landmark category to estimate.
        * @return Number of bytes the estimated landmark category
        *         would take.
        */
        TInt CalculateLandmarkCategoryDisksize(
            const CPosLandmarkCategory& aLandmarkCategory );

    private:

        // By default, prohibit copy constructor
        CPosLmDiskUtilities( const CPosLmDiskUtilities& );
        // Prohibit assigment operator
        CPosLmDiskUtilities& operator= ( const CPosLmDiskUtilities& );

    private:

        // File session
        RFs         iFs;

        // Level for critical disk space
        TInt        iCriticalDiskLevel;

    };

#endif      // CPOSLMINTERALSERVICES_H

