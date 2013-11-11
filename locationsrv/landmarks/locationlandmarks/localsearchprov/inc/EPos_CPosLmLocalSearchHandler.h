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
* Description: Base class for criteria based searches.
*
*/


#ifndef CPOSLMLOCALSEARCHHANDLER_H
#define CPOSLMLOCALSEARCHHANDLER_H

#include <d32dbms.h>
#include <EPos_CPosLmSearchCriteria.h>
#include <epos_rposlmlocalnameindex.h>
#include "EPos_TPosLmLocalSortPref.h"
#include "EPos_TPosLmProgressHelper.h"

const TReal32 KProgressCompleted = 1.0;
const TReal32 KPosUndefinedDistance = -1;

class CPosLmDisplayItem;
class CPosLmDisplayData;
class CPosLmLocalDbAccess;
class CPosLmNameIndexIterator;
class CPosLmLocalDatabase;

/**
*  Base class for criteria based searches.
*/
class CPosLmLocalSearchHandler : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~CPosLmLocalSearchHandler();

    public: // New functions

        /**
        * Factory method for creating an instance of the
        * CPosLmLocalSearchHandler subclass that corresponds to the search
        * criteria.
        *
        * The caller takes ownership of the returned object.
        *
        * @param[in] aDbAccess Access handle to the database to search in.
        * @param[in] aCriteria The search criteria.
        * @param[in] aSortPref A sort preference object.
        * @param[in] aDisplayData The displayable data. NOTE: Ownership is
        *   not transferred.
        * @param aMaxNumOfMatches The max number of items that will be
        *   searched for.
        * @return An instance of a CPosLmLocalSearchHandler subclass.
        */
        static CPosLmLocalSearchHandler* CreateHandlerL(
            CPosLmLocalDatabase& aDatabase,
            const CPosLmSearchCriteria& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            CPosLmDisplayData* aDisplayData,
            TInt aMaxNumOfMatches );

        /**
        * Restricts the next search to only include matches that also are
        * included in this array.
        *
        * @param[in] aPreviousMatches An array of item ids.
        */
        void SetPreviousMatchesToSearchL(
                const RArray<TPosLmItemId>& aPreviousMatches );

        /**
        * Checks if this search handler is valid for landmark searching.
        *
        * @return EFalse unless overriden in derived class.
        */
        virtual TBool IsValidForLandmarkSearch() const;

        /**
        * Checks if this search handler is valid for category searching.
        *
        * @return EFalse unless overriden in derived class.
        */
        virtual TBool IsValidForCategorySearch() const;

        /**
        * Synchronous incremental execution of the search. Performs a single
        * step of the search synchronously.
        *
        * Leaves with an error code if something goes wrong.
        *
        * @param[out] aProgress Will be set to the progress of the search when
        * the step has finished.
        * @return @p KPosLmOperationNotComplete if the step has completed but
        *   more steps are needed before the search has finished,
        *   @p KErrNone if the search has finished successfully.
        */
        virtual TInt NextStepL( TReal32& aProgress ) = 0;

        /**
        * This function must be called if any further searching is cancelled.
        * (I.e. NextStepL has either leaved or if the search operation is
        * deleted before NextStepL has returned @p KErrNone.)
        */
        virtual void CancelSearch();

        /**
        * Get matches from a search. If the search is not completed the
        * matches encountered so far will be returned.
        *
        * @param[out] aMatches Array containing the landmark item IDs found
        *   in the search. The caller is responsible to close the array.
        */
        virtual void GetMatchesL( RArray<TPosLmItemId>& aMatches ) const;

        /**
        * Get matches from a search. If the search is not completed the
        * matches encountered so far will be returned.
        *
        * @return Array containing the landmark item IDs found
        *   in the search. Do NOT call Close() or Reset().
        */
        virtual const RArray<TPosLmItemId>& Matches() const;

    protected:

        /**
        * C++ default constructor.
        *
        * @param[in] aDbAccess Access handle to the database to search in.
        * @param[in] aSortPref A sort preference object.
        * @param[in] aMaxNumOfMatches The max number of items that will be
        *   searched for.
        * @param[in] aIsLast Should be set to @p ETrue if this search handler
        *   is the last search handler as a part of a composite search or
        *   if it is not part of a composite search at all, otherwise @p EFalse.
        */
        CPosLmLocalSearchHandler(
            CPosLmLocalDatabase& aDatabase,
            CPosLmSearchCriteria::TCriteriaType aCriteriaType,
            const TPosLmLocalSortPref& aSortPref,
            TInt aMaxNumOfMatches );

        /**
        * Set display data.
        * Only allowed to call this function once, typically in ConstructL.
        *
        * @param[in,out] aData The displayable data. Ownership not transferred.
        */
        void SetDisplayDataL( CPosLmDisplayData* aData );

        /**
        * Tells if display data has been set.
        * @return @p ETrue if display data has been set otherwise @p EFalse.
        */
        TBool HasDisplayData() const;

        /**
        * @return Reference to display data object.
        */
        CPosLmDisplayData& DisplayData();
        const CPosLmDisplayData& DisplayData() const;

        /**
        * Get current matches from a search. If the search is not completed the
        * matches encountered so far will be returned.
        *
        * @return Array containing the landmark item IDs found
        *   in the search. Do NOT call Close() or Reset().
        */
        RArray<TPosLmItemId>& CurrentMatches();

        /**
        * Retrieves the maximum number of search matches.
        *
        * @return The maximum number of search matches or
        *   @p KPosLmMaxNumOfMatchesUnlimited if unlimited.
        */
        TInt MaxNumOfMatches() const;
        
        /**
        * Checks whether maximum number of matches have been found.
        */
        TBool IsMaxMatchesFound() const;

        /**
        * Returns the sort preference.
        * @return The sort preference.
        */
        const TPosLmLocalSortPref& SortPref() const;

        /**
        * Returns previous matches if set.
        *
        * @return Array containing the landmark item IDs set as previous
        *   matches.
        */
        const RArray<TUint>& PreviousMatches() const;

        /**
        * Checks if a landmark id matches the previous matches array if set.
        *
        * If SetPreviousMatchesToSearchL has NOT been called, this method will
        * always return @p ETrue.
        *
        * If SetPreviousMatchesToSearchL has been called, this method will
        * return @p ETrue only if included in the previous matches, otherwise
        * @p EFalse.
        *
        * @param aCurrentMatch Landmark item id for the current match.
        * @return See above.
        */
        TBool MatchesPreviousMatches( TPosLmItemId aCurrentMatch ) const;

        /**
        * Adds new match from an ongoing search.
        *
        * If SetPreviousMatchesToSearchL has been called, this match will be
        * added only if included in the previous matches.
        *
        * @param aCurrentMatch Landmark item id for the current match.
        * @param aDataRow Pointer to a row, which contains landmark or category
        * 	data. Forwarded o display data.
        */
        void AddMatchL( TPosLmItemId aMatchId, RDbRowSet* aDataRow = NULL );

        /**
        * Appends sort order to a db query.
        * @param[in,out] The db query.
        */
        void AppendSortToQueryL( HBufC* aDbQuery );

        /**
        * Prepares a view for a search.
        *
        * @param[in,out] aView The view to prepare.
        * @param[in] aDbQuery The database query.
        * @param[in] aTextComparison The type of text comparison.
        */
        void PrepareViewL(
            RDbView& aView,
            const TDesC& aDbQuery,
            TDbTextComparison aTextComparison = EDbCompareCollated );

        /**
        * Evaluates a single step of given view if needed.
        * Increments the evaluation counter (needed for progress calculation).
        */
        void EvaluateViewL( RDbView& aView );

        /**
        * Reads the next row in a view.
        * @param[in,out] aView The view to read.
        * @return ETrue if there are more rows in the view. EFalse otherwise.
        */
        TBool MoreInView( RDbView& aView ) const;

        /**
        * Gets the current row data of given view for access.
        * Increments the row counter (needed for progress calculation).
        * @param[in] aView A view.
        */
        void GetRowL( RDbRowSet& aView );

        /**
        * Performs a stepwise synchronous search.
        *
        * It is assumed that the caller of this method does not call
        * this method again when max number of items have been reached
        * (i.e. @p ETrue was returned from the previous call).
        *
        * @param aMatchView A view, which holds match IDs
        * @param aMatchIdColumn Index of the ID column in aMatchView
        * @param aUseMatchViewForDisplayData If ETrue, then aMatchView can be
        *   used to retrieve display data.
        * 
        * @return @p ETrue if maxNumOfMatches has been reached after
        *   this search, otherwise @p EFalse.
        */
        TBool SearchViewL(
            RDbView& aMatchView,
            TInt aMatchIdColumn,
            TBool aUseMatchViewForDisplayData = ETrue );

        /**
        * Calculates progress and search status.
        * Should be called last in NextStepL.
        *
        * @param[in] aSearchFinished Should be set to @p EFalse if more steps are
        *   needed before the search has finished, otherwise @p ETrue.
        * @param[out aProgress Will be set to the progress of the search when
        * the step has finished.
        * @return @p KPosLmOperationNotComplete if the step has completed but
        *   more steps are needed before the search has finished,
        *   @p KErrNone if the search has finished successfully.
        */
        TInt SearchStepCompletedL( TBool aSearchFinished, TReal32& aProgress );

        /**
        * Gets the row count of aTable.
        * @param[in] aTable The table.
        * @return The number of rows.
        */
        TInt GetRowCountL( const TDesC& aTable );

        /**
        * Prepares for calculation of search progress.
        *
        * @param aNoOfRowsToEvaluate The number rows in the table(s) to search.
        * @param aSqlQueryIncludesSearchCondition @p ETrue if the db query
        *   will include a condition of which rows in a table that matches,
        *   @p EFalse if the db query will get all rows in a table.
        */
        void PrepareCalculateProgress(
            TInt aNoOfRowsToEvaluate,
            TBool aSqlQueryIncludesSearchCondition = ETrue );

        /**
        * Calculates the progress of a search.
        * @return The progress of the search.
        */
        TReal32 CalculateProgressL();

        /**
        * Adds a display item to the display data using the prefered sort order.
        *
        * @param aCurrentMatch The item id of the item add to the display data.
        * @param aDistance The distance to be used if the displayable item
        *   should be inserted in distance order.
        *
        * @return The position of the added match.
        */
        TInt AddMatchToDisplayDataL(
            TPosLmItemId aCurrentMatch,
            RDbRowSet* aView = NULL,
            TBool aSuppressSorting = EFalse,
            TReal32 aDistance = KPosUndefinedDistance );
        
        /** Opend LM table and landmarks index */
        void PrepareLandmarkIndexL();
        
        /* Loads more landmarks from index if needed */
        void EvaluateLandmarkIndexL();
        
        /** Loads all items of landmark name index. 
         *  The index should be in EModeFull mode to keep the loaded items. */
        void LoadFullLandmarkIndexL();

        /** Checks whether landmark index has more items */
    	TBool MoreInLandmarkIndex() const; 
               
        /** Finds row in Lm table, by given ID */
        TBool GetLandmarkRowL( TPosLmItemId aLmid );
        
        /**
        * Creates a new display item to be added to the display data.
        *
        * @param aMatchId The item id of the item to create.
        * @param aRow A row, which contains item data 
        * @return A displayable item.
        */
        CPosLmDisplayItem* CreateItemLC( 
        	TPosLmItemId aMatchId,
        	RDbRowSet* aRow = NULL );
        
        /** Returns ETrue if current amount of items in 
         *  display data is equal or greater than given
         *  initial amount. Used to break search loops in more steps,
         *  than Evaluate does,
         *  because reading display data is a long operation.
         *  Always returns EFalse if display data is not use */
        TBool BreakOnDisplayData( TInt aInitialDdMatches ) const;
        
        /** Returns count of display data items, or 0 if no display data used */
        TInt DisplayDataCount() const;
        
        /** Closes view, table and index iterator */
        void CleanupSources();

    private:

        /** Loads more landmarks from index */
        void GetMoreSortedLandmarksL();

        // By default, prohibit copy constructor
        CPosLmLocalSearchHandler( const CPosLmLocalSearchHandler& );
        // Prohibit assigment operator
        CPosLmLocalSearchHandler& operator= ( const CPosLmLocalSearchHandler& );

    protected:  // Data

        // The view used during the search
        RDbView iView;

        // A table used during the search
        RDbTable iTable;
        
        // If ETrue, new display data items are appended,
        // regardless of current sorting preference.
        // Used when data come pre-sorted from source view or table
        TBool iSuppressDisplayDataSorting;

        // Landmark Name index iterator
        CPosLmNameIndexIterator* iLandmarkIndex;
        
        // Array containing results from current search
        RArray<TPosLmItemId> iCurrentMatches;

    private:  // Data

        CPosLmLocalDatabase& iDatabase;
    
        // The database to search
        CPosLmLocalDbAccess* iDbAccess;

        // The local sort preference
        TPosLmLocalSortPref iLocalSortPref;

        // The max number of matches to search for.
        TInt iMaxNumOfMatches;

        // Tells whether previous result should be used or not
        TBool iSearchOnlyPreviousMatches;

        // Array containing results from a previous search
        RArray<TUint> iPreviousMatches;

        TPosLmProgressHelper iProgressHelper;

        // Stores next sort index for separate sorting search
        TInt iNextSortIndex;

        // Stores the criteria type
        CPosLmSearchCriteria::TCriteriaType iCriteriaType;

        // Stores the partial read parameters
        CPosLmPartialReadParameters* iPartialReadParameters;

        // The display data. Not owned by this class
        CPosLmDisplayData* iDisplayData;
        };

#endif      // CPOSLMLOCALSEARCHHANDLER_H

