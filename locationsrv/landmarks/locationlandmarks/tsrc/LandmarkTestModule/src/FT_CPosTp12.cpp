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

#include "FT_CPosTp12.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLMCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLMItemIterator.h>
#include <EPos_TPosLMSortPref.h>
#include <EPos_CPosLMPartialReadParameters.h>  
#include <LbsPosition.h>
#include <e32std.h>
//#include <ss_std.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmNearestCriteria.h>
#include <EPos_CPosLmCompositeCriteria.h>
#include <EPos_CPosLmCatNameCriteria.h>

// CONSTANTS

const TInt KNoLandmarksPanicCodes = 11;
const TInt KTestCaseCategoryIterator = 0;
const TInt KTestCaseLandmarkIterator = 1;

const TInt KTestCasePartialAttributes1 = 2;
const TInt KTestCasePartialAttributes2 = 3;
      
const TInt KTestCaseSortPrefNoAttribute = 4;
const TInt KTestCaseSortPrefSeveralAttribute = 5;

const TInt KTestCaseNegativeMatches = 6;
const TInt KTestCaseNegativeDistance = 7;
const TInt KTestCaseNegativeRadius = 8;

const TInt KTestCaseCompositeSearch1 = 9;
const TInt KTestCaseCompositeSearch2 = 10;
const TInt KTestCaseCompositeSearch3 = 11;
const TInt KTestCaseCompositeSearch4 = 12;

const TInt KTestCaseDuplicateNotifyDatabaseEvent = 13;

const TInt KTestCaseIncorrectCompositeType1 = 14;
const TInt KTestCaseIncorrectCompositeType2 = 15;
const TInt KTestCaseIncorrectCompositeType3 = 16;
const TInt KTestCaseIncorrectCompositeType4 = 17;

const TInt KTestCaseInvalidOperationMode1 = 18;
const TInt KTestCaseInvalidOperationMode2 = 19;
const TInt KTestCaseInvalidOperationMode3 = 20;
const TInt KTestCaseInvalidOperationMode4 = 21;

const TInt KTestCaseInvalidAttributes1 = 22;
const TInt KTestCaseInvalidAttributes2 = 23;

const TInt KTestCaseCompositeSearch5 = 24;
const TInt KTestCaseCompositeSearch6 = 25;
const TInt KTestCaseCompositeSearch7 = 26;
const TInt KTestCaseCompositeSearch8 = 26;

const TInt KTestCaseCategoryIteratorSortPref = 27;
const TInt KTestCaseReferencedCategoryIteratorSortPref = 28;

const TInt KTestCaseCategorySearchSortPref = 29;
const TInt KTestCaseLandmarkSortPref = 30;
const TInt KTestCaseLandmarkSortPref2 = 31;
const TInt KTestCaseLandmarkSearchinvalidAttribute = 32;
const TInt KTestCaseLandmarkSearchinvalidAttribute2 = 33;

const TInt KTestCaseInvalidRequestedPositionFields1 = 34;
const TInt KTestCaseInvalidRequestedPositionFields2 = 35;

const TInt KTestCaseZeroMatches = 36;

const TInt KTestLandmarkSetIconTest1 = 37;
const TInt KTestLandmarkSetIconTest2 = 38;
const TInt KTestCategorySetIconTest1 = 41;
const TInt KTestCategorySetIconTest2 = 42;

_LIT(KResourceFilePath, "c:\\resource\\");
_LIT(KResourceFile, "c:\\resource\\EPOSLMASYNCOPS.RSC");
_LIT(KTempResourceFile, "c:\\resource\\TEMP_EPOSLMASYNCOPS.RSC");


// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp12::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp12::StartL()
    {    
    MakeSurePanicDebugFileExistsL();
    RemoveDefaultDbL();
	TBuf<100> buf;

        
    for (TInt i=0; i<KNoLandmarksPanicCodes; i++)
        {
        iExpectedPanicCode = static_cast<TPositionLandmarksClientPanic>(i);
        buf.Format(_L("Expected Panic code %d"), iExpectedPanicCode);
        iLog->Log(buf);
        
        if (iExpectedPanicCode == EPosSpecifiedIntervalLiesOutsideIteratedSet)
            {
            iTestCase=KTestCaseCategoryIterator;
            ResumeThreadAndVerifyExit();
            iTestCase=KTestCaseLandmarkIterator;
            }
        else if (iExpectedPanicCode == EPosInvalidPartialReadParameters)
            {
            iTestCase=KTestCasePartialAttributes1;
            ResumeThreadAndVerifyExit();
            iTestCase=KTestCasePartialAttributes2;
            }
        else if (iExpectedPanicCode == EPosInvalidRequestedPositionFields)
            {
            iTestCase=KTestCaseInvalidRequestedPositionFields1;
            ResumeThreadAndVerifyExit();
            iTestCase=KTestCaseInvalidRequestedPositionFields2;
            }            
        else if (iExpectedPanicCode == EPosNoneOrMultipleLandmarkAttributeSet)
            {
            iTestCase=KTestCaseSortPrefNoAttribute;
            ResumeThreadAndVerifyExit();
            iTestCase=KTestCaseSortPrefSeveralAttribute;
            }
        else if (iExpectedPanicCode == EPosNegativeValue)
        	{
            iTestCase=KTestCaseNegativeDistance;
        	ResumeThreadAndVerifyExit();
        	iTestCase=KTestCaseNegativeRadius;
        	}
        else if (iExpectedPanicCode == EPosInvalidEnumValue)
            {
            iTestCase = KTestCaseIncorrectCompositeType1;
            ResumeThreadAndVerifyExit();
            iTestCase = KTestCaseIncorrectCompositeType2;
            ResumeThreadAndVerifyExit();
            iTestCase = KTestCaseIncorrectCompositeType3;
            ResumeThreadAndVerifyExit();
            iTestCase = KTestCaseIncorrectCompositeType4;
            }
        else if (iExpectedPanicCode == EPosInvalidOperationMode)
            {
            // Excerpt from EPos_LandmarksErrors.h:
            // "Invalid operation mode. Caused by mixed calls to NextStep and ExecuteL 
	        // for an CPosLmOperation object or subsequent calls to NextStep"
            iTestCase = KTestCaseInvalidOperationMode1;
            ResumeThreadAndVerifyExit();
            iTestCase = KTestCaseInvalidOperationMode2;
            ResumeThreadAndVerifyExit();
            iTestCase = KTestCaseInvalidOperationMode3;
            ResumeThreadAndVerifyExit();
            iTestCase = KTestCaseInvalidOperationMode4;            
            }
		else if (iExpectedPanicCode == EPosInvalidLandmarkAttribute)
			{
			iTestCase = KTestCaseInvalidAttributes1;
            ResumeThreadAndVerifyExit();
            iTestCase = KTestCaseInvalidAttributes2;
			}
	    
	     else  //added for testing
		    {
		        continue;
		    }
						
         ResumeThreadAndVerifyExit();
        }

        iLog->Log(_L("Tests below causes a USER-130 panic to be raised"));
        // Tests below causes a USER-130 panic to be raised
        iExpectedPanicCode = static_cast<TPositionLandmarksClientPanic>(130);

        iTestCase = KTestCaseCompositeSearch1;
        ResumeThreadAndVerifyExit();

        iTestCase = KTestCaseCompositeSearch2;
        ResumeThreadAndVerifyExit();

        iTestCase = KTestCaseCompositeSearch3;
        ResumeThreadAndVerifyExit();

        iTestCase = KTestCaseCompositeSearch4;
        ResumeThreadAndVerifyExit();
        
        iTestCase = KTestCaseCompositeSearch5;
        ResumeThreadAndVerifyExit();
        
        iTestCase = KTestCaseCompositeSearch6;
        ResumeThreadAndVerifyExit();
        
        iTestCase = KTestCaseCompositeSearch7;
        ResumeThreadAndVerifyExit();

        iTestCase = KTestCaseCompositeSearch8;
        ResumeThreadAndVerifyExit();          

        // Test below should raise a EPosEventNotifierAlreadyHasOutstandingRequest (2) panic
        // defined in EPos_LandmarksServerPanics.h
        // Since other testparts check for the same panic number change it to something else
        // and then change it to the correct number (2) in the thread function
        iLog->Log(_L("Test below should raise a EPosEventNotifierAlreadyHasOutstandingRequest (2) panic"));
        iExpectedPanicCode = static_cast<TPositionLandmarksClientPanic>(222);
        iTestCase = KTestCaseDuplicateNotifyDatabaseEvent;
        ResumeThreadAndVerifyExit();
        
        // Test that when using invalid CPosLmCategoryManager::TCategorySortPref in CategoryIteratorL
        //  causes a panic
        iLog->Log(_L("Test that when using invalid CPosLmCategoryManager::TCategorySortPref causes a panic"));
        iExpectedPanicCode = EPosInvalidEnumValue;
        iTestCase = KTestCaseCategoryIteratorSortPref;
        ResumeThreadAndVerifyExit();
        
         // Test that when using invalid CPosLmCategoryManager::TCategorySortPref in ReferencedCategoryIteratorL
         //  causes a panic
        iLog->Log(_L("Test that when using invalid CPosLmCategoryManager::TCategorySortPref causes a panic"));
        iExpectedPanicCode = EPosInvalidEnumValue;
        iTestCase = KTestCaseReferencedCategoryIteratorSortPref;
        ResumeThreadAndVerifyExit();

        // Test that setting SetMaxNumOfMatches to a negative value causes a panic
        iLog->Log(_L("Test that setting SetMaxNumOfMatches to a negative value causes a panic"));
        iExpectedPanicCode = EPosLmInvalidArgument;
    	iTestCase = KTestCaseNegativeMatches;
        ResumeThreadAndVerifyExit();
        
        // Test that setting SetMaxNumOfMatches to 0 causes a panic
        iLog->Log(_L("Test that setting SetMaxNumOfMatches to 0 causes a panic"));
        iExpectedPanicCode = EPosLmInvalidArgument;
    	iTestCase = KTestCaseZeroMatches;
        ResumeThreadAndVerifyExit();
        
        // Test starting a landmark category search with invalid SortPref panics
        iLog->Log(_L("Test starting a landmark category search with invalid SortPref panics"));
        iExpectedPanicCode = EPosInvalidEnumValue;
    	iTestCase = KTestCaseCategorySearchSortPref;
        ResumeThreadAndVerifyExit();
        
        // Test that it is not possible to create an invalid SortPref
        iLog->Log(_L("Test that it is not possible to create an invalid SortPref"));
        iExpectedPanicCode = EPosInvalidEnumValue;
    	iTestCase = KTestCaseLandmarkSortPref;
        ResumeThreadAndVerifyExit();
        
        // Test that it is not possible to set a SortPref to invalid values
        iLog->Log(_L("Test that it is not possible to set a SortPref to invalid values"));
        iExpectedPanicCode = EPosInvalidEnumValue;
    	iTestCase = KTestCaseLandmarkSortPref2;
        ResumeThreadAndVerifyExit();
        
        // Test that searching with an invalid attribute panics
        iLog->Log(_L("Test that it is not possible to set a SortPref with incorrect attributes"));
        iExpectedPanicCode = EPosNoneOrMultipleLandmarkAttributeSet;
    	iTestCase = KTestCaseLandmarkSearchinvalidAttribute;
        ResumeThreadAndVerifyExit();
                
        // Test that it is not possible to set a SortPref with incorrect
        // attributes
        iLog->Log(_L("Test that searching with an invalid attribute panics"));
        iExpectedPanicCode = EPosNoneOrMultipleLandmarkAttributeSet;
    	iTestCase = KTestCaseLandmarkSearchinvalidAttribute2;
        ResumeThreadAndVerifyExit();
        
        // Test that Landmark->SetIcon with invalid index panics
        iLog->Log(_L("Test that Landmark->SetIcon with invalid index panics"));
        iExpectedPanicCode = EPosLmInvalidArgument;
    	iTestCase = KTestLandmarkSetIconTest1;
        ResumeThreadAndVerifyExit();
        
        // Test that Landmark->SetIcon with invalid maskindex panics
        iLog->Log(_L("Test that Landmark->SetIcon with invalid maskindex panics"));
        iExpectedPanicCode = EPosLmInvalidArgument;
    	iTestCase = KTestLandmarkSetIconTest2;
        ResumeThreadAndVerifyExit();
        
        // Test that Category->SetIcon with invalid index panics
        iLog->Log(_L("Test that Category->SetIcon with invalid index panics"));
        iExpectedPanicCode = EPosLmInvalidArgument;
    	iTestCase = KTestCategorySetIconTest1;
        ResumeThreadAndVerifyExit();
        
        // Test that Category->SetIcon with invalid maskindex panics
        iLog->Log(_L("Test that Category->SetIcon with invalid maskindex panics"));
        iExpectedPanicCode = EPosLmInvalidArgument;
    	iTestCase = KTestCategorySetIconTest2;
        ResumeThreadAndVerifyExit();
                
    }

