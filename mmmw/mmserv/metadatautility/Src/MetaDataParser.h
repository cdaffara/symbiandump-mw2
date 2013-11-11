/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class provides the base class for metadata parsers.
*                This class is the highest class in parser hierarchy.
*
*/



#ifndef MMETADATAPARSER_H
#define MMETADATAPARSER_H

// INCLUDES
#include <e32base.h>
#include <MetaDataField.hrh>
#include <f32file.h>
#include <charconv.h>
#include <barsc.h>
#include <barsread.h>

#include "MetaDataID3v1Genre.h"

// FORWARD DECLARATION
class CMetaDataFieldContainer;
class CMetaDataSourceFile;

// CONSTANTS
const TInt KUnicodeBOMNotFound		= 0;
const TInt KUnicodeBOMBigEndian		= 1;
const TInt KUnicodeBOMLittleEndian	= 2;

// CLASS DECLARATION

/**
*  This class implements the base class in metadata hierarchy.
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataParser : public CBase
	{
	public:  // Destructor

		/**
		* Destructor.
		*/
		virtual ~CMetaDataParser();

	protected:  // Constructor

        /**
        * C++ default constructor.
        */
		CMetaDataParser();

    public: // New functions

        /**
        * Parses the source and if metadata is found, the metadata fields are
        * inserted into the container.
        * @since 3.0
		* @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
		* @param aContainer On return, metadata found in aSource is stored here
		* @return void
        */
		virtual void ParseL( const RArray<TMetaDataFieldId>& aWantedFields,
			CMetaDataFieldContainer& aContainer ) = 0;

        /**
        * Returns whether metadata was found or not.
        * @since 3.0
        * @return ETrue, if metadata was found
        */
		TBool MetaDataExists();
		
		/**
        * Returns id3 version for ID3 tag types
        * @since 3.0
        * @return TID3Type enum
        */
		virtual TID3Version ID3Version();
		
		/**
        * Maps ID3v1 genre integer to string
        * @param aNum genre integer code
        * @param aGenrePtr string genre value returned
        * @since 3.0
        * @return void
        */
		void MapID3GenreToStringL( TInt aNum, TDes& aGenrePtr );

		/**
        * Maps ID3v2 genre integer to string
        * @param aNum genre integer code
        * @param aGenrePtr string genre value returned
        * @since 3.0
        * @return void
        */

		void MapID3GenreToStringL( TInt aNum, TDes8& aGenrePtr );
		void GenerateTopCharacterSetsL();
		void SelectCharacterSetsForLanguageL( TInt aLanguage );
		void ReadCharacterSetResourceL( TInt aResourceId );
		TBool IsInTopCharacterSet( TUint aCharacterSetId );

		void SetID32Offset( TUint aOffset );
		TUint ID32Offset();
		
		/**
        * Parses the common metadata, the metadata fields are
        * inserted into the container.
        * @since 9.2
        * @param aSource a Metadata source file.
        * @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
        * @param aContainer On return, metadata found in aSource is stored here
        * @return void
        */
        void CommonParseL( CMetaDataSourceFile* aSource,
                const RArray<TMetaDataFieldId>& aWantedFields, 
                CMetaDataFieldContainer& aContainer );

        /**
        * Parses the common metadata, the metadata fields are
        * inserted into the container.
        * @since 9.2
        * @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
        * @param aContainer On return, metadata found in aSource is stored here
        * @return void
        */
        void CommonParseDesL( const RArray<TMetaDataFieldId>& aWantedFields, 
                CMetaDataFieldContainer& aContainer );

        /**
        * Append DRM Protected data to the container.
        * @param aSource a Metadata source file.
        * @param aContainer On return, metadata ptotected data is stored here
        * @return void
        */
		void GetProtectedL( CMetaDataSourceFile* aSource,
		        CMetaDataFieldContainer& aContainer );

    protected:

        /**
        * Strips trailing zeros at the end of the field.
        * @since 3.0
        * @param aDesc The original descriptor
        * @return Pointer to descriptor without whitespace
        */
        TPtrC8 StripTrailingZeroes( const TDesC8& aDesc, TInt encoding = 0 );
        TPtrC StripTrailingZeroes( const TDesC& aDesc );

        /**
        * Returns BOM indication in the provided unicode.
        * @since 3.0
        * @param aUnicode Descriptor containing unicode.
        * @return KUnicodeBOMNotFound, KUnicodeBOMBigEndian, or KUnicodeBOMLittleEndian
		*/
		TInt UnicodeBOM( const TDesC8& aUnicode );

	protected:     // Data

		// Metadata container
		CMetaDataFieldContainer* iContainer;
		// Metadata exists
		TBool iExists;
		// File session
		RFs iFs;
		// Character set for unicode conversion
		CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* iCharacterSet;
		// Most likely character set for unicode conversion
		CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* iTopCharacterSet;

		RResourceFile iRscFile;
	    TUint32 iID32Offset;

		//CArrayFixFlat<TInt>* installedLanguages;
	};

#endif      // MMETADATAPARSER_H

// End of File
