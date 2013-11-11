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

#include "FT_CPosTp34.h"
#include <f32file.h>

//#include "FT_CSearchResult.h"  
//#include "FT_CActiveSearch.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkSearch.h> 
#include <EPos_CPosLMItemIterator.h>   

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp34::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp34::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP34 - Multiple async operations");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp34::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp34::CloseTest()
    {
    delete iDatabase;
    iDatabase = NULL;
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;

    iSearchResults.ResetAndDestroy();
    }

// ---------------------------------------------------------
// CPosTp34::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp34::StartL()
    {
    _LIT(KInUseErr, "search is already running, StartLandmarksSearch should leave with error code KErrInUse");
    iDatabase = UseGeneratedDbFileL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
    
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    
    CActiveLmOperation* activeLmOperation = new(ELeave) CActiveLmOperation(iLog);//1
    CleanupStack::PushL(activeLmOperation);

    AppendSearchResultsL();
    
    const TInt KSearchResultIndex1=1;
    const TInt KSearchResultIndex2=10;

    CPosLmTextCriteria* textCriteria1 = CPosLmTextCriteria::NewLC(); //2
    textCriteria1->SetTextL(iSearchResults[KSearchResultIndex1]->TextCriteria());
    textCriteria1->SetAttributesToSearch(iSearchResults[KSearchResultIndex1]->Attributes());

    CPosLmTextCriteria* textCriteria2 = CPosLmTextCriteria::NewLC(); //3
    textCriteria2->SetTextL(iSearchResults[KSearchResultIndex2]->TextCriteria());
    textCriteria2->SetAttributesToSearch(iSearchResults[KSearchResultIndex2]->Attributes());

    iOperation = iLandmarkSearch->StartLandmarkSearchL(*textCriteria1); 
    activeLmOperation->Start(iOperation);

    TRAPD(err, iLandmarkSearch->StartLandmarkSearchL(*textCriteria2)); 
    AssertTrueSecL(err == KErrInUse, KInUseErr);

    CActiveLmOperation* activeLmOperation2 = new(ELeave) CActiveLmOperation(iLog); 
    CleanupStack::PushL(activeLmOperation2); //4

    ///
    CPosLandmarkSearch* landmarkSearch2 = CPosLandmarkSearch::NewL(*iDatabase);
    CleanupStack::PushL(landmarkSearch2); //5

    CPosLmOperation* lmo = landmarkSearch2->StartLandmarkSearchL(*textCriteria2);
    CleanupStack::PushL(lmo); //6

    activeLmOperation2->Start(lmo);

    CActiveScheduler::Start();

    CActiveScheduler::Start();

     activeLmOperation->CheckOperationL();
     delete iOperation;
     iOperation = NULL;

    activeLmOperation2->CheckOperationL();
    
    CheckSearchResultL(*iSearchResults[KSearchResultIndex1], *iLandmarkSearch, *textCriteria1);

    CheckSearchResultL(*iSearchResults[KSearchResultIndex2], *landmarkSearch2, *textCriteria2);

    //CleanupStack::PopAndDestroy(5, activeSearch);
    CleanupStack::PopAndDestroy(6, activeLmOperation);

    }

// ---------------------------------------------------------
// CPosTp34::CheckSearchResultL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp34::CheckSearchResultL(CSearchResult& aSearchResult,
                                  CPosLandmarkSearch& aLandmarkSearch,
                                  CPosLmTextCriteria& aTextCriteria)
    {    
    _LIT(KNotFoundErr, "Id %d not found in parsed Lm search results when the search is performed syncronously");
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
    if ((TUint)(aSearchResult.SearchResult()).Count() != aLandmarkSearch.NumOfMatches())
        {
        _LIT(KExpected, "Expected no. of matches: %d");
        _LIT(KReturned, "Returned no. of matches: %d");
        TBuf<100> info;
        info.Format(KExpected, (aSearchResult.SearchResult()).Count());
        iLog->Put(info);
        info.Format(KReturned, aLandmarkSearch.NumOfMatches());
        iLog->Put(info);
        iLog->Put(_L("Search criteria: "));
        iLog->Put(aTextCriteria.Text());
        LogErrorAndLeave(KNumOfMatchesErr);
        }
    CPosLmItemIterator* iter = aLandmarkSearch.MatchIteratorL();
    CleanupStack::PushL(iter);
    
    TPosLmItemId id = iter->NextL();
    while (id != KPosLmNullItemId)
        {
        AssertTrueSecL(aSearchResult.FindSearchResult(id), KNotFoundErr, id);
        id = iter->NextL();
        }
    
    CleanupStack::PopAndDestroy(iter);

    }

//  End of File
