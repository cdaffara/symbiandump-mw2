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

#include "FT_CPosTp10.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkCategory.h>
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLMOperation.h> 
#include <e32math.h> 

#include "FT_LandmarkConstants.h"

// CONSTANTS
const TInt KNoCategoriesTp10  = 100;
const TInt KNoLandmarksTp10 = 100;      

const TInt KAddStartIndex=5;
const TInt KAddEndIndex=20;
const TInt KAddCategoryIndex=8;

const TInt KAddStartIndexAsync=21;
const TInt KAddEndIndexAsync=82;
const TInt KAddCategoryIndexAsync=10;

const TInt KRemoveStartIndex=85;
const TInt KRemoveEndIndex=95;

const TInt KRemoveStartIndexAsync=75;
const TInt KRemoveEndIndexAsync=84;

const TInt KRemoveSetStartIndex=50;
const TInt KRemoveSetEndIndex=74;

const TInt KRemoveSetStartIndexAsync=0;
const TInt KRemoveSetEndIndexAsync=49;


    
// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp10::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::StartL()
    {
    _LIT(KLogFileName, "c:\\documents\\TP10TimeMeasurements.txt");
    
    RemoveDefaultDbL();
    RemoveAllLmDatabasesL();
    // Test LMREQ121 below
    TestMissingCategories1L();
    RemoveDefaultDbL();
    RemoveAllLmDatabasesL();
    TestMissingCategories2L();

    RemoveDefaultDbL();
    RemoveAllLmDatabasesL();

    _LIT(KTestPath, "c:\\system\\test\\testdata\\");
    // copy the test db to the private path
    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);

    TBuf<150> srcPath;

    srcPath.Append(KTestPath);
    srcPath.Append(KTp10TestDb);

    TInt err = fileMan->Copy(srcPath, KLmTp10DefaultDbPath,
            CFileMan::EOverWrite);
    if (err != KErrNone)
        iLog->Log(_L("Error when copying file"));

    CleanupStack::PopAndDestroy(fileMan);
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    // This db must exist
    manager->SetDefaultDatabaseUriL(KTp10TestDb);
    CleanupStack::PopAndDestroy(manager);

    iDatabase= CPosLandmarkDatabase::OpenL(KTp10TestDb);
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL()); // Synchronous since no argument
       }
    
    TTime start, stop, totalStart, totalStop;
    totalStart.UniversalTime();
	
	start.UniversalTime();
    iCategoryManager = CPosLmCategoryManager::NewL(*iDatabase); 
    stop.UniversalTime();
    	
    TInt64 msec = (stop.Int64() - start.Int64());
	TBuf8<KMsgBufSize> msg;
	_LIT8(KTrace, "CPosLmCategoryManager::NewL = %d usec");
	msg.Format(KTrace, msec);
	LogToFileL(KLogFileName, msg);
	
// Add Category    
    start.UniversalTime();
    AddCategoriesL();
    stop.UniversalTime();
    	
    msec = (stop.Int64() - start.Int64())/1000;
	_LIT8(KTrace2, "AddCategoriesL = %d msec");
	msg.Format(KTrace2, msec);
	LogToFileL(KLogFileName, msg);
	
// Read Category    
	start.UniversalTime();
    ReadCategoriesL();
    stop.UniversalTime();
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace3, "ReadCategoriesL = %d msec");
	msg.Format(KTrace3, msec);
	LogToFileL(KLogFileName, msg);
	
// Update Category    
	start.UniversalTime();
    UpdateCategoriesL();
    stop.UniversalTime();
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace4, "UpdateCategoriesL = %d msec");
	msg.Format(KTrace4, msec);
	LogToFileL(KLogFileName, msg);
	
    ReadCategoriesL();
    AddLandmarksL();
    
// Add Categories to LM synch
    start.UniversalTime();
    AddCategoriesToLandmarksL();
    stop.UniversalTime();
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace5, "AddCategoriesToLandmarksL = %d msec");
	msg.Format(KTrace5, msec);
	LogToFileL(KLogFileName, msg);
	
// Add Categories to LM asynch
	start.UniversalTime();
    AddCategoriesToLandmarksAsyncL();
    stop.UniversalTime();
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace6, "AddCategoriesToLandmarksAsyncL = %d msec");
	msg.Format(KTrace6, msec);
	LogToFileL(KLogFileName, msg);
	
    AddCategoriesToLandmarksAsyncErrCheckL();

// Add Categories to LM using WaitForReq
	start.UniversalTime();
    AddCategoriesToLandmarksUsingWaitForReqL();
    stop.UniversalTime();
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace6b, "AddCategoriesToLandmarksUsingWaitForReqL = %d msec");
	msg.Format(KTrace6b, msec);
	LogToFileL(KLogFileName, msg);
	
    AddCategoriesToLandmarksAsyncErrCheckL();

// Remove Category synchronously
    ExecuteAndDeleteLD(iDatabase->CompactL());
    start.UniversalTime();
    RemoveCategoryL();
    stop.UniversalTime();
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace7, "RemoveCategoryL = %d msec");
	msg.Format(KTrace7, msec);
	LogToFileL(KLogFileName, msg);
    
// Remove Category asynchronously
    start.UniversalTime();
    RemoveCategoryAsyncL();
    stop.UniversalTime();
    ExecuteAndDeleteLD(iDatabase->CompactL());
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace8, "RemoveCategoryAsyncL = %d msec");
	msg.Format(KTrace8, msec);
	LogToFileL(KLogFileName, msg);
    
// Remove Category using WaitForRequest
    start.UniversalTime();
    RemoveCategoryUsingWaitForReqL();
    stop.UniversalTime();
    ExecuteAndDeleteLD(iDatabase->CompactL());
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace8b, "RemoveCategoryUsingWaitForReqL = %d msec");
	msg.Format(KTrace8b, msec);
	LogToFileL(KLogFileName, msg);
    
