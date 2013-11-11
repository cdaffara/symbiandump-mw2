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
#include "FT_CPosTp36.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryCriteria.h>
#include <EPos_CPosLmCatNameCriteria.h>
#include <EPos_CPosLmTextCriteria.h>
           
// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp36::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp36::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    delete iDatabase;
    iDatabase=NULL;

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;

    iArray.Close();
    }

// ---------------------------------------------------------
// CPosTp36::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp36::StartL()
    {
    TTime startTime;
    TTime stopTime;

    startTime.UniversalTime();

    // Delete "c:\\system\\data\\eposlm.ldb"
    RemoveDefaultDbL();
    iDatabase = UseGeneratedDbFileL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

    // we should not find any uncategorized landmarks since every landmark in the db
    // belongs to a category
    iLog->Log(_L("Searching async"));
    DoTestL(ETrue, 0, 0);
    DoTestL(ETrue, 1, 0);
    DoTestL(ETrue, 2, 0);
    DoTestL(ETrue, 3, 0);

    iLog->Log(_L("Searching sync"));
    DoTestL(EFalse, 0, 0);
    DoTestL(EFalse, 1, 0);
    DoTestL(EFalse, 2, 0);
    DoTestL(EFalse, 3, 0);

    iLog->Log(_L("Should leave with KErrArgument"));
    TRAPD(err, DoSearchForCategoryL(ETrue));
    if (err != KErrArgument)    
    {
    	iLog->Log(_L("Should leave with KErrArgument"));
    	User::Leave(err);
    }
    else iLog->Log(_L("Function correctly leaved with KErrArgument"));
    
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;

    iLog->Log(_L("Should leave with KErrArgument"));
    TRAP(err, DoSearchForCategoryL(EFalse));
    if (err != KErrArgument)
    {
    	iLog->Log(_L("Should leave with KErrArgument"));
    	User::Leave(err);

    }
    else iLog->Log(_L("Function correctly leaved with KErrArgument"));

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;

    TInt maxNr = GetNrOfLandmarksL();

    TBuf<100> buf;
    buf.Format(_L("Nr of landmarks: %d"), maxNr);
    iLog->Log(buf);
    
    // Remove categories from some landmarks
    RemoveCategoriesFromLandmarksL(5);
    // Check that correct number of uncategorized landmarks are found
    DoTestL(EFalse, 0, 5);
    DoTestL(EFalse, 1, 5);
    DoTestL(EFalse, 2, 5);
    DoTestL(EFalse, 3, 5);
    // Remove categories from some landmarks
    RemoveCategoriesFromLandmarksL(6);
    // Check that correct number of uncategorized landmarks are found
    DoTestL(EFalse, 0, 11);
    DoTestL(EFalse, 1, 11);
    DoTestL(EFalse, 2, 11);
    DoTestL(EFalse, 3, 11);

    // Remove categories from some landmarks
    RemoveCategoriesFromLandmarksL(43);
    // Check that correct number of uncategorized landmarks are found
    DoTestL(EFalse, 0, 54);
    DoTestL(EFalse, 1, 54);
    DoTestL(EFalse, 2, 54);
    DoTestL(EFalse, 3, 54);
    
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;

    delete iDatabase;
    iDatabase=NULL;

    stopTime.UniversalTime();
    TTimeIntervalMicroSeconds executionTime = 
        stopTime.MicroSecondsFrom(startTime);

    TInt resse = executionTime.Int64()/1000000;
    buf.Zero();
    buf.Format(_L("TP36 took %d seconds"), resse);
    iLog->Log(buf);
	}

