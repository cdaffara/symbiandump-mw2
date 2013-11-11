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
#include "FT_CPosTp54.h"
#include "FT_LandmarkConstants.h"
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmOperation.h>
#include <utfw.h>
#include <e32math.h>


#include <LbsPosition.h>

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp54::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp54::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP54 - Sorted DB Iterator");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp54::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp54::CloseTest()
    {
    iLog->Put(_L("CloseTest"));
    delete iDatabase;
    iDatabase = NULL;
    }

// ---------------------------------------------------------
// CPosTp54::InitTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp54::InitTestL()
    {
    }

// ---------------------------------------------------------
// CPosTp54::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp54::StartL()
    {
    // All times in this test are measured in milliseconds
    TBool wholeTestSuccessful = ETrue;
    TBool subTestSuccessful;
    TBool isSorted = ETrue;
    TBool isSynch = ETrue;
    TInt nrOfLms = 1000;
    TInt64 execTime;
    TInt64 maxExecTime;
/*
Use function below for creating landmark dbs with 1000 and 5000 landmarks
Note: Only name and position is set

/*
CreateLargeDbs(1000);
CleanupStack::PopAndDestroy();

CreateLargeDbs(5000);
CleanupStack::PopAndDestroy();
*/

// Measure 1000 sorted landmarks synchronously
    iLog->Put(_L("\nTesting retrieving 1000 sorted landmarks using LandmarkIteratorL()"));
    maxExecTime = 5000;
    RecreateDatabaseL(K1000LandmarksDb);
    execTime = ExecutionTimeSynchL(isSorted);
    subTestSuccessful = VerifyResult(execTime, nrOfLms, isSorted, maxExecTime, isSynch);
    if (wholeTestSuccessful)
        {
// We are not allowed to leave because of this. Error report ESLI-653HBG was ignored
//        wholeTestSuccessful = subTestSuccessful;
        }

// Measure 1000 unsorted landmarks synchronously
    iLog->Put(_L("\nTesting retrieving 1000 unsorted landmarks using LandmarkIteratorL()"));
    maxExecTime = 1000;
    RecreateDatabaseL(K1000LandmarksDb);
    execTime = ExecutionTimeSynchL(!isSorted);
    subTestSuccessful = VerifyResult(execTime, nrOfLms, !isSorted, maxExecTime, isSynch);
    if (wholeTestSuccessful)
        {
// We are not allowed to leave because of this. Error report ESLI-653HBG was ignored
//        wholeTestSuccessful = subTestSuccessful;
        }

// Measure 5000 sorted landmarks synchronously
    iLog->Put(_L("\nTesting retrieving 5000 sorted landmarks using LandmarkIteratorL()"));
    nrOfLms = 5000;
    maxExecTime = 10000;
    RecreateDatabaseL(K5000LandmarksDb);
    execTime = ExecutionTimeSynchL(isSorted);
    subTestSuccessful = VerifyResult(execTime, nrOfLms, isSorted, maxExecTime, isSynch);
    if (wholeTestSuccessful)
        {
// We are not allowed to leave because of this. Error report ESLI-653HBG was ignored
//        wholeTestSuccessful = subTestSuccessful;
        }

// Measure 5000 unsorted landmarks synchronously
    iLog->Put(_L("\nTesting retrieving 5000 unsorted landmarks using LandmarkIteratorL()"));
    maxExecTime = 5000;
    RecreateDatabaseL(K5000LandmarksDb);
    execTime = ExecutionTimeSynchL(!isSorted);
    subTestSuccessful = VerifyResult(execTime, nrOfLms, !isSorted, maxExecTime, isSynch);
    if (wholeTestSuccessful)
        {
// We are not allowed to leave because of this. Error report ESLI-653HBG was ignored
//        wholeTestSuccessful = subTestSuccessful;
        }

// Measure 1000 sorted landmarks asynchronously
    iLog->Put(_L("\nTesting retrieving 1000 sorted landmarks using the search API"));
    nrOfLms = 1000;
    maxExecTime = 5000;
    RecreateDatabaseL(K1000LandmarksDb);
    execTime = ExecutionTimeAsynchL(isSorted);
    subTestSuccessful = VerifyResult(execTime, nrOfLms, isSorted, maxExecTime, !isSynch);
    
    if (wholeTestSuccessful)
        {
        wholeTestSuccessful = subTestSuccessful;
        }

// Measure 1000 unsorted landmarks asynchronously
    iLog->Put(_L("\nTesting retrieving 1000 unsorted landmarks using the search API"));
    maxExecTime = 1000;
    RecreateDatabaseL(K1000LandmarksDb);
    execTime = ExecutionTimeAsynchL(!isSorted);
    subTestSuccessful = VerifyResult(execTime, nrOfLms, !isSorted, maxExecTime, !isSynch);
    if (wholeTestSuccessful)
        {
        wholeTestSuccessful = subTestSuccessful;
        }

// Measure 5000 sorted landmarks asynchronously
    iLog->Put(_L("\nTesting retrieving 5000 sorted landmarks using the search API"));
    nrOfLms = 5000;
    maxExecTime = 10000;
    RecreateDatabaseL(K5000LandmarksDb);
    execTime = ExecutionTimeAsynchL(isSorted);
    subTestSuccessful = VerifyResult(execTime, nrOfLms, isSorted, maxExecTime, !isSynch);
    if (wholeTestSuccessful)
        {
        wholeTestSuccessful = subTestSuccessful;
        }

// Measure 5000 unsorted landmarks asynchronously
    iLog->Put(_L("\nTesting retrieving 5000 unsorted landmarks using the search API"));
    maxExecTime = 5000;
    RecreateDatabaseL(K5000LandmarksDb);
    execTime = ExecutionTimeAsynchL(!isSorted);
    subTestSuccessful = VerifyResult(execTime, nrOfLms, !isSorted, maxExecTime, !isSynch);
    if (wholeTestSuccessful)
        {
        wholeTestSuccessful = subTestSuccessful;
        }

    if (!wholeTestSuccessful)
        {
        User::Leave(KErrGeneral);
        }
    }

