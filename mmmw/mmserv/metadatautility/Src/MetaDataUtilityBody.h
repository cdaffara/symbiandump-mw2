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
* Description:  This class provides implementation body for the API specified
*                by CMetaDataUtility.
*
*/



#ifndef CMETADATAUTILITYBODY_H
#define CMETADATAUTILITYBODY_H

// INCLUDES
#include <e32base.h>
#include <MetaDataField.hrh>
#include <f32file.h>
#include <MDUChunkDataObserver.h>

// FORWARD DECLARATION
class CMetaDataFieldContainer;
class CMetaDataParser;
class CMetaDataSource;
class MMDUChunkDataObserver;
class CMetaDataBufferAgg;

// LITERALS & CONSTANTS

_LIT8(KAmrMimeType, "audio/amr");
_LIT8(KAwbMimeType, "audio/amr-wb");
_LIT8(KMpegMimeType, "audio/mpeg");
_LIT8(KMp3MimeType, "audio/mp3");
_LIT8(KMp4AudioMimeType, "audio/mp4");
_LIT8(K3gpAudioMimeType, "audio/3gpp");
_LIT8(K3g2AudioMimeType, "audio/3gpp2");
_LIT8(KAacMimeType, "audio/aac");
_LIT8(KMidiMimeType, "audio/midi");
_LIT8(KWmaMimeType, "audio/x-ms-wma");
_LIT8(KRmfMimeType, "audio/x-beatnik-rmf");
_LIT8(KRApnMimeType, "audio/x-pn-realaudio");
_LIT8(KRArnMimeType, "audio/vnd.rn-realaudio");
_LIT8(KRAxMimeType, "audio/x-realaudio");

// CLASS DECLARATION

