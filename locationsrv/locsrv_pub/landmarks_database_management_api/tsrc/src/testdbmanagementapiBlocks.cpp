/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Entry point for test cases in stif module
*
*/




// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "testdbmanagementapi.h"
#include "testposlmdatabasemanager.h"
#include "testposlmdatabaseinfo.h"
#include "testposlmdatabasesettings.h"




// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Ctestdbmanagementapi::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Ctestdbmanagementapi::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        { 
        ENTRY("CPosLmDatabaseManager_NewL",Ctestdbmanagementapi::CPosLmDatabaseManager_NewL),
        ENTRY("CPosLmDatabaseManager_DefaultDatabaseUriLC",Ctestdbmanagementapi::CPosLmDatabaseManager_DefaultDatabaseUriLC),
        ENTRY("CPosLmDatabaseManager_SetDefaultDatabaseUriL",Ctestdbmanagementapi::CPosLmDatabaseManager_SetDefaultDatabaseUriL),
        ENTRY("CPosLmDatabaseManager_NotifyDatabaseEvent",Ctestdbmanagementapi::CPosLmDatabaseManager_NotifyDatabaseEventL),
        ENTRY("CPosLmDatabaseManager_CancelNotifyDatabaseEvent",Ctestdbmanagementapi::CPosLmDatabaseManager_CancelNotifyDatabaseEventL),
        ENTRY("CPosLmDatabaseManager_DatabaseUriFromLastEventLC",Ctestdbmanagementapi::CPosLmDatabaseManager_DatabaseUriFromLastEventLC),
        ENTRY("CPosLmDatabaseManager_ListDatabasesLC",Ctestdbmanagementapi::CPosLmDatabaseManager_ListDatabasesLC),
        ENTRY("CPosLmDatabaseManager_ListDatabasesL",Ctestdbmanagementapi::CPosLmDatabaseManager_ListDatabasesL),
        ENTRY("CPosLmDatabaseManager_RegisterDatabaseL",Ctestdbmanagementapi::CPosLmDatabaseManager_RegisterDatabaseL),
        ENTRY("CPosLmDatabaseManager_UnregisterDatabaseL",Ctestdbmanagementapi::CPosLmDatabaseManager_UnregisterDatabaseL),
        ENTRY("CPosLmDatabaseManager_UnregisterAllDatabasesL",Ctestdbmanagementapi::CPosLmDatabaseManager_UnregisterAllDatabasesL),
        ENTRY("CPosLmDatabaseManager_ModifyDatabaseSettingsL",Ctestdbmanagementapi::CPosLmDatabaseManager_ModifyDatabaseSettingsL),
        ENTRY("CPosLmDatabaseManager_GetDatabaseInfoL",Ctestdbmanagementapi::CPosLmDatabaseManager_GetDatabaseInfoL),
        ENTRY("CPosLmDatabaseManager_DatabaseExistsL",Ctestdbmanagementapi::CPosLmDatabaseManager_DatabaseExistsL),
        ENTRY("CPosLmDatabaseManager_CreateDatabaseL",Ctestdbmanagementapi::CPosLmDatabaseManager_CreateDatabaseL),
        ENTRY("CPosLmDatabaseManager_DeleteDatabaseL",Ctestdbmanagementapi::CPosLmDatabaseManager_DeleteDatabaseL),
        ENTRY("CPosLmDatabaseManager_CopyDatabaseL",Ctestdbmanagementapi::CPosLmDatabaseManager_CopyDatabaseL),
        ENTRY("HPosLmDatabaseInfo_NewLC1",Ctestdbmanagementapi::HPosLmDatabaseInfo_NewLC1L),
        ENTRY("HPosLmDatabaseInfo_NewLC2",Ctestdbmanagementapi::HPosLmDatabaseInfo_NewLC2L),
        ENTRY("HPosLmDatabaseInfo_NewL1",Ctestdbmanagementapi::HPosLmDatabaseInfo_NewL1L),
        ENTRY("HPosLmDatabaseInfo_NewL2",Ctestdbmanagementapi::HPosLmDatabaseInfo_NewL2L),
        ENTRY("HPosLmDatabaseInfo_DatabaseUri",Ctestdbmanagementapi::HPosLmDatabaseInfo_DatabaseUriL),
        ENTRY("HPosLmDatabaseInfo_Protocol1",Ctestdbmanagementapi::HPosLmDatabaseInfo_Protocol1L),
        ENTRY("HPosLmDatabaseInfo_Protocol2",Ctestdbmanagementapi::HPosLmDatabaseInfo_Protocol2L),
        ENTRY("HPosLmDatabaseInfo_IsDefault",Ctestdbmanagementapi::HPosLmDatabaseInfo_IsDefaultL),
        ENTRY("HPosLmDatabaseInfo_DatabaseMedia",Ctestdbmanagementapi::HPosLmDatabaseInfo_DatabaseMediaL),
        ENTRY("HPosLmDatabaseInfo_DatabaseDrive",Ctestdbmanagementapi::HPosLmDatabaseInfo_DatabaseDriveL),
        ENTRY("HPosLmDatabaseInfo_Settings1",Ctestdbmanagementapi::HPosLmDatabaseInfo_Settings1L),
        ENTRY("HPosLmDatabaseInfo_Settings2",Ctestdbmanagementapi::HPosLmDatabaseInfo_Settings2L),
        ENTRY("HPosLmDatabaseInfo_Size",Ctestdbmanagementapi::HPosLmDatabaseInfo_SizeL),
        ENTRY("TPosLmDatabaseSettings_TPosLmDatabaseSettings",Ctestdbmanagementapi::TPosLmDatabaseSettings_TPosLmDatabaseSettingsL),
        ENTRY("TPosLmDatabaseSettings_IsAttributeSet",Ctestdbmanagementapi::TPosLmDatabaseSettings_IsAttributeSetL),
        ENTRY("TPosLmDatabaseSettings_UnsetAttribute",Ctestdbmanagementapi::TPosLmDatabaseSettings_UnsetAttributeL),
        ENTRY("TPosLmDatabaseSettings_SetDatabaseName",Ctestdbmanagementapi::TPosLmDatabaseSettings_SetDatabaseNameL),
        ENTRY("TPosLmDatabaseSettings_DatabaseName",Ctestdbmanagementapi::TPosLmDatabaseSettings_DatabaseNameL),
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    TInt ret = KErrNone;
	TRAPD( error, ret = RunInternalL( KFunctions, count, aItem ));
	if ( error )
		{
		return error;
		}
    return ret;

    }



// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_NewL
// This test case is used to test NewL of function CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_NewL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestNewL();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_DefaultDatabaseUriLC
// This test case is used to test DefaultDatabaseUriLC function of CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_DefaultDatabaseUriLC(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestDefaultDatabaseUriLC();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
		
	}
	
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_SetDefaultDatabaseUriL
// This test case is used to test SetDefaultDatabaseUriL function of CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_SetDefaultDatabaseUriL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestSetDefaultDatabaseUriL();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_NotifyDatabaseEventL
// This test case is used to test NotifyDatabaseEvent function of CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_NotifyDatabaseEventL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestNotifyDatabaseEventL();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_CancelNotifyDatabaseEventL
// This test case is used to test CancelNotifyDatabaseEvent function of CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_CancelNotifyDatabaseEventL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestCancelNotifyDatabaseEventL();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
	}		
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_DatabaseUriFromLastEventLC
// This test case is used to test DatabaseUriFromLastEventLC function of CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_DatabaseUriFromLastEventLC(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestDatabaseUriFromLastEventLC();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
	}
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_ListDatabasesLC
// This test case is used to test ListDatabasesLC function of CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_ListDatabasesLC(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestListDatabasesLC();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_ListDatabasesL
// This test case is used to test ListDatabasesL function of CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_ListDatabasesL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestListDatabasesL();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_RegisterDatabaseL
// This test case is used to test RegisterDatabaseL function of CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_RegisterDatabaseL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestRegisterDatabaseL();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_UnregisterDatabaseL
// This test case is used to test UnregisterDatabaseL function of CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_UnregisterDatabaseL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestUnregisterDatabaseL();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_UnregisterAllDatabasesL
// This test case is used to test UnregisterAllDatabasesL function of CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_UnregisterAllDatabasesL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestUnregisterAllDatabasesL();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
	}	
	
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_ModifyDatabaseSettingsL
// This test case is used to test ModifyDatabaseSettingsL function of CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_ModifyDatabaseSettingsL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestModifyDatabaseSettingsL();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
	}	
	
	
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_GetDatabaseInfoL
// This test case is used to test GetDatabaseInfoL function of CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_GetDatabaseInfoL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestGetDatabaseInfoL();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
	}	
	
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_DatabaseExistsL
// This test case is used to test DatabaseExistsL function of CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_DatabaseExistsL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestDatabaseExistsL();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
	}	
	
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_CreateDatabaseL
// This test case is used to test CreateDatabaseL function of CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_CreateDatabaseL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestCreateDatabaseL();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_DeleteDatabaseL
// This test case is used to test DeleteDatabaseL function of CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_DeleteDatabaseL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestDeleteDatabaseL();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
	}		
	
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::CPosLmDatabaseManager_CopyDatabaseL
// This test case is used to test CopyDatabaseL function of CPosLmDatabaseManager class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::CPosLmDatabaseManager_CopyDatabaseL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseManager* manager=CTestPosLmDatabaseManager::NewLC();
	manager->TestCopyDatabaseL();
	CleanupStack::PopAndDestroy(manager);
	return KErrNone;
	}		
	
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::HPosLmDatabaseInfo_NewLC
// This test case is used to test NewLC function of HPosLmDatabaseInfo class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::HPosLmDatabaseInfo_NewLC1L(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseInfo* testDatabaseInfo=CTestPosLmDatabaseInfo::NewLC();
	testDatabaseInfo->TestNewLC1L	();
	CleanupStack::PopAndDestroy(testDatabaseInfo);
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::HPosLmDatabaseInfo_NewLC
// This test case is used to test over loaded NewLC function of HPosLmDatabaseInfo class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::HPosLmDatabaseInfo_NewLC2L(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseInfo* testDatabaseInfo=CTestPosLmDatabaseInfo::NewLC();
	testDatabaseInfo->TestNewLC2L();
	CleanupStack::PopAndDestroy(testDatabaseInfo);
	return KErrNone;	
	}
	
	
