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
* Description:  Provides testing of PosLandmarkSerialization class methods
*
*/



// INCLUDE FILES
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>
#include <LbsPosition.h>
#include <EPos_PosLandmarkSerialization.h>
#include "testposlandmarkserialization.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TestPosLmkSerialization::TestPosLmkSerialization
// C++ default constructor 
// 
// -----------------------------------------------------------------------------
//
TestPosLmkSerialization::TestPosLmkSerialization( CStifLogger* aLog )
	{
	iLog = aLog;
	}

// -----------------------------------------------------------------------------
// TestPosLmkSerialization::PackLandmark
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestPosLmkSerialization::PackLandmarkL( CStifItemParser& /*aItem*/ )
	{
	// Create Landmark to be packed
	CPosLandmark* landmark = CPosLandmark::NewL();
	CleanupStack::PushL( landmark );
	// Set name and description attributes for the landmark
	// Name
	_LIT( KLmkName, "A Test Landmark" );
	landmark->SetLandmarkNameL( KLmkName );
	/// Description
	_LIT( KLmkDesc, "A Landmark Description" );
	landmark->SetLandmarkDescriptionL( KLmkDesc );
	// Pack it
	HBufC8* lmkBuffer = PosLandmarkSerialization::PackL( *landmark );
	//
	CleanupStack::PopAndDestroy( landmark );
	iLog->Log(_L("PackLandmark succeeds"));	
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// TestPosLmkSerialization::UnpackLandmark
// 
// 
// -----------------------------------------------------------------------------
//
TInt TestPosLmkSerialization::UnpackLandmarkL( CStifItemParser& /*aItem*/ )
	{
	// Create Landmark
	CPosLandmark* landmark = CPosLandmark::NewL();
	CleanupStack::PushL( landmark );
	// Set name and description attributes for the landmark
	// Name
	_LIT( KLmkName, "A Test Landmark" );
	landmark->SetLandmarkNameL( KLmkName );
	/// Description
	_LIT( KLmkDesc, "A Landmark Description" );
	landmark->SetLandmarkDescriptionL( KLmkDesc );
	// Pack it
	HBufC8* lmkBuffer = PosLandmarkSerialization::PackL( *landmark );
	// Unpack landmark
	CPosLandmark* unpackedLmk = PosLandmarkSerialization::UnpackL( *lmkBuffer );
	CleanupStack::PushL( unpackedLmk );
	
	CleanupStack::PopAndDestroy( 2, landmark );
	iLog->Log(_L("UnpackLandmark succeeds"));	
	return KErrNone;
	}	
	
