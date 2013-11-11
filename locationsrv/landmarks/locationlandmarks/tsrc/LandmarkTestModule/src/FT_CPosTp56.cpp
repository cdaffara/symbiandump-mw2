/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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

#include "FT_CPosTp56.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkEncoder.h>

// CONSTANTS  

_LIT(KDbUri1, "file://c:eposlm.ldb");
_LIT(KDbUri2, "c:eposlm.ldb");
_LIT(KDbUri3, "eposlm.ldb");
_LIT(KDbUri4, "file://eposlm.ldb");
_LIT(KDbUri5, "testlm.ldb");

// Will need to be changed to the secure dbms path when security is implemented
_LIT(KDbUri6, "c:\\system\\data\\eposlm.ldb"); 
// Will need to be changed to the secure dbms path when security is implemented
_LIT(KDbUri7, "file://c:\\system\\data\\eposlm.ldb"); 

_LIT(KDbUri8, "eposlm");
_LIT(KDbUri9, "c:eposlm");
_LIT(KDbUri10, "file://c:eposlm");

_LIT(KDbUri11, "file://c:XXXX.ldb");
_LIT(KDbUri12, "http://c:eposlm.ldb");
_LIT(KDbUri13, "");

_LIT(KDbUri14, "file://\\eposlm.ldb"); // UNC
_LIT(KDbUri15, "\\eposlm.ldb");


#ifdef __WINS__    
    _LIT(KTestDb, "z:\\system\\test\\TestData\\testlm.ldb");
#else
    _LIT(KTestDb, "c:\\system\\test\\TestData\\testlm.ldb");
#endif

 // Will need to be changed to the secure dbms path when security is implemented
 _LIT(KLocalDbLocation, "c:\\system\\data\\");

_LIT(KExportFileFullPath, "c:\\TP56-LandmarksFromThreeDatabases.xml");
_LIT(KExportFile, "TP56-LandmarksFromThreeDatabases.xml");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp56::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp56::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP56 - Open specific db and multiple db export");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp56::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp56::StartL()
    {
    RemoveDefaultDbL();
    
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    delete lmd;

    TestOpenSpecificDbL();

    TestMultipleExportL();

    TestLockedDatabaseL();
	}

// ---------------------------------------------------------
// CPosTp56::TestMultipleExportL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp56::TestMultipleExportL()
    {
    _LIT(KDb1, "eposlm1.ldb");
    _LIT(KDb2, "eposlm2.ldb");
    _LIT(KDb3, "eposlm3.ldb");
    
    CreateDatabasesL();

    CPosLandmarkDatabase* lmd1 = CPosLandmarkDatabase::OpenL(KDb1);
    CleanupStack::PushL(lmd1);
    
    CPosLandmarkDatabase* lmd2 = CPosLandmarkDatabase::OpenL(KDb2);
    CleanupStack::PushL(lmd2);

    CPosLandmarkDatabase* lmd3 = CPosLandmarkDatabase::OpenL(KDb3);
    CleanupStack::PushL(lmd3);

    CPosLandmark* lm = CreateXMLLandmarkLC(_L("TP56 - LM with all fields set"));
    AddAllXMLFieldsL(lm);    

    lmd1->AddLandmarkL(*lm);
    CleanupStack::PopAndDestroy(lm);  
    
    CPosLandmark* lm2 = CreateXMLLandmarkLC(_L("TP56 - LM2"));
    lmd2->AddLandmarkL(*lm2);
    CleanupStack::PopAndDestroy(lm2);
    
    
    CPosLandmark* lm3 = CreateXMLLandmarkLC(_L("TP56 - LM3"));
    
    CPosLmCategoryManager* cm = CPosLmCategoryManager::NewL(*lmd3);
    CleanupStack::PushL(cm);
    
    CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
    category->SetCategoryNameL(_L("category"));
    TPosLmItemId id = cm->AddCategoryL(*category);
    CleanupStack::PopAndDestroy(2, cm);
    
    lm3->AddCategoryL(id);
    lmd3->AddLandmarkL(*lm3);
    CleanupStack::PopAndDestroy(lm3);
    
    CPosLandmarkEncoder* encoder = CPosLandmarkEncoder::NewL(KMimeType);
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
// CPosTp56::CreateDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp56::CreateDatabasesL()
    {
    // Paths will need to be changed to the secure dbms path when security is implemented
   
    _LIT(KDefaultDbPath, "c:\\system\\data\\eposlm.ldb");

    _LIT(KDb1Path, "c:\\system\\data\\eposlm1.ldb");
    _LIT(KDb2Path, "c:\\system\\data\\eposlm2.ldb");
    _LIT(KDb3Path, "c:\\system\\data\\eposlm3.ldb");

    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);

    if (lmd->IsInitializingNeeded())
        {
        ExecuteAndDeleteLD(lmd->InitializeL()); 
        }
    
    CleanupStack::PopAndDestroy(lmd);  

    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
   
    fs.Delete(KExportFile);

    CFileMan* fileMan = CFileMan::NewL(fs);
    CleanupStack::PushL(fileMan);

    User::LeaveIfError(fileMan->Copy(KDefaultDbPath, KDb1Path, CFileMan::EOverWrite));    
    User::LeaveIfError(fileMan->Copy(KDefaultDbPath, KDb2Path, CFileMan::EOverWrite));    
    User::LeaveIfError(fileMan->Copy(KDefaultDbPath, KDb3Path, CFileMan::EOverWrite));  


    CleanupStack::PopAndDestroy(2, &fs);  
    }

// ---------------------------------------------------------
// CPosTp56::TestOpenSpecificDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp56::TestOpenSpecificDbL()
    {
    _LIT(KHttpErr, "http is not a supported protocol");
    _LIT(KURIErr, "Not a valid URI");
    _LIT(KArgumentErr, "An empty descr is not a valid URI");

    TestOpenDbL(KDbUri1);
    TestOpenDbL(KDbUri2);
    TestOpenDbL(KDbUri3);
    TestOpenDbL(KDbUri4);
    
    CopyTestDbL();
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
    }

// ---------------------------------------------------------
// CPosTp56::TestOpenDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp56::TestOpenDbL(const TDesC& aDbURI)
    {
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL(aDbURI);
    CleanupStack::PushL(lmd);
    
    if (lmd->IsInitializingNeeded())
        {
        ExecuteAndDeleteLD(lmd->InitializeL()); 
        }
 
    CleanupStack::PopAndDestroy(lmd);  
    }

//---------------------------------------------------------
// CPosTp56::CopTestDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp56::CopyTestDbL()
    {
        
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);

    CFileMan* fileMan = CFileMan::NewL(fs);
    CleanupStack::PushL(fileMan);

    User::LeaveIfError(fileMan->Copy(KTestDb, KLocalDbLocation, CFileMan::EOverWrite));     
    
    CleanupStack::PopAndDestroy(2, &fs);  

    }

// ---------------------------------------------------------
// CPosTp56::TestLockedDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp56::TestLockedDatabaseL()
    {
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