// Remove set of categories synchronously
    start.UniversalTime();
    RemoveSetOfCategoriesL();
    stop.UniversalTime();
    ExecuteAndDeleteLD(iDatabase->CompactL());
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace9, "RemoveSetOfCategoriesL = %d msec");
	msg.Format(KTrace9, msec);
	LogToFileL(KLogFileName, msg);
	
// Remove set of categories asynchronously
    start.UniversalTime();
    RemoveSetOfCategoriesAsyncL();
    stop.UniversalTime();
    ExecuteAndDeleteLD(iDatabase->CompactL());
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace10, "RemoveSetOfCategoriesAsyncL = %d msec");
	msg.Format(KTrace10, msec);
	LogToFileL(KLogFileName, msg);
	
// Remove set of categories using WaitForRequest
    start.UniversalTime();
    RemoveSetOfCategoriesUsingWaitForReqL();
    stop.UniversalTime();
    ExecuteAndDeleteLD(iDatabase->CompactL());
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace10b, "RemoveSetOfCategoriesUsingWaitForReqL = %d msec");
	msg.Format(KTrace10b, msec);
	LogToFileL(KLogFileName, msg);
	
// Remove category from set of landmarks synchronously
    iCategories.ResetAndDestroy();
    AddCategoriesL();
    AddCategoriesToLandmarksL();
    ExecuteAndDeleteLD(iDatabase->CompactL());

    start.UniversalTime();
    RemoveCategoryFromSetOfLandmarksL();
    stop.UniversalTime();
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace11, "RemoveCategoryFromSetOfLandmarksL = %d msec");
	msg.Format(KTrace11, msec);
	LogToFileL(KLogFileName, msg);
	
// Remove category from set of landmarks asynchronously
	start.UniversalTime();
    RemoveCategoryFromSetOfLandmarksAsyncL();
    stop.UniversalTime();
    ExecuteAndDeleteLD(iDatabase->CompactL());
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace12, "RemoveCategoryFromSetOfLandmarksAsyncL = %d msec");
	msg.Format(KTrace12, msec);
	LogToFileL(KLogFileName, msg);

// Remove category from set of landmarks using User::WaitForRequest()
	start.UniversalTime();
    RemoveCategoryFromSetOfLandmarksUsingWaitForReqL();
    stop.UniversalTime();
    ExecuteAndDeleteLD(iDatabase->CompactL());
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace12b, "RemoveCategoryFromSetOfLandmarksUsingWaitForReqL = %d msec");
	msg.Format(KTrace12b, msec);
	LogToFileL(KLogFileName, msg);

    AddCategoriesToLandmarksL();
    delete iDatabase;
    iDatabase=NULL;
    delete iCategoryManager;
    iCategoryManager = NULL;
    
    iLog->Log(_L("BEFORE : BackupDbFileL"));
    iDatabase = BackupTp10DbFileL();
    iLog->Log(_L("AFTER : BackupDbFileL"));
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
    iCategoryManager = CPosLmCategoryManager::NewL(*iDatabase); 

	// Cancel tests immediately
    iLog->Log(_L("TESTS BELOW ARE TESTING CANCEL"));
    AddCategoriesToLandmarksAsyncAndCancelL();
    RemoveCategoryAsyncAndCancelL();
    RemoveSetOfCategoriesAsyncAndCancelL();
    RemoveCategoryFromSetOfLandmarksAsyncAndCancelL();

	// Cancel tests after a while
    iLog->Log(_L("TESTS BELOW ARE CANCEL IN CALLBACK"));
    delete iDatabase;
    iDatabase=NULL;
    
    
    iLog->Log(_L("BEFORE : RestoreDbFileL"));
    iDatabase = RestoreTp10DbFileL();
    iLog->Log(_L("AFTER : RestoreDbFileL"));
    
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
    delete iCategoryManager;
    iCategoryManager = NULL;
    iCategoryManager = CPosLmCategoryManager::NewL(*iDatabase); 
    
    ExecuteAndDeleteLD(iDatabase->CompactL());
    AddCategoriesToLandmarksAsyncAndCancelL(ETrue);
    RemoveCategoryAsyncAndCancelL(ETrue);
    RemoveSetOfCategoriesAsyncAndCancelL(ETrue);
    delete iDatabase;
    iDatabase=NULL;
    iDatabase = RestoreTp10DbFileL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
    delete iCategoryManager;
    iCategoryManager = NULL;
    iCategoryManager = CPosLmCategoryManager::NewL(*iDatabase); 
    ExecuteAndDeleteLD(iDatabase->CompactL());
    RemoveCategoryFromSetOfLandmarksAsyncAndCancelL(ETrue);
    

    totalStop.UniversalTime();   
    msec = (totalStop.Int64() - totalStart.Int64())/1000;
    _LIT8(KTrace13, "Complete test case = %d msec");
	msg.Format(KTrace13, msec);
	LogToFileL(KLogFileName, msg);

    }

// ---------------------------------------------------------
// CPosTp10::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::CloseTest()
    {
    iCategories.ResetAndDestroy();
    iCategories.Close();
    iLandmarks.ResetAndDestroy();
    iLandmarks.Close();
    delete iDatabase;
    iDatabase=NULL;
    delete iCategoryManager;
    iCategoryManager=NULL;
    }

