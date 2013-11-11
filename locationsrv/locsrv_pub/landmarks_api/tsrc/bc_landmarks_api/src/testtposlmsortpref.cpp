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
* Description:  Provides testing of TPosLmSortPref class methods
*
*/



// INCLUDE FILES
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>
#include <LbsPosition.h>
#include "testtposlmsortpref.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TTestPosLmSortPref::TTestPosLmSortPref
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TTestPosLmSortPref::TTestPosLmSortPref( CStifLogger* aLog )
	{
	iLog = aLog;
	}

// -----------------------------------------------------------------------------
// TTestPosLmSortPref::CreateSortPref
// 
// 
// -----------------------------------------------------------------------------
//
TInt TTestPosLmSortPref::CreateSortPref( CStifItemParser& /*aItem*/ )
	{
	// Create , sorting by position in ascending order	
	TPosLmSortPref sortPref( CPosLandmark::EPosition );	
	//
	iLog->Log(_L("CreateSortPref succeeds"));	
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// TTestPosLmSortPref::GetSortOrder
// 
// 
// -----------------------------------------------------------------------------
//
TInt TTestPosLmSortPref::GetSortOrder( CStifItemParser& /*aItem*/ )
	{
	// Create , sorting by position in ascending order	
	TPosLmSortPref sortPref( CPosLandmark::EPosition );
	// Get
	TPosLmSortPref::TSortOrder sortOrder = sortPref.SortOrder();
	// Check
	if( sortOrder != TPosLmSortPref::EAscending )
		{
		iLog->Log(_L("Error in GetSortOrder"));
		return KErrGeneral;
		}
	iLog->Log(_L("GetSortOrder succeeds"));	
	return KErrNone;
	}	
	
// -----------------------------------------------------------------------------
// TTestPosLmSortPref::SetAndGetSortAttr
// 
// 
// -----------------------------------------------------------------------------
//
TInt TTestPosLmSortPref::SetAndGetSortAttr( CStifItemParser& /*aItem*/ )
	{
	TPosLmSortPref sortPref( CPosLandmark::EPosition );
	// Set, sort by name
	sortPref.SetSortByLandmarkAttribute( CPosLandmark::ELandmarkName );
	// Get
	CPosLandmark::TAttributes attr = sortPref.LandmarkAttributeToSortBy();
	// Compare
	if( attr != CPosLandmark::ELandmarkName )
		{
		iLog->Log(_L("Error in SetAndGetSortAttr"));
		return KErrGeneral;	
		}
	iLog->Log(_L("SetAndGetSortAttr succeeds"));		
	return KErrNone;
	}
