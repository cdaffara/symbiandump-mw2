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
* Description: Imported landmark iterator.
*
*
*/


#ifndef CPOSLMLOCALIMPORTEDLMITERATOR_H
#define CPOSLMLOCALIMPORTEDLMITERATOR_H

//  INCLUDES
#include "EPos_CPosLmLocalIterator.h"

// FORDWARD DECLARATIONS
class CPosLmOperation;

// CLASS DECLARATION

/**
* Imported landmark iterator. Iterates through all imported landmark from a
* landmark import operation.
*
*  @lib eposlmlocalaccess.lib
*/
NONSHARABLE_CLASS(CPosLmLocalImportedLmIterator) : public CPosLmLocalIterator
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aImportOperation A import operation.
        */
        static CPosLmLocalImportedLmIterator* NewL(
        /* IN */    CPosLmOperation* aImportOperation
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmLocalImportedLmIterator();

    private:

        /**
        * C++ default constructor.
        */
        CPosLmLocalImportedLmIterator();

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param aImportOperation A import operation.
        */
        void ConstructL(
        /* IN */    CPosLmOperation* aImportOperation
        );

        // Prohibit copy constructor if not deriving from CBase.
        CPosLmLocalImportedLmIterator(
            const CPosLmLocalImportedLmIterator&);
        // Prohibit assigment operator if not deriving from CBase.
        CPosLmLocalImportedLmIterator& operator=(
            const CPosLmLocalImportedLmIterator&);

    };

#endif      // CPOSLMLOCALIMPORTEDLMITERATOR_H

// End of File
