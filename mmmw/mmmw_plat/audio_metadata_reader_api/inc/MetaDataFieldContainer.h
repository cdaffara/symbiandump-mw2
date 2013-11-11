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



#ifndef CMETADATAFIELDCONTAINER_H
#define CMETADATAFIELDCONTAINER_H

// INCLUDES
#include <e32base.h>
#include <MetaDataField.hrh>

// FORWARD DECLARATION
class CMetaDataField;

// CLASS DECLARATION

/**
*  This class implements
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataFieldContainer : public CBase
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
		* @return A pointer to a new instance of this class
        */
        static CMetaDataFieldContainer* NewL();

        /**
        * Destructor.
        */
        virtual ~CMetaDataFieldContainer();

    public: // New functions

        /**
        * Returns the number of metadata fields.
        * @since 3.0
		* @return Number of metadata fields
        */
		IMPORT_C TInt Count() const;

        /**
        * Returns the metadata field that corresponds to the field ID.
		* If the field is not found, returns KNullDesC.
        * @since 3.0
        * @param aFieldId Metadata field ID
        * @return Metadata field contents or KNullDesC
        */
		IMPORT_C TPtrC Field( TMetaDataFieldId  aFieldId ) const;

		/**
        * Returns the metadata field that corresponds to the field ID.
		* If the field is not found, returns KNullDesC.
        * @since 3.0
        * @param aFieldId Metadata field ID
        * @return Metadata field contents or KNullDesC
        */
		IMPORT_C TPtrC8 Field8( TMetaDataFieldId  aFieldId ) const;
		
        /**
        * Adds a metadata field to the container.
        * @since 3.2
        * @param aFieldId Metadata field ID
        * @param aData Metadata field contents
		* @return void
        */
		IMPORT_C void AppendL( TMetaDataFieldId  aFieldId, const TDesC& aData );
		
		/**
        * Adds a metadata field to the container.
        * @since 3.2
        * @param aFieldId Metadata field ID
        * @param aData Metadata 8 bit field contents
		* @return void
        */
		IMPORT_C void AppendL( TMetaDataFieldId  aFieldId, const TDesC8& aData );

        /**
        * Returns the field at given location. First element at position 0.
        * @since 3.0
        * @param aPosition Position within the container
        * @param aFieldId On return, metadata field ID
		* @return Metadata field contents, or KNullDesC.
        */
		IMPORT_C TPtrC At( TInt aPosition, TMetaDataFieldId& aFieldId ) const;

		/**
        * Populates the field ID at given location. 
        * @since 3.2
        * @param aPosition Position within the container
        * @param aFieldId On return, metadata field ID
		* @return Metadata field contents, or KNullDesC.
        */
		IMPORT_C void FieldIdAt( TInt aPosition, TMetaDataFieldId& aFieldId ) const;

	private:

        /**
        * C++ default constructor.
        */
		CMetaDataFieldContainer();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

 	private:	// Data

		// Array of metadata fields. Owned.
		RPointerArray<CMetaDataField> iFields;
		mutable TBool i16BitAlbumArt; 
		mutable HBufC16* iD16;
	};

#endif      // CMETADATAFIELDCONTAINER_H

// End of File
