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
#include "FT_CPosTp23.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkSearch.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLMItemIterator.h>
#include <EPos_CPosLmDisplayData.h>
#include <EPos_CPosLmDisplayItem.h>
#include <EPos_TPosLMSortPref.h>
#include "FT_CSearchResult.h"
     
// CONSTANTS

// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp23::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp23::CloseTest()
    {
    //CHECK TR   Delete of landmarks search - ESLI-5WNG9N
    delete iDatabase;
    iDatabase = NULL;

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    
    iSearchResults.ResetAndDestroy();
    }

// ---------------------------------------------------------
// CPosTp23::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp23::StartL()
    {
    _LIT(KEmptyTextCriteriaErr, "An empty string as text criteria should not be allowed");

    iDatabase = UseGeneratedDbFileL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
        
    TestInvalidPositionFieldsL();
        
    // Check iterator before search has started (in multidb search all iterators are empty thus NOT NULL)
    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    if (iter)
    	{
    	CleanupStack::PushL(iter);
    	AssertTrueSecL(iter->NumOfItemsL() == 0, _L("Iterator should be empty"));
    	CleanupStack::PopAndDestroy(iter);
    	}
    else
    	{
    	
    	iLog->Log(_L("Iterator should NOT be NULL"));
    	User::Leave(-1);	
    	}
    
    TUint attr = CPosLandmark::ELandmarkName | CPosLandmark::EDescription;
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    textCriteria->SetAttributesToSearch(attr);
    textCriteria->SetTextL(_L(""));
    
    TRAPD(err, iLandmarkSearch->StartLandmarkSearchL(*textCriteria));
    AssertTrueSecL(err == KErrArgument, KEmptyTextCriteriaErr);
    CleanupStack::PopAndDestroy(textCriteria);
    
    AppendSearchResultsL();
    
    iLog->Log(_L("Testing search with criterias read from LmDbSearchResult.txt syncronously"));
    SearchL(iSearchResults, ESynchronous);

    iLog->Log(_L("Testing search with criterias read from LmDbSearchResult.txt asyncronously"));
    SearchL(SearchResults(), EAsynchronous);

    iLog->Log(_L("Testing search with criterias read from LmDbSearchResult.txt asyncronously using User::WaitForRequest"));
    SearchL(SearchResults(), EWaitForRequest);

    iLog->Log(_L("Testing search with a defined sortorder"));
    SearchWithSortOrderL(iSearchResults);   

    iLog->Log(_L("Testing search anc cancel"));
    SearchAndCancelL();
    
    iLog->Log(_L("Testing search and cancel in callback"));
    SearchAndCancelL(ETrue);

    iLog->Log(_L("Testing a redefined search after a landmark has been removed"));
    SearchAndDeleteL(iSearchResults);

    iLog->Log(_L("Testing a redefined search after landmarks have been renamed"));
    SearchAndRenameL(iSearchResults);
    
    iLog->Log(_L("Test max length for SetTextL"));
    TestMaxLengthL();
    
    // Verify error report "ESLI-62DBXR Landmarks - Sort order is not correctly implemented"
    // Check that CPosLandmarkSearch::MatchIteratorL are CPosLandmarkDatabase::LandmarkIteratorL
    // are sorted correctly (should be identically sorted)
	VerifySortOrderL();
    }

