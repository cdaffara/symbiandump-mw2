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
#include "FT_CPosTp8.h"
#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLandmark.h>
#include <LbsPosition.h>
#include <EPos_CPosLMOperation.h> 
#include "FT_LandmarkConstants.h"

// CONSTANTS
const TInt KNoLandmarks = 100;
 
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp8::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp8::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    iLandmarks.ResetAndDestroy();
    iLandmarks.Close();    
    delete iDatabase;
    iDatabase=NULL;
    }

// ---------------------------------------------------------
// CPosTp8::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp8::StartL()
    {    
    _LIT(KLogFileName, "c:\\documents\\TP8TimeMeasurements.txt");
    
    TTime start, stop, totalStart, totalStop;
    totalStart.UniversalTime();
	start.UniversalTime();
    RemoveDefaultDbL();
    RemoveAllLmDatabasesL();
    
    _LIT(KTestPath, "c:\\system\\test\\testdata\\");
    // copy the test db to the private path
    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);

    TBuf<150> srcPath;

    srcPath.Append(KTestPath);
    srcPath.Append(KTp8TestDb);

    TInt err = fileMan->Copy(srcPath, KLmTp8DefaultDbPath,
            CFileMan::EOverWrite);
    if (err != KErrNone)
        iLog->Log(_L("Error when copying file"));

    CleanupStack::PopAndDestroy(fileMan);
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    // This db must exist
    manager->SetDefaultDatabaseUriL(KTp8TestDb);
    CleanupStack::PopAndDestroy(manager);

    start.UniversalTime();
    iDatabase= CPosLandmarkDatabase::OpenL(KTp8TestDb);
    stop.UniversalTime();
    
    TInt64 msec = (stop.Int64() - start.Int64())/1000;
	TBuf8<KMsgBufSize> msg;
	_LIT8(KTrace, "Opening (creating) an empty database = %d msec");
	msg.Format(KTrace, msec);
	LogToFileL(KLogFileName, msg);
    
    if (iDatabase->IsInitializingNeeded())
       {
       start.UniversalTime();
       ExecuteAndDeleteLD(iDatabase->InitializeL());
        
       stop.UniversalTime();
       msec = (stop.Int64() - start.Int64())/1000;
       _LIT8(KTrace2, "Initializing an empty database = %d msec");
	   msg.Format(KTrace2, msec);
	   LogToFileL(KLogFileName, msg);
       }

// Add landmarks      
    start.UniversalTime();
    AddLandmarksL(iDatabase);
    stop.UniversalTime();    
 
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace3, "AddLandmarksL = %d msec");
	msg.Format(KTrace3, msec);
	LogToFileL(KLogFileName, msg);
        
    delete iDatabase;
    iDatabase=NULL;   
    iDatabase = BackupTp8DbFileL();

if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
       
// Read landmarks    
    start.UniversalTime();
    ReadLandmarksL(iDatabase);
    stop.UniversalTime();    
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace4, "ReadLandmarksL = %d msec");
	msg.Format(KTrace4, msec);
	LogToFileL(KLogFileName, msg);

// Update landmarks    
    start.UniversalTime();
    UpdateLandmarksL(iDatabase);
    stop.UniversalTime();   
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace5, "UpdateLandmarksL = %d msec");
	msg.Format(KTrace5, msec);
	LogToFileL(KLogFileName, msg);
    
    ReadLandmarksL(iDatabase);
    
// Remove landmarks one by one
    start.UniversalTime();
    RemoveLandmarksL(iDatabase);
    stop.UniversalTime();   
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace6, "RemoveLandmarksL = %d msec");
	msg.Format(KTrace6, msec);
	LogToFileL(KLogFileName, msg);

    // Restore database
    delete iDatabase;
    iDatabase=NULL;
    iDatabase=RestoreTp8DbFileL();
        
        if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

// Remove set of landmarks synchronously
    start.UniversalTime();
    RemoveSetOfLandmarksL(iDatabase);
    stop.UniversalTime();   
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace7, "RemoveSetOfLandmarksL = %d msec");
	msg.Format(KTrace7, msec);
	LogToFileL(KLogFileName, msg);
    
    // Restore database
    delete iDatabase;
    iDatabase=NULL;
    iDatabase=RestoreTp8DbFileL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

