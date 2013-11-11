/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*
*/


#ifndef __CHARVESTERM3UPLAYLISTPARSER_H__
#define __CHARVESTERM3UPLAYLISTPARSER_H__

#include <charconv.h>

//
// CAudioMDParser
//
class CHarvesterM3UPlaylistParser : public CBase
	{
	public:
		static CHarvesterM3UPlaylistParser* NewL( RFs& aFs,
		        CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* aAvailableCharacterSet,
		        CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* aTopCharacterSet );
		virtual ~CHarvesterM3UPlaylistParser();
        
        TBool ParseL( const TDesC& aFileName, RPointerArray<HBufC>& aUriArray );
        void Reset();
		
	private:
        CHarvesterM3UPlaylistParser( RFs& aFs,
                CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* aAvailableCharacterSet,
                CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* aTopCharacterSet );

        /**
        * Reads data from playlist file to the buffer
        */
        void ReadPlaylistFileToBufferL();
        
        /**
        * Parses the buffer where playlist file was read to.
        */
        void ParsePlaylistBufferL(
        		RPointerArray<HBufC>& aPlaylist,
                TInt& aInvalidItemCount);
    
        /**
        * Auto detects the character encoding from the supplied character
        * set
        * @param aSample a sample of the file
        * @param aCharSetId auto-detected character set for the supplied
        *        sample
        * @param aCharacterSet a character set to detect the encoding from
        * @return KErrNone if a character set is found; otherwise
        *         KErrNotFound.
        */
        TInt DetectCharacterSetL(
            const TDesC8& aSample,
            const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>& aCharacterSet,
            TUint& aCharSetId );
        
        /**
        * Reads next line from iBuffer and stores it to iLine. Returns EFalse
        * if there are no more new lines.
        */
        TBool ReadNextLineL();
        
        /**
        * Decides what to do with an iLine read from iBuffer
        */
        void ProcessLineL(
        		RPointerArray<HBufC>& aPlaylist,
                TInt& aInvalidItemCount);
        
        /**
        * Parses path or extended info from an iLine and stores them to iItem
        */
        TInt ParseLineL(
                TFileName& aItem,
                TInt& aInvalidItemCount);
        
        /**
        * Parses and returns an absolute path if aPath is relative to playlist
        * file's path. If path is not valid or it doesn't exist, error code is
        * returned in aError.
        *
        * caller assumes ownership of the returned HBufC
        */
        HBufC* ParseAbsolutePathLC(
                const TDesC& aPath,
                TInt& aError);
        
    private:
        RFs& iFs;
        
        TPtrC iPlaylistFilePath;
        
        TInt iInvalidItems;
        
        HBufC* iBuffer;
        TPtrC iBufferPtr;
        
        // Not owned.
        CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* iAvailableCharacterSet;
        CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* iTopCharacterSet;
        
        TInt iEndLineNumber;  
        TInt iCurrentLineNumber;
        
        HBufC* iLine;
        
        TBool iExtendedFormat;
        
        TFileName iItem;
	};



#endif  // __CHARVESTERM3UPLAYLISTPARSER_H__

