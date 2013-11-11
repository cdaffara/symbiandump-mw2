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
* Description:  This class provides base class for metadata source classes used
*                by different parsers.
*
*/



#ifndef CMETADATASOURCE_H
#define CMETADATASOURCE_H

// INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
*  This class implements base class for metadata sources such as files or descriptors.
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataSource : public CBase
	{
	public:  // Destructor

		/**
		* Destructor.
		*/
		virtual ~CMetaDataSource();

	protected:  // Constructor

        /**
        * C++ default constructor.
        */
		CMetaDataSource();

    public: // New functions

        /**
        * Reads sufficient data from the source to fill the specified 8-bit descriptor
        * up to its maximum length.
        * @since 3.0
        * @param aDes Contains the data that was read
		* @param aLength How many bytes will be read
        * @return KErrNone, or one of the systemwide error codes.
		*/
		virtual void ReadL( TDes8& aDes ) = 0;

        /**
        * Reads data of specified length from the source into the specified 8-bit descriptor.
        * @since 3.0
        * @param aDes Contains the data that was read. Note that the descriptor
		*        length must be at least aLength!
		* @param aLength How many bytes will be read
        * @return KErrNone, or one of the systemwide error codes.
		*/
		virtual void ReadL( TDes8& aDes, TInt aLength ) = 0;

        /**
        * Reads data from specified position in the source into the specified 8-bit descriptor
        * up to its maximum length.
        * @since 3.0
        * @param aPos Offset from current position
		* @param aDes Contains the data that was read
        * @return KErrNone, or one of the systemwide error codes.
		*/
		virtual void ReadL( TInt aPos, TDes8& aDes ) = 0;

        /**
        * Reads data from specified position in the source into the specified 8-bit descriptor
        * up to its maximum length.
        * @since 3.0
        * @param aPos Offset from current position
		* @param aDes Contains the data that was read
		* @param aLength How many bytes will be read
        * @return KErrNone, or one of the systemwide error codes.
		*/
		virtual void ReadL( TInt aPos, TDes8& aDes, TInt aLength ) = 0;

        /**
        * Returns the size of the source in bytes.
        * @since 3.0
        * @param aSize On return, the size is stored here.
        * @return KErrNone, or one of the systemwide error codes.
		*/
		virtual TInt Size( TInt& aSize ) const = 0;

	};

#endif      // CMETADATASOURCE_H

// End of File