// Remove set of landmarks and remove all landmarks asynchronously
    start.UniversalTime();
    RemoveLandmarksAsyncL(iDatabase);
    stop.UniversalTime();   
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace8, "RemoveLandmarksAsyncL = %d msec");
	msg.Format(KTrace8, msec);
	LogToFileL(KLogFileName, msg);
	
    // Restore database
    delete iDatabase;
    iDatabase=NULL;
    iDatabase=RestoreTp8DbFileL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
     
// Remove set of landmarks and remove all landmarks with User::WaitForRequest()
    start.UniversalTime();
    RemoveLandmarksAsyncWithWaitForReqL(iDatabase);
    stop.UniversalTime();   
    
    msec = (stop.Int64() - start.Int64())/1000;
    _LIT8(KTrace9, "RemoveLandmarksAsyncWithWaitForReqL = %d msec");
	msg.Format(KTrace9, msec);
	LogToFileL(KLogFileName, msg);

    // Restore database
    delete iDatabase;
    iDatabase=NULL;
    iDatabase=RestoreTp8DbFileL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
    
// Remove set of landmarks asynchronously and cancel
    iLog->Log(_L("TESTS BELOW ARE TESTING CANCEL"));
    RemoveLandmarksAsyncAndCancelL();
    
    // Check maximum length of Landmarkname
    CheckLandmarkNameMaxLengthL();
	
    totalStop.UniversalTime();   
    msec = (totalStop.Int64() - totalStart.Int64())/1000;
    _LIT8(KTrace10, "Complete test case = %d msec");
	msg.Format(KTrace10, msec);
	LogToFileL(KLogFileName, msg);
    }

// ---------------------------------------------------------
// CPosTp8::AddLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp8::AddLandmarksL(CPosLandmarkDatabase* aDatabase)
    {
    iLog->Log(_L("Adding and copying landmarks"));
    
    _LIT(KCategoryAddedErr, "A category that doesn't exist in the database should not be added to the landmark");    
    _LIT(KLmName, "LmTP8 - %d");
    _LIT(KLmDescription, "LmTP8Description - %d");
    
    iLandmarks.ResetAndDestroy();
    for (TInt i=0; i<(KNoLandmarks/4); i++)
        {
        TBuf<100> lmName;
        TBuf<100> lmDesc;
        lmName.Format(KLmName,i);
        CPosLandmark* landmark = CPosLandmark::NewLC();
        landmark->SetLandmarkNameL(lmName);
        lmDesc.Format(KLmDescription,i);
        landmark->SetLandmarkDescriptionL(lmDesc);

        CPosLandmark* copy = CPosLandmark::NewLC(*landmark);
      
        CPosLandmark* landmark2 = CPosLandmark::NewL();
        CleanupStack::PushL(landmark2);    
        landmark2->SetLandmarkNameL(lmName); 
        landmark2->SetLandmarkDescriptionL(lmDesc); 

        CPosLandmark* copy2 = CPosLandmark::NewL(*landmark2);
        CleanupStack::PushL(copy2);
        TInt err = KErrLocked;
        
        ExecuteAndDeleteLD(aDatabase->CompactL());
        
        while (err == KErrLocked)
            {
            TRAP(err, aDatabase->AddLandmarkL(*landmark));
            }
        err = KErrLocked;
        while (err == KErrLocked)
            {
            TRAP(err, aDatabase->AddLandmarkL(*copy));
            }
        err = KErrLocked;
        while (err == KErrLocked)
            {
            TRAP(err, aDatabase->AddLandmarkL(*landmark2));
            }
        err = KErrLocked;
        while (err == KErrLocked)
            {
            TRAP(err, aDatabase->AddLandmarkL(*copy2));
            }
      
        TPosLmItemId id2 = 5123321;
        CPosLandmark* landmark3 = CPosLandmark::NewLC();
        landmark3->AddCategoryL(id2);
        landmark3->AddCategoryL(id2);
        
        err = KErrLocked;
        while (err == KErrLocked)
            {
            TRAP(err, aDatabase->AddLandmarkL(*landmark3)); // CR115 this should not fail any more 
            }
        
        CPosLandmark* readLm3 = aDatabase->ReadLandmarkLC(landmark3->LandmarkId());
        
        RArray<TPosLmItemId> catIds;
        CleanupClosePushL(catIds);
        readLm3->GetCategoriesL(catIds);
        AssertTrueSecL(catIds.Count() == 0, KCategoryAddedErr); 
        CleanupStack::PopAndDestroy(3, landmark3);

        iLandmarks.Append(landmark);
        iLandmarks.Append(copy);
        iLandmarks.Append(landmark2);
        iLandmarks.Append(copy2);
            
        CleanupStack::Pop(4, landmark); 
        }   
    }

