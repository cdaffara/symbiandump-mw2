/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   CTestPosLmDatabaseInfo class
*
*/



#include <EPos_HPosLmDatabaseInfo.h>
#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_TPosLmDatabaseSettings.h>
#include <EPos_CPosLandmarkDatabase.h>
#include "testposlmdatabaseinfo.h"


// DESCRIPTION
// This module contains implementation of CTestPosLmDatabaseInfo 
// class

//
// Constructor.
//
CTestPosLmDatabaseInfo::CTestPosLmDatabaseInfo ()
	{
				
	}


//
// Destructor.
//

CTestPosLmDatabaseInfo ::~CTestPosLmDatabaseInfo()
	{
	
	}

//
// Two phase constructor.
//
CTestPosLmDatabaseInfo* CTestPosLmDatabaseInfo::NewLC()
	{
	CTestPosLmDatabaseInfo* testDatabaseInfo=new(ELeave) CTestPosLmDatabaseInfo;
	CleanupStack::PushL(testDatabaseInfo);
	return testDatabaseInfo;
	}

//
// This function will test NewLC method of HPosLmDatabaseInfo class.
//
void CTestPosLmDatabaseInfo::TestNewLC1L()
	{
	_LIT(KDatabaseUri,"file://c:myLandmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(databaseUri);
	
	CleanupStack::PopAndDestroy(databaseInfo);
	}
	
//
// This function will test overloaded NewLC method of HPosLmDatabaseInfo class.
//
void CTestPosLmDatabaseInfo::TestNewLC2L()
	{
	_LIT(KDatabaseUri,"file://c:myLandmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo1=HPosLmDatabaseInfo::NewL(databaseUri);
	CleanupStack::PushL(databaseInfo1);
	
	HPosLmDatabaseInfo* databaseInfo2=HPosLmDatabaseInfo::NewLC(*databaseInfo1);
	
	CleanupStack::PopAndDestroy(databaseInfo2);
	CleanupStack::PopAndDestroy(databaseInfo1);
	}

//
// This function will test NewL method of HPosLmDatabaseInfo class.
//
void CTestPosLmDatabaseInfo::TestNewL1L()
	{
	_LIT(KDatabaseUri,"file://c:myLandmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewL(databaseUri);
	
	delete databaseInfo;
	}

//
// This function will test overloaded NewL method of HPosLmDatabaseInfo class.
//
void CTestPosLmDatabaseInfo::TestNewL2L()
	{
	_LIT(KDatabaseUri,"file://c:myLandmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo1=HPosLmDatabaseInfo::NewL(databaseUri);
	CleanupStack::PushL(databaseInfo1);
	
	HPosLmDatabaseInfo* databaseInfo2=HPosLmDatabaseInfo::NewL(*databaseInfo1);
	
	delete databaseInfo2;
	CleanupStack::PopAndDestroy(databaseInfo1);
	}

//
// This function will test DatabaseUri method of HPosLmDatabaseInfo class.
//
void CTestPosLmDatabaseInfo::TestDatabaseUriL()
	{
	_LIT(KDatabaseUri,"file://c:myLandmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewL(databaseUri);
	CleanupStack::PushL(databaseInfo);
	
	TPtrC databaseUriPtr=databaseInfo->DatabaseUri();
	if(databaseUriPtr.Compare(KDatabaseUri))
		{
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(databaseInfo);
	}

//
// This function will test Protocol method of HPosLmDatabaseInfo class.
//
void CTestPosLmDatabaseInfo::TestProtocol1L()
	{
	_LIT(KDatabaseUri,"file://c:myLandmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewL(databaseUri);
	CleanupStack::PushL(databaseInfo);
	
	TPtrC protocolPtr=databaseInfo->Protocol();
	for(TInt i=0;i<4;i++)
		{
		if(databaseUri[i]!=protocolPtr[i])
			{
			User::Leave(KErrGeneral);
			}
		}
		
	CleanupStack::PopAndDestroy(databaseInfo);
	}
	
//
// This function will test Protocol method of HPosLmDatabaseInfo class.
// Protocol function must return default protocol when it is not specified.
//
void CTestPosLmDatabaseInfo::TestProtocol2L()
	{
	_LIT(KDatabaseUri,"://c:myLandmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewL(databaseUri);
	CleanupStack::PushL(databaseInfo);
	
	TPtrC protocolPtr=databaseInfo->Protocol();
	if(protocolPtr.Length())
		{
		User::Leave(KErrGeneral);
		}
		
	CleanupStack::PopAndDestroy(databaseInfo);
	}
	
//
// This function will test IsDefault method of HPosLmDatabaseInfo class.
//
void CTestPosLmDatabaseInfo::TestIsDefaultL()
	{
	_LIT(KDatabaseUri,"file://c:myLandmarks1.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewL(databaseUri);
	CleanupStack::PushL(databaseInfo);
	
	if(databaseInfo->IsDefault())
		{
		User::Leave(KErrGeneral);
		}
		
	CleanupStack::PopAndDestroy(databaseInfo);
	}


//
// This function will test DatabaseMedia method of HPosLmDatabaseInfo class.
//
void CTestPosLmDatabaseInfo::TestDatabaseMediaL()
	{
	_LIT(KDatabaseUri,"file://c:myLandmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewL(databaseUri);
	CleanupStack::PushL(databaseInfo);
	
	if(databaseInfo->DatabaseMedia()!=EMediaUnknown)
		{
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(databaseInfo);
	}
	
	
//
// This function will test DatabaseDrive method of HPosLmDatabaseInfo class.
//
void CTestPosLmDatabaseInfo::TestDatabaseDriveL()
	{
	_LIT(KDatabaseUri,"file://c:myLandmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewL(databaseUri);
	CleanupStack::PushL(databaseInfo);
	
	if(databaseInfo->DatabaseDrive())
		{
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(databaseInfo);
	}
		
//
// This function will test the Settings function of HPosLmDatabaseInfo class.
// This function returns constant reference.
//
void CTestPosLmDatabaseInfo::TestSettings1L()
	{
	_LIT(KDatabaseUri,"file://c:eposlm.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	const HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(databaseUri);
    TPosLmDatabaseSettings databaseSettings=databaseInfo->Settings();
		
	_LIT(KDatabaseName,"Nokia");
	TPosLmDatabaseName databaseName(KDatabaseName);
	databaseSettings.SetDatabaseName(databaseName);
	
	CPosLmDatabaseManager* databasemanager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databasemanager);
		
	databasemanager->ModifyDatabaseSettingsL(databaseUri,databaseSettings);
		
	TPtrC databaseNamePtr=databaseSettings.DatabaseName();
	if(databaseNamePtr.Compare(databaseName))	
		{
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(databasemanager);
	CleanupStack::PopAndDestroy();//databaseInfo
	ReleaseLandmarkResources();		
	}	
	
//
// This function will test the Settings function of HPosLmDatabaseInfo class.
// This function returns constant reference.
//
void CTestPosLmDatabaseInfo::TestSettings2L()
	{
	_LIT(KDatabaseUri,"file://c:eposlm.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(databaseUri);
    TPosLmDatabaseSettings databaseSettings=databaseInfo->Settings();
		
	_LIT(KDatabaseName,"Nokia");
	TPosLmDatabaseName databaseName(KDatabaseName);
	databaseSettings.SetDatabaseName(databaseName);
	
	CPosLmDatabaseManager* databasemanager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databasemanager);
		
	databasemanager->ModifyDatabaseSettingsL(databaseUri,databaseSettings);
		
	TPtrC databaseNamePtr=databaseSettings.DatabaseName();
	if(databaseNamePtr.Compare(databaseName))	
		{
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(databasemanager);
	CleanupStack::PopAndDestroy(databaseInfo);
	ReleaseLandmarkResources();		
	}
	
//
// This function will test Size method of HPosLmDatabaseInfo class.
//
void CTestPosLmDatabaseInfo::TestSizeL()
	{
	_LIT(KDatabaseUri,"file://c:myLandmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(databaseUri);
	TInt size=databaseInfo->Size();
	
	CleanupStack::PopAndDestroy(databaseInfo);
	}
		