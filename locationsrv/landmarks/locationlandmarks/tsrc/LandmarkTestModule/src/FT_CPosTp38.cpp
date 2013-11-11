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
#include "FT_CPosTp38.h"
#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLmCompositeCriteria.h>
#include <EPos_CPosLmCategoryCriteria.h>
#include <EPos_CPosLmCatNameCriteria.h>
#include <EPos_CPosLmNearestCriteria.h>
#include <EPos_CPosLmAreaCriteria.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmIdListCriteria.h>
#include "FT_LandmarkConstants.h" 

#include "FT_CCompositeSearchResult.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp38::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp38::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    delete iDatabase;
    iDatabase=NULL;

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;

    }

// ---------------------------------------------------------
// CPosTp38::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp38::StartL()
    {
    TTime startTime;
    TTime stopTime;
    startTime.UniversalTime();
    
    iDatabase = UseCompositeLandmarksDbFileL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

    StartLandmarksCompositeTestL();

    TestCompositeInCompositeL();

    TestSeveralTextArgumentsL();
    
    TestSeveralTextArgumentsWithORL();

	TestSortOrderWithORL();
	
    TestWithSortOrderLandmarksL();
        
    TestWithSortOrderNearestL();
    
    TestTwoLmCategoryCriteriaL();

    TestAddAndRemoveL();

    TInt err = KErrNone;
    TRAP(err, TestWithNoPreviousResultL(0));
    if (err != KErrArgument) 
    
    {
    	iLog->Log(_L("Searching with no previous result should cause StartLandmarkL to leave with KErrArgument"));
    	User::Leave(err);
    }
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;

    SearchAndCancelL(EFalse);
    SearchAndCancelL(ETrue);

    // Since category and composite is no longer allowed, no need to test this
    //TestWithSortOrderCategoriesL();
    //TestCategoryCompositeSearchL();

    stopTime.UniversalTime();
    TTimeIntervalMicroSeconds executionTime = 
        stopTime.MicroSecondsFrom(startTime);

    TInt resse = executionTime.Int64()/1000000;
    TBuf<100> buf;
    buf.Format(_L("TP38 took %d seconds"), resse);
    iLog->Log(buf);
	}

// ---------------------------------------------------------
// CPosTp38::TestWithSortOrderCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp38::TestWithSortOrderCategoriesL()
	{
	iLog->Log(_L("TestWithSortOrderCategoriesL"));
	
	iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
    nameCriteria->SetSearchPatternL(_L("*"));
    TInt err = composite->AddArgument(nameCriteria);
    if (err != KErrNone) 
    {
    	iLog->Log(_L("Error from AddArgument(nameCriteria)"));
    	User::Leave(err);
    }
    CleanupStack::Pop(nameCriteria);
    
    CPosLmCategoryManager::TCategorySortPref sortPref;
    sortPref = CPosLmCategoryManager::ECategorySortOrderNameAscending;
	TInt i=0;
	TInt j=0;    
    for (j=0;j<2;j++)
    	{
	  	if (j==0) 
	    	{
	    	iLog->Log(_L("Sorting Ascending"));
	    	sortPref = CPosLmCategoryManager::ECategorySortOrderNameAscending;
	    	}
	    else 
	    	{
	    	iLog->Log(_L("Sorting Descending"));
	    	sortPref = CPosLmCategoryManager::ECategorySortOrderNameDescending;
	    	}
	    	
    	iLog->Log(_L("iLandmarkSearch->StartCategorySearchL"));
    	iOperation = iLandmarkSearch->StartCategorySearchL(*composite, sortPref);

    	ExecuteAndDeleteLD(iOperation);

    	CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    	CleanupStack::PushL(iter);
    
        // Create a sorted list out of the search result
        RArray<TPosLmItemId> searchResults;
        CleanupClosePushL(searchResults);
        
        for (i=1;i<27;i++) {searchResults.Append(i);}

        CategoriesSortL(searchResults, sortPref);
        
        TPosLmItemId id = iter->NextL();
        TInt counter=0;
        while (id != KPosLmNullItemId)
            {
            // Check that sort order is correct
            AssertTrueSecL(searchResults[counter] == id, _L("Error in sorting"));
            id = iter->NextL();
            counter++;
            }
        CleanupStack::PopAndDestroy(&searchResults);
        CleanupStack::PopAndDestroy(iter);
		}          
    CleanupStack::PopAndDestroy(composite);

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
        
    iLog->Log(_L("Done"));
	}

