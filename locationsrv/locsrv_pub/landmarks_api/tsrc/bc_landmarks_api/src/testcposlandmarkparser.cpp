/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides testing of CPosLandmarkParser class methods
*
*/



// INCLUDE FILES
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>
#include <EPos_CPosLandmarkParser.h>
#include <LbsPosition.h>
#include "testcposlandmarkparser.h"
#include <StifParser.h>
#include <Stiftestinterface.h>

// Literals
_LIT8( KPosMimeTypeLandmarkCollectionXml, "application/vnd.nokia.landmarkcollection+xml" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestPosLandmarkParser::CTestPosLandmarkParser
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestPosLandmarkParser::CTestPosLandmarkParser( CStifLogger* aLog )
	{
	iLog = aLog;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkParser::NewL
// 
// 
// -----------------------------------------------------------------------------
//	
CTestPosLandmarkParser* CTestPosLandmarkParser::NewL(CStifLogger* aLog)
	{
	CTestPosLandmarkParser* self = new (ELeave) CTestPosLandmarkParser( aLog );
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); 
    return self; 
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkParser::ConstructL
// 
// 
// -----------------------------------------------------------------------------
//
void CTestPosLandmarkParser::ConstructL()
	{
	
	}	

// -----------------------------------------------------------------------------
// CTestPosLandmarkParser::~CTestPosLandmarkParser
// 
// 
// -----------------------------------------------------------------------------
//
CTestPosLandmarkParser::~CTestPosLandmarkParser()
	{
	
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkParser::CreateParser
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkParser::CreateParserL( CStifItemParser& /*aItem*/ )
	{
	CPosLandmarkParser* parser = CPosLandmarkParser::NewL( KPosMimeTypeLandmarkCollectionXml );
	delete parser;
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkParser::SetInputBuffer
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkParser::SetInputBufferL( CStifItemParser& /*aItem*/ )
	{
	CPosLandmarkParser* parser = CPosLandmarkParser::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( parser );
	// Set buffer
	TBufC8<1024> inputBuffer;
	parser->SetInputBuffer( inputBuffer );
	iLog->Log(_L("SetInputBuffer successful"));
	CleanupStack::PopAndDestroy( parser );
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkParser::SetInputFile
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkParser::SetInputFileL( CStifItemParser& aItem )
	{
	TPtrC inputFilePtr;
	TInt err = aItem.GetNextString( inputFilePtr );
	
	CPosLandmarkParser* parser = CPosLandmarkParser::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( parser );
	// Set input file
	parser->SetInputFileL( inputFilePtr );
	CleanupStack::PopAndDestroy( parser );
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkParser::SetInputFileHandle
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkParser::SetInputFileHandleL( CStifItemParser& aItem )
	{
	TPtrC inputFilePtr;
	TInt err = aItem.GetNextString( inputFilePtr );
		
	CPosLandmarkParser* parser = CPosLandmarkParser::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( parser );
	// File server session
	RFs rfs;
	TInt error = KErrNone;
	error = rfs.Connect();
	if( KErrNone != error )
		{
		iLog->Log(_L("SetInputFileHandle failed"));
		User::Leave( KErrGeneral );
		}
	// Open file
	RFile file;
	error = file.Open( rfs, inputFilePtr, EFileRead );
	if( KErrNone != error )
		{
		iLog->Log(_L("SetInputFileHandle failed"));
		rfs.Close();
		User::Leave( KErrNotFound );
		}
	// Set input file handle
	parser->SetInputFileHandleL( file );
	iLog->Log(_L("SetInputFileHandle successful"));
	CleanupStack::PopAndDestroy( parser );
	ReleaseLandmarkResources();
	file.Close();
	rfs.Close();
	return KErrNone;
	}			

// -----------------------------------------------------------------------------
// CTestPosLandmarkParser::ParseContent
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkParser::ParseContentL( CStifItemParser& aItem )
	{
	TPtrC inputFilePtr;
	TInt err = aItem.GetNextString( inputFilePtr );
		
	CPosLandmarkParser* parser = CPosLandmarkParser::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( parser );
	// Set input file
	parser->SetInputFileL( inputFilePtr );
	// Parse Content
	CPosLmOperation* operation = parser->ParseContentL( EFalse );
	CleanupStack::PushL( operation );
	// Execute operation
	operation->ExecuteL();
	iLog->Log(_L("ParseContent successful"));
	CleanupStack::PopAndDestroy( 2, parser );
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkParser::NumOfParsedContent
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkParser::NumOfParsedContentL( CStifItemParser& aItem )
	{
	TPtrC inputFilePtr;
	TInt err = aItem.GetNextString( inputFilePtr );
	
	CPosLandmarkParser* parser = CPosLandmarkParser::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( parser );
	// Set input file
	parser->SetInputFileL( inputFilePtr );
	// Parse Content
	CPosLmOperation* operation = parser->ParseContentL( EFalse );
	CleanupStack::PushL( operation );
	// Execute operation
	operation->ExecuteL();
	// Check and print number
	TUint32 numOfParsedLmk = parser->NumOfParsedLandmarks();
	iLog->Log(_L("NumOfParsedContent successful"));
	CleanupStack::PopAndDestroy( 2, parser );
	ReleaseLandmarkResources();
	return KErrNone;
	}	
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkParser::FirstAndNextCollDataId
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkParser::FirstAndNextCollDataIdL( CStifItemParser& aItem )
	{
	TPtrC inputFilePtr;
	TInt err = aItem.GetNextString( inputFilePtr );
	
	CPosLandmarkParser* parser = CPosLandmarkParser::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( parser );
	// Set input file
	parser->SetInputFileL( inputFilePtr );
	// Parse Content
	CPosLmOperation* operation = parser->ParseContentL( EFalse );
	CleanupStack::PushL( operation );
	// Execute operation
	operation->ExecuteL();
	// Get first collection data id
	TPosLmCollectionDataId currentId = parser->FirstCollectionDataId();
	// Check
	if( EPosLmCollDataNone == currentId )
		{
		// No coll data found
		iLog->Log(_L("FirstAndNextCollDataId fails, no collection data exists in the input file"));
		User::Leave( KErrNotFound );
		}
	// Get next ids
	TPosLmCollectionDataId nextId;
	while( ( nextId = parser->NextCollectionDataId( currentId ) )!= EPosLmCollDataNone )
		{
		currentId = nextId;
		}
	iLog->Log(_L("FirstAndNextCollDataId successful"));
	CleanupStack::PopAndDestroy( 2, parser );
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkParser::GetCollectionData
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkParser::GetCollectionDataL( CStifItemParser& aItem )
	{
	TPtrC inputFilePtr;
	TInt err = aItem.GetNextString( inputFilePtr );
	
	CPosLandmarkParser* parser = CPosLandmarkParser::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( parser );
	// Set input file
	parser->SetInputFileL( inputFilePtr );
	// Parse Content
	CPosLmOperation* operation = parser->ParseContentL( EFalse );
	CleanupStack::PushL( operation );
	// Execute operation
	operation->ExecuteL();
	// Get first collection data id
	TPosLmCollectionDataId currentId = parser->FirstCollectionDataId();
	// Check
	if( EPosLmCollDataNone == currentId )
		{
		// No coll data found
		iLog->Log(_L("FirstAndNextCollDataId fails, no collection data exists in the input file"));
		User::Leave( KErrNotFound );
		}
	// Get collection data and print
	TPtrC ptr = parser->CollectionData( currentId );
	iLog->Log( ptr );
	iLog->Log(_L("GetCollectionData successful"));
	CleanupStack::PopAndDestroy( 2, parser );
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkParser::RetrieveParsedLmk
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkParser::RetrieveParsedLmkL( CStifItemParser& aItem )
	{
	TPtrC inputFilePtr;
	TInt err = aItem.GetNextString( inputFilePtr );
	
	CPosLandmarkParser* parser = CPosLandmarkParser::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( parser );
	// Set input file
	parser->SetInputFileL( inputFilePtr );
	// Parse Content
	CPosLmOperation* operation = parser->ParseContentL( EFalse );
	CleanupStack::PushL( operation );
	// Execute operation
	operation->ExecuteL();
	// Retrieve the last parsed landmark
	CPosLandmark* landmark = parser->LandmarkLC(); // Last parsed, indexing is not supported
	TPtrC lmkName;
	landmark->GetLandmarkName( lmkName );
	iLog->Log( lmkName );
	iLog->Log(_L("RetrieveParsedLmk successful"));
	CleanupStack::PopAndDestroy( 3, parser );
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkParser::RetrieveCategory
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkParser::RetrieveCategoryL( CStifItemParser& aItem )
	{
	TPtrC inputFilePtr;
	TInt err = aItem.GetNextString( inputFilePtr );
	CPosLandmarkParser* parser = CPosLandmarkParser::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( parser );
	// Set input file
	parser->SetInputFileL( inputFilePtr );
	// Parse Content
	CPosLmOperation* operation = parser->ParseContentL( EFalse );
	CleanupStack::PushL( operation );
	// Execute operation
	operation->ExecuteL();
	// Retrieve the last parsed landmark
	CPosLandmark* landmark = parser->LandmarkLC(); // Last parsed
	// Retrieve Category
	TPosLmItemId id = 1; // Get first category for landmark
	CPosLandmarkCategory* category = parser->LandmarkCategoryLC( id );
	iLog->Log(_L("RetrieveCategory successful"));
	CleanupStack::PopAndDestroy( 4, parser );
	ReleaseLandmarkResources();
	return KErrNone;
	}	
