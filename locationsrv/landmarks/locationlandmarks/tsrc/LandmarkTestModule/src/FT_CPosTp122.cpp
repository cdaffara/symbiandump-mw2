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
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryCriteria.h>
#include <EPos_CPosLMItemIterator.h>
#include <EPos_CPosLmMultiDbSearch.h>
#include <EPos_CPosLmDatabaseManager.h>
#include "FT_CPosTp122.h"
#include "FT_LandmarkConstants.h"
#include <EPos_CPosLmDisplayData.h>
#include <EPos_CPosLmDisplayItem.h>

#include <e32math.h>

// CONSTANTS
const TInt KMaxFieldLength = 80;
const TInt KDistanceFieldLength = 24;
const TInt KMaxNearestLineLength = 1024;

//const TInt KNrOfDatabases = 5;

_LIT(KLMDB1, "NEARESTLMDB_1.LDB");
_LIT(KLMDB2, "NEARESTLMDB_2.LDB");
_LIT(KLMDB3, "NEARESTLMDB_3.LDB");
_LIT(KLMDB4, "NEARESTLMDB_4.LDB");
_LIT(KLMDB5, "NEARESTLMDB_5.LDB");

_LIT(KNEARESTLMDB, "eposlmnearestsearch.ldb");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp122::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp122::CloseTest()
    {
    iLog->Log(_L("CloseTest"));

    delete iDatabases;

    if (iMultiDbSearch) iMultiDbSearch->UnsetDisplayData();
    delete iDisplayData;
    iDisplayData = NULL;

    delete iMultiDbSearch;
    iNearestSearchResults.ResetAndDestroy();

    TRAPD(err, RemoveAllLmDatabasesL());
    if (err != KErrNone) iLog->Log(_L("Error when RemoveAllLmDatabasesL"));
    }

// ---------------------------------------------------------
// CPosTp122::InitTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp122::InitTestL()
    {
    CLandmarkTestProcedureBase::InitTestL();

    iLog->Log(_L("InitTestL"));
    RemoveAllLmDatabasesL();

    // PrepareDatabases
    PrepareDatabasesL();

	SetupLimits();

    iNrOfDatabases = 5;

    CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    iDatabases = dbManager->ListDatabasesLC();
    CleanupStack::Pop(iDatabases);
    iDatabases->Sort(); // Not really necessary...
    AssertTrueSecL((iDatabases->Count() == iNrOfDatabases), _L("Wrong number of databases"));

    iLog->Log( _L("List sorted"));

    iMultiDbSearch = CPosLmMultiDbSearch::NewL(*iDatabases);
    CleanupStack::PopAndDestroy(dbManager);

    iLog->Log( _L("multi search"));

    // Copy db file, used by CNearestSearchResult
    CopyTestDbFileL(KNEARESTLMDB);

    iLog->Log( _L("copied"));

    // Initiallize iNearestSearchResults
    AppendNearestSearchResultsL();
    }

// ---------------------------------------------------------
// CPosTp122::SetupLimits()
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CPosTp122::SetupLimits()
	{
    iIdDb1Min = 1;
    iIdDb1Max = 50;

    iIdDb2Min = 51;
    iIdDb2Max = 100;

    iIdDb3Min = 101;
    iIdDb3Max = 150;

    iIdDb4Min = 151;
    iIdDb4Max = 200;

    iIdDb5Min = 201;
    iIdDb5Max = 254;
	}

// ---------------------------------------------------------
// CPosTp122::AppendNearestSearchResultsL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CPosTp122::AppendNearestSearchResultsL()
    {
    iNearestSearchResults.ResetAndDestroy();

     _LIT(KFileTestValues, "c:\\system\\test\\testdata\\lmDbNearestSearchResult.txt");

    RFile file;
    CleanupClosePushL(file);
    User::LeaveIfError(file.Open(iFileSession, KFileTestValues,  EFileRead));

    TInt row=0;
    TBuf<KMaxNearestLineLength> line;
    while (ReadLn(file, line) != KErrEof)
        {
        TLex lex(line);
        if (lex.Peek() != '#' && lex.Peek() != '\t')
            {
            ++row;

            //iLog->Log(_L("Nearest search result in progress"));
            CNearestSearchResult* searchResult = CNearestSearchResult::NewL(line, row, iFileSession);
            CleanupStack::PushL(searchResult);

            User::LeaveIfError(iNearestSearchResults.Append(searchResult));
            CleanupStack::Pop(searchResult);
            //iLog->Log(_L("Nearest search result added"));
            }
        }
    CleanupStack::PopAndDestroy(&file);
    }