// ---------------------------------------------------------
// CPosTp12::ResumeThreadAndVerifyExit
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp12::ResumeThreadAndVerifyExit()
    {
    iLog->Log(_L("ResumeThreadAndVerifyExit"));
    _LIT(KPanicErr, "Method didn't Panic when expected"); 
    _LIT(KPanicCodeErr, "Paniced with unexpected panic code");

    CreateThreadL();
    TRequestStatus status;
    iThread.Logon(status);
    iThread.Resume();
        
    User::WaitForRequest(status);
    
    // Used for debugging
    TBuf<100> buf;
    buf.Format(_L("ExitType %d Exit reason %d"), iThread.ExitType(), iThread.ExitReason());
    iLog->Log(buf);

    AssertTrueSecL(iThread.ExitType() == EExitPanic, KPanicErr);
    AssertTrueSecL(iThread.ExitReason() == iExpectedPanicCode, KPanicCodeErr);
       
    iThread.Close();
    }

// ---------------------------------------------------------
// CPosTp12::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp12::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    iThread.Close();
    TRAPD(err, RestoreResourceFileL());
    if (err != KErrNone) iLog->Log(_L("Error in CloseTest"));
    iLog->Log(_L("CloseTest Done"));
	}

// ---------------------------------------------------------
// CPosTp12::RunPanicTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CPosTp12::RunPanicTestL(TAny* aData)
    {
    CPosTp12* self = reinterpret_cast<CPosTp12*>(aData);

    
    if(self->iExpectedPanicCode == EPosInvalidValueSpecifiedInResourceFile)
        {
        self->ResourceFilePanicTestL();
        }
    
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);

    CPosLandmarkDatabase* lmd = self->OpenDatabaseLC();
    CPosLandmark* lm = CPosLandmark::NewLC();
    
    CPosLmCategoryManager* cm = CPosLmCategoryManager::NewL(*lmd); 
    CleanupStack::PushL(cm);

    if (self->iExpectedPanicCode == EPosInvalidPositionFieldId)
        {
        lm->NextPositionFieldId(TUint16(222));
        }
    else if (self->iExpectedPanicCode == EPosSpecifiedIntervalLiesOutsideIteratedSet) // CategoryIterator
        {
        if (self->iTestCase == KTestCaseCategoryIterator)
            {
            CPosLmItemIterator* iter = cm->CategoryIteratorL();
            CleanupStack::PushL(iter);
            const TInt KCategories=20;
            _LIT(KName, "CategoryTP12 - %d");
            for (TInt i=0; i<KCategories; i++)
                {
                TBuf<100> categoryName;
                categoryName.Format(KName,i);
                CPosLandmarkCategory* lmc = CPosLandmarkCategory::NewLC();
                lmc->SetCategoryNameL(categoryName);  
                TInt err = KErrLocked;
                while (err == KErrLocked)
                    {
                    TRAP(err, cm->AddCategoryL(*lmc));
                    }
                CleanupStack::Pop(lmc);
                }
                
            const TInt KStartIndex=5;
            const TInt KNumOfItems=16;
            iter->GetItemIdsL(ids, KStartIndex, KNumOfItems);
            CleanupStack::PopAndDestroy(iter);
            }
        else if (self->iTestCase == KTestCaseLandmarkIterator)
            {
            CPosLmItemIterator* iterLm = lmd->LandmarkIteratorL();
            CleanupStack::PushL(iterLm);
            const TInt KLmStartIndex=0;
            const TInt KLmNumOfItems=1;
            iterLm->GetItemIdsL(ids, KLmStartIndex, KLmNumOfItems);
            CleanupStack::PopAndDestroy(iterLm);
            }
        }
    else if (self->iExpectedPanicCode == EPosNoneOrMultipleLandmarkAttributeSet)
        {
        if (self->iTestCase == KTestCaseSortPrefNoAttribute)
            {
            TPosLmSortPref sortPref(CPosLandmark::ENoAttribute);
            sortPref.SortOrder();
            }
        else if (self->iTestCase == KTestCaseSortPrefSeveralAttribute)
            {
            TUint32 attr = CPosLandmark::ELandmarkName | CPosLandmark::EPosition;
            TPosLmSortPref sortPref(attr);
            sortPref.SortOrder();
            }
        //Test that searching for landmarks with invalid attributes panics
        else if (self->iTestCase == KTestCaseLandmarkSearchinvalidAttribute)
            {
            CPosLandmarkSearch* landmarkSearch = CPosLandmarkSearch::NewL(*lmd);
            CleanupStack::PushL(landmarkSearch);
            // Invalid attribute, (correct ascended sorted)
            TPosLmSortPref sortPref((TUint32)5);

            CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();   
            textCriteria->SetTextL(_L("Library*"));
            CPosLmOperation* operation = landmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref);
            CleanupStack::PushL(operation);
            ExecuteAndDeleteLD(operation);
            CleanupStack::PopAndDestroy(operation);
            }
        //Test that it is not possible to set a TPosLmSortPref with invalid attributes
        else if (self->iTestCase == KTestCaseLandmarkSearchinvalidAttribute2)
            {
            TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
            sortPref.SetSortByLandmarkAttribute((TUint32)5, TPosLmSortPref::EAscending);
            } 
        }
    else if (self->iExpectedPanicCode == EPosNaNCoordinate)
        {
        TCoordinate coord;
        CPosLmNearestCriteria* lmnc = CPosLmNearestCriteria::NewLC(coord);  
        
        CleanupStack::PopAndDestroy(lmnc);
        }
    else if (self->iExpectedPanicCode == EPosInvalidLandmarkAttribute)
        {
        if (self->iTestCase == KTestCaseInvalidAttributes1)
        	{
        	// Test with incorrect attributes
	        CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
	        textCriteria->SetAttributesToSearch(CPosLandmark::EIcon);
	        CleanupStack::PopAndDestroy(textCriteria);
	        }
	    else if (self->iTestCase == KTestCaseInvalidAttributes2)
        	{
        	// Test with invalid attributes
        	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
	        textCriteria->SetAttributesToSearch(5);
	        CleanupStack::PopAndDestroy(textCriteria);
        	}
        }
    else if (self->iExpectedPanicCode == EPosInvalidPartialReadParameters)
        {
        CPosLmPartialReadParameters* partialParam = lmd->PartialReadParametersLC();
        if (self->iTestCase == KTestCasePartialAttributes1)
            {
            TUint attr = 348764;
            partialParam->SetRequestedAttributes(attr);   
            }
        else if (self->iTestCase == KTestCasePartialAttributes2)
            {
            TUint attr = CPosLandmark::ELandmarkName|
									CPosLandmark::EPosition|
									CPosLandmark::ECoverageRadius|
									CPosLandmark::ECategoryInfo|
									CPosLandmark::EIcon|
									CPosLandmark::EDescription;
            // attr equals 63 (see EPos_CPosLmPartialReadParameters.cpp)
            partialParam->SetRequestedAttributes(attr+1);
            }       
        CleanupStack::PopAndDestroy(partialParam);
        }
    else if (self->iExpectedPanicCode == EPosInvalidRequestedPositionFields)
        {
        CPosLmPartialReadParameters* partialParam = lmd->PartialReadParametersLC();
        RArray<TUint> reqFields;
        CleanupClosePushL(reqFields);
        
        if (self->iTestCase == KTestCaseInvalidRequestedPositionFields1)
            {
            // Test with index outside allowed interval
            TInt outside = EPositionFieldIdLast;
            reqFields.Append(++outside);
            partialParam->SetRequestedPositionFields(reqFields);
            }
        else if (self->iTestCase == KTestCaseInvalidRequestedPositionFields2)
            {
            // Test with index outside allowed interval
            TInt outside = EPositionFieldIdLast;

            reqFields.Append(EPositionFieldCountry);
            reqFields.Append(EPositionFieldCity);
            reqFields.Append(++outside);

            partialParam->SetRequestedPositionFields(reqFields);
            }
        CleanupStack::PopAndDestroy(&reqFields);
        CleanupStack::PopAndDestroy(partialParam);
        }
    else if (self -> iExpectedPanicCode == EPosNegativeValue)
        {
        if (self->iTestCase == KTestCaseNegativeDistance)
            {
            TCoordinate corr(19,20,10);
            CPosLmNearestCriteria* criteria =  CPosLmNearestCriteria::NewLC(corr, ETrue);
            criteria -> SetMaxDistance (-1); //should panic
            CleanupStack::PopAndDestroy(criteria);
            }
        else if (self ->iTestCase == KTestCaseNegativeRadius)
            {
            CPosLandmark* lm = CPosLandmark::NewLC();
            lm -> SetCoverageRadius(-1); // should panic
            CleanupStack::PopAndDestroy(lm);
            }
        }
    else if (self->iExpectedPanicCode == 130)
        {
        // The index supplied to method CPosLmCompositeCriteria::Argument must be in the 
        // interval [0, NumOfArguments - 1] otherwise cause a USER-130 Panic to be raised
        if (self->iTestCase == KTestCaseCompositeSearch1)
            {
            CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
            // Panic test 1, invalid position, should raise USER-130 panic
            /*CPosLmSearchCriteria& comp1 = */ composite->Argument(0);
            }
        else if (self->iTestCase == KTestCaseCompositeSearch2 || self->iTestCase == KTestCaseCompositeSearch3)
            {
            CPosLandmarkSearch* search = CPosLandmarkSearch::NewL(*lmd);
            CleanupStack::PushL(search);

            CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

            //CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
            //nameCriteria->SetSearchPatternL(_L("*"));
            
            CPosLmTextCriteria* textCriteria1 = CPosLmTextCriteria::NewLC();
            textCriteria1->SetTextL(_L("*"));
            
            TInt err = composite->AddArgument(textCriteria1);
            if (err != KErrNone) User::Leave(666);
            CleanupStack::Pop(textCriteria1);
            
            CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
            textCriteria->SetTextL(_L("*"));
            TUint attr = CPosLandmark::ELandmarkName | CPosLandmark::EDescription;
            textCriteria->SetAttributesToSearch(attr);
            
            err = composite->AddArgument(textCriteria);
            if (err != KErrNone) User::Leave(667);
            CleanupStack::Pop(textCriteria);
            
            TInt nr = composite->NumOfArguments();
            
            if (self->iTestCase == KTestCaseCompositeSearch2)
                {
                // Should raise a USER-130 panic
                /*CPosLmSearchCriteria& comp3 = */composite->Argument(nr);
                }
            else 
                {
                // Should raise a USER-130 panic
                /*CPosLmSearchCriteria& comp3 = */composite->Argument(nr+1);
                }
            CleanupStack::PopAndDestroy(search);
            }
        else if (self->iTestCase == KTestCaseCompositeSearch4)
            {
            CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
            // Panic test 1, invalid position, should raise USER-130 panic, unfortunately raises a compiler warning
            // about signed/unsigned
            
            TInt dummy = -1;
            composite->Argument(dummy);
            }
		else if (self->iTestCase == KTestCaseCompositeSearch5)
            {
            CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
            // Removing invalid position, should raise USER-130 panic, unfortunately raises a compiler warning
            // about signed/unsigned
            
            TInt dummy = -1;
            composite->RemoveArgument(dummy);
            }
		else if (self->iTestCase == KTestCaseCompositeSearch6)
            {
            CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
            // Removing invalid position, should raise USER-130 panic, unfortunately raises a compiler warning
            // about signed/unsigned
            
            TInt dummy = 0;
            composite->RemoveArgument(dummy);
            }            
		else if (self->iTestCase == KTestCaseCompositeSearch7)
            {
            CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
            // Removing invalid position, should raise USER-130 panic, unfortunately raises a compiler warning
            // about signed/unsigned
            
            
            CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
            textCriteria->SetTextL(_L("*"));
            composite->AddArgument(textCriteria);
            CleanupStack::Pop(textCriteria);
            
            TInt dummy = 1;
            composite->RemoveArgument(dummy);
            }
		else if (self->iTestCase == KTestCaseCompositeSearch8)
            {
            CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
            // Removing invalid position, should raise USER-130 panic, unfortunately raises a compiler warning
            // about signed/unsigned
            
            
            CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
            textCriteria->SetTextL(_L("*"));
            composite->AddArgument(textCriteria);
            CleanupStack::Pop(textCriteria);
            
            TInt dummy = 2;
            composite->RemoveArgument(dummy);
            }            
        }
    else if (self->iTestCase == KTestCaseDuplicateNotifyDatabaseEvent)
        {
        self->iExpectedPanicCode = static_cast<TPositionLandmarksClientPanic>(2);
        CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
        CleanupStack::PushL(lmd);
        TRequestStatus status1;
        TPosLmEvent event1;
        TRequestStatus status2;
        TPosLmEvent event2;
        lmd->NotifyDatabaseEvent(event1, status1);
        // Just in case
        User::After(1000000);
        // Should raise a EPosEventNotifierAlreadyHasOutstandingRequest Panic
        lmd->NotifyDatabaseEvent(event2, status2);
        // Just in case
        User::After(1000000);
        CleanupStack::PopAndDestroy(lmd);
        }
    else if (self->iExpectedPanicCode == EPosInvalidEnumValue)
        {
        CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
        CleanupStack::PushL(lmd);
        CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
        if (self->iTestCase == KTestCaseIncorrectCompositeType1)
            {
            composite->SetCompositionType((CPosLmCompositeCriteria::TCompositionType)10);
            }
        else if (self->iTestCase == KTestCaseIncorrectCompositeType2)
            {
            composite->SetCompositionType((CPosLmCompositeCriteria::TCompositionType)-1);
            }
        else if (self->iTestCase == KTestCaseIncorrectCompositeType3)
            {
            CPosLmCompositeCriteria* composite2 = CPosLmCompositeCriteria::NewLC((CPosLmCompositeCriteria::TCompositionType)5);
            CleanupStack::PopAndDestroy(composite2);
            }
        else if (self->iTestCase == KTestCaseIncorrectCompositeType4)
            {
            CPosLmCompositeCriteria* composite2 = CPosLmCompositeCriteria::NewLC((CPosLmCompositeCriteria::TCompositionType)-5);
            CleanupStack::PopAndDestroy(composite2);
            }
        CleanupStack::PopAndDestroy(composite);
        
        // Test that using invalid sortpref panics
        if (self->iTestCase == KTestCaseCategoryIteratorSortPref)
            {
            CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*lmd);
            CPosLmItemIterator* catIter = categoryManager->CategoryIteratorL((CPosLmCategoryManager::TCategorySortPref)3);
            CleanupStack::PushL(catIter);
            CleanupStack::PopAndDestroy(catIter);
            }
        // Test that using invalid sortpref panics
        else if (self->iTestCase == KTestCaseReferencedCategoryIteratorSortPref)
            {
            CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*lmd);
            CPosLmItemIterator* catIter = categoryManager->ReferencedCategoryIteratorL((CPosLmCategoryManager::TCategorySortPref)4);
            CleanupStack::PushL(catIter);
            CleanupStack::PopAndDestroy(catIter);
            }
        //Test that using invalid sortpref panics
        else if (self->iTestCase == KTestCaseCategorySearchSortPref)
            {
            CPosLandmarkSearch* landmarkSearch = CPosLandmarkSearch::NewL(*lmd);
            CleanupStack::PushL(landmarkSearch);
            
            CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
            // Use wildcard * as search pattern
            nameCriteria->SetSearchPatternL(_L("Pub"));
            ExecuteAndDeleteLD(landmarkSearch->StartCategorySearchL(*nameCriteria, (CPosLmCategoryManager::TCategorySortPref)5));
            CleanupStack::PopAndDestroy(nameCriteria);
            CleanupStack::PopAndDestroy(landmarkSearch);
            }
        //Test that it is not possible to create a TPosLmSortPref with invalid value
        else if (self->iTestCase == KTestCaseLandmarkSortPref)
            {           
            TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, 
                                    (TPosLmSortPref::TSortOrder)4321);
            }
        //Test that it is not possible to set a TPosLmSortPref to invalid values
        else if (self->iTestCase == KTestCaseLandmarkSortPref2)
            {           
            TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
            sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, (TPosLmSortPref::TSortOrder)25);
            }
        CleanupStack::PopAndDestroy(lmd);
        }
    else if (self->iExpectedPanicCode == EPosInvalidOperationMode)
        {
        CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
        CleanupStack::PushL(lmd);
        
        CPosLandmarkSearch* landmarkSearch = CPosLandmarkSearch::NewL(*lmd);
        CleanupStack::PushL(landmarkSearch);

        CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();   
        textCriteria->SetTextL(_L("Library*"));
        CPosLmOperation* operation = landmarkSearch->StartLandmarkSearchL(*textCriteria);
        CleanupStack::PushL(operation);

        if (self->iTestCase == KTestCaseInvalidOperationMode1)
            {
            // Two subsequent ExecuteL calls should Panic
            operation->ExecuteL();
            operation->ExecuteL();
            }
        else if (self->iTestCase == KTestCaseInvalidOperationMode2)
            {
            operation->ExecuteL();
            TRequestStatus status;
            TReal32 progress;
            operation->NextStep(status, progress);
            }
        else if (self->iTestCase == KTestCaseInvalidOperationMode3)
            {
            TRequestStatus status;
            TReal32 progress;
            operation->NextStep(status, progress);
            operation->ExecuteL();
            }
        else if (self->iTestCase == KTestCaseInvalidOperationMode4)
            {
            TRequestStatus status;
            TReal32 progress;
            operation->NextStep(status, progress);
            operation->NextStep(status, progress);
            }
        CleanupStack::PopAndDestroy(operation);
        CleanupStack::PopAndDestroy(textCriteria);
        CleanupStack::PopAndDestroy(landmarkSearch);
        CleanupStack::PopAndDestroy(lmd);
        }
    else if (self->iExpectedPanicCode == EPosLmInvalidArgument)
        {
        if (self->iTestCase == KTestCaseNegativeMatches)
            {
            CPosLandmarkSearch* search = CPosLandmarkSearch::NewL(*lmd);
            CleanupStack::PushL(search);
            search->SetMaxNumOfMatches(-10);
            CleanupStack::PopAndDestroy(search);
            }
       else if (self->iTestCase == KTestCaseZeroMatches)
            {
            CPosLandmarkSearch* search = CPosLandmarkSearch::NewL(*lmd);
            CleanupStack::PushL(search);
            search->SetMaxNumOfMatches(0);
            CleanupStack::PopAndDestroy(search);
            }
        else if (self->iTestCase == KTestLandmarkSetIconTest1)
            {
            CPosLandmark* lm = CPosLandmark::NewLC();
            // Panics here
            lm->SetIconL(_L("IconFileName"), -2, KPosLmIconMaskNotUsed);
            CleanupStack::PopAndDestroy(lm);
            }
        else if (self->iTestCase == KTestLandmarkSetIconTest2)
            {
            CPosLandmark* lm = CPosLandmark::NewLC();
            // Panics here
            lm->SetIconL(_L("IconFileName"), 1, -2);
            CleanupStack::PopAndDestroy(lm);
            }
        else if (self->iTestCase == KTestCategorySetIconTest1)
            {
            CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
            category->SetCategoryNameL(_L("NewCategory1"));
            // Panics here
            category->SetIconL(_L("IconFileName"), -5, KPosLmIconMaskNotUsed);
            CleanupStack::PopAndDestroy(category);
            }
        else if (self->iTestCase == KTestCategorySetIconTest2)
            {
            CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
            category->SetCategoryNameL(_L("NewCategory2"));
            // Panics here
            category->SetIconL(_L("IconFileName"), 5, -2);
            CleanupStack::PopAndDestroy(category);
            }
        }
    
    CleanupStack::PopAndDestroy(4,&ids);
    }

