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
* Description: Landmark iterator.
*
*
*/


#ifndef CPOSLMLOCALLANDMARKITERATOR_H
#define CPOSLMLOCALLANDMARKITERATOR_H

//  INCLUDES
#include <EPos_TPosLmSortPref.h>
#include "EPos_CPosLmLocalIterator.h"

// FORWARD DECLARATIONS
class CPosLmLocalDatabase;

// CLASS DECLARATION

/**
* Landmark iterator. Iterates through all landmarks in a landmarks database.
*
*  @lib eposlmlocalaccess.lib
*/
NONSHARABLE_CLASS(CPosLmLocalLandmarkIterator) : public CPosLmLocalIterator
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aDb A database.
        * @param aAlreadyHasLock If @p ETrue, the iterator will not try to
        *   acquire a read-lock.
        */
        static CPosLmLocalLandmarkIterator* NewL(
        /* IN */        CPosLmLocalDatabase& aDb,
        /* IN */        TBool aAlreadyHasLock = EFalse
        );

        /**
        * Two-phased constructor.
        * @param aDb A database.
        * @param aSortPref A sort preference object.
        */
        static CPosLmLocalLandmarkIterator* NewL(
        /* IN */        CPosLmLocalDatabase& aDb,
        /* IN */        TPosLmSortPref aSortPref
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmLocalLandmarkIterator();

    private:

         /**
        * C++ default constructor.
        */
        CPosLmLocalLandmarkIterator();

        /**
        * C++ default constructor.
        * @param aSortPref A sort preference object.
        */
        CPosLmLocalLandmarkIterator(
        /* IN */        TPosLmSortPref aSortPref
        );

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aDb A database.
        * @param aAlreadyHasLock If @p ETrue, the iterator will not try to
        *   acquire a read-lock.
        */
        void ConstructL(
        /* IN */        CPosLmLocalDatabase& aDb,
        /* IN */        TBool aAlreadyHasLock = EFalse
        );

    private:    // Data

        // Sort preference object.
        TPosLmSortPref iSortPref;

        // Is set to ETrue if sort is used. EFalse otherwise.
        TBool iUseSort;

    };

#endif      // CPOSLMLOCALLANDMARKITERATOR_H

// End of File
