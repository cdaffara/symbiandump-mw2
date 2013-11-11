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
* Description:  Provides testing of CPosLmItemIterator class methods
*
*/



// INCLUDE FILES
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>
#include <LbsPosition.h>
#include "testcposlmitemiterator.h"

// Literals
// Test Database URI
_LIT( KDBUri, "file://c:eposlmtest.ldb" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestPosLmItemIterator::CTestPosLmItemIterator
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestPosLmItemIterator::CTestPosLmItemIterator( CStifLogger* aLog )
	{
	iLog = aLog;
	}

// -----------------------------------------------------------------------------
// CTestPosLmItemIterator::NewL
// 
// 
// -----------------------------------------------------------------------------
//	
CTestPosLmItemIterator* CTestPosLmItemIterator::NewL(CStifLogger* aLog)
	{
	CTestPosLmItemIterator* self = new (ELeave) CTestPosLmItemIterator( aLog );
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); 
    return self; 
	}

// -----------------------------------------------------------------------------
// CTestPosLmItemIterator::ConstructL
// 
// 
// -----------------------------------------------------------------------------
//
void CTestPosLmItemIterator::ConstructL()
	{
	
	}	

// -----------------------------------------------------------------------------
// CTestPosLmItemIterator::~CTestPosLmItemIterator
// 
// 
// -----------------------------------------------------------------------------
//
CTestPosLmItemIterator::~CTestPosLmItemIterator()
	{
	
	}

// -----------------------------------------------------------------------------
// CTestPosLmItemIterator::NumOfItems
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmItemIterator::NumOfItemsL( CStifItemParser& /*aItem*/ )
	{
	// Open and init default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Get Landmark Iterator
	CPosLmItemIterator* iterator = lmkDatabase->LandmarkIteratorL();
	CleanupStack::PushL( iterator );
	// Get number of landmarks in database, which are iterated through iterator
	TUint number = iterator->NumOfItemsL();
	// Print number
	TBuf<10> buf;
	buf.AppendNum( number );
	iLog->Log( buf );
	iLog->Log(_L("NumOfItemsL Success"));
	CleanupStack::PopAndDestroy( 2, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLmItemIterator::NextItem
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmItemIterator::NextItemL( CStifItemParser& /*aItem*/ )
	{
	// Open and init default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD(lmkDatabase->InitializeL());
	iLog->Log(_L("Database done"));
	// Get Landmark Iterator
	CPosLmItemIterator* iterator = lmkDatabase->LandmarkIteratorL();
	CleanupStack::PushL( iterator );
	// Call NextL to get id of next landmark in database
	TPosLmItemId id;
	id = iterator->NextL();
	// Print name of landmark
	CPosLandmark* landmark = lmkDatabase->ReadLandmarkLC(id);
	TPtrC name;
	landmark->GetLandmarkName( name );
	iLog->Log( name );
	// NextL successful
	iLog->Log(_L("NextItem successful"));
	CleanupStack::PopAndDestroy( 3, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CTestPosLmItemIterator::GetItemIdsL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmItemIterator::GetItemIdsL( CStifItemParser& /*aItem*/ )
	{
	// Open and init default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Get Landmark Iterator
	CPosLmItemIterator* iterator = lmkDatabase->LandmarkIteratorL();
	CleanupStack::PushL( iterator );
	// Get number of landmarks in database, which are iterated through iterator
	TUint number = iterator->NumOfItemsL();
	// Get ids of all the items in array
	RArray<TPosLmItemId> idArray;
	iterator->GetItemIdsL( idArray, 0, number );
	//
	if ( idArray.Count() != number )
		{
		iLog->Log(_L("GetItemIdsL fails"));
		User::Leave( KErrGeneral );
		}
	iLog->Log(_L("GetItemIdsL successful"));
	CleanupStack::PopAndDestroy( 2, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLmItemIterator::ResetIteratorL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmItemIterator::ResetIteratorL( CStifItemParser& /*aItem*/ )
	{
	// Open and init default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Get Landmark Iterator
	CPosLmItemIterator* iterator = lmkDatabase->LandmarkIteratorL();
	CleanupStack::PushL( iterator );
	// Get id of first item befor reset
	TPosLmItemId idBeforeReset = iterator->NextL();
	// Reset iterator
	iterator->Reset();
	// Get id after reset, should return id of first item
	TPosLmItemId idAfterReset = iterator->NextL();
	// Compare both the ids
	if ( idBeforeReset != idAfterReset )
		{
		iLog->Log(_L("ResetIteratorL fails"));
		User::Leave( KErrGeneral );
		}
	TBuf<5> buf;
	buf.Append(idAfterReset);
	iLog->Log(buf);	
	iLog->Log(_L("ResetIteratorL successful"));
	CleanupStack::PopAndDestroy( 2, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;	
	}