// ---------------------------------------------------------
// CPosTp38::TestWithSortOrderNearestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp38::TestWithSortOrderNearestL()
	{
	
	iLog->Log(_L("TestWithSortOrderNearestL"));
	
	iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

    // First create a textcriteria
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();   
    textCriteria->SetTextL(_L("*"));
    TInt err = composite->AddArgument(textCriteria);
    if (err != KErrNone)
    {
    iLog->Log(_L("Error when adding argument1"));
    User::Leave(err);
    }
    CleanupStack::Pop(textCriteria);
    
    // Then create a nearest criteria same coordinate as landmark itemid=3 God
    TCoordinate coord(45, 45);
    CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(coord, EFalse);
    err = composite->AddArgument(nearestCriteria);
    if (err != KErrNone)
    
    {
    	iLog->Log(_L("Error when adding argument2"));
    	User::Leave(err);
    
    }
    CleanupStack::Pop(nearestCriteria);
        
    // Then add an IdList
    CPosLmIdListCriteria* idList = CPosLmIdListCriteria::NewLC();
    RArray<TPosLmItemId> list;
    CleanupClosePushL(list);
    
    // Use these four landmarks
    list.Append(6);
    list.Append(7);
    list.Append(3);
    list.Append(10);
    
    idList->SetLandmarkIdsL(list);
    err = composite->AddArgument(idList);
    if (err != KErrNone) 
    
    {
    	iLog->Log(_L("Error when adding argument3"));
    	User::Leave(err);
    
    }
    CleanupStack::PopAndDestroy(&list);
    CleanupStack::Pop(idList);
    
    // Create search result
   	RArray<TPosLmItemId> searchResults;
	CleanupClosePushL(searchResults);
	// This is the correct distance sorted array	
	searchResults.Append(3);
	searchResults.Append(7);
	searchResults.Append(6);
	searchResults.Append(10);

    iOperation = iLandmarkSearch->StartLandmarkSearchL(*composite, EFalse);
    ExecuteAndDeleteLD(iOperation);
          
    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
    
        
    TInt ii=0;
    TPosLmItemId id = iter->NextL();
            
    while (id != KPosLmNullItemId)
    	{
        if (id != (searchResults)[ii])
        	{
            // Check if same name then don't leave, there are three landmarks with same name, Mölndal
            CPosLandmark* source = iDatabase->ReadLandmarkLC(id);
            CPosLandmark* target = iDatabase->ReadLandmarkLC((searchResults)[ii]);
            TPtrC sourceName, targetName;
            source->GetLandmarkName(sourceName);
            TInt sourceId = source->LandmarkId();
            target->GetLandmarkName(targetName);
            TInt targetId = target->LandmarkId();
			
                        
            AssertTrueSecL(sourceName.CompareC(targetName) == KErrNone,_L("Wrong sortorder"), id);
            CleanupStack::PopAndDestroy(2, source);
            }

        id = iter->NextL();
        ++ii;
		}
	CleanupStack::PopAndDestroy(iter);	      	
    CleanupStack::PopAndDestroy(&searchResults);
    CleanupStack::PopAndDestroy(composite);
    
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
	}

// ---------------------------------------------------------
// CPosTp38::TestTwoLmCategoryCriteriaL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp38::TestTwoLmCategoryCriteriaL()
	{
	iLog->Log(_L("TestTwoLmCategoryCriteriaL"));
	
	iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
	
    // First create a textcriteria
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();   
    textCriteria->SetTextL(_L("*"));
    TInt err = composite->AddArgument(textCriteria);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument1"));
    	User::Leave(err);
     }
    CleanupStack::Pop(textCriteria);
    
	// Create a cat search criteria
	CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();
    iLog->Log(_L("SetCategoryItemId"));
    catSearchCriteria->SetCategoryItemId(14);
    composite->AddArgument(catSearchCriteria);
    CleanupStack::Pop(catSearchCriteria);
    
   	// Create another cat search criteria
	CPosLmCategoryCriteria* catSearchCriteria2 = CPosLmCategoryCriteria::NewLC();
    iLog->Log(_L("SetCategoryNameL"));
    catSearchCriteria2->SetCategoryNameL(_L("McDonalds"));
    composite->AddArgument(catSearchCriteria2);
    CleanupStack::Pop(catSearchCriteria2);
    
    // Should find id7=Billigt and id27=LOG9
    iOperation = iLandmarkSearch->StartLandmarkSearchL(*composite, EFalse);
    ExecuteAndDeleteLD(iOperation);
	
	// Create search result
   	RArray<TPosLmItemId> searchResults;
	CleanupClosePushL(searchResults);
	          
    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
    
    // This is the correct result list    
	searchResults.Append(27);
	searchResults.Append(7);
	
	TInt size = iter->NumOfItemsL();
	if (size != searchResults.Count()) 
	
	{
		iLog->Log(_L("Wrong result returned from iterator"));
    	User::Leave(-1);
    
	}

	TInt found = 0;
	TInt ii=0;
    TPosLmItemId id = iter->NextL();
    
    while (id != KPosLmNullItemId)
    	{
    	for (ii=0;ii<size;ii++)
    		{
        	if (id == (searchResults)[ii])
        		{
	            found++;
	            }
	       	}
        	id = iter->NextL();
		}
		
		if (found != searchResults.Count()) 
		
		{
			iLog->Log(_L("Wrong result returned from iterator1"));
    		User::Leave(-1);
    	
		}
		
	// Now refine search with another CPosLmCategoryCriteria
	// Create another cat search criteria 
	CPosLmCategoryCriteria* catSearchCriteria3 = CPosLmCategoryCriteria::NewLC();
    iLog->Log(_L("SetCategoryItemId"));
    catSearchCriteria3->SetCategoryItemId(11);
    composite->AddArgument(catSearchCriteria3);
    CleanupStack::Pop(catSearchCriteria3);
    
    // Should find id27=LOG9
	iOperation = iLandmarkSearch->StartLandmarkSearchL(*composite, ETrue);
    ExecuteAndDeleteLD(iOperation);
    
    iter->Reset();
    CleanupStack::PopAndDestroy(iter);
    iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
        
    searchResults.Reset();
    searchResults.Append(27);
    found=0;
    size = iter->NumOfItemsL();
    id = iter->NextL();
	if (size != searchResults.Count()) 
	
	{
			iLog->Log(_L("Wrong result returned from iterator"));
    		User::Leave(-1);
    
	}
    
        while (id != KPosLmNullItemId)
    	{
    	for (ii=0;ii<size;ii++)
    		{
        	if (id == (searchResults)[ii])
        		{
	            found++;
	            }
	       	}
        	id = iter->NextL();
		}
		
	if (found != searchResults.Count())
	
	{
	iLog->Log(_L("Wrong items returned from iterator2"));
    User::Leave(-1);
    }

	CleanupStack::PopAndDestroy(iter);
	CleanupStack::PopAndDestroy(&searchResults);    
    CleanupStack::PopAndDestroy(composite);
    
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
	}

