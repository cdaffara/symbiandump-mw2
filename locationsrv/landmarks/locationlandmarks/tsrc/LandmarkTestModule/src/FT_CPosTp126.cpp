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
#include "FT_CPosTp126.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmMultiDbSearch.h> 
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmCatNameCriteria.h>
#include <EPos_CPosLmAreaCriteria.h>
#include <EPos_CPosLmCategoryCriteria.h>
#include <EPos_CPosLmCompositeCriteria.h>
#include <EPos_CPosLmIdListCriteria.h>
#include <EPos_CPosLmNearestCriteria.h>
#include <EPos_CPosLmSearchCriteria.h>
#include <EPos_TPosLMSortPref.h>
#include <EPos_CPosLmDatabaseManager.h>
#include "FT_LandmarkConstants.h"
#include <EPos_CPosLandmarkSearch.h>
//#include <ss_std.h>

     
// CONSTANTS
_LIT(KWildcard, "*");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp126::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp126::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    delete iLandmarksMultiDbSearch;
    iLandmarksMultiDbSearch = NULL;
    
    iThread.Close();
    }

// ---------------------------------------------------------
// CPosTp126::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp126::StartL()
    {
    RemoveAllLmDatabasesL();
    CopyTestDbFileL(KDb20);
    CopyTestDbFileL(KDb40);
    CopyTestDbFileL(KDb60);
    CopyTestDbFileL(KDb80);
    CopyTestDbFileL(KDb105);
    
    CDesCArray* dbList = new (ELeave) CDesCArrayFlat(8);
    CleanupStack::PushL(dbList);
    dbList->AppendL(KDb20);
    dbList->AppendL(KDb40);
    dbList->AppendL(KDb60);
    dbList->AppendL(KDb80);
    dbList->AppendL(KDb105);
    
    iLandmarksMultiDbSearch = CPosLmMultiDbSearch::NewL(*dbList);
    TInt err = KErrNone;
    
    // Test that a 'use previous matches' search doesn't work in the first search.
    CPosLmTextCriteria* lmtextCriteria = CPosLmTextCriteria::NewLC();
    lmtextCriteria->SetTextL(KWildcard);
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending); 
    TRAP(err, ExecuteAndDeleteLD(iLandmarksMultiDbSearch->StartLandmarkSearchL(*lmtextCriteria,
                                                                               sortPref,
                                                                               ETrue)));
                                                                               
    _LIT(KErrorWrongCodePreviousMatches, "Wrong error code for searching in previous matches when there are no previous matches");                                       
    AssertTrueSecL(err == KErrArgument, KErrorWrongCodePreviousMatches, err);
    
    // Test that a 'use previous matches' search doesn't work in the first search. (category search)
    CPosLmCategoryManager::TCategorySortPref sortPrefCat = CPosLmCategoryManager::ECategorySortOrderNameAscending;
    CPosLmCatNameCriteria* nameCriteriaCat = CPosLmCatNameCriteria::NewLC();
    nameCriteriaCat->SetSearchPatternL(KWildcard);
    TRAP(err, ExecuteAndDeleteLD(iLandmarksMultiDbSearch->StartCategorySearchL(*nameCriteriaCat,
                                                                               sortPrefCat,
                                                                               ETrue)));
    _LIT(KErrorWrongCodePreviousMatchesCat, "Wrong error code for searching in previous matches when there are no previous matches (categories)");                                       
    AssertTrueSecL(err == KErrArgument, KErrorWrongCodePreviousMatchesCat, err);
    
    CleanupStack::PopAndDestroy(nameCriteriaCat);
    
    // Check that only correct criteria works with category searches:
    CPosLmAreaCriteria* areaCriteria = CPosLmAreaCriteria::NewLC(5,5,5,5);
    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
    CPosLmCompositeCriteria* compCriteria =
      CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
    CPosLmIdListCriteria* idListCriteria = CPosLmIdListCriteria::NewLC();
    CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(TCoordinate(10,10));
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    CPosLmCategoryCriteria* catCriteria = CPosLmCategoryCriteria::NewLC();
    nameCriteria->SetSearchPatternL(KWildcard);
    RArray<TPosLmItemId> idArray;
    CleanupClosePushL(idArray);
    idArray.Append(3);
    idArray.Append(4);
    idListCriteria->SetLandmarkIdsL(idArray);
    textCriteria->SetTextL(KWildcard);
    catCriteria->SetCategoryNameL(KWildcard);
    textCriteria->SetTextL(KWildcard);
    
    _LIT(KErrorWrongCodeForUnsupportedCriteria,"Wrong error code for unsupported criteria in category search");
    TRAP(err, ExecuteAndDeleteLD(iLandmarksMultiDbSearch->StartCategorySearchL(*catCriteria,sortPrefCat)));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCodeForUnsupportedCriteria, err);
    TRAP(err, ExecuteAndDeleteLD(iLandmarksMultiDbSearch->StartCategorySearchL(*textCriteria,sortPrefCat)));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCodeForUnsupportedCriteria, err);
    TRAP(err, ExecuteAndDeleteLD(iLandmarksMultiDbSearch->StartCategorySearchL(*areaCriteria,sortPrefCat)));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCodeForUnsupportedCriteria, err);
    TRAP(err, ExecuteAndDeleteLD(iLandmarksMultiDbSearch->StartCategorySearchL(*compCriteria,sortPrefCat)));
    // Composite is not supported in category search
    AssertTrueSecL(err == KErrNotSupported, KErrorWrongCodeForUnsupportedCriteria, err);
    TRAP(err, ExecuteAndDeleteLD(iLandmarksMultiDbSearch->StartCategorySearchL(*idListCriteria,sortPrefCat)));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCodeForUnsupportedCriteria, err);
    TRAP(err, ExecuteAndDeleteLD(iLandmarksMultiDbSearch->StartCategorySearchL(*nearestCriteria,sortPrefCat)));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCodeForUnsupportedCriteria, err);
    
    // This should not leave:
    ExecuteAndDeleteLD(iLandmarksMultiDbSearch->StartCategorySearchL(*nameCriteria,sortPrefCat));
    
    // Start a search and test that a new search cannot start until it is finished:
    CPosLmOperation* op = iLandmarksMultiDbSearch->StartLandmarkSearchL(*lmtextCriteria,sortPref);
    CleanupStack::PushL(op);
    TRAP(err, iLandmarksMultiDbSearch->StartLandmarkSearchL(*lmtextCriteria,sortPref));
    _LIT(KErrorWrongCodeForInUse, "Wrong error code when testing 'in use' for multi db searches.");
    AssertTrueSecL(err == KErrInUse, KErrorWrongCodeForInUse, err);
    
    // Check that a new db list cannot be set during a search.
    TRAP(err,iLandmarksMultiDbSearch->SetDatabasesToSearchL(*dbList));
    _LIT(KErrorWrongCodeForInUseSetDB, "Wrong error code when testing 'in use' for SetDatabasesToSearchL.");
    AssertTrueSecL(err == KErrInUse, KErrorWrongCodeForInUseSetDB, err);
    TRAP(err, iLandmarksMultiDbSearch->StartCategorySearchL(*nameCriteria,sortPrefCat));
    ExecuteAndDeleteLD(op);
    CleanupStack::Pop(op);
    AssertTrueSecL(err == KErrInUse, KErrorWrongCodeForInUse, err);
    
    // Start a category search and test that a new search cannot start until it is finished:
    op = iLandmarksMultiDbSearch->StartCategorySearchL(*nameCriteria,sortPrefCat);
    CleanupStack::PushL(op);
    TRAP(err, iLandmarksMultiDbSearch->StartCategorySearchL(*nameCriteria,sortPrefCat));
    AssertTrueSecL(err == KErrInUse, KErrorWrongCodeForInUse, err);
    TRAP(err, iLandmarksMultiDbSearch->StartLandmarkSearchL(*lmtextCriteria,sortPref));
    ExecuteAndDeleteLD(op);
    CleanupStack::Pop(op);
    AssertTrueSecL(err == KErrInUse, KErrorWrongCodeForInUse, err);
    
    // Test that an id list criteria cannot be used.
    TRAP(err, ExecuteAndDeleteLD(iLandmarksMultiDbSearch->StartLandmarkSearchL(*idListCriteria,
                                                                               sortPref)));
    _LIT(KErrorWrongCodeForIdListCriteria, "Wrong error code for id list criteria.");
    AssertTrueSecL(err == KErrArgument, KErrorWrongCodeForIdListCriteria, err);
    
    CleanupStack::PopAndDestroy(); // idArray
    CleanupStack::PopAndDestroy(catCriteria);
    CleanupStack::PopAndDestroy(textCriteria);
    CleanupStack::PopAndDestroy(nearestCriteria);
    CleanupStack::PopAndDestroy(idListCriteria);
    CleanupStack::PopAndDestroy(compCriteria);
    CleanupStack::PopAndDestroy(nameCriteria);
    CleanupStack::PopAndDestroy(areaCriteria);
    
    
    ///////////////////////////////////////////////////////////////////////////
    // Panic tests ////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    
    //Set max nr of matches to zero:
    iStep = EStepZeroMaxMatches;
    iExpectedPanicCode = EPosLmInvalidArgument;
    ResumeThreadAndVerifyExitL();

    // Set max nr of matches to a negative value:
    iStep = EStepNegativeMaxMatches;
    iExpectedPanicCode = EPosLmInvalidArgument;
    ResumeThreadAndVerifyExitL();
    
    //Set max nr of matches to constant KPosLmMaxNumOfMatchesUnlimited:
    // This is allowed so we use our own panic code to verify that method 
    // was correctly returned
    iStep = EStepUnlimitedMaxMatches;
    iExpectedPanicCode = (TPositionLandmarksClientPanic)567;
    ResumeThreadAndVerifyExitL();
    
    // Invalid index to DatabaseUriPtr method:
    iStep = EStepOutOfBoundsDatabaseUriPtr;
    iExpectedPanicCode = EPosInvalidIndex;
    ResumeThreadAndVerifyExitL();
    
    // Invalid index for GetSearchError:
    iStep = EStepOutOfBoundsGetSearchError;
    iExpectedPanicCode = EPosInvalidIndex;
    ResumeThreadAndVerifyExitL();
    
    // Invalid TPosLmSortPref
    iStep = EStepIllegalSortPrefLMa;
    iExpectedPanicCode = EPosInvalidEnumValue;
    ResumeThreadAndVerifyExitL();
    
    // Invalid TPosLmSortPref
    iStep = EStepIllegalSortPrefLMb;
    iExpectedPanicCode = EPosInvalidEnumValue;
    ResumeThreadAndVerifyExitL();
    
    // Invalid TPosLmSortPref
    iStep = EStepIllegalSortPrefLMc;
    iExpectedPanicCode = EPosNoneOrMultipleLandmarkAttributeSet;
    ResumeThreadAndVerifyExitL();
    
    // Invalid TPosLmSortPref
    iStep = EStepIllegalSortPrefLMd;
    iExpectedPanicCode = EPosNoneOrMultipleLandmarkAttributeSet;
    ResumeThreadAndVerifyExitL();
    
    // Invalid TPosLmSortPref
    iStep = EStepIllegalSortPrefCat;
    iExpectedPanicCode = EPosInvalidEnumValue;
    ResumeThreadAndVerifyExitL();
    
    // Invalid index for MatchIteratorL:
    iStep = EStepTooBigValueMatchIteratorL;
    iExpectedPanicCode = EPosInvalidIndex;
    ResumeThreadAndVerifyExitL();
    
    // Invalid index for NumOfMatches:
    iStep = EStepTooBigValueNumOfMatches;
    iExpectedPanicCode = EPosInvalidIndex;
    ResumeThreadAndVerifyExitL();
    
    // Test LandmarkSearch with NULL search criteria
    iStep = EStepNULLSearch1;
    iExpectedPanicCode = (TPositionLandmarksClientPanic)3; // KernExec-3
    ResumeThreadAndVerifyExitL();
    
    // Test LandmarkSearch with NULL search criteria
    iStep = EStepNULLSearch2;
    iExpectedPanicCode = (TPositionLandmarksClientPanic)3; // KernExec-3
    ResumeThreadAndVerifyExitL();
    
    // Test CategorySearch with NULL search criteria
    iStep = EStepNULLSearch3;
    iExpectedPanicCode = (TPositionLandmarksClientPanic)3; // KernExec-3
    ResumeThreadAndVerifyExitL();
    
    // Test LandmarkSearch with Databases to search with NULL value
    iStep = EStepNULLSearch4;
    iExpectedPanicCode = (TPositionLandmarksClientPanic)3; // KernExec-3
    ResumeThreadAndVerifyExitL();
    
    CleanupStack::PopAndDestroy(lmtextCriteria);
    CleanupStack::PopAndDestroy(dbList);
    }
    
