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



#ifndef CMETADATAPARSER3GP_H
#define CMETADATAPARSER3GP_H

// INCLUDES
#include <e32base.h>
#include <mp4lib.h>
#include "MetaDataParserMP4.h"
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
class CMetaDataParser3gp : public CMetaDataParserMP4
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @return A pointer to a new instance of parser; NULL if parser is not created.
        */
        static CMetaDataParser3gp* NewL();

        /**
        * Destructor.
        */
        virtual ~CMetaDataParser3gp();

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
		
		/**
		* From CMetaDataParser
		* Validates whether this is valid 3GP source or not.
		* @since 3.0
		* @return ETrue if valid, EFalse otherwise.
		*/	
		TBool ValidateL();

    public: // New functions

    private:

        /**
        * C++ default constructor.
        */
		CMetaDataParser3gp();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

        /**
        * Append specified metadata to the container, if found.
        * @return void
		*/
		void GetAssetBoxL( TUint32 aBoxType, TMetaDataFieldId aFieldId );
		
		/**
        * Sets ID32 metadata start offset (iID32Offset), if found.
        */
		void GetID32L();
		
	private:
		// CenRep key for specific metadata mapping
		TBool iVFKK;
			
    };

#endif      // CMETADATAPARSER3GP_H

// End of File