// ---------------------------------------------------------
// CPosTp122::PrepareDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp122::PrepareDatabasesL()
	{
	iLog->Log(_L("PrepareDatabasesL"));
	// Copy eposlmnearestsearch.ldb into five dbs
    _LIT(KDbFile, "c:\\system\\test\\testdata\\eposlmnearestsearch.ldb");

    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);

    TBuf<100> path;
    path.Zero();
    path.Append(KLmDbPath);
    path.Append(KLMDB1);
    User::LeaveIfError(fileMan->Copy(KDbFile, path, CFileMan::EOverWrite));

    path.Zero();
    path.Append(KLmDbPath);
    path.Append(KLMDB2);
    User::LeaveIfError(fileMan->Copy(KDbFile, path, CFileMan::EOverWrite));

    path.Zero();
    path.Append(KLmDbPath);
    path.Append(KLMDB3);
    User::LeaveIfError(fileMan->Copy(KDbFile, path, CFileMan::EOverWrite));

    path.Zero();
    path.Append(KLmDbPath);
    path.Append(KLMDB4);
    User::LeaveIfError(fileMan->Copy(KDbFile, path, CFileMan::EOverWrite));

    path.Zero();
    path.Append(KLmDbPath);
    path.Append(KLMDB5);
    User::LeaveIfError(fileMan->Copy(KDbFile, path, CFileMan::EOverWrite));

    CleanupStack::PopAndDestroy(fileMan);

    // Now adjust dbs according to following:
    // First db-> Keep landmarks 1-50
    // Second db-> Keep landmarks 51-100
    // Third db-> Keep landmarks 101-150
    // Fourth db-> Keep landmarks 151-200
    // Fifth db-> Keep landmarks 201-254

    RArray<TPosLmItemId> array;
    CleanupClosePushL(array);

    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL(KLMDB1);
    CleanupStack::PushL(lmd);

    TBool debug = EFalse;
    TInt index=0;
    TInt max=254;
   	for (index=50;index<max;index++) array.Append(index+1);
    if (lmd->IsInitializingNeeded()) ExecuteAndDeleteLD(lmd->InitializeL());
    ExecuteAndDeleteLD(lmd->RemoveLandmarksL(array));
    array.Reset();
    if (debug) PrintLm(lmd);
    CleanupStack::PopAndDestroy(lmd);

	lmd = CPosLandmarkDatabase::OpenL(KLMDB2);
    CleanupStack::PushL(lmd);

   	for (index=0;index<50;index++) array.Append(index+1);
   	for (index=100;index<max;index++) array.Append(index+1);
    if (lmd->IsInitializingNeeded()) ExecuteAndDeleteLD(lmd->InitializeL());
    ExecuteAndDeleteLD(lmd->RemoveLandmarksL(array));
    array.Reset();
	if (debug) PrintLm(lmd);
    CleanupStack::PopAndDestroy(lmd);

	lmd = CPosLandmarkDatabase::OpenL(KLMDB3);
    CleanupStack::PushL(lmd);

   	for (index=0;index<100;index++) array.Append(index+1);
   	for (index=150;index<max;index++) array.Append(index+1);
    if (lmd->IsInitializingNeeded()) ExecuteAndDeleteLD(lmd->InitializeL());
    ExecuteAndDeleteLD(lmd->RemoveLandmarksL(array));
    array.Reset();
	if (debug) PrintLm(lmd);
    CleanupStack::PopAndDestroy(lmd);

	lmd = CPosLandmarkDatabase::OpenL(KLMDB4);
    CleanupStack::PushL(lmd);

   	for (index=0;index<150;index++) array.Append(index+1);
   	for (index=200;index<max;index++) array.Append(index+1);
    if (lmd->IsInitializingNeeded()) ExecuteAndDeleteLD(lmd->InitializeL());
    ExecuteAndDeleteLD(lmd->RemoveLandmarksL(array));
    array.Reset();
	if (debug) PrintLm(lmd);
    CleanupStack::PopAndDestroy(lmd);

	lmd = CPosLandmarkDatabase::OpenL(KLMDB5);
    CleanupStack::PushL(lmd);

	for (index=0;index<200;index++) array.Append(index+1);
    if (lmd->IsInitializingNeeded()) ExecuteAndDeleteLD(lmd->InitializeL());
    ExecuteAndDeleteLD(lmd->RemoveLandmarksL(array));
    array.Reset();
	if (debug) PrintLm(lmd);

    CleanupStack::PopAndDestroy(lmd);

    CleanupStack::PopAndDestroy(&array);
    iLog->Log(_L("PrepareDatabasesL Done"));
	}

// ---------------------------------------------------------
// CPosTp122::PrintLm
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp122::PrintLm(CPosLandmarkDatabase* db)
	{
	CPosLmItemIterator* iter = db->LandmarkIteratorL();
	CleanupStack::PushL(iter);
	iLog->Log(_L("PrintLm"));
	//for (TInt i=0;i< iter->Count();i++)
	TPosLmItemId id = iter->NextL();
	TBuf<100> buf;

	while (id != KPosLmNullItemId)
		{
		CPosLandmark* lm = db->ReadLandmarkLC(id);
		TPtrC name;
		lm->GetLandmarkName(name);
		iLog->Log(_L("Found LM '%S' "), &name);
		TInt idde = lm->LandmarkId();

		id = iter->NextL();
		CleanupStack::PopAndDestroy(lm);
		}

	CleanupStack::PopAndDestroy(iter);
	iLog->Log(_L("PrintLm Done"));
	}

// ---------------------------------------------------------
// CPosTp122::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp122::StartL()
    {
	TInt KNrOfSearch = 0;
	iErrorsFound= KErrNone;

	iDisplayData = CPosLmDisplayData::NewL();
	iMultiDbSearch->SetDisplayData(*iDisplayData);

    iLog->Log(_L(">>>>>Search Synchronous with ascending sort order"));
    StartSearchOperationL(iNearestSearchResults, ESynchronous, ESortAscending, KNrOfSearch);

    iLog->Log(_L(">>>>>Search Synchronous with descending sort order"));
    StartSearchOperationL(iNearestSearchResults, ESynchronous, ESortDescending, KNrOfSearch);

    iLog->Log(_L(">>>>>Search Asynchronous with ascending sort order"));
    StartSearchOperationL(iNearestSearchResults, EAsynchronous, ESortAscending, KNrOfSearch);

    iLog->Log(_L(">>>>>Search Asynchronous with descending sort order"));
    StartSearchOperationL(iNearestSearchResults, EAsynchronous, ESortDescending, KNrOfSearch);

    iLog->Log(_L(">>>>>Search Synchronous with no sort order (therefore sorted by distance)"));
    StartSearchOperationL(iNearestSearchResults, ESynchronous, ESortNone);

	iLog->Log(_L(">>>>>Search Asynchronous with no sort order (therefore sorted by distance)"));
    StartSearchOperationL(iNearestSearchResults, EAsynchronous, ESortNone);

    iLog->Log(_L(">>>>>Search Synchronous with no sort order, partial (therefore sorted by distance)"));
    iPartial = ETrue;
    StartSearchOperationL(iNearestSearchResults, ESynchronous, ESortNone);
    iPartial = EFalse;
    iDisplayData->UnsetPartialReadParameters();

    iLog->Log(_L("Test with maximum searches"));
    TestMaximumNrOfMatchesL(iNearestSearchResults, EAsynchronous, ESortNone);
    TestMaximumNrOfMatchesL(iNearestSearchResults, ESynchronous, ESortNone);
    TestMaximumNrOfMatchesL(iNearestSearchResults, EAsynchronous, ESortDescending);
    TestMaximumNrOfMatchesL(iNearestSearchResults, EAsynchronous, ESortAscending);

    iLog->Log(_L("TestOnTheFlySearchL"));
    TestOnTheFlySearchL(iNearestSearchResults);

    // Since TestOnTheFlySearchL is changing displadata
    iMultiDbSearch->UnsetDisplayData();
    delete iDisplayData;
    iDisplayData = NULL;
    iDisplayData = CPosLmDisplayData::NewL();
	iMultiDbSearch->SetDisplayData(*iDisplayData);

    iLog->Log(_L("Test Leave codes for nearest search"));
    TestLeaveCodesL();

    // TC Step 6 "A new array of databases is set to be searched"
    iLog->Log(_L("Test refined search, test different search dbs"));
    DoLastTestPartL(iNearestSearchResults);

    iLog->Log(_L("Testing cancel immediately"));
    CancelTestL(iNearestSearchResults, EFalse);

	// XXX Hangs client
    iLog->Log(_L("Testing cancel in callback"));
    CancelTestL(iNearestSearchResults, ETrue);

    if (iErrorsFound != KErrNone)
    	{

    	iLog->Log(_L("Errors found in TP122"));
    	User::Leave(-1);
    	}
    else iLog->Log(_L("TP122 successfully completed"));
    }

