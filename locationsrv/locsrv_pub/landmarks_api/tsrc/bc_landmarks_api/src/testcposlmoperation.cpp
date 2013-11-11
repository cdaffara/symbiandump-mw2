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
* Description:  provides testing of CPosLmOperation class methods
*
*/



// INCLUDE FILES
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>
#include <LbsPosition.h>
#include "testcposlmoperation.h"

// Literals
// Test Database URI
_LIT( KDBUri, "file://c:eposlmtest.ldb" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestPosLmOperation::CTestPosLmOperation
// 
// 
// -----------------------------------------------------------------------------
//
CTestPosLmOperation::CTestPosLmOperation( CStifLogger* aLog ) :
					CActive(EPriorityStandard), iLog(aLog)
	{
	CActiveScheduler::Add(this);
	}

// -----------------------------------------------------------------------------
// CTestPosLmOperation::NewL
// 
// 
// -----------------------------------------------------------------------------
//	
CTestPosLmOperation* CTestPosLmOperation::NewL( CStifLogger* aLog )
	{
	CTestPosLmOperation* self = new (ELeave) CTestPosLmOperation( aLog );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); 
    return self; 
	}	

// -----------------------------------------------------------------------------
// CTestPosLmOperation::ConstructL
// 
// 
// -----------------------------------------------------------------------------
//
void CTestPosLmOperation::ConstructL()
	{
	
	}

// -----------------------------------------------------------------------------
// CTestPosLmOperation::~CTestPosLmOperation
// 
// 
// -----------------------------------------------------------------------------
//
CTestPosLmOperation::~CTestPosLmOperation()
	{
	
	Cancel();
	
	if( iOperation )
		{
		delete iOperation;
		}
	if( iDatabase )
		{
		delete iDatabase;
		ReleaseLandmarkResources();
		}
	}
	


// -----------------------------------------------------------------------------
// CTestPosLmOperation::DestructOperation
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmOperation::DestructOperationL( CStifItemParser& /*aItem*/ )
	{
	// 
	// Open default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Call Init to Get one operation object which will initializa the DB
	CPosLmOperation* operation = lmkDatabase->InitializeL();
	CleanupStack::PushL( operation );
	// Delete operation object, will invoke the destructor
	CleanupStack::PopAndDestroy( 2, lmkDatabase ); // operation
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLmOperation::Execute
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmOperation::ExecuteL( CStifItemParser& /*aItem*/ )
	{
	// 
	// Open default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Init database, Get one operation object
	CPosLmOperation* operation = lmkDatabase->InitializeL();
	CleanupStack::PushL( operation );
	// Execute the operation
	operation->ExecuteL();
	// Delete operation object
	CleanupStack::PopAndDestroy( 2, lmkDatabase ); // operation
	ReleaseLandmarkResources();
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTestPosLmOperation::ExecuteAndDeleteOperation
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmOperation::ExecuteAndDeleteOperationL( CStifItemParser& /*aItem*/ )
	{
	// 
	// Open default database
	CPosLandmarkDatabase* lmkDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	CleanupStack::PushL( lmkDatabase );
	// Init database, Get one operation object, execute and delete it
	ExecuteAndDeleteLD( lmkDatabase->InitializeL() );
	// Delete DB
	CleanupStack::PopAndDestroy( lmkDatabase ); 
	ReleaseLandmarkResources();
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CTestPosLmOperation::NextStep
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestPosLmOperation::NextStepL( CStifItemParser& /*aItem*/ )
	{
	// 
	// Open default database
	iDatabase = CPosLandmarkDatabase::OpenL( KDBUri );
	// Init database, Get one operation object
	iOperation = iDatabase->InitializeL();
	// Execute the operation Asynchronously
	iOperation->NextStep( iStatus, iProgress );
	SetActive();
	// Wait
	CActiveScheduler::Start();
	return iLastResult;
	}	

// -----------------------------------------------------------------------------
// CTestPosLmOperation::RunL
// 
// 
// -----------------------------------------------------------------------------
//
void CTestPosLmOperation::RunL()
	{
	//
	iLog->Log(_L("RunL Starts..."));
	if( iStatus == KPosLmOperationNotComplete )
		{
		//
		iLog->Log(_L("RunL Before Next"));
		iOperation->NextStep( iStatus, iProgress );
		SetActive();
		iLog->Log(_L("RunL After Next "));
		}
	else
		{
		if( iStatus == KErrNone )
			{
			iLog->Log(_L("NextStep Successful "));
			iLastResult = KErrNone;
			}
		else
			{
			iLog->Log(_L("NextStep failed "));
			iLastResult = KErrGeneral;
			}	
		CActiveScheduler::Stop();	
		}
	}

// -----------------------------------------------------------------------------
// CTestPosLmOperation::DoCancel
// 
// 
// -----------------------------------------------------------------------------
//
void CTestPosLmOperation::DoCancel()
	{
	
	}	
