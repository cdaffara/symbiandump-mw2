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
#include "FT_CPosTp37.h"
#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLmCompositeCriteria.h>
#include <EPos_CPosLmCategoryCriteria.h>
#include <EPos_CPosLmCatNameCriteria.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmIdListCriteria.h>
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp37::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp37::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    delete iDatabase;
    iDatabase=NULL;

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    }

// ---------------------------------------------------------
// CPosTp37::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp37::StartL()
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

    TInt err = KErrNone;

    TRAP(err, DoErrorTestL(0));
    if (err != KErrArgument) {iLog->Log(_L("Wrong error1"));iErrorsFound++;}
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    err = KErrNone;
    TRAP(err, DoErrorTestL(1));
    if (err != KErrNotSupported) {iLog->Log(_L("Wrong error2")); iErrorsFound++;};
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    err = KErrNone;
    TRAP(err, DoErrorTestL(2));
    if (err != KErrNotSupported) {iLog->Log(_L("Wrong error3"));iErrorsFound++;}
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    err = KErrNone;

    TRAP(err, DoErrorTest3L(0));
    if (err != KErrArgument) {iLog->Log(_L("Wrong error5"));iErrorsFound++;}
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    err = KErrNone;
    TRAP(err, DoErrorTest3L(1));
    if (err != KErrNotSupported) {iLog->Log(_L("Wrong error6"));iErrorsFound++;}
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    err = KErrNone;
    TRAP(err, DoErrorTest3L(2));
    if (err != KErrArgument) {iLog->Log(_L("Wrong error7"));iErrorsFound++;}
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    err = KErrNone;
    TRAP(err, DoErrorTest3L(3));
    if (err != KErrNotSupported) {iLog->Log(_L("Wrong error8"));iErrorsFound++;}
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    err = KErrNone;
    
    TRAP(err, DoErrorTest3L(4));
    if (err != KErrArgument) {iLog->Log(_L("Wrong error9"));iErrorsFound++;}
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    err = KErrNone;

    TRAP(err, DoErrorTest4L(0));
    if (err != KErrArgument) {iLog->Log(_L("Wrong error10"));iErrorsFound++;}
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    err = KErrNone;

    TRAP(err, DoErrorTest4L(1));
    if (err != KErrNotSupported) {iLog->Log(_L("Wrong error11"));iErrorsFound++;}
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    err = KErrNone;

    stopTime.UniversalTime();
    TTimeIntervalMicroSeconds executionTime = 
        stopTime.MicroSecondsFrom(startTime);

    TInt resse = executionTime.Int64()/1000000;
    TBuf<100> buf;
    buf.Format(_L("TP37 took %d seconds"), resse);
    iLog->Log(buf);

    if (iErrorsFound != KErrNone) 
        {
        iLog->Log(_L("Errors found in TP37"));
        User::Leave(-1);
        }

	}

// ---------------------------------------------------------
// CPosTp37::DoErrorTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp37::DoErrorTestL(TInt aTestNr)
    {
    TBuf<100> buf;
    buf.Format(_L("DoErrorTestL %d"), aTestNr);
    iLog->Log(buf);
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

    CPosLmCompositeCriteria::TCompositionType type = composite->CompositionType();
    if (type != CPosLmCompositeCriteria::ECompositionAND) 
    {
       iLog->Log( _L("Error1: Wrong type returned"));
       User::Leave(-1);
    }

    TInt nr = composite->NumOfArguments();
    if (nr != 0)
    {
        iLog->Log(_L("Wrong number of arguments returned"));
       User::Leave(-1);

    }

    if (aTestNr == 0)
        { 
        // Test that it is not possible to perform a composite search without any search arguments attached to 
        // the composite object
        iOperation = iLandmarkSearch->StartLandmarkSearchL(*composite, EFalse);
        iLog->Log(_L("Should leave here"));
        ExecuteAndDeleteLD(iOperation);
        }
    else if (aTestNr == 1)
        {
        // Test that it is not possible to perform a composite search without any search arguments attached to 
        // the composite object
        iOperation = iLandmarkSearch->StartCategorySearchL(*composite, CPosLmCategoryManager::ECategorySortOrderNone);
        iLog->Log(_L("Should leave here"));
        ExecuteAndDeleteLD(iOperation);
        }
    else if (aTestNr == 2)
        {
        // Test that it is not possible to attach a category search attribute to
        // a composite object
        CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
        nameCriteria->SetSearchPatternL(_L("*"));
        TInt err = composite->AddArgument(nameCriteria);
        if (err != KErrNotSupported) 
        {
            iLog->Log(_L("Wrong error when adding CPosLmCatNameCriteria to composite"));
            User::Leave(err);
        }
        User::Leave(KErrNotSupported);
        }

    
            iLog->Log(_L("Did not leave!!!"));
            User::Leave(-1);
   
    iLog->Log(_L("ClearArguments"));
    composite->ClearArguments();
    iLog->Log(_L("PopAndDestroy"));
    CleanupStack::PopAndDestroy(composite);
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    iLog->Log(_L("Done"));
	}

