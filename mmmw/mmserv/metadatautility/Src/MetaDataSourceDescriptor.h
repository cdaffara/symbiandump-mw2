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
* Description:  This class implements the interface specified in CMetaDataSource
*				 when audio source is from a descriptor.
*
*/



#ifndef CMETADATASOURCEDESCRIPTOR_H
#define CMETADATASOURCEDESCRIPTOR_H

// INCLUDES
#include "MetaDataSource.h"

// CLASS DECLARATION

/**
*  This class implements the interface specified in CMetaDataSource
*  when audio source is from a descriptor.
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataSourceDescriptor : public CMetaDataSource
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
		* @param aDes Source descriptor
		* @return A pointer to a new instance of this class
        */
        static CMetaDataSourceDescriptor* NewL( const TDesC8& aDes );

        /**
        * Destructor.
        */
        virtual ~CMetaDataSourceDescriptor();

    public: // Functions from base classes

        /**
        * Reads sufficient data from the source to fill the specified 8-bit descriptor
        * up to its maximum length.
        * @since 3.0
        * @param aDes Contains the data that was read
		* @param aLength How many bytes will be read
        * @return KErrNone, or one of the systemwide error codes.
		*/
		void ReadL( TDes8& aDes );

        /**
        * Reads data of specified length from the source into the specified 8-bit descriptor.
        * @since 3.0
        * @param aDes Contains the data that was read. Note that the descriptor
		*        length must be at least aLength!
		* @param aLength How many bytes will be read
        * @return KErrNone, or one of the systemwide error codes.
		*/
		void ReadL( TDes8& aDes, TInt aLength );

        /**
        * Reads data from specified position in the source into the specified 8-bit descriptor
        * up to its maximum length.
        * @since 3.0
        * @param aPos Offset from current position
		* @param aDes Contains the data that was read
        * @return KErrNone, or one of the systemwide error codes.
		*/
		void ReadL( TInt aPos, TDes8& aDes );

        /**
        * Reads data from specified position in the source into the specified 8-bit descriptor
        * up to its maximum length.
        * @since 3.0
        * @param aPos Offset from current position
		* @param aDes Contains the data that was read
		* @param aLength How many bytes will be read
        * @return KErrNone, or one of the systemwide error codes.
		*/
		void ReadL( TInt aPos, TDes8& aDes, TInt aLength );

        /**
        * Size of the source in bytes.
        * @since 3.0
        * @param aSize On return, the size is stored here.
        * @return KErrNone, or one of the systemwide error codes.
		*/
		TInt Size( TInt& aSize ) const;

	private:

        /**
        * C++ default constructor.
        */
		CMetaDataSourceDescriptor();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL( const TDesC8& aDes );

 	private:	// Data

		// Source descriptor
		TPtrC8 iSource;
		// Source length
		TInt iLength;
	};

#endif      // CMETADATASOURCEDESCRIPTOR_H

// End of File
