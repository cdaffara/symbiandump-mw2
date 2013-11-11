/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Converts basic icon to CFbsBitmap.
*
*/



#ifndef CSATSBASICICONCONVERTER_H
#define CSATSBASICICONCONVERTER_H

//  INCLUDES
#include "CSatSIconConverter.h"

// CLASS DECLARATION

/**
*  Converts the basic icon to CFbsBitmap
*
*  @lib SatEngine.lib
*  @since 2.6
*/
class CSatSBasicIconConverter : public CSatSIconConverter
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CSatSBasicIconConverter();

        /**
        * Destructor.
        */
        virtual ~CSatSBasicIconConverter() {};

    public: // Functions from base classes

        /**
        * From CSatSIconConverter
        * @param aData Icon basic data.
        * @param aClut Color lookup table. Not used in basic icon
        * conversion.
        * @return Bitmap.
        */
        CFbsBitmap* ConvertL(
            const TDesC8& aData,
            const TDesC8& aClut ) const;

    private: // New functions

        /**
        * Checks that the icon data has valid length.
        * @param aData Image data.
        */
        void CheckDataValidityL(
            const TDesC8& aData ) const;

    };

#endif      // CSATSBASICICONCONVERTER_H

// End of File