// ---------------------------------------------------------
// CPosTp36::DoTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp36::DoTestL(TBool aSync, TInt aTestNr, TInt aNrOfHits)
    {
    TBuf<150> buf;
    buf.Format(_L("DoTestL part: %d"), aTestNr);
    iLog->Log(buf);
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();

    // Specified in header file EPos_CPosLmCategoryCriteria.h says that
    // SetCategoryItemId(KPosLmNullItemId) should return uncategorized landmarks and that
    // SetGlobalCategory(KPosLmNullGlobalCategory) should return uncategorized landmarks and that
    // SetCategoryNameL(KNullDesC) should return uncategorized landmarks
    if (aTestNr == 0)
        {
        }
    else if (aTestNr == 1)
        {
        catSearchCriteria->SetCategoryItemId(KPosLmNullItemId);
        }
    else if (aTestNr == 2)
        {
        catSearchCriteria->SetGlobalCategory(KPosLmNullGlobalCategory);
        }
    else if (aTestNr == 3)
        {
        catSearchCriteria->SetCategoryNameL(KNullDesC);
        }

    iOperation = iLandmarkSearch->StartLandmarkSearchL(*catSearchCriteria);

    if (aSync)
        {
        RunAsyncOperationLD(iOperation);
        }
    else
        {
        ExecuteAndDeleteLD(iOperation);
        }

    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);

    TInt nr = iter->NumOfItemsL();
    if (nr != aNrOfHits)
        {
        buf.Format(_L("Wrong number of landmarks returned when searching for uncategorized landmarks, found: %d should found %d"), nr, aNrOfHits);
        
        iLog->Log(buf);
    	User::Leave(-1);

        }

    buf.Format(_L("Found %d nr of landmarks when searching GlobaCategories"), nr);
    iLog->Log(buf);
    TPosLmItemId id = iter->NextL();
    TInt res = 0;
    if (nr != 0)
        {
        while (id != KPosLmNullItemId)
            {
            TInt found = iArray.Find(id);
            if (found == KErrNotFound) 
            	{
            	          
            	iLog->Log(_L("Not found"));
            	}
            else 
                {
                res++;
                CPosLandmark* landmark = iDatabase->ReadLandmarkLC(id);
                CPosLandmark* landmark2 = iDatabase->ReadLandmarkLC(iArray[found]);

                CompareLandmarksL(*landmark, *landmark2);

                CleanupStack::PopAndDestroy(landmark2);
                CleanupStack::PopAndDestroy(landmark);
                }
            id = iter->NextL();
            }

        if (res != nr) 
            {
            
            iLog->Log(_L("The correct landmark was never found"));
    		User::Leave(-1);

            }
        }

    CleanupStack::PopAndDestroy(iter);
    CleanupStack::PopAndDestroy(catSearchCriteria);
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
	}

// ---------------------------------------------------------
// CPosTp36::DoSearchForCategoryL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp36::DoSearchForCategoryL(TBool aSync)
    {
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();

    // This method should leave
    iOperation = iLandmarkSearch->StartCategorySearchL(*catSearchCriteria, CPosLmCategoryManager::ECategorySortOrderNone);

    if (aSync)
        {
        RunAsyncOperationLD(iOperation);
        }
    else
        {
        ExecuteAndDeleteLD(iOperation);
        }

    CleanupStack::PopAndDestroy(catSearchCriteria);

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
	}

// ---------------------------------------------------------
// CPosTp36::RemoveCategoriesFromLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp36::RemoveCategoriesFromLandmarksL(TInt aNrOfLandmarks)
    {
    // Use this method to remove the category from a specified number of landmarks   
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();

    textCriteria->SetTextL(_L("*"));
    TUint attr = CPosLandmark::ELandmarkName | CPosLandmark::EDescription;
    textCriteria->SetAttributesToSearch(attr);

    iOperation = iLandmarkSearch->StartLandmarkSearchL(*textCriteria, EFalse); 
    ExecuteAndDeleteLD(iOperation);

    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
    
    TPosLmItemId id = iter->NextL();
    TInt counter=0;
    
    // Remove the category from aNrOfLandmarks nr of landmarks
    while ((id != KPosLmNullItemId) && (counter <aNrOfLandmarks))
        {
        CPosLandmark* landmark = iDatabase->ReadLandmarkLC(id);

        RArray<TPosLmItemId> categories;
        CleanupClosePushL(categories);
        landmark->GetCategoriesL(categories);
        TPtrC landmarkName;
        landmark->GetLandmarkName(landmarkName);
        TBuf<50> buf;

        TInt size = categories.Count();
        if (size != 0)
            {
            buf.Format(_L("Removing from landmark: "));
            buf.Append(landmarkName);
            iLog->Log(buf);
            iArray.Append(id);

            for (TInt j=0;j<size;j++)
                {
                // Remove category from landmark
                landmark->RemoveCategory(categories[j]);
                }
            iDatabase->UpdateLandmarkL(*landmark);
            counter++;
            }
        CleanupStack::PopAndDestroy(&categories);
        CleanupStack::PopAndDestroy(landmark);
        id = iter->NextL();
        if (id == KPosLmNullItemId) iLog->Log(_L("KPosLmNullItemId"));
        }
    iter->Reset();

    CleanupStack::PopAndDestroy(iter);
    CleanupStack::PopAndDestroy(textCriteria);

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    }

// ---------------------------------------------------------
// CPosTp36::GetNrOfLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosTp36::GetNrOfLandmarksL()
    {
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();

    textCriteria->SetTextL(_L("*"));
    TUint attr = CPosLandmark::ELandmarkName | CPosLandmark::EDescription;
    textCriteria->SetAttributesToSearch(attr);

    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    iOperation = iLandmarkSearch->StartLandmarkSearchL(*textCriteria, EFalse); 
    ExecuteAndDeleteLD(iOperation);

    TInt nrOfMatches = iLandmarkSearch->NumOfMatches();

    CleanupStack::PopAndDestroy(textCriteria);
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    return nrOfMatches;
    }

//  End of File
