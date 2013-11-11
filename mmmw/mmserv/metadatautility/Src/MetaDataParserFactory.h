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
* Description:  This class creates an instance of different metadata parsers
*                available in this library.
*
*/



#ifndef CMETADATAPARSERFACTORY_H
#define CMETADATAPARSERFACTORY_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATION
class CMetaDataParser;
class CMetaDataSource;

// CLASS DECLARATION

/**
*  This class creates an instance of different metadata parsers available
*  in this library.
*
*  @lib MetaDataUtility.lib
*  @since 3.0
*/
class CMetaDataParserFactory : public CBase
	{
    public: // New functions

        /**
        * Creates an instance of ID3 parser that can interpret the specified source.
        * @param aSource Reference to a metadata source
		* @return A pointer to a new instance of parser; NULL if parser is not created.
        */
        static CMetaDataParser* CreateID3ParserL( CMetaDataSource& aSource );
        
        /**
        * Creates an instance of ID3 parser that can interpret the specified source.
        * @param aSource Reference to a metadata source
        * @param aOffset File offset of ID3 metadata
        * @return A pointer to a new instance of parser; NULL if parser is not created.
        */
        static CMetaDataParser* CreateID3ParserL( CMetaDataSource& aSource, TUint aOffset );

        /**
        * Creates an instance of MP4 parser that can interpret the specified file.
        * @param aSource Reference to a metadata source
		* @return A pointer to a new instance of parser; NULL if parser is not created.
        */
        static CMetaDataParser* CreateMP4ParserFileL( CMetaDataSource& aSource );

        /**
        * Creates an instance of MP4 parser that can interpret the specified descriptor.
        * @param aDes Source descriptor
		* @return A pointer to a new instance of parser; NULL if parser is not created.
        */
        static CMetaDataParser* CreateMP4ParserDesL( const TDesC8& aDes );
		
		/**
        * Creates an instance of DCF file parser.
        * @param aSource Reference to a metadata source
		* @return A pointer to a new instance of parser; NULL if parser is not created.
        */
        static CMetaDataParser* CreateDcf1FileParserL( CMetaDataSource& aSource );
        
        /**
        * Creates an instance of DCF2 file parser.
        * @param aSource Reference to a metadata source
		* @return A pointer to a new instance of parser; NULL if parser is not created.
        */
        static CMetaDataParser* CreateDcf2FileParserL( CMetaDataSource& aSource );

		/**
        * Creates an instance of WMA file parser.
        * @param aSource Reference to a metadata source
		* @return A pointer to a new instance of parser; NULL if parser is not created.
        */
		static CMetaDataParser* CreateWMAParserFileL( CMetaDataSource& aSource );
		
		/**
        * Creates an instance of RA parser. 
        * @param aSource Reference to a metadata source
		* @return A pointer to a new instance of parser; NULL if parser is not created.
        */
		static CMetaDataParser* CreateRAParserL( CMetaDataSource& aSource );		
	};

#endif      // CMETADATAPARSERFACTORY_H

// End of File
