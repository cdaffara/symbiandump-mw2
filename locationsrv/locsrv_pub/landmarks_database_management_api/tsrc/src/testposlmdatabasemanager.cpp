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
* Description:   CTestPosLmDatabaseManager class
*
*/



#include <EPos_Landmarks.h>
#include <EPos_CPosLandmarkDatabase.h>
#include "EPos_HPosLmDatabaseInfo.h"
#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_TPosLmDatabaseEvent.h>
#include "testposlmdatabasemanager.h"



// DESCRIPTION
// This module contains implementation of CTestDatabaseManager 
// class

//
// Constructor.
//
CTestPosLmDatabaseManager::CTestPosLmDatabaseManager():CActive(EPriorityStandard)
	{
	CActiveScheduler::Add(this);			
	}


//
// Destructor.
//

CTestPosLmDatabaseManager ::~CTestPosLmDatabaseManager()
	{
	Cancel();
	ReleaseLandmarkResources();	
	}

//
// Two phase constructor.
//
CTestPosLmDatabaseManager* CTestPosLmDatabaseManager::NewLC()
	{
	CTestPosLmDatabaseManager* lmkdatabasemanager=new(ELeave) CTestPosLmDatabaseManager;
	CleanupStack::PushL(lmkdatabasemanager);
	return lmkdatabasemanager;
	}
	
//
// RunL implementation.
//
void CTestPosLmDatabaseManager::RunL()
	{
	CActiveScheduler::Stop();
	}
//
// DoCancel
//
void CTestPosLmDatabaseManager::DoCancel()
	{
	
	}

//
// This function will test NewL method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestNewL()
	{
	CPosLmDatabaseManager* databasemanager= CPosLmDatabaseManager::NewL();
	delete databasemanager;
	}

	
//
// This function will test DefaultDatabaseUriLC method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestDefaultDatabaseUriLC()
	{
	CPosLmDatabaseManager* databaseManager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databaseManager);
		
	CPosLandmarkDatabase* landmarkDatabase=CPosLandmarkDatabase::OpenL();
	CleanupStack::PushL(landmarkDatabase);
	
	HBufC* defaultUri=databaseManager->DefaultDatabaseUriLC();
	TInt result=defaultUri->Compare(*(static_cast<TDesC*>(landmarkDatabase->DatabaseUriLC())));
	
	if(result!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::Pop(); 			
	CleanupStack::PopAndDestroy(defaultUri);
	CleanupStack::PopAndDestroy(landmarkDatabase);
	CleanupStack::PopAndDestroy(databaseManager);
	}

//
// This function will test SetDefaultDatabaseUriL method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestSetDefaultDatabaseUriL()
	{
	CPosLmDatabaseManager* databaseManager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databaseManager);
	
	
	_LIT(KDatabaseUri,"file://c:eposlm.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	databaseManager->SetDefaultDatabaseUriL(databaseUri);
		
	CPosLandmarkDatabase* landmarkDatabase=CPosLandmarkDatabase::OpenL();
	CleanupStack::PushL(landmarkDatabase);

	HBufC* defaultUri=databaseManager->DefaultDatabaseUriLC();
	TInt result=defaultUri->Compare(databaseUri);
	
	if(result!=0)
		{
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(defaultUri);
	CleanupStack::PopAndDestroy(landmarkDatabase);
	CleanupStack::PopAndDestroy(databaseManager);
	}
	

//
// This function will test DatabaseUriFromLastEventLC method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestDatabaseUriFromLastEventLC()
	{
	CPosLmDatabaseManager* databaseManager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databaseManager);
	
	HBufC* lastEventUri=NULL;
	CleanupStack::PushL(lastEventUri);
	lastEventUri=databaseManager->DatabaseUriFromLastEventLC();
	
	CleanupStack::PopAndDestroy(lastEventUri);
	CleanupStack::PopAndDestroy(databaseManager);
	}
	
