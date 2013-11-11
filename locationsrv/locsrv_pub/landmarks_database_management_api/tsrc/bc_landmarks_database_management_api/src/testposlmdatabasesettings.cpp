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
* Description:   TTestPosLmDataBaseSettings class
*
*/



#include <EPos_TPosLmDatabaseSettings.h>
#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_HPosLmDatabaseInfo.h>
#include <EPos_CPosLandmarkDatabase.h>
#include "testposlmdatabasesettings.h"

// DESCRIPTION
// This module contains implementation of TTestPosLmDataBaseSettings 
// class

//
// Constructor.
//
TTestPosLmDataBaseSettings::TTestPosLmDataBaseSettings()
	{
				
	}

//
// This function will test the constructor of TPosLmDatabaseSettings class.
//
void TTestPosLmDataBaseSettings::TestTPosLmDatabaseSettingsL()
	{
	TPosLmDatabaseSettings databaseSettings;
	}
	
//
// This function will test the IsAttributeSet function of TPosLmDatabaseSettings class.
//
void TTestPosLmDataBaseSettings::TestIsAttributeSetL(TInt aValue)
	{
	
	if(!aValue)
		{
		TPosLmDatabaseSettings databaseSettings;
		if(databaseSettings.IsAttributeSet(TPosLmDatabaseSettings::EName))
			{
			User::Leave(KErrGeneral);
			}
		}
	else
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
		
		if(!databaseSettings.IsAttributeSet(TPosLmDatabaseSettings::EName))
			{
			User::Leave(KErrGeneral);
			}
		
		CleanupStack::PopAndDestroy(databasemanager);
		CleanupStack::PopAndDestroy(databaseInfo);
		ReleaseLandmarkResources();		
		}		
	}
	
//
// This function will test the UnsetAttribute function of TPosLmDatabaseSettings class.
//
void TTestPosLmDataBaseSettings::TestUnsetAttributeL()
	{
	_LIT(KDatabaseUri,"file://c:eposlm.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	
	HPosLmDatabaseInfo* databaseInfo=HPosLmDatabaseInfo::NewLC(databaseUri);
	TPosLmDatabaseSettings databaseSettings=databaseInfo->Settings();
		
	_LIT(KDatabaseName,"Nokia");
	TPosLmDatabaseName databaseName(KDatabaseName);
	databaseSettings.SetDatabaseName(databaseName);
	databaseSettings.UnsetAttribute(TPosLmDatabaseSettings::EName);
	
	CPosLmDatabaseManager* databasemanager= CPosLmDatabaseManager::NewL();
	CleanupStack::PushL(databasemanager);
		
	databasemanager->ModifyDatabaseSettingsL(databaseUri,databaseSettings);
		
	if(databaseSettings.IsAttributeSet(TPosLmDatabaseSettings::EName))
		{
		User::Leave(KErrGeneral);
		}
		
	CleanupStack::PopAndDestroy(databasemanager);
	CleanupStack::PopAndDestroy(databaseInfo);
	ReleaseLandmarkResources();		
	}
	
//
// This function will test the SetDatabaseName function of TPosLmDatabaseSettings class.
//
void TTestPosLmDataBaseSettings::TestSetDatabaseNameL()
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
// This function will test the DatabaseName function of TPosLmDatabaseSettings class.
//
void TTestPosLmDataBaseSettings::TestDatabaseNameL()
	{
	TPosLmDatabaseSettings databaseSettings;
	TPtrC databaseName=databaseSettings.DatabaseName();
	if(databaseName.Length())
		{
		User::Leave(KErrGeneral);
		}
	}
	
		