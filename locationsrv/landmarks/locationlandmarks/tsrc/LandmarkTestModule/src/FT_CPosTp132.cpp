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

#include "FT_CPosTp132.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkSearch.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLMItemIterator.h>
#include <EPos_CPosLmDisplayData.h>
#include <EPos_CPosLmDisplayItem.h>
#include <EPos_TPosLMSortPref.h>
#include <EPos_CPosLmCatNameCriteria.h>

#include <EPos_CPosLmMultiDbSearch.h>

#include <e32std.h>

// CONSTANTS
//EPosLmServer
_LIT(KPosLandmarksServerName, "*eposlmserver*");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp132::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp132::StartL()
    {
    MakeSurePanicDebugFileExistsL();
    
    iUseLogFromThreadIsDisabled = ETrue;

    RemoveDefaultDbL();
	RemoveAllLmDatabasesL();
	
	iErrorsFound = KErrNone;
	
	// Use multiple db
	_LIT(KDb20, "EPOSLM_020.LDB");
    _LIT(KDb40, "EPOSLM_040.LDB");
    _LIT(KDb60, "EPOSLM_060.LDB");
    _LIT(KDb80, "EPOSLM_080.LDB");
    _LIT(KDb105, "EPOSLM_105.LDB");
    
    // Copy to secure area
    
    CopyTestDbFileL(KDb20);
    CopyTestDbFileL(KDb40);
    CopyTestDbFileL(KDb60);
    CopyTestDbFileL(KDb80);
    CopyTestDbFileL(KDb105);
	
    iDatabase = UseGeneratedDbFileL();

    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

    // <<<<<<<<<<<<<<Panic testing below>>>>>>>>>>>>>>>>>>>
    iLog->Log(_L(">>>>>>>Start Panic test<<<<<<<<<"));
   
    //	1 Try to get item from DisplayData::DisplayItem outside accepted interval
    // Should panic with EPosInvalidIndex
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 1"));
    iTestCase = 1;
    iExpectedErrorCode = EPosInvalidIndex;
    ResumeThreadAndVerifyExit();
    
	//	2 Try to get item from DisplayData::DisplayItem outside accepted interval
    // Should panic with EPosInvalidIndex
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 2"));
    iTestCase = 2;
    iExpectedErrorCode = EPosInvalidIndex;
    ResumeThreadAndVerifyExit();

    // 3 Set displaydata during search, should panic with EPosSearchOperationInUse
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 3"));
    iTestCase = 3;
    iExpectedErrorCode = EPosSearchOperationInUse;
    ResumeThreadAndVerifyExit();
    
    // 4 Unset displaydata during search, should panic with EPosSearchOperationInUse
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 4"));
    iTestCase = 4;
    iExpectedErrorCode = EPosSearchOperationInUse;
    ResumeThreadAndVerifyExit();
    
    // 5 Unset displaydata during search, should panic with EPosSearchOperationInUse
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 5"));
    iTestCase = 5;
    iExpectedErrorCode = EPosSearchOperationInUse;
    ResumeThreadAndVerifyExit();

	// Nothing seems to happen, no leave or no panic
    // 6 NULL check, try to set DisplayData as NULL
    iExpectedExitType = ExitNormal;
    iLog->Log(_L("Panic test 6"));
    iTestCase = 6;
    iExpectedErrorCode = KErrNone;
    ResumeThreadAndVerifyExit();
    
    // 7 Delete displaydata during search -> "may lead to unexpected errors"
    // For some reason it panics with Kern-Exec 3
    /* Excerpt from EPos_CPosLandmarkSearch.h
     * The client owns the display data object. If the client deletes it
     * during a search, this may lead to unexpected errors. The client must
     * call @ref UnsetDisplayData before it deletes the display data object.
     */
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 7"));
    iTestCase = 7;
    //iExpectedErrorCode = KErrNone;
    iExpectedErrorCode = 3; // Kern-Exec 3
    ResumeThreadAndVerifyExit();
    
    // Test multi db search below
    // 8) Set displaydata during search, should panic with EPosSearchOperationInUse
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 8"));
    iTestCase = 8;
    iExpectedErrorCode = EPosSearchOperationInUse;
    ResumeThreadAndVerifyExit();
    
    // 9)  Unset displaydata during search, should panic with EPosSearchOperationInUse
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 9"));
    iTestCase = 9;
    iExpectedErrorCode = EPosSearchOperationInUse;
    ResumeThreadAndVerifyExit();
    
    // 10) Unset displaydata during search, should panic with EPosSearchOperationInUse
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 10"));
    iTestCase = 10;
    iExpectedErrorCode = EPosSearchOperationInUse;
    ResumeThreadAndVerifyExit();
    
    // 11 Delete displaydata during search -> "may lead to unexpected errors"
    /* The client owns the display data object. If the client deletes it
    * during a search, this may lead to unexpected errors. The client must
    * call @ref UnsetDisplayData before it deletes the display data object.
    */
    // For some reason it panics with Kern-Exec 3
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 11"));
    iTestCase = 11;
    iExpectedErrorCode = 3; //3 Kern-Exec 3
    ResumeThreadAndVerifyExit();
    
    // 12) Use the CPosLmDisplayItem::Category() function on a Landmark item
    // Should panic with EPosInvalidItemType
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 12"));
    iTestCase = 12;
    iExpectedErrorCode = EPosInvalidItemType;
    ResumeThreadAndVerifyExit();
    
    // 13) Use the CPosLmDisplayItem::Landmark() function on a Category item
    // Should panic with EPosInvalidItemType
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 13"));
    iTestCase = 13;
    iExpectedErrorCode = EPosInvalidItemType;
    ResumeThreadAndVerifyExit();
    
    // 14) Set DisplayData to NULL and do multipledb search
    // Nothing seems to happen, everything is OK
    iExpectedExitType = ExitNormal;
    iLog->Log(_L("Panic test 14"));
    iTestCase = 14;
    iExpectedErrorCode = KErrNone;
    ResumeThreadAndVerifyExit();
    
    if (iErrorsFound != KErrNone) 
    {
        iLog->Log(_L("TP132 Failed"));
        User::Leave(-1);
    }
    iLog->Log(_L(">>>>>>>Panic test Done<<<<<<<<<"));
    }

