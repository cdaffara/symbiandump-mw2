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
* Description:  This class implements an 3gp metadata parser as specified in
*                www.3gpp.org (specification 3GPP TS 26.244 V6.0.0).
*
*/



#ifndef CMetaDataParserDcf2_H
#define CMetaDataParserDcf2_H

// INCLUDES
//#include <DRMCommon.h>

#include <e32base.h>
#include "MetaDataSourceFile.h"
#include "MetaDataParser.h"
#include "MetaDataFieldContainer.h"
#include "MetaDataField.h"

// CONSTANTS


// CLASS DECLARATION

/**
*  This class implements an 3gp metadata parser.
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataParserDcf2 : public CMetaDataParser
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @return A pointer to a new instance of parser; NULL if parser is not created.
        */
        static CMetaDataParserDcf2* NewL();

        /**
        * Destructor.
        */
        virtual ~CMetaDataParserDcf2();

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

    public: // New functions

        /**
        * Initializes the parser with the specified file.
        * @since 3.0
		* @param aFileName The name of the file
		* @return void
        */
		void InitializeFileL( CMetaDataSource* aSource );

	private:

        /**
        * C++ default constructor.
        */
		CMetaDataParserDcf2();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

		/**
		* Validates whether this is valid DCF source or not.
		* @return ETrue if valid, EFalse otherwise.
		*/
		TBool ValidateL();

        /**
        * Converts to unicode.
		* @param aDesc Source to be converted to unicode.
		* @param aUnicode Contains the converted unicode on return.
        * @return The number of unconverted characters left at the end of the input descriptor
        *	(e.g. because the output descriptor is not long enough to hold all the text),
        *	or one of the error values defined in TError.
		*/
		TInt ConvertToUnicodeL( const TDesC8& aDesc, TDes16& aUnicode );

	private:

        /**
        * Reads meta data from DCF header. Leaves on error.
		* @param aEntryName name of header field to read.
		* @param aFieldId field id defined in MetaDataField.hrh.
        * @return void
		*/
		void ReadMetaDataEntryL(TInt aEntryName, TMetaDataFieldId aFieldId);

 	private:	// Data
		
		// source
		CMetaDataSource* iSource;
		
    };

#endif      // CMetaDataParserDcf2_H

// End of File
