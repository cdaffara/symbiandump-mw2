/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*   ?description_line
*
*/


// INCLUDE FILES
#include "FT_CPosTp668.h"

#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkCategory.h>
#include <EPos_CPosLmCategoryManager.h>

// CONSTANTS
_LIT(KSetCapPath, "c:\\sys\\bin\\setcap.exe");
// Nr of tests in this TP
const TInt KNrOfTests = 12;

// ================= MEMBER FUNCTIONS =======================

// Destructor
CPosTp668::~CPosTp668()
    {
    }

// ---------------------------------------------------------
// CPosTp668::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//

void CPosTp668::GetName(TDes& aName) const
    {
    _LIT(KTestName0, "SetCap-TP109");
    aName = KTestName0;
    }

// ---------------------------------------------------------
// CPosTp668::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp668::StartL()
    {
    iLog->Put(_L("StartL"));
    
    SetupTestPartL(0);
    SetupTestPartL(1);
    SetupTestPartL(2);
    SetupTestPartL(3);
    SetupTestPartL(4);
    SetupTestPartL(5);
    SetupTestPartL(6);
    SetupTestPartL(7);
    SetupTestPartL(8);
    SetupTestPartL(9);
    SetupTestPartL(10);
    SetupTestPartL(11);
    }

// ---------------------------------------------------------
// CPosTp668::SetupTestPartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp668::SetupTestPartL(TInt aIndex)
    {
    iLog->Put(_L("StartL(TInt aIndex)"));
/* 
#ifdef __WINS__
	_LIT(KNotOnWins, "!!!!This test is NOT supported on WINS!!!!");
	LogErrorAndLeave(KNotOnWins);		
	
#endif
*/
TBuf<20> capability;
switch (aIndex)
{
    case 0:
    iLog->Put(_L("Part1"));
    capability.Zero();
    // Set to no capabilites at all
    capability.Append(_L("0"));
    break;
    
    case 1:
    iLog->Put(_L("Part2"));
    // Set to All capabilities of CAP_CLIENT_DLL but ReadUserData
    capability.Append(_L("37FFE"));
    break;
    
    case 2:
    iLog->Put(_L("Part3"));
    // Set to All capabilities of CAP_CLIENT_DLL but WriteUserData
    capability.Append(_L("2FFFE"));
    break;
    
    case 3:
    iLog->Put(_L("Part4"));
    // Set to All capabilities of CAP_CLIENT_DLL but WriteUserData and ReadUserData
    capability.Append(_L("27FFE"));
    break;
    
    case 4:
    iLog->Put(_L("Part5"));
    // Set to all capabilities of CAP_CLIENT_DLL
    capability.Append(_L("3FFFE"));
    break;
    
    case 5:
    iLog->Put(_L("Part6"));
    // Set to Only TCB
    capability.Append(_L("1"));
    break;
    
    case 6:
    iLog->Put(_L("Part7"));
    // Set to All capabilities but CAP_RECOGNIZER_DLL and TCB
    capability.Append(_L("3FEFE"));
    break;
    
    case 7:
    iLog->Put(_L("Part8"));
    // Set to Only CAP_RECOGNIZER_DLL
    capability.Append(_L("100"));
    break;
    
    case 8:
    iLog->Put(_L("Part9"));
    // Set to Only CAP_RECOGNIZER_DLL and ReadUserData
    capability.Append(_L("8100"));
    break;
    
    case 9:
    iLog->Put(_L("Part10"));
    // Set to All capabilities but CAP_SERVER_DLL and TCB
    capability.Append(_L("ECE"));
    break;
    
    case 10:
    iLog->Put(_L("Part11"));
    // Set to CAP_SERVER_DLL
    capability.Append(_L("3F130"));
    break;
    
    case 11:
    iLog->Put(_L("Part12"));
    // Set to CAP_SERVER_DLL and CommDD
    capability.Append(_L("3F132"));
    break;                            
    
    default:
    iLog->Put(_L("Default"));
    LogErrorAndLeave(_L("We should never come here"));
    break;
}
    // Only for target, on winscw use bat-script
	RProcess execlient;
	RFile file;
	RFs fileSession;
	User::LeaveIfError(fileSession.Connect());
	CleanupClosePushL(fileSession);
	CleanupClosePushL(file);
	CFileMan* fileMan = CFileMan::NewL(fileSession);
	CleanupStack::PushL(fileMan);	

    TBuf<100> buf;

    buf.Append(_L("CapabilityTests.exe "));
    buf.Append(capability);
    buf.Append(_L(" CapabilityTests_tmp.exe"));

    // Make call to SetCap
	TInt result = execlient.Create(KSetCapPath, 
		buf,
		TUidType(KNullUid, KNullUid, KNullUid));
	User::LeaveIfError(result);
	execlient.Resume();	//make the execlient visible
	User::After(800000); //Wait 0.8 s
	
	execlient.Close(); //Close handle to exe-client
		
	// now move file CapabilityTests_tmp.exe to CapabilityTests.exe
	TInt err = fileMan->Move(_L("c:\\sys\\bin\\CapabilityTests_tmp.exe"), _L("c:\\sys\\bin\\CapabilityTests.exe"), CFileMan::EOverWrite);
	if (err != KErrNone)
	    {
	    iLog->Put(_L("Move problem"));
	    LogErrorAndLeave(_L("Move problem"), err);
	    }

	CleanupStack::PopAndDestroy(fileMan); //fileMan
	CleanupStack::PopAndDestroy(&file); //file
	CleanupStack::PopAndDestroy(&fileSession); //fileSession

    // Prepare test conditions
    SetupTestEnvironmentL();
    
    buf.Zero();
    buf.Format(_L("SetCap done, Now Execute CapabilityTest TP107, part %d"), aIndex+1);
    TUtfwUserAnswer answer = iUserInfo->ShowDialog(buf, EUtfwDialogTypeOk, EFalse);
    //if(answer == EUtfwUserAnswerNo)
	}

