/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: CPosLmMultiDbSearch class used to perform searches for landmarks or
*				       landmark categories in multiple databases.
*
*
*/


#ifndef CPOSLMMULTIDBSEARCH_H
#define CPOSLMMULTIDBSEARCH_H

#include <e32base.h>
#include <badesca.h>
#include <EPos_CPosLandmarkSearch.h>
#include <EPos_CPosLmCategoryManager.h>

class CPosLmSearchCriteria;
class CPosLmDisplayData;
class CPosLmMultiDbSearchOperation;
class CPosLmMultiDbSortPref;
class CPosLmMultiDbSearchItem;

/**
*  @ref CPosLmMultiDbSearch is used to perform searches for landmarks or
*  landmark categories in multiple databases.
*
*  The client can specify which databases to search.
*
*  Some criterion classes are used for searching for landmarks (e.g.
*  @ref CPosLmCategoryCriteria is used for searching for landmarks which
*  contain a certain category) and some are used
*  to search for landmark categories (e.g. @ref CPosLmCatNameCriteria is used
*  to search for landmark categories by specifying a category name).
*
*  Searches can be run in incremental mode.
*  @ref StartLandmarkSearchL and @ref StartCategorySearchL both return a
*  @p CPosLmOperation object which is used to execute the search. If it is
*  sufficient to run the search synchronously, the client only has to
*  call @p CPosLmOperation::ExecuteL. If it is run incrementally the client can
*  supervise the progress of the operation. The @p CPosLmOperation::NextStep
*  function in the search operations cannot be executed synchronously using
*  @p User::WaitForRequest. Doing so may cause the operation to hang.
*  @p CPosLmOperation::NextStep must be run using an active object. The client
*  can cancel the search by deleting the @p CPosLmOperation object.
*
*  By default, these functions start a new search, but the client can specify
*  that only previous matches should be searched. This can be used to refine
*  the search when there are many matches.
*
*  It is not allowed to search by category item ID in
*  @ref CPosLmCategoryCriteria since an item ID is only valid in one landmark
*  database.
*
*  It is only allowed to specify a named category, a global category,
*  or no category. It is not allowed to search with @ref CPosLmIdListCriteria
*  since an item ID is only valid in one landmark database. In both cases
*  the search functions leave with error code @p KErrArgument.
*
*  During the search execution, a read-lock is acquired for each database.
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
*  (see @ref MatchIteratorL) or by using display data (see
*  @ref SetDisplayData).
*
*  If @ref CPosLmMultiDbSearch is used, the client must call the global
*  function @ref ReleaseLandmarkResources before terminating in order to
*  release all used landmark resources, otherwise the client may receive
*  an ALLOC panic.
*
*  @p NetworkServices capability is required for remote databases.
*
*  @lib eposlmmultidbsearch.lib
*  @since S60 3.0
*  @version $Revision: 1.17 $, $Date: 2005/07/07 13:40:10 $
*/
class CPosLmMultiDbSearch : public CBase
    {
    public:  // Data types

        /**
        *  Struct containing a search error.
        */
        struct TSearchError
            {
            TUint iDatabaseIndex; /**<
                The index of the database where the search has failed. The
                database URI can be retrieved by calling @ref DatabaseUriPtr.*/
            TInt iErrorCode; /**<
                The search error code for the database. */
            };

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * Leaves with @p KErrArgument if the database list is empty.
        *
        * @param[in] aDatabaseList An array containing the URIs of the landmark
        *   databases to be used in the search.
        * @return A new instance of this class.
        */
        IMPORT_C static CPosLmMultiDbSearch* NewL( const CDesCArray&  aDatabaseList );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPosLmMultiDbSearch();

    public: // New functions

        /**
        * Specifies the list of databases that should be used in the search.
        *
        * If this function is called and then @ref StartLandmarkSearchL or
        * @ref StartCategorySearchL is called with the flag
        * aSearchOnlyPreviousMatches set, new databases that were not a part
        * of the previous search will generate no matches.
        *
        * If the client specifies database(s) that do not exist,
        * @ref GetSearchError will report error code @p KErrNotFound for those
        * databases after the search is started.
        *
        * This function will leave with @p KErrInUse if called during a
        * search.
        *
        * If this function is called after a search has completed,
        * database indexes in the results may become invalid.
        *
        * If a database is removed from a previously set list, the search
        * result of that database is unavailable after this function
        * is called. Search errors are reset after this function is called.
        *
        * Leaves with @p KErrArgument if the database list is empty.
        *
        * @since S60 3.0
        * @param[in] aDatabaseList An array containing the URIs of the landmark
        *   databases to be used in the search.
        */
        IMPORT_C void SetDatabasesToSearchL( const CDesCArray&  aDatabaseList );

        /**
        * Returns the list of databases to search. Ownership is transferred
        * to the caller.
        *
        * @return The list of databases to search.
        */
        IMPORT_C CDesCArray* DatabasesToSearchL();

        /**
        * Sets the maximum number of search matches limit for each database.
        *
        * This function is used to limit the number of matches retrieved from
        * each database in the search operation. If the limit is set, the
        * search operation will stop when this limit is reached. By default
        * the maximum number of matches is unlimited.
        *
        * If a new value for maximum number of matches is set when a search is
        * ongoing, it will not affect the current search. The new maximum will
        * be utilized in the next search.
        *
        * @since S60 3.0
        * @param[in] aMaxNumOfMatches The maximum number of search matches for
        *   each landmark database involved in the search.
        */
        IMPORT_C void SetMaxNumOfMatches( TInt  aMaxNumOfMatches = KPosLmMaxNumOfMatchesUnlimited );

        /**
        * Starts a landmark search.
        *
        * If there are no previous matches and the client specifies that
        * previous matches should be used, this function leaves with error
        * code @p KErrArgument.
        *
        * If a search is already running, this function leaves with error
        * code @p KErrInUse.
        *
        * If the search criterion is not valid for landmark searching, this
        * function leaves with error code @p KErrArgument.
        *
        * If the search criterion is not supported, this function leaves
        * with error code @p KErrNotSupported.
        *
        * If @ref CPosLmIdListCriteria is used or @ref CPosLmCategoryCriteria
        * with item ID set, this function leaves with error code
        * @p KErrArgument.
        *
        * The client takes ownership of the returned operation object.
        *
        * This function requires @p ReadUserData capability.
        *
        * @since S60 3.0
        * @param[in] aCriteria The search criterion.
        * @param[in] aSearchOnlyPreviousMatches This flag may be used to perform a
        *   search within the results of previous search.
        * @returns A handle to the search operation.
        */
        IMPORT_C CPosLmOperation* StartLandmarkSearchL(
            const CPosLmSearchCriteria&  aCriteria,
            TBool  aSearchOnlyPreviousMatches = EFalse );

        /**
        * Starts a landmark search.
        *
        * This overload of the @ref StartLandmarkSearchL function lets the
        * client define the sort order for the search matches.
        *
        * Only sorting by landmark name is supported. If the client tries to
        * sort by another attribute, this function leaves with error code
        * @p KErrNotSupported.
        *
        * If there are no previous matches and the client specifies that
        * previous matches should be used, this function leaves with error
        * code @p KErrArgument.
        *
        * If a search is already running, this function will leave with error
        * code @p KErrInUse.
        *
        * If the search criterion is not valid for landmark searching, this
        * function leaves with error code @p KErrArgument.
        *
        * If the search criterion is not supported, this function will leave
        * with error code @p KErrNotSupported.
        *
        * If @ref CPosLmIdListCriteria is used or @ref CPosLmCategoryCriteria
        * with item ID set, this function leaves with error code
        * @p KErrArgument.
        *
        * The client takes ownership of the returned operation object.
        *
        * This function requires @p ReadUserData capability.
        *
        * @since S60 3.0
        * @param[in] aCriteria The search criterion.
        * @param[in] aSortPref A sort preference object.
        * @param[in] aSearchOnlyPreviousMatches This flag may be used to perform a
        *   search within the results of previous search.
        * @returns A handle to the search operation.
        */
        IMPORT_C CPosLmOperation* StartLandmarkSearchL(
            const CPosLmSearchCriteria&  aCriteria,
            const TPosLmSortPref&  aSortPref,
            TBool  aSearchOnlyPreviousMatches = EFalse );

        /**
        * Starts a search for landmark categories.
        *
        * If there are no previous matches and the client specifies that
        * previous matches should be used, this function leaves with error
        * code @p KErrArgument...
        *
        * The criterion, which defines if a landmark category is a match,
        * is passed as input to this function.
        *
        * If a search is already running, this function leaves with error
        * code @p KErrInUse.
        *
        * If the search criterion is not valid for landmark category searching,
        * this function leaves with error code @p KErrArgument.
        *
        * If the search criterion is not supported, this function leaves
        * with error code @p KErrNotSupported.
        *
        * The client takes ownership of the returned operation object.
        *
        * This function requires @p ReadUserData capability.
        *
        * @param[in] aCriteria The search criteria.
        * @param[in] aSortPref Sort preference for the search results.
        * @param[in] aSearchOnlyPreviousMatches This flag may be used to perform a
        *   search within the results of previous search.
        * @returns A handle to the search operation.
        */
        IMPORT_C CPosLmOperation* StartCategorySearchL(
            const CPosLmSearchCriteria&  aCriteria,
            CPosLmCategoryManager::TCategorySortPref  aSortPref,
            TBool  aSearchOnlyPreviousMatches = EFalse );

        /**
        * Returns the number of errors encountered during the search. This is
        * the same as number of databases in which the search has failed.
        *
        * @since S60 3.0
        * @return The number of errors encountered during the search.
        */
        IMPORT_C TUint NumOfSearchErrors() const;

        /**
        * Returns errors encountered in the search.
        *
        * The client specifies an index of the error to retrieve. The index must
        * be strictly less than @ref NumOfSearchErrors, otherwise this function
        * panics with code @p EPosInvalidIndex. Whenever a new search is started
        * this function must be called again to retrieve error codes, if any.
        *
        * @since S60 3.0
        * @param[in] aErrorIndex The index of the error to retrieve.
        * @param[out] aSearchError On return, contains the search error.
        */
        IMPORT_C void GetSearchError(
            TUint  aErrorIndex,
            TSearchError&  aSearchError ) const;

        /**
        * Returns the total number of matches so far in the search.
        *
        * This function can also be called during a search operation.
        *
        * @since S60 3.0
        * @return The number of search matches.
        */
        IMPORT_C TUint TotalNumOfMatches() const;

        /**
        * Returns the number of matches so far in the search for a
        * database specified by index.
        *
        * The index must be strictly less than @ref NumOfDatabasesToSearch,
        * otherwise this function will panic with code @p EPosInvalidIndex.
        * The URI of the database can be retrieved by calling
        * @ref DatabaseUriPtr.
        *
        * This function can also be called during a search operation.
        *
        * @param[in] aDatabaseIndex The index of the database to get number
        *   of search matches for.
        * @return The number of matches per database.
        */
        IMPORT_C TUint NumOfMatches( TUint aDatabaseIndex ) const;

        /**
        * Returns the number of databases involved in the search.
        *
        * @since S60 3.0
        * @return The number of databases involved in the search.
        */
        IMPORT_C TUint NumOfDatabasesToSearch() const;

        /**
        * Returns the URI of a database involved in the search.
        *
        * The client specifies an index of the database URI to retrieve. The
        * index must be strictly less than @ref NumOfDatabasesToSearch,
        * otherwise this function will panic with code @p EPosInvalidIndex.
        *
        * @since S60 3.0
        * @param[in] aDatabaseIndex The index of the database URI to retrieve.
        * @return A pointer to the database URI descriptor. This pointer is
        *   only valid until @ref SetDatabasesToSearch is called, or the
        *   @ref CPosLmMultiDbSearch object is destroyed, whichever happens
        *   first.
        */
        IMPORT_C TPtrC DatabaseUriPtr( TUint aDatabaseIndex ) const;

        /**
        * Creates an iterator object to iterate the matching landmarks or
        * categories from one of the databases involved in the search.
        *
        * The database to get search matches for is specified by index. The
        * index must be strictly less than @ref NumOfDatabasesToSearch,
        * otherwise this function will panic with code @p EPosInvalidIndex.
        * The URI of the database can be retrieved by calling
        * @ref DatabaseUriPtr.
        *
        * This function can also be called during a search in order to read the
        * matches encountered so far. Note that the iterator will not iterate
        * any new matches. If new matches are found, a new iterator needs to be
        * created to retrieve them. The previous matches will also be included.
        *
        * If the client has started a search, but no matches have been found
        * yet in the database, an empty iterator is returned.
        *
        * If a sort preference was specified when the search was started, the
        * landmarks or categories will be sorted when the search is complete
        * but the items are not necessarily sorted if this function is
        * called during a search.
        *
        * The client takes ownership of the returned iterator object.
        *
        * Note: The iterator iterates matches in @ref CPosLmMultiDbSearch.
        * It cannot be used after the search object has been deleted.
        *
        * @since S60 3.0
        * @param aDatabaseIndex The index of the database to get search matches
        *   for.
        * @return A search results iterator.
        */
        IMPORT_C CPosLmItemIterator* MatchIteratorL( TUint aDatabaseIndex );

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
        * If the client sets display data during an ongoing search, this
        * function panics with code @p EPosSearchOperationInUse.
        *
        * The client owns the display data object. If the client deletes it
        * during a search, this may lead to unexpected errors. The client must
        * call @ref UnsetDisplayData before it deletes the display data object.
        *
        * Search results from all databases are collected in the same
        * displayable data collection. The @ref CPosLmDisplayItem::DatabaseIndex
        * may be used to know which database every displayable item belongs to.
        * The database index matches databases specified in this object,
        * see @ref DatabaseUriPtr.
        *
        * @param[in,out] aData The displayable data.
        */
        IMPORT_C void SetDisplayData( CPosLmDisplayData& aData );

        /**
        * Unsets display data. No further data will be added to the display
        * data set with @ref SetDisplayData.
        *
        * If the client unsets display data during an ongoing search, this
        * function panics with code @p EPosSearchOperationInUse.
        */
        IMPORT_C void UnsetDisplayData();

        /**
        * Retrieves the maximum number of search matches limit for each
        * database.
        *
        * By default the maximum number of matches is unlimited.
        *
        * @return The maximum number of search matches for each landmark
        *   database involved in the search or
        *   @p KPosLmMaxNumOfMatchesUnlimited if unlimited.
        */
        IMPORT_C TInt MaxNumOfMatches() const;

        /*
        * Tells if the database with the specified database index is to be
        * searched or not.
        *
        * @param aDatabaseIndex The index of the database.
        * @return ETrue if the database is to be searched, otherwise EFalse.
        */
        TBool ToBeSearched( TUint aDatabaseIndex ) const;

        /*
        * Gets a pointer to the database with the specified database index.
        *
        * @param aDatabaseIndex The index of the database.
        * @return A pointer to the landmark database.
        */
        CPosLandmarkDatabase* DatabaseL( TUint aDatabaseIndex );

        /*
        * Gets a pointer to the single search class for the database with the
        * specified database index.
        *
        * @param aDatabaseIndex The index of the database.
        * @return A pointer to the single search class.
        */
        CPosLandmarkSearch* SearcherL( TUint aDatabaseIndex );

        /*
        * This method must be called to notify this class when a search has
        * been started in the single search class corresponding to the
        * specified database index.
        *
        * @param aDatabaseIndex The index of the database.
        */
        void SearchStarted( TUint aDatabaseIndex );

        /*
        * This method must be called to notify this class when a search has
        * been executed (partly or completely) in the single search class
        * corresponding to the specified database index.
        *
        * @param[in] aDatabaseIndex The index of the database.
        * @param[in] aSortPref The sort pref for the search.
        */
        void SearchExecutedL(
            TUint aDatabaseIndex,
            const CPosLmMultiDbSortPref& aSortPref );

        /*
        * Adds a search error for the specified database index.
        *
        * @param aDatabaseIndex The index of the database.
        * @param aErrorCode The error code.
        */
        void AddSearchError( TUint aDatabaseIndex, TInt aErrorCode );

        /*
        * This function must be called by the CPosLmMultiDbSearchOperation
        * when the search operation has completed or has been cancelled.
        */
        void HandleSearchOperationCompleted();

    private:

        /**
        * C++ default constructor.
        */
        CPosLmMultiDbSearch();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(const CDesCArray& aDatabaseList);

        void CloseDbsNotToSearch(const CDesCArray& aDatabaseList);

        CPosLmOperation* StartSearchL(
            const CPosLmSearchCriteria& aCriteria,
            CPosLmMultiDbSortPref* aSortPref,
            TBool aSearchOnlyPreviousMatches );

    private:    // Data

        TInt                    iMaxNumOfMatches;
        TInt                    iNewMaxNumOfMatches;

        CPosLmMultiDbSearchOperation* iMultiSearchOperation; // No ownership

        RPointerArray<CPosLmMultiDbSearchItem> iSearchItems;

        RArray<TSearchError>    iSearchErrors; // Can contain dummy errors.
        TUint                   iNoOfSearchErrors; // Number of valid errors.

        CPosLmDisplayData*      iClientDisplayData; // No ownership

    };

#endif      // CPOSLMMULTIDBSEARCH_H

