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


//  INCLUDES

#include "FT_CPosTp15.h"
#include <EPos_CPosLandmarkDatabase.h> 
           
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp15::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp15::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP15 - Database initialization - Created");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp13::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp15::StartL()
    {  
    TestInitializeDatabaseL(ETrue);

    TestInitializeDatabaseL();
   
    TestInitializeWithUserWaitForL();
    
    TestInitializeDatabaseAndCancelL();

    // Cancel proceeds even if the client cancels it so added User::After just to make sure it's done when the test case completes
    User::After(2000000); 
	}

// ---------------------------------------------------------
// CPosTp15::TestInitializeDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp15::TestInitializeDatabaseL(const TBool aSyncronously)
    {
    _LIT(KAccessErr, "Access method is not returning KErrPosLmNotInitialized when expected");
    _LIT(KInitializedErr, "Method InitializedNeeded is not returning ETrue when expected");
    _LIT(KAccessAfterInitErr, "Access method leaves with %d after initialized");
    
    TRAPD(err, RemoveDefaultDbL());
    while (err == KErrInUse)
        {
        User::After(1000);
        TRAP(err, RemoveDefaultDbL());
        }
    User::LeaveIfError(err);
    
    
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);
    
    CPosLmCategoryManager* cm = CPosLmCategoryManager::NewL(*lmd); 
    CleanupStack::PushL(cm);
    
    TPosLmGlobalCategory  globalCategory = 1;
    TRAP(err, cm->GetGlobalCategoryL(globalCategory));
    AssertTrueSecL(err == KErrPosLmNotInitialized, KAccessErr);
    
    AssertTrueSecL(lmd->IsInitializingNeeded(), KInitializedErr);
    
    if (aSyncronously)
        {
        ExecuteAndDeleteLD(lmd->InitializeL()); 
        }
    else
        {
        RunAsyncOperationLD(lmd->InitializeL());
        }


    TRAP(err, cm->GetGlobalCategoryL(globalCategory));
    AssertTrueSecL(err == KErrNone, KAccessAfterInitErr, err);

    CleanupStack::PopAndDestroy(2, lmd);  

    }


// ---------------------------------------------------------
// CPosTp15::TestInitializeDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp15::TestInitializeWithUserWaitForL()
    {
    // ESLI-64VGXW
    _LIT(KInitializedErr, "Method InitializedNeeded is not returning ETrue when expected");
     
    TRAPD(err, RemoveDefaultDbL());
    while (err == KErrInUse)
        {
        User::After(1000);
        TRAP(err, RemoveDefaultDbL());
        }
    User::LeaveIfError(err);
    
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);
    
    AssertTrueSecL(lmd->IsInitializingNeeded(), KInitializedErr);
    
    CPosLmOperation* op = lmd->InitializeL(); 
    CleanupStack::PushL(op);

    RunAsyncOperationByWaitForReqL(op);

    CleanupStack::PopAndDestroy(2, lmd);  
    
    }

// ---------------------------------------------------------
// CPosTp15::TestInitializeDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp15::TestInitializeDatabaseAndCancelL()
    {
    iLog->Put(_L("TestInitializeDatabaseAndCancelL"));
    _LIT(KAccessCancelErr, "Access method is not returning KErrPosLmNotInitialized after cancel");

    CPosLandmarkDatabase* lmd = NULL;
    TReal32 progress(0);
    while (progress < 1.0)
        {
        RemoveDefaultDbL();
        lmd = CPosLandmarkDatabase::OpenL();
        CleanupStack::PushL(lmd);

        RunAsyncOperationAndCancelInCallbackLD(lmd->InitializeL(), progress);
        progress = progress + 0.09;
        
        CleanupStack::PopAndDestroy(lmd);
        }
        
    lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);
    CPosLmCategoryManager* cm = CPosLmCategoryManager::NewL(*lmd); 
    CleanupStack::PushL(cm);
    
    TPosLmGlobalCategory  globalCategory = 1;
    TRAPD(err, cm->GetGlobalCategoryL(globalCategory));

    // KErrNone means that the initialization already has completed which is OK
    if (err == KErrNone)
        {
        iLog->Put(_L("Err. code returned is KErrNone->initialization has already completed"));
        }
    else 
        {
        AssertTrueSecL(err == KErrPosLmNotInitialized, KAccessCancelErr, err);
        }

    CleanupStack::PopAndDestroy(2, lmd);
    iLog->Put(_L("TestInitializeDatabaseAndCancelL Done"));
    }
    
//  End of File