// ---------------------------------------------------------
// CPosTp23::SearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp23::SearchL(
    const RPointerArray<CSearchResult>& aSearchResults, 
    TExecutionMode aExecutionMode)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
    _LIT(KNotFoundErr, "Id %d not found in parsed Lm search results when the search is performed syncronously");

    // Set DisplayData
	CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
	CleanupStack::PushL(displayData);
	iLandmarkSearch->SetDisplayData(*displayData);
    	
    for(TInt i=0; i<aSearchResults.Count(); i++)
        {
        CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
        
        textCriteria->SetTextL(aSearchResults[i]->TextCriteria());
        textCriteria->SetAttributesToSearch(aSearchResults[i]->Attributes());
        
        textCriteria->SetPositionFieldsToSearchL(aSearchResults[i]->PositionFields()); 

        CPosLmOperation* operation = 
            iLandmarkSearch->StartLandmarkSearchL(*textCriteria, aSearchResults[i]->Redefined());
        CleanupStack::PushL(operation);
         switch (aExecutionMode)
            {
            case ESynchronous:
                ExecuteAndDeleteLD(operation);
                CleanupStack::Pop(operation);
                break;
            case EAsynchronous:
                RunAsyncOperationLD(operation);
                CleanupStack::Pop(operation);
                break;
            case EWaitForRequest:
                RunAsyncOperationByWaitForReqL(operation);
                CleanupStack::PopAndDestroy(operation);                
                break;
            }

        CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
        CleanupStack::PushL(iter);
		       
        if ((TUint)(aSearchResults[i]->SearchResult()).Count() != iLandmarkSearch->NumOfMatches())
            {
            _LIT(KExpected, "Expected no. of matches: %d");
            _LIT(KReturned, "Returned no. of matches: %d");
            TBuf<100> info;
            info.Format(KExpected, (aSearchResults[i]->SearchResult()).Count());
            iLog->Log(info);
            info.Format(KReturned, iLandmarkSearch->NumOfMatches());
            iLog->Log(info);
            iLog->Log(_L("Search criteria: "));
            iLog->Log(textCriteria->Text());
            iLog->Log(KNumOfMatchesErr);
            User::Leave(-1);
            
            }

		//Check displaydata        
        AssertTrueSecL(iter->NumOfItemsL() == displayData->Count(), _L("Wrong number of DisplayData matches"));
        
       	TInt itemId = displayData->NewItemIndex();
        while (itemId != KPosLmNoNewItems)
            {
            CPosLmDisplayItem& item = displayData->DisplayItem(itemId);
                       
            if (item.DisplayItemType() != CPosLmDisplayItem::ELandmarkItem)
            	{
            	iLog->Log(_L("ERROR :::::"));
                iLog->Log(_L("Wrong item returned, only ELandmarkItem should be returned"));
            	User::Leave(-1);
            	}
            
            if (item.DatabaseIndex() != 0)
            	{
            	// When single search, index should always be 0
                iLog->Log(_L("Wrong DatabaseIndex, should be 0"));
            	User::Leave(-1);
            	
            	}
            
			
				TInt lmId = item.Landmark().LandmarkId();
				
        	AssertTrueSecL(aSearchResults[i]->FindSearchResult(lmId), KNotFoundErr, lmId);
            itemId = displayData->NewItemIndex();
           	}
            	
        TPosLmItemId id = iter->NextL();
        while (id != KPosLmNullItemId)
            {
            AssertTrueSecL(aSearchResults[i]->FindSearchResult(id), KNotFoundErr, id);
                 	
            id = iter->NextL();
            }
        
        CleanupStack::PopAndDestroy(iter);
        CleanupStack::PopAndDestroy(textCriteria);
        }
        iLandmarkSearch->UnsetDisplayData();
        CleanupStack::PopAndDestroy(displayData);
    }

