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
* Description:  This class provides the base class for ID3v2 parsers.
*
*/



#ifndef CMETADATAPARSERID3V2_H
#define CMETADATAPARSERID3V2_H

// INCLUDES
#include <e32base.h>
#include <centralrepository.h>
#include "MetadataUtilityCRKeys.h"
#include "MetaDataParser.h"
#include "MetaDataSource.h"
#include "MetaDataFieldContainer.h"
#include "MetaDataField.h"



// CONSTANTS
const TUint KID3v2format = '(';


// CLASS DECLARATION

/**
*  This class implements the base class for ID3v2 parsers.
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataParserID3v2 : public CMetaDataParser
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aSource A pointer to a metadata source
        * @return A pointer to a new instance of this class
        */
        static CMetaDataParserID3v2* CreateID3v2ParserL( CMetaDataSource& aSource );
        
        /**
        * Two-phased constructor.
        * @param aSource A pointer to a metadata source
        * @param aOffset File offset of ID3 metadata 
        * @return A pointer to a new instance of this class
        */
        static CMetaDataParserID3v2* CreateID3v2ParserL( CMetaDataSource& aSource, TUint aOffset  );

        /**
        * Destructor.
        */
        virtual ~CMetaDataParserID3v2();

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
		virtual void ParseL( const RArray<TMetaDataFieldId>& aWantedFields, CMetaDataFieldContainer& aContainer );
		
		// Possible autodetect commands
        enum TAutoDetectCommand
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

	protected: // New functions

        /**
        * C++ default constructor.
        */
		CMetaDataParserID3v2( CMetaDataSource& aSource );

        /**
        * Parses frames and appends supported fields to the container, if found.
        * @since 3.0
        * @return void
		*/
		virtual void ParseFramesL();

        /**
        * Parses frames and appends the wanted fields to the container, if found.
        * @since 3.0
		* @param aWantedFields An array of TMetaDataFieldId's.
        * @return void
		*/
		virtual void ParseFramesL( const RArray<TMetaDataFieldId>& aWantedFields );

        /**
        * This function is called from ParseL for header parsing and data initialization.
        * @since 3.0
		* @return void
        */
		virtual void PrepareToParseL() = 0;

        /**
        * Gets the next field's properties.
        * @since 3.0
		* @param aFieldId Field Id, on return.
		* @param aFrameHeaderSize Frame header size.
		* @param aFrameSize Frame size, on return.
        * @return void
		*/
		virtual void GetNextFieldL( TMetaDataFieldId& aFieldId, TInt& aFrameHeaderSize, TInt& aFrameSize ) = 0;

        /**
        * Reads the text information from the source and appends it to the container.
        * @since 3.0
		* @param aFieldId Field Id.
		* @param aSize Size of the data.
        * @return void
		*/
		virtual void GetTextInfoL( TMetaDataFieldId aFieldId, TInt aSize ) = 0;

        /**
        * From CMetaDataParserID3v2
        * Reads the Genre from the source and appends it to the container.
		* @param aSize Size of the data.
        * @return void
		*/
		virtual void GetGenreL( TInt aSize ) = 0;

        /**
        * Reads the year from the source and appends it to the container.
        * @since 3.0
		* @param aSize Size of the data.
        * @return void
		*/
		virtual void GetTextYearL( TInt aSize ) = 0;

        /**
        * Reads the comment from the source and appends it to the container.
        * @since 3.0
		* @param aSize Size of the data.
        * @return void
		*/
		virtual void GetCommentL( TInt aSize ) = 0;

        /**
        * Reads the Url from the source and appends it to the container.
        * @since 3.0
		* @param aSize Size of the data.
        * @return void
		*/
		virtual void GetUrlL( TInt aSize ) = 0;
		
		/**
        * Reads the user defined Url from the source and 
        * appends it to the container.
        * @since 3.0
		* @param aSize Size of the data.
        * @return void
		*/
		virtual void GetUserUrlL( TInt aSize ) = 0;

        /**
        * Reads the Jpeg from the source and appends it to the container.
        * @since 3.0
		* @param aSize Size of the data.
        * @return void
		*/
		virtual void GetJpegL( TInt aSize ) = 0;

		/**
        * Reads the duration from the source and appends it to the container.
        * @since 3.0
		* @param aSize Size of the data.
        * @return void
		*/
		virtual void GetDurationL( TInt aSize ) = 0;
		
		/**
        * Reads the date from the source and appends it to the container.
        * @since 3.0
		* @param aSize Size of the data.
        * @return void
		*/
		virtual void GetTextDateL( TInt aSize ) = 0;
		
		/**
        * Reads the popularimeter from the source appends it to the container.
        * @since 3.0
		* @param aSize Size of the data.
        * @return void
		*/
		virtual void GetRatingL (TInt aSize) = 0;
       /**
        * Converts to unicode.
        * @since 3.0
		* @param aEncoding Encoding of aDesc.
		* @param aDesc Source to be converted to unicode.
		* @param aUnicode Contains the converted unicode on return.
        * @return The number of unconverted characters left at the end of the input descriptor
        *	(e.g. because the output descriptor is not long enough to hold all the text),
        *	or one of the error values defined in TError.
		*/
		virtual TInt ConvertToUnicodeL( TInt aEncoding, const TDesC8& aDesc, TDes16& aUnicode ) = 0;
		
		/**
        * Detects the character set, used to support fields with unknown or in-correct
        * text encoding, for the special case of Chinese GB2312 &BIG5
        * @since 3.0
		* @param aDes descriptor data
        * @return KErrNone, or any of the system wide error codes
		*/
		TInt DetectCharacterSetL(const TDesC8& aDesc);
		
		/**
        * Creates a list od character sets, top list, next top list, and rest list
        * for the special case of Chinese GB2312 &BIG5 character set detection
        * @since 3.0
		*/
		void CreateCharacterSetsL();
		
		TInt AutoDetectL(const TDesC8& aDesc, TDes16& aUnicode);
		
		void HandleV2GetGenreL( TInt aSize, TInt aKID3v2FrameHeaderLength, TInt aKID3v2GenreNameLength );
		
		TBool NeedRetrieveAlbumArt( TInt aPicType );
		
		void RetrieveAlbumArtL();
		
		TBool IgnoreAlbumType (TInt aPicType);
		
	private:

        /**
        * Finds the version of ID3v2 tag. (e.g. ID3v2.4 returns 4)
		* @param aSource Data source.
        * @return ID3 tag version, KErrNotFound if none found.
		*/
		static TInt VersionL( CMetaDataSource& aSource, TUint aOffset );
		
		

 	protected:	// Data

		// Metadata source
		CMetaDataSource *iSource;
		// Frame data size
		TInt iFrameDataSize;
		// Offset for frame position
		TInt iFrameOffset;
		// character set id
		TUint iCharacterSetId;
		// AutoDetect for Chinese BIG5 & GB2312
		TBool iAutoDetectChinese;
		// AutoDetect for Japanese 
		TBool iAutoDetectJapanese;
		// AutoDetect for Russian character sets
		TBool iAutoDetectRussian;
		// flag for parse from memory 
		TBool iParseFromDescriptor;
		// tag descriptor 
		HBufC8 *iTag;
		// memory store for tag
		CMetaDataSource *iSourceDes;
		
		//Album type
		TInt iAlbumType;
        //Album pic offset
        TInt iAlbumOffset;
        //Album pointer
        HBufC8* iAlbumPtr;

    };

#endif      // CMETADATAPARSERID3V2_H

// End of File