// ---------------------------------------------------------
// CPosTp37::DoErrorTest3L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp37::DoErrorTest3L(TInt aTestNr)
    {
    TBuf<100> buf;
    buf.Format(_L("DoErrorTest3L %d"), aTestNr);
    iLog->Log(buf);
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    TInt err = KErrNone;
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
    
    if (aTestNr == 0)
        {
        // add empty ID List
        CPosLmIdListCriteria* idList = CPosLmIdListCriteria::NewLC();
        err = composite->AddArgument(idList);
        CleanupStack::Pop(idList);
        if (err != KErrNone) 
        {
            iLog->Log(_L("Error from AddArgument(idList);"));
            User::Leave(err);
        }
        iLog->Log(_L("iLandmarkSearch->StartLandmarkSearchL"));    
        iOperation = iLandmarkSearch->StartLandmarkSearchL(*composite, EFalse);
        iLog->Log(_L("Should leave here"));
        ExecuteAndDeleteLD(iOperation);
        }
    else if (aTestNr == 1)
        {
        // add empty ID List
        CPosLmIdListCriteria* idList = CPosLmIdListCriteria::NewLC();
        TInt err = composite->AddArgument(idList);
        CleanupStack::Pop(idList);
        if (err != KErrNone) 
        {
            iLog->Log(_L("Error from AddArgument(idList);"));
            User::Leave(err);

        }

        iLog->Log(_L("iLandmarkSearch->StartCategorySearchL"));    
        iOperation = iLandmarkSearch->StartCategorySearchL(*composite, CPosLmCategoryManager::ECategorySortOrderNone);
        iLog->Log(_L("Should leave here"));
        ExecuteAndDeleteLD(iOperation);

        }
    else if (aTestNr == 2)
        {
        // Test with list that contain items
        RArray<TPosLmItemId> list;
        CleanupClosePushL(list);

        list.Append(0);
        list.Append(1);
        list.Append(2);
        CPosLmIdListCriteria* idList = CPosLmIdListCriteria::NewLC();
        idList->SetLandmarkIdsL(list);
        err = composite->AddArgument(idList);
        CleanupStack::Pop(idList);
        if (err != KErrNone)
        {
            iLog->Log(_L("Error from AddArgument(idList);"));
            User::Leave(err);
        }
        iLog->Log(_L("iLandmarkSearch->StartLandmarkSearchL"));    
        iOperation = iLandmarkSearch->StartLandmarkSearchL(*composite, EFalse);
        iLog->Log(_L("Should leave here"));
        ExecuteAndDeleteLD(iOperation);

        CleanupStack::PopAndDestroy(&list);
        }
    else if (aTestNr == 3)
        {
        // Test with list that contain items
        RArray<TPosLmItemId> list;
        CleanupClosePushL(list);

        list.Append(0);
        list.Append(1);
        list.Append(2);
        CPosLmIdListCriteria* idList = CPosLmIdListCriteria::NewLC();
        idList->SetLandmarkIdsL(list);
        err = composite->AddArgument(idList);
        CleanupStack::Pop(idList);
        if (err != KErrNone)
        {
            iLog->Log(_L("Error from AddArgument(idList);"));
            User::Leave(err);
        }
        iLog->Log(_L("iLandmarkSearch->StartCategorySearchL"));    
        iOperation = iLandmarkSearch->StartCategorySearchL(*composite, CPosLmCategoryManager::ECategorySortOrderNone);
        iLog->Log(_L("Should leave here"));
        ExecuteAndDeleteLD(iOperation);

        CleanupStack::PopAndDestroy(&list);
        }
    else if (aTestNr == 4)
        {
        // Test with list that contain valid and invalid items
        RArray<TPosLmItemId> list;
        CleanupClosePushL(list);
    
    	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    	textCriteria->SetTextL(_L(""));
    	
    	err = composite->AddArgument(textCriteria);
        CleanupStack::Pop(textCriteria);
        if (err != KErrNone) 
        {
            iLog->Log(_L("Error from AddArgument(idList);"));
            User::Leave(err);
        }
    
        list.Append(1);
        list.Append(2);
        list.Append(2000);
        list.Append(2001);
        
        CPosLmIdListCriteria* idList = CPosLmIdListCriteria::NewLC();
        idList->SetLandmarkIdsL(list);
        err = composite->AddArgument(idList);
        CleanupStack::Pop(idList);
        CleanupStack::PopAndDestroy(&list);
        if (err != KErrNone) 
        {
            iLog->Log(_L("Error from AddArgument(idList);"));
            User::Leave(err);
        }
        iLog->Log(_L("iLandmarkSearch->StartLandmarkSearchL"));    
        iOperation = iLandmarkSearch->StartLandmarkSearchL(*composite, CPosLmCategoryManager::ECategorySortOrderNone);
        
        ExecuteAndDeleteLD(iOperation);
        // We should never come here
        iLog->Log(_L("Should leave here"));
        
        CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
        CleanupStack::PushL(iter);
        CleanupStack::PopAndDestroy(iter);
        }
    else 
        {
        
        iLog->Log(_L("Error in testcode, we should never come here"));
         User::Leave(-1);
        }

    CleanupStack::PopAndDestroy(composite);
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    iLog->Log(_L("Done"));
    }

