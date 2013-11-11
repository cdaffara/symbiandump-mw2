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
* Description: 
*     See class description below
*
*/


#ifndef TPOSLMPROGRESSHELPER_H
#define TPOSLMPROGRESSHELPER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  This is a helper class for calculating progress during a search operation.
*
*  @version $Revision: 1.4 $, $Date: 2005/02/15 13:36:46 $
*/
class TPosLmProgressHelper
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        TPosLmProgressHelper();

    public: // New functions

        /**
        * Prepares for calculation of search progress.
        *
        * @param aNoOfRowsToEvaluate The number rows in the table(s) to search.
        * @param aSqlQueryIncludesSearchCondition @p ETrue if the db query
        *   will include a condition of which rows in a table that matches,
        *   @p EFalse if the db query will get all rows in a table.
        */
        void PrepareCalculateProgress(
        /* IN  */       TInt aNoOfRowsToEvaluate,
        /* IN  */       TBool aSqlQueryIncludesSearchCondition = ETrue
        );

        void HandleViewPrepared();
        void HandleViewEvaluatedL(TInt aNumOfRowsInView);
        void HandleViewGetRow();

        /**
        * Calculates the progress of a search.
        *
        * @return The progress of the search.
        */
        TReal32 CalculateProgressL();

    private: // New functions

        TReal32 CalculateEvaluationProgressL();

    private:     // Data

        TInt iEvalCounter;

        TReal32 iGuessNumOfEvalsDivByTwo;

        TReal32 iPreviousEvaluationProgress;
        TReal32 iEvaluationProgress;

        TInt iNumOfRowsInPreviousEvaluations;
        TInt iNumOfRowsInEvaluation;

        TInt iRowCounter;
    };

#endif  // TPOSLMPROGRESSHELPER_H

// End of File