// ---------------------------------------------------------
// CPosTp8::ReadLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp8::ReadLandmarksL(CPosLandmarkDatabase* aDatabase)
    {
    iLog->Log(_L("Reading landmarks"));

    _LIT(KReadNotFoundErr, "The landmark id doesn't exist so ReadLandmark should return not found");    
    TBuf<100> buf;
    buf.Format(_L("Reading %d nr of landmarks from id %d to id %d"), iLandmarks.Count(), 
    				iLandmarks[0]->LandmarkId(), iLandmarks[iLandmarks.Count()-1]->LandmarkId());
    iLog->Log(buf);
    for (TInt i = 0; i<iLandmarks.Count(); i++)
        {
        
        CPosLandmark* lm = aDatabase->ReadLandmarkLC(iLandmarks[i]->LandmarkId());
        CompareLandmarksL(*iLandmarks[i], *lm);    
        CleanupStack::PopAndDestroy(); // lm 
        }
    
    TPosLmItemId id = 5123321;
    CPosLandmark* lm=NULL;
    TRAPD(err, lm=aDatabase->ReadLandmarkLC(id));
    delete lm;
    lm = NULL;
    AssertTrueSecL(err == KErrNotFound, KReadNotFoundErr);
    
    id = 13434;
    lm=NULL;
    TRAP(err, lm=aDatabase->ReadLandmarkLC(id));
    delete lm;
    AssertTrueSecL(err == KErrNotFound, KReadNotFoundErr);
    
    id = 0;
    lm=NULL;
    TRAP(err, lm=aDatabase->ReadLandmarkLC(id));
    delete lm;
    AssertTrueSecL(err == KErrNotFound, KReadNotFoundErr);
    
    }


