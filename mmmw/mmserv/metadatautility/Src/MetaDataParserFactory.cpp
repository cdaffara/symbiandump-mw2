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



// INCLUDE FILES

#include 	"MetaDataParserFactory.h"
#include 	"MetaDataParserMP4.h"
#include	"MetaDataParserID3v1.h"
#include	"MetaDataParserID3v2.h"
#include	"MetaDataParserMP4.h"
#include	"MetaDataParser3GPExt.h"
#include	"MetaDataParser3gp.h"
#include	"MetaDataParserDcf.h"
#include	"MetaDataParserDcf2.h"
#include	"MetaDataSourceFile.h"
#include	"MetaDataParserWMA.h"
#include 	"MetaDataParserRA.h"
#include	"MetaDataSource.h"
#ifdef _DEBUG
#include	<e32svr.h>
#endif


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataParserFactory::CreateID3ParserL
// -----------------------------------------------------------------------------
//
CMetaDataParser* CMetaDataParserFactory::CreateID3ParserL(
	CMetaDataSource& aSource )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserFactory::CreateID3ParserL"));
#endif

	return CreateID3ParserL( aSource, 0 );
	}

// -----------------------------------------------------------------------------
// CMetaDataParserFactory::CreateID3ParserL
// -----------------------------------------------------------------------------
//
CMetaDataParser* CMetaDataParserFactory::CreateID3ParserL(
    CMetaDataSource& aSource, TUint aOffset )
    {
#ifdef _DEBUG
    RDebug::Print(_L("CMetaDataParserFactory::CreateID3ParserL"));
#endif
	CMetaDataParser* parser = NULL;
    TRAPD( err, parser = CMetaDataParserID3v2::CreateID3v2ParserL( aSource, aOffset ));
	if ( !err && parser )
		{
		if ( parser->MetaDataExists() )
			{
			return parser;
			}
		else
			{
			delete parser;
			parser = NULL;
			}
		}

	parser = CMetaDataParserID3v1::NewL( aSource );
	if ( parser )
		{
		if ( parser->MetaDataExists() )
			{
			return parser;
			}
		else
			{
			delete parser;
			parser = NULL;
			}
		}

	// None found
	return NULL;
	}

// -----------------------------------------------------------------------------
// CMetaDataParserFactory::CreateMP4ParserFileL
// -----------------------------------------------------------------------------
//
CMetaDataParser* CMetaDataParserFactory::CreateMP4ParserFileL(
	CMetaDataSource& aSource )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserFactory::CreateMP4ParserFileL"));
#endif

	CMetaDataSourceFile *source = (CMetaDataSourceFile*)&aSource;

	CMetaDataParserMP4* parser = NULL;
	parser = CMetaDataParser3GPExt::NewL();
	CleanupStack::PushL( parser );
	parser->InitializeFileL(source->DataPtr());
	if ( parser->MetaDataExists() )
		{
		CleanupStack::Pop();	// parser
		return parser;
		}
	else
		{
		CleanupStack::PopAndDestroy();	// parser 
		parser = NULL;
		}

	parser = CMetaDataParser3gp::NewL();
	CleanupStack::PushL( parser );
	parser->InitializeFileL(source->DataPtr());
	if ( parser->MetaDataExists() )
		{
		CleanupStack::Pop();	// parser
		return parser;
		}
	
	else	
		{
		CleanupStack::PopAndDestroy();	// parser 
		}

	return NULL;
	}

// -----------------------------------------------------------------------------
// CMetaDataParserFactory::CreateMP4ParserDesL
// -----------------------------------------------------------------------------
//
CMetaDataParser* CMetaDataParserFactory::CreateMP4ParserDesL(
	const TDesC8& aDes )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserFactory::CreateMP4ParserDesL"));
#endif
	CMetaDataParserMP4* parser = NULL;
	parser = CMetaDataParser3GPExt::NewL();
	CleanupStack::PushL( parser );
	parser->InitializeDesL( aDes);
	if ( parser->MetaDataExists() )
		{
		CleanupStack::Pop();	// parser
		return parser;
		}
	else
		{
		CleanupStack::PopAndDestroy();	// parser
		parser = NULL;
		}
	
	parser = CMetaDataParser3gp::NewL();
	CleanupStack::PushL( parser );
	parser->InitializeDesL( aDes);
	if ( parser->MetaDataExists() )
		{
		CleanupStack::Pop();	// parser
		return parser;
		}
	else
		{
		CleanupStack::PopAndDestroy();	// parser
		parser = NULL;
		}
	return NULL;
	}

// -----------------------------------------------------------------------------
// CMetaDataParserFactory::CreateDcfFileParserL
// -----------------------------------------------------------------------------
//
CMetaDataParser* CMetaDataParserFactory::CreateDcf1FileParserL(
	CMetaDataSource &aSource)
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserFactory::CreateDcfFileParserL"));
#endif
	CMetaDataParser* parser = NULL;
	parser = CMetaDataParserDcf::NewL();
	CleanupStack::PushL( parser );
	((CMetaDataParserDcf*)parser)->InitializeFileL(&aSource);
	if ( parser->MetaDataExists() )
		{
		CleanupStack::Pop(parser);	// parser
		return parser;
		}
	else
		{
		CleanupStack::PopAndDestroy();	// parser
		}
	return NULL;
	}


// -----------------------------------------------------------------------------
// CMetaDataParserFactory::CreateDcfFileParserL
// -----------------------------------------------------------------------------
//
CMetaDataParser* CMetaDataParserFactory::CreateDcf2FileParserL(
	CMetaDataSource &aSource)
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserFactory::CreateDcfFileParserL"));
#endif
	CMetaDataParser* parser = NULL;
	parser = CMetaDataParserDcf2::NewL();
	CleanupStack::PushL( parser );
	((CMetaDataParserDcf2*)parser)->InitializeFileL(&aSource);
	if ( parser->MetaDataExists() )
		{
		CleanupStack::Pop(parser);	// parser
		return parser;
		}
	else
		{
		CleanupStack::PopAndDestroy();	// parser
		}
	return NULL;
	}

// -----------------------------------------------------------------------------
// CMetaDataParserFactory::CreateWMAParserFileL
// -----------------------------------------------------------------------------
//
CMetaDataParser* CMetaDataParserFactory::CreateWMAParserFileL(
	CMetaDataSource& aSource )
{
	CMetaDataParser* parser = NULL;
	parser = CMetaDataParserWMA::NewL(aSource);
	if (parser )
		{
		if ( parser->MetaDataExists() )
			{
			return parser;
			}
		else
			{
			delete parser;
			parser = NULL;
			}
		}
	return NULL;
}

// -----------------------------------------------------------------------------
// CMetaDataParserFactory::CreateRAParserL
// -----------------------------------------------------------------------------
//
CMetaDataParser* CMetaDataParserFactory::CreateRAParserL(
	CMetaDataSource& aSource )
{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserFactory::CreateRAParserL"));
#endif

	CMetaDataParser* parser = NULL;	
	TRAPD( err, parser = CMetaDataParserRA::NewL(aSource));
	if ( !err && parser )
		{
		if ( parser->MetaDataExists() )
			{
			return parser;
			}
		else
			{
			delete parser;
			parser = NULL;
			}
		}

	return parser;
}
//  End of File
