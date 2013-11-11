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
* Description:  This class implements parsing of ID3v2.3 according to the
*                specification found in www.id3.org. Note that only a selected
*				 frames are supported at this time.
*
*/



#ifndef CMETADATAPARSERID3V23_H
#define CMETADATAPARSERID3V23_H

// INCLUDES
#include "MetaDataParserID3v2.h"

// CLASS DECLARATION

/**
*  This class implements parsing of ID3v2.3.
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataParserID3v23 : public CMetaDataParserID3v2
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aSource Reference to a metadata source
        * @return A pointer to a new instance of this class
        */
        static CMetaDataParserID3v23* NewL( CMetaDataSource& aSource );

        /**
        * Destructor.
        */
        virtual ~CMetaDataParserID3v23();

    private: // Functions from base classes

        /**
        * From CMetaDataParserID3v2
        * This function is called from ParseL for header parsing and data initialization.
		* @return void
        */
		void PrepareToParseL();

        /**
        * From CMetaDataParserID3v2
        * Gets the next field's properties.
		* @param aFieldId Field Id, on return.
		* @param aFrameHeaderSize Frame header size.
		* @param aFrameSize Frame size, on return.
        * @return void
		*/
		void GetNextFieldL( TMetaDataFieldId& aFieldId, TInt& aFrameHeaderSize, TInt& aFrameSize );

        /**
        * From CMetaDataParserID3v2
        * Reads the text information from the source and appends it to the container.
		* @param aFieldId Field Id.
		* @param aSize Size of the data.
        * @return void
		*/
		void GetTextInfoL( TMetaDataFieldId aFieldId, TInt aSize );

        /**
        * From CMetaDataParserID3v2
        * Reads the Genre from the source and appends it to the container.
		* @param aSize Size of the data.
        * @return void
		*/
		void GetGenreL( TInt aSize );

        /**
        * From CMetaDataParserID3v2
        * Reads the year from the source and appends it to the container.
		* @param aSize Size of the data.
        * @return void
		*/
		void GetTextYearL( TInt aSize );

        /**
        * From CMetaDataParserID3v2
        * Reads the comment from the source and appends it to the container.
		* @param aSize Size of the data.
        * @return void
		*/
		void GetCommentL( TInt aSize );

        /**
        * From CMetaDataParserID3v2
        * Reads the Url from the source and appends it to the container.
		* @param aSize Size of the data.
        * @return void
		*/
		void GetUrlL( TInt aSize );
		
		/**
        * From CMetaDataParserID3v2
        * Reads the user defined Url from the source and 
        * appends it to the container.
        * @since 3.0
		* @param aSize Size of the data.
        * @return void
		*/	
		void GetUserUrlL( TInt aSize );
		void GetUnsyncLyricsL( TInt aSize ); 

        /**
        * From CMetaDataParserID3v2
        * Reads the Jpeg from the source and appends it to the container.
		* @param aSize Size of the data.
        * @return void
		*/
		void GetJpegL( TInt aSize );
		
		/**
		* From CMetaDataParserID3v2
        * Reads the duration from the source and appends it to the container.
        * @since 3.0
		* @param aSize Size of the data.
        * @return void
		*/
		void GetDurationL( TInt aSize );
		
		/**
		* From CMetaDataParserID3v2
        * Reads the date from the source and appends it to the container.
        * @since 3.0
		* @param aSize Size of the data.
        * @return void
		*/
		void GetTextDateL( TInt aSize );

		/**
		* From CMetaDataParserID3v2
        * Reads the popularimeter appends it to the container.
        * @since 3.0
		* @param aSize Size of the data.
        * @return void
		*/
		void GetRatingL (TInt aSize);
		
        /**
        * From CMetaDataParserID3v2
        * Converts to unicode.
		* @param aEncoding Encoding of aDesc.
		* @param aDesc Source to be converted to unicode.
		* @param aUnicode Contains the converted unicode on return.
        * @return The number of unconverted characters left at the end of the input descriptor
        *	(e.g. because the output descriptor is not long enough to hold all the text),
        *	or one of the error values defined in TError.
		*/
		TInt ConvertToUnicodeL( TInt aEncoding, const TDesC8& aDesc, TDes16& aUnicode );

	private:

        /**
        * C++ default constructor.
        */
		CMetaDataParserID3v23( CMetaDataSource& aSource );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

 	private:	// Data

		// Metadata source
//		CMetaDataSource& iSource;
    };

#endif      // CMETADATAPARSERID3V23_H

// End of File