// ---------------------------------------------------------
// CPosTp10::AddCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::AddCategoriesL()
    {
    iLog->Log(_L("Adding and copying categories"));
    _LIT(KAlreadyExistErr, "A category with the same name is already added so add should return KErrAlreadyExists");    
    _LIT(KNoNameErr, "The Category has no name so add should return KErrArgument");
    
    _LIT(KCategoryName, "CategoryTP10 - %d");
    
    TInt index=0;
    while (index < KNoCategoriesTp10)
        {
        TBuf<100> name;
        name.Format(KCategoryName, ++index);
        CPosLandmarkCategory* category = CreateCategoryLC(name);
        
        CPosLandmarkCategory* copy = CPosLandmarkCategory::NewLC(*category);
        name.Format(KCategoryName, ++index);
        copy->SetCategoryNameL(name); 

        CPosLandmarkCategory* category2 = CPosLandmarkCategory::NewL();
        CleanupStack::PushL(category2);
        name.Format(KCategoryName, ++index);
        category2->SetCategoryNameL(name); 

        CPosLandmarkCategory* copy2 = CPosLandmarkCategory::NewL(*category2);
        CleanupStack::PushL(copy2); 
        name.Format(KCategoryName, ++index);
        copy2->SetCategoryNameL(name); 
        
        ExecuteAndDeleteLD(iDatabase->CompactL());
        iCategoryManager->AddCategoryL(*category);
        iCategoryManager->AddCategoryL(*copy);
        iCategoryManager->AddCategoryL(*category2);
        iCategoryManager->AddCategoryL(*copy2);
      
        CPosLandmarkCategory* category3 = CPosLandmarkCategory::NewLC();
        category3->SetCategoryNameL(name);
  
        TInt err = 0;
        TRAP(err, iCategoryManager->AddCategoryL(*category3));
        AssertTrueSecL(err == KErrAlreadyExists, KAlreadyExistErr);    
        CleanupStack::PopAndDestroy(category3);

        CPosLandmarkCategory* category4 = CPosLandmarkCategory::NewLC();
        TRAP(err, iCategoryManager->AddCategoryL(*category3));
        AssertTrueSecL(err == KErrArgument, KNoNameErr);    
        CleanupStack::PopAndDestroy(category4);

        iCategories.Append(category);
        iCategories.Append(copy);
        iCategories.Append(category2);
        iCategories.Append(copy2);

        CleanupStack::Pop(4); 
        }
    }

// ---------------------------------------------------------
// CPosTp10::CompareTwoCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::CompareTwoCategoriesL(TInt aIndex)
    {
    CPosLandmarkCategory* category = iCategoryManager->ReadCategoryLC(iCategories[aIndex]->CategoryId());
    CompareCategoriesL(*iCategories[aIndex], *category);    
    CleanupStack::PopAndDestroy(category);
    }
    
// ---------------------------------------------------------
// CPosTp10::ReadCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::ReadCategoriesL()
    {
    iLog->Log(_L("Reading categories"));
    _LIT(KReadNotFoundErr, "The category id doesn't exist so ReadCategory should return not found");    
    
    for (TInt i = 0; i<iCategories.Count(); i++)
        {
        
        TInt err = KErrLocked;
        while (err == KErrLocked)
            {
            TRAP(err, CompareTwoCategoriesL(i));
            if (err == KErrLocked) 
                {
                iLog->Log(_L("KErrLocked from CompareTwoCategoriesL, try again"));
                User::After(100000);
                }
            }
        }
    
    TPosLmItemId id = 5123321;
    
    CPosLandmarkCategory* category=NULL;
    TRAPD(err, category=iCategoryManager->ReadCategoryLC(id));
    delete category;
    AssertTrueSecL(err == KErrNotFound, KReadNotFoundErr);
    }

// ---------------------------------------------------------
// CPosTp10::UpdateCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::UpdateCategoriesL()
    {
    iLog->Log(_L("Updating categories"));    
    _LIT(KCategoryNameNotSetErr, "UpdateCategory does not leave with KErrArgumenet when name is not set");
    _LIT(KCategoryExistsErr, "UpdateCategory does not leave with KErrAlreadyExist");

    TInt err;
    ExecuteAndDeleteLD(iDatabase->CompactL());
    CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
    TRAP(err, iCategoryManager->UpdateCategoryL(*category));
    AssertTrueSecL(err == KErrArgument, KCategoryNameNotSetErr);    
    CleanupStack::PopAndDestroy(category);

    CPosLandmarkCategory* category2 = CPosLandmarkCategory::NewLC(*iCategories[0]);

    TPtrC nameThatExist;
    User::LeaveIfError(iCategories[1]->GetCategoryName(nameThatExist));
    category2->SetCategoryNameL(nameThatExist);
    TRAP(err, iCategoryManager->UpdateCategoryL(*category2));
    AssertTrueSecL(err == KErrAlreadyExists, KCategoryExistsErr);    
    CleanupStack::PopAndDestroy(category2);
    
    _LIT(KUpdatedCategoryName, "UpdatedCategoryTP10 - %d");

    for (TInt i = 0; i<iCategories.Count(); i++)
        {
        TBuf<100> name;
        name.Format(KUpdatedCategoryName, i);
        iCategories[i]->SetCategoryNameL(name);
        iCategories[i]->SetIconL(KMbmFileName2, KIconIndex2, KPosLmIconMaskNotUsed);
        err = KErrLocked;
        while (err == KErrLocked)
            {
            TRAP(err, iCategoryManager->UpdateCategoryL(*iCategories[i]));
            }

        }
    }

// ---------------------------------------------------------
// CPosTp10::AddLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::AddLandmarksL()
    {
    iLog->Log(_L("Adding landmarks"));    
    _LIT(KLmName, "LmTP10 - %d");
    _LIT(KLmDescription, "LmTP10Description - %d");
    
    for (TInt i=0; i<(KNoLandmarksTp10); i++)
        {
        TBuf<100> lmName;
        lmName.Format(KLmName,i);
        TBuf<100> lmDesc;
        lmDesc.Format(KLmDescription,i);
     
        CPosLandmark* landmark = CPosLandmark::NewLC();  
        landmark->SetLandmarkNameL(lmName);
        landmark->SetLandmarkDescriptionL(lmDesc);
        
        landmark->AddCategoryL(iCategories[i]->CategoryId());
       
        TInt err=KErrLocked;
        while (err==KErrLocked)
            {
            TRAP(err, iDatabase->AddLandmarkL(*landmark));
            }
        iLandmarks.Append(landmark);
        CleanupStack::Pop(); 
        }
    } 

