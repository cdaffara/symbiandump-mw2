/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPosLandmarkSearch class
*
*/


#ifndef CPOSLANDMARKSEARCH_H
#define CPOSLANDMARKSEARCH_H

#include <e32base.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLmItemIterator.h>
#include <EPos_CPosLmOperation.h>
#include <EPos_TPosLmSortPref.h>

class CPosLmSearchCriteria;
class CPosLmDisplayData;

const TInt KPosLmMaxNumOfMatchesUnlimited = -1;

/**
*  This class is used to perform searches for landmarks or landmark categories
*  in a landmark database.
*
*  To search a landmark database, an instance of this class is created,
*  supplying the database to search. The client creates a criterion object to
*  specify what to search for. There are different criterion classes which all
*  inherit from @ref CPosLmSearchCriteria. For instance, the client can search
*  for landmarks which contain a certain text string by passing a
*  @ref CPosLmTextCriteria.
*
*  Some criterion classes are used for searching for landmarks (e.g.
*  @ref CPosLmCategoryCriteria is used for searching for landmarks which
*  contain a certain category) and some are used
*  to search for landmark categories (e.g. @ref CPosLmCatNameCriteria is used
*  to search for landmark categories by specifying a category name which may
*  contain wild-card characters).
*
*  Searches can be run in incremental mode.
*  @ref StartLandmarkSearchL and @ref StartCategorySearchL both return a
*  @p CPosLmOperation object which is used to execute the search. If it is
*  run incrementally the client can supervise the progress of the operation.
*  If it is sufficient to run the search synchronously, the client only has to
*  call @p CPosLmOperation::ExecuteL. The client can cancel
*  the search by deleting the @p CPosLmOperation object.
*
*  By default, these functions start a new search, but the client can specify
*  that only previous matches should be searched. This can be used to refine
*  the search when there are many matches.
*
*  During the search execution a read-lock is acquired for each database.
*
*  Only one search can be performed at a time by the single instance of
*  the class. If a search is already running, the search function leaves
*  with error code @p KErrInUse.
*
*  After search completion, the client can make a second search and specify
*  that only the matches from the previous search shall be considered.
*
*  The client can also set a limit on how many search matches should be
*  returned (see @ref SetMaxNumOfMatches).
*
*  The client retrieves the matches from the search by requesting an iterator
*  (see @ref MatchIteratorL) or by using display data (see @ref SetDisplayData).
*
*  @p NetworkServices capability is required for remote databases.
*
*  @lib eposlmsearchlib.lib
*  @since S60 3.0
*/
class CPosLandmarkSearch : public CBase
    {
    public:

        /**
        * Two-phased constructor.
        *
        * The client takes ownership of the returned search object.
        *
        * @param[in] aDatabase The landmark database to search.
        * @returns A new instance of this class.
        */
        IMPORT_C static CPosLandmarkSearch* NewL(
            CPosLandmarkDatabase&  aDatabase
        );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPosLandmarkSearch();

    public:

        /**
        * Retrieves the maximum number of search matches limit.
        *
        * By default the maximum number of matches is unlimited.
        *
        * @return The maximum number of search matches or
        *   @p KPosLmMaxNumOfMatchesUnlimited if unlimited.
        */
        IMPORT_C TInt MaxNumOfMatches() const;

        /**
        * Sets the maximum number of search matches limit.
        *
        * If the limit is set, the search operation will stop when this limit
        * is reached.
        *
        * By default the maximum number of matches is unlimited.
        *
        * If a new value for maximum number of matches is set when a search is
        * ongoing, it will not affect the current search. The new maximum will
        * be utilized in the next search.
        *
        * @param[in] aMaxNumOfMatches the maximum number of search matches.
        *   KPosLmMaxNumOfMatchesUnlimited means that the number of matches is
        *   unlimited.
        */
        IMPORT_C void SetMaxNumOfMatches(
            TInt  aMaxNumOfMatches = KPosLmMaxNumOfMatchesUnlimited
        );

        /**
        * Starts a search for landmarks.
        *
        * The client takes ownership of the returned operation object.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[in] aCriteria The search criterion.
        * @param[in] aSearchOnlyPreviousMatches This flag may be used to perform a
        *   search within the results of previous search.
        * @returns A handle to the search operation.
        *
        * @leave KErrInUse Search is already running.
        * @leave KErrNotSupported search criterion is not supported.
        * @leave KErrArgument Search criterion is not valid for landmark searching.
        * @leave KErrArgument There are no previous matches and the client specifies that
        *   previous matches should be used.
        */
        virtual CPosLmOperation* StartLandmarkSearchL(
            const CPosLmSearchCriteria& aCriteria,
            TBool aSearchOnlyPreviousMatches = EFalse
        ) = 0;

        /**
        * Starts a search for landmarks.
        *
        * This overload of the @ref StartLandmarkSearchL function lets the
        * client define the sort order for the search matches. 
        * Only sorting by landmark name is supported.
        *
        * The client takes ownership of the returned operation object.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[in] aCriteria The search criterion.
        * @param[in] aSortPref A sort preference object.
        * @param[in] aSearchOnlyPreviousMatches This flag may be used to perform a
        *   search within the results of previous search.
        * @returns A handle to the search operation.
        *
        * @leave KErrInUse Search is already running.
        * @leave KErrArgument Search criterion is not valid for landmark searching.
        * @leave KErrArgument There are no previous matches and the client specifies that
        *   previous matches should be used.
        * @leave KErrNotSupported Search criterion is not supported.
        * @leave KErrNotSupported Client tries to sort by other attribute than name.
        */
        virtual CPosLmOperation* StartLandmarkSearchL(
            const CPosLmSearchCriteria& aCriteria,
            const TPosLmSortPref& aSortPref,
            TBool  aSearchOnlyPreviousMatches = EFalse
        ) = 0;

        /**
        * Starts a search for landmark categories.
        *
        * The criterion which defines whether a landmark category is a match or
        * not is passed as input to this function.
        *
        * The client takes ownership of the returned operation object.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[in] aCriteria The search criterion.
        * @param[in] aSortPref Sort preference for the search results.
        * @param[in] aSearchOnlyPreviousMatches This flag may be used to perform a
        *   search within the results of previous search.
        * @returns A handle to the search operation.
        *
        * @leave KErrInUse Search is already running.
        * @leave KErrArgument Search criterion is not valid for category searching.
        * @leave KErrNotSupported Search criterion is not supported.
        */
        virtual CPosLmOperation* StartCategorySearchL(
            const CPosLmSearchCriteria& aCriteria,
            CPosLmCategoryManager::TCategorySortPref aSortPref,
            TBool aSearchOnlyPreviousMatches = EFalse
        ) = 0;

        /**
        * Returns the number of matches so far in the search.
        *
        * This function can also be called during a search operation.
        *
        * @return The number of search matches.
        */
        virtual TUint NumOfMatches() const = 0;

        /**
        * Creates an iterator object to iterate the matching landmarks or
        * landmark categories.
        *
        * This function can also be called during a search in order to read the
        * matches encountered so far. Note that the iterator will not iterate
        * any new matches. If new matches are found, a new iterator must be
        * created.
        *
        * If a sort preference was specified when the search was started,
        * the landmarks/categories will be sorted when the search is complete
        * but the items are not necessarily sorted if this function is called
        * during a search.
        *
        * Note: If the client has set display data and resets the display data
        * during a search, the sort order in the iterator might be incorrect.
        *
        * The client takes ownership of the returned iterator object.
        *
        * Note that the iterator iterates matches in @ref CPosLandmarkSearch.
        * It cannot be used after the search object has been deleted. Make sure
        * to delete the iterator first.
        *
        * @return A search match iterator.
        */
        virtual CPosLmItemIterator* MatchIteratorL() = 0;

        /**
        * Display data can be used as an alternative way to get result
        * from a database search. Landmarks or categories are added to the
        * display data collection during a search depending on the search type.
        *
        * This function may replace the combination of using
        * @ref MatchIteratorL and reading landmark or category data.
        * Result data is read already during the search and no duplicate
        * access to database is needed.
        *
        * The display data object will be reset each time a new search is
        * started. No items during the search are removed from the
        * collection. New found matches can be added every time next
        * search step is completed, see @ref CPosLmDisplayData::NewItemIndex.
        *
        * Note: The database index of displayable data items in
        * @ref CPosLmDisplayData will be set to 0 as it has no meaning in a
        * single database search.
        *
        * The client owns the display data object. If the client deletes it
        * during a search, this may lead to unexpected errors. The client must
        * call @ref UnsetDisplayData before it deletes the display data object.
        *
        * Note: If the client resets the display data during a search, the sort
        * order in the iterator might become incorrect.
        *
        * @param[in,out] aData The displayable data.
        *
        * @panic "Landmarks Client"-EPosSearchOperationInUse 
        *   The client set display data during an ongoing search.
        */
        virtual void SetDisplayData( CPosLmDisplayData& aData ) = 0;

        /**
        * Unsets display data. No further data will be added to the display
        * data that was set with @ref SetDisplayData.
        *
        * @panic "Landmarks Client"-EPosSearchOperationInUse 
        *   The client unset display data during an ongoing search.
        */
        virtual void UnsetDisplayData() = 0;

    protected:

        // C++ constructor.
        IMPORT_C CPosLandmarkSearch();

    private:

        // Prohibit copy constructor
        CPosLandmarkSearch( const CPosLandmarkSearch& );
        // Prohibit assigment operator
        CPosLandmarkSearch& operator= ( const CPosLandmarkSearch& );

    private:

        TUid iDtorIdKey;
        TInt iMaxNumOfMatches;
    };

#endif      // CPOSLANDMARKSEARCH_H


