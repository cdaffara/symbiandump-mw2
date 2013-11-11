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


#ifndef __CHARVESTERAUDIOPLAYLISTPARSER_H__
#define __CHARVESTERAUDIOPLAYLISTPARSER_H__

#include <charconv.h>
#include <barsc.h>

#include "harvesterm3uplaylistparser.h"

//
// CAudioMDParser
//
class CHarvesterAudioPlaylistParser : public CBase
	{
	public:
		static CHarvesterAudioPlaylistParser* NewL();
		virtual ~CHarvesterAudioPlaylistParser();
        
        TBool ParseMimeType( const TDesC& aFileName, TDes& aMimeType );
        TBool ParseL( const TDesC& aFileName, RPointerArray<HBufC>& aUriArray, TPtrC aMimeType );
        void Reset();
		
	private:
        CHarvesterAudioPlaylistParser();
        void ConstructL();
        
        /**
        * Generate to character sets based on locale.
        */
        void GenerateTopCharacterSetsL();
        
        /**
        * Select character set(s) for the specified language.
        * @param aLanguage language to select character sets for
        */
        void SelectCharacterSetsForLanguageL( TInt aLanguage );
     
        /**
        * reads the character set for the specified resource.
        * @param aResourceId id of the resource to read from the resource file
        */
        void ReadCharacterSetResourceL( TInt aResourceId );

        /**
        * Determine whether the given character set is specified
        * as top character set.
        * @param aCharacterSetId id of the character set to be tested
        */
        TBool IsInTopCharacterSet( TUint aCharacterSetId );
        
    private:
        RFs iFs;
        
        CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* iTopCharacterSet;
        CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* iAvailableCharacterSet;
        
        RResourceFile iRscFile;
        
        CHarvesterM3UPlaylistParser* iM3UParser;
	};



#endif  // __CHARVESTERAUDIOPLAYLISTPARSER_H__