// ---------------------------------------------------------
// CPosTp38::TestWithSortOrderLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp38::TestWithSortOrderLandmarksL()
	{
	iLog->Log(_L("TestWithSortOrderLandmarksL"));
    
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
	TInt i=0;
	TInt j=0;
	
	TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
	
    // First create a textcriteria
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();   
    textCriteria->SetTextL(_L("*"));
    TInt err = composite->AddArgument(textCriteria);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument1"));
    	User::Leave(err);
    }
    CleanupStack::Pop(textCriteria);
    
    // Then create a nearest criteria
    TCoordinate coord(12.123, -20,123);
    CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(coord, EFalse);
    err = composite->AddArgument(nearestCriteria);
    if (err != KErrNone) 
    {
    	iLog->Log(_L("Error when adding argument2"));
    	User::Leave(err);
    }
    CleanupStack::Pop(nearestCriteria);
        
    // Then add an IdList
    CPosLmIdListCriteria* idList = CPosLmIdListCriteria::NewLC();
    RArray<TPosLmItemId> list;
    CleanupClosePushL(list);
    for (i=1;i<106;i++) {list.Append(i);}
    idList->SetLandmarkIdsL(list);
    err = composite->AddArgument(idList);
    if (err != KErrNone) 
    {
    	iLog->Log(_L("Error when adding argument3"));
    	User::Leave(err);
    }
    CleanupStack::PopAndDestroy(&list);
    CleanupStack::Pop(idList);
    
    // Create search result of all landmark ids: 1-105
   	RArray<TPosLmItemId> searchResults;
	CleanupClosePushL(searchResults);
	for (i=1;i<106;i++) {searchResults.Append(i);}
    
    for (j=0;j<2;j++)
	    {
	    if (j==0) 
	    	{
	    	iLog->Log(_L("Sorting Ascending"));
	    	sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
	    	}
	    else 
	    	{
	    	iLog->Log(_L("Sorting Descending"));
	    	sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
	    	}

	    iOperation = iLandmarkSearch->StartLandmarkSearchL(*composite, sortPref, EFalse);
	    ExecuteAndDeleteLD(iOperation);
	          
	    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
	    CleanupStack::PushL(iter);
	    
	    TInt size = iter->NumOfItemsL();

	  	LandmarksSortL(searchResults, sortPref);
	        
	    TInt ii=0;
	    TPosLmItemId id = iter->NextL();
	            
	    while (id != KPosLmNullItemId)
	    	{	            
	        if (id != (searchResults)[ii])
	        	{
	            // Check if same name then don't leave, there are three landmarks with same name, Mölndal
	            
	            CPosLandmark* source = iDatabase->ReadLandmarkLC(id);
	            CPosLandmark* target = iDatabase->ReadLandmarkLC((searchResults)[ii]);
	            TPtrC sourceName, targetName;
	            source->GetLandmarkName(sourceName);
	            TInt sourceId = source->LandmarkId();
	            target->GetLandmarkName(targetName);
	            TInt targetId = target->LandmarkId();
			
	            AssertTrueSecL(sourceName.CompareC(targetName) == KErrNone,_L("Wrong sortorder"), id);
	            
	            CleanupStack::PopAndDestroy(2, source);
	            }

	        id = iter->NextL();
	        ++ii;
	      	}
    	CleanupStack::PopAndDestroy(iter);	      	
	    }
	    
    CleanupStack::PopAndDestroy(&searchResults);
    CleanupStack::PopAndDestroy(composite);
    
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
	}

