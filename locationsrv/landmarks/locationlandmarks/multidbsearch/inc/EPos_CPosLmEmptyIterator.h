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
* Description: Iterator for landmark item search matches
*
*
*/



#ifndef CPOSLMEMPTYITERATOR_H
#define CPOSLMEMPTYITERATOR_H

//  INCLUDES
#include <EPos_CPosLmItemIterator.h>

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* Iterator for landmark item search matches. This iterator is always empty.
*
*/
NONSHARABLE_CLASS(CPosLmEmptyIterator) : public CPosLmItemIterator
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPosLmEmptyIterator* NewL();

        /**
        * Destructor.
        */
        virtual ~CPosLmEmptyIterator();

    public: // Functions from base classes

        /*
        * From CPosLmItemIterator.
        *
        * @return The next database item ID, or KPosLmNullItemId if there
        *   are no more database items in the iterated set.
        */
        TPosLmItemId NextL();

        /**
        * From CPosLmItemIterator.
        *
        */
        void Reset();

        /**
        * From CPosLmItemIterator.
        *
        * @return Number of items in the iterated database item set.
        */
        TUint NumOfItemsL();

        /**
        * From CPosLmItemIterator.
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
        CPosLmEmptyIterator();

    private:    // Data

    };

#endif      // CPOSLMEMPTYITERATOR_H

// End of File