LOCAL_C TInt ThreadFunction(TAny* aData)
    {
    CTrapCleanup* cleanup=CTrapCleanup::New(); 

    CActiveScheduler* actSch = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(actSch);

    TRAPD(err, CPosTp12::RunPanicTestL(aData));
    
    delete actSch;
    delete cleanup;
    return err;
    }

// ---------------------------------------------------------
// CPosTp12::CreateThreadL
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CPosTp12::CreateThreadL()
    {
    _LIT(KThreadName, "TP12testthread%d");
    _LIT(KCreateThreadErr, "Create thread failed with %d");

	TBuf<100> buf;
	buf.Format(KThreadName, iTreadCounter++);
     TInt err=0;
     err = iThread.Create(buf, ThreadFunction, KDefaultStackSize, KMinHeapSize, KMaxHeapSize, this);
        
     AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);                                                                  
            
    }

// ---------------------------------------------------------
// CPosTp12::ResourceFilePanicTestLL
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CPosTp12::ResourceFilePanicTestL()
    {
    RemoveDefaultDbL();
    CPosLandmarkDatabase* lmd=UseInvalidResourceFileL();
    CleanupStack::PushL(lmd);

    if (lmd->IsInitializingNeeded())
        {
        ExecuteAndDeleteLD(lmd->InitializeL()); 
        }
        
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*lmd); 
    CleanupStack::PushL(categoryManager);

    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);

    ids.Append(1);
    ExecuteAndDeleteLD(categoryManager->RemoveCategoriesL(ids)); 
   
    CleanupStack::PopAndDestroy(3, lmd);
    }

