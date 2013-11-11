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
* Description: Operation to search for landmarks or landmark categories from 
*							 multiple databases
*
*/


#ifndef CPOSLMMULTIDBSEARCHOPERATION_H
#define CPOSLMMULTIDBSEARCHOPERATION_H

//  INCLUDES
#include <EPos_CPosLmOperation.h>
#include "EPos_CPosLmMultiDbSortPref.h"

// FORWARD DECLARATION
class CPosLmMultiDbSearch;
class CPosLmSearchCriteria;
class CPosLmActiveOperation;

// CLASS DECLARATION

/**
* Operation to search for landmarks or landmark categories from multiple
* databases.
*/
NONSHARABLE_CLASS(CPosLmMultiDbSearchOperation) : public CPosLmOperation
    {
    public:  // Data types

        enum TPosLmOperationStatus
        {
        EOperationNotStarted = 0,
        EAsyncStarted,
        ESyncStarted,
        EOperationCompleted
        };

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aMultiDbSearch A pointer to the multi db search object.
        * @param aCriteria The search criteria.
        * @param aSortPref A pointer to the sort preference object. This class
        *    takes ownership.
        * @param aSearchOnlyPreviousMatches This flag may be used to perform a
        *   search within the results of previous search.
        * @return A new instance of this class.
        */
        static CPosLmMultiDbSearchOperation* NewL(
        /* IN  */       CPosLmMultiDbSearch* aMultiDbSearch,
        /* IN  */       const CPosLmSearchCriteria& aCriteria,
        /* IN  */       CPosLmMultiDbSortPref* aSortPref,
        /* IN  */       TBool aSearchOnlyPreviousMatches
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmMultiDbSearchOperation();

    public: // New functions

        /**
        * This function must be called by the CPosLmMultiDbSearch class if it is
        * deleted before the search operation has completed.
        */
        void HandleMultiDbSearchDeleted();

        /*
        * The callback function that is called from a CPosLmActiveOperation when
        * an asynchronous step has been executed.
        *
        * @param aPtr A pointer to the CPosLmActiveOperation where a step has
        *   been executed.
        * @return Always returns 0.
        */
        static TInt OperationCallBack(TAny* aPtr);

    public: // From base classes

        /**
        * From CPosLmOperation.
        *
        * Performs one step of the operation asynchronously.
        *
        * @param aStatus The request status. Will be completed when the step
        *   has been performed. The request status will be @p KRequestPending
        *   if the step has not completed. It will be
        *   @p KPosLmOperationNotComplete if the step has completed but more
        *   steps are needed before the operation has finished. The request
        *   status will be @p KErrNone if the operation has finished
        *   successfully.
        * @param aProgress Will be set to the progress of the operation when the
        *   step has finished.
        */
        void NextStep(
        /* OUT */   TRequestStatus& aStatus,
        /* OUT */   TReal32& aProgress
        );

        /**
        * From CPosLmOperation.
        *
        * Synchronous execution of the operation.
        *
        * When this function returns, the operation has successfully finished.
        */
        void ExecuteL();

    private:

        /**
        * C++ default constructor.
        */
        CPosLmMultiDbSearchOperation(
        /* IN  */       CPosLmMultiDbSearch* aMultiDbSearch,
        /* IN  */       TBool aSearchOnlyPreviousMatches
        );

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL(
        /* IN  */       const CPosLmSearchCriteria& aCriteria,
        /* IN  */       CPosLmMultiDbSortPref* aSortPref
        );

        void HandleOperationCompleted();
        void CompleteClientRequest();

        void IncrementSearchStep();
        void IncrementIndexAndSearchStatus();
        void IncrementSearchStatus();

        void ExecuteSyncStep();
        void ExecuteSyncStepL();

        CPosLmOperation* OpenDbAndCreateInitOpL();
        CPosLmOperation* CreateSearchOpL();
        void AppendAsActiveOperationL(CPosLmOperation* aOperation);

        void ExecuteAsyncStep();

        TInt DoOperationCallBack(CPosLmActiveOperation* aActiveOp);

        void CalculateProgress();
        TReal32 CalculateSyncStepProgress();
        TReal32 CalculateAsyncStepProgress();

    private:  // Data types

        enum TPosLmMultiAsyncSearchStatus
        {
        ESearchNotStarted = 0,
        EOpenDbs,
        EInitDbs,
        ECreateSearchOps,
        ESearchDbs,
        ESearchCompleted
        };

    private:    // Data

        CPosLmMultiDbSearch*                    iMultiDbSearch; // No ownership
        CPosLmSearchCriteria*                   iCriteria;
        TBool                                   iSearchOnlyPreviousMatches;
        CPosLmMultiDbSortPref*                  iSortPref;

        TUint                                   iCurrentDbIndex;

        TPosLmOperationStatus                   iOperationStatus;

        TPosLmMultiAsyncSearchStatus            iSearchStatus;
        TReal32                                 iProgress;

        TRequestStatus*                         iClientStatus;
        TReal32*                                iClientProgress;

        RPointerArray<CPosLmActiveOperation>    iActiveOperations;
    };

#endif      // CPOSLMMULTIDBSEARCHOPERATION_H

// End of File
