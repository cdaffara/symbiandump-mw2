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
* Description:  This class implements an MP4 metadata parser 
*
*/



#ifndef CMETADATAPARSERMP4_H
#define CMETADATAPARSERMP4_H

// INCLUDES
#include <e32base.h>
#include <mp4lib.h>

#include "MetaDataParser.h"
#include "MetaDataFieldContainer.h"
#include "MetaDataField.h"

// CONSTANTS

const TInt K3gpMetaLength = 256;

// CLASS DECLARATION

/**
*  This class implements an MP4 metadata parser.
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataParserMP4 : public CMetaDataParser
	{
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
		CMetaDataParserMP4();

        /**
        * Destructor.
        */
        virtual ~CMetaDataParserMP4();

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
		virtual void ParseL( const RArray<TMetaDataFieldId>& aWantedFields, CMetaDataFieldContainer& aContainer ) = 0;

    public: // New functions

        /**
        * Initializes the parser with the specified file.
        * @since 3.0
		* @param aFileName The name of the file
		* @return void
        */
		void InitializeFileL( ContentAccess::CData *aData);

        /**
        * Initializes the parser with the specified descriptor.
        * @since 3.0
		* @param aDes Source descriptor
		* @return void
        */
		void InitializeDesL( const TDesC8& aDes );

       	/**
		* Validates whether this is valid MP4 source or not.
		* @since 3.0
		* @return ETrue if valid, EFalse otherwise.
		*/
		virtual TBool ValidateL() = 0;

        /**
        * Translates error code from MP4 library to one of System Errors.
        * @since 3.0
        * @param aError error code from MP4 library
        * @return System error
        */
		TInt TranslateMP4Err( MP4Err aError );
		
		/**
        * Reads the duration from the source and appends it to the container.
        * @since 3.0
		* @param aSize Size of the data.
        * @return void
		*/
		void GetDurationL(TMetaDataFieldId aFieldId );

        /**
        * Converts to unicode.
		* @param aDesc Source to be converted to unicode.
		* @param aUnicode Contains the converted unicode on return.
        * @return The number of unconverted characters left at the end of the input descriptor
        *	(e.g. because the output descriptor is not long enough to hold all the text),
        *	or one of the error values defined in TError.
		*/
		TInt ConvertToUnicodeL( const TDesC8& aDesc, TDes16& aUnicode );
		
		
 	protected:	// Data

		// MP4 library handle
        MP4Handle iMP4Handle;
    };

#endif      // CMETADATAPARSERMP4_H

// End of File
