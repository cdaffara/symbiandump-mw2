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
* Description:A local representation of the operation class assigned for operations that
* modifies the database.
*
*/


#ifndef CPOSLMLOCALMODIFYOP_H
#define CPOSLMLOCALMODIFYOP_H

#include <epos_cposlmlocaloperation.h>
#include <epos_cposlmcompactdatabase.h>

class CPosLmLocalDatabase;

/**
* A local representation of the operation class assigned for operations that
* modifies the database. It provides an interface for operations that are
* performed in small synchronous steps.
*/
NONSHARABLE_CLASS(CPosLmLocalModifyOp) : public CPosLmLocalOperation
    {

    public:  // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~CPosLmLocalModifyOp();

    public: // New functions

        /**
        * Performs a step of the operation. Before this is called, this class
        * has begun a transaction.
        *
        * @param[out] aProgress The current progress.
        */
        virtual void DoNextStepPreL( TReal32& aProgress ) = 0;

        /**
        * Performs actions after a transaction commit.
        *
        * @param[out] aProgress The current progress.
        * @return The sub class should return ETrue if it wants the base class
        *   to begin a new transaction. EFalse otherwise.
        */
        virtual TBool DoNextStepPostL( TReal32& aProgress ) = 0;

        /**
        * Returns the current progress step size. This is the value of how much
        * the progress increases between each step.
        *
        * @return Current step size.
        */
        virtual TReal32 Step() = 0;

        /**
        * Panics if the number of sub operations is invalid.
        *
        * @param aResourceId A resource id.
        * @param aNoOfSubOperations A number of sub operations.
        */
        virtual void CheckResourceValue( TInt aResourceId, TInt aNrOfSubOperations ) = 0;

    public: // From base classes

        /**
        * From CPosLmLocalOperation.
        *
        * @param[out] aProgress Will be set to the progress of the operation when
        * the step has finished.
        * @return @p KPosLmOperationNotComplete if the step has completed but
        *   more steps are needed before the operation has finished,
        *   @p KErrNone if the operation has finished successfully.
        */
        TInt NextStepL( TReal32& aProgress );

    protected:

        /**
        * C++ default constructor.
        *
        * @param[in] aDb A local database.
        */
        CPosLmLocalModifyOp( CPosLmLocalDatabase& aDb );

        /**
        * Symbian 2nd phase constructor.
        */
        void BaseConstructL();

        /**
        * Reads information about number of suboperations for asynchronous
        * operations from a resource file.
        * @param aResourceId The id of the resource to read.
        */
        void ReadInfoFromResourceFileL( TInt aResourceId );

        /**
        * The transaction is rolled back if needed.
        */
        void RollbackIfNeeded();

        /**
        * Tells whether in compacting state or not.
        * @return @p ETrue if in compacting state, @p EFalse otherwise.
        */
        TBool IsInCompactingState();

    private:

        /**
        * Calculates the progress when a compacting operation is used as a
        * sub operation to another operation.
        *
        * @param aProgressStep The progress step for the operation (i.e. 0.1 if
        *   the operation takes 10 steps).
        * @param aCompactProgress The current progress of the compact operation.
        * @return A progress to add to the current operation progress. The
        *   operation progress should be constant during the sub operation.
        *   This means that the operation progress should be saved before
        *   starting the sub operation [i.e. iCurrentProgress = iSavedProgress +
        *   CalculateSubOperationProgress(step, compactProgress)].
        */
        TReal32 CalculateSubOperationProgress(
            TReal32 aProgressStep,
            TReal32 aCompactProgress );

        /**
        * Recalculates the progress if the compacting sub operation should be
        * performed.
        *
        * @param[in,out] aProgress An operation progress.
        * @param[in] aProgressStep The progress step for the operation (i.e. 0.1 if
        *   the operation takes 10 steps).
        */
        void RecalculateProgressDueToCompact(
            TReal32& aProgress,
            TReal32 aProgressStep );

        // By default, prohibit copy constructor
        CPosLmLocalModifyOp( const CPosLmLocalModifyOp& );
        // Prohibit assigment operator
        CPosLmLocalModifyOp& operator= ( const CPosLmLocalModifyOp& );

    private:  // Data types

        enum TPosSubOperationState
            {
            ECompactSubOperationOn = 0,
            ECompactSubOperationOff
            };

    protected:  // Data

        // Status flag. Holding the current status.
        TInt iStatusFlag;

        // Number of suboperations in each transaction.
        TInt iNoOfSubOperations;
        
        // Whether this operation uses server-side data,
        // which needs transactions
        TBool iUsesServerData;

    private:    // Data

        // Compact sub operation
        TPosSubOperationState iCompactState;
        CPosLmCompactDatabase* iCompactSubOp;
        TReal32 iSavedProgress;
    };

#endif      // CPOSLMLOCALMODIFYOP_H

