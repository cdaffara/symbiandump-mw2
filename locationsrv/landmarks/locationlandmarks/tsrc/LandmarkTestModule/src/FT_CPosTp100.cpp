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

#include "FT_CPosTp100.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <EPos_CPosLmDatabaseManager.h>
#include "FT_LandmarkConstants.h"


// CONSTANTS  

_LIT(KDb1, "eposlm1.ldb");
_LIT(KDb2, "eposlm2.ldb");
_LIT(KDb3, "eposlm3.ldb");

_LIT(KDbUri1, "file://c:eposlm.ldb");
_LIT(KDbUri2, "c:eposlm.ldb");
_LIT(KDbUri3, "eposlm.ldb");
_LIT(KDbUri4, "file://eposlm.ldb");
_LIT(KDbUri5, "testlm.ldb");

// Use hardcoded path here
_LIT(KDbUri6, "c:\\private\\100012a5\\DBS_101FE978_eposlm.ldb"); 
// Use hardcoded path here
_LIT(KDbUri7, "file://c:\\private\\100012a5\\DBS_101FE978_eposlm.ldb"); 

_LIT(KDbUri8, "eposlm");
_LIT(KDbUri9, "c:eposlm");
_LIT(KDbUri10, "file://c:eposlm");

_LIT(KDbUri11, "file://c:XXXX.ldb");
_LIT(KDbUri12, "http://c:eposlm.ldb");
_LIT(KDbUri13, "");

_LIT(KDbUri14, "file://\\eposlm.ldb"); // UNC
_LIT(KDbUri15, "\\eposlm.ldb");

// File on media that does not exist
_LIT(KDbUri16, "file://R:eposlm.ldb");

_LIT(KTestDb, "testlm.ldb");

// Will need to be changed to the secure dbms path when security is implemented


_LIT(KExportFileFullPath, "c:\\TP100-LandmarksFromThreeDatabases.xml");
_LIT(KExportFile, "TP100-LandmarksFromThreeDatabases.xml");

// ================= MEMBER FUNCTIONS =======================



// ---------------------------------------------------------
// CPosTp100::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp100::StartL()
    {
    RemoveDefaultDbL();
    RemoveAllLmDatabasesL();
        
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);
    
    HPosLmDatabaseInfo* dbInfo = HPosLmDatabaseInfo::NewLC(KDefaultDb);
	manager->CreateDatabaseL(*dbInfo);
	CleanupStack::PopAndDestroy(dbInfo);
    // This db must exist
    manager->SetDefaultDatabaseUriL(KDefaultDb);
    
    
    CleanupStack::PopAndDestroy(manager);
    
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    delete lmd;
    
    TestOpenSpecificDbL();

    TestMultipleExportL();
    
    TestLockedDatabaseL();
	}

// ---------------------------------------------------------
// CPosTp100::TestMultipleExportL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp100::TestMultipleExportL()
    {
    iLog->Log(_L("TestMultipleExportL()"));
    CreateDatabasesL();
    iLog->Log(_L("1"));

    CPosLandmarkDatabase* lmd1 = CPosLandmarkDatabase::OpenL(KDb1);
    CleanupStack::PushL(lmd1);
    iLog->Log(_L("2"));
    CPosLandmarkDatabase* lmd2 = CPosLandmarkDatabase::OpenL(KDb2);
    CleanupStack::PushL(lmd2);
    iLog->Log(_L("3"));
    CPosLandmarkDatabase* lmd3 = CPosLandmarkDatabase::OpenL(KDb3);
    CleanupStack::PushL(lmd3);
    ExecuteAndDeleteLD(lmd3->CompactL());
    iLog->Log(_L("4"));
    CPosLandmark* lm = CreateXMLLandmarkLC(_L("TP100 - LM with all fields set"));
    AddAllXMLFieldsL(lm);    
    iLog->Log(_L("5"));
    lmd1->AddLandmarkL(*lm);
    CleanupStack::PopAndDestroy(lm);  
    iLog->Log(_L("6"));
    CPosLandmark* lm2 = CreateXMLLandmarkLC(_L("TP100 - LM2"));
    lmd2->AddLandmarkL(*lm2);
    CleanupStack::PopAndDestroy(lm2);
    iLog->Log(_L("7"));
    CPosLandmark* lm3 = CreateXMLLandmarkLC(_L("TP100 - LM3"));
    iLog->Log(_L("8"));
    CPosLmCategoryManager* cm = CPosLmCategoryManager::NewL(*lmd3);
    CleanupStack::PushL(cm);
    iLog->Log(_L("9"));
    CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
    category->SetCategoryNameL(_L("category"));

    TPosLmItemId id = cm->AddCategoryL(*category);
    CleanupStack::PopAndDestroy(2, cm);
    iLog->Log(_L("10"));
    lm3->AddCategoryL(id);
    iLog->Log(_L("11"));
    
    iLog->Log(_L("12"));
    lmd3->AddLandmarkL(*lm3);
    CleanupStack::PopAndDestroy(lm3);
    iLog->Log(_L("13"));
    
    CPosLandmarkEncoder* encoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    CleanupStack::PushL(encoder);

    encoder->SetOutputFileL(KExportFileFullPath);
    
    CPosLmItemIterator* iter = lmd1->LandmarkIteratorL();
    CleanupStack::PushL(iter);
    
    RArray<TPosLmItemId> arrayOfIds;
    CleanupClosePushL(arrayOfIds);
    
    TInt numberOfElements = iter->NumOfItemsL();    
    iter->GetItemIdsL(arrayOfIds, 0, (numberOfElements));

    ExecuteAndDeleteLD(lmd1->ExportLandmarksL(*encoder, arrayOfIds, CPosLandmarkDatabase::EDefaultOptions));

    RunAsyncOperationLD(lmd2->ExportLandmarksL(*encoder, arrayOfIds, CPosLandmarkDatabase::EDefaultOptions));

    ExecuteAndDeleteLD(lmd3->ExportLandmarksL(*encoder, arrayOfIds, CPosLandmarkDatabase::EIncludeCategories));

    ExecuteAndDeleteLD(encoder->FinalizeEncodingL()); 
    
    CleanupStack::PopAndDestroy(6, lmd1);

    TInt err = CompareL(KExportFile);
    AssertTrueSecL(err == KErrNone, _L("The exported file is not equal to file exported prevoius release"));
    }

