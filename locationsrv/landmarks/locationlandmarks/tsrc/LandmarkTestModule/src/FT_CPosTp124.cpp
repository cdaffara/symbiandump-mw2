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
#include "FT_CPosTp124.h"
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

#include <EPos_CPosLmDisplayData.h>
#include <EPos_CPosLmDisplayItem.h>

#include <EPos_CPosLMItemIterator.h>    
#include <EPos_CPosLmMultiDbSearch.h>    
#include <EPos_CPosLmDatabaseManager.h>

#include <e32math.h>

// CONSTANTS
_LIT(KLMDB1, "COMPOSITELMDB_1.LDB");
_LIT(KLMDB2, "COMPOSITELMDB_2.LDB");
_LIT(KLMDB3, "COMPOSITELMDB_3.LDB");
_LIT(KLMDB4, "COMPOSITELMDB_4.LDB");
_LIT(KLMDB5, "COMPOSITELMDB_5.LDB");
_LIT(KLMDB1Path,"file://C:COMPOSITELMDB_1.LDB");
_LIT(KLMDB2Path,"file://C:COMPOSITELMDB_2.LDB");
_LIT(KLMDB3Path,"file://C:COMPOSITELMDB_3.LDB");
_LIT(KLMDB4Path,"file://C:COMPOSITELMDB_4.LDB");
_LIT(KLMDB5Path,"file://C:COMPOSITELMDB_5.LDB");

//_LIT(KCOMPOSITELMDB, "compositeeposlm.ldb");

// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp124::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    delete iDatabase;
    iDatabase=NULL;
    
    delete iDatabases;
    
    if (iMultiDbSearch) iMultiDbSearch->UnsetDisplayData();
    delete iDisplayData;
    iDisplayData = NULL;
    
    delete iMultiDbSearch;
    
    TRAPD(err, RemoveAllLmDatabasesL());
    if (err != KErrNone) iLog->Log(_L("Error when RemoveAllLmDatabasesL"));
    }

// ---------------------------------------------------------
// CPosTp124::InitTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::InitTestL()
    {
    CLandmarkTestProcedureBase::InitTestL();
    
    iLog->Log(_L("InitTestL"));
    RemoveAllLmDatabasesL();
    CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    // check the count of databases that could not be delted since they were in use
    iDatabases = dbManager->ListDatabasesLC();
    CleanupStack::Pop(iDatabases);
    iDbCount = iDatabases->Count();

    
    // PrepareDatabases
    PrepareDatabasesL();
    
	SetupLimits();
    
    
    
    iDatabases = dbManager->ListDatabasesLC();
    CleanupStack::Pop(iDatabases);
    iDatabases->Sort(); // Not really necessary...
    // database count is equal to ones added plus ones not been able to delte since they were in use
    TInt dbCount = iDatabases->Count();
    iNrOfDatabases = iDbCount + 5;
    
    iMultiDbSearch = CPosLmMultiDbSearch::NewL(*iDatabases);
    CleanupStack::PopAndDestroy(dbManager);
    
    iDisplayData = CPosLmDisplayData::NewL();
	iMultiDbSearch->SetDisplayData(*iDisplayData);
                    
    }
    
// ---------------------------------------------------------
// CPosTp124::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::StartL()
    {
    TTime startTime;
    TTime stopTime;
    startTime.UniversalTime();
    
    MakeSurePanicDebugFileExistsL();
        
    iDatabase = UseCompositeLandmarksDbFileL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

	// TC_Part 1
	// ASynchronous, Sort ascending
    StartLandmarksCompositeTestL(EAsynchronous, ESortAscending);
    // Synchronous, Sort descending
    StartLandmarksCompositeTestL(EAsynchronous, ESortDescending);
    // ASynchronous, Sort ascending
    TestWithSortOrderLandmarksL(EAsynchronous, ESortAscending);
        
    TestWithSortOrderNearestL(EAsynchronous, ESortAscending);
    TestWithSortOrderNearestL(ESynchronous, ESortNone);
    TestWithSortOrderNearestL(EAsynchronous, ESortDescending);
    TestWithSortOrderNearestL(EAsynchronous, ESortDistance);
		
    // TC Part 2_3 Test with several text criterias
    TestSeveralTextArgumentsL();

	// TC Part 4-5 Test add and remove from composite object
    TestAddAndRemoveL();
    
   	// TC Part 6
	// Cancel immediately
    CancelTestL(EFalse);
    
    // Cancel in callback
    CancelTestL(ETrue);
    
	// TC Part 7
    TestOnTheFlySearchL();

    // Since TestOnTheFlySearchL is changing displadata
    iMultiDbSearch->UnsetDisplayData();
    delete iDisplayData;
    iDisplayData = NULL;
    iDisplayData = CPosLmDisplayData::NewL();
	iMultiDbSearch->SetDisplayData(*iDisplayData);
	
    // TC Part 8-11
  //  TestLessDbsToSearchL();

    // TC Part 12 
    // It is not possible to use Composite in a composite object in multi search
    TestCompositeInCompositeL();
        
    // TC Part 13
    // It is not possible to use IdList in a composite object in multi search
    TestWithIdListAndCompositeL();
    
    // TC Part 14
    // It is not possible to use CPosLmCatNameCriteria in a composite object in multi search
    TestCategoryCompositeSearchL();
    
    // TC Part 15
    // Test with no previous result but try to refine search anyway
	TestWithNoPreviousResultL();
	
	TInt KNrOfObjectsInComposite = 100;
    TInt KStartIndex = 90;
    CompositeStressTestL(KStartIndex, KNrOfObjectsInComposite);
	
    stopTime.UniversalTime();
    TTimeIntervalMicroSeconds executionTime = 
        stopTime.MicroSecondsFrom(startTime);

    TInt resse = executionTime.Int64()/1000000;
    TBuf<100> buf;
    buf.Format(_L("TP124 took %d seconds"), resse);
    iLog->Log(buf);
    
    if (iErrorsFound != KErrNone) 
    	{
    	iLog->Log(_L("Errors found in TP124"));
    	User::Leave(-1);
    	}
	}

