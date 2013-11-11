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
* Description:  This class implements an 3GPExt metadata parser 
*
*/



#ifndef CMETADATAPARSER3GPEXT_H
#define CMETADATAPARSER3GPEXT_H

// INCLUDES
#include <e32base.h>
#include "MetaDataParserMP4.h"
#include "MetaDataFieldContainer.h"
#include "MetaDataField.h"

// CONSTANTS

// CLASS DECLARATION
class C3GPExtParser;

/**
*  This class implements an 3GPExt metadata parser.
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataParser3GPExt : public CMetaDataParserMP4
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @return A pointer to a new instance of parser; NULL if parser is not created.
        */
        static CMetaDataParser3GPExt* NewL();

        /**
        * Destructor.
        */
        virtual ~CMetaDataParser3GPExt();

    public: // Functions from base classes

        /**
        * From CMetaDataParserMP4
        * Parses the source and if metadata is found, the metadata fields are
        * inserted into the container.
        * @since 3.0
		* @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
        * @param aContainer On return, metadata found in aSource is stored here
		* @return void
        */
		void ParseL( const RArray<TMetaDataFieldId>& aWantedFields, CMetaDataFieldContainer& aContainer );
 		
 		/**
 		* From CMetaDataParserMP4
		* Validates whether this is valid 3GPExt source or not.
		* @since 3.0
		* @return ETrue if valid, EFalse otherwise.
		*/
		TBool ValidateL();
     

	private:
	
		void ConvertAndAppendL(HBufC8** buf8, TMetaDataFieldId aFieldId);
		/**
        * Converts to unicode.
		* @param aDesc Source to be converted to unicode.
		* @param aUnicode Contains the converted unicode on return.
        * @return KErrNone if successful, otherwise one of the system errors.
		*/
		TInt ConvertToUnicodeL( const TDesC8& aDesc, TDes16& aUnicode );
		
        /**
        * C++ default constructor.
        */
		CMetaDataParser3GPExt();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();
		
		/**
        * Utility function to Append
        */
		void AppendL(HBufC** aBuf, TMetaDataFieldId aFieldId);
		/**
        * Utility function to Append
        */
		void AppendL(HBufC8** aBuf, TMetaDataFieldId aFieldId);
		
	private:
	
		// C3GPExtParser
		C3GPExtParser *i3GPExtParser;
    };

#endif      // CMETADATAPARSER3GPEXT_H

// End of File