// ---------------------------------------------------------
// CPosTp38::TestCompositeInCompositeL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp38::TestCompositeInCompositeL()
    {
    iLog->Log(_L("TestCompositeInCompositeL"));
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

    // First create a textcriteria
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    
    textCriteria->SetTextL(_L("*"));
   
    TInt err = composite->AddArgument(textCriteria);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument1"));
    	User::Leave(err);
    }
    CleanupStack::Pop(textCriteria);

    CPosLmAreaCriteria* areaCriteria = CPosLmAreaCriteria::NewLC(-48, 5, -5, 20);
                
    err = composite->AddArgument(areaCriteria);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument2"));
    	User::Leave(err);
    }
    CleanupStack::Pop(areaCriteria);

    //Create composite object that should be used as argument to the first composite object
    CPosLmCompositeCriteria* composite2 = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

    CPosLmTextCriteria* textCriteria2 = CPosLmTextCriteria::NewLC();
    
    textCriteria2->SetTextL(_L("TE,*"));
    
    err = composite2->AddArgument(textCriteria2);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument3"));
    	User::Leave(err);
    }
    CleanupStack::Pop(textCriteria2);

    // Add a IdList
    RArray<TPosLmItemId> list;
    CleanupClosePushL(list);
    
    CPosLmIdListCriteria* idList = CPosLmIdListCriteria::NewLC();
    
    for (TInt i=0;i<100;i++)
        {
        list.Append(i);
        }
    idList->SetLandmarkIdsL(list);
    
    err = composite2->AddArgument(idList);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument4"));
    	User::Leave(err);
    }
    CleanupStack::Pop(idList);
    CleanupStack::PopAndDestroy(&list);

    // Nested composite is not allowed
    err = composite->AddArgument(composite2);
    if (err != KErrNotSupported)
    {
    	iLog->Log(_L("Wrong error code when adding composite to a composite"));
    	User::Leave(err);
    }
    CleanupStack::PopAndDestroy(composite2);

    CleanupStack::PopAndDestroy(composite);

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    iLog->Log(_L("Done"));
    }

// ---------------------------------------------------------
// CPosTp38::StartLandmarksCompositeTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp38::StartLandmarksCompositeTestL()
    {       
    iLog->Log(_L("StartTestL"));
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

    CCompositeSearchResult* res = CCompositeSearchResult::NewL(iLog);
    CleanupStack::PushL(res);

    TInt index=0;
    TInt errorsFound = KErrNone;

    TBuf<100> buf;
    buf.Format(_L(">>>>>>>Test round %d <<<<<<<"), index);

    while (!res->GetCompositeSearchResultsL(*composite))
        {
        PrintCompositeInfo(composite);
        
        iLog->Log(_L("iLandmarkSearch->StartLandmarkSearchL"));    
        iOperation = iLandmarkSearch->StartLandmarkSearchL(*composite, res->Redefined());
        ExecuteAndDeleteLD(iOperation);

        CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
        CleanupStack::PushL(iter);
        TPosLmItemId id = iter->NextL();
        
        TInt matches = iLandmarkSearch->NumOfMatches();
        TInt expMatches = res->iResultIds.Count();

        buf.Format(_L("nr of matches: %d nr of exp matches: %d"), matches, expMatches);
        iLog->Log(buf);

        while (id != KPosLmNullItemId)
            {
            buf.Format(_L("Result from iterator id: %d"), id);
            iLog->Log(buf);
            TInt idde = res->iResultIds.Find(id);
            if (idde == KErrNotFound) 
                {
                iLog->Log(_L("MyError: Could not find id!!!"));
                errorsFound++;
                }
            else 
                {
                // Remove so that we dont find the same id again
                res->iResultIds.Remove(idde);
                }
            id = iter->NextL();
            }

        iter->Reset();
        CleanupStack::PopAndDestroy(iter);

        if (expMatches != matches) 
            {
            iLog->Log(_L("MyError: Wrong number of matches"));
            errorsFound++;
            }
    }

    if (errorsFound != KErrNone)
    {
    	iLog->Log(_L("Errors found in TP38"));
    	User::Leave(-1);

    }
    iLog->Log(_L("----Done---"));
    
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;

    CleanupStack::PopAndDestroy(res);
    CleanupStack::PopAndDestroy(composite);
    }


// ---------------------------------------------------------
// CPosTp38::TestSeveralTextArgumentsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp38::TestSeveralTextArgumentsL()
    {
    iLog->Log(_L("TestSeveralTextArgumentsL"));
    
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

    // First create a textcriteria
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();    
    textCriteria->SetTextL(_L("Nokia*"));
    TInt err = composite->AddArgument(textCriteria);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument1"));
    	User::Leave(err);
    }
    CleanupStack::Pop(textCriteria);

    // First create a textcriteria
    CPosLmTextCriteria* textCriteria2 = CPosLmTextCriteria::NewLC();    
    textCriteria2->SetTextL(_L("*Stockholm"));
    err = composite->AddArgument(textCriteria2);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument2"));
    	User::Leave(err);
    }
    CleanupStack::Pop(textCriteria2);

    // Should find landmark id: 63 name: "Nokia, Stockholm"
    iLog->Log(_L("iLandmarkSearch->StartLandmarkSearchL"));    
    iOperation = iLandmarkSearch->StartLandmarkSearchL(*composite, EFalse);
    ExecuteAndDeleteLD(iOperation);
    
    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
    TPosLmItemId id = iter->NextL();
    
    TInt matches = iLandmarkSearch->NumOfMatches();
    if (matches != 1) 
    {
    	iLog->Log(_L("Wrong number of matches"));
    	User::Leave(-1);
    }

    if (id != 63)
    {
    	
    	iLog->Log(_L("Wrong landmark found, should find landmark with id 63"));
    	User::Leave(-1);
    }
    
    iter->Reset();
    CleanupStack::PopAndDestroy(iter);

    CleanupStack::PopAndDestroy(composite);

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    }