// ---------------------------------------------------------
// CPosTp54::ExecutionTimeSynchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt64 CPosTp54::ExecutionTimeSynchL(TBool aIsSorted)
    {
    TTime startTime;
    TTime stopTime;
    CPosLmItemIterator* iterator = NULL;

    if (aIsSorted)
        {
        TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);

        // The method to measure
        startTime.UniversalTime();
        iterator = iDatabase->LandmarkIteratorL(sortPref);
        stopTime.UniversalTime();
        }
    else 
        {
        // The method to measure
        startTime.UniversalTime();
        iterator = iDatabase->LandmarkIteratorL();
        stopTime.UniversalTime();
        }
    delete iterator;

    TTimeIntervalMicroSeconds execTime = 
        stopTime.MicroSecondsFrom(startTime);
    return execTime.Int64() / 1000; // return execution time in milliseconds
    }

// ---------------------------------------------------------
// CPosTp54::ExecutionTimeAsynchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt64 CPosTp54::ExecutionTimeAsynchL(TBool aIsSorted)
    {
    // Create a landmark searcher
    CPosLandmarkSearch* lmSearcher = CPosLandmarkSearch::NewL(*iDatabase);
    CleanupStack::PushL(lmSearcher);

    // Create search criteria, search by name.
    CPosLmTextCriteria* searchCriteria = CPosLmTextCriteria::NewLC();
    searchCriteria->SetTextL(_L("*"));
    searchCriteria->SetAttributesToSearch(CPosLandmark::ELandmarkName);

    CPosLmOperation* searchOp = NULL;
    if (aIsSorted)
        {
        // Create search operation, sort ascending by name
        TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
        searchOp = lmSearcher->StartLandmarkSearchL(*searchCriteria, sortPref);
        }
    else 
        {
        // Create search operation, without sorting
        searchOp = lmSearcher->StartLandmarkSearchL(*searchCriteria);
        }

    TInt res(0);
    TTimeIntervalMicroSeconds longestExecTimeSoFar = ExecuteLmOp(searchOp, res);
    AssertTrueSecL(res == KErrNone, _L("Searching did not result in KErrNone but in %d."), res);

    // Get the iterator
    TTime startTime, stopTime;
    startTime.UniversalTime();
    CPosLmItemIterator* iterator = lmSearcher->MatchIteratorL();
    stopTime.UniversalTime();
    delete iterator;

    TTimeIntervalMicroSeconds execTime = stopTime.MicroSecondsFrom(startTime);
    if (execTime > longestExecTimeSoFar)
        {
        longestExecTimeSoFar = execTime;
        }
    
    CleanupStack::PopAndDestroy(2, lmSearcher);
    return longestExecTimeSoFar.Int64() / 1000; // return execution time in milliseconds
    }

