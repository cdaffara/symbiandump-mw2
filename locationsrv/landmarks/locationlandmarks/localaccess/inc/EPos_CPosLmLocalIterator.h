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
* Description: Local iterator base class.
*
*
*/



#ifndef CPOSLMLOCALITERATOR_H
#define CPOSLMLOCALITERATOR_H

#include <EPos_CPosLmItemIterator.h>

/**
* Local iterator base class.
*/
NONSHARABLE_CLASS(CPosLmLocalIterator) : public CPosLmItemIterator
    {

    public:  // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~CPosLmLocalIterator();

    public: // From CPosLmItemIterator.

        /*
        * @return The next database item ID, or KPosLmNullItemId if there
        *   are no more database items in the iterated set. */
        TPosLmItemId NextL();

        void Reset();

        /**
        * @return Number of items in the iterated database item set.
        */
        TUint NumOfItemsL();

        /**
        * @param[out] aIdArray Will contain the requested items.
        * @param[in] aStartIndex The index of the first item to fetch.
        * @param[in] aNumOfItems The number of items to fetch.
        */
        void GetItemIdsL(
            RArray<TPosLmItemId>&  aIdArray,
            TInt aStartIndex,
            TInt aNumOfItems );

    protected:

        CPosLmLocalIterator();

    private:
        // Prohibit copy constructor if not deriving from CBase.
        CPosLmLocalIterator(const CPosLmLocalIterator&);
        // Prohibit assigment operator if not deriving from CBase.
        CPosLmLocalIterator& operator=(const CPosLmLocalIterator&);

    protected:  // Data

        // Current item.
        TInt iCurrent;

        // All landmark ids.
        RArray<TPosLmItemId> iIdArray;

    };

#endif      // CPOSLMLOCALITERATOR_H

// End of File