// ---------------------------------------------------------
// CPosTp122::StartSearchOperationL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp122::StartSearchOperationL(
    const RPointerArray<CNearestSearchResult>& aSearchResults,
    TExecutionMode aExecutionMode,
    TTp122SortOrder aSortOrder,
    TInt aNrOfSearches)
    {

    // Test SetMaxNumOfMatches, first set to one and then back to unlimited
    iMultiDbSearch->SetMaxNumOfMatches(1);
    iMultiDbSearch->SetMaxNumOfMatches();

    TBuf<100> buf;
    TInt maxSearches = 0;
    if (aNrOfSearches == 0) maxSearches = aSearchResults.Count();
    else maxSearches = aNrOfSearches;

    for(TInt i=0; i<maxSearches; i++)
        {
        TCoordinate coord(aSearchResults[i]->iLatitude, aSearchResults[i]->iLongitude);
        CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(coord, aSearchResults[i]->iUseCoverageRadius);
        nearestCriteria->SetMaxDistance(aSearchResults[i]->iMaxDistance);
		buf.Format(_L("StartSearchOperationL: Searching for Lat %f Long %f"), aSearchResults[i]->iLatitude, aSearchResults[i]->iLongitude);
		//iLog->Log(buf);

        if (aSearchResults[i]->iMaxSearchResult > 0)
            {
            // Does not work anymore
            //iMultiDbSearch->SetMaxNumOfMatches(aSearchResults[i]->iMaxSearchResult);
            }
        else
            {
            iMultiDbSearch->SetMaxNumOfMatches(KPosLmMaxNumOfMatchesUnlimited);
            }

        if (iNearestSearchResults[i]->Redefined() && i > 0)
            {
            // Remove distances that are not present in the previous

            for (TInt j=0; j<iNearestSearchResults[i]->iDistances.Count(); j++)
                {
                TBool found = EFalse;
                TInt id = 0;
                TPosLmItemId idde;

                id = j;
                idde = iNearestSearchResults[i]->iDistances[j].Id();

                TBuf<100> debug;
                debug.Format(_L("Looking for id %d or id: %d "), idde, id);
                //iLog->Log(debug);

                for (TInt p=0;p<iNearestSearchResults[i-1]->iDistances.Count();p++)
                    {

                    if (iNearestSearchResults[i]->iDistances[j].Id() == iNearestSearchResults[i-1]->iDistances[p].Id())
                        {
                        found = ETrue;
                        TBuf<100> buf;
                        buf.Format(_L("Found id %d or id: %d "), idde, id);
                        //iLog->Log(buf);
                        }
                    }
                if (!found)
                    {
                    // Remove since not found, only keep ids that was found in previous search
                    TBuf<100> buf;
                    buf.Format(_L("Removing lm Id %d from index %d"), idde, id);
                    //iLog->Log(buf);
                    iNearestSearchResults[i]->iDistances.Remove(id);
                    //iLog->Log(_L("Removing Done"));
                    j--;
                    }
                //iLog->Log(_L("***********"));
                }
            }

        TTime iStart;
     	TTime iStop;

     	iStart.UniversalTime();
    	SearchL(nearestCriteria, aExecutionMode, aSearchResults[i]->Redefined(), aSortOrder);
    	iStop.UniversalTime();

    	TInt64 msec = (iStop.Int64() - iStart.Int64())/1000;
		buf.Format(_L("Search op nr %d took %d msec"), i, msec);
		iLog->Log(buf);

    	AssertTrueSecL(NrOfSearchErrors(iMultiDbSearch) == 0, _L("Found search errors!"));

    	// Add check of result here
    	CheckSearchResultL(*aSearchResults[i], aSortOrder);

    	CleanupStack::PopAndDestroy(nearestCriteria);
    	iDisplayData->Reset();

        }
    }

// ---------------------------------------------------------
// CPosTp122::SearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp122::SearchL(
    CPosLmNearestCriteria* aNearestSearchCriteria,
    TExecutionMode aExecutionMode,
    TBool aRefinedSearch,
    TTp122SortOrder aSortOrder)
    {
    if (iPartial)
        {
        RArray<TUint> reqFields;
        CleanupClosePushL(reqFields);
        reqFields.Append(EPositionFieldStreet);
        reqFields.Append(EPositionFieldBuildingFloor);
        CPosLmPartialReadParameters* partialParam = CPosLmPartialReadParameters::NewLC();
        partialParam->SetRequestedPositionFields(reqFields);
        iDisplayData->SetPartialReadParametersL(*partialParam);
        CleanupStack::PopAndDestroy(2);
        }

    switch (aSortOrder)
        {
        case ESortNone:
        	{
            iOperation = iMultiDbSearch->StartLandmarkSearchL(*aNearestSearchCriteria, aRefinedSearch);
            break;
        	}
        case ESortAscending:
        	{
            TPosLmSortPref ascending(CPosLandmark::ELandmarkName);
            iOperation = iMultiDbSearch->StartLandmarkSearchL(*aNearestSearchCriteria, ascending, aRefinedSearch);
            break;
        	}
        case ESortDescending:
        	{
            TPosLmSortPref descending(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
            iOperation = iMultiDbSearch->StartLandmarkSearchL(*aNearestSearchCriteria, descending, aRefinedSearch);
            break;
        	}
        default:
            User::Leave(KErrArgument);
            break;
        }

    switch (aExecutionMode)
        {
        case ESynchronous:
            ExecuteAndDeleteLD(iOperation);
            break;
        case EAsynchronous:
            RunAsyncOperationLD(iOperation);
            break;
        default:
            User::Leave(KErrArgument);
            break;
        }
    }

// ---------------------------------------------------------
// CPosTp122::TestMaximumNrOfMatchesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp122::TestMaximumNrOfMatchesL(
    const RPointerArray<CNearestSearchResult>& aSearchResults,
    TExecutionMode aExecutionMode, TTp122SortOrder aSortOrder)
    {
    iLog->Log(_L("TestMaximumNrOfMatchesL"));
    for (TInt j=0;j<aSearchResults.Count();j++)
    	{
	    TInt KIndex = j;
		TCoordinate coord(aSearchResults[KIndex]->iLatitude, aSearchResults[KIndex]->iLongitude);
	    CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(coord, aSearchResults[KIndex]->iUseCoverageRadius);
	    nearestCriteria->SetMaxDistance(aSearchResults[KIndex]->iMaxDistance);

	    for (TInt i=1;i<10;i++)
		    {
		    // Test with different maximum nr of matches
		    iMultiDbSearch->SetMaxNumOfMatches(i*2);

			TBuf<100> buf;
			SearchL(nearestCriteria, aExecutionMode, aSearchResults[KIndex]->Redefined(), aSortOrder);


			TInt nrOfMatches = 0;
			for (TInt j = 0; j < iNrOfDatabases; j++)
			    {
			    CPosLmItemIterator* iter = iMultiDbSearch->MatchIteratorL(j);
			    CleanupStack::PushL(iter);
			    nrOfMatches += iter->NumOfItemsL();
			    CleanupStack::PopAndDestroy(iter);
			    }

			AssertTrueSecL(nrOfMatches <= i*2 * iNrOfDatabases, _L("Total nr of matches are bigger than the set value of maximum nr of matches"));
			AssertTrueSecL(nrOfMatches == iMultiDbSearch->TotalNumOfMatches(), _L("Total nr of matches doesn't match sum of matches in iterators"));
			AssertTrueSecL(nrOfMatches == iDisplayData->Count(), _L("Total nr of matches doesn't match sum of matches in displaydata"));
			}

		CleanupStack::PopAndDestroy(nearestCriteria);
		}

    }