//-----------------------------------------------------------------------------
// Ctestdbmanagementapi::HPosLmDatabaseInfo_NewL
// This test case is used to test NewL function of HPosLmDatabaseInfo class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::HPosLmDatabaseInfo_NewL1L(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseInfo* testDatabaseInfo=CTestPosLmDatabaseInfo::NewLC();
	testDatabaseInfo->TestNewL1L();
	CleanupStack::PopAndDestroy(testDatabaseInfo);
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestdbmanagementapi::HPosLmDatabaseInfo_NewL
// This test case is used to test over loaded NewL function of HPosLmDatabaseInfo class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::HPosLmDatabaseInfo_NewL2L(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseInfo* testDatabaseInfo=CTestPosLmDatabaseInfo::NewLC();
	testDatabaseInfo->TestNewL2L();
	CleanupStack::PopAndDestroy(testDatabaseInfo);
	return KErrNone;	
	}
	
//-----------------------------------------------------------------------------
// Ctestdbmanagementapi::HPosLmDatabaseInfo_DatabaseUriL
// This test case is used to test DatabaseUri function of HPosLmDatabaseInfo class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::HPosLmDatabaseInfo_DatabaseUriL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseInfo* testDatabaseInfo=CTestPosLmDatabaseInfo::NewLC();
	testDatabaseInfo->TestDatabaseUriL();
	CleanupStack::PopAndDestroy(testDatabaseInfo);
	return KErrNone;	
	}

//-----------------------------------------------------------------------------
// Ctestdbmanagementapi::HPosLmDatabaseInfo_Protocol1L
// This test case is used to test Protocol function  of HPosLmDatabaseInfo class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::HPosLmDatabaseInfo_Protocol1L(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseInfo* testDatabaseInfo=CTestPosLmDatabaseInfo::NewLC();
	testDatabaseInfo->TestProtocol1L();
	CleanupStack::PopAndDestroy(testDatabaseInfo);
	return KErrNone;	
	}
	
//-----------------------------------------------------------------------------
// Ctestdbmanagementapi::HPosLmDatabaseInfo_Protocol2L
// This test case is used to test Protocol of HPosLmDatabaseInfo class
// Protocol function must return default protocol function when it is not specified.
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::HPosLmDatabaseInfo_Protocol2L(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseInfo* testDatabaseInfo=CTestPosLmDatabaseInfo::NewLC();
	testDatabaseInfo->TestProtocol2L();
	CleanupStack::PopAndDestroy(testDatabaseInfo);
	return KErrNone;	
	}	
		
//-----------------------------------------------------------------------------
// Ctestdbmanagementapi::HPosLmDatabaseInfo_IsDefaultL
// This test case is used to test IsDefault function of HPosLmDatabaseInfo class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::HPosLmDatabaseInfo_IsDefaultL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseInfo* testDatabaseInfo=CTestPosLmDatabaseInfo::NewLC();
	testDatabaseInfo->TestIsDefaultL();
	CleanupStack::PopAndDestroy(testDatabaseInfo);
	return KErrNone;	
	}
	
//-----------------------------------------------------------------------------
// Ctestdbmanagementapi::HPosLmDatabaseInfo_DatabaseMediaL
// This test case is used to test DatabaseMedia function of HPosLmDatabaseInfo class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::HPosLmDatabaseInfo_DatabaseMediaL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseInfo* testDatabaseInfo=CTestPosLmDatabaseInfo::NewLC();
	testDatabaseInfo->TestDatabaseMediaL();
	CleanupStack::PopAndDestroy(testDatabaseInfo);
	return KErrNone;	
	}
	
