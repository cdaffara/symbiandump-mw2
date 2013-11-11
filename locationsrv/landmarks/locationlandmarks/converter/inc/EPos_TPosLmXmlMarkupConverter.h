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
* Description: Helper class for converting from/to xml markup data.
*
*/



#ifndef TPOSLMXMLMARKUPCONVERTER_H
#define TPOSLMXMLMARKUPCONVERTER_H

//  INCLUDES
#include <Lbs.h>
#include "EPos_LmConverter.h"

// CLASS DECLARATION

/**
*  Helper class for converting from/to xml markup data.
*
*/
class TPosLmXmlMarkupConverter
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        TPosLmXmlMarkupConverter();

    public: // New functions


        /**
        * Resets the usage of start and end tags.
        */
        void Reset();


        static void FixChildTagL(
        /* IN */        TUint32 aTreeMask,
        /* IN/OUT */    TPosXmlTagType& aTagType
        );

    private:


    };

#endif      // TPOSLMXMLMARKUPCONVERTER_H

// End of File
