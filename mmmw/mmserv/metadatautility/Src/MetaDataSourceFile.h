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
*				 when audio source is from a file.
*
*/



#ifndef CMETADATASOURCEFILE_H
#define CMETADATASOURCEFILE_H

// INCLUDES
#include <caf/content.h>
#include <caf/data.h>
#include "MetaDataSource.h"

using namespace ContentAccess;
// CLASS DECLARATION

/**
*  This class implements the interface specified in CMetaDataSource
*  when audio source is from a file.
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataSourceFile : public CMetaDataSource
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
		* @param aFileName The name of the file
		* @return A pointer to a new instance of this class
        */
        static CMetaDataSourceFile* NewL( const TDesC& aFileName );
        
        /**
        * Two-phased constructor.
		* @param aFile File handle
		* @return A pointer to a new instance of this class
        */
        static CMetaDataSourceFile* NewL( RFile& aFile );

        /**
        * Destructor.
        */
        virtual ~CMetaDataSourceFile();

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

		/**
        * Positions the file pointer to aPos.
        * @since 3.0
        * @param aMode seek modes from TSeek
        * @param aPos position to seek
        * @return KErrNone, or one of the systemwide error codes.
		*/
		TInt Seek(TSeek aMode,TInt& aPos) const;
		
		/**
        * Is the file DRM protected
        * @since 3.0
        * @return ETrue if DRM protected, EFalse otherwise
		*/
		TBool FileProtected() const;
		
		/**
        * Does the file have play rights?
        * @since 3.0
        * @return ETrue if file has play rights, EFalse otherwise
		*/
		TBool CanPlay() const;
	
		/**
        * Whats the DRM Type?
        * @since 3.0
        * @return ETrue if file has play rights, EFalse otherwise
		*/
		TInt DrmType() const;
		
		/**
        * Does the file have play rights?
        * @since 3.0
        * @param aAttribute the attribute to get
        * @param aValue the value on return
        * @return KErrNone, or one of the systemwide error codes.
		*/
		TInt GetStringAttribute(TInt aAttribute, TDes& aValue) const;
  
		TInt GetAttributeL(TInt aAttribute, HBufC8** aValue);

		
		/**
        * Return a pointer to the CAF data 
        * @since 3.0
        * @return CData pointer
		*/
		ContentAccess::CData* DataPtr();
		
		/**
        * Returns iFileName to caller 
        * @since 3.2
        * @return TDesC&
		*/
		TDesC8& FileName();	
		
		/**
        * Returns iFileHandler to caller 
        * @since 3.2
        * @return RFile
		*/
		RFile FileHandler();	
		
		/**
        * Allows caller to set iRFile
        * @since 3.2
        * @return void
		*/
		void SetFileHandler(RFile aRFile);		
		
		/**
        * Returns boolean value for iIsFileHandle
        * @since 3.2
        * @return TBool
		*/
		TBool IsFileHandler();	
		
		/**
        * Allows caller to set boolean iIsFileHandler
        * @since 3.2
        * @return void
		*/
		void SetIsFileHandler(TBool aIsFileHandler);
		
		/**
        * Does the file have any rights?
        * @since 3.2
        * @return ETrue if file has any rights, EFalse otherwise
		*/
		TBool KeyExist() const;
						
				
	private:

        /**
        * C++ default constructor.
        */
		CMetaDataSourceFile();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL( const TDesC& aFileName );
		
		/**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL( RFile& aFile );
		
		/**
        * Gets Source Attributes, used by ConstructL
        */
		void GetSourceAttributesL();

 	private:	// Data

		// full path name of the file
		TBuf8<256> iFileName;
		// RFile file handler
		RFile iFileHandler;	
		// Is File Handler
		TBool iIsFileHandler;	
		// pointer to CAF content 
		CContent *iContent;
		// pointer to CAF Data
		CData *iData;
		// DRM protected or not
		TBool iProtected;
		// Play rights or not
		TBool iCanPlay;
		// DRM Type
		TInt iDRMType;
		// Key exist or not
		TBool iKeyExist;
	};

#endif      // CMETADATASOURCE_H

// End of File
