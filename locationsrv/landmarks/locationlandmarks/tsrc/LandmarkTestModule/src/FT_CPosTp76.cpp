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
#include "FT_CPosTp76.h"
#include <EPos_CPosLandmarkDatabase.h>
#include "FT_LandmarkConstants.h"
           
//_LIT(KCopyFilePath, "c:\\system\\data\\EPOSLMGLOBALCATEGORIES.R01");
_LIT(KCopyFilePath, "c:\\resource\\EPOSLMGLOBALCATEGORIES.R01");
#ifdef __WINS__
_LIT(KCORRUPTFILE, "z:\\system\\test\\testdata\\corrupt_EPOSLMGLOBALCATEGORIES.R06");
#else
_LIT(KCORRUPTFILE, "c:\\system\\test\\testdata\\corrupt_EPOSLMGLOBALCATEGORIES.R06");
#endif

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp76::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp76::GetName(TDes& aName) const
    {
    _LIT(KTestName, "Tp76 - Corrupt Global Categories resource file");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp76::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp76::StartL()
    {  
    
    // Test error report ESLI-65WADU
	TestCorruptGlobalCategoriesFileL();

	}

// ---------------------------------------------------------
// CPosTp76::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp76::CloseTest()
	{
	iLog->Put(_L("CloseTest"));
	// Delete "corrupt" global categories file
	DeleteFileL(KCopyFilePath);

	delete iOperation;
	iOperation = NULL;

	}

// ---------------------------------------------------------
// CPosTp76::TestCorruptGlobalCategoriesFileL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp76::TestCorruptGlobalCategoriesFileL()
	{
	iLog->Put(_L("TestCorruptGlobalCategoriesFileL"));
	RemoveDefaultDbL();

	RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    
    CFileMan* fileMan = CFileMan::NewL(fs);
    CleanupStack::PushL(fileMan);
   
	// use emtpy landmarks db as corrupt global categories file
    //User::LeaveIfError(fileMan->Copy(KLandmarksEmptyDb, KCopyFilePath, CFileMan::EOverWrite));
    User::LeaveIfError(fileMan->Copy(KCORRUPTFILE, KCopyFilePath, CFileMan::EOverWrite));
     
    CleanupStack::PopAndDestroy(2, &fs);
	iLog->Put(_L("Open Landmarksdb"));
	CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);

	if (lmd->IsInitializingNeeded())
        {
		iLog->Put(_L("IsInitializingNeeded()"));
        ExecuteAndDeleteLD(lmd->InitializeL());
        iLog->Put(_L("After InitializeL()"));
		}

    // FrBo TBD below, what will happen below when error above is fixed
	CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*lmd);
	CleanupStack::PushL(categoryManager);
	// Make sure that global categories are included in lm db, use default global categories
	iLog->Put(_L("categoryManager->ResetGlobalCategoriesL()"));
	
	TRAPD(err, iOperation = categoryManager->ResetGlobalCategoriesL());

	if (err == KErrNone) LogErrorAndLeave(_L("ResetGlobalCategoriesL should leave with errorcode"));
	TBuf<100> buf;
	buf.Format(_L("ResetGlobalCategoriesL leaved with code: %d"), err);
	iLog->Put(buf);

	delete iOperation;
	iOperation = NULL;

	CleanupStack::PopAndDestroy(categoryManager);
    CleanupStack::PopAndDestroy(lmd);

	DeleteFileL(KCopyFilePath);
	iLog->Put(_L("TestCorruptGlobalCategoriesFileL"));
	}

//  End of File
