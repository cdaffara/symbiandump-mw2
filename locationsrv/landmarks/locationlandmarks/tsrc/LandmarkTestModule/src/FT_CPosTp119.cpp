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
#include "FT_CPosTp119.h"
#include "FT_LandmarkConstants.h"
#include <EPos_CPosLmMultiDbSearch.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_TPosLmSortPref.h>
#include "badesca.h"

//  CONSTANTS

// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp119::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp119::StartL()
    {  
    // PrepareDatabases
    const TInt KNrOfDatabases = 5;
    RemoveAllLmDatabasesL();
    CopyTestDbFileL(KDb20);
    CopyTestDbFileL(KDb40);
    CopyTestDbFileL(KDb60);
    CopyTestDbFileL(KDb80);
    CopyTestDbFileL(KDb105);
    
    // List databases
    // TBool searchRefined = ETrue;
    CPosLmDatabaseManager* dbMan = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbMan);
    CDesCArray* dbUris = dbMan->ListDatabasesLC();
    CleanupStack::Pop(dbUris);
    AssertTrueSecL(dbUris->Count() == KNrOfDatabases, _L("Wrong number of test databases!"));
    CleanupStack::PopAndDestroy(dbMan);
    CleanupStack::PushL(dbUris);
    
    // 1-2. Search in all databases
    SearchAllDbsL(dbUris, ENoSort);
    
    // 3-4. Remove one of the databases and re-search
    RemoveDatabaseL(_L("c:\\private\\100012a5\\DBS_101FE978_EPOSLM_080.LDB"));
    SearchAllButOneL(dbUris);
    
    // 5-6. Restore all ofthe databases but one and re-search
    RemoveDatabaseL(_L("c:\\private\\100012a5\\DBS_101FE978_EPOSLM_105.LDB"));
    RemoveDatabaseL(_L("c:\\private\\100012a5\\DBS_101FE978_EPOSLM_040.LDB"));
    RemoveDatabaseL(_L("c:\\private\\100012a5\\DBS_101FE978_EPOSLM_020.LDB"));
    SearchOneDbL(dbUris);    
    
    // 7-8. Restore all ofthe databases and re-search
    CopyTestDbFileL(KDb20);
    CopyTestDbFileL(KDb40);
    CopyTestDbFileL(KDb80);
    CopyTestDbFileL(KDb105);
    SearchAllDbsL(dbUris, EAsc);
    
    // 9. Test SetMaxNumOfMatches
    TestMaxNrOfMatchesL(dbUris);
   
    // 10. Test MatchIteratorL
    TestMatchIteratorL(dbUris);
    
    // 11-12. Search all + one non-existing
    SearchNonExistingDbL(dbUris);
    
    // 13. Search database multiple times
    SearchDbMultipleTimesL(dbUris);
    
    // 14-15. Search all + one non-existing + one with invalid URI
    SearchInvalidURIDbL(dbUris);
    
    CleanupStack::PopAndDestroy(dbUris);
	}

// ---------------------------------------------------------
// CPosTp119::SearchAllDbsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp119::SearchAllDbsL(CDesCArray* aDbUris, TTp119SortOrder aSortOrder)
    {
    CPosLmMultiDbSearch* multiSearcher = CPosLmMultiDbSearch::NewL(*aDbUris);
    CleanupStack::PushL(multiSearcher);
    VerifyDbsToSearchL(multiSearcher, aDbUris);
    
    TBool searchRefined = ETrue;
    SearchForLandmarksL(multiSearcher, aSortOrder, !searchRefined);
    
    // Verify Result - no errors should have occurred and all databases should return matches
    TUint searchErrors = VerifyResultL(multiSearcher, *aDbUris, aSortOrder);
    AssertTrueSecL(searchErrors == 0, _L("Found %d search errors"), searchErrors);
    for (TInt i = 0; i < aDbUris->Count(); i++)
        {
        AssertTrueSecL(multiSearcher->NumOfMatches(i) > 0, _L("Found no matches"));
        }
        
    CleanupStack::PopAndDestroy(multiSearcher);    
    }
       
