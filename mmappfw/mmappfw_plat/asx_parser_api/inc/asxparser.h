/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Simple v2/v3 ASX-fileparser
 *                Finds streaming URLs from ASX files and provides an
 *                interface to query them
 *
*/

// Version : %version: 4.1.2 %



#ifndef CASXPARSER_H
#define CASXPARSER_H

// INCLUDES
#include <f32file.h>


// FORWARD DECLARATIONS

// CLASS DECLARATIONS


struct AsxStruct {
    HBufC8* url;
    TBool   seek;
};

/*
 *  CAsxParser.
 *  @since 3.2
 */
class CAsxParser : public CBase
{
    public:    // Constructors and destructor

        /*
         * Two-phased constructor.
         * @since 3.2
         * @param const TDesC& File to parse
         * @return CAsxParser object initialized
         */
        IMPORT_C static CAsxParser* NewL( const TDesC& aFileName );

        /*
         * Two-phased constructor.
         * @since 5.0
         * @param aFile file handle
         * @return CAsxParser object initialized
         */
        IMPORT_C static CAsxParser* NewL( RFile& aFile );

        /*
         * Destructor.
         */
        IMPORT_C ~CAsxParser();

        /*
         * GetUrl
         * @since 3.2
         * @param const TUint The number of URLs in the file
         * @return TInt
         */
        IMPORT_C void GetUrlCount( TUint &aHowManyUrls );

        /*
         * GetUrl
         * @since 3.2
         * @param const TInt The index number of the URL to retrieve
         * @return TInt
         */
        IMPORT_C TInt GetUrl( const TUint aUrlIndex, TPtrC8& aUrl );
        IMPORT_C AsxStruct* GetUrl( const TUint aUrlIndex );

        /*
         * GetUrl
         * @since 3.2
         * @param none
         * @return TInt
         */
        IMPORT_C TInt FileVersion();         
		
        /*
         * PrintUrl
         * @since 5.0
         * @param aUrl8
         * @return aUrl
         */
		IMPORT_C void PrintUrl( TPtrC8& aUrl8, TPtr& aUrl );


    private:    // New methods

        /*
         * C++ default constructor.
         */
        CAsxParser();

        /*
         * By default Symbian 2nd phase constructor is private.
         * @param TDesC& File to parse
         * @return void
         */
        void ConstructL( const TDesC& aFileName );

		/*
         * By default Symbian 2nd phase constructor is private.
         * @since 5.0
         * @param aFile file handle
         * @return void
         */
        void ConstructL( RFile& aFile );

        /*
         * Parse a Asx V2 Header file
         * @param   TPtr8 pointer to buffer containing asx file
         * @return  void
         */
        void ParseAsxV2HeaderL( TPtr8 aPtr );

        /*
         * Parse a Asx V3 Header file
         * @param   TPtr8 pointer to buffer containing asx file
         * @return  void
         */
        void ParseAsxV3HeaderL( TPtr8 aPtr );
       
        /*
         * Handle common file parsing
         * @since 5.0
         * @param aFile file handle
         * @return  void
         */
		void HandleFileParsingL( RFile& aFile );
        

    private:    // Data

        RPointerArray<AsxStruct>    iAsxArray;
        TInt                       iVersion;
};

#endif      // CBAVPCONTROLLERVIDEO_H

// End of File