// ---------------------------------------------------------
// CPosTp38::TestSeveralTextArgumentsWithORL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp38::TestSeveralTextArgumentsWithORL()
    {
    iLog->Log(_L("TestSeveralTextArgumentsWithORL"));
    
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionOR);

    // First create a textcriteria
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();    
    textCriteria->SetTextL(_L("Nokia*"));
    textCriteria->SetAttributesToSearch(CPosLandmark::ELandmarkName);
    TInt err = composite->AddArgument(textCriteria);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument1"));
    	User::Leave(err);
    }
    CleanupStack::Pop(textCriteria);

    // First create a textcriteria
    CPosLmTextCriteria* textCriteria2 = CPosLmTextCriteria::NewLC();    
    textCriteria2->SetTextL(_L("*Eriksberg"));
    textCriteria2->SetAttributesToSearch(CPosLandmark::ELandmarkName);
    err = composite->AddArgument(textCriteria2);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument2"));
    	User::Leave(err);
    }
    CleanupStack::Pop(textCriteria2);
    
    //Create nearest criteria instance and try adding it to composite criteria,should leave with KErrNotSupported since ORcomposition
    // is supported only for text criteria
    TCoordinate coord(45, 45);
    CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(coord, EFalse);
    err = composite->AddArgument(nearestCriteria);
    if (err != KErrNotSupported)
    
    {
    	iLog->Log(_L("Error,should not support any other criteria except Text criteria"));
    	User::Leave(err);
    
    }
    CleanupStack::Pop(nearestCriteria);
    
    
    
    

    // Should find landmark ids 48,55,62 & 63 with name:"Nokia","TE Eriksberg","Nokia Copenhagen" "Nokia, Stockholm"
    iLog->Log(_L("iLandmarkSearch->StartLandmarkSearchL"));    
    iOperation = iLandmarkSearch->StartLandmarkSearchL(*composite, EFalse);
    ExecuteAndDeleteLD(iOperation);
    
    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
    
    RArray<TPosLmItemId> idArray;
    iter->GetItemIdsL(idArray, 0, iter->NumOfItemsL());
            
    
    TInt matches = iLandmarkSearch->NumOfMatches();
    if (matches != 4) 
    {
    	iLog->Log(_L("Wrong number of matches"));
    	User::Leave(-1);
    }

	for(int i=0; i<idArray.Count(); i++ )
	{
		if(idArray[i] != 48 && idArray[i] != 55 && idArray[i] != 62 && idArray[i] != 63)
		{
			iLog->Log(_L("Wrong landmark found, should find landmark with id 48, 55, 62 & 63"));
    		User::Leave(-1);
		}
	}
       
    iter->Reset();
    CleanupStack::PopAndDestroy(iter);
    CleanupStack::PopAndDestroy(composite);
	delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    }
    
    
// ---------------------------------------------------------
// CPosTp38::TestSortOrderWithORL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp38::TestSortOrderWithORL()
    {
    
    
    
    iLog->Log(_L("TestSeveralTextArgumentsWithORL"));
    
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionOR);

	TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
		
    // First create a textcriteria
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();    
    textCriteria->SetTextL(_L("Nokia*"));
    textCriteria->SetAttributesToSearch(CPosLandmark::ELandmarkName);
    TInt err = composite->AddArgument(textCriteria);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument1"));
    	User::Leave(err);
    }
    CleanupStack::Pop(textCriteria);

    // First create a textcriteria
    CPosLmTextCriteria* textCriteria2 = CPosLmTextCriteria::NewLC();    
    textCriteria2->SetTextL(_L("*Eriksberg"));
    textCriteria2->SetAttributesToSearch(CPosLandmark::ELandmarkName);
    err = composite->AddArgument(textCriteria2);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument2"));
    	User::Leave(err);
    }
    CleanupStack::Pop(textCriteria2);

    // Should find landmark ids 48,55,62 & 63 with name:"Nokia","TE Eriksberg","Nokia Copenhagen" "Nokia, Stockholm"
    iLog->Log(_L("iLandmarkSearch->StartLandmarkSearchL"));    
    iOperation = iLandmarkSearch->StartLandmarkSearchL(*composite,sortPref, EFalse);
    ExecuteAndDeleteLD(iOperation);
    
    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
    
    RArray<TPosLmItemId> idArray;
    iter->GetItemIdsL(idArray, 0, iter->NumOfItemsL());
            
    
    TInt matches = iLandmarkSearch->NumOfMatches();
    if (matches != 4) 
    {
    	iLog->Log(_L("Wrong number of matches"));
    	User::Leave(-1);
    }