// ---------------------------------------------------------
// CPosTp126::RunPanicTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CPosTp126::RunPanicTestL(TAny* aData)
    {
    CPosTp126* self = reinterpret_cast<CPosTp126*>(aData);
    
    CDesCArray* dbList = new (ELeave) CDesCArrayFlat(8);
    CleanupStack::PushL(dbList);
    dbList->AppendL(KDb20);
    dbList->AppendL(KDb40);
    dbList->AppendL(KDb60);
    dbList->AppendL(KDb80);
    dbList->AppendL(KDb105);
    
    CPosLmTextCriteria* textCriteria = NULL;
    CPosLmMultiDbSearch* multiSearch = CPosLmMultiDbSearch::NewL(*dbList);
    switch(self->iStep)
        {
        case EStepZeroMaxMatches:
            // Not allowed to set 0 number of matches
            multiSearch->SetMaxNumOfMatches(0);
        break;
        case EStepUnlimitedMaxMatches:
            multiSearch->SetMaxNumOfMatches(KPosLmMaxNumOfMatchesUnlimited);
            User::Panic(_L("567"), 567);
        break;
        case EStepNegativeMaxMatches:
        multiSearch->SetMaxNumOfMatches(-10);
        break;
        case EStepOutOfBoundsDatabaseUriPtr:
            multiSearch->DatabaseUriPtr(6);
        break;
        case EStepOutOfBoundsGetSearchError:
            textCriteria = CPosLmTextCriteria::NewLC();
            textCriteria->SetTextL(KWildcard);
            multiSearch->SetMaxNumOfMatches(3);
            ExecuteAndDeleteLD(multiSearch->StartLandmarkSearchL(*textCriteria));
            TInt errors = multiSearch->NumOfSearchErrors();
            CPosLmMultiDbSearch::TSearchError searchError;
            multiSearch->GetSearchError(errors + 2, searchError); // too large, should panic
            CleanupStack::PopAndDestroy(textCriteria);
        break;
        case EStepTooBigValueMatchIteratorL:
            textCriteria = CPosLmTextCriteria::NewLC();
            textCriteria->SetTextL(KWildcard);
            ExecuteAndDeleteLD(multiSearch->StartLandmarkSearchL(*textCriteria));
            multiSearch->MatchIteratorL(6); // Panic here
            CleanupStack::PopAndDestroy(textCriteria);
        break;
        case EStepTooBigValueNumOfMatches:
            textCriteria = CPosLmTextCriteria::NewLC();
            textCriteria->SetTextL(KWildcard);
            ExecuteAndDeleteLD(multiSearch->StartLandmarkSearchL(*textCriteria));
            multiSearch->NumOfMatches(6); // Panic here
            CleanupStack::PopAndDestroy(textCriteria);
        break;
        case EStepIllegalSortPrefLMa:
            TPosLmSortPref sortPrefa(CPosLandmark::ELandmarkName, 
                                    (TPosLmSortPref::TSortOrder)4321);
        break;
        case EStepIllegalSortPrefLMb:
            TPosLmSortPref sortPrefb(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
            sortPrefb.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName,
                                                 (TPosLmSortPref::TSortOrder)4321); // Panic here
        break;
        case EStepIllegalSortPrefLMc:
            TPosLmSortPref sortPrefc(0, TPosLmSortPref::EAscending);
        break;
        case EStepIllegalSortPrefLMd:
            TPosLmSortPref sortPrefd(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
            sortPrefd.SetSortByLandmarkAttribute(0, TPosLmSortPref::EAscending);
        case EStepIllegalSortPrefCat:
            CPosLmCategoryManager::TCategorySortPref sortPrefCat = 
               (CPosLmCategoryManager::TCategorySortPref)12345;
            CPosLmCatNameCriteria* nameCriteriaCat = CPosLmCatNameCriteria::NewLC();
            nameCriteriaCat->SetSearchPatternL(KWildcard);
            ExecuteAndDeleteLD(multiSearch->StartCategorySearchL(*nameCriteriaCat,
                                                                 sortPrefCat)); // Panic here
            CleanupStack::PopAndDestroy(nameCriteriaCat);
        break;
        case EStepNULLSearch1:
            textCriteria = NULL;
            ExecuteAndDeleteLD(multiSearch->StartLandmarkSearchL(*textCriteria));
        break;
        case EStepNULLSearch2:
            textCriteria = NULL;
            TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending); 
            ExecuteAndDeleteLD(multiSearch->StartLandmarkSearchL(*textCriteria, sortPref));
        break;
        case EStepNULLSearch3:
            CPosLmCategoryManager::TCategorySortPref sortPrefCat2 = CPosLmCategoryManager::ECategorySortOrderNameAscending;
            CPosLmCatNameCriteria* criteria = NULL;
            ExecuteAndDeleteLD(multiSearch->StartCategorySearchL(*criteria, sortPrefCat2));
        break;
        case EStepNULLSearch4:
            CDesCArray* list = NULL;
            CPosLmMultiDbSearch* multiSearch = CPosLmMultiDbSearch::NewL(*list);
            textCriteria = CPosLmTextCriteria::NewLC();
            textCriteria->SetTextL(KWildcard);
            TPosLmSortPref sortPref4(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending); 
            ExecuteAndDeleteLD(multiSearch->StartLandmarkSearchL(*textCriteria, sortPref4));
        break;
        default:
        break;
                
        }
    }
    
