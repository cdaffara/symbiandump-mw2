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

#include "FT_CPosTp6.h"

#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLandmarkCategory.h> 
#include <EPos_CPosLMCategoryManager.h>

#include "FT_LandmarkConstants.h"

// CONSTANTS
// _LIT(KGetCategoryNameErr, "GetCategoryName returns incorrect error code");
_LIT(KCategoryNameErr, "Category name is incorrect");
_LIT(KCategoryIdErr, "Category Id is incorrect");
_LIT(KGlobalCategoryIdErr, "Global Category Id is incorrect");
_LIT(KCategoryIconIndexErr, "Category Icon index is incorrect");
_LIT(KCategoryIconMaskIndexErr, "Category Icon Mask Index is incorrect");
_LIT(KCategoryMbmFileNameErr, "Category MbmFileName is incorrect");
_LIT(KAddCategoryErr, "AddCategory should return an error");


// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp6::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp6::StartL()
    {
    iLog->Log(_L("In the StartL of FT_CPosTp6.h"));
    RemoveDefaultDbL();
    
    CPosLandmarkCategory* category = CreateCategoryLC(KCategoryName);
    CPosLandmarkCategory* copy = CPosLandmarkCategory::NewLC(*category);

    CheckGetMethodsL(category, KCategoryName);
    
    copy->SetCategoryNameL(KCategoryName2);
    CheckGetMethodsL(copy, KCategoryName2);

    CheckRemoveMethodsL(category);
    CheckRemoveMethodsL(copy);
    
    CPosLandmarkDatabase* lmd = OpenDatabaseLC();
    ExecuteAndDeleteLD(lmd->CompactL());
    CPosLmCategoryManager* cm = CPosLmCategoryManager::NewL(*lmd); 
    CleanupStack::PushL(cm); 
    User::After(1000000);
    TRAPD(err, cm->AddCategoryL(*copy));
    AssertTrueSecL(err == KErrArgument, KAddCategoryErr);
    
    CheckFieldsMaxLengthL();
    
    CheckEmptyCategoryNameL();
    
    CheckSetIconL();

    CleanupStack::PopAndDestroy(4); // category, copy, lmd, cm
	}

// ---------------------------------------------------------
// CPosTp6::CheckGetmethods
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp6::CheckGetMethodsL(CPosLandmarkCategory* aCategory, const TDesC& aName)
    {
    TPtrC categoryName;
   
    aCategory->GetCategoryName(categoryName);  
   
    AssertTrueSecL(categoryName.Compare(aName) == KErrNone, KCategoryNameErr);
    AssertTrueSecL(aCategory->CategoryId() == 0, KCategoryIdErr);
    AssertTrueSecL(aCategory->GlobalCategory() == 0, KGlobalCategoryIdErr);
    
    TPtrC mbmFileName;
    TInt index;
    TInt indexMask;
    aCategory->GetIcon(mbmFileName, index, indexMask);
    AssertTrueSecL(mbmFileName.Compare(KMbmFileName) == KErrNone, KCategoryMbmFileNameErr);
    AssertTrueSecL(index == KIconIndex, KCategoryIconIndexErr);
    AssertTrueSecL(indexMask == KIconMaskIndex, KCategoryIconMaskIndexErr);    

    CPosLandmarkDatabase* lmd = OpenDatabaseLC();
    CPosLmCategoryManager* cm = CPosLmCategoryManager::NewL(*lmd); 
    CleanupStack::PushL(cm);
    
    TPosLmItemId id1 = cm->AddCategoryL(*aCategory);
    User::After(1000000);
    TRAPD(err, cm->AddCategoryL(*aCategory));
    AssertTrueSecL(err == KErrAlreadyExists, KAddCategoryErr);
    AssertTrueSecL(id1 == aCategory->CategoryId(), KCategoryIdErr);
    
    CleanupStack::PopAndDestroy(2); // lmd, cm
	}