//Check if the landmarks are in ascending order i.e "Nokia","Nokia Copenhagen","Nokia Stockholm","TE Eriksberg"


	if(idArray[0] != 48 || idArray[1] != 62 || idArray[2] != 63 || idArray[3] != 55)
		{
			iLog->Log(_L("Wrong landmark found, should find landmark with id 48, 55, 62 & 63"));
    		User::Leave(-1);
		}
	
       
    iter->Reset();
    CleanupStack::PopAndDestroy(iter);
    CleanupStack::PopAndDestroy(composite);
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    }

// ---------------------------------------------------------
// CPosTp38::TestCategoryCompositeSearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp38::TestCategoryCompositeSearchL()
    {
    iLog->Log(_L("TestCategoryCompositeSearchL"));

    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

	// Use differen text pattern
    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
    nameCriteria->SetSearchPatternL(_L("*"));
    TInt err = composite->AddArgument(nameCriteria);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error from AddArgument(nameCriteria);"));
    	User::Leave(err);
    }
    CleanupStack::Pop(nameCriteria);

    CPosLmCatNameCriteria* nameCriteria2 = CPosLmCatNameCriteria::NewLC();
    nameCriteria2->SetSearchPatternL(_L("*kontor"));
    err = composite->AddArgument(nameCriteria2);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error from AddArgument(nameCriteria);"));
    	User::Leave(err);
    }
    CleanupStack::Pop(nameCriteria2);

    // Should find category id: 13 name "Nokia Kontor" and id: 14 name: "TietoEnator kontor"
    iLog->Log(_L("iLandmarkSearch->StartCategorySearchL"));
    iOperation = iLandmarkSearch->StartCategorySearchL(*composite, CPosLmCategoryManager::ECategorySortOrderNone);

    ExecuteAndDeleteLD(iOperation);

    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
    
    TInt matches = iLandmarkSearch->NumOfMatches();
    if (matches != 2)
    {
    	iLog->Log(_L("Wrong number of matches"));
    	User::Leave(-1);
    }
    else iLog->Log(_L("Found 2 nr of matches"));

    TPosLmItemId id1 = iter->NextL();
    TPosLmItemId id2 = iter->NextL();

    if ((id1 == 13 && id2 != 14) || (id1 == 14 && id2 != 13) 
        || (id2 == 13 && id1 != 14) || (id2 == 14 && id1 != 13))
        {
        	iLog->Log(_L("Wrong landmark found"));
    		User::Leave(-1);
        }
    
    iter->Reset();
    CleanupStack::PopAndDestroy(iter);
    
	// Use same text pattern
	nameCriteria = CPosLmCatNameCriteria::NewLC();
    nameCriteria->SetSearchPatternL(_L("TietoEnator*"));
    err = composite->AddArgument(nameCriteria);
    if (err != KErrNone)
    {
    	
    	iLog->Log(_L("Error from AddArgument(nameCriteria);"));
    	User::Leave(err);
    }
    CleanupStack::Pop(nameCriteria);

    nameCriteria2 = CPosLmCatNameCriteria::NewLC();
    nameCriteria2->SetSearchPatternL(_L("TietoEnator*"));
    err = composite->AddArgument(nameCriteria2);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error from AddArgument(nameCriteria);"));
    	User::Leave(err);

    }
    CleanupStack::Pop(nameCriteria2);

    // Should find category id: 14 name "TietoEnator Kontor"
    iLog->Log(_L("iLandmarkSearch->StartCategorySearchL"));
    iOperation = iLandmarkSearch->StartCategorySearchL(*composite, CPosLmCategoryManager::ECategorySortOrderNone);

    ExecuteAndDeleteLD(iOperation);
    iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
    
    matches = iLandmarkSearch->NumOfMatches();
    if (matches != 1) 
    {
    	iLog->Log(_L("Wrong number of matches"));
    	User::Leave(-1);

    }
    else iLog->Log(_L("Found 1 nr of matches"));

    id1 = iter->NextL();

    if (id1 != 14)
    {
	    iLog->Log(_L("Wrong landmark found"));
    	User::Leave(-1);
	}
    
    iter->Reset();
    CleanupStack::PopAndDestroy(iter);
    CleanupStack::PopAndDestroy(composite);

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
        
    iLog->Log(_L("Done"));
    }