// ---------------------------------------------------------
// CPosTp122::TestLeaveCodesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp122::TestLeaveCodesL()
    {
    iLog->Log(_L("TestLeaveCodesL"));

    TPosLmSortPref sortPref(CPosLandmark::ECategoryInfo, TPosLmSortPref::EAscending);
	TCoordinate coord(25, 45);
	CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(coord);

	// Only ECategoryName is supported,all other leave with KErrNotSupported
    sortPref.SetSortByLandmarkAttribute(CPosLandmark::EDescription, TPosLmSortPref::EDescending);
    TRAPD(err, iMultiDbSearch->StartLandmarkSearchL(*nearestCriteria, sortPref));
    AssertTrueSecL(err == KErrNotSupported, _L("Should have left with KErrNotSupported"));

    CleanupStack::PopAndDestroy(nearestCriteria);
    }

// ---------------------------------------------------------
// CPosTp122::CancelTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp122::CancelTestL(const RPointerArray<CNearestSearchResult>& aSearchResults, TBool aCancelInCallback)
    {
    //Test search with one criteria
    const TInt KIndex=0;

    TCoordinate coord(aSearchResults[KIndex]->iLatitude, aSearchResults[KIndex]->iLongitude);
    CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(coord, aSearchResults[KIndex]->iUseCoverageRadius);
    nearestCriteria->SetMaxDistance(aSearchResults[KIndex]->iMaxDistance);

	// Cancel in callback, cancel in every progress step phase
	// when progress has increased with 0.09
    if (aCancelInCallback)
    	{
	    TReal32 progress(0);
	    while (progress < 1.0)
	        {
	        RunAsyncOperationAndCancelInCallbackLD(
	            iMultiDbSearch->StartLandmarkSearchL(
	                *nearestCriteria,
	                aSearchResults[KIndex]->Redefined()),
	            progress);

	        TInt nrOfMatches = 0;
	        for (TInt j = 0; j < iNrOfDatabases; j++)
	            {
	            CPosLmItemIterator* iter = iMultiDbSearch->MatchIteratorL(j);
	            CleanupStack::PushL(iter);
	            nrOfMatches += iter->NumOfItemsL();
	            CleanupStack::PopAndDestroy(iter);
	            }
	        AssertTrueSecL(nrOfMatches == iMultiDbSearch->TotalNumOfMatches(), _L("Total nr of matches doesn't match sum of matches in iterators when cancelling"));
	        //AssertTrueSecL(iMultiDbSearch->TotalNumOfMatches() == iDisplayData->Count(),_L("Wrong number of matches after cancel"));

	        progress = progress + 0.09;
	        }
    	}
    else
    	{
    	// Cancel immediately
    	RunAsyncOperationAndCancelLD(
	            iMultiDbSearch->StartLandmarkSearchL(
	                *nearestCriteria,
	                aSearchResults[KIndex]->Redefined()));

		AssertTrueSecL(iMultiDbSearch->TotalNumOfMatches() == iDisplayData->Count(),_L("Wrong number of matches after cancel"));
    	}

    	if (iMultiDbSearch->NumOfSearchErrors() != 0) iLog->Log(_L("Errors after search"));
		for (TInt i=0;i<iMultiDbSearch->NumOfSearchErrors(); i++)
			{
    		CPosLmMultiDbSearch::TSearchError searchErr;
    		iMultiDbSearch->GetSearchError(i, searchErr);
    		TBuf<150> buf;
    		buf.Format(_L("ErrorCode %d returned from db nr %d"),searchErr.iErrorCode, searchErr.iDatabaseIndex);
    		iLog->Log(buf);
    		}

    CleanupStack::PopAndDestroy(nearestCriteria);
    }