// ---------------------------------------------------------
// CPosTp124::SetupLimits()
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CPosTp124::SetupLimits()
	{
	// First db-> Keep landmarks 1-20
    // Second db-> Keep landmarks 21-40
    // Third db-> Keep landmarks 41-60
    // Fourth db-> Keep landmarks 61-80
    // Fifth db-> Keep landmarks 81-105
    
    iIdDb1Min = 1;
    iIdDb1Max = 20;
    
    iIdDb2Min = 21;
    iIdDb2Max = 40;
    
    iIdDb3Min = 41;
    iIdDb3Max = 60;
    
    iIdDb4Min = 61;
    iIdDb4Max = 80;
    
    iIdDb5Min = 81;
    iIdDb5Max = 105;
	}

// ---------------------------------------------------------
// CPosTp124::ValidateIdL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::ValidateIdL(TPosLmItemId aId, TInt aDbIndex)
    {
    TBool condition = EFalse;
    switch (aDbIndex)
        {        
        case 0:
            // Db 1 contains landmarks with id = 1-20
            condition = aId >= iIdDb1Min && aId <= iIdDb1Max;
            break;
        case 1:
            // Db 2 contains landmarks with id = 21-40
            condition = aId >= iIdDb2Min && aId <= iIdDb2Max;
            break;
        case 2:
            // Db 3 contains landmarks with id = 41-60
            condition = aId >= iIdDb3Min && aId <= iIdDb3Max;
            break;
        case 3:
            // Db 4 contains landmarks with id = 61-80
            condition = aId >= iIdDb4Min && aId <= iIdDb4Max;
            break;
        case 4:
            // Db 5 contains landmarks with id = 81-105
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
// CPosTp124::PrepareDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::PrepareDatabasesL()
	{
	iLog->Log(_L("PrepareDatabasesL"));
	// Copy eposlmnearestsearch.ldb into five dbs
    _LIT(KDbFile, "c:\\system\\test\\testdata\\compositeeposlm.ldb");

    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);
    
    // Create five copies of testdata\\compositeeposlm.ldb
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
    // First db-> Keep landmarks 1-20
    // Second db-> Keep landmarks 21-40
    // Third db-> Keep landmarks 41-60
    // Fourth db-> Keep landmarks 61-80
    // Fifth db-> Keep landmarks 81-105
    
    RArray<TPosLmItemId> array;
    CleanupClosePushL(array);
    
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL(KLMDB1);
    CleanupStack::PushL(lmd);
        
    TBool debug = EFalse;
    TInt index=0;
    TInt max=105;
   	for (index=20;index<max;index++) array.Append(index+1);
    if (lmd->IsInitializingNeeded()) ExecuteAndDeleteLD(lmd->InitializeL());
    ExecuteAndDeleteLD(lmd->RemoveLandmarksL(array));
    User::After(200000);
    ExecuteAndDeleteLD(lmd->CompactL());
    array.Reset();
    if (debug) PrintLm(lmd);
    CleanupStack::PopAndDestroy(lmd);
            
	lmd = CPosLandmarkDatabase::OpenL(KLMDB2);
    CleanupStack::PushL(lmd);
        
   	for (index=0;index<20;index++) array.Append(index+1);
   	for (index=40;index<max;index++) array.Append(index+1);
    if (lmd->IsInitializingNeeded()) ExecuteAndDeleteLD(lmd->InitializeL());
    ExecuteAndDeleteLD(lmd->RemoveLandmarksL(array));
    User::After(200000);
    ExecuteAndDeleteLD(lmd->CompactL());
    array.Reset();
	if (debug) PrintLm(lmd);
    CleanupStack::PopAndDestroy(lmd);
    
	lmd = CPosLandmarkDatabase::OpenL(KLMDB3);
    CleanupStack::PushL(lmd);
        
   	for (index=0;index<40;index++) array.Append(index+1);        
   	for (index=60;index<max;index++) array.Append(index+1);
    if (lmd->IsInitializingNeeded()) ExecuteAndDeleteLD(lmd->InitializeL());
    ExecuteAndDeleteLD(lmd->RemoveLandmarksL(array));
    User::After(200000);
    ExecuteAndDeleteLD(lmd->CompactL());
    array.Reset();
	if (debug) PrintLm(lmd);
    CleanupStack::PopAndDestroy(lmd);
    
	lmd = CPosLandmarkDatabase::OpenL(KLMDB4);
    CleanupStack::PushL(lmd);
        
   	for (index=0;index<60;index++) array.Append(index+1);
   	for (index=80;index<max;index++) array.Append(index+1);
    if (lmd->IsInitializingNeeded()) ExecuteAndDeleteLD(lmd->InitializeL());
    ExecuteAndDeleteLD(lmd->RemoveLandmarksL(array));
    User::After(200000);
    ExecuteAndDeleteLD(lmd->CompactL());
    array.Reset();
	if (debug) PrintLm(lmd);
    CleanupStack::PopAndDestroy(lmd);            
    
	lmd = CPosLandmarkDatabase::OpenL(KLMDB5);
    CleanupStack::PushL(lmd);
        
	for (index=0;index<80;index++) array.Append(index+1);
    if (lmd->IsInitializingNeeded()) ExecuteAndDeleteLD(lmd->InitializeL());
    ExecuteAndDeleteLD(lmd->RemoveLandmarksL(array));
    User::After(200000);
    ExecuteAndDeleteLD(lmd->CompactL());
    array.Reset();
	if (debug) PrintLm(lmd);
    
    CleanupStack::PopAndDestroy(lmd);    
    
    CleanupStack::PopAndDestroy(&array);
    iLog->Log(_L("PrepareDatabasesL Done"));
	}
	
// ---------------------------------------------------------
// CPosTp124::PrintLm
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::PrintLm(CPosLandmarkDatabase* db)
	{
	CPosLmItemIterator* iter = db->LandmarkIteratorL();
	CleanupStack::PushL(iter);
	iLog->Log(_L("PrintLm"));

	TPosLmItemId id = iter->NextL();
	TBuf<100> buf;

	while (id != KPosLmNullItemId)
		{
		CPosLandmark* lm = db->ReadLandmarkLC(id);
		TPtrC name;
		lm->GetLandmarkName(name);
		iLog->Log(_L("Found LM: "));
		iLog->Log(name);
		TInt idde = lm->LandmarkId();
		
		id = iter->NextL();
		CleanupStack::PopAndDestroy(lm);
		}
	
	CleanupStack::PopAndDestroy(iter);
	iLog->Log(_L("PrintLm Done"));
	}

// ---------------------------------------------------------
// CPosTp124::TestOnTheFlySearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::TestOnTheFlySearchL()
    {
    iLog->Log(_L("TestOnTheFlySearchL"));
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

    CCompositeSearchResult* res = CCompositeSearchResult::NewL(iLog);
    CleanupStack::PushL(res);

    while (!res->GetCompositeSearchResultsL(*composite))
        {
        // Check if IdList is included in composite, if so dont 
        // use this search since IdList is not possible to use in multiple db seach
        if (!CheckCompositeForIdListL(composite))
        	{
        	PrintCompositeInfo(composite);
			iLog->Log(_L("iLandmarkSearch->StartLandmarkSearchL"));
			TBuf<150> buf;
    		COnTheFlyTester* onTheFlyTester = COnTheFlyTester::NewLC();
    		onTheFlyTester->StartSearchCampaignL(iMultiDbSearch, res->iResultIds, composite);
    
    		TInt result = onTheFlyTester->Err();
    		if (result != KErrNone)
		        {
		        TPtrC msg;
		        onTheFlyTester->GetErrMsg(msg);
		        iLog->Log(msg);
    			User::Leave(result);
		        }
    		CleanupStack::PopAndDestroy(onTheFlyTester);
			}
    }
    
    CleanupStack::PopAndDestroy(res);
    CleanupStack::PopAndDestroy(composite);
    iLog->Log(_L("TestOnTheFlySearchL Done"));
    
    }

// ---------------------------------------------------------
// CPosTp124::TestLessDbsToSearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::TestLessDbsToSearchL()
	{
	iLog->Log(_L("TestLessDbsToSearchL"));
	CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
	
	TBuf<100> buf;
	
    // First create a textcriteria
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();   
    textCriteria->SetTextL(_L("LO*"));
    User::LeaveIfError(composite->AddArgument(textCriteria));
    CleanupStack::Pop(textCriteria);
    
    // Create search result
   	RArray<TPosLmItemId> searchResults;
	CleanupClosePushL(searchResults);
	
	// Will find:
	searchResults.Append(1);
	searchResults.Append(2);
	searchResults.Append(20);
	searchResults.Append(21);
	searchResults.Append(22);
	searchResults.Append(23);
	searchResults.Append(24);
	searchResults.Append(26);
	searchResults.Append(27);
	searchResults.Append(28);
	
	iLog->Log(_L("Search in all dbs"));
		
	SearchL(composite, ESynchronous, EFalse, ESortNone);
	ValidateSearchResultsL(searchResults, ESortNone);
	    
	// delete the COMPOSITELMDB_1 & COMPOSITELMDB_3
	TInt count = iDatabases->Count();
	for(TInt i =0;i<count;i++)
	    {
	TPtrC16 dbName((*iDatabases)[i]);
        if(dbName.Compare(KLMDB1Path) == KErrNone || dbName.Compare(KLMDB4Path) == KErrNone )
            {
            iDatabases->Delete(i);
            }
	    }
  //  iDatabases->Delete(0);
  //  iDatabases->Delete(3);
	
    iNrOfDatabases-=2;
    
    iIdDb1Min = 21;
    iIdDb1Max = 40;
    
    iIdDb2Min = 41;
    iIdDb2Max = 60;
    
    iIdDb3Min = 81;
    iIdDb3Max = 105;
    
    iMultiDbSearch->SetDatabasesToSearchL(*iDatabases);
    
    iLog->Log(_L("Search in all dbs but db0 and db3"));
    // Remove three first items, found in db0
    searchResults.Remove(0); // id 1
    searchResults.Remove(0); // id 2
    searchResults.Remove(0); // id 20
	SearchL(composite, ESynchronous, EFalse, ESortNone);
	ValidateSearchResultsL(searchResults, ESortNone);
             	
	// Open DB last in list and add a new landmark
 	CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL(KLMDB3);
    CleanupStack::PushL(db);
 	CPosLandmark* lm1 = CPosLandmark::NewLC();
	lm1->SetLandmarkNameL(_L("LOOOPIA Landmark"));
	lm1->SetLandmarkDescriptionL(_L("Some Description"));
	
	iLog->Log(_L("Add new Landmark to db nr 0"));
    TPosLmItemId newLmId = db->AddLandmarkL(*lm1);
	CleanupStack::PopAndDestroy(lm1);
	
	iLog->Log(_L("Search in all dbs but db0 and db3"));
	//Refine search. new landmark should not be included
	SearchL(composite, ESynchronous, ETrue, ESortNone);
	ValidateSearchResultsL(searchResults, ESortNone);
	
	CPosLmCategoryCriteria* catSearchCriteria1 = CPosLmCategoryCriteria::NewLC();
	CPosLmCategoryCriteria* catSearchCriteria2 = CPosLmCategoryCriteria::NewLC();
    
    catSearchCriteria1->SetCategoryNameL(_L("Stadsbibliotek"));
    catSearchCriteria2->SetCategoryNameL(_L("Systembolag"));
    
    User::LeaveIfError(composite->AddArgument(catSearchCriteria1));
    User::LeaveIfError(composite->AddArgument(catSearchCriteria2));
    CleanupStack::Pop(catSearchCriteria2);
    CleanupStack::Pop(catSearchCriteria1);
    
    iLog->Log(_L("Search in all dbs but db0 and db3"));
	//Refine search with more specific composite search object
	// new landmark should not be included
	
	searchResults.Reset();
	searchResults.Append(27);
	
	SearchL(composite, ESynchronous, ETrue, ESortNone);
	ValidateSearchResultsL(searchResults, ESortNone);

	// Remove some criterias from composite and do a new search,
	iLog->Log(_L("Search in all dbs but db0 and db3, remove last argument from composite"));
	// Remove last added argument
	CPosLmSearchCriteria* obj1 = composite->RemoveArgument(composite->NumOfArguments()-1);
	delete obj1;
    obj1 = NULL;

	//Refine search. new landmark should not be included
	SearchL(composite, ESynchronous, ETrue, ESortNone);
	ValidateSearchResultsL(searchResults, ESortNone);
	
	// Remove last added argument
	obj1 = composite->RemoveArgument(composite->NumOfArguments()-1);
	delete obj1;
    obj1 = NULL;
    
	//Do a new search (not more refined). new landmark should be included
	searchResults.Append(newLmId);
	iIdDb3Max = newLmId;
	searchResults.Reset();
	searchResults.Append(21);
	searchResults.Append(22);
	searchResults.Append(23);
	searchResults.Append(24);
	searchResults.Append(26);
	searchResults.Append(27);
	searchResults.Append(28);
	searchResults.Append(newLmId);
	iLog->Log(_L("Search in all dbs but db0 and db3, without refined"));
	SearchL(composite, ESynchronous, EFalse, ESortNone);
	ValidateSearchResultsL(searchResults, ESortNone);
    
    // Remove the added landmark
    db->RemoveLandmarkL(newLmId);
    CleanupStack::PopAndDestroy(db);
    	
    CleanupStack::PopAndDestroy(&searchResults);
    CleanupStack::PopAndDestroy(composite);
    // Correct limits if more testing after this part
    SetupLimits();
    
    CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    delete iDatabases;
    iDatabases = NULL;
    iDatabases = dbManager->ListDatabasesLC();
    CleanupStack::Pop(iDatabases);
    iDatabases->Sort(); // Not really necessary...
    // Remove compositeeposlm.ldb that should not be searched
    iNrOfDatabases = iDbCount + 5;
    iDatabases->Delete(0);
    AssertTrueSecL((iDatabases->Count() == iNrOfDatabases), _L("Wrong number of databases"));
    
    iMultiDbSearch->UnsetDisplayData();
    delete iDisplayData;
    iDisplayData = NULL;
    
    delete iMultiDbSearch;
    iMultiDbSearch = NULL;
    
    iMultiDbSearch = CPosLmMultiDbSearch::NewL(*iDatabases);
    iDisplayData = CPosLmDisplayData::NewL();
	iMultiDbSearch->SetDisplayData(*iDisplayData);	
    
    CleanupStack::PopAndDestroy(dbManager);

	}
	
// ---------------------------------------------------------
// CPosTp124::TestWithSortOrderNearestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::TestWithSortOrderNearestL(TExecutionMode aExecutionMode,
    										 TTp124SortOrder aSortOrder)
	{
	iLog->Log(_L("TestWithSortOrderNearestL"));
	
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
	
    // First create a textcriteria
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();   
    textCriteria->SetTextL(_L("*"));
    User::LeaveIfError(composite->AddArgument(textCriteria));
    CleanupStack::Pop(textCriteria);

    // Then create a nearest criteria same coordinate as landmark itemid=31 Text1
    TCoordinate coord(45.5, 0.5);
    iCoord = coord;
    CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(coord, ETrue);
    nearestCriteria->SetMaxDistance(80000);
    
    
    User::LeaveIfError(composite->AddArgument(nearestCriteria));
    CleanupStack::Pop(nearestCriteria);
    
    // Create search result
   	RArray<TPosLmItemId> searchResults;
	CleanupClosePushL(searchResults);
	
	// Will find Unique, Text'1 and Text1
	searchResults.Append(29);
	searchResults.Append(31);
	searchResults.Append(30);
		
	iLog->Log(_L("Innan search"));
	SearchL(composite, aExecutionMode, EFalse, aSortOrder);
	iLog->Log(_L("Innan validate"));
	ValidateSearchResultsL(searchResults, aSortOrder);
          
    CleanupStack::PopAndDestroy(&searchResults);
    CleanupStack::PopAndDestroy(composite);
	}