// ---------------------------------------------------------
// CPosTp132::ResumeThreadAndVerifyExit
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp132::ResumeThreadAndVerifyExit()
    {
    iLog->Log(_L("ResumeThreadAndVerifyExit"));
    _LIT(KPanicErr, "Err: Method didn't Panic when expected"); 
    _LIT(KPanicCodeErr, "Err: Paniced with unexpected panic code");
    _LIT(KLeaveErr, "Err: Method didn't end correctly"); 
    _LIT(KLeaveCodeErr, "Err: Unexpected leave error");

    CreateThreadL();
    TRequestStatus status;
    iThread.Logon(status);
    iThread.Resume();
        
    User::WaitForRequest(status);
    
    // Used for debugging
    TBuf<100> buf;
    buf.Format(_L("ExitType %d Exit reason %d "), iThread.ExitType(), iThread.ExitReason());
    iLog->Log(buf);
    
    TExitCategoryName buf1;
    buf1=iThread.ExitCategory();
    iLog->Log(buf1);

    if (iExpectedExitType == ExitPanic)
        {
        
        if (iThread.ExitType() != EExitPanic)
        {iErrorsFound++;iLog->Log(KPanicErr);}
        
        if(iTestCase != 11)
        {
        if (iThread.ExitReason() != iExpectedErrorCode)
        {iErrorsFound++;iLog->Log(KPanicCodeErr);}	
        }
        
        }
    else if (iExpectedExitType == ExitLeave)
        {
        
        if (iThread.ExitType() != EExitKill)
        {iErrorsFound++;iLog->Log(KLeaveErr);}
        
        if (iThread.ExitReason() != iExpectedErrorCode)
        {iErrorsFound++;iLog->Log(KLeaveCodeErr);}
        }
	else if (iExpectedExitType == ExitNormal)
        {
        
        if (iThread.ExitType() != KErrNone)
        {iErrorsFound++;iLog->Log(KLeaveErr);}
        
        if (iThread.ExitReason() != iExpectedErrorCode)
        {iErrorsFound++;iLog->Log(KLeaveCodeErr);}
        }        
        
    iThread.Close();
    
    TBool alive = ServerAlive(KPosLandmarksServerName);
    if (!alive)
        {
        iLog->Log(_L("Landmarks Server is not alive"));
        User::Leave(-1);

        }
    }