// ---------------------------------------------------------
// CPosTp38::TestAddAndRemoveL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp38::TestAddAndRemoveL()
    {
    iLog->Log(_L("TestAddAndRemoveL"));
    
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

    // 1) Add a text criteria
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    textCriteria->SetTextL(_L("*"));
    TInt err = composite->AddArgument(textCriteria);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument1"));
    	User::Leave(err);

    }
    CleanupStack::Pop(textCriteria);

    // 2) Add a text criteria 
    textCriteria = CPosLmTextCriteria::NewLC();
    textCriteria->SetTextL(_L("Natur*"));
    err = composite->AddArgument(textCriteria);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument2"));
    	User::Leave(err);

    }
    CleanupStack::Pop(textCriteria);

    // 3) Add an area criteria
    CPosLmAreaCriteria* areaCriteria = CPosLmAreaCriteria::NewLC(-12.23, 34.4, -2.05, 45.5);
    err = composite->AddArgument(areaCriteria);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument3"));
    	User::Leave(err);

    }
    CleanupStack::Pop(areaCriteria);

    // 4) Add a category criteria
    CPosLmCategoryCriteria* catCriteria = CPosLmCategoryCriteria::NewLC();
    catCriteria->SetCategoryNameL(_L("Museum"));
    err = composite->AddArgument(catCriteria);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument3"));
    	User::Leave(err);

    }
    CleanupStack::Pop(catCriteria);

    // 5) Add a nearest criteria
    TCoordinate cord(12.23, 12);
    CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(cord, ETrue);
    err = composite->AddArgument(nearestCriteria);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument3"));
    	User::Leave(err);

    }
    CleanupStack::Pop(nearestCriteria);

    if (composite->NumOfArguments() != 5) 
    {
    	iLog->Log(_L("Error when adding argument1"));
    	User::Leave(-1);

    }

    // Remove (3) area criteria
    CPosLmSearchCriteria* obj1 = composite->RemoveArgument(2);
    delete obj1;
    obj1 = NULL;

    // Remove (5) nearest criteria (after 3 removed it has id 4)
    CPosLmSearchCriteria* obj2 = composite->RemoveArgument(3);
    delete obj2;
    obj2 = NULL;

    // Should find landmark with id: 87 name: "Naturhistoriska"
    iOperation = iLandmarkSearch->StartLandmarkSearchL(*composite);
    ExecuteAndDeleteLD(iOperation);

    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);

    TInt nr = iter->NumOfItemsL();
    TBuf<100> buf;
    buf.Format(_L("Found %d nr of landmarks after removing two arguments "), nr);
    iLog->Log(buf);

    TPosLmItemId id = iter->NextL();
    if (id != 87) 
    {
    	iLog->Log(_L("Wrong landmark found"));
    	User::Leave(-1);

    }
    iter->Reset();

    // Remove (1) first text criteria
    CPosLmSearchCriteria* obj4 = composite->RemoveArgument(0);
    delete obj4;
    obj4 = NULL;

    // Remove (2) the second text criteria
    CPosLmSearchCriteria* obj3 = composite->RemoveArgument(0);
    delete obj3;
    obj3 = NULL;

    CleanupStack::PopAndDestroy(iter);

    // Should find six landmarks belonging to category "museum",
    iOperation = iLandmarkSearch->StartLandmarkSearchL(*composite);
    ExecuteAndDeleteLD(iOperation);

    iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);

    nr = iter->NumOfItemsL();
    if (nr != 6) 
    {
    	iLog->Log(_L("Wrong number of landmarks returned"));
    	User::Leave(-1);
    }
    buf.Format(_L("Found %d nr of landmarks after removing two more arguments"), nr);
    iLog->Log(buf);

    iter->Reset();

    CleanupStack::PopAndDestroy(iter);
    CleanupStack::PopAndDestroy(composite);

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    
    iLog->Log(_L("Done"));
    }

// ---------------------------------------------------------
// CPosTp38::SearchAndCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp38::SearchAndCancelL(const TBool& aInCallback)
    {
    iLog->Log(_L("SearchAndCancelL"));
    
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);

    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);    
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    textCriteria->SetTextL(_L("*"));
    
    TInt err = composite->AddArgument(textCriteria);
    if (err != KErrNone)
    {
		iLog->Log(_L("Error when adding argument3"));
    	User::Leave(err);
    }
    CleanupStack::Pop(textCriteria);
    
    if (aInCallback)
        {
        RunAsyncOperationAndCancelInCallbackLD(iLandmarkSearch->StartLandmarkSearchL(
                *composite, EFalse));
        }
    else
        {
        RunAsyncOperationAndCancelLD(iLandmarkSearch->StartLandmarkSearchL(
                *composite, EFalse));
        }

    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
 
    if (aInCallback)
        {
        _LIT(KExpected, "Iter. of matches: %d");
        _LIT(KReturned, "LandmarksSearch no. of matches: %d");
        TBuf<100> info;
        info.Format(KExpected, iter->NumOfItemsL());
        iLog->Log(info);
        info.Format(KReturned, iLandmarkSearch->NumOfMatches());
        iLog->Log(info);

        TBuf<100> info2;
        _LIT(KNof, "No. of matches found before cancel = %d");
        info2.Format(KNof, iter->NumOfItemsL());
        iLog->Log(info2);

        
        }
    CleanupStack::PopAndDestroy(iter);
    CleanupStack::PopAndDestroy(composite);

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    }