// ---------------------------------------------------------
// CPosTp124::CheckCompositeForIdListL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp124::CheckCompositeForIdListL(CPosLmCompositeCriteria* aComposite)
	{
	iLog->Log(_L("CheckCompositeForIdListL"));
	TBool res = EFalse;
    for (TUint i=0;i<aComposite->NumOfArguments();i++)
    	{
        CPosLmSearchCriteria& comp = aComposite->Argument(i);
        TInt type = comp.CriteriaType();
        if (type == CPosLmSearchCriteria::ECriteriaIdList)
            {
			res = ETrue;
            }
		}
		return res;
	}

// ---------------------------------------------------------
// CPosTp124::StartLandmarksCompositeTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::StartLandmarksCompositeTestL(TExecutionMode aExecutionMode,
    										 TTp124SortOrder aSortOrder)
    {       
    iLog->Log(_L("StartLandmarksCompositeTestL"));
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

    CCompositeSearchResult* res = CCompositeSearchResult::NewL(iLog);
    CleanupStack::PushL(res);

    

    while (!res->GetCompositeSearchResultsL(*composite))
        {
        // Check if IdList is included in composite, if so dont 
        // use this search since IdList is not possible to use in multiple db seach
        if (!CheckCompositeForIdListL(composite))
        	{
        	PrintCompositeInfo(composite);
			iLog->Log(_L("iLandmarkSearch->StartLandmarkSearchL"));
	        SearchL(composite, aExecutionMode, res->Redefined(), aSortOrder);
			ValidateSearchResultsL(res->iResultIds, aSortOrder);
			}
    }
    
    CleanupStack::PopAndDestroy(res);
    CleanupStack::PopAndDestroy(composite);
    iLog->Log(_L("StartLandmarksCompositeTestL Done"));
    }
    