//-----------------------------------------------------------------------------
// Ctestdbmanagementapi::HPosLmDatabaseInfo_DatabaseDriveL
// This test case is used to test DatabaseDrive function of HPosLmDatabaseInfo class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::HPosLmDatabaseInfo_DatabaseDriveL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseInfo* testDatabaseInfo=CTestPosLmDatabaseInfo::NewLC();
	testDatabaseInfo->TestDatabaseDriveL();
	CleanupStack::PopAndDestroy(testDatabaseInfo);
	return KErrNone;	
	}

//-----------------------------------------------------------------------------
// Ctestdbmanagementapi::HPosLmDatabaseInfo_Settings1L
// This test case is used to test Settings of function HPosLmDatabaseInfo class
// which returns constant reference
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::HPosLmDatabaseInfo_Settings1L(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseInfo* testDatabaseInfo=CTestPosLmDatabaseInfo::NewLC();
	testDatabaseInfo->TestSettings1L();
	CleanupStack::PopAndDestroy(testDatabaseInfo);
	return KErrNone;	
	}	
	
//-----------------------------------------------------------------------------
// Ctestdbmanagementapi::HPosLmDatabaseInfo_Settings2L
// This test case is used to test Settings of function HPosLmDatabaseInfo class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::HPosLmDatabaseInfo_Settings2L(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseInfo* testDatabaseInfo=CTestPosLmDatabaseInfo::NewLC();
	testDatabaseInfo->TestSettings2L();
	CleanupStack::PopAndDestroy(testDatabaseInfo);
	return KErrNone;	
	}	
	
//-----------------------------------------------------------------------------
// Ctestdbmanagementapi::HPosLmDatabaseInfo_SizeL
// This test case is used to test Size of function HPosLmDatabaseInfo class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::HPosLmDatabaseInfo_SizeL(CStifItemParser& /*aItem*/)
	{
	CTestPosLmDatabaseInfo* testDatabaseInfo=CTestPosLmDatabaseInfo::NewLC();
	testDatabaseInfo->TestSizeL();
	CleanupStack::PopAndDestroy(testDatabaseInfo);
	return KErrNone;	
	}			

//-----------------------------------------------------------------------------
// Ctestdbmanagementapi::TPosLmDatabaseSettings_TPosLmDatabaseSettingsL
// This test case is used to test constructor of TPosLmDatabaseSettings class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::TPosLmDatabaseSettings_TPosLmDatabaseSettingsL(CStifItemParser& /*aItem*/)
	{
	TTestPosLmDataBaseSettings testDatabaseSettings;
	testDatabaseSettings.TestTPosLmDatabaseSettingsL();
	return KErrNone;	
	}	
	
//-----------------------------------------------------------------------------
// Ctestdbmanagementapi::TPosLmDatabaseSettings_IsAttributeSetL
// This test case is used to test IsAttributeSet function of TPosLmDatabaseSettings class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::TPosLmDatabaseSettings_IsAttributeSetL(CStifItemParser& aItem)
	{
	TTestPosLmDataBaseSettings testDatabaseSettings;
	TInt value;
	aItem.GetNextInt(value);
	testDatabaseSettings.TestIsAttributeSetL(value);
	return KErrNone;	
	}	
//-----------------------------------------------------------------------------
// Ctestdbmanagementapi::TPosLmDatabaseSettings_UnsetAttributeL
// This test case is used to test UnsetAttribute function of TPosLmDatabaseSettings class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::TPosLmDatabaseSettings_UnsetAttributeL(CStifItemParser& /*aItem*/)
	{
	TTestPosLmDataBaseSettings testDatabaseSettings;
	testDatabaseSettings.TestUnsetAttributeL();
	return KErrNone;	
	}	
	
//-----------------------------------------------------------------------------
// Ctestdbmanagementapi::TPosLmDatabaseSettings_SetDatabaseNameL
// This test case is used to test SetDatabaseName function of TPosLmDatabaseSettings class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::TPosLmDatabaseSettings_SetDatabaseNameL(CStifItemParser& /*aItem*/)
	{
	TTestPosLmDataBaseSettings testDatabaseSettings;
	testDatabaseSettings.TestSetDatabaseNameL();
	return KErrNone;	
	}
	
//-----------------------------------------------------------------------------
// Ctestdbmanagementapi::TPosLmDatabaseSettings_DatabaseNameL
// This test case is used to test DatabaseName function of TPosLmDatabaseSettings class
// -----------------------------------------------------------------------------
//

TInt Ctestdbmanagementapi::TPosLmDatabaseSettings_DatabaseNameL(CStifItemParser& /*aItem*/)
	{
	TTestPosLmDataBaseSettings testDatabaseSettings;
	testDatabaseSettings.TestDatabaseNameL();
	return KErrNone;	
	}
	
	
			
						
// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  End of File
