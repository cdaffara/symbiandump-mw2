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
*    This is a helper class for calculating progress during a search operation.
*
*/


// INCLUDE FILES
#include <e32math.h>
#include "EPos_TPosLmProgressHelper.h"

// CONSTANTS
const TInt KGuessNumOfEvaluatedRows = 16;
const TInt KGuessNumOfEvaluatedRowsNoSearchCondition = 257;
const TReal32 KTarget = 0.5;
const TReal32 KPosNumOfEvalsMinLimit = 1.0;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TPosLmProgressHelper::TPosLmProgressHelper
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TPosLmProgressHelper::TPosLmProgressHelper()
:   iEvalCounter(0),
    iGuessNumOfEvalsDivByTwo(0),
    iPreviousEvaluationProgress(0),
    iEvaluationProgress(0),
    iNumOfRowsInPreviousEvaluations(0),
    iNumOfRowsInEvaluation(0),
    iRowCounter(0)
    {
    }

// -----------------------------------------------------------------------------
// TPosLmProgressHelper::PrepareCalculateProgress
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TPosLmProgressHelper::PrepareCalculateProgress(
    TInt aNoOfRowsToEvaluate,
    TBool aSqlQueryIncludesSearchCondition)
    {
    iEvalCounter = 0;

    TInt guessNumOfEvaluatedRows = KGuessNumOfEvaluatedRows;
    if (!aSqlQueryIncludesSearchCondition)
        {
        guessNumOfEvaluatedRows = KGuessNumOfEvaluatedRowsNoSearchCondition;
        }

    TReal32 guessNumOfEvals =
        TReal32(aNoOfRowsToEvaluate) / TReal32(guessNumOfEvaluatedRows);

    if (guessNumOfEvals < KPosNumOfEvalsMinLimit)
        {
        // We always need to evaluate the view at least one time.
        guessNumOfEvals = KPosNumOfEvalsMinLimit;
        }

    iGuessNumOfEvalsDivByTwo = guessNumOfEvals * KTarget;
    }

// -----------------------------------------------------------------------------
// TPosLmProgressHelper::HandleViewPrepared
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TPosLmProgressHelper::HandleViewPrepared()
    {
    iNumOfRowsInPreviousEvaluations = 0;
    iNumOfRowsInEvaluation = 0;
    iRowCounter = 0;
    }

// -----------------------------------------------------------------------------
// TPosLmProgressHelper::HandleViewEvaluatedL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TPosLmProgressHelper::HandleViewEvaluatedL(TInt aNumOfRowsInView)
    {
    iEvalCounter++;
    iPreviousEvaluationProgress = iEvaluationProgress;
    iEvaluationProgress = CalculateEvaluationProgressL();

    iNumOfRowsInEvaluation = aNumOfRowsInView - iNumOfRowsInPreviousEvaluations;
    iNumOfRowsInPreviousEvaluations = aNumOfRowsInView;
    iRowCounter = 0;
    }

// -----------------------------------------------------------------------------
// TPosLmProgressHelper::HandleViewGetRow
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TPosLmProgressHelper::HandleViewGetRow()
    {
    iRowCounter++;
    }

// -----------------------------------------------------------------------------
// TPosLmProgressHelper::CalculateProgressL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TReal32 TPosLmProgressHelper::CalculateProgressL()
    {
    TReal32 progress = iEvaluationProgress;

    if (iRowCounter != iNumOfRowsInEvaluation)
        {
        __ASSERT_DEBUG(iNumOfRowsInEvaluation != 0,
            User::Leave(KErrDivideByZero));
        progress = iPreviousEvaluationProgress +
                   (iEvaluationProgress - iPreviousEvaluationProgress) *
                   iRowCounter / iNumOfRowsInEvaluation;
        }

    return progress;
    }

// -----------------------------------------------------------------------------
// TPosLmProgressHelper::CalculateEvaluationProgressL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TReal32 TPosLmProgressHelper::CalculateEvaluationProgressL()
    {
    // The formula 1-2^(-steps/(guessed number of steps/2)) has the following
    // characteristics:
    // * After 0 steps, the progress is 0%
    // * Progress increases for each step
    // * After half the guessed number of steps, the progress is 50%
    // * Progress will increase towards 100% but will never get there even if
    //   the number of steps passes the guessed number of steps.

    TReal target;
    User::LeaveIfError(
        Math::Pow(target,
                  TReal(2),
                  TReal(-(iEvalCounter / iGuessNumOfEvalsDivByTwo))));

    return TReal32(1 - target);
    }

// End of File
