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
* Description:  provides testing of CPosLandmark class methods
*
*/



// INCLUDE FILES
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>
#include <LbsPosition.h>
#include "testcposlandmark.h"
#include <StifParser.h>
#include <Stiftestinterface.h>

// Literals
// Test Database URI
_LIT( KDBUri, "file://c:eposlmtest.ldb" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestPosLandmark::CTestPosLandmark
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTestPosLandmark::CTestPosLandmark( CStifLogger* aLog )
	{
	iLog = aLog;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::NewL
// 
// 
// -----------------------------------------------------------------------------
//	
CTestPosLandmark* CTestPosLandmark::NewL( CStifLogger* aLog )
	{
	CTestPosLandmark* self = new (ELeave) CTestPosLandmark( aLog );
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); 
    return self; 
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::ConstructL
// 
// 
// -----------------------------------------------------------------------------
//	
void CTestPosLandmark::ConstructL()
	{
	
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmark::~CTestPosLandmark
// 
// 
// -----------------------------------------------------------------------------
//		
CTestPosLandmark::~CTestPosLandmark()
	{
	
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmark::CreateNewLandmarkL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::CreateNewLandmarkL( CStifItemParser& /*aItem*/ )
	{
	//Create an object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	delete lmk;
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::CreateNewLandmarkLC
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::CreateNewLandmarkLC( CStifItemParser& /*aItem*/ )
	{
	//Create an object using NewLC()
	CPosLandmark* lmk = CPosLandmark::NewLC();
	CleanupStack::PopAndDestroy( lmk );
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::CreateLandmarkCopyL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::CreateLandmarkCopyL( CStifItemParser& /*aItem*/ )
	{
	// Create an object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// Set radius of original landmark
	lmk->SetCoverageRadius( 2104 );
	// Create another object using copy constructor NewL
	CPosLandmark* lmkCopy = CPosLandmark::NewL( *lmk );
	CleanupStack::PushL( lmkCopy );
	// Get radius of new landmark
	TReal32 radius;
	lmkCopy->GetCoverageRadius( radius );
	// Check radius
	if( radius == 2104 )
		{
		iLog->Log( _L( "Landmark Creation passed" ) );
		//delete both the landmarks
		CleanupStack::PopAndDestroy( 2, lmk ); // lmkCopy
		return KErrNone; 	
		}
	else
		{
		iLog->Log( _L( "Landmark Creation failed" ) );
		CleanupStack::PopAndDestroy( 2, lmk ); // lmkCopy
		return KErrGeneral;
		}
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::CreateLandmarkCopyLC
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::CreateLandmarkCopyLC( CStifItemParser& /*aItem*/ )
	{
	//Create an object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// Create another landmark object using copy constructor NewLC
	CPosLandmark* lmkCopy = CPosLandmark::NewLC( *lmk );
	//
	CleanupStack::PopAndDestroy( 2, lmk ); // lmkCopy
	return KErrNone; 	
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::SetLandmarkNameL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::SetLandmarkNameL( CStifItemParser& aItem )
	{
	TPtrC name;
	TInt err = aItem.GetNextString( name ); // Landmark name
	//Create a new landmark object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// Set name of landmark
	lmk->SetLandmarkNameL( name );
	// Get name
	TPtrC getNamePtr;
	lmk->GetLandmarkName( getNamePtr );
	//
	iLog->Log( _L("Name of landmark was set...") );
	iLog->Log( getNamePtr );
	// Delete object
	CleanupStack::PopAndDestroy( lmk );	
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::GetLandmarkNameL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::GetLandmarkNameL( CStifItemParser& aItem )
	{
	TInt num;
	TInt err = aItem.GetNextInt( num ); // To set Landmark name or not
	//Create a new landmark object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// Set name of landmark if integer value passed from cfg file is 1
	if( num == 1 )
		{
		_LIT( KLmkName, "A landmark Name" );
		TBufC<20> nameBuf( KLmkName );
		lmk->SetLandmarkNameL( KLmkName );
		}
	// Get name
	TPtrC getNamePtr;
	TInt error = lmk->GetLandmarkName( getNamePtr );
	// Check name
	iLog->Log( _L("Got the name of landmark...") );
	iLog->Log( getNamePtr );
	// Delete object
	CleanupStack::PopAndDestroy( lmk );		
	return error;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::SetAndGetLmkDescL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::SetAndGetLmkDescL( CStifItemParser& aItem )
	{
	// For setting or not setting desc
	TInt num;
	TInt err = aItem.GetNextInt( num );	
	// Description
	TPtrC setDescPtr;
	err = aItem.GetNextString( setDescPtr ); // Landmark Desc
	//Create a new landmark object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// Set description of landmark if num == 1
	if( num == 1 )
		{
		lmk->SetLandmarkDescriptionL( setDescPtr );
		}
	// Get desc
	TPtrC getDescPtr;
	TInt error = lmk->GetLandmarkDescription( getDescPtr );
	if( KErrNone != error )
		{
		// Error
		iLog->Log( _L( "Error in getting description of landmark..." ) );
		User::Leave( error );
		}
	// Print desc
	iLog->Log( _L("Got the description of landmark...") );
	iLog->Log( getDescPtr );
	//Compare desc
	if( getDescPtr.Compare( setDescPtr ) != 0 )
		{
		iLog->Log( _L("Error in getting description of landmark...") );
		User::Leave( error );
		}
	//Test successful
	iLog->Log( _L("Set and get description of landmark successful...") );	
	// Delete object
	CleanupStack::PopAndDestroy( lmk );
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::SetAndGetCovRadiusL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::SetAndGetCovRadiusL( CStifItemParser& aItem )
	{
	// For setting or not setting radius
	TInt num;
	TInt err = aItem.GetNextInt( num );
	// Radius
	TPtrC radiusPtr;
	err = aItem.GetNextString( radiusPtr );
	TLex lex( radiusPtr );
	TReal32 radius = 23.56;
	lex.Val( radius );
	//Create a new landmark object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// Set coverage radius of landmark if num == 1
	if( num == 1 )
		{
		lmk->SetCoverageRadius( radius ); //Return type void
		}
	// Get cov radius
	TInt error;
	TReal32 getRadius;
	error = lmk->GetCoverageRadius( getRadius );
	if( KErrNone != error )
		{
		// Error
		iLog->Log( _L("Error in getting radius of landmark...") );
		User::Leave( error );
		}
	//Compare radius
	if( radius != getRadius )
		{
		iLog->Log( _L("Error in getting radius of landmark...") );
		User::Leave( error );
		}
	//Test successful
	iLog->Log( _L("Set and get radius of landmark successful...") );	
	// Delete object
	CleanupStack::PopAndDestroy( lmk );
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::SetAndGetPositionL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::SetAndGetPositionL( CStifItemParser& aItem )
	{
	// Decides whether to use or not Wgs84 data, and to set the position or not 
	TInt num;
	TInt err = aItem.GetNextInt( num );
	
	TPtrC latPtr;
	TPtrC longPtr;
	err = aItem.GetNextString( latPtr );
	err = aItem.GetNextString( longPtr );
	TLex lex1( latPtr );
	TLex lex2( longPtr );
	TReal64 latitude = 21.2435; 
	TReal64 longitude = 89.0001;
	lex1.Val( latitude );
	lex2.Val( longitude );	
		
	//Create a new landmark object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// Set position of landmark
	TInt error;
	
	TLocality position;
	position.SetCoordinate( latitude, longitude );
	// if num is -1 then use datum other than Wsg84
	if( num == -1 )
		{
		const TPositionDatumId KId = {0x00}; // Wgs84 is 0x101FAA29
		position.SetDatum( KId );
		}
	// set position only if num is not 0	
	if( num != 0 )
		{
		lmk->SetPositionL( position );
		}
	// Get position
	TLocality getPosition;
	error = lmk->GetPosition( getPosition );
	if( KErrNone != error )
		{
		// Error
		iLog->Log( _L("Error in getting position of landmark...") );
		User::Leave( error );
		}
	//Compare position
	//Latitude
	if( getPosition.Latitude() != latitude )
		{
		iLog->Log( _L("Error in getting position of landmark...") );
		User::Leave( KErrGeneral );
		}
	//Longitude
	if( getPosition.Longitude() != longitude )
		{
		iLog->Log( _L("Error in getting position of landmark...") );
		User::Leave( KErrGeneral );
		}
	//Test successful
	iLog->Log( _L("Set and get position of landmark successful...") );	
	// Delete object
	CleanupStack::PopAndDestroy( lmk );
	return KErrNone;
	}


// -----------------------------------------------------------------------------
// CTestPosLandmark::SetAndGetPositionFieldL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::SetAndGetPositionFieldL( CStifItemParser& aItem )
	{
	// Decides whether to set position field or not
	TInt num;
	TInt err = aItem.GetNextInt( num );
	
	TPtrC fieldNamePtr;
	err = aItem.GetNextString( fieldNamePtr );
	
	// Create a new landmark object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// Set Position field
	TPositionFieldId fieldId = 1;
	
	TInt error;
	// Set only if num == 1
	if( num == 1 )
		{
		lmk->SetPositionFieldL( fieldId, fieldNamePtr );	
		}
	
	//Get Position field
	TPtrC getPosField; 
	error = lmk->GetPositionField( fieldId, getPosField );
	if( KErrNone != error )
		{
		iLog->Log( _L("Error in getting position field...") );
		User::Leave( error );
		}
	//Print Position field
	iLog->Log( _L("Getting position field of landmark successful...") );
	iLog->Log(getPosField);
	//Compare position fields
	if( getPosField.Compare( fieldNamePtr ) != 0)
		{
		//Error
		iLog->Log( _L("Error in getting position field...") );
		User::Leave( KErrGeneral );//return KErrGeneral;
		}
	iLog->Log( _L("Get and set position field of landmark successful...") );
	CleanupStack::PopAndDestroy( lmk );
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::NumOfPosFieldsL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::NumOfPosFieldsL( CStifItemParser& /*aItem*/ )
	{
	// Create a new landmark object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	//
	TUint numOfPosFields;
	numOfPosFields = lmk->NumOfAvailablePositionFields();
	if( numOfPosFields != 0 )
		{
		iLog->Log( _L("Error in getting number of position fields of landmark...") );
		User::Leave( KErrGeneral );
		}
	iLog->Log( _L("Getting number of position fields of landmark successful...") );	
	CleanupStack::PopAndDestroy( lmk );
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::IsPosFieldAvailableL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::IsPosFieldAvailableL( CStifItemParser& /*aItem*/ )
	{
	// Create a new landmark object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// Check for position field with id 1, should return EFalse
	TPositionFieldId fieldId = 1;
	TBool result;
	result = lmk->IsPositionFieldAvailable( fieldId );
	if ( result )
		{
		iLog->Log( _L("Error in IsPositionFieldAvailable1...") );
		User::Leave( KErrGeneral );
		}
		
	// Add a position field to landmark at index 'fieldId'
	_LIT( KFieldName, "The City Name" );
	lmk->SetPositionFieldL( fieldId, KFieldName );
	// Check for position field, should return ETrue
	result = lmk->IsPositionFieldAvailable( fieldId );
	if ( !result )
		{
		iLog->Log( _L("Error in IsPositionFieldAvailable2...") );
		User::Leave( KErrGeneral );
		}
	iLog->Log( _L("IsPositionFieldAvailable successful...") );	
	CleanupStack::PopAndDestroy( lmk );
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::RemovePositionField
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::RemovePositionFieldL( CStifItemParser& /*aItem*/ )
	{
	// Create a new landmark object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// Add a position field
	TPositionFieldId fieldId = 1;
	_LIT( KFieldName, "The City Name" );
	//TInt error;
	lmk->SetPositionFieldL( fieldId, KFieldName );
	// Check if field is available, should return ETrue
	TBool result;
	result = lmk->IsPositionFieldAvailable( fieldId );
	if ( !result )
		{
		iLog->Log( _L("Error in setting position field, can not test RemovePositionField.") );
		User::Leave( KErrGeneral );
		}
	// Remove position field
	lmk->RemovePositionField( fieldId ); // No return
	// Check if field is available, should return EFalse
	result = lmk->IsPositionFieldAvailable( fieldId );
	if ( result )
		{
		iLog->Log( _L("Error in RemovePositionField...") );
		User::Leave( KErrGeneral );
		}
	iLog->Log( _L("RemovePositionField successful...") );	
	CleanupStack::PopAndDestroy( lmk );
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::RemoveAttributes
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::RemoveAttributesL( CStifItemParser& /*aItem*/ )
	{
	TInt error;
	// Create a new landmark object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// Set name of landmark
	_LIT( KLmkName, "A landmarkk Name" );
	lmk->SetLandmarkNameL( KLmkName );
	// Set position
	TReal64 latitude = 21.2435; //An arbitrary lat
	TReal64 longitude = 89.0001; //An arbitrary long
	TLocality position;
	position.SetCoordinate( latitude, longitude );
	lmk->SetPositionL( position );
	// Set coverage radius of landmark
	TReal32 radius = 250.5;
	lmk->SetCoverageRadius( radius ); //Return type void
	// Set description of landmark
	_LIT( KLmkDesc, "A Landmark Description" );
	lmk->SetLandmarkDescriptionL( KLmkDesc );
	
	// Remove name
	error = RemoveAttrName( lmk );
	if ( KErrNone != error )
		{
		iLog->Log(_L("Error in RemoveAttributes"));
		User::Leave( KErrGeneral );
		}
	// Remove position
	error = RemoveAttrPosition( lmk );
	if ( KErrNone != error)
		{
		iLog->Log(_L("Error in RemoveAttributes"));
		User::Leave( KErrGeneral );
		}
	// Remove radius
	error = RemoveAttrCovRadius( lmk );
	if ( KErrNone != error )
		{
		iLog->Log(_L("Error in RemoveAttributes"));
		User::Leave( KErrGeneral );
		}
	// Remove description
	error = RemoveAttrDescription( lmk );
	if ( KErrNone != error )
		{
		iLog->Log(_L("Error in RemoveAttributes"));
		User::Leave( KErrGeneral );
		}
	// Test successful	
	iLog->Log(_L("RemoveAttributes successful"));	
	CleanupStack::PopAndDestroy( lmk );
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmark::RemoveAttrName
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::RemoveAttrName( CPosLandmark* aLandmark )
	{
	// Get name
	TPtrC getNamePtr;
	TInt error = aLandmark->GetLandmarkName( getNamePtr );
	if( KErrNone != error )
		{
		iLog->Log( _L("Error in getting name of landmark, can not test RemoveAttrName...") );
		return error;
		}
	// Print name
	iLog->Log(getNamePtr);
	//Remove attributes, name
	aLandmark->RemoveLandmarkAttributes( CPosLandmark::ELandmarkName );
	// Check if removed
	error = aLandmark->GetLandmarkName( getNamePtr );
	if(KErrNotFound != error )
		{
		iLog->Log(_L("Error in RemoveAttributes Name"));
		return KErrGeneral;
		}
	iLog->Log(_L("RemoveAttributes Name successful"));	
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::RemoveAttrPosition
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::RemoveAttrPosition( CPosLandmark* aLandmark )
	{
	TInt error;
	// Get position
	TLocality getPosition;
	error = aLandmark->GetPosition( getPosition );
	if( KErrNone != error )
		{
		// Error
		iLog->Log( _L("Error in getting position of landmark, can not test RemoveAttrPosition...") );
		return error;
		}
	// Remove Position
	aLandmark->RemoveLandmarkAttributes( CPosLandmark::EPosition );
	// Check if removed successfully
	error = aLandmark->GetPosition( getPosition );
	if(KErrNotFound != error )
		{
		iLog->Log(_L("Error in RemoveAttributes Position"));
		return KErrGeneral;
		}
	iLog->Log(_L("RemoveAttributes Position successful"));	
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::RemoveAttrCovRadius
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::RemoveAttrCovRadius( CPosLandmark* aLandmark )
	{
	// Get cov radius
	TInt error;
	TReal32 getRadius;
	error = aLandmark->GetCoverageRadius( getRadius );
	if( KErrNone != error )
		{
		// Error
		iLog->Log( _L("Error in getting radius of landmark, can not test RemoveAttrCovRadius...") );
		return error;
		}
	// Remove radius
	aLandmark->RemoveLandmarkAttributes( CPosLandmark::ECoverageRadius );
	// Check if removed successfully
	error = aLandmark->GetCoverageRadius( getRadius );
	if( KErrNotFound != error )
		{
		iLog->Log(_L("Error in RemoveAttributes Radius"));
		return KErrGeneral;
		}
	iLog->Log(_L("RemoveAttributes Radius successful"));	
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::RemoveAttrDescription
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::RemoveAttrDescription( CPosLandmark* aLandmark )
	{
	TInt error;
	// Get desc
	TPtrC getDescPtr;
	error = aLandmark->GetLandmarkDescription( getDescPtr );
	if( KErrNone != error )
		{
		// Error
		iLog->Log( _L("Error in getting description of landmark, can not test RemoveAttrDescription...") );
		return error;
		}
	// Remove description
	aLandmark->RemoveLandmarkAttributes( CPosLandmark::EDescription );
	// Check if removed successfully
	error = aLandmark->GetLandmarkDescription( getDescPtr );
	if( KErrNotFound != error )
		{
		iLog->Log(_L("Error in RemoveAttributes Description"));
		return KErrGeneral;
		}
	iLog->Log(_L("RemoveAttributes Description successful"));	
	return KErrNone;	
	}

	
// -----------------------------------------------------------------------------
// CTestPosLandmark::PositionFieldIdL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::PositionFieldIdL( CStifItemParser& /*aItem*/ )
	{
	// Create a new landmark object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// Check without adding any position field
	TPositionFieldId currentFieldId;
	currentFieldId = lmk->FirstPositionFieldId();
	if( EPositionFieldNone != currentFieldId )
		{
		//
		iLog->Log( _L("Error in getting first position field id") );
		User::Leave( KErrGeneral );
		}
	// Set 5 position fields for the landmark, from index 1 to 5
	_LIT( KFieldName, "Position Field " );
	TInt error;
	for( TInt i = 1; i <= 5; i++ )
		{
		currentFieldId = i;
		TBuf<20> currentFieldName;
		currentFieldName.Copy( KFieldName );
		currentFieldName.AppendNum( i );
		lmk->SetPositionFieldL( currentFieldId, currentFieldName );
		} // Loop
	// Check
	// Get First Id
	currentFieldId = lmk->FirstPositionFieldId();
	if( EPositionFieldNone == currentFieldId )
		{
		//
		iLog->Log( _L("Error in getting first position field id") );
		User::Leave( KErrGeneral );
		}
	// Get and Print first position field
	TPtrC getPosField; 
	error = lmk->GetPositionField( currentFieldId, getPosField );
	if( KErrNone != error )
		{
		iLog->Log( _L("Error in getting position field, first position field id is wrong...") );
		User::Leave( KErrGeneral );
		}
	iLog->Log( getPosField );
	// Get Next 4 Ids, get corresponding position field and print
	TPositionFieldId previoudFieldId;
	for( TInt i = 1; i <= 4; i++ )
		{
		// Get id
		previoudFieldId = currentFieldId;
		currentFieldId = lmk->NextPositionFieldId( previoudFieldId );
		// Check
		if( EPositionFieldNone == currentFieldId )
			{
			//
			iLog->Log( _L("Error in getting next position field id") );
			User::Leave( KErrGeneral );
			}
		// Get field
		error = lmk->GetPositionField( currentFieldId, getPosField );
		//Check
		if( KErrNone != error )
			{
			iLog->Log( _L("Error in getting position field, next position field id is wrong...") );
			User::Leave( KErrGeneral );
			}
		// Print	
		iLog->Log( getPosField );
		} // Loop
	// Try to retrieve one more id, should return EPositionFieldNone
	previoudFieldId = currentFieldId;
	currentFieldId = lmk->NextPositionFieldId( previoudFieldId );
	// Check
	if( EPositionFieldNone != currentFieldId )
		{
		//
		iLog->Log( _L("Error in getting next position field id") );
		User::Leave( KErrGeneral );
		}
	iLog->Log( _L("PositionFieldId successful") );
	CleanupStack::PopAndDestroy( lmk );
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmark::LandmarkIdL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::LandmarkIdL( CStifItemParser& /*aItem*/ )
	{
	// Create a new landmark object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// lmk has not been added to the database yet, should return KPosLmNullItemId
	TPosLmItemId lmkId;
	lmkId = lmk->LandmarkId();
	if( KPosLmNullItemId != lmkId )
		{
		iLog->Log( _L("Error in getting LandmarkId") );
		User::Leave( KErrGeneral );
		}
	// Add lmk to database
	// Open default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Initialize database
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Add landmark lmk to database
	TPosLmItemId addId;
	addId=lmkDatabase->AddLandmarkL( *lmk );
	// Now lmk is added to database, should return valid id
	lmkId = lmk->LandmarkId();
	if( KPosLmNullItemId == lmkId )
		{
		iLog->Log( _L("Error in getting LandmarkId") );
		User::Leave( KErrGeneral );
		}
			
	// Compare ids
	if( lmkId != addId )
		{
		iLog->Log( _L("Error in getting LandmarkId, wrong id returned") );
		User::Leave( KErrGeneral );
		}
	iLog->Log( _L("LandmarkId Successfulll") );
	// Remove the added landmark from database
	lmkDatabase->RemoveLandmarkL( lmkId );
	CleanupStack::PopAndDestroy( 2, lmk );
	ReleaseLandmarkResources();
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmark::IsPartial
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::IsPartialL( CStifItemParser& /*aItem*/ )
	{
	// Create a new landmark object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// Check if partial, should return EFalse
	TBool result;
	result = lmk->IsPartial();
	if( result )
		{
		iLog->Log( _L("Error in IsPartial") );
		User::Leave( KErrGeneral );
		}
	// Open database, add landmark, read again as partial, check with IsPartial
	
	// Open and initialize DB
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Initialize database
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Add landmark lmk to database
	TPosLmItemId lmkId;
	lmkId = lmkDatabase->AddLandmarkL( *lmk );
	// Read partial landmark
	CPosLandmark* partialLmk = lmkDatabase->ReadPartialLandmarkLC( lmkId );
	// Check if partial, should return ETrue
	result = partialLmk->IsPartial();
	if( !result )
		{
		iLog->Log( _L("Error in IsPartial") );
		User::Leave( KErrGeneral );
		}
	//	
	iLog->Log( _L("IsPartial successful") );
	//Remove landmark from database
	lmkDatabase->RemoveLandmarkL( lmkId );
	
	CleanupStack::PopAndDestroy( 3, lmk );
	ReleaseLandmarkResources();
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// CTestPosLandmark::SetAndGetIcon
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::SetAndGetIconL( CStifItemParser& aItem )
	{
	TInt num;
	TInt err = aItem.GetNextInt( num );
	
	TPtrC iconPathPtr;
	err = aItem.GetNextString( iconPathPtr );	
	//
	// Create a landmark
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// Set Icon for landmark if num == 1
	if( num == 1 )
		{
		lmk->SetIconL( iconPathPtr, 1, 1 );	
		}
	// Get Icon
	TPtrC fileName;
	TInt index;
	TInt mask;
	TInt error;
	error = lmk->GetIcon( fileName, index, mask);
	if( KErrNone != error )
		{
		iLog->Log( _L("Error in Getting Icon"));
		User::Leave( error );
		}
	iLog->Log( fileName );
	iLog->Log( _L("SetAndGetIcon successful"));	
	CleanupStack::PopAndDestroy( lmk );
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::AddAndRemoveLmCategory
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::AddAndRemoveLmCategoryL( CStifItemParser& aItem )
	{
	TPtrC catNamePtr;
	TInt err = aItem.GetNextString( catNamePtr );	
	// Create a new landmark object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	
	// Open default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Initialize database
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );

	// Create a category manager
	CPosLmCategoryManager* categoryMgr = CPosLmCategoryManager::NewL( *lmkDatabase );
	CleanupStack::PushL( categoryMgr );
	// Add a category to database, get category id
	// Create category
	CPosLandmarkCategory* category = CPosLandmarkCategory::NewL();
	CleanupStack::PushL( category );
	category->SetCategoryNameL( catNamePtr );
	// Add to database
	TPosLmItemId categoryId;
	categoryId = categoryMgr->AddCategoryL( *category );
	// Add category to landmark
	lmk->AddCategoryL( categoryId );
	// Get and check
	RArray<TPosLmItemId> idArray;
	lmk->GetCategoriesL( idArray );
	if( idArray.Count() != 1 )
		{
		iLog->Log( _L("AddCateGory fails, category not added to landmark ") );
		//Remove category from database
		ExecuteAndDeleteLD( categoryMgr->RemoveCategoryL( categoryId ) );
		User::Leave( KErrGeneral );
		}
	// Remove category from landmark
	lmk->RemoveCategory( categoryId );
	// Get and check
	lmk->GetCategoriesL( idArray );
	if( idArray.Count() != 0 )
		{
		iLog->Log( _L("RemoveCategory fails, category not removed from landmark ") );
		//Remove category from database
		ExecuteAndDeleteLD( categoryMgr->RemoveCategoryL( categoryId ) );
		//Cleanup
		User::Leave(KErrGeneral);
		}
	iLog->Log( _L("AddAndRemoveCategory successful") );
	//Remove category from database
	ExecuteAndDeleteLD( categoryMgr->RemoveCategoryL( categoryId ) );
	//Cleanup
	CleanupStack::PopAndDestroy( 4, lmk );
	ReleaseLandmarkResources();
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CTestPosLandmark::GetCategories
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::GetCategoriesL( CStifItemParser& /*aItem*/ )
	{
	// Create a new landmark object using NewL()
	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	// Get Categories
	RArray<TPosLmItemId> idArray;
	lmk->GetCategoriesL( idArray );
	// Check, no category exists in landmark, should be 0
	if( idArray.Count() != 0 )
		{
		iLog->Log(_L("GetCategories fails"));
		User::Leave( KErrGeneral );
		}
	// Add a category to landmark and test again
		
	// Open default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Initialize database
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// New Category
	CPosLandmarkCategory* category = CPosLandmarkCategory::NewL();
	CleanupStack::PushL( category );
	_LIT( KCategoryName, "A category Name" );
	TBufC<20> categoryBuf( KCategoryName );
	category->SetCategoryNameL( categoryBuf );
	// New Category manager
	CPosLmCategoryManager* categoryMgr = CPosLmCategoryManager::NewL( *lmkDatabase );
	CleanupStack::PushL( categoryMgr );
	// Add Category to database
	TPosLmItemId categoryId;
	categoryId = categoryMgr->AddCategoryL( *category );
	// Add Category to landmark
	lmk->AddCategoryL( categoryId );
	// Get Categories
	lmk->GetCategoriesL( idArray );
	// Check, 1 category exists in landmark, count of array items should be 1
	if( idArray.Count() != 1 )
		{
		iLog->Log(_L("GetCategories fails"));
		//Remove category from database
		ExecuteAndDeleteLD( categoryMgr->RemoveCategoryL( categoryId ) );
		User::Leave( KErrGeneral );
		}
	iLog->Log( _L("GetCategories successful") );
	//Remove category from database
	ExecuteAndDeleteLD( categoryMgr->RemoveCategoryL( categoryId ) );
	//Cleanup
	CleanupStack::PopAndDestroy( 4, lmk );
	ReleaseLandmarkResources();
	return KErrNone;	
	}
// -----------------------------------------------------------------------------
// CTestPosLandmark::SetAndGetPositionFieldL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::SetAndGetPlaceId( CStifItemParser& aItem )
    {
    TInt returnCode = KErrGeneral;
    _LIT(KPID, "1234");
    CPosLandmark* lmk = CPosLandmark::NewL();
    CleanupStack::PushL( lmk );
   
    TPtrC retPID;
    lmk->SetPlaceIdL(KPID);
    returnCode = lmk->GetPlaceId(retPID);
    if(KErrNone != returnCode || (retPID != KPID))
        {
        returnCode =  KErrGeneral; 
        }
    else
        {
        returnCode = KErrNone;
        }
    CleanupStack::PopAndDestroy(lmk);
    return returnCode;
     }
// -----------------------------------------------------------------------------
// CTestPosLandmark::SetAndGetPositionFieldL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLandmark::SetAndGetTimeStamp( CStifItemParser& aItem )
    {
    TInt returnCode = KErrGeneral;
    CPosLandmark* lmk = CPosLandmark::NewL();
    CleanupStack::PushL( lmk );
    
    TTime setTime;
    setTime.Set(_L("20091006:000000.000000"));
    TTime retTime;
    
    lmk->SetTimeStampL(setTime);
    returnCode = lmk->GetTimeStamp(retTime);
    if(setTime!=retTime || returnCode!= KErrNone)
        {
        returnCode = KErrGeneral;
        }
    else
        returnCode = KErrNone;
    CleanupStack::PopAndDestroy(lmk);
    return returnCode;
    }