// ---------------------------------------------------------
// CPosTp122::DoLastTestPartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp122::DoLastTestPartL(const RPointerArray<CNearestSearchResult>& aSearchResults)
	{
	TInt KIndex = 0;
	TCoordinate coord(aSearchResults[KIndex]->iLatitude, aSearchResults[KIndex]->iLongitude);
    CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(coord, aSearchResults[KIndex]->iUseCoverageRadius);
    nearestCriteria->SetMaxDistance(aSearchResults[KIndex]->iMaxDistance);

	TBuf<100> buf;
	iLog->Log(_L("Search in all dbs"));
	SearchL(nearestCriteria, ESynchronous, aSearchResults[KIndex]->Redefined(), ESortNone);

	// Check displaydata/iterator here
	CheckDbL();

    HBufC16* db4 = (*iDatabases)[4].AllocL();
    HBufC16* db2 = (*iDatabases)[2].AllocL();

    // Remove two dbs from uri list
    iDatabases->Delete(4);
    iDatabases->Delete(2);
    iNrOfDatabases-=2;

    iMultiDbSearch->SetDatabasesToSearchL(*iDatabases);

    iIdDb3Min = 151;
    iIdDb3Max = 200;

    iLog->Log(_L("Search in all dbs but db2 and db4"));
    SearchL(nearestCriteria, ESynchronous, aSearchResults[KIndex]->Redefined(), ESortNone);
	CheckDbL();

	// Now search in all dbs but db nr 2
	buf.Zero();
	buf.Format(_L("%S"), db4);
	iDatabases->AppendL(buf);
    iNrOfDatabases++;
    iDatabases->Sort();

    iMultiDbSearch->SetDatabasesToSearchL(*iDatabases);

 	// Add a new landmark to db nr 4, nr 2 in the list
 	buf.Zero();
 	buf.Format(_L("Open Database: %S"), db4);
 	iLog->Log(buf);

	buf.Zero();
	buf.Format(_L("%S"), db4);
 	CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL(buf);
    if (db->IsInitializingNeeded())
   {
   ExecuteAndDeleteLD(db->InitializeL());
   }

    CleanupStack::PushL(db);
 	CPosLandmark* lm1 = CPosLandmark::NewLC();
	lm1->SetLandmarkNameL(_L("TP122_Landmark"));
	lm1->SetLandmarkDescriptionL(_L("Some Description"));
	TCoordinate newCoord(aSearchResults[KIndex]->iLatitude-5, aSearchResults[KIndex]->iLongitude+1);

	TLocality loc(newCoord, 0.5);
	lm1->SetPositionL(loc);
	iLog->Log(_L("Add new Landmark to db nr 4"));
    TPosLmItemId newLmId = db->AddLandmarkL(*lm1);
	CleanupStack::PopAndDestroy(lm1);
	CleanupStack::PopAndDestroy(db);

	iLog->Log(_L("Search in all dbs but db2"));
	//Refine search. new landmark should not be included
	SearchL(nearestCriteria, ESynchronous, ETrue, ESortNone);

	SetupLimits();
    iIdDb3Min = 151;
    iIdDb3Max = 200;
    iIdDb4Min = 201;
    iIdDb4Max = 254;

	CheckDbL();

	TInt itemId = iDisplayData->NewItemIndex();
	while (itemId != KPosLmNoNewItems)
		{

		CPosLmDisplayItem& item = iDisplayData->DisplayItem(itemId);
	    TInt lmId = item.Landmark().LandmarkId();
        TPtrC name;
    	item.Landmark().GetLandmarkName(name);
    	// Verify that the landmark is not found
    	if (name.CompareC(_L("TP122_Landmark")) == 0) {
    	iLog->Log(_L("Added landmarks should not be found in refined search"));
    	User::Leave(-1);

    	}
	    itemId = iDisplayData->NewItemIndex();
	    }

	// Add all dbs to the search list
	SetupLimits();

	// Add the last db to the search list
	buf.Zero();
	buf.Format(_L("%S"), db2);
	iDatabases->AppendL(buf);
    iNrOfDatabases++;
    iDatabases->Sort();

    iMultiDbSearch->SetDatabasesToSearchL(*iDatabases);
    iLog->Log(_L("Search in all dbs"));
    //Refine search. new landmark should not be included
	SearchL(nearestCriteria, ESynchronous, ETrue, ESortNone);
	CheckDbL();
	itemId = iDisplayData->NewItemIndex();
	while (itemId != KPosLmNoNewItems)
		{

		CPosLmDisplayItem& item = iDisplayData->DisplayItem(itemId);
	    TInt lmId = item.Landmark().LandmarkId();
        TPtrC name;
    	item.Landmark().GetLandmarkName(name);
    	// Verify that the landmark is not found
    	if (name.CompareC(_L("TP122_Landmark")) == 0) {

    	iLog->Log(_L("Added landmarks should not be found in refined search"));
    	User::Leave(-1);

    	}

	    itemId = iDisplayData->NewItemIndex();
	    }

    CleanupStack::PopAndDestroy(nearestCriteria);

    // Correct limits if more testing after this part
    SetupLimits();
    delete db4;
    delete db2;
	}

// ---------------------------------------------------------
// CPosTp122::CheckDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp122::CheckDbL()
	{
	iLog->Log(_L("CheckDbL"));
	CDesCArray* dbToSearch = iMultiDbSearch->DatabasesToSearchL();
    CleanupStack::PushL(dbToSearch);
    TBuf<100> buf;

    for (TInt i=0;i<dbToSearch->Count();i++)
		{
		buf.Zero();
		buf.Format(_L("DB nr %d "), i);

		buf.Append((*dbToSearch)[i]);
		iLog->Log(buf);
		}

    if (iMultiDbSearch->NumOfDatabasesToSearch() != dbToSearch->Count()) iLog->Log(_L("Differ..."));

    CleanupStack::PopAndDestroy(dbToSearch);

	 for (TInt j = 0; j <iMultiDbSearch->NumOfDatabasesToSearch(); j++)
		{
		CPosLmItemIterator* iter = iMultiDbSearch->MatchIteratorL(j);
    	CleanupStack::PushL(iter);

    	TPosLmItemId id = iter->NextL();
        while (id != KPosLmNullItemId)
            {
            ValidateIdL(id, j);
            id = iter->NextL();
            }
    	//GetSearchError
    	//NumOfSearchErrors
    	CleanupStack::PopAndDestroy(iter);
		}
		iLog->Log(_L("CheckDbL Done"));
	}