// ---------------------------------------------------------
// CPosTp119::SearchAllButOneL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp119::SearchAllButOneL(CDesCArray* aDbUris)
    {
    CPosLmMultiDbSearch* multiSearcher = CPosLmMultiDbSearch::NewL(*aDbUris);
    CleanupStack::PushL(multiSearcher);
    VerifyDbsToSearchL(multiSearcher, aDbUris);
    
    TBool searchRefined = ETrue;
    SearchForLandmarksL(multiSearcher, EAsc, !searchRefined);
    
    // Verify Result - KErrNotFound for the removed database (eposlm_80.ldb) - Matches from all but this.
    TUint searchErrors = VerifyResultL(multiSearcher, *aDbUris, EAsc);
    AssertTrueSecL(searchErrors == 1, _L("Found %d search errors"), searchErrors);
    CPosLmMultiDbSearch::TSearchError searchErr;
    multiSearcher->GetSearchError(0, searchErr);
    AssertTrueSecL(searchErr.iErrorCode == KErrNotFound, _L("Expected KErrNotFound but got %d"), searchErr.iErrorCode);
    TBuf<64> uriWithError((*aDbUris)[searchErr.iDatabaseIndex]);
    uriWithError.LowerCase();
    AssertTrueSecL(uriWithError == _L("file://c:eposlm_080.ldb"), _L("Error from wrong database"));
    for (TInt i = 0; i < aDbUris->Count(); i++)
        {
        if (i != searchErr.iDatabaseIndex)
            {
            AssertTrueSecL(multiSearcher->NumOfMatches(i) > 0, _L("Found no matches"));
            }
        }
        
    CleanupStack::PopAndDestroy(multiSearcher);
    }
    
// ---------------------------------------------------------
// CPosTp119::SearchOneDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp119::SearchOneDbL(CDesCArray* aDbUris)
    {
    CPosLmMultiDbSearch* multiSearcher = CPosLmMultiDbSearch::NewL(*aDbUris);
    CleanupStack::PushL(multiSearcher);
    VerifyDbsToSearchL(multiSearcher, aDbUris);
    
    TBool searchRefined = ETrue;
    SearchForLandmarksL(multiSearcher, EAsc, !searchRefined);
    
    // Verify Result - Only matches from the one (eposlm_60.ldb) remaining. KErrNotFound from the others
    TUint searchErrors = VerifyResultL(multiSearcher, *aDbUris, EAsc);
    AssertTrueSecL(searchErrors == 4, _L("Found %d search errors"), searchErrors);
    for (TInt i = 0; i < searchErrors; i++)
        {
        CPosLmMultiDbSearch::TSearchError searchErr;
        multiSearcher->GetSearchError(i, searchErr);
        AssertTrueSecL(searchErr.iErrorCode == KErrNotFound, _L("Expected KErrNotFound but got %d"), searchErr.iErrorCode);
        TBuf<64> uriWithError((*aDbUris)[searchErr.iDatabaseIndex]);
        uriWithError.LowerCase();
        AssertTrueSecL(uriWithError != _L("file://c:eposlm_60.ldb"), _L("Eror from wong database"));
        }
        
    CleanupStack::PopAndDestroy(multiSearcher);
    }

// ---------------------------------------------------------
// CPosTp119::SearchNonExistingDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp119::SearchNonExistingDbL(CDesCArray* aDbUris)
    {    
    CPosLmMultiDbSearch* multiSearcher = CPosLmMultiDbSearch::NewL(*aDbUris);
    CleanupStack::PushL(multiSearcher);    
    VerifyDbsToSearchL(multiSearcher, aDbUris);
  
    _LIT(KNonExistingDb, "file://pellefant.ldb");
    aDbUris->AppendL(KNonExistingDb);
    multiSearcher->SetDatabasesToSearchL(*aDbUris);
    VerifyDbsToSearchL(multiSearcher, aDbUris);
    
    TBool searchRefined = ETrue;
    SearchForLandmarksL(multiSearcher, EAsc, !searchRefined);
    
    // Verify Result - one error from the non-existing should have been returned. 
    TUint searchErrors = VerifyResultL(multiSearcher, *aDbUris, EAsc);
    AssertTrueSecL(searchErrors == 1, _L("Found %d search errors"), searchErrors);
    CPosLmMultiDbSearch::TSearchError searchErr;
    multiSearcher->GetSearchError(0, searchErr);
    AssertTrueSecL(searchErr.iErrorCode == KErrNotFound, _L("Expected KErrNotFound but got %d"), searchErr.iErrorCode);
    TBuf<64> uriWithError = (*aDbUris)[searchErr.iDatabaseIndex];
    uriWithError.LowerCase();
    AssertTrueSecL(uriWithError == KNonExistingDb, _L("Error from wrong database"));
    for (TInt i = 0; i < aDbUris->Count(); i++)
        {
        if (i != searchErr.iDatabaseIndex)
            {
            AssertTrueSecL(multiSearcher->NumOfMatches(i) > 0, _L("Found no matches"));
            }
        }
        
    CleanupStack::PopAndDestroy(multiSearcher);
    }
    