// ---------------------------------------------------------
// CPosTp124::TestWithSortOrderLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::TestWithSortOrderLandmarksL(TExecutionMode aExecutionMode,
    										 TTp124SortOrder aSortOrder)
	{
	iLog->Log(_L("TestWithSortOrderLandmarksL"));
    
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
	TInt i=0;
	
	TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
	
    // First create a textcriteria
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();   
    textCriteria->SetTextL(_L("*"));
    User::LeaveIfError(composite->AddArgument(textCriteria));
    CleanupStack::Pop(textCriteria);
    
    // Then create a nearest criteria
    TCoordinate coord(12.123, -20,123);
    CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(coord, EFalse);
    User::LeaveIfError(composite->AddArgument(nearestCriteria));
    CleanupStack::Pop(nearestCriteria);
            
    // Create search result of all landmark ids: 1-105
   	RArray<TPosLmItemId> searchResults;
	CleanupClosePushL(searchResults);
	for (i=1;i<106;i++) {searchResults.Append(i);}
	
	SearchL(composite, aExecutionMode, EFalse, aSortOrder);
	ValidateSearchResultsL(searchResults, aSortOrder);

	CleanupStack::PopAndDestroy(&searchResults);
    CleanupStack::PopAndDestroy(composite);
    
	}
	
