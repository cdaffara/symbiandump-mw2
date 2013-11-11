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
* Description:  This class implements a wma parser 
*
*/



#ifndef CMetaDataParserWMA_H
#define CMetaDataParserWMA_H

// INCLUDES
#include <e32base.h>
#include "MetaDataParser.h"
#include "MetaDataSource.h"
#include "MetaDataFieldContainer.h"
#include "MetaDataField.h"

// CONSTANTS

// CLASS DECLARATION

/**
*  This class implements a WMA metadata parser.
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataParserWMA : public CMetaDataParser
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aSource Reference to a metadata source
        * @return A pointer to a new instance of this class
        */
        static CMetaDataParserWMA* NewL( CMetaDataSource& aSource );

        /**
        * Destructor.
        */
        virtual ~CMetaDataParserWMA();

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

	private:

        /**
        * C++ default constructor.
        */
		CMetaDataParserWMA( CMetaDataSource& aSource );

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
		void GetAuthorL();

		/**
        * Append Copyright to the container, if found.
        * @return void
		*/
		void GetCopyrightL();

		/**
        * Append Comment to the container, if found.
        * @return void
		*/
		void GetCommentL();

		/**
        * Append jpeg picture to the container, if found.
        * @return void
		*/
		void GetJpegL();
		
		/**
        * Append duration to the container, if found.
        * @return void
		*/
		void GetDurationL();
		
		/**
        * Append sample rate to the container, if found.
        * @return void
		*/
		void GetSampleRateL();
		
		/**
        * Append bit rate to the container, if found.
        * @return void
		*/
		void GetBitRateL();

		/**
        * Parses all entries in Content Description Object
        * @return void
		*/
		void ParseContentDescriptionObject();

		/**
        * Parses all entries in Extended Content Description Object
        * @return void
		*/
		void ParseExtendedContentDescriptionObjectL();

		/**
        * Gets a Extended Content Description Object Entry
        * @param aFieldId the field to parse
        * @param aOffset offset at which to find the field
        * @return void
		*/
		TBool GetExtContDesEntryL(TMetaDataFieldId aFieldId, TInt aOffset);

        /**
        * Converts to unicode.
		* @param aDesc Source to be converted to unicode.
		* @param aUnicode Contains the converted unicode on return.
        * @return KErrNone if successful, otherwise one of the system errors.
		*/
		TInt ConvertToUnicodeL( const TDesC8& aDesc, TDes16& aUnicode );

		/**
        * Formats Global UID
        * @return void
		*/
 		void FormatGUID(TDes8 &aGUID);

		/**
        * Descriptor to 64 bit integer conversion
        * @return void
		*/
		TInt64 ConvertToInt64(TDesC8& aDes);

		/**
        * Descriptor to 32 bit integer conversion
        * @return void
		*/
		TInt ConvertToInt32(TDesC8& aDes);

		/**
        * Descriptor to 16 bit integer conversion
        * @return void
		*/
		TInt ConvertToInt16(TDesC8& aDes);

		/**
        * 8 bit Descriptor to 16 bit Descriptor conversion
        * @return void
		*/
		void ConvertDes8toDes16(const TDesC8& aDes8,TDes16& aDes16);

		/**
        * Parses all entries in Header Extension Object
        * @return void
		*/
		void ParseHeaderExtensionObjectL();
		
		/**
        * Parses all entries in Metadata Library Object
        * @return void
		*/
		void ParseMetadataLibraryObjectL();
	private:	// Data
	
		// Metadata source
		CMetaDataSource& iSource;
		// Header data
		HBufC8* iHeaderData;
		// Character Set Id used during Unicode conversion
    	TUint iCharacterSetId;
		
		// offset of Content Description Object
		TInt iContentDescriptionOffset;
		// offset of File Properties Object
		TInt iFilePropertiesOffset;
		// offset of Extended Content Description Object
		TInt iExtendedContentDescriptionOffset;
		// offset of Header Extension Object
		TInt iHeaderExtensionOffset;
		TInt64 iHeaderExtensionObjectSize;
		// offset of Metadata Library Object
		TInt iMetadataLibraryOffset;
		TInt64 iMetadataLibraryObjectSize;
		TInt iMetadataLibraryDescriptionCount;
		TInt iExtendedContentDescriptionCount;
		TInt iAlbumTitleOffset;
		TInt iPictureOffset;
		TInt iTextOffset;
		TInt iComposerOffset;
		TInt iGenreOffset;
		TInt iYearOffset;
		TInt iOriginalArtistOffset;
		TInt iTrackNumberOffset;
		TInt iUniqueFileIdentifierOffset;
		TInt iAudioFileURLOffset;
		TInt iSharedUserRatingOffset;
		TInt iDateOffset;
		TInt iLyricsOffset;
        TInt iAlbumArtistOffset;
	#ifdef __WINDOWS_MEDIA
		TInt iProviderOffset;
	#endif

		TInt iTitleLength;
		TInt iAuthorLength;
		TInt iCopyrightLength;
		TInt iDescriptionLength;
		TInt iRatingLength;
		TBool iContentDescriptionObjectExists;
		TBool iFilePropertiesObjectExists;
		TBool iExtendedContentDescriptionObjectExists;
		TBool iHeaderExtensionObjectExists;
		TBool iMetadataLibraryObjectExists;
		TBool iMetadatLibraryObjectJpegExists;
		TBool iStreamPropertiesObjectExists;
		
		TInt iStreamPropertiesOffset;
    };

#endif      // CMetaDataParserWMA_H

// End of File
