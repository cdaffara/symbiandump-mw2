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
* Description:  This class implements an ID3v1 and v1.1 parser as specified in
*                www.id3.org.
*
*/



#ifndef CMETADATAPARSERID3V1_H
#define CMETADATAPARSERID3V1_H

// INCLUDES
#include <e32base.h>
#include "MetaDataParser.h"
#include "MetaDataSource.h"
#include "MetaDataFieldContainer.h"
#include "MetaDataField.h"

// CONSTANTS
const TInt KID3v1TagLength = 128;

// CLASS DECLARATION

/**
*  This class implements an ID3v1 and v1.1 parser.
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataParserID3v1 : public CMetaDataParser
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aSource Reference to a metadata source
        * @return A pointer to a new instance of this class
        */
        static CMetaDataParserID3v1* NewL( CMetaDataSource& aSource );

        /**
        * Destructor.
        */
        virtual ~CMetaDataParserID3v1();

    public: // Functions from base classes

        /**
        * From CMetaDataParser
        * Parses the source and if metadata is found, the metadata fields are
        * inserted into the container.
        * @since 3.0
		* @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
        * @param aContainer On return, metadata found in aSource is stored here
		* @return void
        */
		void ParseL( const RArray<TMetaDataFieldId>& aWantedFields, CMetaDataFieldContainer& aContainer );

        enum TAutoDetectCommand		// Possible autodetect commands
		{
	    EDetectFromTopList = 0,
		EDetectFromNextList,
		EDetectFromRestList
		};

		/**
        * From CMetaDataParser
        * returns ID3 version
        * @since 3.0
		* @return TID3Version
        */
		TID3Version ID3Version();
		
	private:

        /**
        * C++ default constructor.
        */
		CMetaDataParserID3v1( CMetaDataSource& aSource );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

		/**
		* Validates whether there is ID3v1 tag in the source or not.
		*/
		TBool ValidateL();

        /**
        * Append title to the container, if found.
        * @return void
		*/
		void GetTitleL();

        /**
        * Append artist to the container, if found.
        * @return void
		*/
		void GetArtistL();

        /**
        * Append album to the container, if found.
        * @return void
		*/
		void GetAlbumL();

        /**
        * Append year to the container, if found.
        * @return void
		*/
		void GetYearL();

        /**
        * Append comment to the container, if found.
        * @return void
		*/
		void GetCommentL();

        /**
        * Append album track to the container, if found.
        * @return void
		*/
		void GetAlbumTrackL();

        /**
        * Append genre to the container, if found.
        * @return void
		*/
		void GetGenreL();

        /**
        * Converts to unicode.
		* @param aDesc Source to be converted to unicode.
		* @param aUnicode Contains the converted unicode on return.
        * @return KErrNone if successful, otherwise one of the system errors.
		*/
		TInt ConvertToUnicodeL( const TDesC8& aDesc, TDes16& aUnicode );

        /**
        * Detects the character set for the descriptor.
		* @param aDesc Source to be converted to unicode.
        * @return KErrNone if successful, otherwise one of the system errors.
		*/
		TInt DetectCharacterSetL( const TDesC8& aDesc);

 	private:	// Data

		// Metadata source
		CMetaDataSource& iSource;
		// Tag data
		TBuf8<KID3v1TagLength> iTagData;
		// Character Set Id used during Unicode conversion
    	TUint iCharacterSetId;
    };

#endif      // CMETADATAPARSERID3V1_H

// End of File