// ---------------------------------------------------------
// CPosTp124::SearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::SearchL(
    CPosLmCompositeCriteria* aCompositeSearchCriteria,
    TExecutionMode aExecutionMode,
    TBool aRefinedSearch,
    TTp124SortOrder aSortOrder)
    {
    switch (aSortOrder)
        {
        case ESortNone:
        	{
            iOperation = iMultiDbSearch->StartLandmarkSearchL(*aCompositeSearchCriteria, aRefinedSearch);
            break;
            }
        case ESortAscending:
        	{
            TPosLmSortPref ascending(CPosLandmark::ELandmarkName);
            iOperation = iMultiDbSearch->StartLandmarkSearchL(*aCompositeSearchCriteria, ascending, aRefinedSearch);
            break;
            }
        case ESortDescending:
        	{
            TPosLmSortPref descending(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
            iOperation = iMultiDbSearch->StartLandmarkSearchL(*aCompositeSearchCriteria, descending, aRefinedSearch);
            break;
            }
		case ESortDistance:
			{
            iOperation = iMultiDbSearch->StartLandmarkSearchL(*aCompositeSearchCriteria, aRefinedSearch);
            break;
            }
        default:
        	{
            User::Leave(KErrArgument);
            break;            
            }
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
// CPosTp124::TestSeveralTextArgumentsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::TestSeveralTextArgumentsL()
    {
    iLog->Log(_L("TestSeveralTextArgumentsL"));        
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

	// >>>>>>> Test different text criterias <<<<<<<<<<
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
    iLog->Log(_L("iMultiDbSearch->StartLandmarkSearchL"));
    RIdArray expectedIds;
    CleanupClosePushL(expectedIds);
    User::LeaveIfError(expectedIds.Append(63));
    
    SearchL(composite, EAsynchronous, EFalse, ESortAscending);
    ValidateSearchResultsL(expectedIds, ESortAscending);
    composite->ClearArguments();
    
    // >>>>>>> Test same text as criterias in composite <<<<<<<
    textCriteria = CPosLmTextCriteria::NewLC();    
    textCriteria->SetTextL(_L("Text9"));
    err = composite->AddArgument(textCriteria);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument1"));
    	User::Leave(err);
    }
    CleanupStack::Pop(textCriteria);

    // First create a textcriteria
    textCriteria2 = CPosLmTextCriteria::NewLC();    
    textCriteria2->SetTextL(_L("Text9"));
    err = composite->AddArgument(textCriteria2);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error when adding argument2"));
    	User::Leave(err);
    }
    CleanupStack::Pop(textCriteria2);
	expectedIds.Reset();
    // Should find landmark id: 39 name: Text9
    iLog->Log(_L("iMultiDbSearch->StartLandmarkSearchL"));
    User::LeaveIfError(expectedIds.Append(39));
    
    SearchL(composite, EAsynchronous, EFalse, ESortDescending);
    ValidateSearchResultsL(expectedIds, ESortDescending);
    
    CleanupStack::PopAndDestroy(&expectedIds);
    CleanupStack::PopAndDestroy(composite);
	iDisplayData->Reset();
    }

// ---------------------------------------------------------
// CPosTp124::TestAddAndRemoveL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::TestAddAndRemoveL()
    {
    iLog->Log(_L("TestAddAndRemoveL"));
    
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

    // Add a text criteria
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    textCriteria->SetTextL(_L("PELLE"));
    User::LeaveIfError(composite->AddArgument(textCriteria));
    CleanupStack::Pop(textCriteria);
    
    // Test that ClearArguments is correct
    composite->ClearArguments();
    
    if (composite->NumOfArguments() != 0)
    {
    	iLog->Log(_L("Wrong number of arguments"));
    	User::Leave(-1);
    }
    
    // 1) Add a text criteria
    textCriteria = CPosLmTextCriteria::NewLC();
    textCriteria->SetTextL(_L("*"));
    User::LeaveIfError(composite->AddArgument(textCriteria));
    CleanupStack::Pop(textCriteria);

    // 2) Add a text criteria 
    textCriteria = CPosLmTextCriteria::NewLC();
    textCriteria->SetTextL(_L("Natur*"));
    User::LeaveIfError(composite->AddArgument(textCriteria));
    CleanupStack::Pop(textCriteria);

    // 3) Add an area criteria
    CPosLmAreaCriteria* areaCriteria = CPosLmAreaCriteria::NewLC(-12.23, 34.4, -2.05, 45.5);
    User::LeaveIfError(composite->AddArgument(areaCriteria));
    CleanupStack::Pop(areaCriteria);

    // 4) Add a category criteria
    CPosLmCategoryCriteria* catCriteria = CPosLmCategoryCriteria::NewLC();
    catCriteria->SetCategoryNameL(_L("Museum"));
    User::LeaveIfError(composite->AddArgument(catCriteria));
    CleanupStack::Pop(catCriteria);

    // 5) Add a nearest criteria
    TCoordinate cord(12.23, 12);
    CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(cord, ETrue);
    User::LeaveIfError(composite->AddArgument(nearestCriteria));
    CleanupStack::Pop(nearestCriteria);

    if (composite->NumOfArguments() != 5)
    {
    	iLog->Log(_L("Wrong number of arguments"));
    	User::Leave(-1);
    }

    // Remove (3) area criteria
    CPosLmSearchCriteria* obj1 = composite->RemoveArgument(2);
    delete obj1;obj1 = NULL;

    // Remove (5) nearest criteria (after 3 removed it has id 4)
    CPosLmSearchCriteria* obj2 = composite->RemoveArgument(3);
    delete obj2;obj2 = NULL;

    RIdArray expectedIds;
    CleanupClosePushL(expectedIds);
    
    // Should find landmark with id: 87 name: "Naturhistoriska"
    User::LeaveIfError(expectedIds.Append(87));
    SearchL(composite, EAsynchronous, EFalse, ESortAscending);
    ValidateSearchResultsL(expectedIds, ESortAscending);

    // Remove (1) first text criteria
    CPosLmSearchCriteria* obj4 = composite->RemoveArgument(0);
    delete obj4; obj4 = NULL;

    // Remove (2) the second text criteria
    CPosLmSearchCriteria* obj3 = composite->RemoveArgument(0);
    delete obj3; obj3 = NULL;
   
    expectedIds.Reset();
    // Should find six landmarks belonging to category "museum",
    User::LeaveIfError(expectedIds.Append(22));
    User::LeaveIfError(expectedIds.Append(84));
    User::LeaveIfError(expectedIds.Append(85));
    User::LeaveIfError(expectedIds.Append(86));
    User::LeaveIfError(expectedIds.Append(87));
    User::LeaveIfError(expectedIds.Append(89));
    
    SearchL(composite, EAsynchronous, EFalse, ESortNone);
    ValidateSearchResultsL(expectedIds, ESortNone);

	CleanupStack::PopAndDestroy(&expectedIds);
    CleanupStack::PopAndDestroy(composite);

    iLog->Log(_L("TestAddAndRemoveL Done"));
    }

// ---------------------------------------------------------
// CPosTp124::ValidateSearchResultsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::ValidateSearchResultsL(
            RIdArray& aExpectedIds, TTp124SortOrder aSortOrder,
            TInt aNrOfExpectedErrors)
    {
    iLog->Log(_L("ValidateSearchResultsL"));
    
    _LIT(KNotFoundErr, "Id %d not found in parsed Lm search results");
    _LIT(KNotFoundErr2, "Id %d not found in search result from displaydata");
    _LIT(KNrOfMatchError, "Nr of matches from DisplayData is not correct");
    _LIT(KNrOfMatchError2, "Expected matches is not correct");
    
    TBuf<100> buf;

    TInt nrOfMatches = 0;
    // list of databases to search in
    CDesCArray* dbList = new (ELeave) CDesCArrayFlat(5);
    CleanupStack::PushL(dbList);
    dbList->AppendL(KLMDB1);
    dbList->AppendL(KLMDB2);
    dbList->AppendL(KLMDB3);
    dbList->AppendL(KLMDB4);
    dbList->AppendL(KLMDB5);
    TInt dbListCount = dbList->Count();
    for (TInt j = 0; j < dbListCount; j++)
        {
    //    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL((*iDatabases)[j]);
    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL((*dbList)[j]);
        CleanupStack::PushL(db);
        CPosLmItemIterator* iter = iMultiDbSearch->MatchIteratorL(j);
        CleanupStack::PushL(iter);
        
        nrOfMatches += iter->NumOfItemsL();
        TPosLmItemId id = iter->NextL();
        while (id != KPosLmNullItemId)
            {
            buf.Format(_L("Found idde %d in db %d"), id, j);
            //iLog->Log(buf);
            ValidateIdL(id, j);
            AssertTrueSecL(aExpectedIds.Find(id) != KErrNotFound, KNotFoundErr, id);
            
            CPosLandmark* lm1 = db->ReadLandmarkLC(id);
            id = iter->NextL();
            // Check sort order in every iterator
            if (aSortOrder != ESortNone && aSortOrder != ESortDistance && id != KPosLmNullItemId)
                {
                CPosLandmark* lm2 = db->ReadLandmarkLC(id);
                TPtrC name1, name2;
                lm1->GetLandmarkName(name1);
                lm2->GetLandmarkName(name2);

                if (aSortOrder == ESortAscending)
                    {                        
                    AssertTrueSecL(name1.CompareC(name2) <= 0, _L("Not sorted ascending as expected"));
                    }
                else if (aSortOrder == ESortDescending)
                    {
                    AssertTrueSecL(name1.CompareC(name2) >= 0, _L("Not sorted descending as expected"));
                    }
                CleanupStack::PopAndDestroy(lm2);
                }
            CleanupStack::PopAndDestroy(lm1);
            }
            
		// Now check that iterator is sorted on distance
        if (aSortOrder == ESortDistance)
			{
			iter->Reset();
			RArray<TPosLmItemId> result;
        	CleanupClosePushL(result);
        	
        	if (iter->NumOfItemsL() > 0)
        		{ 
        		iter->GetItemIdsL(result,0, iter->NumOfItemsL());
				LandmarksSortL(result, aSortOrder);
				TPosLmItemId idde = iter->NextL();
				TInt index=0;
		    	while (idde != KPosLmNullItemId)
		        	{
		        	iLog->Log(_L("##### Check iterator sorted on distance"));
		        	AssertTrueSecL(idde == result[index], _L("Iterator is not sorted on distance"));
		        	index++;
		        	idde = iter->NextL();
		        	}
				}
			CleanupStack::PopAndDestroy(&result);
			}
			
		CleanupStack::PopAndDestroy(iter);            
        CleanupStack::PopAndDestroy(db);
        }
        
    AssertTrueSecL(nrOfMatches == iMultiDbSearch->TotalNumOfMatches(), _L("Total nr of matches doesn't match sum of matches in iterators"));
    AssertTrueSecL(NrOfSearchErrors(iMultiDbSearch) == aNrOfExpectedErrors, _L("Found search errors!"));
    AssertTrueSecL(nrOfMatches == iDisplayData->Count(), KNrOfMatchError);
    AssertTrueSecL(aExpectedIds.Count() == iDisplayData->Count(), KNrOfMatchError2);

    TInt itemId = iDisplayData->NewItemIndex();
	while (itemId != KPosLmNoNewItems)
		{
		CPosLmDisplayItem& item1 = iDisplayData->DisplayItem(itemId);
	    TInt lmId = item1.Landmark().LandmarkId();
        TPtrC name1;
    	item1.Landmark().GetLandmarkName(name1);
    	TInt dbIndex = item1.DatabaseIndex();
    	// Validate lm from correct db
    	ValidateIdL(lmId, dbIndex);

    	buf.Format(_L("LM id: %d"), lmId);
    	//iLog->Log(buf);
	    AssertTrueSecL(aExpectedIds.Find(lmId) != KErrNotFound, KNotFoundErr2, lmId);
	    itemId = iDisplayData->NewItemIndex();
	    if (itemId != KPosLmNoNewItems)
	    	{
		    CPosLmDisplayItem& item2 = iDisplayData->DisplayItem(itemId);
		    lmId = item2.Landmark().LandmarkId();
		    TPtrC name2;
		    lmId = item2.Landmark().LandmarkId();
		    dbIndex = item2.DatabaseIndex();
    		// Validate lm from correct db
    		ValidateIdL(lmId, dbIndex);
    		
			item2.Landmark().GetLandmarkName(name2);
		    AssertTrueSecL(aExpectedIds.Find(lmId) != KErrNotFound, KNotFoundErr2, lmId);
		    
		    // Check sort order for display data items
			if (aSortOrder == ESortAscending)
			    {                        
			    AssertTrueSecL(name1.CompareC(name2) <= 0, _L("Not sorted ascending as expected"));
			    }
			else if (aSortOrder == ESortDescending)
			    {
			    AssertTrueSecL(name1.CompareC(name2) >= 0, _L("Not sorted descending as expected"));
			    }
		    }
	    }
	    
	    if (aSortOrder == ESortDistance)
			{
			iLog->Log(_L("##### Check sorted on distance"));
			LandmarksSortL(aExpectedIds, aSortOrder);
			for (TInt i=0;i<aExpectedIds.Count();i++)
				{
				CPosLmDisplayItem& item1 = iDisplayData->DisplayItem(i);
    			TInt lmId = item1.Landmark().LandmarkId();
				if (aExpectedIds[i] != lmId) 
					{
					buf.Format(_L("Incorrect distance sorting Id1 %d Id2 %d"),aExpectedIds[i], lmId );
					//iLog->Log(buf);
					iLog->Log(buf);
    				User::Leave(-1);
					}
				}
			}
	    CleanupStack::PopAndDestroy(dbList);
    }