//
// This function will test NotifyDatabaseEvent method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestNotifyDatabaseEventL()
	{
	CPosLmDatabaseManager* databaseManager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databaseManager);
	
	TPosLmDatabaseEvent databaseEvent;
	databaseManager->NotifyDatabaseEvent(databaseEvent,iStatus);
	
	_LIT(KDatabaseUri,"file://c:eposlm.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(KDatabaseUri);
	
	databaseManager->DeleteDatabaseL(databaseUri);
	databaseManager->CreateDatabaseL(*databaseInfo);
	
	RPointerArray<HPosLmDatabaseInfo> databaseList;
	
	databaseManager->ListDatabasesL(databaseList);

	
	SetActive();
	CActiveScheduler::Start();
		
	CleanupStack::PopAndDestroy(databaseInfo);
	CleanupStack::PopAndDestroy(databaseManager);
	}
	
//
// This function will test CancelNotifyDatabaseEvent method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestCancelNotifyDatabaseEventL()
	{
	CPosLmDatabaseManager* databaseManager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databaseManager);
	
	TPosLmDatabaseEvent databaseEvent;
	databaseManager->NotifyDatabaseEvent(databaseEvent,iStatus);
	databaseManager->CancelNotifyDatabaseEvent();
	
	SetActive();
	CActiveScheduler::Start();
		
	CleanupStack::PopAndDestroy(databaseManager);
	}

//
// This function will test ListDatabasesLC method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestListDatabasesLC()
	{
	CPosLmDatabaseManager* databaseManager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databaseManager);

	_LIT(KDatabaseUri,"file://c:eposlm.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(KDatabaseUri);
	
	databaseManager->DeleteDatabaseL(databaseUri);
	databaseManager->CreateDatabaseL(*databaseInfo);
	
	CDesCArray* databaseList=databaseManager->ListDatabasesLC();
	TInt count=databaseList->Count();
	
	delete databaseList;
	
	CleanupStack::Pop();
	CleanupStack::PopAndDestroy(databaseInfo);
	CleanupStack::PopAndDestroy(databaseManager);
	}
//
// This function will test ListDatabasesL method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestListDatabasesL()
	{
	CPosLmDatabaseManager* databaseManager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databaseManager);

	_LIT(KDatabaseUri,"file://c:eposlm.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(KDatabaseUri);
	
	databaseManager->DeleteDatabaseL(databaseUri);
	databaseManager->CreateDatabaseL(*databaseInfo);
	
	RPointerArray<HPosLmDatabaseInfo> databaseList;
	
	databaseManager->ListDatabasesL(databaseList);
	TInt count=databaseList.Count();
	
	CleanupStack::PopAndDestroy(databaseInfo);
	CleanupStack::PopAndDestroy(databaseManager);
	}
	
//
// This function will test RegisterDatabaseL method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestRegisterDatabaseL()
	{
	CPosLmDatabaseManager* databaseManager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databaseManager);

	_LIT(KDatabaseUri,"file://c:eposlm.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(KDatabaseUri);
	
	databaseManager->RegisterDatabaseL(*databaseInfo);
		
	CleanupStack::PopAndDestroy(databaseInfo);
	CleanupStack::PopAndDestroy(databaseManager);
	}
		
//
// This function will test UnregisterDatabaseL method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestUnregisterDatabaseL()
	{
	CPosLmDatabaseManager* databaseManager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databaseManager);

	_LIT(KDatabaseUri,"file://c:eposlm.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(KDatabaseUri);
	
	databaseManager->UnregisterDatabaseL(databaseUri);
		
	CleanupStack::PopAndDestroy(databaseInfo);
	CleanupStack::PopAndDestroy(databaseManager);
	}	
	
//
// This function will test UnregisterAllDatabasesL method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestUnregisterAllDatabasesL()
	{
	CPosLmDatabaseManager* databaseManager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databaseManager);

	_LIT(KDatabaseUri,"file://c:eposlm.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(KDatabaseUri);
	
	_LIT(KProtocol,"file");
	TBufC<10> protocol(KProtocol);
	
	databaseManager->UnregisterAllDatabasesL(KProtocol);
		
	CleanupStack::PopAndDestroy(databaseInfo);
	CleanupStack::PopAndDestroy(databaseManager);
	}	
	