// ---------------------------------------------------------
// CPosTp38::TestWithNoPreviousResultL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp38::TestWithNoPreviousResultL(TInt aTestNr)
    {
    iLog->Log(_L("TestWithNoPreviousResultL"));
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);

    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
    
    if (aTestNr == 0)
        {
        CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();    
        textCriteria->SetTextL(_L("*"));
        
        TInt err = composite->AddArgument(textCriteria);
        if (err != KErrNone)
        {
        iLog->Log(_L("Error from AddArgument(textCriteria);"));
    	User::Leave(err);	
        }
        CleanupStack::Pop(textCriteria);
        
        iOperation = iLandmarkSearch->StartLandmarkSearchL(*composite, ETrue);
        ExecuteAndDeleteLD(iOperation);
        iLog->Log(_L("ERROR: Should leave here"));

		iLog->Log(_L("Should never come here in the code"));
    	User::Leave(-1);	
		
    } 
    else if (aTestNr == 1)
        {
    
        CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
        nameCriteria->SetSearchPatternL(_L("*kontor"));
        TInt err = composite->AddArgument(nameCriteria);
        if (err != KErrNone)
        {
        iLog->Log(_L("Error from AddArgument(nameCriteria);"));
    	User::Leave(err);		
        }
        CleanupStack::Pop(nameCriteria);
        
        iOperation = iLandmarkSearch->StartCategorySearchL(*composite, CPosLmCategoryManager::ECategorySortOrderNone, ETrue);
        ExecuteAndDeleteLD(iOperation);
        iLog->Log(_L("ERROR: Should leave here"));

		iLog->Log(_L("Should never come here in the code"));
    	User::Leave(-1);		
        
        }
    
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    
    }

// ---------------------------------------------------------
// CPosTp38::PrintCompositeInfo
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp38::PrintCompositeInfo(CPosLmCompositeCriteria* aComposite)
    {
    TBuf<100> buf;
    buf.Format(_L("PrintCompositeInfo() NrOfArg %d"), aComposite->NumOfArguments());
    iLog->Log(buf);
    
    for (TUint i=0;i<aComposite->NumOfArguments();i++)
    {
        
        CPosLmSearchCriteria& comp = aComposite->Argument(i);
        TInt type = comp.CriteriaType();
        
        if (type == CPosLmSearchCriteria::ECriteriaArea) 
            {
            iLog->Log(_L("Arg Area found"));
            
            CPosLmAreaCriteria& tt = static_cast<CPosLmAreaCriteria&>(comp);

            TReal64 southLat;
            TReal64 northLat;
            TReal64 westLong;
            TReal64 eastLong;

            tt.GetSearchArea(southLat, northLat, westLong, eastLong);

            buf.Format(_L("SouthL %f NorthL: %f WestL :%f EastL: %f"), (TReal)southLat, (TReal)northLat, (TReal)westLong, (TReal)eastLong);
            iLog->Log(buf);

            }
        else if (type == CPosLmSearchCriteria::ECriteriaText) 
            {
            iLog->Log(_L("Arg Text found:"));
            CPosLmTextCriteria& tt = static_cast<CPosLmTextCriteria&>(comp);
            iLog->Log(tt.Text());
            }
        else if (type == CPosLmSearchCriteria::ECriteriaComposite)
            {
            iLog->Log(_L("Arg Composite found"));
            }
        else if (type == CPosLmSearchCriteria::ECriteriaCategory)
            {
            iLog->Log(_L("Arg Category found"));

            CPosLmCategoryCriteria& tt = static_cast<CPosLmCategoryCriteria&>(comp);
            iLog->Log(tt.CategoryName());
            }
        else if (type == CPosLmSearchCriteria::ECriteriaFindNearest)
            {
            CPosLmNearestCriteria& tt = static_cast<CPosLmNearestCriteria&>(comp);
            TCoordinate cord;
            tt.GetCoordinate(cord);
            iLog->Log(_L("Arg Nearest found"));
            buf.Format(_L("Latitude %f Longitude: %f MaxDist:%f"), (TReal)cord.Latitude(), (TReal)cord.Longitude(), (TReal)tt.MaxDistance());
            iLog->Log(buf);
            }
        else if (type == CPosLmSearchCriteria::ECriteriaIdList)
            {
            iLog->Log(_L("Arg Idlist found"));
            CPosLmIdListCriteria& tt = static_cast<CPosLmIdListCriteria&>(comp);

            RArray<TPosLmItemId> result;
            CleanupClosePushL(result);
            tt.GetLandmarkIdsL(result);

            buf.Format(_L("Nr of Ids: %d"), result.Count());
            iLog->Log(buf);
            
            for (TInt i=0;i<result.Count();i++)
                {
                buf.Format(_L("ID%d: %d"), i, result[i]);
                
                }
            CleanupStack::PopAndDestroy(&result);

            }
        else if (type == CPosLmSearchCriteria::ECriteriaCategoryByName) 
            {
            // Should never come here, ECriteriaCategoryByName not supported in composite objects
            iLog->Log(_L("Arg Categorybyname found"));
            }
        }
    }

//  End of File