/**
*  This class implements the API specified by CMetaDataUtility.
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataUtilityBody : public CBase
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
		* @return A pointer to a new instance of this class
        */
        static CMetaDataUtilityBody* NewL();

        /**
        * Destructor.
        */
        virtual ~CMetaDataUtilityBody();

    public: // New functions

        /**
        * Opens and reads metadata from the specified file.
        * @since 3.0
		* @param aFileName The name of the file
		* @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
		* @return void
        */
		void OpenFileL( const TDesC& aFileName, const RArray<TMetaDataFieldId>& aWantedFields );

		/**
        * Opens and reads metadata from the specified file handle
        * @since 3.0
		* @param aFile File handle already open
		* @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
		* @return void
        */
		void OpenFileL(RFile& aFile, const RArray<TMetaDataFieldId>& aWantedFields );

        /**
        * Reads metadata from the specified descriptor.
        * @since 3.0
		* @param aDes Source descriptor
		* @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
		* @return void
        */
		void OpenDesL( const TDesC8& aDes, const RArray<TMetaDataFieldId>& aWantedFields  );

        /**
        * Returns the number of metadata.
        * @since 3.0
		* @return Number of metadata
        */
		TInt MetaDataCount() const;

        /**
        * Returns the metadata fields found in this source.
        * Calling this function when MetaDataCount() returns 0, will result in
        * User::Leave with KErrNotFound.
        * @since 3.0
        * @return Metadata field container object
        */
		const CMetaDataFieldContainer& MetaDataFieldsL();

        /**
        * Resets the utility. Must be called before OpenFileL or OpenDesL can be called again.
        * @since 3.0
		* @return void
        */
		void ResetL();

		/**
        * Returns the ID3 version if the file has ID3 metadata.
        * @since 3.0
		* @return TID3Version
        */
		TID3Version ID3Version();
		/**
        * Opens and reads metadata from the specified file.
        * @since 3.2
		* @param aFileName The name of the file
		* @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
		* @param aMimeType The MIME type of the file
		* @return void
        */
		void OpenFileL( const TDesC& aFileName, const RArray<TMetaDataFieldId>& aWantedFields, const TDesC8& aMimeType );

		/**
        * Opens and reads metadata from the specified file handle
        * @since 3.2
		* @param aFile File handle already open
		* @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
		* @param aMimeType The MIME type of the file
		* @return void
        */
		void OpenFileL(RFile& aFile, const RArray<TMetaDataFieldId>& aWantedFields, const TDesC8& aMimeType );

        /**
        * Reads metadata from the specified descriptor.
        * @since 3.2
		* @param aDes Source descriptor
		* @param aWantedFields An array of TMetaDataFieldId's. Empty array is interpreted as all fields.
		* @param aMimeType The MIME type of the file
		* @return void
        */
		void OpenDesL( const TDesC8& aDes, const RArray<TMetaDataFieldId>& aWantedFields, const TDesC8& aMimeType );
		
        /**
        * Inititalization for parsing Chunk Data.
        * @since 5.0
        * @param aMimeType The MIME type
        * @param aObserver should not be NULL
        * @return value
        *      KErrNone
        *      KErrInvalidArgument invalid mime type or null observer
        */
        TInt InitChunkData( TDesC8& aMimeType, 
                            MMDUChunkDataObserver& aObserver );
        
        /** 
        * Process the chunk data.
        * @since 5.0
        *      Max size of buffer passed in is N bytes for performance optimization
        *      Buffer overflow (out of memory) results in callback with error.
        *      invalid chunk data results in callback with error
        *      parsing APIs are invoked before callback is received results in 0 count or empty container
        *      parsing APIs are invoked after callback is received results in positive count and full container
        * @param aChunk the data chunk to be processed
        * @return value
        *      KErrNone
        *      KErrNotReady    Buffer is still being used
        *      KErrArgument    Chunk size exceeds maximum
        *      KErrCompletion  Stop sending data. Meta-data has been captured and is being parsed.
        */
        TInt ProcessChunkData( TDesC8& aChunk,
                               TBool aFinalChunk = EFalse  );
		
        /**
        * Parse the aggregated metadata.
        * @since 5.0
        * This is a request from the user to start parsing
        * @return value
        *      KErrNone
        *      KErrNotReady    Buffer is still being used
        */
        TInt ParseChunkData();
        
        
        /** 
        * Verify whether it is in Chunk Data mode.
        * @since 5.0
        * @return value
        *      ETrue
        *      EFalse        
        */
        TBool IsChunkDataMode();
        
        /** 
        * Close and clean up the Chunk Data mode.
        * Need to kill the MetadataUtility since it wasn't designed to be reused.
        * @since 5.0
		* @return void
        */
        void CloseChunkData();
		
        /**
        * Parses the aggregated metadata.
        * @since 5.0
        * This is a callback from the "Buffer Aggregator" to start parsing after all
        * meta-data has been aggregated. 
        * @param aChunk The aggregated metadata
        * @param aMimeType The MIME type
        * @return void
        */
        void DoParseChunkDataL(TDesC8& aChunk, TDesC8& aMimeType);
        

	private:

        /**
        * C++ default constructor.
        */
		CMetaDataUtilityBody();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

        /**
        * Finds the metadata parser that can interpret the specified file.
        * @param aFileName The name of the file
        * @return Pointer to parser; NULL if unable to find one.
		*/
		CMetaDataParser* FindParserFileL( const TDesC& aFileName );

		/**
        * Finds the metadata parser that can interpret the specified file handle.
        * @param aFile File Handle
        * @return Pointer to parser; NULL if unable to find one.
		*/
		CMetaDataParser* FindParserFileL( RFile& aFile );

		/**
        * Implemetation of finding a file parser, used by FindParserFileL
        * @param none
        * @return Pointer to parser; NULL if unable to find one.
		*/
		CMetaDataParser* DoFindParserFileL();
        /**
        * Finds the metadata parser that can interpret the specified descriptor.
        * @param aDes Source descriptor
        * @return Pointer to parser; NULL if unable to find one.
		*/
		CMetaDataParser* FindParserDesL( const TDesC8& aDes );
		/**
        * Finds the metadata parser that can interpret the specified file.
        * @param aFileName The name of the file
        * @return Pointer to parser; NULL if unable to find one.
		*/
		CMetaDataParser* FindParserFileL( const TDesC& aFileName, const TDesC8& aMimeType );
		/**
        * Finds the metadata parser that can interpret the specified file handle.
        * @since 3.2
        * @param aFile File Handle
        * @return Pointer to parser; NULL if unable to find one.
		*/
		CMetaDataParser* FindParserFileL( RFile& aFile, const TDesC8& aMimeType );
		/**
        * Implemetation of finding a file parser, used by FindParserFileL
        * @since 3.2
        * @param aMimeType
        * @return Pointer to parser; NULL if unable to find one.
		*/
		CMetaDataParser* DoFindParserFileL( const TDesC8& aMimeType);

        /**
        * Finds the metadata parser that can interpret the specified descriptor.
        * @since 3.2
        * @param aDes Source descriptor
        * @return Pointer to parser; NULL if unable to find one.
		*/
		CMetaDataParser* FindParserDesL( const TDesC8& aDes, const TDesC8& aMimeType );

		/**
        * Implemetation of
        * @since 3.2
        * @param aMimeType
        * @return TInt 0(KErrNone) if supported MIME Type else KErrNotSupported(-5)
		*/
		TInt IsSupportedMimeType( const TDesC8& aMimeType);

 	private:	// Data

		// Metadata container
		CMetaDataFieldContainer* iContainer;
		// Metadata parser
		CMetaDataParser* iParser;
		// Metadata source
		CMetaDataSource* iSource;
		// ID3 Version
		TInt iID3Version;
		// Operate in chunk data mode
	    TBool iChunkMode;
	    // Metadata Async
	    CMetaDataBufferAgg* iBufferAgg;
	    
	};

#endif      // CMETADATAUTILITYBODY_H

// End of File