//
// This function will test ModifyDatabaseSettingsL method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestModifyDatabaseSettingsL()
	{
	CPosLmDatabaseManager* databaseManager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databaseManager);
	
	_LIT(KDatabaseUri,"file://c:eposlm.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(databaseUri);
	TPosLmDatabaseSettings databaseSettings=databaseInfo->Settings();
		
	_LIT(KDatabaseName,"Nokia");
	TPosLmDatabaseName databaseName(KDatabaseName);
	databaseSettings.SetDatabaseName(databaseName);
	
	databaseManager->ModifyDatabaseSettingsL(databaseUri,databaseSettings);
		
	TPtrC databaseNamePtr=databaseSettings.DatabaseName();
	if(databaseNamePtr.Compare(databaseName))	
		{
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(databaseInfo);
	CleanupStack::PopAndDestroy(databaseManager);
	}

	
//
// This function will test GetDatabaseInfoL method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestGetDatabaseInfoL()
	{
	CPosLmDatabaseManager* databaseManager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databaseManager);
	
	_LIT(KDatabaseUri,"file://c:eposlm.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(databaseUri);
	
	databaseManager->GetDatabaseInfoL(*databaseInfo);
	
	TChar databaseDrive=databaseInfo->DatabaseDrive();
	if(databaseDrive!='C')
		{
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(databaseInfo);
	CleanupStack::PopAndDestroy(databaseManager);
	}

//
// This function will test DatabaseExistsL method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestDatabaseExistsL()
	{
	CPosLmDatabaseManager* databaseManager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databaseManager);
	
	_LIT(KDatabaseUri,"file://c:eposlm.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(databaseUri);
	
	TBool databaseExists=databaseManager->DatabaseExistsL(databaseUri);
	
	if(!databaseExists)
		{
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(databaseInfo);
	CleanupStack::PopAndDestroy(databaseManager);
	}
	

//
// This function will test CreateDatabaseL method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestCreateDatabaseL()
	{
	CPosLmDatabaseManager* databaseManager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databaseManager);
	
	_LIT(KDatabaseUri,"file://c:myLandmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(databaseUri);
	
	databaseManager->DeleteDatabaseL(databaseUri);
	databaseManager->CreateDatabaseL(*databaseInfo);
	
	if(!(databaseManager->DatabaseExistsL(databaseUri)))
		{
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(databaseInfo);
	CleanupStack::PopAndDestroy(databaseManager);
	}
	
//
// This function will test DeleteDatabaseL method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestDeleteDatabaseL()
	{
	CPosLmDatabaseManager* databaseManager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databaseManager);
	
	_LIT(KDatabaseUri,"file://c:myLandmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(databaseUri);
	
	if(!databaseManager->DatabaseExistsL(databaseUri))
		{
		databaseManager->CreateDatabaseL(*databaseInfo);	
		}
	
	databaseManager->DeleteDatabaseL(databaseUri);
	
	if(databaseManager->DatabaseExistsL(databaseUri))
		{
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(databaseInfo);
	CleanupStack::PopAndDestroy(databaseManager);
	}

//
// This function will test CopyDatabaseL method of CPosLmDatabaseManager class.
//
void CTestPosLmDatabaseManager::TestCopyDatabaseL()
	{
	CPosLmDatabaseManager* databaseManager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databaseManager);
	
	_LIT(KDatabaseUri1,"file://c:myLandmarks.ldb");
	TBufC<40> databaseUri1(KDatabaseUri1);
	
	_LIT(KDatabaseUri2,"file://c:eposlm.ldb");
	TBufC<40> databaseUri2(KDatabaseUri2);

	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(databaseUri1);
	
	databaseManager->DeleteDatabaseL(databaseUri1);
	databaseManager->CopyDatabaseL(databaseUri2,databaseUri1);
		
	CleanupStack::PopAndDestroy(databaseInfo);
	CleanupStack::PopAndDestroy(databaseManager);
	}
