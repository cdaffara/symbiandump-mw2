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
* Description:  Converts icon to bitmap.
*
*/



#ifndef CSATSICONCONVERTER_H
#define CSATSICONCONVERTER_H

//  INCLUDES
#include <e32base.h>
#include <etelsat.h>
#include <gdi.h>

// FORWARD DECLARATION
class CFbsBitmap;

// CLASS DECLARATION

/**
*  Converts icon data to CFbsBitmap.
*
*  @lib SatEngine.lib
*  @since 2.6
*/
class CSatSIconConverter : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Factory method to create converter.
        * @param aCoding Coding scheme of the icon.
        * @return New converter instance.
        */
        static CSatSIconConverter* CreateConverterL(
            const RSat::TImageCoding& aCoding );

        /**
        * Destructor.
        */
        virtual ~CSatSIconConverter() {};

    public: // New functions

        /**
        * Converts the icon data to CFbsBitmap.
        * @param aData Icon instance data.
        * @param aClut Color look-up table. In basic icon clut is not provided.
        * @return Bitmap.
        */
        virtual CFbsBitmap* ConvertL(
            const TDesC8& aData,
            const TDesC8& aClut ) const = 0;

    protected:

        /**
        * Creates bitmap.
        * @param aWidth Width of the bitmap.
        * @param aHeight Height of the bitmap.
        * @param aDisplayMode Display mode.
        * @return New bitmap instance.
        */
        CFbsBitmap* CreateBitmapL(
            TUint8 aWidth,
            TUint8 aHeight,
            TDisplayMode aDisplayMode ) const;

    protected:

        /**
        * C++ default constructor.
        */
        CSatSIconConverter();

    };

#endif      // CSATSICONCONVERTER_H

// End of File