// ---------------------------------------------------------
// CPosTp119::SearchInvalidURIDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp119::SearchInvalidURIDbL(CDesCArray* aDbUris)
    {
    iLog->Log(_L("SearchInvalidURIDbL"));
    CPosLmMultiDbSearch* multiSearcher = CPosLmMultiDbSearch::NewL(*aDbUris);
    CleanupStack::PushL(multiSearcher);    
    VerifyDbsToSearchL(multiSearcher, aDbUris);
  
    _LIT(KNonExistingDb, "file://pellefant.ldb");
    aDbUris->AppendL(KNonExistingDb);
    _LIT(KRemoteURI, "http://kallekanin.ldb");
    aDbUris->AppendL(KRemoteURI);
    _LIT(KInvalidDrive, "file://r:nonexistingdrive.ldb");
    aDbUris->AppendL(KInvalidDrive);
    
    multiSearcher->SetDatabasesToSearchL(*aDbUris);
    VerifyDbsToSearchL(multiSearcher, aDbUris);
    
    TBool searchRefined = ETrue;
    SearchForLandmarksL(multiSearcher, EAsc, !searchRefined);
    
    // Verify Result - three errors should be returned
    iLog->Log(_L("Verify Result - three errors should be returned"));
    TUint searchErrors = VerifyResultL(multiSearcher, *aDbUris, EAsc);
    AssertTrueSecL(searchErrors == 3, _L("Found %d search errors"), searchErrors);
    CPosLmMultiDbSearch::TSearchError searchErr;
    TInt id1;
    TInt id2;
    TInt id3;
    
    multiSearcher->GetSearchError(0, searchErr);
    AssertTrueSecL(searchErr.iErrorCode == KErrNotFound, _L("Expected KErrNotFound but got %d"), searchErr.iErrorCode);
    
    TBuf<64> uriWithError = (*aDbUris)[searchErr.iDatabaseIndex];
    uriWithError.LowerCase();
    AssertTrueSecL(uriWithError == KNonExistingDb, _L("Error from wrong database1"));
    id1 = searchErr.iDatabaseIndex;
    
    multiSearcher->GetSearchError(1, searchErr);
    AssertTrueSecL(searchErr.iErrorCode == KErrNotSupported, _L("Expected KErrNotSupported but got %d"), searchErr.iErrorCode);
    uriWithError.Zero();
    uriWithError = (*aDbUris)[searchErr.iDatabaseIndex];
    uriWithError.LowerCase();
    AssertTrueSecL(uriWithError == KRemoteURI, _L("Error from wrong database2"));
    id2 = searchErr.iDatabaseIndex;
    
    // KErrNotReady from dbs on non existing drives
    multiSearcher->GetSearchError(2, searchErr);
    AssertTrueSecL(searchErr.iErrorCode == KErrNotReady, _L("Expected KErrNotReady but got %d"), searchErr.iErrorCode);
    uriWithError.Zero();
    uriWithError = (*aDbUris)[searchErr.iDatabaseIndex];
    uriWithError.LowerCase();
    AssertTrueSecL(uriWithError == KInvalidDrive, _L("Error from wrong database3"));
    id3 = searchErr.iDatabaseIndex;
    
    for (TInt i=0; i < aDbUris->Count(); i++)
        {
        if (i != id1 && i != id2 && i != id3)
            {
            AssertTrueSecL(multiSearcher->NumOfMatches(i) > 0, _L("Found no matches"));
            }
        }
        
    CleanupStack::PopAndDestroy(multiSearcher);
    }
    