// ---------------------------------------------------------
// CPosTp100::CreateDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp100::CreateDatabasesL()
    {
    iLog->Log(_L("CreateDatabasesL()"));

    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);

    if (lmd->IsInitializingNeeded())
        {
        ExecuteAndDeleteLD(lmd->InitializeL()); 
        }
    
    CleanupStack::PopAndDestroy(lmd);  

    TInt res = iFileSession.Delete(KExportFileFullPath);
    if (res != KErrNone && res != KErrNotFound) 
    {
        iLog->Log(_L("File could not be deleted"));
        User::Leave(res);

    }

    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);
    
    TBuf<150> srcPath;
    TBuf<150> destPath;
    
    srcPath.Append(KLmDbPath);
    srcPath.Append(KDefaultDb);
    
    // Copy file eposlm.ldb to three databases
    destPath.Append(KLmDbPath);
    destPath.Append(KDb1);
    User::LeaveIfError(fileMan->Copy(srcPath, destPath, CFileMan::EOverWrite));
    
    destPath.Zero();
    destPath.Append(KLmDbPath);
    destPath.Append(KDb2);
    User::LeaveIfError(fileMan->Copy(srcPath, destPath, CFileMan::EOverWrite));
    
    destPath.Zero();
    destPath.Append(KLmDbPath);
    destPath.Append(KDb3);
    User::LeaveIfError(fileMan->Copy(srcPath, destPath, CFileMan::EOverWrite));

    CleanupStack::PopAndDestroy(fileMan);  
    }

// ---------------------------------------------------------
// CPosTp100::TestOpenSpecificDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp100::TestOpenSpecificDbL()
    {
    _LIT(KHttpErr, "http is not a supported protocol");
    _LIT(KURIErr, "Not a valid URI");
    _LIT(KArgumentErr, "An empty descr is not a valid URI");
    
    iLog->Log(_L("TestOpenSpecificDbL()"));

    TestOpenDbL(KDbUri1);
    TestOpenDbL(KDbUri2);
    TestOpenDbL(KDbUri3);
    TestOpenDbL(KDbUri4);
    
    // Copy to secure area
    CopyTestDbFileL(KTestDb);
    
    TestOpenDbL(KDbUri5);
    
    TRAPD(err, TestOpenDbL(KDbUri6));
    AssertTrueSecL(err == KErrArgument, KURIErr);

    TRAP(err, TestOpenDbL(KDbUri7));
    AssertTrueSecL(err == KErrArgument, KURIErr);

    TRAP(err, TestOpenDbL(KDbUri8));
    AssertTrueSecL(err == KErrArgument, KURIErr);

    TRAP(err, TestOpenDbL(KDbUri9));
    AssertTrueSecL(err == KErrArgument, KURIErr);

    TRAP(err, TestOpenDbL(KDbUri10));
    AssertTrueSecL(err == KErrArgument, KURIErr);

    TRAP(err, TestOpenDbL(KDbUri11));
    AssertTrueSecL(err == KErrNotFound, KURIErr);

    TRAP(err, TestOpenDbL(KDbUri12));
    AssertTrueSecL(err == KErrNotSupported, KHttpErr);

    TRAP(err, TestOpenDbL(KDbUri13))
    AssertTrueSecL(err == KErrArgument, KArgumentErr);

    TRAP(err, TestOpenDbL(KDbUri14));
    AssertTrueSecL(err == KErrArgument, KURIErr);
    
    TRAP(err, TestOpenDbL(KDbUri15));
    AssertTrueSecL(err == KErrArgument, KURIErr);
    
    TRAP(err, TestOpenDbL(KDbUri16));
    AssertTrueSecL(err == KErrNotReady, KURIErr);
    }

// ---------------------------------------------------------
// CPosTp100::TestOpenDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp100::TestOpenDbL(const TDesC& aDbURI)
    {
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL(aDbURI);
    CleanupStack::PushL(lmd);
    
    if (lmd->IsInitializingNeeded())
        {
        ExecuteAndDeleteLD(lmd->InitializeL()); 
        }
 
    CleanupStack::PopAndDestroy(lmd);  
    }
    
// ---------------------------------------------------------
// CPosTp100::TestLockedDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp100::TestLockedDatabaseL()
    {
    iLog->Log(_L("TestLockedDatabaseL()"));
    
    CPosLandmarkDatabase* db1 = UseGeneratedDbFileL();
    CleanupStack::PushL(db1);

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
    CPosLandmarkDatabase* db2 = CPosLandmarkDatabase::OpenL(KDbUri1);
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
    TRAPD(err, db2 = CPosLandmarkDatabase::OpenL(KDbUri1));
    delete db2;
    User::WaitForRequest(status);
    AssertTrueSecL(err == KErrLocked, _L("OpenL() didn't result in KErrLocked but in %d"), err);
    
    CleanupStack::PopAndDestroy(3, db1);
    }    

//  End of File