// ---------------------------------------------------------
// CPosTp23::VerifySortOrderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp23::VerifySortOrderL()
    {
    iLog->Log(_L("VerifySortOrderL"));
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect");
    _LIT(KNumOfMatchesErr2, "No. of matches in DisplayData is incorrect");
    _LIT(KSortorderErr, "The sortorder is incorrect");
    _LIT(KNameMatchError, "The landmark names does not match");
    _LIT(KNameMatchError2, "The landmark name in DisplayData does not match");
    
    // Just in case, add some landmarks
    CPosLandmark* landmark = CPosLandmark::NewLC();
    landmark->SetLandmarkNameL(_L("CategoryA")); 
    TPosLmItemId id1 = iDatabase->AddLandmarkL(*landmark);

    CleanupStack::PopAndDestroy(landmark);

    landmark = CPosLandmark::NewLC();
    landmark->SetLandmarkNameL(_L("Categorya")); 
    TPosLmItemId id2 = iDatabase->AddLandmarkL(*landmark);
    CleanupStack::PopAndDestroy(landmark);

    landmark = CPosLandmark::NewLC();
    landmark->SetLandmarkNameL(_L("AA")); 
    TPosLmItemId id3 = iDatabase->AddLandmarkL(*landmark);
    CleanupStack::PopAndDestroy(landmark);

    landmark = CPosLandmark::NewLC();
    landmark->SetLandmarkNameL(_L("aa")); 
    TPosLmItemId id4 = iDatabase->AddLandmarkL(*landmark);
    CleanupStack::PopAndDestroy(landmark);
   
    TPosLmSortPref sortDesc(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
    TPosLmSortPref sortAsc(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);

    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    // Search pattern "*" should match all existing landmarks -> it is possible to compare
    // sort order of CPosLandmarkSearch::MatchIteratorL with CPosLandmarkDatabase::LandmarkIteratorL
    textCriteria->SetTextL(_L("*"));
    
    // Set DisplayData
    CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
	CleanupStack::PushL(displayData);
    iLandmarkSearch->SetDisplayData(*displayData);

    CPosLmItemIterator* lmDbIter = NULL;

    for (TInt i=0; i<6; i++)
        {
        if(i==0)
            {
            // Sorted ascending
            lmDbIter = iDatabase->LandmarkIteratorL(sortAsc);
            CleanupStack::PushL(lmDbIter);
            ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(
                *textCriteria, sortAsc, EFalse));
            }
        else if (i==1)
            {
            // Sorted descending
            lmDbIter = iDatabase->LandmarkIteratorL(sortDesc);
            CleanupStack::PushL(lmDbIter);
            ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(
                *textCriteria, sortDesc, EFalse));
            }
        else if (i==2)
            {
            //**** Async search, sorted ascending
            lmDbIter = iDatabase->LandmarkIteratorL(sortAsc);
            CleanupStack::PushL(lmDbIter);
            RunAsyncOperationLD(iLandmarkSearch->StartLandmarkSearchL(
                *textCriteria, sortAsc, EFalse));
            }
        else if (i==3)
            {
            //**** Async search, sorted descending
            lmDbIter = iDatabase->LandmarkIteratorL(sortDesc);
            CleanupStack::PushL(lmDbIter);
            RunAsyncOperationLD(iLandmarkSearch->StartLandmarkSearchL(
                *textCriteria, sortDesc, EFalse));
            }
        else if (i==4)
            {
            //**** Async search with User::WaitForRequest(), sorted ascending
            lmDbIter = iDatabase->LandmarkIteratorL(sortAsc);
            CleanupStack::PushL(lmDbIter);
            CPosLmOperation* op = iLandmarkSearch->StartLandmarkSearchL(
                *textCriteria, sortAsc, EFalse);
            CleanupStack::PushL(op);
            RunAsyncOperationByWaitForReqL(op);
            CleanupStack::PopAndDestroy(op);
            }
        else if (i==5)
            {
            //**** Async search with User::WaitForRequest(), sorted descending
            lmDbIter = iDatabase->LandmarkIteratorL(sortDesc);
            CleanupStack::PushL(lmDbIter);
            CPosLmOperation* op = iLandmarkSearch->StartLandmarkSearchL(
                *textCriteria, sortDesc, EFalse);
            CleanupStack::PushL(op);
            RunAsyncOperationByWaitForReqL(op);
            CleanupStack::PopAndDestroy(op);
            }

        CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
        CleanupStack::PushL(iter);

        AssertTrueSecL(iter->NumOfItemsL() == lmDbIter->NumOfItemsL(), KNumOfMatchesErr);
        // Check correct displayData
        AssertTrueSecL(iter->NumOfItemsL() == displayData->Count(), KNumOfMatchesErr2);

		TInt itemId = displayData->NewItemIndex();

        TPosLmItemId id = iter->NextL();
        TPosLmItemId lmDbId = lmDbIter->NextL();
        // check that the two iterators are sorted identically
        while (id != KPosLmNullItemId)
            {
            CPosLandmark* source = iDatabase->ReadLandmarkLC(id);
            CPosLandmark* target = iDatabase->ReadLandmarkLC(lmDbId);
                   	
        	CPosLmDisplayItem& item = displayData->DisplayItem(itemId);
			TPtrC lmName;
			item.Landmark().GetLandmarkName(lmName);
            
            TPtrC sourceName, targetName;
            source->GetLandmarkName(sourceName);
            target->GetLandmarkName(targetName);
    		
            // For debugging purposes
            TBuf<150> buf;
            buf.Append(_L("Name:"));
            buf.Append(sourceName);
            buf.Append(_L("<->"));
            buf.Append(targetName);
            buf.Append(_L("<->"));
            buf.Append(lmName);
            //iLog->Log(buf);
            
           
            if (id != lmDbId) iLog->Log(_L("ids are not identical, therefore check name"));
            // Should always be the same
            AssertTrueSecL(sourceName.Compare(targetName) == KErrNone, KNameMatchError);
            AssertTrueSecL(sourceName.Compare(lmName) == KErrNone, KNameMatchError2);
            
            CleanupStack::PopAndDestroy(target);
            CleanupStack::PopAndDestroy(source);
            id = iter->NextL();
            lmDbId = lmDbIter->NextL();
            itemId = displayData->NewItemIndex();
            }
        CleanupStack::PopAndDestroy(iter);
        CleanupStack::PopAndDestroy(lmDbIter);
        }
    // Unset displayData before deleting
    iLandmarkSearch->UnsetDisplayData();
	CleanupStack::PopAndDestroy(displayData);
    CleanupStack::PopAndDestroy(textCriteria);

    //Remove landmarks created in this part
    iDatabase->RemoveLandmarkL(id1);
    iDatabase->RemoveLandmarkL(id2);
    iDatabase->RemoveLandmarkL(id3);
    iDatabase->RemoveLandmarkL(id4);

    iLog->Log(_L("VerifySortOrderL Done"));
    }