// ---------------------------------------------------------
// CPosTp10::AddCategoriesToSetOfLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::AddCategoriesToLandmarksL()
    {
    iLog->Log(_L("Adding categories to landmarks"));  
    
    RArray<TPosLmItemId> landmarksIds;
    CleanupClosePushL(landmarksIds);
    for(TInt i=KAddStartIndex; i<=KAddEndIndex; i++)
        {
        landmarksIds.Append(iLandmarks[i]->LandmarkId());
        iLandmarks[i]->AddCategoryL(iCategories[KAddCategoryIndex]->CategoryId());
        }
 
    // Add some invalid as well
    landmarksIds.Append(2000);
    landmarksIds.Append(3000);
    
    TInt err=KErrLocked;
    while (err==KErrLocked)
    	{
    	TRAP( err, ExecuteAndDeleteLD(iCategoryManager->AddCategoryToLandmarksL(
                                    iCategories[KAddCategoryIndex]->CategoryId(), 
                                    landmarksIds)) );
      }
    
    CleanupStack::PopAndDestroy(&landmarksIds); 
    CheckLandmarksL();
    }

// ---------------------------------------------------------
// CPosTp10::AddCategoriesToSetOfLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::AddCategoriesToLandmarksAsyncL()
    {
    iLog->Log(_L("Adding categories to landmarks asynchronously"));
    RArray<TPosLmItemId> landmarksIds;
    CleanupClosePushL(landmarksIds);
    
    for(TInt i=KAddStartIndexAsync; i<=KAddEndIndexAsync; i++)
        {
        landmarksIds.Append(iLandmarks[i]->LandmarkId());
        iLandmarks[i]->AddCategoryL(iCategories[KAddCategoryIndexAsync]->CategoryId());
        }
 
    
    RunAsyncOperationLD(iCategoryManager->AddCategoryToLandmarksL(
                                        iCategories[KAddCategoryIndexAsync]->CategoryId(), 
                                        landmarksIds
                                        ));
    CheckLandmarksL();
    
    CleanupStack::PopAndDestroy(&landmarksIds);
    }

// ---------------------------------------------------------
// CPosTp10::AddCategoriesToLandmarksUsingWaitForReqL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::AddCategoriesToLandmarksUsingWaitForReqL()
    {
    iLog->Log(_L("AddCategoriesToLandmarksUsingWaitForReqL"));
    RArray<TPosLmItemId> landmarksIds;
    CleanupClosePushL(landmarksIds);
    
    for(TInt i=KAddStartIndexAsync; i<=KAddEndIndexAsync; i++)
        {
        landmarksIds.Append(iLandmarks[i]->LandmarkId());
        iLandmarks[i]->AddCategoryL(iCategories[KAddCategoryIndexAsync]->CategoryId());
        }
 
    CPosLmOperation* op = iCategoryManager->AddCategoryToLandmarksL(
        iCategories[KAddCategoryIndexAsync]->CategoryId(), 
        landmarksIds);
    CleanupStack::PushL(op);
    RunAsyncOperationByWaitForReqL(op);
    CleanupStack::PopAndDestroy(op);

    CheckLandmarksL();
    
    CleanupStack::PopAndDestroy(&landmarksIds);
    }

// ---------------------------------------------------------
// CPosTp10::AddCategoriesToSetOfLandmarksAndCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::AddCategoriesToLandmarksAsyncAndCancelL(const TBool& aInCallback)
    {
    iLog->Log(_L("Adding categories to landmarks asyncronously and cancel"));
    RArray<TPosLmItemId> landmarksIds;
    CleanupClosePushL(landmarksIds);
    
    for(TInt i=KAddStartIndexAsync; i<=KAddEndIndexAsync; i++)
        {
        landmarksIds.Append(iLandmarks[i]->LandmarkId());
        iLandmarks[i]->AddCategoryL(iCategories[KAddCategoryIndexAsync]->CategoryId());
        }
 
    if (aInCallback)
        {
        RunAsyncOperationAndCancelInCallbackLD(iCategoryManager->AddCategoryToLandmarksL(
                                        iCategories[KAddCategoryIndexAsync]->CategoryId(), 
                                        landmarksIds
                                        ));
        }
    else
        {
        RunAsyncOperationAndCancelLD(iCategoryManager->AddCategoryToLandmarksL(
                                        iCategories[KAddCategoryIndexAsync]->CategoryId(), 
                                        landmarksIds 
                                        ));
        }

    CleanupStack::PopAndDestroy(&landmarksIds);
    }

// ---------------------------------------------------------
// CPosTp10::AddCategoriesToLandmarksAsyncErrCheckL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::AddCategoriesToLandmarksAsyncErrCheckL()
    {
    iLog->Log(_L("Adding category not in db to landmarks async."));

    _LIT(KNotFoundErr, "AddCategoryToLandmarksL is not returning -1 for a category id not in db");

    TPosLmItemId idNotInDb = 5123321;
    
    RArray<TPosLmItemId> landmarksIds;
    CleanupClosePushL(landmarksIds);

    landmarksIds.Append(iLandmarks[0]->LandmarkId());
   
    TRAPD(err, iOperation = iCategoryManager->AddCategoryToLandmarksL(idNotInDb, landmarksIds));
    
    AssertTrueSecL(err == KErrNotFound, KNotFoundErr);
    
  
    iOperation = NULL;
    CleanupStack::PopAndDestroy(&landmarksIds);
    }

