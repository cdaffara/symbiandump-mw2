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

#include "FT_CPosTp81.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmDatabaseManager.h>
           
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp81::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp81::GetName(TDes& aName) const
    {
    _LIT(KTestName, "Tp81 - category with apostrophe in name (SBUY-6A9A5B)");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp81::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp81::StartL()
    {
    _LIT(KCategoryName, "Category with apostrophe'");
    _LIT(KCategoryName2, "Category with apostrophe(')");
    
    RemoveAllLmDatabasesL();

    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(db);
    if (db->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(db->InitializeL()); // Synchronous since no argument
       }
    
    CPosLmCategoryManager* manager = CPosLmCategoryManager::NewL(*db);
    CleanupStack::PushL(manager);
    
    // Test with only apostrophe
    CPosLandmarkCategory* cat = CPosLandmarkCategory::NewL();
    CleanupStack::PushL(cat);
    iLog->Put(_L("Set name with apostrophe"));
    cat->SetCategoryNameL(KCategoryName); 
    iLog->Put(_L("AddCategory with apostrophe in name"));
    TPosLmItemId id = manager->AddCategoryL(*cat);
    User::After(200000);
    iLog->Put(_L("AddCategory was succesful"));
    CleanupStack::PopAndDestroy(cat);
    
    CPosLandmarkCategory* category = manager->ReadCategoryLC(id);
    TPtrC catName;
    category->GetCategoryName(catName);
    iLog->Put(catName);
    if (catName.CompareC(KCategoryName) != KErrNone) iLog->Put(_L("ERROR: Names are not equal"));
    CleanupStack::PopAndDestroy(category);
    CleanupStack::PopAndDestroy(manager);
    
    // Test with apostrophe and paranthesis
    manager = CPosLmCategoryManager::NewL(*db);
    CleanupStack::PushL(manager);
    
    cat = CPosLandmarkCategory::NewL();
    CleanupStack::PushL(cat);
    iLog->Put(_L("Set name with apostrophe"));
    cat->SetCategoryNameL(KCategoryName2); 
    iLog->Put(_L("AddCategory with apostrophe and paranthesis in name"));
    id = manager->AddCategoryL(*cat);
    User::After(200000);
    iLog->Put(_L("AddCategory was succesful"));
    CleanupStack::PopAndDestroy(cat);
    
    category = manager->ReadCategoryLC(id);
    category->GetCategoryName(catName);
    iLog->Put(catName);
    if (catName.CompareC(KCategoryName2) != KErrNone) iLog->Put(_L("ERROR: Names are not equal (2)"));
    CleanupStack::PopAndDestroy(category);
    
    ExecuteAndDeleteLD(db->CompactL());

    CleanupStack::PopAndDestroy(manager);
    CleanupStack::PopAndDestroy(db);

    TestDifferentNamesL();
    
    }
// ---------------------------------------------------------
// CPosTp81::TestDifferentNamesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp81::TestDifferentNamesL()
    {
    iLog->Put(_L("TestDifferentNamesL"));        
    ////////////////////////////////////////////////////////////
    _LIT(KCategory1, "file://:://c:::en helt \" sjuk .ldb databas\".ldb.ldb.ldb");
    _LIT(KCategory2, "file://c:en helt sjuk databas.ldb");
    _LIT(KCategory3, "le:///c:en helt sjuk databas.ldb");
    _LIT(KCategory4, "/c::en helt sjuk databas.ldb");
    _LIT(KCategory5, "en helt sjuk .ldb databas.ldb");
    _LIT(KCategory6, "en helt sjuk databas.ldb.ldb.ldb");
    _LIT(KCategory7, "en helt \" sjuk databas.ldb");
    _LIT(KCategory8, "en helt < sjuk databas.ldb");
    _LIT(KCategory9, "en helt > sjuk databas.ldb");
    _LIT(KCategory10, "en helt | sjuk databas.ldb");
    _LIT(KCategory11, "en helt / sjuk databas.ldb");
    _LIT(KCategory12, "en helt ? sjuk databas.ldb");
    _LIT(KCategory13, "en helt \\ sjuk databas.ldb");
    _LIT(KCategory14, "en helt : sjuk databas.ldb");
    _LIT(KCategory15, ":!#§%&()=+`¥^®~'ßΩ-_,;.{}[]$£@0123456789Â‰ˆ≈ƒ÷.ldb");
    _LIT(KCategory16, "file://c:qwertyuiopasdfghjklzxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM.ldb");
    _LIT(KCategory17, "file://en otroligt helt sjuk databas.ldb");
    _LIT(KCategory18, "c:en helsjuk databas.ldb");
    _LIT(KCategory19, "file://c:\\system\\data\\en helt sjuk databas.ldb");
    _LIT(KCategory20, "file://c:.ldb");
    _LIT(KCategory21, "file://c:filename");
    _LIT(KCategory22, "file://c:filename.dip");
    _LIT(KCategory23, "file://c:filename. ldb");
    _LIT(KCategory24, "file://c:*.ldb");
    _LIT(KCategory25, "file://allan:en helt sjuk databas.ldb");
    _LIT(KCategory26, "file://g:en helt sjuk databas.ldb");
    _LIT(KCategory27, "file://c:filename.ldb");
    _LIT(KCategory28, "file://c:012345678901234567890123456789012345678901234567890123456789.ldb");
    
    const TInt KNrOfUris = 28;
    const TDesC* listOfCategories[] = {&KCategory1, &KCategory2, &KCategory3 ,&KCategory4, &KCategory5, &KCategory6,
     &KCategory7, &KCategory8, &KCategory9
    , &KCategory10, &KCategory11, &KCategory12, &KCategory13, &KCategory14, &KCategory15
    , &KCategory16, &KCategory17, &KCategory18, &KCategory19, &KCategory20, &KCategory21
    , &KCategory22, &KCategory23, &KCategory24, &KCategory25, &KCategory26, &KCategory27, &KCategory28};

/*
    const TInt KNrOfNonWorkingUris = 19;
    const TDesC* listOfUrisNonWorking[] = {&KCategory1, &KCategory3 ,&KCategory4, &KCategory7, &KCategory8, &KCategory9
    , &KCategory10, &KCategory11, &KCategory12, &KCategory13, &KCategory14, &KCategory19, &KCategory20, &KCategory21
    , &KCategory22, &KCategory23, &KCategory24, &KCategory25, &KCategory26};
    
    TInt errorList[] = {KErrBadName, KErrBadName, KErrBadName, KErrBadName, KErrBadName ,KErrBadName,KErrBadName,
                   KErrBadName, KErrBadName, KErrArgument, KErrBadName, KErrArgument, KErrArgument,
                   KErrArgument, KErrArgument,KErrArgument, KErrBadName, KErrBadName, KErrNotReady};
                   */
    //////////////////////////////////////////////////
    
    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(db);
    
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*db);
    CleanupStack::PushL(categoryManager);
    
    TBuf<100> buf;
    
    for (TInt i=0;i<KNrOfUris;i++)
        {
        CPosLandmarkCategory* cat = CPosLandmarkCategory::NewL();
        CleanupStack::PushL(cat);
        iLog->Put(_L("Set name to:"));
        iLog->Put(*listOfCategories[i]);
        TRAPD(err, cat->SetCategoryNameL(*listOfCategories[i]));
        if (err == KErrNone)
            {
            iLog->Put(_L("AddCategory"));
            TPosLmItemId id = -1;
            TRAPD(err2, id = categoryManager->AddCategoryL(*cat));
            User::After(200000);
            if (err2 != KErrNone)
                {
                buf.Format(_L("Error %d from AddCategoryL"), err2);
                LogErrorAndLeave(buf);
                }
            else
                {
                buf.Format(_L("Checking landmark with id %d"), id);
                iLog->Put(buf);
                
                CPosLandmarkCategory* category = categoryManager->ReadCategoryLC(id);
                User::After(200000);
                TPtrC catName;
                category->GetCategoryName(catName);
                iLog->Put(catName);
                if (catName.CompareC(*listOfCategories[i]) != KErrNone) iLog->Put(_L("ERROR: Names are not equal"));
                CleanupStack::PopAndDestroy(category);
                }
            }
        else
            {
            buf.Format(_L("Error %d from SetCategoryNameL"), err);
            LogErrorAndLeave(buf);
            }
        CleanupStack::PopAndDestroy(cat);
        ExecuteAndDeleteLD(db->CompactL());
        }
    
    CleanupStack::PopAndDestroy(categoryManager);
    CleanupStack::PopAndDestroy(db);
    }
//  End of File
