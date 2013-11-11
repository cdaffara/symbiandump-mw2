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
#include "FT_CPosTp666.h"

#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkCategory.h>
#include <EPos_CPosLmCategoryManager.h>

// CONSTANTS
_LIT(KSetCapPath, "c:\\sys\\bin\\setcap.exe");
// Nr of tests in this TP
const TInt KNrOfTests = 4;

// ================= MEMBER FUNCTIONS =======================

// Destructor
CPosTp666::~CPosTp666()
    {
    }

// ---------------------------------------------------------
// CPosTp666::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//

void CPosTp666::GetName(TDes& aName) const
    {
    _LIT(KTestName0, "SetCap-TP107");
    aName = KTestName0;
    }

// ---------------------------------------------------------
// CPosTp666::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp666::StartL()
    {
    iLog->Put(_L("StartL"));
    
    SetupTestPartL(0);
    SetupTestPartL(1);
    SetupTestPartL(2);
    SetupTestPartL(3);
    }

// ---------------------------------------------------------
// CPosTp666::SetupTestPartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp666::SetupTestPartL(TInt aIndex)
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
    // Set to all cap but readuserdata and writeuserdata
    capability.Append(_L("27FFF"));
    break;
    
    case 1:
    iLog->Put(_L("Part2"));
    // Set to Set to all cap but writeuserdata
    capability.Append(_L("2FFFF"));
    break;
    
    case 2:
    iLog->Put(_L("Part3"));
    // Set to Set to all cap but readuserdata
    capability.Append(_L("37FFF"));
    break;
    
    case 3:
    iLog->Put(_L("Part4"));
    // Set to only readuserdata and writeuserdata
    capability.Append(_L("18000"));
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
		//_L("CapabilityTests.exe 3FFFF CapabilityTests_All.exe"),
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
// CPosTp666::SetupTestEnvironmentL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp666::SetupTestEnvironmentL()
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