// ---------------------------------------------------------
// CPosTp668::SetupTestEnvironmentL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp668::SetupTestEnvironmentL()
    {
    iLog->Put(_L("SetupTestEnvironmentL"));
    
    // Setup start conditions for compatibility test
    _LIT(KDefaultDb, "file://c:eposlm.ldb");

    CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    
    // Remove all landmark databases
    CDesCArray* uris = dbManager->ListDatabasesL();
    CleanupStack::PushL(uris);
    for (TInt i = 0; i < uris->Count(); i++)
        {
        dbManager->DeleteDatabaseL((*uris)[i]);
        }
    CleanupStack::PopAndDestroy(uris);
        
    // Create and initialize a default database
    dbManager->CreateDatabaseL(KDefaultDb);
    dbManager->SetDefaultDatabaseL(KDefaultDb);
    CPosLandmarkDatabase* database = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(database);
    ExecuteAndDeleteLD(database->InitializeL());
    
    // Add 5 landmarks - will generate ids 1 - 5
    const TInt KFive = 5;
    for (TInt j = 0; j < KFive; j++)
        {
        CPosLandmark* lm = CPosLandmark::NewLC();
        lm->SetLandmarkNameL(_L("Landmark"));
        database->AddLandmarkL(*lm);
        CleanupStack::PopAndDestroy(lm);
        }
        
    // Add 5 categories
    CPosLmCategoryManager* catManager = CPosLmCategoryManager::NewL(*database);
    CleanupStack::PushL(catManager);
    for (TInt k = 0; k < KFive; k++)
        {
        CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
        TBuf<15> name;
        name = _L("Category ");
        name.AppendNum(k);
        category->SetCategoryNameL(name);
        catManager->AddCategoryL(*category);
        CleanupStack::PopAndDestroy(category);
        }
       
    CleanupStack::PopAndDestroy(3, dbManager);
    }

// End of File