// ---------------------------------------------------------
// CPosTp54::VerifyResult
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp54::VerifyResult(
    TInt64 aExecTime, 
    TInt aNrOfLandmarks, 
    TBool aSorted, 
    TInt64 aLimit, 
    TBool aSynch)
    {
    _LIT(KOups, "Ouppps! ");
    _LIT(KSynch, "%SRetrieving %d %S landmarks using %S took %d milliseconds");
    _LIT(KAsynch, "%SLongest execution time in RunL() took %d milliseconds when retrieving %d %S landmarks using %S.");
    _LIT(KSorted, "sorted");
    _LIT(KUnSorted, "UNsorted");
    _LIT(KSynchIterator, "CPosLandmarkDatabse::LandmarkIteratorL()");
    _LIT(KAsynchSearch, "LM search API");
    TBool testSucessful;
    TInt time = aExecTime;
    // Present result
    TBuf<200> buf;

    if (aExecTime > aLimit)
        {           
        aSorted    
            ? aSynch
                ? buf.Format(KSynch, &KOups, aNrOfLandmarks, &KSorted, &KSynchIterator, time)
                : buf.Format(KAsynch, &KOups, time, aNrOfLandmarks, &KSorted, &KAsynchSearch)
            : aSynch
                ? buf.Format(KSynch, &KOups, aNrOfLandmarks, &KUnSorted, &KSynchIterator, time)
                : buf.Format(KAsynch, &KOups, time, aNrOfLandmarks, &KUnSorted, &KAsynchSearch);

        iLog->PutError(buf);
        testSucessful = EFalse;
        }
    else
        {
        aSorted 
            ? aSynch
                // XXX ? buf.Format(KSynch, &KNullDesC, aNrOfLandmarks, &KSorted, &KSynchIterator, aExecTime.Low())
                ? buf.Format(KSynch, &KNullDesC, aNrOfLandmarks, &KSorted, &KSynchIterator, time)
                //XXX : buf.Format(KAsynch, &KNullDesC, aExecTime.Low(), aNrOfLandmarks, &KSorted, &KAsynchSearch)
                : buf.Format(KAsynch, &KNullDesC, time, aNrOfLandmarks, &KSorted, &KAsynchSearch)
            : aSynch
                //? buf.Format(KSynch, &KNullDesC, aNrOfLandmarks, &KUnSorted, &KSynchIterator, aExecTime.Low())
                //: buf.Format(KAsynch, &KNullDesC, aExecTime.Low(), aNrOfLandmarks, &KUnSorted, &KAsynchSearch);
                ? buf.Format(KSynch, &KNullDesC, aNrOfLandmarks, &KUnSorted, &KSynchIterator, time)
                : buf.Format(KAsynch, &KNullDesC, time, aNrOfLandmarks, &KUnSorted, &KAsynchSearch);

        iLog->Put(buf);
        testSucessful = ETrue;
        }

    return testSucessful;
    }

// ---------------------------------------------------------
// CPosTp54::TestIteratorSynchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt64 CPosTp54::TestIteratorSynchL(const TDesC& aDatabase, TBool aIsSorted)
    {
    RecreateDatabaseL(aDatabase);

    return ExecutionTimeSynchL(aIsSorted);
    }

// ---------------------------------------------------------
// CPosTp54::TestIteratorAsynchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt64 CPosTp54::TestIteratorAsynchL(const TDesC& aDatabase, TBool aIsSorted)
    {
    RecreateDatabaseL(aDatabase);

    return ExecutionTimeAsynchL(aIsSorted);
    }

// ---------------------------------------------------------
// CPosTp54::RecreateDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp54::RecreateDatabaseL(const TDesC& aDatabase)
    {
    delete iDatabase;
    iDatabase = NULL;
    CopyDbFileL(aDatabase);
    iDatabase = CPosLandmarkDatabase::OpenL(KDefaultDb);
    }

