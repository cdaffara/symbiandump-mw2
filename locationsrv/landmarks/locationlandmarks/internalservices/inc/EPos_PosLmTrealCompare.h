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
* Description:Provides functions to compare two TReal values.
*
*/


#ifndef POSLMTREALCOMPARE_H
#define POSLMTREALCOMPARE_H

// INCLUDES
#include <e32math.h>

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* Provides functions to compare two TReal values.
*/
class PosLmTrealCompare
    {

    public:  // New functions

        /**
        * Compares two real values. Also compares with NaN.
        * If aReal1 == NaN == aReal2, ETrue is returned.
        *
        * @param aReal1 A real value.
        * @param aReal2 A real value.
        * @return ETrue if equal, otherwise EFalse.
        */
        IMPORT_C static TBool IsEqual(
        /* IN */        TReal32 aReal1,
        /* IN */        TReal32 aReal2
        );

        /**
        * Compares two real values. Also compares with NaN.
        * If aReal1 == NaN == aReal2, ETrue is returned.
        *
        * @param aReal1 A real value.
        * @param aReal2 A real value.
        * @return ETrue if equal, otherwise EFalse.
        */
        IMPORT_C static TBool IsEqual(
        /* IN */        TReal64 aReal1,
        /* IN */        TReal64 aReal2
        );

        /**
        * Compares two real values. Also compares with NaN.
        * If aReal1 == NaN == aReal2, ETrue is returned.
        *
        * @param aReal1 A real value.
        * @param aReal2 A real value.
        * @return ETrue if equal, otherwise EFalse.
        */
        IMPORT_C static TBool IsEqual(
        /* IN */        TRealX aReal1,
        /* IN */        TRealX aReal2
        );

    };

#endif      // POSLMTREALCOMPARE_H

// End of File
