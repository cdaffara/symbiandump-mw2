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
* Description: Provides testing of CPosLmCategoryManager class methods
*
*/



// INCLUDE FILES
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>
#include <LbsPosition.h>
#include "testcposlmcategorymanager.h"
#include <StifParser.h>
#include <Stiftestinterface.h>

// Literals
// Test Database URI
_LIT( KDBUri, "file://c:eposlmtest.ldb" );


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::CTestPosLmCategoryManager
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestPosLmCategoryManager::CTestPosLmCategoryManager( CStifLogger* aLog )
	{
	iLog = aLog;
	}

// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::NewL
// 
// 
// -----------------------------------------------------------------------------
//	
CTestPosLmCategoryManager* CTestPosLmCategoryManager::NewL(CStifLogger* aLog)
	{
	CTestPosLmCategoryManager* self = new (ELeave) CTestPosLmCategoryManager( aLog );
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); 
    return self; 
	}


// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::ConstructL
// 
// 
// -----------------------------------------------------------------------------
//
void CTestPosLmCategoryManager::ConstructL()
	{
	
	}	

// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::~CTestPosLmCategoryManager
// 
// 
// -----------------------------------------------------------------------------
//
CTestPosLmCategoryManager::~CTestPosLmCategoryManager()
	{
	
	}

// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::NewLCreateCategoryManager
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmCategoryManager::NewLCreateCategoryManagerL( CStifItemParser& /*aItem*/ )
	{
	// Open and init default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create category manager using NewL
	CPosLmCategoryManager* categoryMgr = CPosLmCategoryManager::NewL( *lmkDatabase );
	iLog->Log(_L("NewLCreateCategoryManager successful"));
	delete categoryMgr;
	CleanupStack::PopAndDestroy( lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::AddAndRemoveCategory
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmCategoryManager::AddAndRemoveCategoryL( CStifItemParser& aItem )
	{
	TInt toSetName;
	TInt err = aItem.GetNextInt( toSetName );
	TPtrC catNamePtr;
	err = aItem.GetNextString( catNamePtr );
		
	// Open and init default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create category manager using NewL
	CPosLmCategoryManager* categoryMgr = CPosLmCategoryManager::NewL( *lmkDatabase );
	CleanupStack::PushL( categoryMgr );
	// Create a category
	CPosLandmarkCategory* category = CPosLandmarkCategory::NewL();
	CleanupStack::PushL( category );
	// Set category name if toSetName == 1
	if( toSetName == 1 )
		{
		category->SetCategoryNameL( catNamePtr );
		}
	// Add category to database
	TPosLmItemId addCategoryId = categoryMgr->AddCategoryL( *category );
	// Test if added successfully
	TPosLmItemId getCategoryId;
	getCategoryId = categoryMgr->GetCategoryL( catNamePtr );
	if( KPosLmNullItemId == getCategoryId )
		{
		iLog->Log(_L("Error in getting category, category not found in database"));
		User::Leave( KErrGeneral );
		//ReleaseLandmarkResources();
		}
	// Added successfully
	// Compare ids, should match
	if( getCategoryId != addCategoryId )
		{
		iLog->Log(_L("Error in adding category to database"));
		User::Leave( KErrGeneral );
		//ReleaseLandmarkResources();
		}
	// Remove category from database
	ExecuteAndDeleteLD( categoryMgr->RemoveCategoryL(addCategoryId) );
	// Test if Removed successfully
	getCategoryId = categoryMgr->GetCategoryL( catNamePtr ); // Should return KPosLmNullItemId
	if( KPosLmNullItemId != getCategoryId )
		{
		iLog->Log(_L("Error in removing category from database"));
		User::Leave( KErrGeneral );
		//ReleaseLandmarkResources();
		}
	iLog->Log(_L("AddAndRemoveCategory successful"));
	CleanupStack::PopAndDestroy( 3, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::UpdateCategory
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmCategoryManager::UpdateCategoryL( CStifItemParser& /*aItem*/ )
	{
	// DB
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Initialize database
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Cat Mgr
	CPosLmCategoryManager* categoryMgr = CPosLmCategoryManager::NewL( *lmkDatabase );
	CleanupStack::PushL( categoryMgr );
	// Read a Category
	// Get valid id
	CPosLmItemIterator* iter = categoryMgr->CategoryIteratorL();
	CleanupStack::PushL( iter );
	TPosLmItemId id = iter->NextL();
	if( KPosLmNullItemId == id )
		{
		iLog->Log(_L("No category exists in database, can not test UpdateCategoryL"));	
		User::Leave( KErrGeneral );
		/// Release
		}
	CPosLandmarkCategory* category = categoryMgr->ReadCategoryLC( id );
	// Get original name
	TPtrC originalName;
	category->GetCategoryName( originalName );
	// Change Name
	_LIT( KCatName, "A New Category Name" );
	category->SetCategoryNameL( KCatName );
	// Update category
	categoryMgr->UpdateCategoryL( *category );
	// Restore original
	category->SetCategoryNameL( originalName );
	categoryMgr->UpdateCategoryL( *category );
	CleanupStack::PopAndDestroy( 4, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::AddCatToLandmarks
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmCategoryManager::AddCatToLandmarksL( CStifItemParser& aItem )
	{
	TInt num;
	TInt err = aItem.GetNextInt( num );
	
	// Open and init default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create category manager using NewL
	CPosLmCategoryManager* categoryMgr = CPosLmCategoryManager::NewL( *lmkDatabase );
	CleanupStack::PushL( categoryMgr );
	// Create landmark iterator and get ids of  landmarks
	CPosLmItemIterator* lmkIter = lmkDatabase->LandmarkIteratorL();
	CleanupStack::PushL( lmkIter );
	RArray<TPosLmItemId> lmkIdArray;
	TPosLmItemId id;
	while( (id = lmkIter->NextL()) != KPosLmNullItemId )
		{
		lmkIdArray.AppendL( id );
		}
	// Leave if no landmark exist
	if( lmkIdArray.Count() == 0 )
		{
		iLog->Log(_L("No landmark exist, can not test AddCatToLandmarks"));	
		User::Leave( KErrGeneral );
		}	
	// Create category iterator and get a category id
	CPosLmItemIterator* catIter = categoryMgr->CategoryIteratorL();
	CleanupStack::PushL( catIter );
	TPosLmItemId catId = catIter->NextL();
	if( catId == KPosLmNullItemId )
		{
		iLog->Log(_L("No category defined..returning"));
		User::Leave( KErrGeneral );
		}
	// Add category to landmarks
	if( num == 0 )
		{
		// Use invalid category id
		catId = 99999;
		}
	ExecuteAndDeleteLD( categoryMgr->AddCategoryToLandmarksL( catId, lmkIdArray ) );
	iLog->Log(_L("AddCatToLandmarks successful"));
	CleanupStack::PopAndDestroy( 4, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;
	}	
	
// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::RemoveCatFromLandmarks
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmCategoryManager::RemoveCatFromLandmarksL( CStifItemParser& aItem )
	{
	TInt num;
	TInt err = aItem.GetNextInt( num );
	
	// Open and init default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create category manager using NewL
	CPosLmCategoryManager* categoryMgr = CPosLmCategoryManager::NewL( *lmkDatabase );
	CleanupStack::PushL( categoryMgr );
	// Create landmark iterator and get ids of  landmarks
	CPosLmItemIterator* lmkIter = lmkDatabase->LandmarkIteratorL();
	CleanupStack::PushL( lmkIter );
	RArray<TPosLmItemId> lmkIdArray;
	TPosLmItemId id;
	while( (id = lmkIter->NextL()) != KPosLmNullItemId )
		{
		lmkIdArray.AppendL( id );
		}
	// Leave if no landmark exist
	if( lmkIdArray.Count() == 0 )
		{
		iLog->Log(_L("No landmark exist, can not test AddCatToLandmarks"));	
		User::Leave( KErrGeneral );
		}		
	// Create category iterator and get a category id
	CPosLmItemIterator* catIter = categoryMgr->CategoryIteratorL();
	CleanupStack::PushL( catIter );
	TPosLmItemId catId = catIter->NextL();
	if( catId == KPosLmNullItemId )
		{
		iLog->Log(_L("No category defined..returning"));
		User::Leave( KErrGeneral );
		}
	// Remove category from landmarks
	if( num == 0 )
		{
		// Use invalid category id
		catId = 99999;
		}
	ExecuteAndDeleteLD( categoryMgr->RemoveCategoryFromLandmarksL( catId, lmkIdArray ) );
	iLog->Log(_L("RemoveCatFromLandmarks successful"));
	CleanupStack::PopAndDestroy( 4, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;
	}		
	
	
// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::CategoryIterator
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmCategoryManager::CategoryIteratorL( CStifItemParser& /*aItem*/ )
	{
	// Open and init default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create category manager using NewL
	CPosLmCategoryManager* categoryMgr = CPosLmCategoryManager::NewL( *lmkDatabase );
	CleanupStack::PushL( categoryMgr );
	// Get Iterator
	CPosLmItemIterator* iterator = categoryMgr->CategoryIteratorL();
	CleanupStack::PushL( iterator );
	// Loop
	TPosLmItemId categoryId;
	while( (categoryId = iterator->NextL()) != KPosLmNullItemId )
		{
		CPosLandmarkCategory* category = categoryMgr->ReadCategoryLC( categoryId );
		TPtrC name;
		category->GetCategoryName( name );
		iLog->Log( name );
		CleanupStack::PopAndDestroy( category );
		}
	iLog->Log(_L("CategoryIterator Successfulll"));	
	CleanupStack::PopAndDestroy( 3, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::ReferencedIterator
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmCategoryManager::ReferencedIteratorL( CStifItemParser& /*aItem*/ )
	{
	// Open and init default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create category manager using NewL
	CPosLmCategoryManager* categoryMgr = CPosLmCategoryManager::NewL( *lmkDatabase );
	CleanupStack::PushL( categoryMgr );
		
	// Get Referenced Iterator
	CPosLmItemIterator* iterator = categoryMgr->ReferencedCategoryIteratorL();
	CleanupStack::PushL( iterator );
	// Loop
	TPosLmItemId categoryId;
	while( (categoryId = iterator->NextL()) != KPosLmNullItemId )
		{
		CPosLandmarkCategory* category = categoryMgr->ReadCategoryLC( categoryId );
		TPtrC name;
		category->GetCategoryName( name );
		iLog->Log( name );
		CleanupStack::PopAndDestroy( category );
		}
	iLog->Log(_L("ReferencedIterator Successfulll"));	
	CleanupStack::PopAndDestroy( 3, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;	
	}
	
	
// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::ReadCategory
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmCategoryManager::ReadCategoryL( CStifItemParser& aItem )
	{
	TInt num;
	TInt err = aItem.GetNextInt( num );
	// Open and init default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create category manager using NewL
	CPosLmCategoryManager* categoryMgr = CPosLmCategoryManager::NewL( *lmkDatabase );
	CleanupStack::PushL( categoryMgr );
	// Get Iterator
	CPosLmItemIterator* iterator = categoryMgr->CategoryIteratorL();
	CleanupStack::PushL( iterator );
	// Get a category id
	TPosLmItemId categoryId = iterator->NextL();
	if( KPosLmNullItemId == categoryId )
		{
		iLog->Log(_L("No category exists, can not test ReadCategory"));
		User::Leave( KErrGeneral );	
		}
	if( num == 0 )
		{
		// Set invalid id
		categoryId = 99999;
		}
	// Read category
	CPosLandmarkCategory* category = categoryMgr->ReadCategoryLC( categoryId );
	// Print name	
	TPtrC categoryNamePtr;
	category->GetCategoryName( categoryNamePtr );
	iLog->Log( categoryNamePtr );
	iLog->Log(_L("ReadCategory Successfulll"));	
	CleanupStack::PopAndDestroy( 4, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::GetCategory
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmCategoryManager::GetCategoryL( CStifItemParser& aItem )
	{
	TPtrC catNamePtr;
	TInt err = aItem.GetNextString( catNamePtr );	
	
	_LIT( KCatBusiness, "Business" ); 
	
	// Open and init default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL();
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create category manager using NewL
	CPosLmCategoryManager* categoryMgr = CPosLmCategoryManager::NewL( *lmkDatabase );
	CleanupStack::PushL( categoryMgr );
	//
	TPosLmItemId id = categoryMgr->GetCategoryL( KCatBusiness );
	if( KPosLmNullItemId == id )
		{
		// Not found
		CPosLandmarkCategory* category = NULL;
		TRAPD(error, category = CPosLandmarkCategory::NewL());
		if( KErrNone != error )
			{
			CleanupStack::PopAndDestroy( 2, lmkDatabase );	
			return error;
			}
		CleanupStack::PushL( category );
		TRAP( error, category->SetCategoryNameL( KCatBusiness ) );
		if( KErrNone != error )
			{
			CleanupStack::PopAndDestroy( 3, lmkDatabase );	
			return error;
			}
		// Add
		TRAP( error, categoryMgr->AddCategoryL( *category ) );
		if( KErrNone!= error )
			{
			CleanupStack::PopAndDestroy( 3, lmkDatabase );	
			return error;
			}
		CleanupStack::PopAndDestroy( category );	
		}
	// Get category
	id = categoryMgr->GetCategoryL( catNamePtr );
	if( KPosLmNullItemId == id )
		{
		// Not found
		iLog->Log(_L("Not found"));
		User::Leave(KErrNotFound);
		}
	// Found
	iLog->Log(_L("Name was found"));
	CleanupStack::PopAndDestroy( 2, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::RemoveCategories
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmCategoryManager::RemoveCategoriesL( CStifItemParser& /*aItem*/ )
	{
	// Open and init default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create category manager using NewL
	CPosLmCategoryManager* categoryMgr = CPosLmCategoryManager::NewL( *lmkDatabase );
	CleanupStack::PushL( categoryMgr );
	// Add 5 categories to test RemoveCategoriesL
	// Create new category
	RArray<TPosLmItemId> categoryIdArray;
	CPosLandmarkCategory* category = CPosLandmarkCategory::NewL();
	CleanupStack::PushL( category );
	// Set category name
	_LIT( KCategoryName, "A Test Category" );
	TBuf<25> categoryBuf;
	for(TInt i=1; i <= 5; i++)
		{
		// Set name
		categoryBuf.Copy( KCategoryName );
		categoryBuf.AppendNum( i );
		category->SetCategoryNameL( categoryBuf );
		// Add to database
		TPosLmItemId addCategoryId = categoryMgr->AddCategoryL( *category );
		// Added to database, save id into index array
		categoryIdArray.Append( addCategoryId );	
		} // Add loop
	
	// Remove these newly added 5 categories using RemoveCategories
	ExecuteAndDeleteLD( categoryMgr->RemoveCategoriesL(categoryIdArray) );
	iLog->Log(_L("RemoveCategories"));
	CleanupStack::PopAndDestroy( 3, lmkDatabase );
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::GetGlobalCategoryL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmCategoryManager::GetGlobalCategoryL( CStifItemParser& /*aItem*/ )
	{
	// Open and init default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create category manager using NewL
	CPosLmCategoryManager* categoryMgr = CPosLmCategoryManager::NewL( *lmkDatabase );
	CleanupStack::PushL( categoryMgr );
	// Global category id
	TPosLmGlobalCategory globalCatId = 1;
	TPosLmItemId id = categoryMgr->GetGlobalCategoryL( globalCatId );
	if( KPosLmNullItemId == id )
		{
		iLog->Log(_L("Global category not found in database"));
		CleanupStack::PopAndDestroy( 2, lmkDatabase );
		ReleaseLandmarkResources();	
		return KErrNotFound;
		}
	iLog->Log(_L("Global category found in database"));	
	CleanupStack::PopAndDestroy( 2, lmkDatabase );
	ReleaseLandmarkResources();	
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::GlobalCategoryNameL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmCategoryManager::GlobalCategoryNameL( CStifItemParser& /*aItem*/ )
	{
	// Open and init default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create category manager using NewL
	CPosLmCategoryManager* categoryMgr = CPosLmCategoryManager::NewL( *lmkDatabase );
	CleanupStack::PushL( categoryMgr );
	// Global category id
	TPosLmGlobalCategory globalCatId = 1;
	HBufC* name = categoryMgr->GlobalCategoryNameL( globalCatId );
	if ( name == NULL )
		{
		iLog->Log(_L("Global category not defined"));
		CleanupStack::PopAndDestroy( 2, lmkDatabase );
		ReleaseLandmarkResources();	
		return KErrNotFound;	
		}
	iLog->Log(_L("Global category is defined"));
	CleanupStack::PopAndDestroy( 2, lmkDatabase );
	ReleaseLandmarkResources();	
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// CTestPosLmCategoryManager::ResetGlobalCategoriesL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmCategoryManager::ResetGlobalCategoriesL( CStifItemParser& /*aItem*/ )
	{
	// Open and init default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create category manager using NewL
	CPosLmCategoryManager* categoryMgr = CPosLmCategoryManager::NewL( *lmkDatabase );
	CleanupStack::PushL( categoryMgr );
	// Reset
	ExecuteAndDeleteLD( categoryMgr->ResetGlobalCategoriesL());
	//
	iLog->Log(_L("Global category reset successfully"));
	CleanupStack::PopAndDestroy( 2, lmkDatabase );
	ReleaseLandmarkResources();	
	return KErrNone;	
	}