// ---------------------------------------------------------
// CPosTp10::CheckLandmarks
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::CheckLandmarksL()
    {
    for (TInt i = 0; i<iLandmarks.Count(); i++)
        {
        CPosLandmark* lm = iDatabase->ReadLandmarkLC(iLandmarks[i]->LandmarkId());
        CompareLandmarksL(*iLandmarks[i], *lm);    
        CleanupStack::PopAndDestroy(); // lm 
        }
    }

// ---------------------------------------------------------
// CPosTp10::RemoveCategoryL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::RemoveCategoryL()
    {
    iLog->Log(_L("Removing categories"));
  
    TInt i=0;  
    for (i=0; i<iLandmarks.Count(); i++)
        {
        iLandmarks[i]->AddCategoryL(iCategories[KRemoveStartIndex]->CategoryId());
        if (i % 2)
            {
            TInt index = 1+KRemoveStartIndex;
            iLandmarks[i]->AddCategoryL(iCategories[index]->CategoryId());
            }
        }
    
    for (TInt j=0; j<2; j++)
        {
        for (i = KRemoveStartIndex; i <= KRemoveEndIndex; i++)
            {    
            ExecuteAndDeleteLD(iCategoryManager->RemoveCategoryL(iCategories[i]->CategoryId()));  
            }
        CheckRemovedL(KRemoveStartIndex, KRemoveEndIndex);
        }
    }

// ---------------------------------------------------------
// CPosTp10::RemoveCategoryAsyncL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::RemoveCategoryAsyncL()
    {
    iLog->Log(_L("Removing categories asyncronously"));
    TInt i=0;
    for (i=0; i<iLandmarks.Count(); i++)
        {
        iLandmarks[i]->AddCategoryL(iCategories[KRemoveStartIndexAsync]->CategoryId());
        if (i % 2)
            {
            TInt index = 1+KRemoveStartIndexAsync;
            iLandmarks[i]->AddCategoryL(iCategories[index]->CategoryId());
            }
        }

    for (TInt j=0; j<2; j++)
        {
        for (i = KRemoveStartIndexAsync; i <= KRemoveEndIndexAsync; i++)
            {
            RunAsyncOperationLD(iCategoryManager->RemoveCategoryL(iCategories[i]->CategoryId())); 
            }
        CheckRemovedL(KRemoveStartIndexAsync, KRemoveEndIndexAsync);
        }
    }

// ---------------------------------------------------------
// CPosTp10::RemoveCategoryUsingWaitForReqL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::RemoveCategoryUsingWaitForReqL()
    {
    iLog->Log(_L("Removing categories using User::WaitForRequest"));
    TInt i=0;
    for (i=0; i<iLandmarks.Count(); i++)
        {
        iLandmarks[i]->AddCategoryL(iCategories[KRemoveStartIndexAsync]->CategoryId());
        if (i % 2)
            {
            TInt index = 1+KRemoveStartIndexAsync;
            iLandmarks[i]->AddCategoryL(iCategories[index]->CategoryId());
            }
        }

    for (TInt j=0; j<2; j++)
        {
        for (i = KRemoveStartIndexAsync; i <= KRemoveEndIndexAsync; i++)
            {
            CPosLmOperation* op = iCategoryManager->RemoveCategoryL(iCategories[i]->CategoryId());
            CleanupStack::PushL(op);
            RunAsyncOperationByWaitForReqL(op);
            CleanupStack::PopAndDestroy(op);
            }
        CheckRemovedL(KRemoveStartIndexAsync, KRemoveEndIndexAsync);
        }
    }

// ---------------------------------------------------------
// CPosTp10::RemoveCategoryAsyncAndCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::RemoveCategoryAsyncAndCancelL(const TBool& aInCallback)
    {
    iLog->Log(_L("Removing categories asyncronously and cancel"));
    TInt i=0;
    for (i=0; i<iLandmarks.Count(); i++)
        {
        iLandmarks[i]->AddCategoryL(iCategories[KRemoveStartIndexAsync]->CategoryId());
        if (i % 2)
            {
            TInt index = 1+KRemoveStartIndexAsync;
            iLandmarks[i]->AddCategoryL(iCategories[index]->CategoryId());
            }
        }

    for (TInt j=0; j<2; j++)
        {
        for (i = KRemoveStartIndexAsync; i <= KRemoveEndIndexAsync; i++)
            {
            if (aInCallback)
                {
                RunAsyncOperationAndCancelInCallbackLD(iCategoryManager->RemoveCategoryL(iCategories[i]->CategoryId()));     
                }
            else
                {
                RunAsyncOperationAndCancelLD(iCategoryManager->RemoveCategoryL(iCategories[i]->CategoryId()));     
                }
            }
        }
    }

// ---------------------------------------------------------
// CPosTp10::RemoveSetOfCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::RemoveSetOfCategoriesL()
    {
    iLog->Log(_L("Removing set of categories"));
  
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);

    for (TInt i=KRemoveSetStartIndex; i<=KRemoveSetEndIndex; i++)
        {
        ids.Append(iCategories[i]->CategoryId());
        }
    
    for (TInt j=0; j<2; j++)
        {
        ExecuteAndDeleteLD(iCategoryManager->RemoveCategoriesL(ids));
        
        CheckRemovedL(KRemoveSetStartIndex, KRemoveSetEndIndex);
        }
    CleanupStack::PopAndDestroy(&ids); 
    }

