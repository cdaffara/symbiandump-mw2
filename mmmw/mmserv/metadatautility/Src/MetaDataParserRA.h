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



#ifndef CMetaDataParserRA_H
#define CMetaDataParserRA_H

// INCLUDES
#include <e32base.h>
#include "MetaDataParser.h"
#include "MetaDataSource.h"
#include "MetaDataFieldContainer.h"
#include "MetaDataField.h"

// CONSTANTS

// CLASS DECLARATION
class CHXMetaDataUtility;

/**
*  This class implements a RA metadata parser.
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataParserRA : public CMetaDataParser
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aSource Reference to a metadata source
        * @return A pointer to a new instance of this class
        */
        static CMetaDataParserRA* NewL( CMetaDataSource& aSource );

        /**
        * Destructor.
        */
        virtual ~CMetaDataParserRA();

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
		CMetaDataParserRA( CMetaDataSource& aSource );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

		/**
		* Determines whether or not there is valid source data.
		*/
		TBool ValidateL();

        /**
        * Append title to the container, if found.
        * @return TInt status
		*/
		TInt GetTitleL(TInt aCount);

        /**
        * Append artist to the container, if found.
        * @return TInt status
		*/
		TInt GetArtistL(TInt aCount);

        /**
        * Append genre to the container, if found.
        * @return TInt status
		*/
		TInt GetGenreL(TInt aCount);

        /**
        * Append composer to the container, if found.
        * @return Tint status
		*/
		TInt GetComposerL(TInt aCount);

		/**
        * Append Copyright to the container, if found.
        * @return Tint status
		*/
		TInt GetCopyrightL(TInt aCount);
		
		/**
        * Allows OpenFile on Helix
        * @return void
		*/		
		void OpenFileL(RFile& aFile);
		
		/**
        * Allows OpenDes on Helix
        * @return void
		*/		
		void OpenDesL(const TDesC8& aDes);		

	private:	// Data
	
		// Metadata source
		CMetaDataSource& iSource;
        HBufC* iData;
        CHXMetaDataUtility* iHxMetaDataUtility;
		
    };

#endif      // CMetaDataParserRA_H

// End of File