// ---------------------------------------------------------
// CPosTp8::UpdateLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp8::UpdateLandmarksL(CPosLandmarkDatabase* aDatabase)    
    {
    iLog->Log(_L("Updating landmarks"));
    _LIT(KCategoryName, "UpdatedCategory %d");
    _LIT(KCategoryName2, "UpdatedCategory2:%d:%d");
    _LIT(KCategoryAddedErr, "A category that doesn't exist in the database should not be added to the landmark"); 
    
    for (TInt i = 0; i<iLandmarks.Count(); i++)
        {
        TCoordinate coord(KLatitude2, KLongitude2, KAltitude2);
        TLocality loc(coord, KHorizontalAcc2, KVerticalAcc2);
        TInt j = 0;
        switch(i)
            {
            case 0:
                iLandmarks[i]->SetLandmarkNameL(KName2);
                break;
            case 1:        
                iLandmarks[i]->SetPositionL(loc);
                break;
            case 2:
                iLandmarks[i]->SetIconL(KMbmFileName2, KIconIndex2, KPosLmIconMaskNotUsed);
                break;
            case 3:
                iLandmarks[i]->SetLandmarkDescriptionL(KDescription2);
                break;
            case 4:
                iLandmarks[i]->SetCoverageRadius(KCoverageRadius); 
                break;
            case 5:
                AddFields2L(iLandmarks[i]);
                break;
            case 6:
                for(j=0; j<KNoCategories; j++)
                    {
                    TBuf<100> name;
                    name.Format(KCategoryName, j);
                    iLandmarks[i]->AddCategoryL(CreateCategoryL(name));
                    }
                break;
            case 8:
                AddFields3L(iLandmarks[i]);
                break;
            case 9:
                break;
            default:
                iLandmarks[i]->SetLandmarkNameL(KName2);
                iLandmarks[i]->SetPositionL(loc);
                iLandmarks[i]->SetIconL(KMbmFileName2, KIconIndex2, KPosLmIconMaskNotUsed);
                iLandmarks[i]->SetLandmarkDescriptionL(KDescription2);
                iLandmarks[i]->SetCoverageRadius(KCoverageRadius); 
                AddFields2L(iLandmarks[i]);
               
                for(j=0; j<KNoCategories; j++)
                    {
                    TBuf<100> name;
                    name.Format(KCategoryName2, j, i);
                    iLandmarks[i]->AddCategoryL(CreateCategoryL(name));
                    }
                break;
            } 
        }
    TInt err = KErrLocked;
    for (TInt k = 0; k<iLandmarks.Count(); k++)
        {
        err = KErrLocked;
        while (err == KErrLocked)
            {
            TRAP(err, aDatabase->UpdateLandmarkL(*iLandmarks[k]));
            }
        }

    ExecuteAndDeleteLD(aDatabase->CompactL());
             
    _LIT(KTemp, "temp");
    TPosLmItemId id2 = 8787876;
    CPosLandmark* landmark = CreateLandmarkLC(KTemp);
    err = KErrLocked;
    while (err == KErrLocked)
        {
        TRAP(err, aDatabase->AddLandmarkL(*landmark));
        }
    
    landmark->AddCategoryL(id2);
    
    // 0 is equal to KPosLmNullItemId, should leave
    id2 = 0;
    TRAP(err, landmark->AddCategoryL(id2));
    AssertTrueSecL(err == KErrArgument, _L("AddCategory(0) should leave with KErrArgument"));
    err = KErrLocked;
    while (err == KErrLocked)
        {  
        TRAP(err, aDatabase->UpdateLandmarkL(*landmark)); // CR115 this should not fail any more 
        }
    
    // Make sure category not added to lm (TR ESLI-5X7FTZ)
    CPosLandmark* readLm = aDatabase->ReadLandmarkLC(landmark->LandmarkId());
    RArray<TPosLmItemId> catIds;
    CleanupClosePushL(catIds);
    readLm->GetCategoriesL(catIds);
    AssertTrueSecL(catIds.Count() == 3, KCategoryAddedErr); 
    
    aDatabase->RemoveLandmarkL(landmark->LandmarkId());
      
    CleanupStack::PopAndDestroy(3, landmark);
    }

// ---------------------------------------------------------
// CPosTp8::RemoveLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp8::RemoveLandmarksL(CPosLandmarkDatabase* aDatabase)
    {
    iLog->Log(_L("Removing landmarks"));
    ExecuteAndDeleteLD(aDatabase->CompactL());   
    const TInt start=9;
    const TInt end=55;
    
    // First check invalid landmark ids, nothing should happen
    CheckRemovedL(iLandmarks.Count(), iLandmarks.Count());
    TPosLmItemId id = 5123321;
    TInt err = KErrLocked;
    while (err == KErrLocked)
        {
        TRAP(err, aDatabase->RemoveLandmarkL(id));
        }
        
    id = 11233;
    err = KErrLocked;
    while (err == KErrLocked)
        {
        TRAP(err, aDatabase->RemoveLandmarkL(id));
        }
    id = 0;
    err = KErrLocked;
    while (err == KErrLocked)
        {
        TRAP(err, aDatabase->RemoveLandmarkL(id));
        }
    
    CheckRemovedL(iLandmarks.Count(), iLandmarks.Count());

    for (TInt i = start; i<=end; i++)
        {
        err = KErrLocked;
        while (err == KErrLocked)
            {
            TRAP(err, aDatabase->RemoveLandmarkL(iLandmarks[i]->LandmarkId()));
            }
        }
    CheckRemovedL(start, end);
    for (TInt j = start; j<=end; j++)
        {
        err = KErrLocked;
        while (err == KErrLocked)
            {
            TRAP(err, aDatabase->RemoveLandmarkL(iLandmarks[j]->LandmarkId())); // nothing should happen 
            }
        } 
    
    CheckRemovedL(start, end);
    
    iLog->Log(_L("Removing all landmarks"));
    ExecuteAndDeleteLD(aDatabase->CompactL());
    ExecuteAndDeleteLD(aDatabase->RemoveAllLandmarksL());
    
    CheckRemovedL(0, iLandmarks.Count()-1);

    ExecuteAndDeleteLD(aDatabase->RemoveAllLandmarksL());
    
    CheckRemovedL(0, iLandmarks.Count()-1);
    
    }