// ---------------------------------------------------------
// CPosTp10::RemoveSetOfCategoriesAsyncL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::RemoveSetOfCategoriesAsyncL()
    {
    iLog->Log(_L("Removing set of categories asyncronously"));
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);

    TInt i=0;
    for (i=KRemoveSetStartIndexAsync; i<=KRemoveSetEndIndexAsync; i++)
        {
        ids.Append(iCategories[i]->CategoryId());
        }

    for (TInt j=0; j<2; j++)
        {
        for (i = KRemoveSetStartIndexAsync; i <= KRemoveSetEndIndexAsync; i++)
            {
            TInt err = KErrLocked;
            while (err == KErrLocked)
                {
                TRAP(err, RunAsyncOperationLD(iCategoryManager->RemoveCategoriesL(ids)));
                }
            } 
        CheckRemovedL(KRemoveSetStartIndexAsync, KRemoveSetEndIndexAsync);
        }
    CleanupStack::PopAndDestroy(&ids); 
    }

// ---------------------------------------------------------
// CPosTp10::RemoveSetOfCategoriesUsingWaitForReqL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::RemoveSetOfCategoriesUsingWaitForReqL()
    {
    iLog->Log(_L("Removing set of categories using User::WaitForRequest"));
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);

    TInt i=0;
    for (i=KRemoveSetStartIndexAsync; i<=KRemoveSetEndIndexAsync; i++)
        {
        ids.Append(iCategories[i]->CategoryId());
        }
        
    // Add some unvalid ids as well
    ids.Append(222);
    ids.Append(223);

    for (TInt j=0; j<2; j++)
        {
        for (i = KRemoveSetStartIndexAsync; i <= KRemoveSetEndIndexAsync; i++)
            {
            CPosLmOperation* op = iCategoryManager->RemoveCategoriesL(ids);
            CleanupStack::PushL(op);
            RunAsyncOperationByWaitForReqL(op);
            CleanupStack::PopAndDestroy(op);
            } 
        CheckRemovedL(KRemoveSetStartIndexAsync, KRemoveSetEndIndexAsync);
        }
    CleanupStack::PopAndDestroy(&ids); 
    }

// ---------------------------------------------------------
// CPosTp10::RemoveSetOfCategoriesAsyncAndCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::RemoveSetOfCategoriesAsyncAndCancelL(const TBool& aInCallback)
    {
    iLog->Log(_L("Removing set of categories asyncronously and cancel"));
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);

    TInt i=0;
    for (i=KRemoveSetStartIndexAsync; i<=KRemoveSetEndIndexAsync; i++)
        {
        ids.Append(iCategories[i]->CategoryId());
        }

    for (TInt j=0; j<2; j++)
        {
        for (i = KRemoveSetStartIndexAsync; i <= KRemoveSetEndIndexAsync; i++)
            {
            if (aInCallback)
                {
                RunAsyncOperationAndCancelInCallbackLD(iCategoryManager->RemoveCategoriesL(ids)); 
                }
            else
                {
                RunAsyncOperationAndCancelLD(iCategoryManager->RemoveCategoriesL(ids)); 
                }
            } 
        }
    CleanupStack::PopAndDestroy(&ids); 
    }

// ---------------------------------------------------------
// CPosTp10::RemoveCategoryFromSetOfLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::RemoveCategoryFromSetOfLandmarksL()
    {
    iLog->Log(_L("Removing set of categories from landmarks"));
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);

    for (TInt i = KAddStartIndex; i<=KAddEndIndex; i++)
        {
        ids.Append(iLandmarks[i]->LandmarkId());
        
        iLandmarks[i]->RemoveCategory(iCategories[KAddCategoryIndex]->CategoryId());
        }
    // Append some non-existing as well
    ids.Append(666);
    ids.Append(667);
    
    for (TInt j=0; j<2; j++)
        {
        ExecuteAndDeleteLD(
            iCategoryManager->RemoveCategoryFromLandmarksL(
                iCategories[KAddCategoryIndex]->CategoryId(), 
                ids)); 
        
        CheckLandmarksL();
        }
        
    // Test that RemoveCategoryFromLandmarksL leaves with KErrNotFound if
    // category is not found
    TRAPD(err, iCategoryManager->ReadCategoryLC(123456));
    if (err != KErrNotFound)
    {
    iLog->Log(_L("ReadCategoryLC should leave with KErrNotFound"));
    User::Leave(err);
    	
    }
    TRAP(err, iOperation = iCategoryManager->RemoveCategoryFromLandmarksL(123456 ,ids));
    if (err == KErrNone) 
        {
        delete iOperation;
        iOperation = NULL;
        iLog->Log(_L("RemoveCategoryFromLandmarksL should leave with KErrNotFound"));
        User::Leave(err);
        }
    if (err != KErrNotFound) 
    {      
    iLog->Log(_L("RemoveCategoryFromLandmarksL should leave with KErrNotFound"));
    User::Leave(err);
    }
    CleanupStack::PopAndDestroy(&ids); 
    }

// ---------------------------------------------------------
// CPosTp10::RemoveCategoryFromSetOfLandmarksAsyncL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::RemoveCategoryFromSetOfLandmarksAsyncL()
    {
    iLog->Log(_L("Removing set of categories from landmarks asyncronously"));
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);

    TInt i=0;
    for (i=KAddStartIndexAsync; i<=KAddEndIndexAsync; i++)
        {
        ids.Append(iLandmarks[i]->LandmarkId());
        }
    for (TInt j=0; j<2; j++)
        {
        
        for (i = KAddStartIndexAsync; i <= KAddEndIndexAsync; i++)
            {
            RunAsyncOperationLD(iCategoryManager->RemoveCategoryFromLandmarksL(
                iCategories[KAddCategoryIndexAsync]->CategoryId(),
                ids
                )); 
            }
        CheckLandmarksL(); 
        }
    CleanupStack::PopAndDestroy(&ids); 
    }

