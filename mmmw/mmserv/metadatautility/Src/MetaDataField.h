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
* Description:  This class provides
*
*/



#ifndef CMETADATAFIELD_H
#define CMETADATAFIELD_H

// INCLUDES
#include <e32base.h>
#include <MetaDataField.hrh>

// CLASS DECLARATION

/**
*  This class implements
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataField : public CBase
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
		* @param aId The ID of this metadata field
		* @param aData The field data
		* @return A pointer to a new instance of this class
        */
        static CMetaDataField* NewLC( TMetaDataFieldId aId, const TDesC& aData );

		/**
        * Two-phased constructor.
		* @param aId The ID of this metadata field
		* @param aData An 8 bit field data 
		* @return A pointer to a new instance of this class
        */
        static CMetaDataField* NewLC( TMetaDataFieldId aId, const TDesC8& aData );
        

        /**
        * Destructor.
        */
        virtual ~CMetaDataField();

    public: // New functions

        /**
        * Returns the metadata field ID.
        * @since 3.0
		* @return This field's ID
        */
		TMetaDataFieldId Id() const;

        /**
        * Returns the metadata field data.
        * @since 3.0
		* @return Pointer to data
        */
		TPtrC Data() const;
		/**
        * Returns the metadata field 8 bit data.
        * @since 3.0
		* @return Pointer to data
        */
		TPtrC8 Data8() const;

	private:

        /**
        * C++ default constructor.
        */
		CMetaDataField();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL( TMetaDataFieldId aId, const TDesC& aData );

		/**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL( TMetaDataFieldId aId, const TDesC8& aData );
		
 	private:	// Data

		// MetaData field ID
		TMetaDataFieldId iId;
		// MetaData field content. Owned.
        HBufC* iData;
        // MetaData Field 8-bit content. Owned.
        HBufC8* iData8;
	};

#endif      // CMETADATAFIELD_H

// End of File