// ---------------------------------------------------------
// CPosTp6::CheckRemoveMethodsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//    
void CPosTp6::CheckRemoveMethodsL(CPosLandmarkCategory* aCategory)
    {
    aCategory->RemoveCategoryAttributes(CPosLandmarkCategory::ENoAttribute);
    aCategory->RemoveCategoryAttributes(CPosLandmarkCategory::ECategoryName);
    aCategory->RemoveCategoryAttributes(CPosLandmarkCategory::EIcon);
    
    CheckRemovedL(aCategory);

    CPosLandmarkCategory* category = CreateCategoryLC(KCategoryName);
    category->RemoveCategoryAttributes(CPosLandmarkCategory::EAllAttributes);
    CheckRemovedL(category);
    CleanupStack::PopAndDestroy(category);
	
	// Check with attributes outside defined interval
	// since TAttributes is a TUint32 -1 is casted to EAllAttributes = 0xFFFF 
    category = CreateCategoryLC(_L("NewCategory"));
    category->RemoveCategoryAttributes((CPosLandmarkCategory::TAttributes)-1);
    
    TPtrC categoryName;
    if (category->GetCategoryName(categoryName) == KErrNotFound) iLog->Log(_L("Category Name has been removed"));
    TPtrC mbmFileName;
    TInt index;
    TInt indexMask;
    if (aCategory->GetIcon(mbmFileName, index, indexMask) == KErrNotFound) iLog->Log(_L("Catgory Icon has been removed"));
    
    CleanupStack::PopAndDestroy(category);
    
    // Check with attributes outside defined interval
    category = CreateCategoryLC(_L("NewCategory2"));
    category->RemoveCategoryAttributes(0);
    category->GetCategoryName(categoryName);
    AssertTrueSecL(categoryName.CompareC(_L("NewCategory2")) == KErrNone, _L("Wrong category name returned after remove invalid attribute"));
	
    CleanupStack::PopAndDestroy(category);
    }

// ---------------------------------------------------------
// CPosTp6::CheckRemovedL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp6::CheckRemovedL(CPosLandmarkCategory* aCategory)
    {
    TPtrC name;
    AssertTrueSecL(aCategory->GetCategoryName(name) == KErrNotFound, KCategoryNameErr);

    TPtrC mbmFileName;
    TInt index;
    TInt indexMask;
    AssertTrueSecL(aCategory->GetIcon(mbmFileName, index, indexMask) == KErrNotFound, KCategoryMbmFileNameErr);   
    
    }
    
//---------------------------------------------------------
// CPosTp6::CheckEmptyCategoryNameL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp6::CheckEmptyCategoryNameL()
	{
	
	iLog->Log(_L("CheckEmptyCategoryNameL"));
	CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
	
	// Set empty string as category name
	// should not be accepted
    TRAPD(err, category->SetCategoryNameL(_L("")));
    
    if (err != KErrArgument)
    {
    iLog->Log(_L("Empty category name should not be accepted"));
    User::Leave(err);	
    }
    
    
    CleanupStack::PopAndDestroy(category);
	}

//---------------------------------------------------------
// CPosTp6::CheckSetIconL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp6::CheckSetIconL()
	{
	iLog->Log(_L("CheckSetIconL"));
	
	_LIT(K124Error, "Icon name is not limited to 255 characters");

    
	// Defined in KMaxFileName e32const.h
    HBufC* hbuf = HBufC::NewLC(KMaxFileName+1);
    while(hbuf->Des().Length() < KMaxFileName)
        {
        hbuf->Des().Append(_L("X"));
        }
        
    CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
    category->SetIconL(*hbuf, 0, KPosLmIconMaskNotUsed);
    
    hbuf->Des().Append(_L("X"));

    TRAPD(err, category->SetIconL(*hbuf, 0, KPosLmIconMaskNotUsed));
    AssertTrueSecL(err == KErrArgument, K124Error);

    CleanupStack::PopAndDestroy(2, hbuf);
	}
	
//---------------------------------------------------------
// CPosTp6::CheckFieldsMaxLength
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp6::CheckFieldsMaxLengthL()
    {
    _LIT(K124Error, "Category name is not limited to 255 characters");

    const TInt KMaxFileName = 124;
    HBufC* hbuf = HBufC::NewLC(KMaxFileName+1);
 
    while(hbuf->Des().Length() < KMaxFileName)
        {
        hbuf->Des().Append(_L("X"));
        }

    CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
    category->SetCategoryNameL(*hbuf);
    
    hbuf->Des().Append(_L("X"));

    TRAPD(err, category->SetCategoryNameL(*hbuf));
    AssertTrueSecL(err == KErrArgument, K124Error);

    CleanupStack::PopAndDestroy(2, hbuf);
    }
    
//  End of File