// ---------------------------------------------------------
// CPosTp10::RemoveCategoryFromSetOfLandmarksUsingWaitForReqL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::RemoveCategoryFromSetOfLandmarksUsingWaitForReqL()
    {
    iLog->Log(_L("Removing set of categories from landmarks using User::WaitForRequest()"));
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);

    TInt i=0;
    for (i=KAddStartIndexAsync; i<=KAddEndIndexAsync; i++)
        {
        ids.Append(iLandmarks[i]->LandmarkId());
        }
    for (TInt j=0; j<2; j++)
        {
        
        for (i = KAddStartIndexAsync; i <= KAddEndIndexAsync; i++)
            {
            CPosLmOperation* op = iCategoryManager->RemoveCategoryFromLandmarksL(
                iCategories[KAddCategoryIndexAsync]->CategoryId(),
                ids); 
            CleanupStack::PushL(op);
            RunAsyncOperationByWaitForReqL(op);
            CleanupStack::PopAndDestroy(op);
            }
        CheckLandmarksL(); 
        }
    CleanupStack::PopAndDestroy(&ids); 
    }

// ---------------------------------------------------------
// CPosTp10::RemoveCategoryFromSetOfLandmarksAsyncAndCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::RemoveCategoryFromSetOfLandmarksAsyncAndCancelL(const TBool& aInCallback)
    {
    iLog->Log(_L("Removing set of categories from landmarks asyncronously and cancel"));
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);

    TInt i=0;
    for (i=KAddStartIndexAsync; i<=KAddEndIndexAsync; i++)
        {
        ids.Append(iLandmarks[i]->LandmarkId());
        }
    for (TInt j=0; j<2; j++)
        {
        for (i = KAddStartIndexAsync; i <= KAddEndIndexAsync; i++)
            {
            if (aInCallback)
                {
                RunAsyncOperationAndCancelInCallbackLD(
                    iCategoryManager->RemoveCategoryFromLandmarksL(
                    iCategories[KAddCategoryIndexAsync]->CategoryId(),
                    ids
                    ));
                }
            else
                {
                RunAsyncOperationAndCancelLD(
                    iCategoryManager->RemoveCategoryFromLandmarksL(
                    iCategories[KAddCategoryIndexAsync]->CategoryId(),
                    ids 
                    ));
                }
            }
        }

    CleanupStack::PopAndDestroy(&ids); 
    }

// ---------------------------------------------------------
// CPosTp10::CheckRemoved
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::CheckRemovedL(const TInt aStartIndex, const TInt aStopIndex)
    {
    _LIT(KRemoveNotFoundErr, "Category has not been removed");
    _LIT(KRemoveFoundErr, "Category should not been removed");

    for (TInt i = 0; i<=aStopIndex; i++)
        {
        TRAPD(err, ReadCategoryL(iCategories[i]->CategoryId()));
        if (i >= aStartIndex && i <= aStopIndex)
            {
            AssertTrueSecL(err == KErrNotFound, KRemoveNotFoundErr);                  
            RemovedFromLandmarksInternalL(iCategories[i]->CategoryId());
            }
        else
            {
            AssertTrueSecL(err == KErrNone, KRemoveFoundErr);
            }
        }

    CheckLandmarksL();

    }

// ---------------------------------------------------------
// CPosTp10::RemovedFromLandmarksInternalL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::RemovedFromLandmarksInternalL(TPosLmItemId aCategoryId)
    {
    for (TInt i = 0; i < iLandmarks.Count(); i++)
        {
        iLandmarks[i]->RemoveCategory(aCategoryId);
        }
    }

// ---------------------------------------------------------
// CPosTp10::ReadCategoryL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::ReadCategoryL(TPosLmItemId aCategoryId)
    {
    CPosLandmarkCategory* lmc = iCategoryManager->ReadCategoryLC(aCategoryId);
    CleanupStack::PopAndDestroy(lmc);
    }

// ---------------------------------------------------------
// CPosTp10::TestMissingCategories1L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::TestMissingCategories1L()
    {
    iLog->Log(_L("TestMissingCategories1L"));
    _LIT(KTestPath, "c:\\system\\test\\testdata\\");
    // copy the test db to the private path
    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);

    TBuf<150> srcPath;

    srcPath.Append(KTestPath);
    srcPath.Append(KTp10TestDb);

    TInt err = fileMan->Copy(srcPath, KLmTp10DefaultDbPath,
            CFileMan::EOverWrite);
    if (err != KErrNone)
        iLog->Log(_L("Error when copying file"));

    CleanupStack::PopAndDestroy(fileMan);
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    // This db must exist
    manager->SetDefaultDatabaseUriL(KTp10TestDb);
    CleanupStack::PopAndDestroy(manager);

    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL(KTp10TestDb);
    CleanupStack::PushL(lmd);

    if (lmd->IsInitializingNeeded())
        {
        ExecuteAndDeleteLD(lmd->InitializeL());
        }
        
    ExecuteAndDeleteLD(lmd->CompactL());
    
    // Remove categories 1,2,3 (Pizzeria, Hamburgerbar, Kinarestaurang)
    // Landmark with id=7 "Billigt" contains categories: 1,2,3 and 14-17
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*lmd);
    CleanupStack::PushL(categoryManager);   

    CPosLandmark* lm = lmd->ReadLandmarkLC(7);

    ExecuteAndDeleteLD(categoryManager->RemoveCategoryL(1));
    ExecuteAndDeleteLD(categoryManager->RemoveCategoryL(2));
    ExecuteAndDeleteLD(categoryManager->RemoveCategoryL(3));

    // Change landmark name and update landmark, the server must not crash
    lm->SetLandmarkNameL(_L("NyttBilligtNamn"));

    lmd->UpdateLandmarkL(*lm);

    CleanupStack::PopAndDestroy(lm);
    lm = lmd->ReadLandmarkLC(7);

    RArray<TPosLmItemId> categories;
    CleanupClosePushL(categories);
    lm->GetCategoriesL(categories);

    // Now list all categories belonging to this landmark
    // verify that the removed categories 1,2,3 are no longer referred from this landmark
    // Should now only contain 14-17
    if (categories.Count() != 4)
        {
         iLog->Log(_L("Wrong number of categories"));
         User::Leave(-1);
        }

    for (TInt i=0;i<categories.Count();i++)
        {
        if (categories[i] == 1 || categories[i] == 2 || categories[i] == 3)
            {
            iLog->Log(_L("Wrong number of categories"));
         	User::Leave(-1);	
            }
        }

    CleanupStack::PopAndDestroy(&categories);
    CleanupStack::PopAndDestroy(lm);
    CleanupStack::PopAndDestroy(categoryManager);
    CleanupStack::PopAndDestroy(lmd);
    }