// ---------------------------------------------------------
// CPosTp126::ResumeThreadAndVerifyExitL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp126::ResumeThreadAndVerifyExitL()
    {
    iLog->Log(_L("ResumeThreadAndVerifyExitL"));
    _LIT(KPanicErr, "Method didn't Panic when expected"); 
    _LIT(KPanicCodeErr, "Panicked with unexpected panic code");

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
// ThreadFunction
//
// ---------------------------------------------------------

LOCAL_C TInt ThreadFunction(TAny* aData)
    {
    CTrapCleanup* cleanup=CTrapCleanup::New(); 

    CActiveScheduler* actSch = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(actSch);

    TRAPD(err, CPosTp126::RunPanicTestL(aData));
    
    delete actSch;
    delete cleanup;
    return err;
    }
    
// ---------------------------------------------------------
// ---------------------------------------------------------
void CPosTp126::CreateThreadL()
    {
    _LIT(KThreadName, "TP126testthread%d");
    _LIT(KCreateThreadErr, "Create thread failed with %d");

	TBuf<100> buf;
	buf.Format(KThreadName, (TInt)iStep);
     
    TInt err = iThread.Create(buf, ThreadFunction, KDefaultStackSize, KMinHeapSize, KMaxHeapSize, this);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    }


//  End of File
