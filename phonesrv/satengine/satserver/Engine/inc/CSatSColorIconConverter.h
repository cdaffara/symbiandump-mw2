/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Converts color icon to CFbsBitmap
*
*/



#ifndef CSATSCOLORICONCONVERTER_H
#define CSATSCOLORICONCONVERTER_H

//  INCLUDES
#include "CSatSIconConverter.h"

// CLASS DECLARATION

/**
*  Converts color icon to CFbsBitmap
*
*  @lib SatEngine.lib
*  @since 2.6
*/
class CSatSColorIconConverter : public CSatSIconConverter
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CSatSColorIconConverter();

        /**
        * Destructor.
        */
        virtual ~CSatSColorIconConverter() {};

    private: // New functions

        /**
        * Checks that image data and clut from SIM is valid.
        * @param aData Image data from SIM.
        */
        void CheckDataValidityL(
            const TDesC8& aData ) const;

        /**
        * Checks that clut is valid.
        * @param aData Image data.
        * @param aClut Clut.
        */
        void CheckClutValidityL(
            const TDesC8& aData,
            const TDesC8& aClut ) const;

    public: // Functions from base classes

        /**
        * From CSatSIconConverter
        * @param aData Image data.
        * @param aClut Clut.
        * @return Bitmap.
        */
        CFbsBitmap* ConvertL(
            const TDesC8& aData,
            const TDesC8& aClut ) const;

    };

#endif      // CSATSCOLORICONCONVERTER_H

// End of File
