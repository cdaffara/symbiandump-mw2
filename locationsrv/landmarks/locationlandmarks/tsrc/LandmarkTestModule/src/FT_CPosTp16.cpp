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

#include "FT_CPosTp16.h"
#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLandmarkSearch.h> 
#include <EPos_CPosLmTextCriteria.h>

#include "FT_LandmarkConstants.h"
           
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp16::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp16::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP16 - Initialize corrupt database");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp16::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp16::CloseTest()
    {
    delete iDatabase;
    iDatabase = NULL;

    iGlobalCategoryIds.Close();
    iGlobalCategoryNames.Close();
    RemoveGlobalCategoriesL();
    }

// ---------------------------------------------------------
// CPosTp16::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp16::StartL()
    {
    _LIT(KAccessErr, "Access method is not returning KErrPosLmNotInitialized when expected");
    _LIT(KInitializedErr, "Method InitializedNeeded is not returning ETrue when expected");
   
    SetupGlobalCategoriesL();

    iDatabase=UseCorruptDbL();
    delete iDatabase;
    iDatabase = NULL;
    
    iDatabase = UseGlobalCategoriesL();
    
    CPosLmItemIterator* iter = NULL; 
    TRAPD(err, iter = iDatabase->LandmarkIteratorL());
    delete iter;
    AssertTrueSecL(err == KErrPosLmNotInitialized, KAccessErr);
    AssertTrueSecL(iDatabase->IsInitializingNeeded(), KInitializedErr);

    ExecuteAndDeleteLD(iDatabase->InitializeL()); 
    
    iter = iDatabase->LandmarkIteratorL();
    delete iter;
    iter=NULL;

    // check that global categories are automatically inserter into lm db 
    // when recovering database
    CheckGlobalCategoriesL();

    delete iDatabase;
    iDatabase = NULL;

    iDatabase=UseCorruptDbL();
    TRAP(err, iter = iDatabase->LandmarkIteratorL());
    delete iter;
    AssertTrueSecL(err == KErrPosLmNotInitialized, KAccessErr);
    AssertTrueSecL(iDatabase->IsInitializingNeeded(), KInitializedErr);

    RunAsyncOperationLD(iDatabase->InitializeL());

    iter = iDatabase->LandmarkIteratorL();
    delete iter;
    
    // check that global categories are automatically inserter into lm db 
    // when recovering database
    CheckGlobalCategoriesL();

    delete iDatabase;
    iDatabase = NULL;
    
    iDatabase = UseTextDbL();

    ExecuteAndDeleteLD(iDatabase->InitializeL()); 
    
    iter = iDatabase->LandmarkIteratorL();
    delete iter;
    iter=NULL;

    // check that global categories are automatically inserter into lm db 
    // when recovering database
    CheckGlobalCategoriesL();
    
    delete iDatabase;
    iDatabase = NULL;
    
    iDatabase = UseContactDbL();
    
    ExecuteAndDeleteLD(iDatabase->InitializeL()); 
    
    iter = iDatabase->LandmarkIteratorL();
    delete iter;
    iter=NULL;

    // check that global categories are automatically inserter into lm db 
    // when recovering database
    CheckGlobalCategoriesL();

    delete iDatabase;
    iDatabase = NULL;
	}


// ---------------------------------------------------------
// CPosTp16::CheckGlobalCategoriesL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp16::CheckGlobalCategoriesL()
    {
    iLog->Put(_L("CheckGlobalCategoriesL"));
  
    CPosLmCategoryManager* cm = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(cm);

    iLog->Put(_L("using CategoryIteratorL"));
    CPosLmItemIterator* catIter = cm->CategoryIteratorL();
    CleanupStack::PushL(catIter);

    AssertTrueSecL(catIter->NumOfItemsL() == (TUint)iGlobalCategoryIds.Count(), _L("Wrong number of global categories"));

    TPosLmItemId id = catIter->NextL();
    TInt i=0;
    while (id != KPosLmNullItemId)
        {       
/*
        // For debugging purposes
        TBuf<100> buf;
        buf.Format(_L("Id %d Exp Id %d"),id, iGlobalCategoryIds[i]);
        iLog->Put(buf);
        if (id != iGlobalCategoryIds[i]) LogErrorAndLeave(_L("Wrong id"));
        else iLog->Put(_L("++"));
*/
        id = catIter->NextL();
        i++;
        }
    CleanupStack::PopAndDestroy(catIter);
    CleanupStack::PopAndDestroy(cm);

    }