// ---------------------------------------------------------
// CPosTp119::SearchDbMultipleTimesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp119::SearchDbMultipleTimesL(CDesCArray* aDbUris)
    {    
    // Remove all dbs but the first
    aDbUris->Delete(1, aDbUris->Count() - 1);
    
    // 13. Repeat the same database multiple times (3 in this case)
    aDbUris->AppendL((*aDbUris)[0]);
    aDbUris->AppendL((*aDbUris)[0]);
    
    CPosLmMultiDbSearch* multiSearcher = CPosLmMultiDbSearch::NewL(*aDbUris);
    CleanupStack::PushL(multiSearcher);    
    VerifyDbsToSearchL(multiSearcher, aDbUris);
  
    TBool searchRefined = ETrue;
    SearchForLandmarksL(multiSearcher, EAsc, !searchRefined);
    
    // Verify Result - doubles should have been found 
    TUint searchErrors = VerifyResultL(multiSearcher, *aDbUris, EAsc);
    AssertTrueSecL(searchErrors == 0, _L("Found %d search errors"), searchErrors);
    for (TInt i = 0; i < aDbUris->Count(); i++)
        {
        AssertTrueSecL(multiSearcher->NumOfMatches(i) > 0, _L("Found no matches"));
        }
    for (TInt i = 0; i < aDbUris->Count(); i++)
        {
        // Log landmark names to identify doublets
        CPosLmItemIterator* iter = multiSearcher->MatchIteratorL(i);
        if (iter)
            {
            CleanupStack::PushL(iter);
            _LIT(KFormat, "Printing landmark names from db %S");
            TBuf<100> buf;
            TPtrC dbName = (*aDbUris)[i];
            buf.Format(KFormat, &dbName);
            iLog->Log(buf);
            TPosLmItemId id = iter->NextL();
            CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL((*aDbUris)[i]);
            CleanupStack::PushL(db);
            while (id != KPosLmNullItemId)
                {
                CPosLandmark* lm = db->ReadLandmarkLC(id);
                TPtrC name;
                lm->GetLandmarkName(name);
                iLog->Log(name);
                CleanupStack::PopAndDestroy(lm);
                id = iter->NextL();
                }
            CleanupStack::PopAndDestroy(2, iter);
            }
        }
        
    CleanupStack::PopAndDestroy(multiSearcher);
    } 
       