// ---------------------------------------------------------
// CPosTp12::UseInvalidGlobalCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
CPosLandmarkDatabase* CPosTp12::UseInvalidResourceFileL()
    {
    _LIT(KInvalidResourceFile, "c:\\system\\test\\testdata\\EPOSLMASYNCOPS.RSC");
    
    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);

    TInt err = fileMan->Rename(KResourceFile, KTempResourceFile, CFileMan::EOverWrite);
    if (err != KErrNone && err != KErrNotFound)
        {
        iLog->Log(_L("Unexpected error when trying to rename EPOSLMASYNCOPS.RSC"));
        User::Leave(err);
        }

    User::LeaveIfError(fileMan->Copy(KInvalidResourceFile, KResourceFilePath, CFileMan::EOverWrite));

    CleanupStack::PopAndDestroy(fileMan);

    return CPosLandmarkDatabase::OpenL();
    }

// ---------------------------------------------------------
// CPosTp12::RestoreResourceL
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CPosTp12::RestoreResourceFileL()
    {
    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);

    User::LeaveIfError(fileMan->Delete(KResourceFile));

    TInt err = fileMan->Rename(KTempResourceFile, KResourceFile, CFileMan::EOverWrite);
    if (err != KErrNone && err != KErrNotFound)
        {
        iLog->Log(_L("Unexpected error when trying to rename TEMP_EPOSLMASYNCOPS.RSC"));
        User::Leave(err);
        }

    CleanupStack::PopAndDestroy(fileMan);

    }

//  End of File
