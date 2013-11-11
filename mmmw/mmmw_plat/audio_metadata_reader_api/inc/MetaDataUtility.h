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
* Description:  This class is the main interface to MetaDataUtility.
*
*/



#ifndef CMETADATAUTILITY_H
#define CMETADATAUTILITY_H

// INCLUDES
#include <e32base.h>
#include <MetaDataField.hrh>
#include <f32file.h>
#include <MDUChunkDataObserver.h>

// FORWARD DECLARATION
class CMetaDataFieldContainer;
class CMetaDataUtilityBody;

// CLASS DECLARATION

/**
*  This class implements the main interface to this library.
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataUtility : public CBase
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @since 3.0
		* @return A pointer to a new instance of this class
        */
        IMPORT_C static CMetaDataUtility* NewL();

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CMetaDataUtility();

    public: // New functions

        /**
        * Opens and reads metadata from the specified file.
        * @since 3.0
        * @param aFileName The name of the file
		* @return void
        */
		IMPORT_C void OpenFileL( const TDesC& aFileName );
        /**
        * Opens and reads metadata from the specified file.
        * @since 3.0
		* @param aFileName The name of the file
		* @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
		* @return void
        */
		IMPORT_C void OpenFileL( const TDesC& aFileName, const RArray<TMetaDataFieldId>& aWantedFields );

		/**
		* Opens and reads metadata from the specified file handle.
	    * CContent will use a duplicate of this file handle, the client is 
	    * free to close its file handle any time after calling this API 
        * @since 3.0
        * @param aFile The file handle. The file must already be open before calling this API.
	    * @return void
		*/
		IMPORT_C void OpenFileL( RFile& aFile);
		
		/**
		* Opens and reads metadata from the specified file handle.
	    * CContent will use a duplicate of this file handle, the client is 
	    * free to close its file handle any time after calling this API 
        * @since 3.0
        * @param aFile The file handle. The file must already be open before calling this API.
	    * @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
	    * @return void
		*/
		IMPORT_C void OpenFileL( RFile& aFile, const RArray<TMetaDataFieldId>& aWantedFields );

        /**
        * Reads metadata from the specified descriptor.
        * @since 3.0
        * @param aDes Source descriptor
		* @return void
        */
		IMPORT_C void OpenDesL( const TDesC8& aDes );

        /**
        * Reads metadata from the specified descriptor.
        * @since 3.0
		* @param aDes Source descriptor
		* @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
		* @return void
        */
		IMPORT_C void OpenDesL( const TDesC8& aDes, const RArray<TMetaDataFieldId>& aWantedFields  );

        /**
        * Returns the number of metadata.
        * @since 3.0
		* @return Number of metadata
        */
		IMPORT_C TInt MetaDataCount() const;

        /**
        * Returns the metadata fields found in this source.
        * Calling this function when MetaDataCount() returns 0, will result in
        * User::Leave with KErrNotFound.
        * @since 3.0
        * @return Metadata field container object
        */
		IMPORT_C const CMetaDataFieldContainer& MetaDataFieldsL();

        /**
        * Resets the utility. Must be called before OpenFileL or OpenDesL can be called again.
        * @since 3.0
		* @return void
        */
		IMPORT_C void ResetL();
		
		/**
        * Retunrs the ID3 version if the file has ID3 metadata. 
        * Must be called after OpenFileL()
        * @since 3.0
		* @return TID3Version
        */
		IMPORT_C TID3Version ID3Version();

		/*
        * Opens and reads metadata from the specified file.
        * @since 3.0
        * @param aFileName The name of the file
		* @param The MIME type 
		* @return void
        */
		IMPORT_C void OpenFileL( const TDesC& aFileName, const TDesC8& aMimeType );
        /**
        * Opens and reads metadata from the specified file.
        * @since 3.0
		* @param aFileName The name of the file
		* @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
		* @param The MIME type 
		* @return void
        */
		IMPORT_C void OpenFileL( const TDesC& aFileName, const RArray<TMetaDataFieldId>& aWantedFields, const TDesC8& aMimeType );

		/**
		* Opens and reads metadata from the specified file handle.
	    * CContent will use a duplicate of this file handle, the client is 
	    * free to close its file handle any time after calling this API 
        * @since 3.0
        * @param aFile The file handle. The file must already be open before calling this API.
	    * @param The MIME type 
		* @return void
		*/
		IMPORT_C void OpenFileL( RFile& aFile, const TDesC8& aMimeType);
		
		/**
		* Opens and reads metadata from the specified file handle.
	    * CContent will use a duplicate of this file handle, the client is 
	    * free to close its file handle any time after calling this API 
        * @since 3.0
        * @param aFile The file handle. The file must already be open before calling this API.
	    * @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
	    * @param The MIME type 
		* @return void
		*/
		IMPORT_C void OpenFileL( RFile& aFile, const RArray<TMetaDataFieldId>& aWantedFields, const TDesC8& aMimeType );

        /**
        * Reads metadata from the specified descriptor.
        * @since 3.0
        * @param aDes Source descriptor
        * @param The MIME type 
		* @return void
        */
		IMPORT_C void OpenDesL( const TDesC8& aDes, const TDesC8& aMimeType );

        /**
        * Reads metadata from the specified descriptor.
        * @since 3.0
		* @param aDes Source descriptor
		* @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
		* @param aMimeType The MIME type 
		* @return void
        */
		IMPORT_C void OpenDesL( const TDesC8& aDes, const RArray<TMetaDataFieldId>& aWantedFields, const TDesC8& aMimeType );
				
        /**
        * Inititalization for parsing Chunk Data.
        * NOTE: Currently the following MIME types are supported:
        *      - audio/mpeg
        *      - audio/mp3
        * @since 5.0
        * @param aMimeType The MIME type
        * @param aObserver should not be NULL
        * @return value
        *      KErrNone
        *      KErrInvalidArgument invalid Mime type or null observer
        */
        IMPORT_C TInt InitChunkData( const TDesC8& aMimeType, MMDUChunkDataObserver& aObserver );
        
        /** 
        * Process the chunk data.
        * @since 5.0
        *      User should call this once, then wait for one of these callbacks :
        *            - HandleChunkDataProcessed : will be called to indicate ProcessChunkData()
        *                                         should be called again to pass the next chunk
        *            - HandleChunkDataReadyToBeParsed : will be called to indicate ParseChunkData()
        *                                         should be called to start parsing the metadata         
        *      Invalid chunk data results in callback with error.
        *      Invoking parsing APIs before ParseChunkData() is called, will result in 0 count.
        * @param aChunk the data chunk to be processed
        * @param aFinalChunk indicate this is the final chunk
        * @return value
        *      KErrNone
        *      KErrNotReady    Buffer is still being used
        *      KErrCompletion  Stop sending data. Meta-data has been captured.
        *      KErrAlreadyExists    Metadata has already been parsed
        */
        IMPORT_C TInt ProcessChunkData( const TDesC8& aChunk,
                                        TBool aFinalChunk = EFalse );
		
        /** 
        * Parse the chunk data.
        * @since 5.0
        *      User should call this, after receiving the HandleChunkDataReadyToBeParsed() callback.
        *      All metadata has been collected, and it is ready to be parsed. 
        *      Parsing APIs are invoked after HandleParseDataCompeted() callback.
        *      Calling this function before receiving HandleChunkDataReadyToBeParsed() will return error.
        *      Invoking parsing APIs before ParseChunkData() is called, will result in 0 count.
        * @return value
        *      KErrNone
        *      KErrNotReady         Metadata is still being collected or parsed
        *      KErrAlreadyExists    Metadata has already been parsed
        */
        IMPORT_C TInt ParseChunkData();
        
        
        /** 
        * Verify whether it is in Chunk Data mode.
        * @since 5.0
        * @return value
        *      ETrue
        *      EFalse        
        */
        IMPORT_C TBool IsChunkDataMode();
        
        /** 
        * Close and clean up the Chunk Data mode.
        * Need to kill the MetadataUtility since it wasn't designed to be reused.
        * @since 5.0
        * @return void
        */
        IMPORT_C void CloseChunkData();
		
	private:

        /**
        * C++ default constructor.
        */
		CMetaDataUtility();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

 	private:	// Data

 		// Metadata utility implementation
 		CMetaDataUtilityBody* iBody;
	};

#endif      // CMETADATAUTILITY_H

// End of File