// ---------------------------------------------------------
// CPosTp8::RemoveLandmarksAsyncL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp8::RemoveLandmarksAsyncL(CPosLandmarkDatabase* aDatabase)
    {
    iLog->Log(_L("Removing landmarks asynchronously"));
    
    // Remove a set of landmarks async
    const TInt start=3;
    const TInt end=47;
    
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);
    
    for(TInt i=start; i<=end; i++)
        {
        ids.Append(iLandmarks[i]->LandmarkId());
        }
    
    RunAsyncOperationLD(aDatabase->RemoveLandmarksL(ids));
    CleanupStack::PopAndDestroy(&ids);

    CheckRemovedL(start, end);    
    
    // Remove all async
    delete iDatabase;
    iDatabase=NULL;
    iDatabase=RestoreTp8DbFileL();
    
        if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
   
    iLog->Log(_L("Removing all landmarks asynchronously"));

    RunAsyncOperationLD(iDatabase->RemoveAllLandmarksL());
    
    CheckRemovedL(0, iLandmarks.Count()-1);
    }

// ---------------------------------------------------------
// CPosTp8::RemoveLandmarksAsyncWithWaitForReqL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp8::RemoveLandmarksAsyncWithWaitForReqL(CPosLandmarkDatabase* aDatabase)
    {
    iLog->Log(_L("Removing landmarks asynchronously with User::WaitForRequest"));
    
    // Remove a set of landmarks async with WaitForReq
    const TInt start=3;
    const TInt end=47;
    
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);
    
    for(TInt i=start; i<=end; i++)
        {
        ids.Append(iLandmarks[i]->LandmarkId());
        }
    
    CPosLmOperation* op = aDatabase->RemoveLandmarksL(ids);
    CleanupStack::PushL(op);
    RunAsyncOperationByWaitForReqL(op);
    CleanupStack::PopAndDestroy(op);

    CheckRemovedL(start, end);
    
    // Remove all async with WaitForReq
    delete iDatabase;
    iDatabase=NULL;
    iDatabase=RestoreTp8DbFileL();
   
       
        if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
    
    iLog->Log(_L("Removing all landmarks asynchronously with User::WaitForRequest"));
    TRAPD(error,op = iDatabase->RemoveAllLandmarksL());
    iLog->Log(_L("error while RemoveAllLandmarksL = %d"),error);
    CleanupStack::PushL(op);
    RunAsyncOperationByWaitForReqL(op);
    CleanupStack::PopAndDestroy(2, &ids);
    
    CheckRemovedL(0, iLandmarks.Count()-1);
    }

// ---------------------------------------------------------
// CPosTp8::RemoveLandmarksAsyncAndCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp8::RemoveLandmarksAsyncAndCancelL()
    {
    iLog->Log(_L("Removing landmarks asynchronously and cancel"));
    
    // Remove a set of landmarks async
    const TInt start=3;
    const TInt end=47;
    
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);
    
    for(TInt i=start; i<=end; i++)
        {
        ids.Append(iLandmarks[i]->LandmarkId());
        }
    
    TReal32 progress(0);
    while (progress < 1.0)
        {
        RunAsyncOperationAndCancelInCallbackLD(iDatabase->RemoveLandmarksL(ids), progress);
       
        delete iDatabase;
        iDatabase=NULL;
        iDatabase=RestoreTp8DbFileL();
         if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
        progress = progress + 0.09;
        }
    CleanupStack::PopAndDestroy(&ids);
   
    iLog->Log(_L("Removing all landmarks asynchronously and cancel"));

    progress = 0;
    while (progress < 1.0)
        {
        RunAsyncOperationAndCancelInCallbackLD(iDatabase->RemoveAllLandmarksL(), progress);
        
        delete iDatabase;
        iDatabase=NULL;
        iDatabase=RestoreTp8DbFileL();
         if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
        
        progress = progress + 0.09;
        }
    }

