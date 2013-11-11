/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Capsulates the icon information from the SIM.  
*
*/


#ifndef TSATICONINFO_H
#define TSATICONINFO_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
 *  Capsulates the SAT icon information.
 *
 *  @lib SatClient.lib
 *  @since 2.6
 */
NONSHARABLE_CLASS( TSatIconInfo )
    {
    public: // Enums

        /**
         * Coding scheme of the icon.
         * @since 2.6
         */
        enum TCodingScheme
            {
            EBasic = 0x11,  ///< Basic (Black and White) icon
            EColor = 0x21   ///< Color icon
            };

    public:  // Constructors

        /**
         * Constructor.
         */
        TSatIconInfo();

    public: // New functions

        /**
         * Size of the icon.
         *
         * @since 5.1
         * @return Size of the icon.
         */
        IMPORT_C TSize IconSize() const;

        /**
         * Coding scheme of the icon.
         * 
         * @since 5.1
         * @return Icon coding scheme.
         */
        IMPORT_C TCodingScheme CodingScheme() const;

        /**
         * Identifies the EF (Elementary File), where the icon is located in
         * the SIM.
         * 
         * @since 5.1
         * @return EF identifier.
         */
        IMPORT_C TUint16 FileIdentifier() const;

        /**
         * Offset to the EF, where the icon is located.
         * 
         * @since 5.1
         * @return Offset to the EF.
         */
        IMPORT_C TUint16 Offset() const;

        /**
         * Length of the icon data.
         * 
         * @since 5.1
         * @return Length of the icon data.
         */
        IMPORT_C TUint16 DataLength() const;
        
        /**
         * Set size of the icon.
         * 
         * @since 5.1
         * @param aIconSize Size of the icon.
         */
        void SetIconSize( const TSize aIconSize );

        /**
         * Set coding scheme of the icon.
         * 
         * @since 5.1
         * @param aCodingScheme Icon coding scheme.
         */
        void SetCodingScheme( const TCodingScheme aCodingScheme );

        /**
         * Set identifies the EF (Elementary File), where the icon is located in
         * the SIM.
         * @since 5.1
         * @param EF identifier.
         */
        void SetFileIdentifier( const TUint16 aFileIdentifier );

        /**
         * Set offset to the EF, where the icon is located.
         * 
         * @since 5.1
         * @param aOffset Offset to the EF.
         */
        void SetOffset( const TUint16 aOffset);

        /**
         * Set length of the icon data.
         * 
         * @since 5.1
         * @param aDataLength Length of the icon data.
         */
        void SetDataLength( const TUint16 aDataLength );

    private: // Data

        /** 
         * Size of the icon. 
         */
        TSize iSize;

        /** 
         * Coding scheme of the icon. 
         */
        TCodingScheme iCodingScheme;

        /** 
         * File identifier 
         */
        TUint16 iFileIdentifier;

        /** 
         * Offset of the icon in EF. 
         */
        TUint16 iOffset;

        /** 
         * Length of the icon data. 
         */
        TUint16 iLength;
    };

#endif // TSATICONINFO_H

// End of File
