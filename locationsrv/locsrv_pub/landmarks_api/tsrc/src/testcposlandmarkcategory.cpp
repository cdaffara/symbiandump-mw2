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
* Description:  Provides testing of CPosLandmarkCategory class methods
*
*/



// INCLUDE FILES
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>
#include <LbsPosition.h>
#include "testcposlandmarkcategory.h"
#include <StifParser.h>
#include <Stiftestinterface.h>

// Literals
// Test Database URI
_LIT( KDBUri, "file://c:eposlmtest.ldb" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestPosLandmarkCategory::CTestPosLandmarkCategory
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestPosLandmarkCategory::CTestPosLandmarkCategory( CStifLogger* aLog )
	{
	iLog = aLog;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkCategory::NewL
// 
// 
// -----------------------------------------------------------------------------
//	
CTestPosLandmarkCategory* CTestPosLandmarkCategory::NewL(CStifLogger* aLog)
	{
	CTestPosLandmarkCategory* self = new (ELeave) CTestPosLandmarkCategory( aLog );
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); 
    return self; 
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkCategory::ConstructL
// 
// 
// -----------------------------------------------------------------------------
//
void CTestPosLandmarkCategory::ConstructL()
	{
	
	}	

// -----------------------------------------------------------------------------
// CTestPosLandmarkCategory::~CTestPosLandmarkCategory
// 
// 
// -----------------------------------------------------------------------------
//	
CTestPosLandmarkCategory::~CTestPosLandmarkCategory()
	{
	
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkCategory::NewLCreateCategory
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkCategory::NewLCreateCategoryL( CStifItemParser& /*aItem*/ )
	{
	// Create category using NewL
	CPosLandmarkCategory* category = CPosLandmarkCategory::NewL();
	delete category;
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkCategory::NewLCCreateCategory
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkCategory::NewLCCreateCategoryLC( CStifItemParser& /*aItem*/ )
	{
	// Create category using NewLC
	CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
	CleanupStack::PopAndDestroy( category );
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkCategory::NewLCreateCategoryCopy
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkCategory::NewLCreateCategoryCopyL( CStifItemParser& /*aItem*/ )
	{
	// Create category using NewL
	CPosLandmarkCategory* category = category = CPosLandmarkCategory::NewL();
	CleanupStack::PushL( category );
	// Create copy using NewL
	CPosLandmarkCategory* categoryCopy = CPosLandmarkCategory::NewL( *category );
	iLog->Log(_L("NewLCreateCategoryCopy Successful"));
	delete categoryCopy;
	CleanupStack::PopAndDestroy( category );
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkCategory::NewLCCreateCategoryCopy
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkCategory::NewLCCreateCategoryCopyLC( CStifItemParser& /*aItem*/ )
	{
	// Create category using NewL
	CPosLandmarkCategory* category = CPosLandmarkCategory::NewL();
	CleanupStack::PushL( category );
	// Create copy using NewLC
	CPosLandmarkCategory* categoryCopy = CPosLandmarkCategory::NewLC( *category );
	iLog->Log(_L("NewLCCreateCategoryCopy Successful"));
	CleanupStack::PopAndDestroy( 2, category );
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkCategory::SetAndGetCategoryName
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkCategory::SetAndGetCategoryNameL( CStifItemParser& aItem )
	{
	TInt num;
	TInt error = aItem.GetNextInt( num );
	TPtrC setNamePtr;
	error = aItem.GetNextString( setNamePtr );	
	// Create category using NewL
	CPosLandmarkCategory* category = CPosLandmarkCategory::NewL();
	CleanupStack::PushL( category );
	// Set name
	if( num == 1 )
		{
		category->SetCategoryNameL( setNamePtr );
		}
	
	// Get name
	TPtrC getNamePtr;	
	error = category->GetCategoryName( getNamePtr );
	if( KErrNone != error )
		{
		// Error
		iLog->Log(_L("Error in getting category name"));
		User::Leave( KErrNotFound );
		}
	// Compare name
	if( getNamePtr.Compare( setNamePtr ) != 0 )
		{
		// Error
		iLog->Log(_L("Set name and Get name does not match, error in SetAndGetCategoryName"));
		User::Leave( KErrGeneral );
		}
	iLog->Log(_L("SetAndGetCategoryName Successful"));
	CleanupStack::PopAndDestroy( category );
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkCategory::SetAndGetCategoryIcon
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkCategory::SetAndGetCategoryIconL( CStifItemParser& aItem )
	{
	TInt num;
	TInt err = aItem.GetNextInt( num );
	TPtrC setIconFilePtr;
	err = aItem.GetNextString( setIconFilePtr );
	
	// Create category using NewL
	CPosLandmarkCategory* category = CPosLandmarkCategory::NewL();
	CleanupStack::PushL( category );
	// Set Icon for category if num == 1
	if ( num == 1 )
		{
		category->SetIconL( setIconFilePtr, 1, 1 );	
		}
	// Get Icon
	TPtrC getIconFilePtr;
	TInt index;
	TInt mask;
	TInt error = category->GetIcon( getIconFilePtr, index, mask );
	if( KErrNone != error )
		{
		iLog->Log( _L("Error in Getting Icon"));
		User::Leave( error );
		}
	iLog->Log(getIconFilePtr);
	iLog->Log(_L("SetAndGetCategoryIcon Successful"));
	CleanupStack::PopAndDestroy( category );
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmarkCategory::CategoryId
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkCategory::CategoryIdL( CStifItemParser& aItem )
	{
	TPtrC catNamePtr;
	TInt err = 	aItem.GetNextString( catNamePtr );
	// Create category using NewL
	CPosLandmarkCategory* category = CPosLandmarkCategory::NewL();
	CleanupStack::PushL( category );
	// Check id, should return KPosLmNullItemId as category is not yet added to database
	TPosLmItemId getCategoryId =  category->CategoryId();
	if( KPosLmNullItemId != getCategoryId )
		{
		iLog->Log(_L("Error in getting category id"));
		User::Leave( KErrGeneral );
		}
	// Add category to default database
	// Open and intialize default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Create category manager
	CPosLmCategoryManager* categoryMgr = CPosLmCategoryManager::NewL( *lmkDatabase );	
	CleanupStack::PushL( categoryMgr );
	// Add category to databse
	// Set name, a category must have name set before being added to the database
	category->SetCategoryNameL( catNamePtr );
	// Add
	TPosLmItemId addCategoryId;
	addCategoryId = categoryMgr->AddCategoryL( *category );
	// Get category id, should return proper id as category is now added to database
	getCategoryId = category->CategoryId();
	// Compare ids
	if( getCategoryId != addCategoryId )
		{
		iLog->Log(_L("Error in getting category id"));
		//Remove category from database
		ExecuteAndDeleteLD( categoryMgr->RemoveCategoryL( addCategoryId ) );
		User::Leave( KErrGeneral );
		//ReleaseLandmarkResources(); //?????
		}
	iLog->Log(_L("CategoryId Successful"));
	//Remove category from database
	ExecuteAndDeleteLD( categoryMgr->RemoveCategoryL( addCategoryId ) );
	CleanupStack::PopAndDestroy( 3, category );
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkCategory::CheckGlobal
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkCategory::CheckGlobalL( CStifItemParser& /*aItem*/ )
	{
	// Create a category using NewL
	CPosLandmarkCategory* category = CPosLandmarkCategory::NewL();
	CleanupStack::PushL( category );
	// Check if global, should return KPosLmNullGlobalCategory
	TPosLmGlobalCategory globalId = category->GlobalCategory();
	if( KPosLmNullGlobalCategory != globalId )
		{
		//Error
		iLog->Log(_L("Error in GlobalCategory"));
		User::Leave( KErrGeneral );
		}
	iLog->Log(_L("SetAndCheckGlobal successful"));
	CleanupStack::PopAndDestroy( category );
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmarkCategory::RemoveCategoryAttributes
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmarkCategory::RemoveCategoryAttributesL( CStifItemParser& /*aItem*/ )
	{
	// Create a category using NewL
	_LIT( KIconFilePath, "z:\\resource\\eposlmglcategories.mif" );
	CPosLandmarkCategory* category = CPosLandmarkCategory::NewL();
	CleanupStack::PushL( category );
	// Set Name
	_LIT( KCatName, "A Test Category" );
	category->SetCategoryNameL( KCatName );
	// Set Icon
	category->SetIconL( KIconFilePath, 1, 1);
	// Remove name
	category->RemoveCategoryAttributes( CPosLandmarkCategory::ECategoryName );
	// Check
	TPtrC catName;
	TInt error = category->GetCategoryName( catName );
	if( KErrNotFound != error )
		{
		iLog->Log(_L("RemoveCategoryAttributes failed"));
		User::Leave( KErrGeneral );	
		}
	// Remove Icon
	category->RemoveCategoryAttributes( CPosLandmarkCategory::EIcon );
	// Check
	TPtrC fileName;
	TInt index;
	TInt mask;
	error = category->GetIcon( fileName, index, mask);
	if( KErrNotFound != error )
		{
		iLog->Log(_L("RemoveCategoryAttributes failed"));
		User::Leave( KErrGeneral );
		}
	iLog->Log(_L("RemoveCategoryAttributes successful"));
	CleanupStack::PopAndDestroy( category );
	return KErrNone;
	}	
