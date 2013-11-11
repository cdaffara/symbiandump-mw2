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
* Description:  Provides testing of CPosLandmarkEncoder class methods
*
*/



// INCLUDE FILES
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <LbsPosition.h>
#include "testcposlandmarkencoder.h"
#include <StifParser.h>
#include <Stiftestinterface.h>

// Literals
_LIT8( KPosMimeTypeLandmarkCollectionXml, "application/vnd.nokia.landmarkcollection+xml" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestPosLandmarkEncoder::CTestPosLandmarkEncoder
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestPosLandmarkEncoder::CTestPosLandmarkEncoder( CStifLogger* aLog )
	{
	iLog = aLog;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkEncoder::NewL
// 
// 
// -----------------------------------------------------------------------------
//	
CTestPosLandmarkEncoder* CTestPosLandmarkEncoder::NewL(CStifLogger* aLog)
	{
	CTestPosLandmarkEncoder* self = new (ELeave) CTestPosLandmarkEncoder( aLog );
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); 
    return self; 
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkEncoder::ConstructL
// 
// 
// -----------------------------------------------------------------------------
//
void CTestPosLandmarkEncoder::ConstructL()
	{
	
	}	

// -----------------------------------------------------------------------------
// CTestPosLandmarkEncoder::ConstructL
// 
// 
// -----------------------------------------------------------------------------
//
CTestPosLandmarkEncoder::~CTestPosLandmarkEncoder()
	{
	
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkEncoder::CreateEncoder
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkEncoder::CreateEncoderL( CStifItemParser& /*aItem*/ )
	{	
	CPosLandmarkEncoder* encoder = CPosLandmarkEncoder::NewL( KPosMimeTypeLandmarkCollectionXml );
	iLog->Log(_L("CreateEncoder successful"));
	delete encoder;
	ReleaseLandmarkResources();
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkEncoder::SetOutputFile
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkEncoder::SetOutputFileL( CStifItemParser& aItem )
	{
	TPtrC exportFilePath;
	TInt err = aItem.GetNextString( exportFilePath );	
	
	CPosLandmarkEncoder* encoder = CPosLandmarkEncoder::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( encoder );
	// Delete output file if already exists
	RFs rfs;
  rfs.Connect();
  rfs.Delete( exportFilePath );
  rfs.Close();
	// Set output file
	encoder->SetOutputFileL( exportFilePath );
	iLog->Log(_L("SetOutputFile successful"));
	CleanupStack::PopAndDestroy( encoder );
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkEncoder::SetOutputBuffer
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkEncoder::SetOutputBufferL( CStifItemParser& /*aItem*/ )
	{
	CPosLandmarkEncoder* encoder = CPosLandmarkEncoder::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( encoder );
	// Set use output buffer
	CBufBase* buffer = encoder->SetUseOutputBufferL();
	iLog->Log(_L("SetOutputBuffer successful"));
	delete buffer;
	CleanupStack::PopAndDestroy( encoder );
	ReleaseLandmarkResources();
	return KErrNone;
	}	
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkEncoder::AddCollectionData
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkEncoder::AddCollectionDataL( CStifItemParser& aItem )
	{
	TPtrC collNamePtr;
	TPtrC collDescPtr;
	TInt err = aItem.GetNextString( collNamePtr );	
	err = aItem.GetNextString( collDescPtr );
	
	CPosLandmarkEncoder* encoder = CPosLandmarkEncoder::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( encoder );
	// Set use output buffer
	CBufBase* buffer = encoder->SetUseOutputBufferL();
	CleanupStack::PushL( buffer );
	// Add Collection Data
	TPosLmCollectionDataId id = EPosLmCollDataCollectionName;
	encoder->AddCollectionDataL( id, collNamePtr );
	//
	id = EPosLmCollDataCollectionDescription;
	encoder->AddCollectionDataL( id, collDescPtr );
	// Finalize Encoding
	//ExecuteAndDeleteLD(encoder->FinalizeEncodingL());
	//
	iLog->Log(_L("AddCollectionData asdf successful")); 
	CleanupStack::PopAndDestroy( 2, encoder );
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkEncoder::AddLandmarkToEncode
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkEncoder::AddLandmarkToEncodeL( CStifItemParser& /*aItem*/ )
	{
	CPosLandmarkEncoder* encoder = CPosLandmarkEncoder::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( encoder );
	// Set use output buffer
	CBufBase* buffer = encoder->SetUseOutputBufferL();
	CleanupStack::PushL( buffer );
	// Create a landmark
	CPosLandmark* landmark = CPosLandmark::NewL();
	CleanupStack::PushL( landmark );
	// Set name
	_LIT( KLmkName, "A Test Landmark" );
	landmark->SetLandmarkNameL( KLmkName );
	// Add Landmark for encoding
	encoder->AddLandmarkL( *landmark );
	// Finalize Encoding
	ExecuteAndDeleteLD( encoder->FinalizeEncodingL() );
	//
	iLog->Log(_L("AddLandmarkToEncode successful")); 
	CleanupStack::PopAndDestroy( 3, encoder );
	ReleaseLandmarkResources();
	return KErrNone;
	}	
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkEncoder::AddCategoryForLatestLandmark
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkEncoder::AddCategoryForLatestLandmarkL( CStifItemParser& /*aItem*/ )
	{
	CPosLandmarkEncoder* encoder = CPosLandmarkEncoder::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( encoder );
	// Set use output buffer
	CBufBase* buffer = encoder->SetUseOutputBufferL();
	CleanupStack::PushL( buffer );
	// Create a landmark
	CPosLandmark* landmark = CPosLandmark::NewL();
	CleanupStack::PushL( landmark );
	// Set name
	_LIT( KLmkName, "A Test Landmark" );
	landmark->SetLandmarkNameL( KLmkName );
	// Add Landmark for encoding
	encoder->AddLandmarkL( *landmark );
	// Create a category
	CPosLandmarkCategory* category = CPosLandmarkCategory::NewL();
	CleanupStack::PushL( category );
	// Set name
	_LIT( KCategoryName, "A Category Name" );
	category->SetCategoryNameL( KCategoryName );
	// Add category for latest landmark
	encoder->AddCategoryForLatestLandmarkL( *category );
	// Finalize Encoding
	ExecuteAndDeleteLD( encoder->FinalizeEncodingL() );
	//
	iLog->Log(_L("AddCategoryForLatestLandmark successful")); 
	CleanupStack::PopAndDestroy( 4, encoder );
	ReleaseLandmarkResources();
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkEncoder::FinalizeEncoding
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkEncoder::FinalizeEncodingL( CStifItemParser& /*aItem*/ )
	{
	CPosLandmarkEncoder* encoder = CPosLandmarkEncoder::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( encoder );
	// Set use output buffer
	CBufBase* buffer = encoder->SetUseOutputBufferL();
	CleanupStack::PushL( buffer );
	// Create a landmark
	CPosLandmark* landmark = CPosLandmark::NewL();
	CleanupStack::PushL( landmark );
	// Set name
	_LIT( KLmkName, "A Test Landmark" );
	landmark->SetLandmarkNameL( KLmkName );
	// Add Landmark for encoding
	encoder->AddLandmarkL( *landmark );
	// Finalize Encoding
	CPosLmOperation* operation = encoder->FinalizeEncodingL();
	CleanupStack::PushL( operation );
	// Execute operation
	operation->ExecuteL();
	//
	iLog->Log(_L("FinalizeEncoding successful")); 
	CleanupStack::PopAndDestroy( 4, encoder );
	ReleaseLandmarkResources();
	return KErrNone;
	}		