// ---------------------------------------------------------
// CPosTp124::NrOfSearchErrors
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosTp124::NrOfSearchErrors(CPosLmMultiDbSearch* aMultiDbSearcher)
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
// Tp124InternalLandmarksSortL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//	
TInt Tp124InternalLandmarksSortL(const CPosLandmark& aSource, const CPosLandmark& aTarget)
    {
	// NOTE: Always sorted on distance to TCoordinate iCoord(45.5, 0.5);
    TLocality loc1;
    TLocality loc2;
        
    aSource.GetPosition(loc1);
    aTarget.GetPosition(loc2);
    TReal32 distance1, distance2;
    // Not nice, ugly hardcoded
    TCoordinate iCoord(45.5, 0.5);
    loc1.Distance(iCoord, distance1);
    loc2.Distance(iCoord, distance2);
    
    iCoord.Distance(loc1, distance1);
    iCoord.Distance(loc2, distance2);
    
    TInt res = distance1-distance2;
    
    return res;
    }
    	
// ---------------------------------------------------------
// CPosTp124::LandmarksSortL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::LandmarksSortL(RArray<TPosLmItemId>& aArray, TTp124SortOrder aSortOrder)
    {
    iLog->Log(_L("LandmarksSortL"));
    
    RPointerArray<CPosLandmark> landmarks;
    CleanupClosePushL(landmarks);
    
    for (TInt i=0; i<aArray.Count(); i++)
        {
        CPosLandmark* lm = iDatabase->ReadLandmarkLC(aArray[i]);
        landmarks.Append(lm);
        CleanupStack::Pop(lm);
        }

	// Sort on distance, possible to add a ascending and descending sort here as well
	// if needed
    TLinearOrder<CPosLandmark> order(Tp124InternalLandmarksSortL);
    landmarks.Sort(order);
    aArray.Reset();
	 if (aSortOrder == ESortDistance)
    	{
	    for (TInt j=0; j<landmarks.Count(); j++)
        	{
        	aArray.Append(landmarks[j]->LandmarkId());
        	}
    	}

		
    landmarks.ResetAndDestroy();
    CleanupStack::PopAndDestroy(&landmarks);
    }