// ---------------------------------------------------------
// CPosTp132::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp132::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    iThread.Close();

    delete iDatabase;
    iDatabase = NULL;

    iLog->Log(_L("CloseTest Done"));
	}
	
// ---------------------------------------------------------
// CPosTp132::UnsetTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
TInt CPosTp132::UnsetTest(TAny* aSelf)
	{
	CPosLandmarkSearch* aLmSearch = static_cast<CPosLandmarkSearch*>(aSelf);
	aLmSearch->UnsetDisplayData();
	return 0;
	}
	
// ---------------------------------------------------------
// CPosTp132::UnsetMultiSearch
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
TInt CPosTp132::UnsetMultiSearch(TAny* aSelf)
	{
	CPosLmMultiDbSearch* aLmSearch = static_cast<CPosLmMultiDbSearch*>(aSelf);
	aLmSearch->UnsetDisplayData();
	return 0;
	}
		
// ---------------------------------------------------------
// CPosTp132::RunPanicTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp132::RunPanicTestL(TAny* aData)
    {
    CPosTp132* self = reinterpret_cast<CPosTp132*>(aData);

	CPosLandmarkDatabase* database = self->OpenDatabaseLC();
	CPosLandmarkSearch* lmSearch = CPosLandmarkSearch::NewL(*database);
	CleanupStack::PushL(lmSearch);
	
    CDesCArray* dbUris = new (ELeave) CDesC16ArrayFlat(2);
	CleanupStack::PushL(dbUris);

 	_LIT(KDb20, "c:EPOSLM_020.LDB");
	_LIT(KDb40, "c:EPOSLM_040.LDB");
	
	dbUris->AppendL(KDb20);
	dbUris->AppendL(KDb40);

     if (self->iTestCase == 1 || self->iTestCase == 2)
     	{
        TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);  
    	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    
    	// Set DisplayData
		CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
		CleanupStack::PushL(displayData);
    	lmSearch->SetDisplayData(*displayData);
    	
    	textCriteria->SetTextL(_L("*"));
    	
		CPosLmOperation* operation = lmSearch->StartLandmarkSearchL(*textCriteria, EFalse);
		ExecuteAndDeleteLD(operation);
		operation = NULL;

		// Outside accepted interval, should Panic with EPosInvalidIndex		
		if (self->iTestCase == 1)
			{
        	CPosLmDisplayItem& item = displayData->DisplayItem(displayData->Count());
        	}
        else if (self->iTestCase == 2)
        	{
        	CPosLmDisplayItem& item = displayData->DisplayItem(-1);
        	}
        	
        	lmSearch->UnsetDisplayData();
        	CleanupStack::PopAndDestroy(displayData);
        	
        }
     else if (self->iTestCase == 3)
     	{
     	// Set displaydata during search, should panic with EPosSearchOperationInUse
     	TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);  
    	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    
    	// Set DisplayData    	
    	textCriteria->SetTextL(_L("*"));
    	
		CPosLmOperation* operation = lmSearch->StartLandmarkSearchL(*textCriteria, EFalse);
		
		CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
		CleanupStack::PushL(displayData);
    	lmSearch->SetDisplayData(*displayData);
    	
    	ExecuteAndDeleteLD(operation);
		operation = NULL;
    	
    	lmSearch->UnsetDisplayData();
        CleanupStack::PopAndDestroy(displayData);
        CleanupStack::PopAndDestroy(textCriteria);
        }
     else if (self->iTestCase == 4)
     	{
     	// Unset displaydata during search, should panic with EPosSearchOperationInUse
    	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    
    	// Set DisplayData    	
    	textCriteria->SetTextL(_L("*"));
    	CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
		CleanupStack::PushL(displayData);
    	lmSearch->SetDisplayData(*displayData);
    	
		CPosLmOperation* operation = lmSearch->StartLandmarkSearchL(*textCriteria, EFalse);
		lmSearch->UnsetDisplayData();    	
    	ExecuteAndDeleteLD(operation);
		operation = NULL;
    	
    	lmSearch->UnsetDisplayData();
        CleanupStack::PopAndDestroy(displayData);
        CleanupStack::PopAndDestroy(textCriteria);

     }
     else if (self->iTestCase == 5)
     	{
     	// Unset displaydata during search, should panic with EPosSearchOperationInUse
    	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    
    	// Set DisplayData    	
    	textCriteria->SetTextL(_L("*"));
    	CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
		CleanupStack::PushL(displayData);
    	lmSearch->SetDisplayData(*displayData);
    	
		CPosLmOperation* operation = lmSearch->StartLandmarkSearchL(*textCriteria, EFalse);
    	// Unset displaydata in callback method
    	TCallBack callback(UnsetTest, lmSearch);
    	CPeriodic* periodicTimer = CPeriodic::NewL(CActive::EPriorityHigh);
    	CleanupStack::PushL(periodicTimer);
    	periodicTimer->Start(1, 1, callback);
    
    	self->RunAsyncOperationLD(operation);
		operation = NULL;
		
		CPosLmItemIterator* iter = lmSearch->MatchIteratorL();	
		TInt size = iter->NumOfItemsL();
    	
    	CleanupStack::PopAndDestroy(periodicTimer);
    	lmSearch->UnsetDisplayData();
        CleanupStack::PopAndDestroy(displayData);
        CleanupStack::PopAndDestroy(textCriteria);

     }     
     else if (self->iTestCase == 6)
     	{
     	// NULL check
    	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    
    	// Set DisplayData    	
    	textCriteria->SetTextL(_L("*"));
    	CPosLmDisplayData* displayData = NULL;
    	lmSearch->SetDisplayData(*displayData);
    	
		CPosLmOperation* operation = lmSearch->StartLandmarkSearchL(*textCriteria, EFalse);
    	ExecuteAndDeleteLD(operation);
		operation = NULL;

		CPosLmItemIterator* iter = lmSearch->MatchIteratorL();	
		TInt size = iter->NumOfItemsL();
		
		lmSearch->UnsetDisplayData();
		CleanupStack::PopAndDestroy(textCriteria);
     }
     else if (self->iTestCase == 7)
     	{
     	// Delete displaydata during search, "may lead to unexpected errors"
    	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    
    	// Set DisplayData    	
    	textCriteria->SetTextL(_L("*"));
    	CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
		CleanupStack::PushL(displayData);
    	lmSearch->SetDisplayData(*displayData);

		CPosLmOperation* operation = lmSearch->StartLandmarkSearchL(*textCriteria, EFalse);
		
		// Delete display data here
        CleanupStack::PopAndDestroy(displayData);
    	ExecuteAndDeleteLD(operation);
		operation = NULL;

        CleanupStack::PopAndDestroy(textCriteria);
     	}
     else if (self->iTestCase == 8)
     	{
     	// Set displaydata during search, should panic with EPosSearchOperationInUse
     	CPosLmMultiDbSearch* multiSearcher = CPosLmMultiDbSearch::NewL(*dbUris);
    	CleanupStack::PushL(multiSearcher);
    	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();	
    	textCriteria->SetTextL(_L("*"));
    	CPosLmOperation* operation = multiSearcher->StartLandmarkSearchL(*textCriteria, EFalse);
    	CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
		CleanupStack::PushL(displayData);
    	multiSearcher->SetDisplayData(*displayData);
    	ExecuteAndDeleteLD(operation);
		operation = NULL;
    	CleanupStack::PopAndDestroy(displayData);
    	CleanupStack::PopAndDestroy(multiSearcher);
    	
    	}
    else if (self->iTestCase == 9)
     	{
    	// Unset displaydata during search, should panic with EPosSearchOperationInUse
     	CPosLmMultiDbSearch* multiSearcher = CPosLmMultiDbSearch::NewL(*dbUris);
    	CleanupStack::PushL(multiSearcher);
    	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();	
    	textCriteria->SetTextL(_L("*"));
    	
    	CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
		CleanupStack::PushL(displayData);
    	multiSearcher->SetDisplayData(*displayData);
    	    	
    	CPosLmOperation* operation = multiSearcher->StartLandmarkSearchL(*textCriteria, EFalse);
		multiSearcher->UnsetDisplayData();    	
    	ExecuteAndDeleteLD(operation);
		operation = NULL;
		
		multiSearcher->UnsetDisplayData();
    	CleanupStack::PopAndDestroy(displayData);		
    	CleanupStack::PopAndDestroy(multiSearcher);
    	}
	else if (self->iTestCase == 10)
     	{    	
     	// Unset displaydata during search, should panic with EPosSearchOperationInUse
     	// Unset displaydata in callback method
     	CPosLmMultiDbSearch* multiSearcher = CPosLmMultiDbSearch::NewL(*dbUris);
    	CleanupStack::PushL(multiSearcher);
    	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();	
    	textCriteria->SetTextL(_L("*"));
    	
    	CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
		CleanupStack::PushL(displayData);
    	multiSearcher->SetDisplayData(*displayData);
    	    	
    	CPosLmOperation* operation = multiSearcher->StartLandmarkSearchL(*textCriteria, EFalse);
    	
    	TCallBack callback(UnsetMultiSearch, lmSearch);
    	CPeriodic* periodicTimer = CPeriodic::NewL(CActive::EPriorityHigh);
    	CleanupStack::PushL(periodicTimer);
    	periodicTimer->Start(1, 1, callback);
    	self->RunAsyncOperationLD(operation);
		operation = NULL;
		
		ExecuteAndDeleteLD(operation);
		multiSearcher->UnsetDisplayData();
    	CleanupStack::PopAndDestroy(displayData);		
    	CleanupStack::PopAndDestroy(multiSearcher);
     	}
	else if (self->iTestCase == 11)
     	{  
     	// Delete displaydata during search, "may lead to unexpected errors"
     	CPosLmMultiDbSearch* multiSearcher = CPosLmMultiDbSearch::NewL(*dbUris);
    	CleanupStack::PushL(multiSearcher);
    	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();	
    	textCriteria->SetTextL(_L("*"));
    	CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
    	multiSearcher->SetDisplayData(*displayData);
    	
    	CPosLmOperation* operation = multiSearcher->StartLandmarkSearchL(*textCriteria, EFalse);
    	// Delete displaydata here
    	delete displayData;
    	
    	ExecuteAndDeleteLD(operation);
		operation = NULL;

    	CleanupStack::PopAndDestroy(multiSearcher);
    	}
	else if (self->iTestCase == 12)
     	{  
		// Use the CPosLmDisplayItem::Category() function on a Landmark item
    	// Should panic with EPosInvalidItemType
     	CPosLmMultiDbSearch* multiSearcher = CPosLmMultiDbSearch::NewL(*dbUris);
    	CleanupStack::PushL(multiSearcher);
    	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();	
    	textCriteria->SetTextL(_L("*"));
    	CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
    	multiSearcher->SetDisplayData(*displayData);
    	
    	CPosLmOperation* operation = multiSearcher->StartLandmarkSearchL(*textCriteria, EFalse);    	
    	ExecuteAndDeleteLD(operation);
		
		// Verify that something was found
		if (displayData->Count() < 2) User::Leave(-900);
		
		CPosLmDisplayItem& item = displayData->DisplayItem(0);
	    TInt lmId = item.Landmark().LandmarkId();
	    
	    if (item.DisplayItemType() != CPosLmDisplayItem::ELandmarkItem) User::Leave(-901);
		
		// This call should panic
		item.Category();
		
		multiSearcher->UnsetDisplayData();
    	CleanupStack::PopAndDestroy(displayData);
    	CleanupStack::PopAndDestroy(textCriteria);
    	CleanupStack::PopAndDestroy(multiSearcher);
    	}
	else if (self->iTestCase == 13)
     	{  
		// Use the CPosLmDisplayItem::Landmark() function on a Category item
    	// Should panic with EPosInvalidItemType
     	CPosLmMultiDbSearch* multiSearcher = CPosLmMultiDbSearch::NewL(*dbUris);
    	CleanupStack::PushL(multiSearcher);
    	CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
    	nameCriteria->SetSearchPatternL(_L("*"));
    	
    	CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
    	multiSearcher->SetDisplayData(*displayData);
    	
    	CPosLmOperation* operation = multiSearcher->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone);
    	ExecuteAndDeleteLD(operation);
		
		// Verify that something was found
		if (displayData->Count() < 2) User::Leave(-900);
		
		CPosLmDisplayItem& item = displayData->DisplayItem(0);
	    TInt lmId = item.Landmark().LandmarkId();
	    
	    if (item.DisplayItemType() != CPosLmDisplayItem::ECategoryItem) User::Leave(-901);
		
		// This call should panic
		item.Landmark();
		
		multiSearcher->UnsetDisplayData();
    	CleanupStack::PopAndDestroy(displayData);
    	CleanupStack::PopAndDestroy(nameCriteria);
    	CleanupStack::PopAndDestroy(multiSearcher);
    	}
    else if (self->iTestCase == 14)
     	{
    	// NULL check, set DisplayData to NULL
     	CPosLmMultiDbSearch* multiSearcher = CPosLmMultiDbSearch::NewL(*dbUris);
    	CleanupStack::PushL(multiSearcher);
    	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();	
    	textCriteria->SetTextL(_L("*"));
    	
    	CPosLmDisplayData* displayData = NULL;
    	multiSearcher->SetDisplayData(*displayData);
    	    	
    	CPosLmOperation* operation = multiSearcher->StartLandmarkSearchL(*textCriteria, EFalse);
    	ExecuteAndDeleteLD(operation);
		
		multiSearcher->UnsetDisplayData();
		CleanupStack::PopAndDestroy(textCriteria);
    	CleanupStack::PopAndDestroy(multiSearcher);
    	}
    
    CleanupStack::PopAndDestroy(dbUris);
    CleanupStack::PopAndDestroy(lmSearch);
    CleanupStack::PopAndDestroy(database);
    }
	
// ---------------------------------------------------------
// LOCAL_C ThreadFunction
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
LOCAL_C TInt ThreadFunction(TAny* aData)
    {
    CTrapCleanup* cleanup=CTrapCleanup::New(); 

    CActiveScheduler* actSch = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(actSch);

    TRAPD(err, CPosTp132::RunPanicTestL(aData));
    
    delete actSch;
    delete cleanup;
    return err;
    }

// ---------------------------------------------------------
// CPosTp132::CreateThreadL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp132::CreateThreadL()
    {
    _LIT(KCreateThreadErr, "Create thread failed with %d");
	TBuf<100> buf;
	buf.Format(_L("Tp132 test thread %d"), iTestCase);
     TInt err=0;
     err = iThread.Create(buf, ThreadFunction, KDefaultStackSize, KMinHeapSize, KMaxHeapSize, this);
     AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    }

//  End of File
