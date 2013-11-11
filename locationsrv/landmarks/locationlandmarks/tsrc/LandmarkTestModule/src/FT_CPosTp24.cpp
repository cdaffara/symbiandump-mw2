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

#include "FT_CPosTp24.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmark.h> 
#include <EPos_CPosLandmarkSearch.h> 
#include <EPos_CPosLmAreaCriteria.h>
#include <EPos_CPosLMItemIterator.h> 
           
// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp24::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp24::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    delete iDatabase;
    iDatabase = NULL;
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    iAreaSearchResults.ResetAndDestroy();
    delete iOperation;iOperation=NULL;
    }

// ---------------------------------------------------------
// CPosTp24::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp24::StartL()
    {
    iDatabase = UseGeneratedDbFileL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);  

    AppendAreaSearchResultsL();	

    // Invalid areas is tested (see LmDbAreaSearchResult.txt)   
    iLog->Log(_L("Testing area search with criterias read from LmDbAreaSearchResult.txt syncronously"));
    SearchL(ESynchronous);

    iLog->Log(_L("Testing area search with criterias read from LmDbAreaSearchResult.txt asyncronously"));
    SearchL(EAsynchronous);
   
    iLog->Log(_L("Testing area search with criterias read from LmDbAreaSearchResult.txt asyncronously with User::WaitForRequest()"));
    SearchL(EWaitForRequest);
   
    iLog->Log(_L("Testing area search with a defined sortorder"));
    SearchWithSortOrderL();
    
    iLog->Log(_L("Testing area search and cancel"));
    SearchAndCancelL();
    iLog->Log(_L("Testing area search and cancel in callback"));
    SearchAndCancelL(ETrue);
    }

// ---------------------------------------------------------
// CPosTp24::SearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp24::SearchL(TExecutionMode aExecutionMode)    
    {
    TBuf<255> info;
    for (TInt i=0; i<iAreaSearchResults.Count(); i++)
        {
        CPosLmAreaCriteria* areaCriteria=NULL;
        TRAPD(err, areaCriteria = CPosTp24::NewLmAreaCriteriaL(iAreaSearchResults[i]->SouthLat(),
                                                               iAreaSearchResults[i]->NorthLat(),
                                                               iAreaSearchResults[i]->WestLong(),
                                                               iAreaSearchResults[i]->EastLong()
                                                               ));
        CleanupStack::PushL(areaCriteria);
        if (err != iAreaSearchResults[i]->ErrorCode())
            {
            iLog->Log(_L("Unexpected error code returned"));    
            iLog->Log(_L("Area Search result read from file:"));
            
            iAreaSearchResults[i]->Print(info);
            iLog->Log(info);

            _LIT(KReturned, "Search returned errorCode = %d");
            info.Format(KReturned, err);
            iLog->Log(info);
            User::Leave(KErrGeneral);
            }
        
        if (!err)
            {
            iOperation = iLandmarkSearch->StartLandmarkSearchL(
                *areaCriteria, iAreaSearchResults[i]->Redefined());
            switch (aExecutionMode)
                {
                case ESynchronous:
                    ExecuteAndDeleteLD(iOperation);
                    iOperation = NULL;
                    break;
                case EAsynchronous:
                    RunAsyncOperationLD(iOperation);
                    iOperation = NULL;
                    break;
                case EWaitForRequest:
                    RunAsyncOperationByWaitForReqL(iOperation);
                    delete iOperation;iOperation=NULL;
                    break;
                }

            CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
            CleanupStack::PushL(iter);

            if ((TUint)(iAreaSearchResults[i]->SearchResult()).Count() != iLandmarkSearch->NumOfMatches())
                {
                iLog->Log(_L("Unexpected no. of matches returned"));  
                _LIT(KExpected, "Expected no. of matches: %d ");
                _LIT(KReturned, "Returned no. of matches: %d ");
                info.Format(KExpected, (iAreaSearchResults[i]->SearchResult()).Count());
                iLog->Log(info);
                info.Format(KReturned, iLandmarkSearch->NumOfMatches());
                iLog->Log(info);
                iLog->Log(_L("Area Search result read from file:"));
                info.Zero();
                iAreaSearchResults[i]->Print(info);
                iLog->Log(info);

                iLog->Log(_L("Landmarks expected to find:"));
                for (TInt j=0; j<iAreaSearchResults[i]->SearchResult().Count(); j++)
                    {
                    RArray<TPosLmItemId>* searchResults = &iAreaSearchResults[i]->SearchResult();
                    CPosLandmark* lm = iDatabase->ReadLandmarkLC((*searchResults)[j]); 
                    info.Zero();
                    PrintLandmark(*lm, info);
                    iLog->Log(info);
                    CleanupStack::PopAndDestroy(lm);
                    }     

               iLog->Log(_L("Landmarks returned by Area search:"));
               TPosLmItemId id = iter->NextL();
               while (id != KPosLmNullItemId)
                    {
                    CPosLandmark* lm2 = iDatabase->ReadLandmarkLC(id); 
                    info.Zero();
                    PrintLandmark(*lm2, info);
                    iLog->Log(info);
                    CleanupStack::PopAndDestroy(lm2);
                    id = iter->NextL();
                    }
          
                User::Leave(KErrGeneral);
                }

            TPosLmItemId id = iter->NextL();
            while (id != KPosLmNullItemId)
                {
                if (!iAreaSearchResults[i]->FindSearchResult(id))
                    {
                    _LIT(KUnexpectedId, "Unexpected id, %d returned from search which is not in read area search result");  
                    info.Format(KUnexpectedId, id);
                    iLog->Log(_L("Area Search result read from file:"));
                    iAreaSearchResults[i]->Print(info);
                    iLog->Log(info);
                    User::Leave(KErrGeneral);
                    }
                id = iter->NextL();
                }
            CleanupStack::PopAndDestroy(iter);
            }
        CleanupStack::PopAndDestroy(areaCriteria);
        }
    }

