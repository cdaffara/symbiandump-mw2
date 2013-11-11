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
#include "FT_CPosTp141.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkSearch.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLMItemIterator.h>
#include <EPos_CPosLmDisplayData.h>
#include <EPos_CPosLmDisplayItem.h>
#include <EPos_TPosLMSortPref.h>
#include "FT_CSearchResult.h"
     

// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp141::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp141::CloseTest()
    {
    
    delete iDatabase;
    iDatabase = NULL;

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    
    iSearchResults.ResetAndDestroy();
    }

// ---------------------------------------------------------
// CPosTp141::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp141::StartL()
    {
    _LIT(KEmptyTextCriteriaErr, "An empty string as text criteria should not be allowed");
    _LIT(KLandMarkattrErr, "Landmark attribute set -> AttributesToSearch  should return ELandmarkName |EDescription  ");   
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
    AssertTrueSecL(textCriteria->AttributesToSearch() == CPosLandmark::EDescription | CPosLandmark::ELandmarkName, KLandMarkattrErr);
    
    textCriteria->SetTextL(_L(""));
    
    TRAPD(err, iLandmarkSearch->StartLandmarkSearchL(*textCriteria));
    AssertTrueSecL(err == KErrArgument, KEmptyTextCriteriaErr);
    
       
    CleanupStack::PopAndDestroy(textCriteria);
    
    }


// ---------------------------------------------------------
// CPosTp141::TestInvalidPositionFieldsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp141::TestInvalidPositionFieldsL()
	{
	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
	RArray<TUint> arr;
	CleanupClosePushL(arr);
	textCriteria->GetPositionFieldsToSearchL(arr);	
	textCriteria->SetTextL(_L("*"));
	textCriteria->SetPositionFieldsToSearchL(arr);

	// Search with empty positionfields list, should find all landmarks
	iOperation = iLandmarkSearch->StartLandmarkSearchL(*textCriteria);
    ExecuteAndDeleteLD(iOperation);
    TInt nr = iLandmarkSearch->NumOfMatches();
    TBuf<100> buf;
    buf.Format(_L("Found %d nr of matches when searching in empty position fields list"), nr);
    iLog->Log(buf);
    
    	
		
	User::LeaveIfError(arr.Append(1));
	User::LeaveIfError(arr.Append(2));
	User::LeaveIfError(arr.Append(3));
	
	textCriteria->SetPositionFieldsToSearchL(arr);
    textCriteria->GetPositionFieldsToSearchL(arr);	
	TBuf<100> buf1;
	for(TInt i=0;i< arr.Count();i++)
	    {
	    buf1.Format(_L(" %d position fields--%d- "),i, arr[i]);
	    iLog->Log(buf1); 
	    }
	
	
    //Clears the position fields list used in a search
    textCriteria->ClearPositionFieldsToSearch();
	textCriteria->GetPositionFieldsToSearchL(arr);
	if(arr.Count()==0)
	{
	    iLog->Log(_L("ClearPositionFieldsToSearch()-Cleared the position fields list used in a search."));
	}
	
	
    
    iLog->Log(_L("Sets the position fields list used in a search"));
	User::LeaveIfError(arr.Append(0));
	User::LeaveIfError(arr.Append(50));
	User::LeaveIfError(arr.Append(500));
	
	textCriteria->SetPositionFieldsToSearchL(arr);
	textCriteria->GetPositionFieldsToSearchL(arr);
	TBuf<100> buf2;
    for(TInt i=0;i< arr.Count();i++)
	    {
	    buf2.Format(_L(" %d position fields--%d- "),i, arr[i]);
	    iLog->Log(buf2); 
	    }
	

	// Search with invalid positionfields list, should NOT find any landmarks
	iOperation = iLandmarkSearch->StartLandmarkSearchL(*textCriteria);
    ExecuteAndDeleteLD(iOperation);
    nr = iLandmarkSearch->NumOfMatches();

    buf.Format(_L("Found %d nr of matches when searching in invalid position fields"), nr);
    iLog->Log(buf);
    
    AssertTrueSecL(nr == 0, _L("We should not find any matches"));
    textCriteria->ClearPositionFieldsToSearch();
    CleanupStack::PopAndDestroy(&arr);
    CleanupStack::PopAndDestroy(textCriteria);
	}

//  End of File
