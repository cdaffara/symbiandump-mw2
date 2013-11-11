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
* Description:This class is used to start searches for landmarks or landmark categories
*  in a local landmark database.
*
*/


#ifndef CPOSLMLOCALSEARCH_H
#define CPOSLMLOCALSEARCH_H

//  INCLUDES
#include <e32base.h>
#include <EPos_CPosLandmarkSearch.h>

// FORWARD DECLARATIONS
class CPosLmLocalDatabase;
class CPosLmLocalSearchHandler;
class TPosLmLocalSortPref;
class CPosLmLocalSearchOperation;

// CONSTANT DECLARATIONS

// CLASS DECLARATION

/**
*  This class is used to start searches for landmarks or landmark categories
*  in a local landmark database.
*
*  @version $Revision: 1.8 $, $Date: 2005/02/15 13:36:46 $
*/
class CPosLmLocalSearch : public CPosLandmarkSearch
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aDatabase A landmark database.
        * @returns A new instance of this class.
        */
        static CPosLmLocalSearch* NewL(
        /* IN  */       TAny* aDatabase
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmLocalSearch();

    public: // New functions

        /**
        * This method must be called by the search operation when the operation
        * has completed (successfully or due to an error) or if the search
        * operation was cancelled.
        *
        */
        void SearchCompleted();

    public: // Functions from base classes

        /**
        * From CPosLandmarkSearch.
        *
        * Start a search for landmarks.
        *
        * The criteria which defines whether a landmark is a match or not is
        * passed as input to this function.
        *
        * The function returns an operation object which can be run in either
        * synchronous or asynchronous mode. If it is run in asynchronous mode
        * the client can supervise the progress of the operation.
        *
        * The client can cancel the search by deleting the
        * CPosLmOperation object.
        *
        * By default, this function starts a new search, but the client can
        * specify that only previous matches should be searched. This can be
        * used to refine the search when there are many matches. If there are
        * no previous matches, this function will leave with error code
        * @p KErrArgument.
        *
        * If a search is already running, this function will leave with error
        * code @p KErrInUse.
        *
        * If the search criteria is not supported, this function will leave
        * with error code @p KErrNotSupported.
        *
        * The client is allowed to delete the criteria object after the search
        * is started.
        *
        * The client takes ownership of the returned operation object.
        *
        * @param aCriteria The search criteria.
        * @param aSearchOnlyPreviousMatches This flag may be used to perform a
        *   search within the results of previous search.
        * @returns A handle to the search operation.
        */
        CPosLmOperation* StartLandmarkSearchL(
        /* IN  */       const CPosLmSearchCriteria&  aCriteria,
        /* IN  */       TBool  aSearchOnlyPreviousMatches = EFalse
        );

        /**
        * From CPosLandmarkSearch.
        *
        * Start a search for landmarks.
        *
        * The criteria which defines whether a landmark is a match or not is
        * passed as input to this function.
        *
        * The function returns an operation object which can be run in either
        * synchronous or asynchronous mode. If it is run in asynchronous mode
        * the client can supervise the progress of the operation.
        *
        * The client can cancel the search by deleting the
        * CPosLmOperation object.
        *
        * By default, this function starts a new search, but the client can
        * specify that only previous matches should be searched. This can be
        * used to refine the search when there are many matches. If there are
        * no previous matches, this function will leave with error code
        * @p KErrArgument.
        *
        * If a search is already running, this function will leave with error
        * code @p KErrInUse.
        *
        * If the search criteria is not supported, this function will leave
        * with error code @p KErrNotSupported.
        *
        * The client is allowed to delete the criteria object after the search
        * is started.
        *
        * This overload of the StartLandmarkSearchL function let's the client
        * define the sort order for the search matches.
        *
        * Only sorting by landmark name is supported. If the client tries to
        * sort by another attribute, this function will leave with error code
        * @p KErrNotSupported.
        *
        * The client takes ownership of the returned operation object.
        *
        * @param aCriteria The search criteria.
        * @param aSortPref A sort preference object.
        * @param aSearchOnlyPreviousMatches This flag may be used to perform a
        *   search within the results of previous search.
        * @returns A handle to the search operation.
        */
        CPosLmOperation* StartLandmarkSearchL(
        /* IN  */       const CPosLmSearchCriteria&  aCriteria,
        /* IN  */       const TPosLmSortPref&  aSortPref,
        /* IN  */       TBool  aSearchOnlyPreviousMatches = EFalse
        );

        /**
        * From CPosLandmarkSearch.
        *
        * Start a search for landmark categories.
        *
        * The criteria which defines whether a landmark category is a match or
        * not is passed as input to this function.
        *
        * The function returns an operation object which can be run in either
        * synchronous or asynchronous mode. If it is run in asynchronous mode
        * the client can supervise the progress of the operation.
        *
        * The client can cancel the search by deleting the
        * CPosLmOperation object.
        *
        * By default, this function starts a new search, but the client can
        * specify that only previous matches should be searched. This can be
        * used to refine the search when there are many matches. If there are
        * no previous matches, this function will leave with error code
        * @p KErrArgument.
        *
        * If a search is already running, this function will leave with error
        * code @p KErrInUse.
        *
        * If the search criteria is not supported, this function will leave
        * with error code @p KErrNotSupported.
        *
        * The client is allowed to delete the criteria object after the search
        * is started.
        *
        * The client passes a sort preference to specify if and how the
        * matching categories should be sorted.
        *
        * The client takes ownership of the returned operation object.
        *
        * @param aCriteria The search criteria.
        * @param aSortPref Sort preference for the search results.
        * @param aSearchOnlyPreviousMatches This flag may be used to perform a
        *   search within the results of previous search.
        * @returns A handle to the search operation.
        */
        CPosLmOperation* StartCategorySearchL(
        /* IN  */       const CPosLmSearchCriteria&  aCriteria,
        /* IN  */       CPosLmCategoryManager::TCategorySortPref  aSortPref,
        /* IN  */       TBool  aSearchOnlyPreviousMatches = EFalse
        );

        /**
        * From CPosLandmarkSearch.
        *
        * Returns the number of matches so far in the search.
        *
        * This function can also be called during a search operation.
        *
        * @return The number of search matches.
        */
        TUint NumOfMatches() const;

        /**
        * From CPosLandmarkSearch.
        *
        * Creates an iterator object to iterate the matching landmarks or
        * landmark categories.
        *
        * This function can also be called during a search in order to read the
        * matches encountered so far. Note, that the iterator will not iterate
        * any new matches. If new matches are found a new iterator must be
        * created.
        *
        * If a sort preference was specified when the search was started,
        * the landmarks/categories will be sorted when the search is complete
        * but the items will not be sorted if this function is called during
        * a search.
        *
        * The client takes ownership of the returned iterator object.
        *
        * @return A search match iterator.
        */
        CPosLmItemIterator* MatchIteratorL();

        /**
        * From CPosLandmarkSearch.
        *
        * @param aData The displayable data.
        */
        void SetDisplayData(
        /* IN/OUT */        CPosLmDisplayData& aData
        );

        /**
        * From CPosLandmarkSearch.
        */
        void UnsetDisplayData();

    private:

        /**
        * C++ default constructor.
        */
        CPosLmLocalSearch();

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aDatabase A landmark database.
        */
        void ConstructL(
        /* IN  */       TAny* aDatabase
        );

        /**
        * Creates the proper CPosLmSearchHandler and stores it in
        * iSearchHandler. Then creates the CPosLmLocalSearchOperation and
        * stores it in iSearchOperation.
        *
        * If a search is already running, this function will leave with error
        * code @p KErrInUse.
        *
        * If the search criteria is not supported, this function will leave
        * with error code @p KErrNotSupported.
        *
        * If the search criteria is incorrect, this function will leave
        * with error code @p KErrArgument.
        *
        * The client is allowed to delete the criteria object after the search
        * is started.
        *
        * @param aSearchForLandmarks Search for landmarks if this flag is true,
        *   otherwise search for categories.
        * @param aCriteria The search criteria.
        * @param aSortPref The sort preference for the search.
        * @param aSearchOnlyPreviousMatches This flag may be used to perform a
        *   search within the results of previous search.
        */
        void StartSearchL(
        /* IN  */       TBool aSearchForLandmarks,
        /* IN  */       const CPosLmSearchCriteria& aCriteria,
        /* IN  */       const TPosLmLocalSortPref& aSortPref,
        /* IN  */       TBool aSearchOnlyPreviousMatches
        );

    private:

        // By default, prohibit copy constructor
        CPosLmLocalSearch( const CPosLmLocalSearch& );
        // Prohibit assigment operator
        CPosLmLocalSearch& operator= ( const CPosLmLocalSearch& );

    private:    // Data

        // The local database. Not owned by this class.
        CPosLmLocalDatabase*        iDb;

        // Pointer to the current search operation. Not owned by this class.
        CPosLmLocalSearchOperation* iSearchOperation;

        // Pointer to a search handler.
        // Owned by this class when iOwnsSearchHandler is true.
        CPosLmLocalSearchHandler*   iSearchHandler;

        // Tells whether this class owns an instance of CPosLmLocalSearchHandler
        TBool                       iOwnsSearchHandler;

        // The displayable data. Not owned by this class.
        CPosLmDisplayData*  iDisplayData;
    };

#endif      // CPOSLMLOCALSEARCH_H

// End of File