// ---------------------------------------------------------
// CPosTp24::SearchWithSortOrderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp24::SearchWithSortOrderL()
    {
    const TInt KSortOrderIndex=4;
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect");
    _LIT(KSortorderErr, "The sortorder is incorrect");

    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
   
    CPosLmAreaCriteria* areaCriteria=CPosLmAreaCriteria::NewLC(
                    iAreaSearchResults[KSortOrderIndex]->SouthLat(),
                    iAreaSearchResults[KSortOrderIndex]->NorthLat(),
                    iAreaSearchResults[KSortOrderIndex]->WestLong(),
                    iAreaSearchResults[KSortOrderIndex]->EastLong()
                                                              );
                                                              
    // Test SetSearchArea
    areaCriteria->SetSearchArea(iAreaSearchResults[KSortOrderIndex]->SouthLat(),
                    iAreaSearchResults[KSortOrderIndex]->NorthLat(),
                    iAreaSearchResults[KSortOrderIndex]->WestLong(),
                    iAreaSearchResults[KSortOrderIndex]->EastLong());
                    
                    
    TReal64 southLat;
    TReal64 northLat;
    TReal64 westLong;
    TReal64 eastLong;
    areaCriteria->GetSearchArea(southLat, northLat, westLong, eastLong);
    AssertTrueSecL(southLat == iAreaSearchResults[KSortOrderIndex]->SouthLat(), _L("SouthLat is wrong"));
    AssertTrueSecL(northLat == iAreaSearchResults[KSortOrderIndex]->NorthLat(), _L("NorthLat is wrong"));
    AssertTrueSecL(westLong == iAreaSearchResults[KSortOrderIndex]->WestLong(), _L("WestLong is wrong"));
    AssertTrueSecL(eastLong == iAreaSearchResults[KSortOrderIndex]->EastLong(), _L("EastLong is wrong"));
    
                    
    for (TInt i=0; i<6; i++)
        {
        if(i==0)
            {
            iOperation = iLandmarkSearch->StartLandmarkSearchL(*areaCriteria, sortPref);
            ExecuteAndDeleteLD(iOperation);
            }
        else if (i==1)
            {
            sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
            iOperation = iLandmarkSearch->StartLandmarkSearchL(*areaCriteria, sortPref);
            ExecuteAndDeleteLD(iOperation);
            }
        else if (i==2)
            {
            iOperation = iLandmarkSearch->StartLandmarkSearchL(*areaCriteria, sortPref);
            RunAsyncOperationLD(iOperation);
            }
        else if (i==3)
            {
            sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
            iOperation = iLandmarkSearch->StartLandmarkSearchL(*areaCriteria, sortPref);
            RunAsyncOperationLD(iOperation);
            }
        else if (i==4)
            {
            iOperation = iLandmarkSearch->StartLandmarkSearchL(*areaCriteria, sortPref);
            RunAsyncOperationByWaitForReqL(iOperation);
            delete iOperation;iOperation=NULL;
            }
        else if (i==5)
            {
            sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
            iOperation = iLandmarkSearch->StartLandmarkSearchL(*areaCriteria, sortPref);
            RunAsyncOperationByWaitForReqL(iOperation);
            delete iOperation;iOperation=NULL;
            }
        iOperation = NULL;
        CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
        CleanupStack::PushL(iter);
        
        RArray<TPosLmItemId>* searchResults = &iAreaSearchResults[KSortOrderIndex]->SearchResult();
        AssertTrueSecL((TUint)searchResults->Count() == iLandmarkSearch->NumOfMatches(), KNumOfMatchesErr);    
        
        LandmarksSortL(*searchResults, sortPref);
        
        TInt ii=0;
        TPosLmItemId id = iter->NextL();
        while (id != KPosLmNullItemId)
            {  
            AssertTrueSecL(id == (*searchResults)[ii], KSortorderErr, id);
            
            id = iter->NextL();
            ++ii;
            }
        CleanupStack::PopAndDestroy(iter);
        }

    CleanupStack::PopAndDestroy(areaCriteria);
    }