// ---------------------------------------------------------
// CPosTp10::TestMissingCategories2L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp10::TestMissingCategories2L()
    {
    iLog->Log(_L("TestMissingCategories2L"));
    
    _LIT(KTestPath, "c:\\system\\test\\testdata\\");
    // copy the test db to the private path
    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);

    TBuf<150> srcPath;

    srcPath.Append(KTestPath);
    srcPath.Append(KTp10TestDb);

    TInt err = fileMan->Copy(srcPath, KLmTp10DefaultDbPath,
            CFileMan::EOverWrite);
    if (err != KErrNone)
        iLog->Log(_L("Error when copying file"));

    CleanupStack::PopAndDestroy(fileMan);
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    // This db must exist
    manager->SetDefaultDatabaseUriL(KTp10TestDb);
    CleanupStack::PopAndDestroy(manager);

    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL(KTp10TestDb);
    CleanupStack::PushL(lmd);

    if (lmd->IsInitializingNeeded())
        {
        ExecuteAndDeleteLD(lmd->InitializeL());
        }

    ExecuteAndDeleteLD(lmd->CompactL());

    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*lmd);
    CleanupStack::PushL(categoryManager);

    // Now create a landmark containing some categories, before adding this landmark to
    // the database remove some of the categories from the landmarks database
    CPosLandmark* lm = CreateXMLLandmarkLC(_L("PelleFantLandmark"));
    // Add categories with id=7, id=14 and id=10 to this landmark
    lm->AddCategoryL(7);
    lm->AddCategoryL(14);
    lm->AddCategoryL(10);

    //Then remove categories id=14 and id=7 from the landmark db
    ExecuteAndDeleteLD(categoryManager->RemoveCategoryL(14));
    ExecuteAndDeleteLD(categoryManager->RemoveCategoryL(7));

    TPosLmItemId id = lmd->AddLandmarkL(*lm);
    
    CleanupStack::PopAndDestroy(lm);

    lm = lmd->ReadLandmarkLC(id);

    RArray<TPosLmItemId> categories;
    CleanupClosePushL(categories);
    lm->GetCategoriesL(categories);

    // Now list all categories belonging to this landmark
    if (categories.Count() != 1)
        {
        TBuf<50> buf;
        buf.Format(_L("Wrong number of categories, was %d should be %d"), categories.Count(), 1);
        for (TInt j=0;j<categories.Count();j++)
            {
            TBuf<50> buffe;
            buffe.Format(_L("%d Category id: %d"), j, categories[j]);
            iLog->Log(buffe);
            }
        iLog->Log(buf);
        User::Leave(KErrNone);
        }

    for (TInt i=0;i<categories.Count();i++)
        {
        if (categories[i] == 7 || categories[i] == 14)
            {
            iLog->Log(_L("Wrong number of categories"));
            User::Leave(-1);
            }
        }
    CleanupStack::PopAndDestroy(&categories);
    CleanupStack::PopAndDestroy(lm);
    CleanupStack::PopAndDestroy(categoryManager);
    CleanupStack::PopAndDestroy(lmd);

    }

// ---------------------------------------------------------
// CPosTp10::BackupTp10DbFileL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CPosTp10::BackupTp10DbFileL()
    {

    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);
    manager->SetDefaultDatabaseUriL(KTp10TestDb);
    HBufC* defaultDbUri = manager->DefaultDatabaseUriLC();
    TBuf<255> buf;
    // Extract URI except characters ".ldb"
    buf.Append(defaultDbUri->Left(defaultDbUri->Length()-4));
    buf.Append(_L("COPY.LDB"));
    // Delete in case it already exist
    TRAPD(err, manager->DeleteDatabaseL(buf));
    if (err != KErrNone && err != KErrNotFound) iLog->Log(_L("Could not delete file"));
    manager->CopyDatabaseL(*defaultDbUri, buf);

    CleanupStack::PopAndDestroy(defaultDbUri);
    CleanupStack::PopAndDestroy(manager);
    return CPosLandmarkDatabase::OpenL();
    }

// ---------------------------------------------------------
// CPosTp10::RestoreTp10DbFileL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CPosTp10::RestoreTp10DbFileL()
    {


    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);
    manager->SetDefaultDatabaseUriL(KTp10TestDb);
    HBufC* defaultDbUri = manager->DefaultDatabaseUriLC();
    TBuf<255> buf;
    // Extract URI except characters ".ldb"
    buf.Append(defaultDbUri->Left(defaultDbUri->Length()-4));
    buf.Append(_L("COPY.LDB"));
    // Delete in case it already exist
    TRAPD(err, manager->DeleteDatabaseL(*defaultDbUri));
    if (err != KErrNone && err != KErrNotFound) iLog->Log(_L("Could not delete file"));
    manager->CopyDatabaseL(buf, *defaultDbUri);

    CleanupStack::PopAndDestroy(defaultDbUri);
    CleanupStack::PopAndDestroy(manager);
    return CPosLandmarkDatabase::OpenL();
    }

//  End of File