// ---------------------------------------------------------
// CPosTp37::DoErrorTest4L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp37::DoErrorTest4L(TInt aTestNr)
    {
    TBuf<100> buf;
    buf.Format(_L("DoErrorTest4L %d"), aTestNr);
    iLog->Log(buf);
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
    
    if (aTestNr == 0)
        {
        // Test with several lists
        RArray<TPosLmItemId> list1;
        CleanupClosePushL(list1);

        list1.Append(0);
        list1.Append(1);
        list1.Append(2);

        RArray<TPosLmItemId> list2;
        CleanupClosePushL(list2);

        list2.Append(3);
        list2.Append(4);
        list2.Append(5);

        CPosLmIdListCriteria* idList1 = CPosLmIdListCriteria::NewLC();
        CPosLmIdListCriteria* idList2 = CPosLmIdListCriteria::NewLC();

        idList1->SetLandmarkIdsL(list1);
        idList2->SetLandmarkIdsL(list2);
        TInt err = composite->AddArgument(idList2);
        if (err != KErrNone)
        {
         iLog->Log(_L("Error from AddArgument(idList2);"));
         User::Leave(err);
        }
        CleanupStack::Pop(idList2);
        err = composite->AddArgument(idList1);
        if (err != KErrNone) 
        {
         iLog->Log(_L("Error from AddArgument(idList2);"));
         User::Leave(err);

        }
        CleanupStack::Pop(idList1);
        iLog->Log(_L("iLandmarkSearch->StartLandmarkSearchL"));    
        iOperation = iLandmarkSearch->StartLandmarkSearchL(*composite, EFalse);
        iLog->Log(_L("Should leave here"));
        ExecuteAndDeleteLD(iOperation);
        }
    else if (aTestNr == 1)
        {
        // Test with several lists
        RArray<TPosLmItemId> list1;
        CleanupClosePushL(list1);

        list1.Append(0);
        list1.Append(1);
        list1.Append(2);

        RArray<TPosLmItemId> list2;
        CleanupClosePushL(list2);

        list2.Append(3);
        list2.Append(4);
        list2.Append(5);

        CPosLmIdListCriteria* idList1 = CPosLmIdListCriteria::NewLC();
        CPosLmIdListCriteria* idList2 = CPosLmIdListCriteria::NewLC();

        idList1->SetLandmarkIdsL(list1);
        idList2->SetLandmarkIdsL(list2);

        TInt err = composite->AddArgument(idList2);
        if (err != KErrNone) 
        {
            iLog->Log(_L("Error from AddArgument(idList2);"));
            User::Leave(err);
        }
        CleanupStack::Pop(idList2);
        err = composite->AddArgument(idList1);
        if (err != KErrNone) 
        {
         iLog->Log(_L("Error from AddArgument(idList2);"));
         User::Leave(err);

        }
        CleanupStack::Pop(idList1);

        iLog->Log(_L("iLandmarkSearch->StartCategorySearchL"));    
        iOperation = iLandmarkSearch->StartCategorySearchL(*composite, CPosLmCategoryManager::ECategorySortOrderNone);
        iLog->Log(_L("Should leave here"));
        ExecuteAndDeleteLD(iOperation);
        }
    else 
        {
        
        iLog->Log(_L("Error in testcode, we should never come here"));
         User::Leave(-1);

        }

    
    iLog->Log(_L("Did not leave!!!"));
    User::Leave(-1);


    CleanupStack::PopAndDestroy(composite);
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    iLog->Log(_L("Done"));
    }

// ---------------------------------------------------------
// CPosTp37::TestCompositeAndCategoryL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp37::TestCompositeAndCategoryL(TInt /*aTestNr*/)
    {
    iLog->Log(_L("TestCompositeAndCategoryL"));

    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

    // Test with list that contain items
    RArray<TPosLmItemId> list;
    CleanupClosePushL(list);
    
    list.Append(0);
    list.Append(1);
    list.Append(2);
    
    CPosLmIdListCriteria* idList = CPosLmIdListCriteria::NewLC();
    idList->SetLandmarkIdsL(list);
    TInt err = composite->AddArgument(idList);
    if (err != KErrNone)
    {
    iLog->Log(_L("Error from AddArgument(idList);"));
    User::Leave(err);

    }
    CleanupStack::Pop(idList);

    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
    nameCriteria->SetSearchPatternL(_L("*"));
    err = composite->AddArgument(nameCriteria);
    if (err != KErrNotSupported)
    {
    iLog->Log(_L("Wrong error from AddArgument(nameCriteria); should be KErrNotSupported"));
    User::Leave(err);

    }
    CleanupStack::PopAndDestroy(nameCriteria);
    User::Leave(KErrNotSupported);


    CleanupStack::PopAndDestroy(composite);
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    iLog->Log(_L("Done"));
    }

//  End of File