// ---------------------------------------------------------
// CPosTp122::CheckSearchResultL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp122::CheckSearchResultL(CNearestSearchResult& aSearchResult, TTp122SortOrder aSortOrder)
    {
	TBuf<150> buf;
    TUint expectedNoMatches = aSearchResult.iDistances.Count();
    if (aSearchResult.iMaxSearchResult != 0 && aSearchResult.iMaxSearchResult < aSearchResult.iDistances.Count())
        {

        }

    ValidateTotalNrOfMatchesL(expectedNoMatches, iMultiDbSearch->TotalNumOfMatches());

	//iLog->Log(_L("Adding..."));

	// First check that result comes from correct db
	TInt nrOfMatches = 0;
	for (TInt j = 0; j < iNrOfDatabases; j++)
        {
        CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL((*iDatabases)[j]);
        if (db->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(db->InitializeL());
       }

        CleanupStack::PushL(db);
        CPosLmItemIterator* iter = iMultiDbSearch->MatchIteratorL(j);
        CleanupStack::PushL(iter);

        nrOfMatches += iter->NumOfItemsL();
        TPosLmItemId id = iter->NextL();
        while (id != KPosLmNullItemId)
            {
            ValidateIdL(id, j);
            id = iter->NextL();
            }
        TBuf<100> buf;
        buf.Format(_L("Found %d nr of matches from db nr %d"), iter->NumOfItemsL(), j);
        //iLog->Log(buf);

        CleanupStack::PopAndDestroy(iter);
        CleanupStack::PopAndDestroy(db);
        }

	//iLog->Log(_L("Done adding..."));

	if (nrOfMatches != iDisplayData->Count())
	{
		iLog->Log(_L("Wrong number of items"));
    	User::Leave(-1);

	}

	// Sorted on distance
	if (aSortOrder == ESortNone)
		{
		TInt index=0;
		TInt itemId = iDisplayData->NewItemIndex();
		while (itemId != KPosLmNoNewItems)
			{

			CPosLmDisplayItem& item = iDisplayData->DisplayItem(itemId);
		    TInt lmId = item.Landmark().LandmarkId();

		       	TReal32 distance =0;
            	TInt dist = item.GetDistance(distance);
            	if(dist == KErrNone)
            		{
            		//iLog->Log(_L("CPosLmDisplayItem::GetDistanced returns KErrNone"));
            		}

            	else if(dist == KErrNotFound)
            		{
            		//iLog->Log(_L("CPosLmDisplayItem::GetDistanced returns KErrNotFound"));
            		}

			// Validate lm from correct db
        	TInt dbIndex = item.DatabaseIndex();

			ValidateIdL(lmId, dbIndex);

		    TBuf<100> buf;
	        buf.Format(_L("Found exp id %d returned id %d"),aSearchResult.iDistances[index].iId, lmId);
	        //iLog->Log(buf);

			if (aSearchResult.iDistances[index].iId != lmId)
	            {
	            // if the distance is equal to the distance to the landmark
	            // before or after then no error.
	            // This to be able to use a database with landmarks with the same position.

	            TBool noError=EFalse;
	            if (index > 0)
	                {
	                noError = (aSearchResult.iDistances[index-1].iDistance == aSearchResult.iDistances[index].iDistance);
	                }
	            if (index < aSearchResult.iDistances.Count() && !noError)
	                {
	                noError = (aSearchResult.iDistances[index+1].iDistance == aSearchResult.iDistances[index].iDistance);
	                }
	            if (!noError)
	                {
	                _LIT(KUnexpectedItemId, "ERROR: Unexpected item id");

	                iErrorsFound++;
	                iLog->Log(KUnexpectedItemId);
	                }
	            }
	            itemId = iDisplayData->NewItemIndex();
	            index++;
	        }
    	}
    	// Ascended and descended
    else
		{
		///////////
	 	for (TInt i=0;i<iDisplayData->Count();i++)
	    	{
            // Check for all but first and last
            if (i>0 && i<iDisplayData->Count()-1)
            	{
            	CPosLmDisplayItem& item = iDisplayData->DisplayItem(i);
            	TInt lmId = item.Landmark().LandmarkId();
            	TReal32 distance =0;
            	TInt dist = item.GetDistance(distance);
            	if(dist == KErrNone)
            		{
            		//iLog->Log(_L("CPosLmDisplayItem::GetDistanced returns KErrNone"));
            		}

            	else if(dist == KErrNotFound)
            		{
            		//iLog->Log(_L("CPosLmDisplayItem::GetDistanced returns KErrNotFound"));
            		}

            	CPosLmDisplayItem& nextItem = iDisplayData->DisplayItem(i+1);
            	TInt nextLmId = nextItem.Landmark().LandmarkId();

            	TPtrC name, nextLmName;
            	item.Landmark().GetLandmarkName(name);
            	nextItem.Landmark().GetLandmarkName(nextLmName);

    			// Validate lm from correct db
            	TInt dbIndex = item.DatabaseIndex();
    			ValidateIdL(lmId, dbIndex);
    			TInt nextDbIndex = nextItem.DatabaseIndex();
    			// Validate next lm from correct db
    			ValidateIdL(nextLmId, nextDbIndex);

            	//debugging purposes


            	TInt order = nextLmName.CompareC(name);
                if (aSortOrder == ESortAscending)
                    { // sorted ascended
                    if (order < 0)
                    	{
                    	iLog->Log(_L("ERROR: Wrong sortorder when ascended"));
                    	iErrorsFound++;
                    	}
                    }
                else // sorted descended
                    {
                    if (order > 0)
                    	{
                    	iLog->Log(_L("ERROR: Wrong sortorder when descended"));
                    	iErrorsFound++;
                    	}

            		}
				}
			}

		}
     iLog->Log(_L("CheckSearchResultL Done"));
    }

// ---------------------------------------------------------
// CPosTp122::ValidateTotalNrOfMatchesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp122::ValidateTotalNrOfMatchesL (
    TUint aRetrievedNrOfMatches,
    TUint aExpectedNrOfMatches)
    {
    _LIT(KNumOfMatchesErr, "ERROR: No. of matches is incorrect when the search is performed");

    if (aRetrievedNrOfMatches != aExpectedNrOfMatches)
        {
        _LIT(KExpected, "Expected no. of matches: %d");
        _LIT(KReturned, "Returned no. of matches: %d");
        TBuf<100> info;
        info.Format(KExpected, aExpectedNrOfMatches);
        iLog->Log(info);
        info.Format(KReturned, aRetrievedNrOfMatches);
        iLog->Log(info);

        iLog->Log(KNumOfMatchesErr);
        iErrorsFound++;
        }
    }

// ---------------------------------------------------------
// CPosTp122::ValidateIdL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp122::ValidateIdL(TPosLmItemId aId, TInt aDbIndex)
    {
    TBool condition = EFalse;
    switch (aDbIndex)
        {
        case 0:
            // Db 1 contains landmarks with id = 1-50
            condition = aId >= iIdDb1Min && aId <= iIdDb1Max;
            break;
        case 1:
            // Db 2 contains landmarks with id = 51-100
            condition = aId >= iIdDb2Min && aId <= iIdDb2Max;
            break;
        case 2:
            // Db 3 contains landmarks with id = 101-150
            condition = aId >= iIdDb3Min && aId <= iIdDb3Max;
            break;
        case 3:
            // Db 4 contains landmarks with id = 151-200
            condition = aId >= iIdDb4Min && aId <= iIdDb4Max;
            break;
        case 4:
            // Db 5 contains landmarks with id = 201-254
            condition = aId >= iIdDb5Min && aId <= iIdDb5Max;
            break;
        }

    if (!condition)
    	{
    	iErrorsFound++;
    	TBuf<150> buf;
    	buf.Format(_L("ERROR: Invalid match from database reported, index %d in db %d"), aId, aDbIndex);
    	iLog->Log(buf);
    	}
    }

