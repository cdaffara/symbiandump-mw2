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

#include "FT_CPosTp1.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmDatabaseManager.h>
#include "FT_LandmarkConstants.h"
           
// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp1::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp1::StartL()
    {  
    _LIT(KUriErr, "Incorrect URI is returned from the API");
    _LIT(KDefaultDbUri, "file://C:eposlm.ldb");
  
    // Init test part
    RemoveAllLmDatabasesL();
    
    
    // End init test part
    
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    iLog->Log(_L("CPosLandmarkDatabase::OpenL done"));
    CleanupStack::PushL(lmd);

    // Check that THAN-65NHQW has been corrected by doing a ExecuteAndDeleteLD(lmd->InitializeL()) followed by 
  
    if (lmd->IsInitializingNeeded())
        {
	    iLog->Log(_L("IsInitializingNeeded()"));
        ExecuteAndDeleteLD(lmd->InitializeL());
	    RunAsyncOperationLD(lmd->InitializeL());
        }
    else
 	    {
        iLog->Log(_L("IsInitializingNeeded() Not needed"));
        ExecuteAndDeleteLD(lmd->InitializeL());
        RunAsyncOperationLD(lmd->InitializeL());
        }
    

    CleanupStack::PopAndDestroy(lmd); 

    VerifyLockedDatabaseL();
  
	}

// ---------------------------------------------------------
// CPosTp1::VerifyLockedDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp1::VerifyLockedDatabaseL()
    {
    _LIT(KTestPath, "c:\\system\\test\\testdata\\");
    // copy the test db to the private path
    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);

    TBuf<150> srcPath;

    srcPath.Append(KTestPath);
    srcPath.Append(KTp1TestDb);

    TInt err = fileMan->Copy(srcPath, KLmTp1DefaultDbPath,
            CFileMan::EOverWrite);
    if (err != KErrNone)
        iLog->Log(_L("Error when copying file"));

    CleanupStack::PopAndDestroy(fileMan);
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    // This db must exist
    manager->SetDefaultDatabaseUriL(KTp1TestDb);
    CleanupStack::PopAndDestroy(manager);

    CPosLandmarkDatabase* db1 = CPosLandmarkDatabase::OpenL(KTp1TestDb);
    CleanupStack::PushL(db1);
    if (db1->IsInitializingNeeded())
        {
        TRAPD( err, ExecuteAndDeleteLD( db1->InitializeL() ) );
        AssertTrueSecL(err == KErrNone, _L("Init db failed"));
        }
    
    

    // Take read lock
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);
    User::LeaveIfError(ids.Append(1));
    User::LeaveIfError(ids.Append(3));
    User::LeaveIfError(ids.Append(4));
    User::LeaveIfError(ids.Append(6));
    User::LeaveIfError(ids.Append(7));
    User::LeaveIfError(ids.Append(9));
    User::LeaveIfError(ids.Append(24));
    User::LeaveIfError(ids.Append(14));
    User::LeaveIfError(ids.Append(66));
    User::LeaveIfError(ids.Append(61));
    User::LeaveIfError(ids.Append(14));
    CPosLmOperation* operation = db1->PreparePartialLandmarksL(ids);
    CleanupStack::PushL(operation);
    TReal32 progress(0);
    TRequestStatus status;
    while (progress < 0.4)
        {
        operation->NextStep(status, progress);
        User::WaitForRequest(status);
        }

    // Try to instantiate a new db handle
    operation->NextStep(status, progress);
    CPosLandmarkDatabase* db2 = CPosLandmarkDatabase::OpenL();
    iLog->Log(_L("CPosLandmarkDatabase::OpenL done"));
    delete db2;
    User::WaitForRequest(status);

    // Release read lock
    CleanupStack::PopAndDestroy(operation);
    
    // Take write lock
    operation = db1->RemoveAllLandmarksL();
    CleanupStack::PushL(operation);
    progress = 0;
    while (progress < 0.4)
        {
        operation->NextStep(status, progress);
        User::WaitForRequest(status);
        }

    // Try to instantiate a new db handle - should result in KErrLocked
    operation->NextStep(status, progress);
    db2 = NULL;
    TRAPD(err1, db2 = CPosLandmarkDatabase::OpenL());
    delete db2;
    User::WaitForRequest(status);
    AssertTrueSecL(err1 == KErrLocked, _L("OpenL() didn't result in KErrLocked but in %d"), err1);
    
    CleanupStack::PopAndDestroy(3, db1);
    }

//  End of File