// ---------------------------------------------------------
// CPosTp119::TestMaxNrOfMatchesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp119::TestMaxNrOfMatchesL(CDesCArray* aDbUris)
    {
    CPosLmMultiDbSearch* multiSearcher = CPosLmMultiDbSearch::NewL(*aDbUris);
    CleanupStack::PushL(multiSearcher);
    TInt nrOfDbs = aDbUris->Count();
    TInt maxNrOfMatches = 4;
    TBool searchRefined = ETrue;
    TInt fetchedMaxNrOfMatches = multiSearcher->MaxNumOfMatches();
    AssertTrueSecL(fetchedMaxNrOfMatches == KPosLmMaxNumOfMatchesUnlimited, _L("Not unlimited number of matches by default."));
    multiSearcher->SetMaxNumOfMatches(maxNrOfMatches);
    fetchedMaxNrOfMatches = multiSearcher->MaxNumOfMatches();
    AssertTrueSecL(fetchedMaxNrOfMatches == maxNrOfMatches, _L("Unexpected maxNrOfMatches"));
    
    // Test normal flow
    SearchForLandmarksL(multiSearcher, ENoSort, !searchRefined);
    TUint searchErrors = VerifyResultL(multiSearcher, *aDbUris, ENoSort);
    AssertTrueSecL(searchErrors == 0, _L("Found %d search errors"), searchErrors);
    for (TInt i = 0; i < nrOfDbs; i++)
        {
        CPosLmItemIterator* iter = multiSearcher->MatchIteratorL(i);
        CleanupStack::PushL(iter);
        AssertTrueSecL(iter->NumOfItemsL() == maxNrOfMatches, _L("Wrong number of matches"));
        CleanupStack::PopAndDestroy(iter);
        }
    AssertTrueSecL(multiSearcher->TotalNumOfMatches() == nrOfDbs * maxNrOfMatches, _L("Wrong number of total matches"));
    
    // Test resetting of max nr of matches during search - should not affect ongoing search
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    textCriteria->SetTextL(_L("*e*"));
    textCriteria->SetAttributesToSearch(CPosLandmark::ELandmarkName);
    CPosLmOperation* operation = multiSearcher->StartLandmarkSearchL(*textCriteria, searchRefined);        
    CleanupStack::PushL(operation);
    
    // Update maxNrOfMatches twice
    TInt oldMaxNrOfMatches = maxNrOfMatches;
    maxNrOfMatches--;
    multiSearcher->SetMaxNumOfMatches(maxNrOfMatches);
    maxNrOfMatches--;
    multiSearcher->SetMaxNumOfMatches(maxNrOfMatches);
    fetchedMaxNrOfMatches = multiSearcher->MaxNumOfMatches();
    AssertTrueSecL(fetchedMaxNrOfMatches == maxNrOfMatches, _L("Unexpected maxNrOfMatches"));
    
    operation->ExecuteL();
    CleanupStack::PopAndDestroy(operation);
    CleanupStack::PopAndDestroy(textCriteria);
    
    searchErrors = VerifyResultL(multiSearcher, *aDbUris, ENoSort);
    AssertTrueSecL(searchErrors == 0, _L("Found %d search errors"), searchErrors);
    for (TInt i = 0; i < nrOfDbs; i++)
        {
        CPosLmItemIterator* iter = multiSearcher->MatchIteratorL(i);
        CleanupStack::PushL(iter);
        AssertTrueSecL(iter->NumOfItemsL() == oldMaxNrOfMatches, _L("Wrong number of matches"));
        CleanupStack::PopAndDestroy(iter);
        }
    AssertTrueSecL(multiSearcher->TotalNumOfMatches() == nrOfDbs * oldMaxNrOfMatches, _L("Wrong number of total matches"));
    
    // Test new search - the new maxNrOfMatches should have effect
    SearchForLandmarksL(multiSearcher, ENoSort, !searchRefined);
    searchErrors = VerifyResultL(multiSearcher, *aDbUris, ENoSort);
    AssertTrueSecL(searchErrors == 0, _L("Found %d search errors"), searchErrors);
    fetchedMaxNrOfMatches = multiSearcher->MaxNumOfMatches();
    AssertTrueSecL(fetchedMaxNrOfMatches == maxNrOfMatches, _L("Unexpected maxNrOfMatches"));
    for (TInt i = 0; i < nrOfDbs; i++)
        {
        CPosLmItemIterator* iter = multiSearcher->MatchIteratorL(i);
        CleanupStack::PushL(iter);
        AssertTrueSecL(iter->NumOfItemsL() == maxNrOfMatches, _L("Wrong number of matches"));
        CleanupStack::PopAndDestroy(iter);
        }
    AssertTrueSecL(multiSearcher->TotalNumOfMatches() == nrOfDbs * maxNrOfMatches, _L("Wrong number of total matches"));
        
    CleanupStack::PopAndDestroy(multiSearcher);
    }

// ---------------------------------------------------------
// CPosTp119::TestMatchIteratorL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp119::TestMatchIteratorL(CDesCArray* /*aDbUris*/)
    {
    // Moved to TP130
    }
    
// ---------------------------------------------------------
// CPosTp119::SearchForLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp119::SearchForLandmarksL(
    CPosLmMultiDbSearch* aSearcher, 
    TTp119SortOrder aSortOrder,
    TBool aRefined)
    {
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    textCriteria->SetTextL(_L("*e*"));
    textCriteria->SetAttributesToSearch(CPosLandmark::ELandmarkName);
    
    CPosLmOperation* operation = NULL;
    if (aSortOrder == ENoSort)
        {
        operation = aSearcher->StartLandmarkSearchL(*textCriteria, aRefined);
        }
    else
        {
        TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
        if (aSortOrder == EDesc)
            {
            sortPref = TPosLmSortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
            }
        operation = aSearcher->StartLandmarkSearchL(*textCriteria, sortPref, aRefined);
        }
        
    CleanupStack::PushL(operation);
    operation->ExecuteL();
    CleanupStack::PopAndDestroy(operation);
    CleanupStack::PopAndDestroy(textCriteria);
    }
    