// ---------------------------------------------------------
// CPosTp122::TestOnTheFlySearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp122::TestOnTheFlySearchL(const RPointerArray<CNearestSearchResult>& aSearchResults)
    {
    iMultiDbSearch->SetMaxNumOfMatches(KPosLmMaxNumOfMatchesUnlimited);
    iMultiDbSearch->SetMaxNumOfMatches();

    TInt KIndex = 0;
    TCoordinate coord(aSearchResults[KIndex]->iLatitude, aSearchResults[KIndex]->iLongitude);
    CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(coord, aSearchResults[KIndex]->iUseCoverageRadius);
    nearestCriteria->SetMaxDistance(aSearchResults[KIndex]->iMaxDistance);

    RIdArray expectedMatches;
    CleanupClosePushL(expectedMatches);

	TBuf<150> buf;
    COnTheFlyTester* onTheFlyTester = COnTheFlyTester::NewLC();

    // Remove lm with index 48,49,50, they have no location (see lmDbNearestSearch in landmarksdb.xls)
    TInt pos = aSearchResults[KIndex]->iSearchResult.Find(48);
    aSearchResults[KIndex]->iSearchResult.Remove(pos);
    pos = aSearchResults[KIndex]->iSearchResult.Find(49);
    aSearchResults[KIndex]->iSearchResult.Remove(pos);
    pos = aSearchResults[KIndex]->iSearchResult.Find(50);
    aSearchResults[KIndex]->iSearchResult.Remove(pos);

    onTheFlyTester->StartSearchCampaignL(iMultiDbSearch, aSearchResults[KIndex]->iSearchResult, nearestCriteria);

    TInt result = onTheFlyTester->Err();
    if (result != KErrNone)
        {
        TPtrC msg;
        onTheFlyTester->GetErrMsg(msg);

        iLog->Log(msg);
    	User::Leave(result);
        }
    CleanupStack::PopAndDestroy(onTheFlyTester);
    CleanupStack::PopAndDestroy(&expectedMatches);
    CleanupStack::PopAndDestroy(nearestCriteria);
    iLog->Log(_L("TestOnTheFlySearchL Done"));
    }

// ---------------------------------------------------------
// CPosTp122::NrOfSearchErrors
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosTp122::NrOfSearchErrors(CPosLmMultiDbSearch* aMultiDbSearcher)
    {
    TInt nrOfSearchErrors = aMultiDbSearcher->NumOfSearchErrors();
    if (nrOfSearchErrors != 0)
        {
        for (TInt i = 0; i < nrOfSearchErrors; i++)
            {
            CPosLmMultiDbSearch::TSearchError error;
            aMultiDbSearcher->GetSearchError(i, error);
            TBuf<100> msg;
            msg.Format(_L("Found error %d in database %d"), error.iErrorCode, error.iDatabaseIndex);
            iLog->Log(msg);
            }
        }
    return nrOfSearchErrors;
    }


// ---------------------------------------------------------
// CNearestSearchResult::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
CNearestSearchResult* CNearestSearchResult::NewL(
    const TDesC& aDes,
    const TInt& aIndex,
    RFs& aFileSession)
    {
    CNearestSearchResult* self = new(ELeave) CNearestSearchResult(aFileSession);
    CleanupStack::PushL(self);
    self->ConstructL(aDes, aIndex);
    CleanupStack::Pop();
    return self;
    }

// C++ default constructor can NOT contain any code, that
// might leave.
//
CNearestSearchResult::CNearestSearchResult(RFs& aFileSession)
: iFileSession( aFileSession )
    {
    TRealX nan;
    nan.SetNaN();
    iMaxDistance = nan;
    }

// ---------------------------------------------------------
// CNearestSearchResult::CounstructL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CNearestSearchResult::ConstructL(const TDesC& aDes, const TInt& aIndex)
    {
    // Use KNEARESTLMDB as correct template
    iDatabase = CPosLandmarkDatabase::OpenL(KNEARESTLMDB);

    if ( iDatabase->IsInitializingNeeded() )
        {
        TRAPD( err, ExecuteAndDeleteLD( iDatabase->InitializeL() ) );
        }

    TLex line(aDes);
    line.Mark();
    TInt redefined=0, useCoverageRadius;

    if (line.Peek() != '#' && line.Peek() != '\t')
        {
        TInt index=0;
        while (line.Peek() != '\n' && !(line.Peek()).Eos())
            {
            line.Inc();
            if (line.Peek() == '\t')
                {
                TPtrC token = line.MarkedToken();
                TLex val(token);
                switch(index)
                    {
                    case ELatitude:
                        val.Val(iLatitude, TChar(','));
                        break;
                    case ELongitude:
                        val.Val(iLongitude, TChar(','));
                        break;
                    case EUseCoverageRadius:
                        val.Val(useCoverageRadius);
                        if (useCoverageRadius == 1)
                            {
                            iUseCoverageRadius = ETrue;
                            }
                        break;
                    case EMaxDistance:
                        val.Val(iMaxDistance);
                        break;
                    case EErrorCode:
                        val.Val(iErrorCode);
                        break;
                    case ESearchResult:
                        ParseSearchResult(token);
                        break;
                    case ERedefined:
                        val.Val(redefined);
                        if (redefined == 1)
                            {
                            iRedefined = ETrue;
                            }
                        break;
                    case EMaxSearchResult:
                        val.Val(iMaxSearchResult);
                        break;
                    }
                line.Inc();
                while (line.Peek() == '\t') // Empty value
                    {
                    line.Inc();
                    ++index;
                    }
                line.Mark();
                ++index;
                }
            }
        AppendDistancesL(aIndex);

        }
    }


// Destructor
CNearestSearchResult::~CNearestSearchResult()
    {
    iDistances.Close();
    delete iDatabase;
    iDatabase = NULL;
    }

