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
* Description: Operation for reading partial landmarks.
*
*
*/

#ifndef CPOSLMLOCALREADPARTIALLMOP_H
#define CPOSLMLOCALREADPARTIALLMOP_H

//  INCLUDES
#include "EPos_CPosLmLocalOperation.h"
#include <EPos_Landmarks.h>

// FORWARD DECLARATIONS
class CPosLandmark;
class CPosLmLocalDatabase;
class CPosLmPartialReadParameters;

// CLASS DECLARATION

/**
* Operation for reading partial landmarks. This operation can be
* either synchronous or asynchronous.
*
*  @lib eposlmlocalaccess.lib
*/
NONSHARABLE_CLASS(CPosLmLocalReadPartialLmOp) : public CPosLmLocalOperation
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aDb A landmark database.
        * @param aLandmarkIdArray An array with IDs of the landmarks to read.
        * @param aPartialSettings The partial read parameter settings.
        */
        static CPosLmLocalReadPartialLmOp* NewL(
        /* IN */        CPosLmLocalDatabase& aDb,
        /* IN */        const RArray<TPosLmItemId>& aLandmarkIdArray,
        /* IN */        CPosLmPartialReadParameters* aPartialSettings
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmLocalReadPartialLmOp();

    public: // Functions from base classes

        /**
        * From CPosLmLocalOperation
        * Synchronous incremental execution of the operation. Performs a single
        * step of the operation synchronously.
        *
        * Leaves with an error code if something goes wrong.
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
        * From CPosLmLocalOperation
        * Handles any error generated by NextStepL. Will be called when
        * NextStepL leaves. The error code is allowed to be changed, if
        * necessary.
        *
        * @param aError An error code generated by NextStepL.
        */
        void HandleError(
        /* IN/OUT */    TInt& aError
        );

    public:     // New functions

        /**
        * Returns an array containing partial landmarks.
        * @returns An array containing the prepared partial landmark objects.
        */
        CArrayPtr<CPosLandmark>* GetResultL();

    private:

        /**
        * C++ default constructor.
        * @param aDb A landmark db.
        */
        CPosLmLocalReadPartialLmOp(
        /* IN */        CPosLmLocalDatabase& aDb
        );

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aLandmarkIdArray An array with IDs of the landmarks to read.
        * @param aPartialSettings The partial read parameter settings.
        */
        void ConstructL(
        /* IN */        const RArray<TPosLmItemId>& aLandmarkIdArray,
        /* IN */        CPosLmPartialReadParameters* aPartialSettings
        );

        void AddLandmarkToArrayL(TInt aIndex);

        void ReadInfoFromResourceFileL();

        // Prohibit copy constructor if not deriving from CBase.
        CPosLmLocalReadPartialLmOp(
            const CPosLmLocalReadPartialLmOp&);
        // Prohibit assigment operator if not deriving from CBase.
        CPosLmLocalReadPartialLmOp& operator=(
            const CPosLmLocalReadPartialLmOp&);


    private:    // Data

        CArrayPtr<CPosLandmark>* iLmArray;
        RArray<TPosLmItemId> iLandmarkIdArray;

        CPosLmPartialReadParameters* iPartialSettings;

        TInt iStatusFlag;
        TInt iCurrent;
        TInt iRequestCounter;

    };

#endif      // CPOSLMLOCALREADPARTIALLMOP_H

// End of File