// ---------------------------------------------------------
// CPosTp24::NewLmAreaCriteriaL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLmAreaCriteria* CPosTp24::NewLmAreaCriteriaL(const TReal64 &aSouthLat, 
                                                 const TReal64 &aNorthLat,
                                                 const TReal64 &aWestLong, 
                                                 const TReal64 &aEastLong) 
    {
    CPosLmAreaCriteria* areaCriteria = CPosLmAreaCriteria::NewLC(aSouthLat, 
                                                                aNorthLat, 
                                                                aWestLong, 
                                                                aEastLong
                                                                );
    CleanupStack::Pop();
    return areaCriteria;
    }

// ---------------------------------------------------------
// CPosTp24::SearchAndCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp24::SearchAndCancelL(const TBool& aInCallback)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
    _LIT(KCancelErr, "The search is canceled after progress has increased but no search result is found");

    // Test search with one criteria
    const TInt KIndex=0;

   CPosLmAreaCriteria* areaCriteria=NULL;
   areaCriteria = CPosLmAreaCriteria::NewLC(iAreaSearchResults[KIndex]->SouthLat(),
                                            iAreaSearchResults[KIndex]->NorthLat(),
                                            iAreaSearchResults[KIndex]->WestLong(),
                                            iAreaSearchResults[KIndex]->EastLong()
                                            );

    if (aInCallback)
        {
        RunAsyncOperationAndCancelInCallbackLD(iLandmarkSearch->StartLandmarkSearchL(
                *areaCriteria, iAreaSearchResults[KIndex]->Redefined()));
        }
    else
        {
        RunAsyncOperationAndCancelLD(iLandmarkSearch->StartLandmarkSearchL(
                *areaCriteria, iAreaSearchResults[KIndex]->Redefined()));
        }

    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
 
    if (aInCallback)
        {
        if (iter->NumOfItemsL() != iLandmarkSearch->NumOfMatches())
            {
            iLog->Log(_L("Iterators no. of items is not equal to LandmarksSearch no. matches"));
            _LIT(KExpected, "Iter. of matches: %d");
            _LIT(KReturned, "LandmarksSearch no. of matches: %d");
            TBuf<100> info;
            info.Format(KExpected, iter->NumOfItemsL());
            iLog->Log(info);
            info.Format(KReturned, iLandmarkSearch->NumOfMatches());
            iLog->Log(info);
           
            iLog->Log(KNumOfMatchesErr);
            User::Leave(-1);
            }
        TBuf<100> info2;
        _LIT(KNof, "No. of matches found before cancel = %d");
        info2.Format(KNof, iter->NumOfItemsL());
        iLog->Log(info2);

        AssertTrueSecL(iter->NumOfItemsL() != 0, KCancelErr);
        }
        
    CleanupStack::PopAndDestroy(2, areaCriteria);
    
    }

//  End of File