// ---------------------------------------------------------
// CPosTp119::VerifyResultL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TUint CPosTp119::VerifyResultL(
    CPosLmMultiDbSearch* aSearcher, 
    CDesCArray& aDbUris,
    TTp119SortOrder aSortOrder)
    {
    TUint searchErrors = aSearcher->NumOfSearchErrors();
    for (TUint j = 0; j < searchErrors; j++)
        {
        CPosLmMultiDbSearch::TSearchError searchErr;
        aSearcher->GetSearchError(j, searchErr);
        TBuf<100> buf;
        _LIT(KFormat, "Got error code %d from database nr%d : %S");
        TPtrC uri = aDbUris[searchErr.iDatabaseIndex];
        buf.Format(KFormat, searchErr.iErrorCode,searchErr.iDatabaseIndex, &uri);
        iLog->Log(buf);
        }
        
    // Verify sort order
    if (aSortOrder != ENoSort)
        {        
        for (TInt i = 0; i < aDbUris.Count(); i++)
            {
            CPosLmItemIterator* iterator = aSearcher->MatchIteratorL(i);
            CleanupStack::PushL(iterator);
            if (iterator && iterator->NumOfItemsL() > 0)
                {
                CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL(aDbUris[i]);
                CleanupStack::PushL(db);
                TPosLmItemId currentId = iterator->NextL();
                while (currentId != KPosLmNullItemId)
                    {
                    TPosLmItemId nextId = iterator->NextL();
                    if (nextId != KPosLmNullItemId)
                        {
                        CPosLandmark* currentLM = db->ReadLandmarkLC(currentId);
                        CPosLandmark* nextLM = db->ReadLandmarkLC(nextId);
                        TPtrC currentName, nextName;
                        currentLM->GetLandmarkName(currentName);
                        nextLM->GetLandmarkName(nextName);
                        TInt order = nextName.CompareC(currentName);
                        if (aSortOrder == EAsc)
                            {                        
                            AssertTrueSecL(order >= 0, _L("Wrong sort order (not ascending)"));
                            }
                        else // aSortOrder == EDesc
                            {
                            AssertTrueSecL(order <= 0, _L("Wrong sort order (not descending)"));
                            }
                        CleanupStack::PopAndDestroy(2, currentLM);
                        }
                    currentId = nextId;
                    }
                CleanupStack::PopAndDestroy(db);
                }
            CleanupStack::PopAndDestroy(iterator);
            }
        }
        
    return searchErrors;
    }
    
// ---------------------------------------------------------
// CPosTp119::RemoveDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp119::RemoveDatabaseL(const TDesC& aFile)
    {    
    RFs fileServer;
    User::LeaveIfError(fileServer.Connect());
    CleanupClosePushL(fileServer);
    CFileMan* fileMan = CFileMan::NewL(fileServer);
    CleanupStack::PushL(fileMan);
    User::LeaveIfError(fileMan->Delete(aFile));
    CleanupStack::PopAndDestroy(2, &fileServer);
    }
    
// ---------------------------------------------------------
// CPosTp119::VerifyDbsToSearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp119::VerifyDbsToSearchL(
    CPosLmMultiDbSearch* aMultiSearcher, 
    CDesCArray* aDbUris)
    {
    CDesCArray* dbToSearch = aMultiSearcher->DatabasesToSearchL();
    CleanupStack::PushL(dbToSearch);
    
    // Verify that lengths of specified array and retrived array match
    AssertTrueSecL(aDbUris->Count() == dbToSearch->Count(), _L("aDbUris->Count() != dbToSearch->Count()"));
    
    // Verify that length of specified array equals retrived length
    AssertTrueSecL(aDbUris->Count() == aMultiSearcher->NumOfDatabasesToSearch(), _L("aDbUris->Count() != aMultiSearcher->NumOfDatabasesToSearch()"));
    
    for (TInt i = 0; i < aDbUris->Count(); i++)
        {
        TInt pos, pos2;
        
        // Verify that specified db can be found in retrieved list.
        TInt err = dbToSearch->Find((*aDbUris)[i], pos);
        
        // Verify that retrieved db URI can be found in specified list
        TInt err2 = aDbUris->Find(aMultiSearcher->DatabaseUriPtr(i), pos2);
        
        // Error handling
        if (err != KErrNone || err2 != KErrNone)
            {
            // No match found
            iLog->Log(_L("Didn't find expected db in db list"));
            User::Leave(err);
            }
        else
            {
            dbToSearch->Delete(pos);
            }
        }
        
    // Verify that no more db exist in search list
    AssertTrueSecL(dbToSearch->Count() == 0, _L("More dbs to search than expected"));
    
    CleanupStack::PopAndDestroy(dbToSearch);
    }
    
//  End of File
