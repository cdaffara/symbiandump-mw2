/*
* ==============================================================================
*  Name        : 3GPExtParser.h
*  Part of     : 3GPExtParser
*  Interface   : ?Interface_category, ?Interface_name
*  Description : This class implements an 3GPExt metadata parser 
*  Version     : 6
*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
* ==============================================================================
*/


#ifndef C3GPEXTPARSER_H
#define C3GPEXTPARSER_H

// INCLUDES
#include <e32base.h>
#include <mp4lib.h>

#include "MetaDataFieldContainer.h"
#include "MetaDataField.hrh"

// CONSTANTS

_LIT8 (K3GPExtMetaTitleFormat, "%cnam"); 
_LIT8 (K3GPExtMetaArtistFormat, "%cART"); 
_LIT8 (K3GPExtMetaComposerFormat, "%cwrt"); 
_LIT8 (K3GPExtMetaAlbumFormat, "%calb"); 
_LIT8 (K3GPExtMetaCommentFormat, "%ccmt"); 
_LIT8 (K3GPExtMetaCustomGenreFormat, "%cgen");  // mapped to rating
_LIT8 (K3GPExtMetaYearFormat, "%cday"); 
_LIT8 (K3GPExtMetaGenre, "gnre");
_LIT8 (K3GPExtMetaTrack, "trkn"); 
_LIT8 (K3GPExtMetaJpeg, "covr"); 


// CLASS DECLARATION

/**
*  This class implements an 3GPExt metadata parser.
*
*  @lib 3GPExtParser.lib
*  @since 3.0
*/
class C3GPExtParser : public CBase
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @return A pointer to a new instance of parser; NULL if parser is not created.
        */
        IMPORT_C static C3GPExtParser* NewL(MP4Handle& aMP4Handle);

        /**
        * Destructor.
        */
        IMPORT_C virtual ~C3GPExtParser();
        
        /**
		* Extract meta data boxes from ilst
		* @since 3.0
		* @param aBox wanted metadata box
        * @param aFieldId metadata field in 3GPExtParser
		* @return void
		*/
		IMPORT_C void GetilstBoxesL(const TDesC8& aBox, TMetaDataFieldId aFieldId, HBufC** aBuf);
		/**
		* Extract meta data boxes from ilst
		* 8-bit version to support international languages in string metadata
		* @since 3.1
		* @param aBox wanted metadata box
        * @param aFieldId metadata field in 3GPExtParser
		* @return void
		*/
		IMPORT_C void GetilstBoxesL(const TDesC8& aBox, TMetaDataFieldId aFieldId, HBufC8** aBuf);
				
		/** 
		* Checks if file has 3GPExt metadata
		*/ 
		IMPORT_C TBool Is3GPExtMetadataL();

 	private:

        /**
        * C++ default constructor.
        */
		C3GPExtParser(MP4Handle& aMP4Handle);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();
		
		/**
        * Maps ID3v1 genre integer to string
        * @param aNum genre integer code
        * @param aGenrePtr string genre value returned
        * @since 3.0
        * @return void
        */
		void MapID3GenreToString(TInt aNum, TDes& aGenrePtr);
		
		/**
        * Translates error code from MP4 library to one of System Errors.
        * @since 3.0
        * @param aError error code from MP4 library
        * @return System error
        */
		TInt TranslateMP4Err( MP4Err aError );

		/**
        * Parses iList Boxes
        * @since 3.0
        * @param none
        * @return void 
        */
		void ParseIlstBoxesL();

	private:
	
		// MP4 Handle
		MP4Handle& iMP4Handle;
		// ilst box
		TPtrC8 ilst;
		// 3GPExt metadata exists
		TBool iExists;
		// local chunk
		RChunk iChunk;
		// chuck status
		TInt iChunkStatus;
 			
		
		TUint32 iTitleOffset;
		TUint32 iTitleSize;
 		TUint32 iArtistOffset;	
 		TUint32 iArtistSize;
 		TUint32 iComposerOffset;
 		TUint32 iComposerSize;
 		TUint32 iAlbumOffset;
 		TUint32 iAlbumSize;
 		TUint32 iCommentOffset;
 		TUint32 iCommentSize;
 		TUint32 iCustomGenreOffset;
 		TUint32 iCustomGenreSize;
 		TUint32 iYearOffset;
 		TUint32 iYearSize;
 		TUint32 iGenreOffset;
 		TUint32 iGenreSize;
 		TUint32 iTrackNumberOffset;
 		TUint32 iTrackNumberSize;
 		TUint32 iCoverOffset;
 		TUint32 iCoverSize;
 		
 	public:
 				// constants
		TBuf8<4> K3GPExtMetaTitle;
		TBuf8<4> K3GPExtMetaArtist;
		TBuf8<4> K3GPExtMetaComposer;
		TBuf8<4> K3GPExtMetaAlbum;
		TBuf8<4> K3GPExtMetaComment;
		TBuf8<4> K3GPExtMetaCustomGenre;
		TBuf8<4> K3GPExtMetaYear;
    };

#endif      // C3GPEXTPARSER_H

// End of File