// ---------------------------------------------------------
// CNearestSearchResult::AppendDistancesL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CNearestSearchResult::AppendDistancesL(const TInt& aIndex)
    {
    iDistances.Reset();

     _LIT(KFileTestValues, "c:\\system\\test\\testdata\\lmDbNearestSearch.txt");

    RFile file;
    CleanupClosePushL(file);
    User::LeaveIfError(file.Open(iFileSession, KFileTestValues,  EFileRead));

    TInt row=0;
    TBuf<KMaxNearestLineLength> line;
    while (CLandmarkTestProcedureBase::ReadLn(file, line) != KErrEof)
        {
        ParseDistanceL(line, aIndex, row);
        ++row;
        }

    SortDistances();

    CleanupStack::PopAndDestroy(&file);
    }

// ---------------------------------------------------------
// CNearestSearchResult::ParseDistancesL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CNearestSearchResult::ParseDistanceL(const TDesC& aDes,
                                          const TInt& aIndex,
                                          const TPosLmItemId& aId)
    {

    const TInt KStartIndex = 5;

    TLex line(aDes);
    line.Mark();
    TInt index = 0;
    TReal64 dist=0;

    if (line.Peek() != '#' && line.Peek() != '\t')
        {
        while (line.Peek() != '\n' && !(line.Peek()).Eos())
            {
            line.Inc();
            if (line.Peek() == '\t' || line.Peek() == '\n' || (line.Peek()).Eos())
                {
                TPtrC token = line.MarkedToken();

                if (index == (KStartIndex + aIndex))
                    {
                    TLex val(token);
                    val.Val(dist, TChar(','));

                    if (iUseCoverageRadius)
                        {
                        CPosLandmark* lm = iDatabase->ReadLandmarkLC(aId);

                        TReal32 aCoverageRadius;
                        if (lm->GetCoverageRadius(aCoverageRadius) == KErrNone)
                            {
                            dist -= aCoverageRadius;
                            if (dist < 0)
                                {
                                dist = 0;
                                }
                            }
                        CleanupStack::PopAndDestroy(lm);
                        }

                    if (Math::IsNaN(iMaxDistance) || dist <= iMaxDistance)
                        {
                        if (dist != -1)
                            {
                            TSearchResult searchResult(aId, TReal32(dist));
                            iDistances.Append(searchResult);
                            }
                        }

                    break;
                    }
                line.Inc();
                while (line.Peek() == '\t') // Empty value
                    {
                    line.Inc();
                    ++index;
                    }
                line.Mark();
                ++index;
                }
            }
        }
    }
// ---------------------------------------------------------
// CNearestSearchResult::Latitude
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal64 CNearestSearchResult::Latitude() const
    {
    return iLatitude;
    }

// ---------------------------------------------------------
// CNearestSearchResult::Latitude
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal64 CNearestSearchResult::Longitude() const
    {
    return iLongitude;
    }

// ---------------------------------------------------------
// CNearestSearchResult::UseCoverageRadius
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CNearestSearchResult::UseCoverageRadius() const
    {
    return iUseCoverageRadius;
    }

// ---------------------------------------------------------
// CNearestSearchResult::MaxDistance
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal32 CNearestSearchResult::MaxDistance() const
    {
    return iMaxDistance;
    }

// ---------------------------------------------------------
// CNearestSearchResult::ErrorCode
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CNearestSearchResult::ErrorCode() const
    {
    return iErrorCode;
    }

// ---------------------------------------------------------
// InternalLandmarksSortL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt InternalSort(const TSearchResult& aSource, const TSearchResult& aTarget)
    {
    // zero, if the two objects are equal
    // a negative value, if the first object is less than the second.
    // a positive value, if the first object is greater than the second.

    if ( aSource.Distance() == aTarget.Distance())
        {
        return 0;
        }
    else if (aSource.Distance() < aTarget.Distance())
        {
        return -1;
        }

    return 1;
    }

// ---------------------------------------------------------
// CNearestSearchResult::ErrorCode
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CNearestSearchResult::SortDistances()
    {
    TLinearOrder<TSearchResult> order(InternalSort);
    iDistances.Sort(order);
    }

// ---------------------------------------------------------
// CNearestSearchResult::Print
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CNearestSearchResult::Print(TDes& aPrint)
    {
    _LIT(KPosInfo, "Latitude=%g, Longitude=%g");
    TChar del(',');

    TBuf<KMaxFieldLength> info;
    info.Format(KPosInfo, iLatitude, iLongitude);
    aPrint.Append(info);
    aPrint.Append(del);

    _LIT(KUseCoverageRadius, "UseCoverageRadius=%d");
    info.Format(KUseCoverageRadius, iUseCoverageRadius);
    aPrint.Append(info);
    aPrint.Append(del);

    if (!Math::IsNaN(iMaxDistance))
        {
        _LIT(KMaxDistance, "MaxDistance=%g");
        info.Format(KMaxDistance, iMaxDistance);
        aPrint.Append(info);
        aPrint.Append(del);
        }

    _LIT(KErrorCode, "ErrorCode = %d");
    info.Format(KErrorCode, iErrorCode);
    aPrint.Append(info);
    aPrint.Append(del);

    _LIT(KRedefined, "Redefined=%d");
    info.Format(KRedefined, iRedefined);
    aPrint.Append(info);
    aPrint.Append(del);

     _LIT(KMaxSearchResult, "MaxSearchResult=%d");
    info.Format(KMaxSearchResult, iMaxSearchResult);
    aPrint.Append(info);
    aPrint.Append(del);


    PrintDistances(aPrint);
    }

// ---------------------------------------------------------
// CNearestSearchResult::PrintDistances
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CNearestSearchResult::PrintDistances(TDes& aPrint)
    {
    TBuf<KDistanceFieldLength> info2;
    TChar del(':');
    aPrint.Append(_L("Distances: "));

    for (TInt j=0; j<iDistances.Count(); j++)
        {
        _LIT(KDistance, "%g,%d");
        info2.Format(KDistance, iDistances[j].Distance(), iDistances[j].Id());
        aPrint.Append(info2);
        aPrint.Append(del);
        }
    }

// ---------------------------------------------------------
// TSearchResult::TSearchResult
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TSearchResult::TSearchResult()
    {
    TRealX nan;
    nan.SetNaN();
    iDistance=nan;
    iId=0;
    }

TSearchResult::TSearchResult(const TPosLmItemId& aId, const TReal32& aDistance) :
    iDistance(aDistance),
    iId(aId)
    {
    }

// ---------------------------------------------------------
// TSearchResult::Distance
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal64 TSearchResult::Distance() const
    {
    return iDistance;
    }

// ---------------------------------------------------------
// TSearchResult::Id
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TPosLmItemId TSearchResult::Id() const
    {
    return iId;
    }
//  End of File