// ---------------------------------------------------------
// CPosTp8::RemoveSetOfLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp8::RemoveSetOfLandmarksL(CPosLandmarkDatabase* aDatabase)
    {
    iLog->Log(_L("Removing set of landmarks"));
   
    const TInt start=3;
    const TInt end=57;
    
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);
    
    // Test with empty list, should leave with KErrArgument
    TRAPD(err, iOperation = aDatabase->RemoveLandmarksL(ids));
    if (err != KErrArgument)
    {
    iLog->Log(_L("Removing empty iterator should leave with KErrArgument"));
   	User::Leave(err);
    
    }
    
    #if 0
    // Added for testing -- Start    
    iOperation->ExecuteL(); 
    // Added for testing -- End
    
    delete iOperation;
    iOperation = NULL;
    #endif 
    
    // Test with invalid sets of lm id arrays
    ids.Append(11234);
    ids.Append(556);
    ids.Append(908);
   
    ExecuteAndDeleteLD(aDatabase->RemoveLandmarksL(ids));
   
    // Check that no landmark has been removed 
    CheckRemovedL(iLandmarks.Count(), iLandmarks.Count());
    
    for(TInt i=start; i<=end; i++)
        {
        ids.Append(iLandmarks[i]->LandmarkId());
        // Also add invalid ids
        ids.Append(i+5000);
        }
    
    ExecuteAndDeleteLD(aDatabase->RemoveLandmarksL(ids));

    CheckRemovedL(start, end);

    ExecuteAndDeleteLD(aDatabase->RemoveLandmarksL(ids));
    
    CleanupStack::PopAndDestroy(&ids);

    CheckRemovedL(start, end);
    }

// ---------------------------------------------------------
// CPosTp8::CheckRemoved
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp8::CheckRemovedL(const TInt startIndex, const TInt stopIndex)
    {
    _LIT(KRemoveNotFoundErr, "Landmark has not been removed");
    _LIT(KRemoveFoundErr, "Landmark should not been removed");

       for (TInt i = 0; i<iLandmarks.Count(); i++)
        {
        TRAPD(err, ReadLandmarkL(iLandmarks[i]->LandmarkId()));
        if (i >= startIndex && i <= stopIndex)
            {    
            AssertTrueSecL(err == KErrNotFound, KRemoveNotFoundErr);               
            }
        else
            {
            AssertTrueSecL(err == KErrNone, KRemoveFoundErr);
            }
        }
    }

// ---------------------------------------------------------
// CPosTp8::ReadLandmarkL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp8::ReadLandmarkL(TPosLmItemId aCategoryId)
    {
    CPosLandmark* lm = iDatabase->ReadLandmarkLC(aCategoryId); 
    CleanupStack::PopAndDestroy(lm);
    }

//---------------------------------------------------------
// CPosTp8::CheckLandmarkNameMaxLengthL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp8::CheckLandmarkNameMaxLengthL()
    {
    _LIT(K255Error, "Landmark name is not limited to 255 characters");
    
    iLog->Log(_L("CheckLandmarkNameMaxLengthL"));

    const TInt KPosLmMaxTextFieldLength = 255;
    
    HBufC* hbuf = HBufC::NewLC(KPosLmMaxTextFieldLength+1);

    CPosLandmark* lm = CPosLandmark::NewLC();
    // Test empty name, is OK
    lm->SetLandmarkNameL(*hbuf);
 
    while(hbuf->Des().Length() < KPosLmMaxTextFieldLength)
        {
        hbuf->Des().Append(_L("X"));
        }

    lm->SetLandmarkNameL(*hbuf);
    
    hbuf->Des().Append(_L("X"));

    TRAPD(err, lm->SetLandmarkNameL(*hbuf));
    AssertTrueSecL(err == KErrArgument, K255Error);

    CleanupStack::PopAndDestroy(2, hbuf);
    }

// ---------------------------------------------------------
// CPosTp8::BackupTp8DbFileL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CPosTp8::BackupTp8DbFileL()
    {

    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);
    manager->SetDefaultDatabaseUriL(KTp8TestDb);
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
// CPosTp8::RestoreTp8DbFileL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CPosTp8::RestoreTp8DbFileL()
    {


    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);
    manager->SetDefaultDatabaseUriL(KTp8TestDb);
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
