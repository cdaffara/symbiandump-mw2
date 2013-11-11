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
* Description:  Provides testing of CPosLandmarkDatabase class methods
*
*/



// INCLUDE FILES
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <epos_cposlandmarkdatabaseextended.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <EPos_CPosLandmarkParser.h>
#include <LbsPosition.h>
#include <EPos_CPosLmDatabaseManager.h> 
#include "testcposlandmarkdatabase.h"
#include <StifParser.h>
#include <Stiftestinterface.h>


// Literals
_LIT8( KPosMimeTypeLandmarkCollectionXml, "application/vnd.nokia.landmarkcollection+xml" );
_LIT( KDBUri, "file://c:eposlmtest.ldb" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::CTestPosLandmarkDatabase
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestPosLandmarkDatabase::CTestPosLandmarkDatabase( CStifLogger* aLog ) :
				CActive(EPriorityStandard), iLog(aLog)
	{
	CActiveScheduler::Add(this);
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::NewL
// 
// 
// -----------------------------------------------------------------------------
//		
CTestPosLandmarkDatabase* CTestPosLandmarkDatabase::NewL(CStifLogger* aLog)
	{
	CTestPosLandmarkDatabase* self = new (ELeave) CTestPosLandmarkDatabase( aLog );
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); 	
    return self; 
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::ConstructL
// 
// 
// -----------------------------------------------------------------------------
//	
void CTestPosLandmarkDatabase::ConstructL()
	{
	
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::~CTestPosLandmarkDatabase
// 
// 
// -----------------------------------------------------------------------------
//		
CTestPosLandmarkDatabase::~CTestPosLandmarkDatabase()
	{
	Cancel();
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::OpenDefaultDatabase
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::OpenDefaultDatabaseL( CStifItemParser& /*aItem*/ )
	{
	
	// Open default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL();
	
	delete lmkDatabase;
	ReleaseLandmarkResources();
	return KErrNone;
	}

	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::OpenDatabase
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::OpenDatabaseL( CStifItemParser& aItem )
	{
	TPtrC uriPtr;
	TInt err = aItem.GetNextString( uriPtr );	
	// Open database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	delete lmkDatabase;
	ReleaseLandmarkResources();
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::IsInitializingNeeded
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::IsInitializingNeededL( CStifItemParser& /*aItem*/ )
	{	
	// Open default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL(KDBUri);
	CleanupStack::PushL( lmkDatabase );	
	// Check if init required
	TBool result = lmkDatabase->IsInitializingNeeded();
	if( result )
		{
		// Initializing needed
		iLog->Log(_L("IsInitializingNeeded successful, initialization needed"));
		CleanupStack::PopAndDestroy( lmkDatabase );
		ReleaseLandmarkResources();
		return KErrNone;
		}
	else
		{
		// Initializing not needed
		iLog->Log(_L("IsInitializingNeeded successful, initialization not needed"));
		CleanupStack::PopAndDestroy( lmkDatabase );
		ReleaseLandmarkResources();
		return KErrNone;
		}		
	}


// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::Initialize
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::InitializeL( CStifItemParser& /*aItem*/ )
	{
	// Open default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL(KDBUri);
	CleanupStack::PushL( lmkDatabase );	
	// Init
	CPosLmOperation* initOperation = lmkDatabase->InitializeL();
	CleanupStack::PushL( initOperation );
	// Perform initialization
	initOperation->ExecuteL();
	iLog->Log(_L("Initialize successful"));
	CleanupStack::PopAndDestroy( 2, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;
	}


// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::GetDatabaseUri
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::GetDatabaseUriL( CStifItemParser& /*aItem*/ )
	{
	// Open default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );	
	// Init
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Get URI
	HBufC* uri = lmkDatabase->DatabaseUriLC(); 
	// Print URI
	iLog->Log(uri->Des());
	iLog->Log(_L("GetDatabaseUri successful"));
	CleanupStack::PopAndDestroy( 2, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;
	}

	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::GetDatabaseSize
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::GetDatabaseSizeL( CStifItemParser& /*aItem*/ )
	{	
	// Open default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );	
	// Init
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Get Size
	CPosLandmarkDatabase::TSize size;
	size = lmkDatabase->SizeL();
	// Print size
	_LIT( KSize, "Size = " );
	TBuf<25> sizebuf(KSize);
	sizebuf.AppendNum(size.iFileSize);
	iLog->Log(sizebuf);	
	// Size retrieved	
	iLog->Log(_L("GetDatabaseSize successful"));
	CleanupStack::PopAndDestroy( lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::LandmarkIterator
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::LandmarkIteratorL( CStifItemParser& /*aItem*/ )
	{
	// Open default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );	
	// Init
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Get Landmark Iterator
	CPosLmItemIterator* iterator = lmkDatabase->LandmarkIteratorL();
	CleanupStack::PushL( iterator );	
	// print number of lmks
	TInt numOfLmks = iterator->NumOfItemsL();
	_LIT( KTotal, "Total = " );
	TBuf<20> buffer;
	buffer.Copy(KTotal);
	buffer.AppendNum(numOfLmks);
	iLog->Log(buffer);
	iLog->Log(_L("LandmarkIterator successful"));
	CleanupStack::PopAndDestroy( 2, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::LmkIteratorSortPref
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::LmkIteratorSortPrefL( CStifItemParser& /*aItem*/ )
	{
	// Open default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );	
	// Init
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create sort preference
	TPosLmSortPref sortPref( CPosLandmark::ELandmarkName );
	// Get Landmark Iterator with sort preference
	CPosLmItemIterator* iterator = lmkDatabase->LandmarkIteratorL( sortPref );
	CleanupStack::PushL( iterator );
	// LandmarkIterator successful
	iLog->Log(_L("LmkIteratorSortPref successful"));
	CleanupStack::PopAndDestroy( 2, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;	
	}	
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::ReadLandmark
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::ReadLandmarkL( CStifItemParser& /*aItem*/ )
	{
	// Open default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );	
	// Init
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Get Landmark Iterator
	CPosLmItemIterator* iterator = lmkDatabase->LandmarkIteratorL();
	CleanupStack::PushL( iterator );	
	// Call NextL to get id of next landmark in database
	TPosLmItemId id = iterator->NextL();
	if( KPosLmNullItemId == id )
		{
		iLog->Log(_L("ReadLandmark fails, no landmark in database"));
		User::Leave( KErrGeneral );
		}
	// Read landmark
	CPosLandmark* landmark = lmkDatabase->ReadLandmarkLC( id );
	// ReadLandmark successful
	iLog->Log(_L("ReadLandmark successful"));
	CleanupStack::PopAndDestroy( 3, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;	
	}	
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::AddAndRemoveLandmark
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::AddAndRemoveLandmarkL( CStifItemParser& aItem )
	{
	TPtrC lmkNamePtr;
	TInt err = aItem.GetNextString( lmkNamePtr );	
	// Open default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );	
	// Init
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create a new landmark
	CPosLandmark* landmark = CPosLandmark::NewL();
	CleanupStack::PushL( landmark );	
	// Set name
	landmark->SetLandmarkNameL( lmkNamePtr );
	// Add to database
	TPosLmItemId lmkId = lmkDatabase->AddLandmarkL( *landmark );
	// Check if added
	lmkId = landmark->LandmarkId(); // Should return valid id of landmark
	if( KPosLmNullItemId == lmkId )
		{
		iLog->Log(_L("Error in adding landmark to database, AddAndRemoveLandmark failed"));
		User::Leave( KErrGeneral );
		}
	// Remove from database
	lmkDatabase->RemoveLandmarkL( lmkId );
	// Success
	iLog->Log(_L("AddAndRemoveLandmark successful"));
	CleanupStack::PopAndDestroy( 2, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::UpdateLandmark
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::UpdateLandmarkL( CStifItemParser& aItem )
	{
	TPtrC lmkNamePtr;
	TInt err = aItem.GetNextString( lmkNamePtr );
	// Open default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );	
	// Init
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Read a landmark
	// Get a valid id
	CPosLmItemIterator* iter = lmkDatabase->LandmarkIteratorL();
	CleanupStack::PushL( iter );
	TPosLmItemId id = iter->NextL();
	if( KPosLmNullItemId == id )
		{
		iLog->Log(_L("No landmark found in database"));	
		User::Leave( KErrNotFound );
		}
	CPosLandmark* landmark = lmkDatabase->ReadLandmarkLC( id );
	// Save original name
	TPtrC originalName;
	landmark->GetLandmarkName( originalName );
	// Change name of landmark
	landmark->SetLandmarkNameL( lmkNamePtr );
	// Update landmark in DB
	lmkDatabase->UpdateLandmarkL( *landmark );
	// Reset the original name
	landmark->SetLandmarkNameL( originalName );
	// Update landmark in DB
	lmkDatabase->UpdateLandmarkL( *landmark );
	iLog->Log(_L("UpdateLandmark successful"));
	CleanupStack::PopAndDestroy( 3, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::RemoveLandmarks
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::RemoveLandmarksL( CStifItemParser& /*aItem*/ )
	{
	// Open Database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Initialize database
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Add 3 test landmarks, which will be removed by RemoveLandmarksL()
	TPosLmItemId id;
	RArray<TPosLmItemId> idArray;
	CPosLandmark* landmark = CPosLandmark::NewL();
	CleanupStack::PushL( landmark );
	_LIT( KName, "A Test Landmark " );
	TBuf<25> nameBuffer;
	for(TInt i=1;i<4;i++)
		{
		nameBuffer.Copy( KName );
		nameBuffer.AppendNum( i );
		landmark->SetLandmarkNameL( nameBuffer );
		id = lmkDatabase->AddLandmarkL( *landmark );
		idArray.AppendL( id );
		}
	// Create iterator and get number of landmarks	
	CPosLmItemIterator* iterator = lmkDatabase->LandmarkIteratorL();
	CleanupStack::PushL( iterator );
	TInt totalLmkBeforeRemove = iterator->NumOfItemsL();
	nameBuffer.Copy( KName );
		nameBuffer.AppendNum( totalLmkBeforeRemove );
	iLog->Log(nameBuffer);	
	CleanupStack::PopAndDestroy( iterator );
	// Remove landmarks (3 landmarks)
	ExecuteAndDeleteLD( lmkDatabase->RemoveLandmarksL( idArray ) );
	// Get 	number of landmarks	after remove
	iterator = lmkDatabase->LandmarkIteratorL();
	CleanupStack::PushL( iterator );
	TInt totalLmkAfterRemove = iterator->NumOfItemsL();
	nameBuffer.Copy( KName );
		nameBuffer.AppendNum( totalLmkAfterRemove );
	iLog->Log(nameBuffer);
	// Check
	if( totalLmkAfterRemove != (totalLmkBeforeRemove - 3) )
		{
		iLog->Log(_L("RemoveLandmarks fails"));
		User::Leave( KErrGeneral );
		}
	iLog->Log(_L("RemoveLandmarks succeeds"));
	CleanupStack::PopAndDestroy( 3, lmkDatabase );
	ReleaseLandmarkResources();			
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::RemoveAllLandmarks
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::RemoveAllLandmarksL( CStifItemParser& /*aItem*/ )
	{
	// Open database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Initialize database
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Save 
	// Remova All landmarks from database
	ExecuteAndDeleteLD( lmkDatabase->RemoveAllLandmarksL() );
	// Restore
	iLog->Log(_L("RemoveAllLandmarks successful"));
	CleanupStack::PopAndDestroy( lmkDatabase );
	ReleaseLandmarkResources();		
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::Compact
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::CompactL( CStifItemParser& /*aItem*/ )
	{
	// Open database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Initialize database
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Compact DB
	CPosLmOperation* operation = lmkDatabase->CompactL();
	CleanupStack::PushL( operation );
	operation->ExecuteL();
	iLog->Log(_L("Compact successful"));
	CleanupStack::PopAndDestroy( 2, lmkDatabase );
	ReleaseLandmarkResources();	
	return KErrNone;	
	}
		
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::SetAndGetPartialReadParams
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::SetAndGetPartialReadParamsL( CStifItemParser& /*aItem*/ )
	{
	// Open and initialize DB
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Initialize database
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Partial read params
	CPosLmPartialReadParameters* setReadParams = CPosLmPartialReadParameters::NewLC();
	// Set 2 attributes, position and coverage radius
	setReadParams->SetRequestedAttributes( CPosLandmark::EPosition | CPosLandmark::ECoverageRadius );
	// Set partial read params
	lmkDatabase->SetPartialReadParametersL( *setReadParams );
	// Read a landmark, and check for name, name should not be found
	CPosLmItemIterator* iterator = lmkDatabase->LandmarkIteratorL();
	CleanupStack::PushL( iterator );
	TPosLmItemId id = iterator->NextL();
	CPosLandmark* landmark = lmkDatabase->ReadPartialLandmarkLC( id );
	TInt error = KErrNone;
	TPtrC namePtr;
	// Check for name, should return KErrNotFound
	error = landmark->GetLandmarkName( namePtr );
	if( KErrNotFound != error )
		{
		iLog->Log(_L("SetAndGetPartialReadParams fails"));
		User::Leave( KErrGeneral );
		}
	// Get partial read params
	CPosLmPartialReadParameters* getReadParams = lmkDatabase->PartialReadParametersLC();
	
	// Get attributes
	CPosLandmark::TAttributes attributes = getReadParams->RequestedAttributes();
	// Check
	// EPosition, should exist
	if( !( attributes & CPosLandmark::EPosition ) )
		{
		iLog->Log(_L("SetAndGetPartialReadParams fails, position not found"));
		User::Leave( KErrGeneral );
		}
	// ECoverageRadius, should exist	
	if( !( attributes & CPosLandmark::ECoverageRadius ) )
		{
		iLog->Log(_L("SetAndGetPartialReadParams fails, coverage radius not found"));
		User::Leave( KErrGeneral );
		}	
	// EName, should not exist
	if( attributes & CPosLandmark::ELandmarkName )
		{
		iLog->Log(_L("SetAndGetPartialReadParams fails"));
		User::Leave( KErrGeneral );
		}	
	iLog->Log(_L("SetAndGetPartialReadParams succeeds"));
	CleanupStack::PopAndDestroy( 5, lmkDatabase );
	ReleaseLandmarkResources();	
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::ReadPartialLandmark
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::ReadPartialLandmarkL( CStifItemParser& /*aItem*/ )
	{
	// Open database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Initialize database
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Partial read params
	CPosLmPartialReadParameters* setReadParams = CPosLmPartialReadParameters::NewLC();
	// Set 2 attributes, position and name
	setReadParams->SetRequestedAttributes( CPosLandmark::EPosition | CPosLandmark::ELandmarkName );
	// Set partial read params
	lmkDatabase->SetPartialReadParametersL( *setReadParams );
	// Read a landmark using iterator
	CPosLmItemIterator* iterator = lmkDatabase->LandmarkIteratorL();
	CleanupStack::PushL( iterator );
	TPosLmItemId id = iterator->NextL();
	if( KPosLmNullItemId == id )
		{
		iLog->Log(_L("No landmark exists in database, ReadPartialLandmark fails"));
		User::Leave( KErrGeneral );
		}
	// This landmark should only have Position and Name, no other attributes should be present
	CPosLandmark* landmark = lmkDatabase->ReadPartialLandmarkLC( id );
	TInt error = KErrNone;
	TPtrC namePtr;
	// Check if landmark is partial
	if( !landmark->IsPartial() )
		{
		iLog->Log(_L("ReadPartialLandmark fails, landmark is not partial"));
		User::Leave( KErrGeneral );
		}
	// Check for name, should return KErrNone
	error = landmark->GetLandmarkName( namePtr );
	if( KErrNone != error )
		{
		iLog->Log(_L("ReadPartialLandmark fails, name not found"));
		User::Leave( KErrGeneral );
		}
	// Check for Coverage radius, should return KErrNotFound
	TReal32 radius;
	error = landmark->GetCoverageRadius( radius );
	if( KErrNotFound != error )
		{
		iLog->Log(_L("ReadPartialLandmark fails, coverage radius was found in partial landmark"));
		User::Leave( KErrGeneral );
		}
	//
	iLog->Log(_L("ReadPartialLandmark suceeds"));
	CleanupStack::PopAndDestroy( 4, lmkDatabase );
	ReleaseLandmarkResources();		
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::PrepareAndTakePartialLmks
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::PrepareAndTakePartialLmksL( CStifItemParser& /*aItem*/ )
	{
	// Open and initialize DB
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Initialize database
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Partial read params
	CPosLmPartialReadParameters* readParams = CPosLmPartialReadParameters::NewLC();
	// Set attributes, only name
	readParams->SetRequestedAttributes( CPosLandmark::ELandmarkName );
	// Set partial read params
	lmkDatabase->SetPartialReadParametersL( *readParams );
	// Get ids of all the landmarks using an iterator
	RArray<TPosLmItemId> idArray;
	TPosLmItemId id;
	CPosLmItemIterator* iterator = lmkDatabase->LandmarkIteratorL();
	CleanupStack::PushL( iterator );
	if( iterator->NumOfItemsL() == 0 )
		{
		iLog->Log(_L("No landmark exists in database, PrepareAndTakePartialLmks fails"));
		User::Leave( KErrGeneral );
		}
	// Iterate through all the landmarks and save ids into array
	while( ( id = iterator->NextL()) != KPosLmNullItemId )
		{
		idArray.Append( id );
		}
	// Prepare partial landmark
	CPosLmOperation* operation = lmkDatabase->PreparePartialLandmarksL( idArray );
	CleanupStack::PushL( operation );
	operation->ExecuteL();
	iLog->Log(_L("operation done"));
	// Take partial landmark
	CArrayPtr<CPosLandmark>* lmkPtrArray = lmkDatabase->TakePreparedPartialLandmarksL( operation );
	iLog->Log(_L("Takeprepared done"));
	//CleanupStack::PushL(lmkPtrArray);
	iLog->Log(_L("PrepareAndTakePartialLmks succeeds"));
//	CleanupStack::Pop(lmkPtrArray);
	lmkPtrArray->ResetAndDestroy();
	iLog->Log(_L("after reset done"));
	CleanupStack::PopAndDestroy( 4, lmkDatabase );
	ReleaseLandmarkResources();	
	return KErrNone; 	
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::ExportLandmarks
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::ExportLandmarksL( CStifItemParser& aItem )
	{
	TPtrC exportFilePtr;
	TInt err = aItem.GetNextString( exportFilePtr );
	// Open and initialize DB
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Initialize database
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create encoder
	CPosLandmarkEncoder* encoder = CPosLandmarkEncoder::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( encoder );
	
	// delete output file if already exists
	RFs rfs;
  	User::LeaveIfError(rfs.Connect());
  	rfs.Delete( exportFilePtr );
  	rfs.Close();
	// Set output file
	encoder->SetOutputFileL( exportFilePtr );
	// Create iterator
	CPosLmItemIterator* iterator = lmkDatabase->LandmarkIteratorL();
	CleanupStack::PushL( iterator );
	//
	if( iterator->NumOfItemsL() == 0 )
		{
		iLog->Log(_L("No landmark exists in database, ExportLandmarks fails"));
		User::Leave( KErrGeneral );
		}
	// Get ids
	RArray<TPosLmItemId> idArray;
	TPosLmItemId id;
	while( ( id = iterator->NextL() ) != KPosLmNullItemId )
		{
		idArray.AppendL( id );
		}
	// export landmarks
	CPosLmOperation* operation = lmkDatabase->ExportLandmarksL( *encoder, idArray, CPosLandmarkDatabase::EDefaultOptions );	
	CleanupStack::PushL( operation );
	operation->ExecuteL();
	// Finalize Encoding
	ExecuteAndDeleteLD( encoder->FinalizeEncodingL() );
	//
	iLog->Log(_L("ExportLandmarks successful")); 
	CleanupStack::PopAndDestroy( 4, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::ImportLandmarks
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::ImportLandmarksL( CStifItemParser& aItem )
	{
	TPtrC importFilePtr;
	TInt err = aItem.GetNextString( importFilePtr );	
	// Open and initialize DB
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL(  KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Initialize database
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create parser
	CPosLandmarkParser* parser = CPosLandmarkParser::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( parser );
	// Set Input file
	parser->SetInputFileL( importFilePtr );
	// Import landmarks to DB	
	CPosLmOperation* operation = lmkDatabase->ImportLandmarksL( *parser, CPosLandmarkDatabase::EDefaultOptions );
	CleanupStack::PushL( operation );
	operation->ExecuteL();
	iLog->Log(_L("ImportLandmarks successful")); 
	CleanupStack::PopAndDestroy( 3, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::ImportSelectedLandmarks
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::ImportSelectedLandmarksL( CStifItemParser& aItem )
	{
	TPtrC importFilePtr;
	TInt err = aItem.GetNextString( importFilePtr );	
	// Open and initialize DB
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Initialize database
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create parser
	CPosLandmarkParser* parser = CPosLandmarkParser::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( parser );
	// Set Input file
	parser->SetInputFileL( importFilePtr );
	// Import landmarks to DB
	// Define index array
	RArray<TUint> indexArray;
	// Add indexes of selected landmarks, only these should be imported
	indexArray.AppendL( 0 );
	indexArray.AppendL( 1 );	
	CPosLmOperation* operation = lmkDatabase->ImportLandmarksL( *parser, indexArray, CPosLandmarkDatabase::EDefaultOptions );
	CleanupStack::PushL( operation );
	operation->ExecuteL();
	iLog->Log(_L("ImportSelectedLandmarks successful")); 
	CleanupStack::PopAndDestroy( 3, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::ImportedLmkIterator
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::ImportedLmkIteratorL( CStifItemParser& aItem )
	{
	TPtrC importFilePtr;
	TInt err = aItem.GetNextString( importFilePtr );	
	// Open and initialize DB
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Initialize database
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create parser
	CPosLandmarkParser* parser = CPosLandmarkParser::NewL( KPosMimeTypeLandmarkCollectionXml );
	CleanupStack::PushL( parser );
	// Set Input file
	parser->SetInputFileL( importFilePtr );
	// Import landmarks to DB	
	CPosLmOperation* operation = lmkDatabase->ImportLandmarksL( *parser, CPosLandmarkDatabase::EDefaultOptions );
	CleanupStack::PushL( operation );
	operation->ExecuteL();
	// Get iterator for imported landmarks
	CPosLmItemIterator* iterator = lmkDatabase->ImportedLandmarksIteratorL( operation );
	CleanupStack::PushL( iterator );
	// Get ids of imported landmarks
	RArray<TPosLmItemId> idArray;
	TPosLmItemId id;
	while( (id = iterator->NextL()) != KPosLmNullItemId )
		{
		idArray.AppendL( id );
		}
	iLog->Log(_L("ImportedLmkIterator successful")); 
	CleanupStack::PopAndDestroy( 4, lmkDatabase);
	ReleaseLandmarkResources();		
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::NotifyDatabaseEventL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::NotifyDatabaseEventL( CStifItemParser& /*aItem*/ )
	{
	// Open and initialize DB
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// NotfiyEvent request
	lmkDatabase->NotifyDatabaseEvent( iEvent, iStatus );
	SetActive();
	// Create a new landmark and add to database
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// Add to database
	TPosLmItemId id = lmkDatabase->AddLandmarkL( *lmk );
	//
	// Wait
	CActiveScheduler::Start();
	// 
	//lmkDatabase->CancelNotifyDatabaseEvent();
	CleanupStack::PopAndDestroy( 2, lmkDatabase );
	ReleaseLandmarkResources();
	return iLastResult;
	//return KErrNone;	
	}	
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::CancelNotifyDatabaseEventL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::CancelNotifyDatabaseEventL( CStifItemParser& /*aItem*/ )
	{
	// Open and initialize DB
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// NotfiyEvent request
	lmkDatabase->NotifyDatabaseEvent( iEvent, iStatus );
	SetActive();
	// Cancel
	lmkDatabase->CancelNotifyDatabaseEvent();
	// Wait
	CActiveScheduler::Start();
	// 
	CleanupStack::PopAndDestroy( lmkDatabase );
	ReleaseLandmarkResources();
	//return iLastResult;
	return KErrNone;	
	}	
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::OpenExtendedDatabaseL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::OpenExtendedDatabaseL( CStifItemParser& /*aItem*/ )
	{
	// Open default database as extended database
	CPosLandmarkDatabaseExtended* lmkDatabase = CPosLandmarkDatabaseExtended::OpenL();
	
	delete lmkDatabase;
	
	// Open test database as  extended database
	lmkDatabase = CPosLandmarkDatabaseExtended::OpenL( KDBUri );
	
	delete lmkDatabase;
	
	ReleaseLandmarkResources();
	return KErrNone;
	}		
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::ExtendedDbLandmarkCountL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::ExtendedDbLandmarkCountL( CStifItemParser& /*aItem*/ )
	{
	// Open test database as extended database
	CPosLandmarkDatabaseExtended* lmkDatabase = CPosLandmarkDatabaseExtended::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase ); 
	
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	
	TInt lmkCount = lmkDatabase->LandmarksCount();
	
	CleanupStack::PopAndDestroy( lmkDatabase );
	
	ReleaseLandmarkResources();
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::ExtendedDbCategoryCountL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::ExtendedDbCategoryCountL( CStifItemParser& /*aItem*/ )
	{
	// Open test database as extended database
	CPosLandmarkDatabaseExtended* lmkDatabase = CPosLandmarkDatabaseExtended::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase ); 
	
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	
	TInt catCount = lmkDatabase->CategoriesCount();
	
	//delete lmkDatabase;
	CleanupStack::PopAndDestroy( lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CTestPosLmOperation::RunL
// 
// 
// -----------------------------------------------------------------------------
//
void CTestPosLandmarkDatabase::RunL()
	{
	iLog->Log(_L("RunL called"));
	if( iEvent.iEventType == EPosLmEventLandmarkCreated)
		{
		iLog->Log(_L("A Landmark is created"));
		iLastResult = KErrNone;
		}
	CActiveScheduler::Stop();
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::DoCancel
// 
// 
// -----------------------------------------------------------------------------
//
void CTestPosLandmarkDatabase::DoCancel()
	{
	
	}	
	

// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::InitTestDatabase
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::InitTestDatabaseL( CStifLogger* aLog )
	{
	// URI
	
	// Check if exists
	CPosLmDatabaseManager *manager = CPosLmDatabaseManager::NewL();
	CleanupStack::PushL( manager );
	if( manager->DatabaseExistsL( KDBUri ) )
		{
		// if yes
		// Open 
		CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL(KDBUri);
		CleanupStack::PushL( lmkDatabase );
		// Initialize database
		ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
		// check if populated
		// Get landmark iterator
		CPosLmItemIterator* iterator = lmkDatabase->LandmarkIteratorL();
		CleanupStack::PushL( iterator );
		TInt numOfLmks = iterator->NumOfItemsL();
		if( numOfLmks > 0 )
			{
			// if populated
			// return
			CleanupStack::PopAndDestroy( 3, manager );
			ReleaseLandmarkResources();
			return KErrNone;
			}	
		else
			{		
			// else
			// populate
			TInt error = AddLandmarksToTestDbL( lmkDatabase );	
			if( KErrNone != error )
				{
				aLog->Log(_L("Error in populating Test Database"));
				CleanupStack::PopAndDestroy( 3, manager );
				ReleaseLandmarkResources();
				return error;	
				}
			error = AddCategoriesToTestDbL( lmkDatabase );
			if( KErrNone != error )
				{
				aLog->Log(_L("Error in populating Test Database"));
				CleanupStack::PopAndDestroy( 3, manager );
				ReleaseLandmarkResources();
				return error;		
				}	
			// populated
			CleanupStack::PopAndDestroy( 3, manager );
			ReleaseLandmarkResources();
			return KErrNone;		
			}
		}
	else
		{
			// else
			// create
			HPosLmDatabaseInfo* dbInfo = HPosLmDatabaseInfo::NewLC( KDBUri );
			manager->CreateDatabaseL( *dbInfo );
			// populate
			// Open 
			CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
			CleanupStack::PushL( lmkDatabase );
			// Initialize database
			ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
			// add landmarks
			TInt error = AddLandmarksToTestDbL( lmkDatabase );	
			if( KErrNone != error )
				{
				aLog->Log(_L("Error in populating Test Database"));
				CleanupStack::PopAndDestroy( 3, manager );
				ReleaseLandmarkResources();
				return error;	
				}
			// add categories	
			error = AddCategoriesToTestDbL( lmkDatabase );
			if( KErrNone != error )
				{
				aLog->Log(_L("Error in populating Test Database"));
				CleanupStack::PopAndDestroy( 3, manager );
				ReleaseLandmarkResources();
				return error;		
				}
			// success
			CleanupStack::PopAndDestroy( 3, manager );
			ReleaseLandmarkResources();
			return error;							
		}
		// success
	}	
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::AddLandmarksToTestDb
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::AddLandmarksToTestDbL( CPosLandmarkDatabase* aDatabase )
	{
	_LIT( KLmkName, "A Test Landmark " );	
	// Create a landmark, has to be trapped
	CPosLandmark* landmark = NULL;
	TRAPD( error, landmark = CPosLandmark::NewL() );
	if( KErrNone != error )
		{
		return error;
		}
	CleanupStack::PushL( landmark );
	// Add 5 landmarks to DB
	TBuf<20> nameBuffer;
	for(TInt i = 1; i < 6; i++ )
		{
		// Set name	
		nameBuffer.Copy( KLmkName );
		nameBuffer.AppendNum( i );
		TRAP( error, landmark->SetLandmarkNameL( nameBuffer ) );
		if( KErrNone != error )
			{
			CleanupStack::PopAndDestroy( landmark );	
			return error;
			}
		// Add
		TRAP( error, aDatabase->AddLandmarkL( *landmark ) );
		if( KErrNone != error )
			{
			CleanupStack::PopAndDestroy( landmark );	
			return error;
			}	
		}// Add loop
	CleanupStack::PopAndDestroy( landmark );
	return KErrNone;	  		
	}
// -----------------------------------------------------------------------------
// CTestPosLandmarkDatabase::AddCategoriesToTestDb
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkDatabase::AddCategoriesToTestDbL( CPosLandmarkDatabase* aDatabase )
	{
	_LIT( KCatName, "A Test Category " );
	// Create category manager
	CPosLmCategoryManager* categoryMgr = NULL;
	TRAPD( error,  categoryMgr = CPosLmCategoryManager::NewL( *aDatabase ));
	if( KErrNone != error )
		{
		return error;
		}
	CleanupStack::PushL( categoryMgr );		
	// Create a category, has to be trapped
	CPosLandmarkCategory* category = NULL;
	TRAP(error, category = CPosLandmarkCategory::NewL());
	if( KErrNone != error )
		{
		CleanupStack::PopAndDestroy( categoryMgr );	
		return error;
		}
	CleanupStack::PushL( category );
	// Add 5 landmarks to DB
	TBuf<20> nameBuffer;
	for(TInt i = 1; i < 6; i++)
		{
		// Set name	
		nameBuffer.Copy( KCatName );
		nameBuffer.AppendNum( i );
		TRAP( error, category->SetCategoryNameL( nameBuffer ) );
		if( KErrNone != error )
			{
			CleanupStack::PopAndDestroy( 2, categoryMgr );	
			return error;
			}
		// Add
		TRAP( error, categoryMgr->AddCategoryL( *category ) );
		if( KErrNone!= error )
			{
			CleanupStack::PopAndDestroy( 2, categoryMgr );	
			return error;
			}	
		}// Add loop
	CleanupStack::PopAndDestroy( 2, categoryMgr );
	return KErrNone;	  		
	}