// ---------------------------------------------------------
// CPosTp23::SearchWithSortOrderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp23::SearchWithSortOrderL(const RPointerArray<CSearchResult>& aSearchResults)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed syncronously");
    _LIT(KSortorderErr, "The sortorder is incorrect");
   
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);  
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
       
    for(TInt j=0; j<aSearchResults.Count(); j++)
        {   
        textCriteria->SetTextL(aSearchResults[j]->TextCriteria());
        textCriteria->SetAttributesToSearch(aSearchResults[j]->Attributes());
        textCriteria->SetPositionFieldsToSearchL(aSearchResults[j]->PositionFields()); 

        //iLog->Log(aSearchResults[j]->TextCriteria());
        
        for (TInt i=0; i<6; i++)
            {
            switch (i)
                {
                //**** sync
                case 0:
                    ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(
                        *textCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined()));
                    break;
                case 1:
                    sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
                    ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(
                        *textCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined()));
                    break;
                case 2:
                //**** async
                    RunAsyncOperationLD(iLandmarkSearch->StartLandmarkSearchL(
                        *textCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined()));
                    break;
                case 3:
                    sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
                    RunAsyncOperationLD(iLandmarkSearch->StartLandmarkSearchL(
                        *textCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined()));
                    break;
                //**** asych with User::WaitForRequest()
                case 4:
                    {
                    CPosLmOperation* op = iLandmarkSearch->StartLandmarkSearchL(
                        *textCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined());
                    CleanupStack::PushL(op);
                    RunAsyncOperationByWaitForReqL(op);
                    CleanupStack::PopAndDestroy(op);
                    }
                    break;
                case 5:
                    {
                    sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
                    CPosLmOperation* op = iLandmarkSearch->StartLandmarkSearchL(
                        *textCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined());
                    CleanupStack::PushL(op);
                    RunAsyncOperationByWaitForReqL(op);
                    CleanupStack::PopAndDestroy(op);
                    }
                    break;
                }
           
        CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
        CleanupStack::PushL(iter);
        
        RArray<TPosLmItemId>* searchResults = &aSearchResults[j]->SearchResult();
        if ((TUint)searchResults->Count() != iLandmarkSearch->NumOfMatches())
            {
            TBuf<50> buf;
            buf.Append(_L("KNumOfMatchesError when searching for "));
            buf.Append(aSearchResults[j]->TextCriteria());
            iLog->Log(buf);
            
            iLog->Log(KNumOfMatchesErr);
            User::Leave(-1);
            }
        
        LandmarksSortL(*searchResults, sortPref);
        
        TInt ii=0;
        TPosLmItemId id = iter->NextL();
        
        while (id != KPosLmNullItemId)
            {  
            if (id != (*searchResults)[ii])
                {
                //Check if same name then don't Leave
                CPosLandmark* source = iDatabase->ReadLandmarkLC(id);
                CPosLandmark* target = iDatabase->ReadLandmarkLC((*searchResults)[ii]);
                TPtrC sourceName, targetName;
                source->GetLandmarkName(sourceName);
                target->GetLandmarkName(targetName);

                AssertTrueSecL(!sourceName.CompareC(targetName),KSortorderErr, id);
                CleanupStack::PopAndDestroy(2, source);
                }
            id = iter->NextL();
            ++ii;
            }
        CleanupStack::PopAndDestroy(iter);
        } // for (TInt i=0; i<6; i++)
        } // for(TInt j=0; j<aSearchResults.Count(); j++)
    CleanupStack::PopAndDestroy(textCriteria);
    }