// ---------------------------------------------------------
// CPosTp124::CancelTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::CancelTestL(const TBool& aCancelInCallback)
    {
    iLog->Log(_L("CancelTestL"));
        
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
    
    CCompositeSearchResult* res = CCompositeSearchResult::NewL(iLog);
    CleanupStack::PushL(res);
    
    // Get first composite search from lmDbCompositeSearchResult.txt
    res->GetCompositeSearchResultsL(*composite);
    
	// Cancel in callback, cancel in every progress step phase
	// when progress has increased with 0.09
    if (aCancelInCallback)
    	{
    	iLog->Log(_L("Cancel in Callback"));
	    TReal32 progress(0);
	    while (progress < 1.0)
	        {
	        RunAsyncOperationAndCancelInCallbackLD(
	            iMultiDbSearch->StartLandmarkSearchL(
	                *composite, 
	                EFalse),
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
    	iLog->Log(_L("Cancel immediately"));
    	RunAsyncOperationAndCancelLD(
	            iMultiDbSearch->StartLandmarkSearchL(
	                *composite, 
	                EFalse));
	            
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
	    		
    CleanupStack::PopAndDestroy(res);
    CleanupStack::PopAndDestroy(composite);
    }
    
// ---------------------------------------------------------
// CPosTp124::CompositeStressTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::CompositeStressTestL(TInt aStartNr, TInt aTotalNr)
	{
	iLog->Log(_L("CompositeStressTestL"));
	CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
	TInt i=0;
	
	for (i=0;i<aTotalNr;i++)
		{
		CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();    
    	textCriteria->SetTextL(_L("*"));
    	
    	User::LeaveIfError(composite->AddArgument(textCriteria));
    	CleanupStack::Pop(textCriteria);
	
		if (i>aStartNr)
			{
			SearchL(composite, EAsynchronous, EFalse, ESortAscending);
			TBuf<100> buf;
			buf.Format(_L("Found %d nr of matches"), iDisplayData->Count());
			iLog->Log(buf);
			}
		}
	
	TInt antal = composite->NumOfArguments();
	if (antal != aTotalNr) iLog->Log(_L("nr of composite arguments is incorrect"));
	
	TBuf<100> buf;
	buf.Format(_L("Found %d nr of matches"), iDisplayData->Count());
	iLog->Log(buf);

	TInt itemId = iDisplayData->NewItemIndex();
	while (itemId != KPosLmNoNewItems)
		{
		CPosLmDisplayItem& item1 = iDisplayData->DisplayItem(itemId);
	    TInt lmId = item1.Landmark().LandmarkId();
        TPtrC name1;
    	item1.Landmark().GetLandmarkName(name1);
    	TInt dbIndex = item1.DatabaseIndex();
    	// Validate lm from correct db
    	ValidateIdL(lmId, dbIndex);
    	buf.Format(_L("LM id: %d from DB %d"), lmId, dbIndex);
    	iLog->Log(buf);
    	itemId = iDisplayData->NewItemIndex();
		}
		
	// 105 is the total number of landmarks in the five dbs
	if (iDisplayData->Count() != 105) 
	{
		iLog->Log(_L("Wrong number of matches found"));
    	User::Leave(-1);
	}
	
	CleanupStack::PopAndDestroy(composite);
	}
    
// ---------------------------------------------------------
// CPosTp124::TestWithNoPreviousResultL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::TestWithNoPreviousResultL()
    {
    iLog->Log(_L("TestWithNoPreviousResultL"));

    if (iMultiDbSearch) iMultiDbSearch->UnsetDisplayData();
    delete iDisplayData;
    iDisplayData = NULL;
    delete iMultiDbSearch;
    iMultiDbSearch = CPosLmMultiDbSearch::NewL(*iDatabases);
    
    iDisplayData = CPosLmDisplayData::NewL();
	iMultiDbSearch->SetDisplayData(*iDisplayData);
    
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
   
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();    
    textCriteria->SetTextL(_L("*"));
    
    User::LeaveIfError(composite->AddArgument(textCriteria));
    CleanupStack::Pop(textCriteria);
    
	// No previous result-> should leave with KErrArgument
    TRAPD(err, SearchL(composite, EAsynchronous, ETrue, ESortAscending));
    if (err != KErrArgument) 
    {
    	iLog->Log(_L("It should not be possible to search when no previous result has been retrieved"));
    	User::Leave(err);
    }
    
    CleanupStack::PopAndDestroy(composite);
    }

// ---------------------------------------------------------
// CPosTp124::TestWithIdListAndCompositeL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::TestWithIdListAndCompositeL()
	{
	iLog->Log(_L("TestWithIdListAndCompositeL"));
	
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

    // First create a textcriteria
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();    
    textCriteria->SetTextL(_L("Nokia*"));
    User::LeaveIfError(composite->AddArgument(textCriteria));
    CleanupStack::Pop(textCriteria);

    // Then create an IdList
    CPosLmIdListCriteria* idList = CPosLmIdListCriteria::NewLC();
	RIdArray ids;
    CleanupClosePushL(ids);
    User::LeaveIfError(ids.Append(63));
    User::LeaveIfError(ids.Append(33));
    idList->SetLandmarkIdsL(ids);
    CleanupStack::PopAndDestroy(&ids);
    
    User::LeaveIfError(composite->AddArgument(idList));
    CleanupStack::Pop(idList);

	// It is not possible to use IdList in combination with composite in multiple db search    
    TRAPD(err, SearchL(composite, EAsynchronous, EFalse, ESortAscending));
    if (err != KErrArgument)
    {
    	iLog->Log(_L("It should not be possible to use CPosLmIdListCriteria in a composite search"));
    	User::Leave(err);

    }

    CleanupStack::PopAndDestroy(composite);
	iDisplayData->Reset();
	}

// ---------------------------------------------------------
// CPosTp124::TestCategoryCompositeSearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::TestCategoryCompositeSearchL()
    {
    iLog->Log(_L("TestCategoryCompositeSearchL"));

    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

	// It should not be possible to add a CPosLmCatNameCriteria to composite
    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
    nameCriteria->SetSearchPatternL(_L("*"));
    TInt err = composite->AddArgument(nameCriteria);
    if (err != KErrNotSupported) 
    {
    	iLog->Log(_L("It should not be possible to use CPosLmIdListCriteria in a composite search"));
    	User::Leave(err);

    }
    CleanupStack::PopAndDestroy(nameCriteria);

    // Test once more
    CPosLmCatNameCriteria* nameCriteria2 = CPosLmCatNameCriteria::NewLC();
    nameCriteria2->SetSearchPatternL(_L("*kontor"));
    err = composite->AddArgument(nameCriteria2);
    if (err != KErrNotSupported)
    {
    	iLog->Log(_L("It should not be possible to use CPosLmIdListCriteria in a composite search"));
    	User::Leave(err);

    }
    CleanupStack::PopAndDestroy(nameCriteria2);
        
    CleanupStack::PopAndDestroy(composite);
    }
    
// ---------------------------------------------------------
// CPosTp124::TestCompositeInCompositeL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::TestCompositeInCompositeL()
    {
    iLog->Log(_L("TestCompositeInCompositeL"));
    CPosLmCompositeCriteria* composite = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

    // First create a textcriteria
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    
    textCriteria->SetTextL(_L("*"));
   
    User::LeaveIfError(composite->AddArgument(textCriteria));
    CleanupStack::Pop(textCriteria);

    CPosLmAreaCriteria* areaCriteria = CPosLmAreaCriteria::NewLC(-48, 5, -5, 20);
                
    User::LeaveIfError(composite->AddArgument(areaCriteria));
    CleanupStack::Pop(areaCriteria);

    //Create composite object that should be used as argument to the first composite object
    CPosLmCompositeCriteria* composite2 = CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);

    CPosLmTextCriteria* textCriteria2 = CPosLmTextCriteria::NewLC();
    textCriteria2->SetTextL(_L("TE,*"));
    
    User::LeaveIfError(composite2->AddArgument(textCriteria2));
    CleanupStack::Pop(textCriteria2);

    //User::LeaveIfError(composite->AddArgument(composite2));
    TInt res = composite->AddArgument(composite2);
    if (res != KErrNotSupported) 
    {
    	iLog->Log(_L("It should not be possible to add to a composite "));
    	User::Leave(-1);

    }

    CleanupStack::PopAndDestroy(composite2);
    
    
    CleanupStack::PopAndDestroy(composite);

    iLog->Log(_L("TestCompositeInCompositeL Done"));
    
    }
    
// ---------------------------------------------------------
// CPosTp124::PrintCompositeInfo
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp124::PrintCompositeInfo(CPosLmCompositeCriteria* aComposite)
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