// ---------------------------------------------------------
// CPosTp16::UseCorruptDbL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CPosTp16::UseCorruptDbL()
    {
    RemoveDefaultDbL();
    
    #ifdef __WINS__
        _LIT(KCorruptDbFile, "z:\\system\\test\\testdata\\SRVeposdamaged.ldb");
    #else
        _LIT(KCorruptDbFile, "c:\\system\\test\\testdata\\SRVeposdamaged.ldb");
    #endif
    
    _LIT(KCorruptDbFileOld, "c:\\system\\data\\SRVeposdamaged.ldb");

    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    
    CFileMan* fileMan = CFileMan::NewL(fs);
    CleanupStack::PushL(fileMan);
    
    User::LeaveIfError(fileMan->Copy(KCorruptDbFile, KDefaultDbPath, CFileMan::EOverWrite));
    User::LeaveIfError(fileMan->Rename(KCorruptDbFileOld, KDefaultDb, CFileMan::EOverWrite));
    
    CleanupStack::PopAndDestroy(2, &fs);
    
    return CPosLandmarkDatabase::OpenL();
    }

// ---------------------------------------------------------
// CPosTp16::UseTextDbL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CPosTp16::UseTextDbL()
    {
    RemoveDefaultDbL();
    
    #ifdef __WINS__
        _LIT(KCorruptDbFile, "z:\\system\\test\\testdata\\SRVepostext.ldb");
    #else
        _LIT(KCorruptDbFile, "c:\\system\\test\\testdata\\SRVepostext.ldb");
    #endif
    
    _LIT(KCorruptDbFileOld, "c:\\system\\data\\SRVepostext.ldb");

    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    
    CFileMan* fileMan = CFileMan::NewL(fs);
    CleanupStack::PushL(fileMan);
    
    User::LeaveIfError(fileMan->Copy(KCorruptDbFile, KDefaultDbPath, CFileMan::EOverWrite));
    User::LeaveIfError(fileMan->Rename(KCorruptDbFileOld, KDefaultDb, CFileMan::EOverWrite));
    
    CleanupStack::PopAndDestroy(2, &fs);
    
    return CPosLandmarkDatabase::OpenL();
    }

// ---------------------------------------------------------
// CPosTp16::UseContactDbL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CPosTp16::UseContactDbL()
    {
    RemoveDefaultDbL();
    
    #ifdef __WINS__
        _LIT(KCorruptDbFile, "z:\\system\\test\\testdata\\SRVeposcontact.ldb");
    #else
        _LIT(KCorruptDbFile, "c:\\system\\test\\testdata\\SRVeposcontact.ldb");
    #endif
    
    _LIT(KCorruptDbFileOld, "c:\\system\\data\\SRVeposcontact.ldb");

    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    
    CFileMan* fileMan = CFileMan::NewL(fs);
    CleanupStack::PushL(fileMan);
    
    User::LeaveIfError(fileMan->Copy(KCorruptDbFile, KDefaultDbPath, CFileMan::EOverWrite));
    User::LeaveIfError(fileMan->Rename(KCorruptDbFileOld, KDefaultDb, CFileMan::EOverWrite));
    
    CleanupStack::PopAndDestroy(2, &fs);
    
    return CPosLandmarkDatabase::OpenL();
    }

// ---------------------------------------------------------
// CPosTp16::SetupGlobalCategoriesL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp16::SetupGlobalCategoriesL()
    {
    iGlobalCategoryNames.Reset();
    iGlobalCategoryIds.Reset();

    iGlobalCategoryNames.Append(_L("Restaurant"));
    iGlobalCategoryIds.Append(1);

    iGlobalCategoryNames.Append(_L("Bar"));
    iGlobalCategoryIds.Append(2);

    iGlobalCategoryNames.Append(_L("Gas station"));
    iGlobalCategoryIds.Append(3);

    iGlobalCategoryNames.Append(_L("Hotel"));
    iGlobalCategoryIds.Append(4);

    iGlobalCategoryNames.Append(_L("Shop"));
    iGlobalCategoryIds.Append(5);

    iGlobalCategoryNames.Append(_L("Sight"));
    iGlobalCategoryIds.Append(6);

    iGlobalCategoryNames.Append(_L("Car park"));
    iGlobalCategoryIds.Append(7);

    iGlobalCategoryNames.Append(_L("Bus stop"));
    iGlobalCategoryIds.Append(8);

    iGlobalCategoryNames.Append(_L("Pools"));
    iGlobalCategoryIds.Append(9);

    iGlobalCategoryNames.Append(_L("Shopping streets"));
    iGlobalCategoryIds.Append(10);
    }

//  End of File