// ---------------------------------------------------------
// CPosTp23::SearchAndDeleteL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp23::SearchAndDeleteL(const RPointerArray<CSearchResult>& aSearchResults)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when a redefined search is performed"); 
    _LIT(KSortorderErr, "The sortorder is incorrect after a delete when a redefined search is performed");
    
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
    
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
        
    textCriteria->SetTextL(aSearchResults[11]->TextCriteria());
    textCriteria->SetAttributesToSearch(aSearchResults[11]->Attributes());
    textCriteria->SetPositionFieldsToSearchL(aSearchResults[11]->PositionFields()); 
    
    ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref));
    
    RArray<TPosLmItemId>* searchResults = &aSearchResults[11]->SearchResult();
    
    iDatabase->RemoveLandmarkL((*searchResults)[0]); 
    searchResults->Remove(0);
    searchResults->Compress();

    ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref, ETrue));
    
    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);

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
       
    CleanupStack::PopAndDestroy(2, textCriteria);
    }

// ---------------------------------------------------------
// CPosTp23::SearchAndRenameL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp23::SearchAndRenameL(const RPointerArray<CSearchResult>& aSearchResults)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when a redefined search is performed"); 
    const TInt KTestIndex=4;

    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
    
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
        
    textCriteria->SetTextL(aSearchResults[KTestIndex]->TextCriteria());
    textCriteria->SetAttributesToSearch(aSearchResults[KTestIndex]->Attributes());
    textCriteria->SetPositionFieldsToSearchL(aSearchResults[KTestIndex]->PositionFields()); 
    
    ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref));
    
    RArray<TPosLmItemId>* searchResults = &aSearchResults[KTestIndex]->SearchResult();
    
    _LIT(KLmName, "LmTP23-%d");
    for (TInt i=0; i<searchResults->Count(); i++)
        {
        CPosLandmark* lm =iDatabase->ReadLandmarkLC((*searchResults)[i]);
        
        TBuf<100> lmName;
        lmName.Format(KLmName,i);

        lm->SetLandmarkNameL(lmName);
        iDatabase->UpdateLandmarkL(*lm);
        CleanupStack::PopAndDestroy(lm);   
        }
    
    ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref, ETrue));
    
    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);

   
             
    AssertTrueSecL(iLandmarkSearch->NumOfMatches()==0, KNumOfMatchesErr);    

    CleanupStack::PopAndDestroy(2, textCriteria);
   
    }