// ---------------------------------------------------------
// CPosTp54::ExecuteLmOp
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt64 CPosTp54::ExecuteLmOp(CPosLmOperation* aOp, TInt& aResult)
    {
    TReal32 progress(0);
    TRequestStatus status(KPosLmOperationNotComplete);
    TTime startTime;
    TTime stopTime;
    TTimeIntervalMicroSeconds longestExecTimeSoFar(0);

    while (status.Int() == KPosLmOperationNotComplete)
        {
        startTime.UniversalTime();
        aOp->NextStep(status, progress);
        User::WaitForRequest(status);
        stopTime.UniversalTime();

        TTimeIntervalMicroSeconds execTime = stopTime.MicroSecondsFrom(startTime);
        if (execTime > longestExecTimeSoFar)
            {
            longestExecTimeSoFar = execTime;
            }
        _LIT(KExecTime, "Progress = %d%%. Execution of NextStep took %d µs");
        TBuf<100> buf;
        //buf.Format(KExecTime, (TInt) (progress * 100), execTime.Int64().Low());
        buf.Format(KExecTime, (TInt) (progress * 100), execTime.Int64());
//        iLog->Put(buf);
        }

    aResult = status.Int();
    return longestExecTimeSoFar.Int64();
    }
    
    
// ---------------------------------------------------------
// CPosTp54::CreateLargeDbs
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CPosTp54::CreateLargeDbs(TInt aNrOfItems)
    {
    
    TTime startTime;
    TTime stopTime;
    
    iLog->Put(_L("RemoveAllLmDatabasesL();"));
    RemoveAllLmDatabasesL();
    startTime.UniversalTime();
    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(db);
    if (db->IsInitializingNeeded())
        {
	    iLog->Put(_L("IsInitializingNeeded()"));
        ExecuteAndDeleteLD(db->InitializeL());
        }
        
        CPosLmCategoryManager* catManager = CPosLmCategoryManager::NewL(*db);
        CleanupStack::PushL(catManager);
        TInt j = 0;

        while (j < aNrOfItems)
            {
            TTime seedTime;
            seedTime.UniversalTime();
            
            TInt64 seed = seedTime.Int64();
            TBuf<150> name;
            TReal randNr = Math::FRand(seed);
            
            TUint bokstav1 = (randNr*25)+65;
            randNr = Math::FRand(seed);
            TUint bokstav2 = (randNr*25)+65;
            
            //Create a landmark name that contains of two random characters+"NearestSearch"+iCounter
            name.Format(_L("%c%cLandmark Nr %d"),bokstav1, bokstav2, j);
            CPosLandmark* landmark = CPosLandmark::NewLC();

            landmark->SetLandmarkNameL(name);
            
            /*
            HBufC* desc = HBufC::NewLC(4095);
            TPtr descPtr = desc->Des();
            for (TInt k = 0; k < 16; k++)
                {
                descPtr.Append(name);
                }
            descPtr = descPtr.Left(4095);
            landmark->SetLandmarkDescriptionL(descPtr);
            CleanupStack::PopAndDestroy(desc);
            
            */
/*
            // Update coverage radius
            TReal32 radius = 345345345;
            landmark->SetCoverageRadius(radius);
            // Update street
            landmark->SetPositionFieldL(EPositionFieldStreet, name);
            // Update postal code
            landmark->SetPositionFieldL(EPositionFieldPostalCode, name);
            // Update city
            landmark->SetPositionFieldL(EPositionFieldCity, name);

            // Update country
            landmark->SetPositionFieldL(EPositionFieldCountry, name);
*/
            TReal latitude(123), longitude(321);
            TReal32 altitude(54), horAcc(1), verAcc(12);
            TCoordinate coordinate(latitude, longitude, altitude);
            TLocality newLocation(coordinate, horAcc, verAcc);
            landmark->SetPositionL(newLocation);

            db->AddLandmarkL(*landmark);
            CleanupStack::PopAndDestroy(landmark);

            /*
            if (j<100)
                {
                // Add category
                CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();

                _LIT(KCatName, "Category nr %d");
                TBuf<100> nameCategory;
                nameCategory.Format(KCatName, j);
                category->SetCategoryNameL(nameCategory);
                catManager->AddCategoryL(*category);

                CleanupStack::PopAndDestroy(category);
                }
                */
            j++;
            }
        ExecuteAndDeleteLD(db->CompactL());
        stopTime.UniversalTime();
        CleanupStack::PopAndDestroy(2, db);
        
        TTimeIntervalMicroSeconds execTime = stopTime.MicroSecondsFrom(startTime);
        
        TTimeIntervalMicroSeconds executionTime = 
        stopTime.MicroSecondsFrom(startTime);

        TInt resse = executionTime.Int64()/1000000;
        TBuf<100> buf;
        buf.Format(_L("Create %d nr of landmarks took %d seconds"), aNrOfItems, resse);
        iLog->Put(buf);

    }
// End of file

