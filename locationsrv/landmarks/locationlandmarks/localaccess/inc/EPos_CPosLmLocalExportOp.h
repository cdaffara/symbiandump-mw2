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
* Description: Operation for exporting landmarks from a database.
*
*
*/


#ifndef CPOSLMLOCALEXPORTOP_H
#define CPOSLMLOCALEXPORTOP_H

//  INCLUDES
#include <EPos_CPosLandmarkDatabase.h>
#include "EPos_CPosLmLocalOperation.h"

// CONSTANTS

// FORWARD DECLARATIONS
class CPosLmLocalDatabase;
class CPosLmGlobalCategoryReader;
class CPosLandmarkEncoder;

// CLASS DECLARATION

/**
* Operation for exporting landmarks from a database.
*
* @lib eposlmlocalaccess.lib
*/
NONSHARABLE_CLASS(CPosLmLocalExportOp) : public CPosLmLocalOperation
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aDb A landmark database.
        * @param aLandmarkEncoder A landmark encoder object.
        * @param aLandmarkIdArray The landmarks which should be exported.
        * @param aTransferOptions A bitmap representing the options for the
        *   export operation. The bitmap values are defined by
        *   @ref _TTransferOptions.
        * @return A new instance of this class.
        */
        static CPosLmLocalExportOp* NewL(
        /* IN  */       CPosLmLocalDatabase& aDb,
        /* IN  */       CPosLandmarkEncoder& aLandmarkEncoder,
        /* IN  */       const RArray<TPosLmItemId>& aLandmarkIdArray,
        /* IN  */       CPosLandmarkDatabase::TTransferOptions aTransferOptions
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmLocalExportOp();

    public: // Functions from base classes

        /**
        * From CPosLmLocalOperation;
        *
        * @param aProgress Will be set to the progress of the operation when
        * the step has finished.
        * @return @p KPosLmOperationNotComplete if the step has completed but
        *   more steps are needed before the operation has finished,
        *   @p KErrNone if the operation has finished successfully.
        */
        TInt NextStepL(
        /* OUT */   TReal32& aProgress
        );

        /**
        * From CPosLmLocalOperation;
        *
        * @param aError An error code generated by NextStepL.
        */
        void HandleError(
        /* IN/OUT */    TInt& aError
        );

    private:

        /**
        * C++ default constructor.
        * @param aDb A landmark db.
        */
        CPosLmLocalExportOp(
        /* IN  */       CPosLmLocalDatabase& aDb,
        /* IN  */       CPosLandmarkEncoder& aLandmarkEncoder,
        /* IN  */       CPosLandmarkDatabase::TTransferOptions aTransferOptions
        );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(
        /* IN  */       const RArray<TPosLmItemId>& aLandmarkIdArray
        );

        CPosLandmark* ReadLmFromDbLC(
        /*  IN  */      TPosLmItemId aId
        );

        CPosLandmarkCategory* ReadCategoryFromDbLC(
        /*  IN  */      TPosLmItemId aId
        );

        void SetPredefinedGlobalNameL(
        /*  IN/OUT */   CPosLandmarkCategory* aCategory
        );

        /**
        * Reads information about number of suboperations for asynchronous
        * operations from a resource file.
        */
        void ReadInfoFromResourceFileL();

        // Prohibit copy constructor if not deriving from CBase.
        CPosLmLocalExportOp(const CPosLmLocalExportOp&);
        // Prohibit assigment operator if not deriving from CBase.
        CPosLmLocalExportOp& operator=(const CPosLmLocalExportOp&);

    private:    // Data

        // Landmark global category reader.
        CPosLmGlobalCategoryReader* iReader;

        // Landmark encoder (no ownership).
        CPosLandmarkEncoder* iLandmarkEncoder;

        // Transfer options
        CPosLandmarkDatabase::TTransferOptions iTransferOptions;

        // Status flag. Holding the current status.
        TInt iStatusFlag;

        // Export landmark in next increment if ETrue, otherwise
        // export categories.
        TBool iCurrentIsLm;

        // Item list with landmarks to export.
        RArray<TPosLmItemId> iLandmarkIdArray;

        // The current landmark handled in the id array.
        TInt iCurrentLm;

        // Item list with categories to export.
        RArray<TPosLmItemId> iCategoryIdArray;

        // The current category handled in the id array.
        TInt iCurrentCategory;

        // Number of suboperations in each increment that export categories.
        TInt iNrOfSubOperations;
    };

#endif      // CPOSLMLOCALEXPORTOP_H

// End of File