// ---------------------------------------------------------
// CPosTp23::SearchAndCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp23::SearchAndCancelL(const TBool& aInCallback)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
    _LIT(KCancelErr, "The search is canceled after progress has increased but no search result is found");

    // Test search with one criteria
    const TInt KIndex=0;

    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
        
    textCriteria->SetTextL(iSearchResults[KIndex]->TextCriteria());
    textCriteria->SetAttributesToSearch(iSearchResults[KIndex]->Attributes());
        
    textCriteria->SetPositionFieldsToSearchL(iSearchResults[KIndex]->PositionFields()); 

    if (aInCallback)
        {
        RunAsyncOperationAndCancelInCallbackLD(iLandmarkSearch->StartLandmarkSearchL(
                *textCriteria, iSearchResults[KIndex]->Redefined()));
        }
    else
        {
        RunAsyncOperationAndCancelLD(iLandmarkSearch->StartLandmarkSearchL(
                *textCriteria, iSearchResults[KIndex]->Redefined()));
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
            iLog->Log(_L("Search criteria: "));
            iLog->Log(textCriteria->Text());
            
            iLog->Log(KNumOfMatchesErr);
            User::Leave(-1);
            }
        TBuf<100> info2;
        _LIT(KNof, "No. of matches found before cancel = %d");
        info2.Format(KNof, iter->NumOfItemsL());
        iLog->Log(info2);

        AssertTrueSecL(iter->NumOfItemsL() != 0, KCancelErr);
        }
        
    CleanupStack::PopAndDestroy(2, textCriteria);
       
    }
    
// ---------------------------------------------------------
// CPosTp23::TestMaxLengthL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp23::TestMaxLengthL()
	{
	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
	_LIT(K255Error, "Exceeding limit should leave with KErrArgument");
    
	_LIT(K255CHAR, "ABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJ12345");
	_LIT(K256CHAR, "ABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJABCDEFGHIJ123456");

	textCriteria->SetTextL(K255CHAR);
	
	TRAPD(err, textCriteria->SetTextL(K256CHAR););
	AssertTrueSecL(err == KErrArgument, K255Error);

    iOperation = iLandmarkSearch->StartLandmarkSearchL(*textCriteria);
    ExecuteAndDeleteLD(iOperation);
    
    TInt nr = iLandmarkSearch->NumOfMatches();
    TBuf<100> buf;
    buf.Format(_L("Found %d nr of matches"), nr);
    iLog->Log(buf);

	AssertTrueSecL(nr == 0, _L("We should not find any matches"));	
    CleanupStack::PopAndDestroy(textCriteria);	    
	}

// ---------------------------------------------------------
// CPosTp23::TestInvalidPositionFieldsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp23::TestInvalidPositionFieldsL()
	{
	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
	RArray<TUint> arr;
	CleanupClosePushL(arr);
		
	textCriteria->SetTextL(_L("*"));
	textCriteria->SetPositionFieldsToSearchL(arr);

	// Search with empty positionfields list, should find all landmarks
	iOperation = iLandmarkSearch->StartLandmarkSearchL(*textCriteria);
    ExecuteAndDeleteLD(iOperation);
    TInt nr = iLandmarkSearch->NumOfMatches();
    TBuf<100> buf;
    buf.Format(_L("Found %d nr of matches when searching in empty position fields list"), nr);
    iLog->Log(buf);
    
    	
		
	User::LeaveIfError(arr.Append(0));
	User::LeaveIfError(arr.Append(50));
	User::LeaveIfError(arr.Append(500));
	textCriteria->SetPositionFieldsToSearchL(arr);

	// Search with invalid positionfields list, should NOT find any landmarks
	iOperation = iLandmarkSearch->StartLandmarkSearchL(*textCriteria);
    ExecuteAndDeleteLD(iOperation);
    nr = iLandmarkSearch->NumOfMatches();

    buf.Format(_L("Found %d nr of matches when searching in invalid position fields"), nr);
    iLog->Log(buf);
    
    AssertTrueSecL(nr == 0, _L("We should not find any matches"));
    
    CleanupStack::PopAndDestroy(&arr);
    CleanupStack::PopAndDestroy(textCriteria);
	}

//  End of File
