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
* Description: Iterator for landmark item search matches.
*
*
*/



#ifndef CPOSLMLOCALSEARCHITERATOR_H
#define CPOSLMLOCALSEARCHITERATOR_H

//  INCLUDES
#include <EPos_CPosLmItemIterator.h>

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* Iterator for landmark item search matches. Iterates through all matches
* found in a search.
*
*  @lib eposlmlocalsearchprovider.lib
*/
class CPosLmLocalSearchIterator : public CPosLmItemIterator
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aMatchArray An array of matching landmark item ids.
        */
        static CPosLmLocalSearchIterator* NewL(
        /* IN  */       const RArray<TPosLmItemId>& aMatchArray
        );

        /**
        * Two-phased constructor.
        */
        static CPosLmLocalSearchIterator* NewL();

        /**
        * Destructor.
        */
        virtual ~CPosLmLocalSearchIterator();

    public: // Functions from base classes

        /*
        * From CPosLmItemIterator.
        *
        * Returns the next database item ID in the iterated set.
        * The first call will return the first database item ID in the set.
        *
        * @return The next database item ID, or KPosLmNullItemId if there
        *   are no more database items in the iterated set.
        */
        TPosLmItemId NextL();

        /**
        * From CPosLmItemIterator.
        *
        * Resets the database item iterator. The next call to NextL will return
        * the first database item ID in the iterated set.
        */
        void Reset();

        /**
        * From CPosLmItemIterator.
        *
        * Returns the number of database items which are iterated.
        * @return Number of items in the iterated database item set.
        */
        TUint NumOfItemsL();

        /**
        * From CPosLmItemIterator.
        *
        * Fetches a sequence of database items from the iterated set.
        *
        * The client supplies an id array which will be filled by this
        * operation. The array will be resetted before any items are added.
        *
        * The client specifies which database item sequence to fetch by
        * supplying a start index (0 is the first item) and the number of items
        * to fetch. If the specified interval lies partially outside the
        * iterated set, this function will panic.
        *
        * @param aIdArray Array to fetch from.
        * @param aStartIndex Index of first item.
        * @param aNumOfItems Number of items to fetch.
        */
        void GetItemIdsL(
        /* OUT */       RArray<TPosLmItemId>&  aIdArray,
        /* IN  */       TInt aStartIndex,
        /* IN  */       TInt aNumOfItems
        );

    private:

        /**
        * C++ default constructor.
        */
        CPosLmLocalSearchIterator();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(
        /* IN  */       const RArray<TPosLmItemId>& aMatchArray
        );

        // Prohibit copy constructor if not deriving from CBase.
        CPosLmLocalSearchIterator(const CPosLmLocalSearchIterator&);
        // Prohibit assigment operator if not deriving from CBase.
        CPosLmLocalSearchIterator& operator=(const CPosLmLocalSearchIterator&);

    private:    // Data

        // Current item.
        TInt iCurrent;

        // Landmark item ids.
        RArray<TPosLmItemId> iIdArray;
    };

#endif      // CPOSLMLOCALSEARCHITERATOR_H

// End of File
