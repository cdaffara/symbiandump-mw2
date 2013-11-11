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
* Description:  Provides testing of CPosLmPartialReadParameters class methods
*
*/



// INCLUDE FILES
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>
#include <LbsPosition.h>
#include "testcposlmpartialreadparameters.h"



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestPosLmPartialReadParams::CTestPosLmPartialReadParams
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestPosLmPartialReadParams::CTestPosLmPartialReadParams( CStifLogger* aLog )
	{
	iLog = aLog;
	}

// -----------------------------------------------------------------------------
// CTestPosLmPartialReadParams::NewL
// 
// 
// -----------------------------------------------------------------------------
//	
CTestPosLmPartialReadParams* CTestPosLmPartialReadParams::NewL( CStifLogger* aLog )
	{
	CTestPosLmPartialReadParams* self = new (ELeave) CTestPosLmPartialReadParams( aLog );
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); 
    return self; 
	}

// -----------------------------------------------------------------------------
// CTestPosLmPartialReadParams::ConstructL
// 
// 
// -----------------------------------------------------------------------------
//
void CTestPosLmPartialReadParams::ConstructL()
	{
	
	}	

// -----------------------------------------------------------------------------
// CTestPosLmPartialReadParams::~CTestPosLmPartialReadParams
// 
// 
// -----------------------------------------------------------------------------
//
CTestPosLmPartialReadParams::~CTestPosLmPartialReadParams()
	{
	
	}

// -----------------------------------------------------------------------------
// CTestPosLmPartialReadParams::CreatePartialReadParams
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmPartialReadParams::CreatePartialReadParamsL( CStifItemParser& /*aItem*/ )
	{
	CPosLmPartialReadParameters* partReadParams = CPosLmPartialReadParameters::NewLC();
	
	CleanupStack::PopAndDestroy( partReadParams );
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLmPartialReadParams::SetAndGetRequestedAttr
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmPartialReadParams::SetAndGetRequestedAttrL( CStifItemParser& /*aItem*/ )
	{
	CPosLmPartialReadParameters* partReadParams = CPosLmPartialReadParameters::NewLC();
	// Set requested attributes
	partReadParams->SetRequestedAttributes( CPosLandmark::ELandmarkName | CPosLandmark::EPosition );
	// Get Requested attributes
	CPosLandmark::TAttributes attr;
	attr = partReadParams->RequestedAttributes();
	// Check
	// Name, should be found
	if( !(attr & CPosLandmark::ELandmarkName) )
		{
		iLog->Log(_L("Error in SetAndGetRequestedAttr"));
		CleanupStack::PopAndDestroy( partReadParams );
		return KErrGeneral;	
		}
	// Position, should be found
	if( !(attr & CPosLandmark::EPosition) )
		{
		iLog->Log(_L("Error in SetAndGetRequestedAttr"));
		CleanupStack::PopAndDestroy( partReadParams );
		return KErrGeneral;	
		}
	// Description, should not be found
	if( attr & CPosLandmark::EDescription )
		{
		iLog->Log(_L("Error in SetAndGetRequestedAttr"));
		CleanupStack::PopAndDestroy( partReadParams );
		return KErrGeneral;	
		}
	iLog->Log(_L("SetAndGetRequestedAttr succeeds"));	
	CleanupStack::PopAndDestroy( partReadParams );	
	return KErrNone;
	}	
	
// -----------------------------------------------------------------------------
// CTestPosLmPartialReadParams::SetAndGetRequestedPosField
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmPartialReadParams::SetAndGetRequestedPosFieldL( CStifItemParser& /*aItem*/ )
	{
	CPosLmPartialReadParameters* partReadParams = CPosLmPartialReadParameters::NewLC();
	// Set First and Second position field
	RArray<TUint> setReqFieldArray;
	//
	setReqFieldArray.Append( 0 );
	setReqFieldArray.Append( 1 );
	TInt error;
	error = partReadParams->SetRequestedPositionFields( setReqFieldArray );
	if( KErrNone != error )
		{
		iLog->Log(_L("Error in SetAndGetRequestedPosField"));
		CleanupStack::PopAndDestroy( partReadParams );
		return KErrGeneral;		
		}
	// Get position fields
	RArray<TUint> getReqFieldArray;
	error = partReadParams->GetRequestedPositionFields( getReqFieldArray );
	if( KErrNone != error )
		{
		iLog->Log(_L("Error in SetAndGetRequestedPosField"));
		CleanupStack::PopAndDestroy( partReadParams );
		return KErrGeneral;		
		}
	// Check
	if( getReqFieldArray.Count() != 2 )
		{
		iLog->Log(_L("Error in SetAndGetRequestedPosField"));
		CleanupStack::PopAndDestroy( partReadParams );
		return KErrGeneral;			
		}
	iLog->Log(_L("SetAndGetRequestedPosField succeeds"));
	CleanupStack::PopAndDestroy( partReadParams );
	return KErrNone;
	}
