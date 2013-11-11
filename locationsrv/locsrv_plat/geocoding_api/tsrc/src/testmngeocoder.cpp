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
* Description:  Test Module DLL to Geocoding API
*
*/


// System Includes
#include <mngeocoder.h>
#include <w32std.h>
#include <coemain.h>
#include <eikon.hrh>

#include <mnproviderfinder.h>
#include <mnprovider.h>
#include <mngeocoder.h>
#include <EPos_CPosLandmark.h>
#include <LbsPosition.h>
#include <LbsFieldIds.h>

// User Includes
#include "testmngeocoder.h"


// Constant Declarations

    
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ Default Constructor
// ---------------------------------------------------------------------------
//
CTestMnGeocoder* CTestMnGeocoder::NewL(CStifLogger* aLog)
    {
    CTestMnGeocoder* self = new(ELeave) CTestMnGeocoder(aLog);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// C++ Default Constructor
// ---------------------------------------------------------------------------
//
void CTestMnGeocoder::ConstructL()
    {
    MnProviderFinder::FindProvidersL(iProviderArray);
    iUid.iUid = KProviderUid;
    iLmk = CPosLandmark::NewL();
    
    int i;
    for( i=0; i<iProviderArray.Count(); i++ )
    	{
    	if((iProviderArray.operator[](i))->Uid() == iUid)
			iMnProvider = iProviderArray.operator[](i);
    	}
    iLog->Log(_L("B4 Create"));
    iMnGeocoder = CMnGeocoder::NewL(*iMnProvider);
    iLog->Log(_L("After Create"));
    }
    
// ---------------------------------------------------------------------------
// C++ Default Constructor
// ---------------------------------------------------------------------------
//
CTestMnGeocoder::CTestMnGeocoder(CStifLogger* aLog)
    :CActive(0),iLog(aLog)
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// C++ Destructor
// ---------------------------------------------------------------------------
//
CTestMnGeocoder::~CTestMnGeocoder()
    {
    if( IsActive() )
	    {
	    Cancel(); 	
	    }
	    
    if(iMnGeocoder)
    	{
    	delete iMnGeocoder;	
    	iMnGeocoder = NULL;
    	}
    	
    if(iLmk)
    	{
    	delete iLmk;	
    	iLmk = NULL;
    	}	
    	
    iProviderArray.ResetAndDestroy();
    iProviderArray.Close();
    }

// ---------------------------------------------------------
// CTestMnGeocoder::RunL 
//
// RunL to Handle Completed Request
// ---------------------------------------------------------
//	
void CTestMnGeocoder::RunL()
	{
	iLog->Log(_L("RunL %d"),iStatus.Int());		
	CActiveScheduler::Stop();			
	}
	
// ---------------------------------------------------------
// CTestMnGeocoder::DoCancel 
//
// To Handle Cancel Request
// ---------------------------------------------------------
//	
void CTestMnGeocoder::DoCancel()
	{
	iMnGeocoder->Cancel();
	}

// ---------------------------------------------------------
// CTestMnGeocoder::RunError 
//
// To Handle Error in RunL
// ---------------------------------------------------------
//
TInt CTestMnGeocoder::RunError(TInt aError)
	{
	iLog->Log(_L("Leave in RunError %d"),aError);
	CActiveScheduler::Stop();
	return KErrNone;
	}
	
// ---------------------------------------------------------
// CTestMnGeocoder::TestNewL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMnGeocoder::TestNewL()
	{
    iLog->Log(_L("B4 Create"));
    CMnGeocoder* MnGeocoder = CMnGeocoder::NewL(*iMnProvider);
    delete MnGeocoder;
	return KErrNone;
	}
	
// ---------------------------------------------------------
// CTestMnGeocoder::TestNewLC
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMnGeocoder::TestNewChainedL()
	{
	CMnGeocoder* MnGeocoder = NULL;
    iLog->Log(_L("B4 Create"));
    TRAPD(err, MnGeocoder = CMnGeocoder::NewChainedL(*iMnProvider));
    iLog->Log(_L("TestNewChainedL Error Code - %d"),err);
    if( KErrNone == err )
   		delete MnGeocoder;
	return err;
	}
		
// ---------------------------------------------------------
// CTestMnGeocoder::TestGetSetOptionsL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMnGeocoder::TestGetSetOptionsL( CStifItemParser& aItem )
	{
	TInt opt;
	User::LeaveIfError(aItem.GetNextInt(opt));
	if( 0 == opt )
		{
		iMnGeocoder->SetOptions(CMnGeocoder::EOptionNone);
		if( CMnGeocoder::EOptionNone == iMnGeocoder->Options() )
			return KErrNone;
		else
			return KErrGeneral;
		}
	else
		{
		iMnGeocoder->SetOptions(CMnGeocoder::EOptionDialogDisabled);
		if( CMnGeocoder::EOptionDialogDisabled == iMnGeocoder->Options() )
			return KErrNone;
		else
			return KErrGeneral;
		}
	}

// ---------------------------------------------------------
// CTestMnGeocoder::TestFindCoordinateByAddressL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMnGeocoder::TestFindAddressByCoordinateL( CStifItemParser& aItem )
	{
	TPtrC city;
	TPtrC latPtr;
	TPtrC longPtr;
	User::LeaveIfError(aItem.GetNextString(city));
	User::LeaveIfError(aItem.GetNextString(latPtr));
	User::LeaveIfError(aItem.GetNextString(longPtr));
	TLex lex1( latPtr );
	TLex lex2( longPtr );
	TReal64 latitude = 12.98; 
	TReal64 longitude = 77.67;
	lex1.Val( latitude );
	lex2.Val( longitude );	
	
	TLocality position;
	position.SetCoordinate( latitude, longitude );
    iLmk->SetPositionL( position );
	
	iMnGeocoder->SetOptions(CMnGeocoder::EOptionDialogDisabled);
	iMnGeocoder->FindAddressByCoordinateL(*iLmk, iStatus);
	SetActive();
	
	CActiveScheduler::Start();

	if(iStatus.Int() != KErrNone)
		{
		iLog->Log(_L("FindAddressByCoordinateL Failed with %d"),iStatus.Int());
		return KErrGeneral;
		}

	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	
	TRAPD(err, iMnGeocoder->RetrieveGeocodingResultL(*lmk));
	
	if(err != KErrNone)
		{
		CleanupStack::PopAndDestroy(lmk);
		iLog->Log(_L("RetrieveGeocodingResultL Leave with %d"),err);
		return err;
		}
	
	TPtrC getCity;
	TInt error = lmk->GetPositionField( EPositionFieldCity, getCity );
	
	if(error != KErrNone)
		{
		CleanupStack::PopAndDestroy(lmk);
		iLog->Log(_L("GetPositionField Failed with %d"),error);
		return error;
		}
	
	if( 0 != getCity.Compare(city) )
		{
		CleanupStack::PopAndDestroy(lmk);
		iLog->Log(_L("RetrieveGeocodingResultL Failed to Match"));
		return KErrGeneral;
		}
		
	TBuf<100> msg;
	msg.Copy(_L("RetrieveGeocodingResultL City :"));
	msg.Append(getCity);	  
	iLog->Log(msg); 
	
	CleanupStack::PopAndDestroy(lmk);
	return KErrNone;
	}

// ---------------------------------------------------------
// CTestMnGeocoder::TestFindCoordinateByAddressL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMnGeocoder::TestFindCoordinateByAddressL( CStifItemParser& aItem )
	{
	TPtrC city;
	TPtrC latPtr;
	TPtrC longPtr;
	User::LeaveIfError(aItem.GetNextString(city));
	User::LeaveIfError(aItem.GetNextString(latPtr));
	User::LeaveIfError(aItem.GetNextString(longPtr));
	TLex lex1( latPtr );
	TLex lex2( longPtr );
	TReal64 latitude = 12.98; 
	TReal64 longitude = 77.67;
	lex1.Val( latitude );
	lex2.Val( longitude );	
	
    iLmk->SetPositionFieldL( EPositionFieldCity, city );
	
	iMnGeocoder->SetOptions(CMnGeocoder::EOptionDialogDisabled);
	iMnGeocoder->FindCoordinateByAddressL(*iLmk, iStatus);
	SetActive();
	
	CActiveScheduler::Start();

	if(iStatus.Int() != KErrNone)
		{
		iLog->Log(_L("FindAddressByCoordinateL Failed with %d"),iStatus.Int());
		return KErrGeneral;
		}

	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	
	TRAPD(err, iMnGeocoder->RetrieveGeocodingResultL(*lmk));
	
	if(err != KErrNone)
		{
		CleanupStack::PopAndDestroy(lmk);
		iLog->Log(_L("RetrieveGeocodingResultL Leave with %d"),err);
		return err;
		}
	
	TLocality getPosition;
	TInt error = lmk->GetPosition( getPosition );
	
	if(error != KErrNone)
		{
		CleanupStack::PopAndDestroy(lmk);
		iLog->Log(_L("GetPositionField Failed with %d"),error);
		return error;
		}

	if( getPosition.Latitude() != latitude &&
		getPosition.Longitude() != longitude )
		{
		CleanupStack::PopAndDestroy(lmk);
		iLog->Log(_L("RetrieveGeocodingResultL Failed to Match"));
		return KErrGeneral;
		}
		
	TRealFormat real;
	TBuf<100> msg;
	msg.Copy(_L("RetrieveGeocodingResultL Latitude :"));
	msg.AppendNum(getPosition.Longitude(), real);	  
	iLog->Log(msg); 
	
	CleanupStack::PopAndDestroy(lmk);
	return KErrNone;
	}

// ---------------------------------------------------------
// CTestMnGeocoder::TestFindCoordinateByAddressL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMnGeocoder::TestFindCoordinateByAddressDescL( CStifItemParser& aItem )
	{
	TPtrC city;
	TPtrC latPtr;
	TPtrC longPtr;
	User::LeaveIfError(aItem.GetNextString(city));
	User::LeaveIfError(aItem.GetNextString(latPtr));
	User::LeaveIfError(aItem.GetNextString(longPtr));
	TLex lex1( latPtr );
	TLex lex2( longPtr );
	TReal64 latitude = 12.98; 
	TReal64 longitude = 77.67;
	lex1.Val( latitude );
	lex2.Val( longitude );	
	
	iMnGeocoder->SetOptions(CMnGeocoder::EOptionDialogDisabled);
	iMnGeocoder->FindCoordinateByAddressL(city, iStatus);
	SetActive();
	
	CActiveScheduler::Start();

	if(iStatus.Int() != KErrNone)
		{
		iLog->Log(_L("FindAddressByCoordinateL Failed with %d"),iStatus.Int());
		return KErrGeneral;
		}

	CPosLandmark* lmk = CPosLandmark::NewL();
	CleanupStack::PushL( lmk );
	
	TRAPD(err, iMnGeocoder->RetrieveGeocodingResultL(*lmk));
	
	if(err != KErrNone)
		{
		CleanupStack::PopAndDestroy(lmk);
		iLog->Log(_L("RetrieveGeocodingResultL Leave with %d"),err);
		return err;
		}
	
	TLocality getPosition;
	TInt error = lmk->GetPosition( getPosition );
	
	if(error != KErrNone)
		{
		CleanupStack::PopAndDestroy(lmk);
		iLog->Log(_L("GetPositionField Failed with %d"),error);
		return error;
		}
		
	if( getPosition.Latitude() != latitude &&
		getPosition.Longitude() != longitude )
		{
		CleanupStack::PopAndDestroy(lmk);
		iLog->Log(_L("RetrieveGeocodingResultL Failed to Match"));
		return KErrGeneral;
		}
		
	iLog->Log(_L("RetrieveGeocodingResultL Matches"));
	
	CleanupStack::PopAndDestroy(lmk);
	return KErrNone;
	}

// ---------------------------------------------------------
// CTestMnGeocoder::TestSetExitObserverL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMnGeocoder::TestSetExitObserverL( )
	{
	// Set exit observer
	TRAPD(err, iMnGeocoder->SetExitObserverL( *this ));
	if(err != KErrNone)
		{
		iLog->Log(_L("TestSetExitObserverL Fails"));
		return KErrGeneral;
		}
	iLog->Log(_L("TestSetExitObserverL Passes"));
	return KErrNone;
	}	
	
// ---------------------------------------------------------
// CTestMnGeocoder::TestRemoveExitObserverL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMnGeocoder::TestRemoveExitObserverL( )
	{
	// Set exit observer
	TRAPD(err, iMnGeocoder->SetExitObserverL( *this ));
	if(err != KErrNone)
		{
		iLog->Log(_L("TestRemoveExitObserverL Fails"));
		return KErrGeneral;
		}
	iMnGeocoder->RemoveExitObserver( );
	iLog->Log(_L("TestRemoveExitObserverL Passes"));
	return KErrNone;
	}		

// ---------------------------------------------------------
// CTestMnGeocoder::HandleServerAppExit
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
void CTestMnGeocoder::HandleServerAppExit( TInt /*aReason*/ )
	{
	iLog->Log(_L("Provider exits"));
	}

// ---------------------------------------------------------
// CTestMnGeocoder::TestCancelRequestL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMnGeocoder::TestCancelRequestL( CStifItemParser& aItem )
	{
	TInt choice;
	User::LeaveIfError(aItem.GetNextInt(choice));
	
	TPtrC city;
	TPtrC latPtr;
	TPtrC longPtr;
	User::LeaveIfError(aItem.GetNextString(city));
	User::LeaveIfError(aItem.GetNextString(latPtr));
	User::LeaveIfError(aItem.GetNextString(longPtr));
	TLex lex1( latPtr );
	TLex lex2( longPtr );
	TReal64 latitude = 12.98; 
	TReal64 longitude = 77.67;
	lex1.Val( latitude );
	lex2.Val( longitude );	
	
	TLocality position;
	position.SetCoordinate( latitude, longitude );
	iMnGeocoder->SetOptions(CMnGeocoder::EOptionDialogDisabled);

	switch(choice)
		{
		case EFindAddressByCoordinateL:
		    iLmk->SetPositionL( position );
			iMnGeocoder->FindAddressByCoordinateL(*iLmk, iStatus);
		break;
		case EFindCoordinateByAddressL:
			iLmk->SetPositionFieldL( EPositionFieldCity, city );
			iMnGeocoder->FindCoordinateByAddressL(*iLmk, iStatus);
		break;
		case EFindCoordinateByAddressDescL:
			iMnGeocoder->FindCoordinateByAddressL(city, iStatus);
		break;
		}
	SetActive();
	iLog->Log(_L("Started RunSession with status %d"),iStatus.Int());
	
  	Cancel();
  	
    TBuf<100> msg;
    msg.Copy(_L("In TestCancelRequest: TestCancelRequest returned with :"));
    msg.AppendNum(iStatus.Int());
    iLog->Log(msg);
    
    if(KErrNone == iStatus.Int() || KErrCancel == iStatus.Int() )
    	return KErrCancel;
    else
    	return KErrGeneral;
	
	}

// ---------------------------------------------------------
// CTestMnGeocoder::TestRequestPanicL
//
// (other items are commented in a header).
// ---------------------------------------------------------
//
TInt CTestMnGeocoder::TestRequestPanicL( CStifItemParser& aItem )
	{
	TInt choice;
	User::LeaveIfError(aItem.GetNextInt(choice));
	
	TPtrC city;
	TPtrC latPtr;
	TPtrC longPtr;
	User::LeaveIfError(aItem.GetNextString(city));
	User::LeaveIfError(aItem.GetNextString(latPtr));
	User::LeaveIfError(aItem.GetNextString(longPtr));
	TLex lex1( latPtr );
	TLex lex2( longPtr );
	TReal64 latitude = 12.98; 
	TReal64 longitude = 77.67;
	lex1.Val( latitude );
	lex2.Val( longitude );	
	
	TLocality position;
	position.SetCoordinate( latitude, longitude );
	iMnGeocoder->SetOptions(CMnGeocoder::EOptionDialogDisabled);
	
	switch(choice)
		{
		case EFindAddressByCoordinateL:
		    iLmk->SetPositionL( position );
			iMnGeocoder->FindAddressByCoordinateL(*iLmk, iStatus);
			iMnGeocoder->FindAddressByCoordinateL(*iLmk, iStatus);
		break;
		case EFindCoordinateByAddressL:
			iLmk->SetPositionFieldL( EPositionFieldCity, city );
			iMnGeocoder->FindCoordinateByAddressL(*iLmk, iStatus);
			iMnGeocoder->FindCoordinateByAddressL(*iLmk, iStatus);
		break;
		case EFindCoordinateByAddressDescL:
			iMnGeocoder->FindCoordinateByAddressL(city, iStatus);
			iMnGeocoder->FindCoordinateByAddressL(city, iStatus);
		break;
		}
		
	return KErrNone;
	}

