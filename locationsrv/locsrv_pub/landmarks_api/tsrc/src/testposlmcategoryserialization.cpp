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
* Description:  Provides testing of PosLmCategorySerialization class methods
*
*/



// INCLUDE FILES
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>
#include <LbsPosition.h>
#include <EPos_PosLmCategorySerialization.h>
#include "testposlmcategoryserialization.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TestPosLmkCatSerialization::TestPosLmkCatSerialization
// 
// 
// -----------------------------------------------------------------------------
//
TestPosLmkCatSerialization::TestPosLmkCatSerialization( CStifLogger* aLog )
	{
	iLog = aLog;
	}

// -----------------------------------------------------------------------------
// TestPosLmkCatSerialization::PackCategory
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestPosLmkCatSerialization::PackCategoryL( CStifItemParser& /*aItem*/ )
	{
	// Create category
	CPosLandmarkCategory* category = CPosLandmarkCategory::NewL();
	CleanupStack::PushL( category );
	// Set name 
	// Name
	_LIT( KCatName, "A Test Category" );
	category->SetCategoryNameL( KCatName );
	// Pack it
	HBufC8* catBuffer = PosLmCategorySerialization::PackL( *category );
	CleanupStack::PopAndDestroy( category );
	iLog->Log(_L("PackCategory succeeds"));	
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// TestPosLmkCatSerialization::UnpackCategory
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestPosLmkCatSerialization::UnpackCategoryL( CStifItemParser& /*aItem*/ )
	{
	// Create category
	CPosLandmarkCategory* category = CPosLandmarkCategory::NewL();
	CleanupStack::PushL( category );
	// Set name 
	// Name
	_LIT( KCatName, "A Test Category" );
	category->SetCategoryNameL( KCatName );
	// Pack it
	HBufC8* catBuffer = PosLmCategorySerialization::PackL( *category );
	// Unpack it
	CPosLandmarkCategory* unpackedCat = PosLmCategorySerialization::UnpackL( *catBuffer );
	CleanupStack::PushL( unpackedCat );
	// Check name
	CleanupStack::PopAndDestroy( 2, category );
	iLog->Log(_L("PackCategory succeeds"));	
	return KErrNone;
	}	
