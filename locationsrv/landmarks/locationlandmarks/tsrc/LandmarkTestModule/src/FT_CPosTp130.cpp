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
#include "FT_CPosTp130.h"
#include "FT_LandmarkConstants.h"
#include <EPos_CPosLmMultiDbSearch.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_CPosLmItemIterator.h>
#include <EPos_TPosLmSortPref.h>
#include <EPos_CPosLmCatNameCriteria.h>
#include "badesca.h"

//  CONSTANTS
const TInt KNrOfDatabases = 5;
_LIT(KSearchPattern, "*e*");  

class CNamedLmItem : public CBase
	{
	public:
		~CNamedLmItem() { delete iName; };
		TPosLmItemId iId;
		HBufC* iName;
		
		static TInt CompareByName( const CNamedLmItem& aLeft, const CNamedLmItem& aRight )
			{
			return aLeft.iName->CompareC( *aRight.iName );
			};
	};

// ================= LOCAL FUNCTIONS =======================

void ResetAndDestroy(TAny* aAny)
    {
    RPointerArray<CPosLmItemIterator>* pointerArray = 
        reinterpret_cast <RPointerArray<CPosLmItemIterator>*> (aAny);
    pointerArray->ResetAndDestroy();
    }

void ResetAndDestroyNamedLmItem(TAny* aAny)
    {
    RPointerArray<CNamedLmItem>* pointerArray = 
        reinterpret_cast <RPointerArray<CNamedLmItem>*> (aAny);
    pointerArray->ResetAndDestroy();
    }

// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CPosTp130::InitTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp130::InitTestL()
    {
    CLandmarkTestProcedureBase::InitTestL();
    
    // PrepareDatabases
    const TInt KNrOfDatabases = 5;
    RemoveAllLmDatabasesL();
    // Get the list of Dbs remaining after removing the possible ones.
    CPosLmDatabaseManager* dbMan = CPosLmDatabaseManager::NewL();
        CleanupStack::PushL(dbMan);
        iDbUris = dbMan->ListDatabasesLC();
        iInitialDbCount = iDbUris->Count();
        CleanupStack::PopAndDestroy(iDbUris);
    CopyTestDbFileL(KDb20);
    CopyTestDbFileL(KDb40);
    CopyTestDbFileL(KDb60);
    CopyTestDbFileL(KDb80);
    CopyTestDbFileL(KDb105);
    
    // List databases
    
    iDbUris = dbMan->ListDatabasesLC();
    CleanupStack::Pop(iDbUris);
    AssertTrueSecL(iDbUris->Count() == iInitialDbCount+KNrOfDatabases, _L("Wrong number of test databases!"));
    CleanupStack::PopAndDestroy(dbMan);
    
    // Get only the list of dburi in which this test case operations need to be performed
        TInt dbUriCount = iDbUris->Count();
        
        for ( TInt i= 0;i < dbUriCount;i++)
            {
        iLog->Log((*iDbUris)[i]);
        // Action to be performed only on the newly added dbs in this test case, therefore remove any other 
        // db apart from the ones added in this test case from the iDbUris list
            TPtrC dbUri((*iDbUris)[i]);
                if ( (dbUri != KDb20Uri) && (dbUri != KDb40Uri) && (dbUri != KDb60Uri) &&
                       ( dbUri != KDb80Uri) && (dbUri != KDb105Uri) )
                    {
                iDbUris->Delete(i);
                    }

            }
    
    iDbSearcher = CPosLmMultiDbSearch::NewL(*iDbUris);
    
    // Initialize expected landmark search result
    
    for ( TInt i = 0; i < iDbUris->Count(); i++ )
    	{
        RIdArray* lmArray = new (ELeave) RIdArray;
        InitExpectedResultsL( (*iDbUris)[i], *lmArray, ETrue );
        iExpectedLmResult.AppendL( lmArray );

        RIdArray* catArray = new (ELeave) RIdArray;
        InitExpectedResultsL( (*iDbUris)[i], *catArray, EFalse );
        iExpectedCatResult.AppendL( catArray );
    	}

    }
    
// ---------------------------------------------------------
// CPosTp130::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp130::CloseTest()
    {
    delete iDbUris;
    delete iDbSearcher;
    for (TInt i = 0; i < iExpectedLmResult.Count(); i++)
        {
        iExpectedLmResult[i]->Close();
        iExpectedCatResult[i]->Close();
        }
    iExpectedLmResult.ResetAndDestroy();
    iExpectedCatResult.ResetAndDestroy();
    }
    
// ---------------------------------------------------------
// CPosTp130::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp130::StartL()
    {
    // nrOfExpectedMatches contains the number of matches for each db when search pattern is *e*
    RArray<TInt> nrOfExpectedMatches;
    CleanupClosePushL(nrOfExpectedMatches);
    for (TInt i = 0; i < KNrOfDatabases; i++)
        {
        nrOfExpectedMatches.AppendL((*iExpectedLmResult[i]).Count());
        }

// 1. Search for landmarks
    iLog->Log(_L("1"));
    SearchForLandmarksL();    
    
// 2-9. Verify result
    iLog->Log(_L("2-9"));
    VerifyIteratorsL(nrOfExpectedMatches, ETrue);
    
    iLog->Log(_L("10.true"));
// 10. Test iterators dynamically
    TestMatchIteratorL(ETrue);
    iLog->Log(_L("10.false"));
    TestMatchIteratorL(EFalse);
    
    iLog->Log(_L("11"));
// 11. Test error codes
    TestErrorCodesL();
    
    iLog->Log(_L("12"));
// 12. Check iterators from matching categories
    nrOfExpectedMatches.Reset();
    for (TInt i = 0; i < KNrOfDatabases; i++)
        {
        nrOfExpectedMatches.AppendL((*iExpectedCatResult[i]).Count());
        }
    SearchForCategoriesL();
    VerifyIteratorsL(nrOfExpectedMatches, EFalse);
    CleanupStack::PopAndDestroy(&nrOfExpectedMatches);
    }

// ---------------------------------------------------------
// CPosTp130::SearchForLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp130::SearchForLandmarksL()
    {
    // Search for landmarks containing a specific string in name
    CPosLmTextCriteria* criteria = CPosLmTextCriteria::NewLC();
    criteria->SetTextL(KSearchPattern);
    criteria->SetAttributesToSearch(CPosLandmark::ELandmarkName);

    // Do the search    
    ExecuteAndDeleteLD(iDbSearcher->StartLandmarkSearchL(*criteria));
    
    // Check errors
    TUint nrOfSearchErrors = iDbSearcher->NumOfSearchErrors();
    AssertTrueSecL(nrOfSearchErrors == 0, _L("Found errors during search"));
    
    CleanupStack::PopAndDestroy(criteria);
    }
    
// ---------------------------------------------------------
// CPosTp130::SearchForCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp130::SearchForCategoriesL()
    {
    // Search for categories containing a specific string in name
    CPosLmCatNameCriteria* criteria = CPosLmCatNameCriteria::NewLC();
    criteria->SetSearchPatternL(KSearchPattern);

    // Do the search    
    ExecuteAndDeleteLD(iDbSearcher->StartCategorySearchL(*criteria, CPosLmCategoryManager::ECategorySortOrderNone));
    
    // Check errors
    TUint nrOfSearchErrors = iDbSearcher->NumOfSearchErrors();
    AssertTrueSecL(nrOfSearchErrors == 0, _L("Found errors during search"));
    
    CleanupStack::PopAndDestroy(criteria);
    }
    
// ---------------------------------------------------------
// CPosTp130::VerifyIteratorsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp130::VerifyIteratorsL(RArray<TInt>& aNrOfExpectedItems, TBool aLm)
    {    
    RPointerArray<CPosLmItemIterator> iterators;
    CleanupClosePushL(iterators);
    CleanupStack::PushL(TCleanupItem(ResetAndDestroy, &iterators));
    
    TInt totalNrOfMatches(0), totalNrOfExpectedMatches(0);
    for (TInt i = 0; i < iDbUris->Count(); i++)
        {
        // 2. Ask for the iterator for each database
        iterators.AppendL(iDbSearcher->MatchIteratorL(i));
        
        // 3. Verify that the number of each iterator equals to the expected result
        AssertTrueSecL(aNrOfExpectedItems[i] == iterators[i]->NumOfItemsL(), _L("aNrOfExpectedItems[i] != iterators[i]->NumOfItemsL()"));
        AssertTrueSecL(aNrOfExpectedItems[i] == iDbSearcher->NumOfMatches(i), _L("aNrOfExpectedItems[i] != iDbSearcher->NumOfMatches(i)"));
        
        totalNrOfExpectedMatches += aNrOfExpectedItems[i];
        totalNrOfMatches += iterators[i]->NumOfItemsL();
        }
        
    // 4. Verify that the total number of matches equals the sum of expected
    AssertTrueSecL(totalNrOfMatches == totalNrOfExpectedMatches, _L("totalNrOfMatches != totalNrOfExpectedMatches"));
    AssertTrueSecL(totalNrOfMatches == iDbSearcher->TotalNumOfMatches(), _L("totalNrOfMatches != iDbSearcher->TotalNumOfMatches()"));
    
    for (TInt dbIndex = 0; dbIndex < iDbUris->Count(); dbIndex++)
        {
        // 5. Ask for the first item in each iterator
        TPosLmItemId id1 = iterators[dbIndex]->NextL();
        
        // 6. Perform an operation on the first item, e.g. read it from db
        CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL((*iDbUris)[dbIndex]);
        CleanupStack::PushL(db);
        if (aLm)
            {            
            CPosLandmark* lm = db->ReadLandmarkLC(id1);
            TPtrC name;
            lm->GetLandmarkName(name);
            AssertTrueSecL(name.MatchC(KSearchPattern) != KErrNotFound, _L("First lm in iterator does not contain \"e\""));
            CleanupStack::PopAndDestroy(lm);
            }
        else
            {
            CPosLmCategoryManager* catMan = CPosLmCategoryManager::NewL(*db);
            CleanupStack::PushL(catMan);
            CPosLandmarkCategory* cat = catMan->ReadCategoryLC(id1);
            TPtrC name;
            cat->GetCategoryName(name);
            AssertTrueSecL(name.MatchC(KSearchPattern) != KErrNotFound, _L("First category in iterator does not contain \"e\""));            
            CleanupStack::PopAndDestroy(2, catMan);
            }
        CleanupStack::PopAndDestroy(db);
        
        RIdArray allIds;
        CleanupClosePushL(allIds);        
        
        // 7. Reset iterator and ask for next item
        iterators[dbIndex]->Reset();
        TPosLmItemId id2 = iterators[dbIndex]->NextL();
        AssertTrueSecL(id1 == id2, _L("id1 != id2"));
        allIds.AppendL(id2);
        
        // 8. Iterate until there are no more items to iterate
        while (id2 != KPosLmNullItemId)
            {
            id2 = iterators[dbIndex]->NextL();
            allIds.AppendL(id2);
            }
        AssertTrueSecL(id2 == KPosLmNullItemId, _L("id2 != KPosLmNullItemId"));
        
        // 9. Ask for sequences of iterator
        RIdArray idArray;
        CleanupClosePushL(idArray);
        TInt nrOfItems = iterators[dbIndex]->NumOfItemsL();
        for (TInt iterIndex = 0; iterIndex < nrOfItems; iterIndex++)
            {
            // Get sequence of the tail of the iterator
            iterators[dbIndex]->GetItemIdsL(idArray, iterIndex, nrOfItems - iterIndex);
            AssertTrueSecL(idArray.Count() == nrOfItems - iterIndex, _L("A: Wrong number of items in id array"));
            for (TInt i = iterIndex; i < nrOfItems - iterIndex; i++)
                {
                id1 = idArray[i - iterIndex];
                id2 = allIds[i];
                AssertTrueSecL(id1 == id2, _L("A: Unexpected id in part of all ids array"));
                }
            
            // Get sequence of the nose of the iterator
            iterators[dbIndex]->GetItemIdsL(idArray, 0, iterIndex);
            AssertTrueSecL(idArray.Count() == iterIndex, _L("B: Wrong number of items in id array"));
            for (TInt j = 0; j < iterIndex; j++)
                {
                id1 = idArray[j];
                id2 = allIds[j];
                AssertTrueSecL(id1 == id2, _L("B: Unexpected id in part of all ids array"));
                }
            }
        CleanupStack::PopAndDestroy(2, &allIds);
        }
        
    CleanupStack::PopAndDestroy(2, &iterators);
    }
       
// ---------------------------------------------------------
// CPosTp130::TestErrorCodesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp130::TestErrorCodesL()
    {
    // No error codes to test
    }

// ---------------------------------------------------------
// CPosTp130::TestMatchIteratorL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp130::TestMatchIteratorL(TBool aSearchLm)
    {    
    CMatchIteratorTester* matchIterTester = CMatchIteratorTester::NewLC(iDbUris, iLog);
    CPosLmOperation* operation = NULL;
    
    if (aSearchLm)
        {        
        CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
        textCriteria->SetTextL(KSearchPattern);
        textCriteria->SetAttributesToSearch(CPosLandmark::ELandmarkName);
        TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
        operation = iDbSearcher->StartLandmarkSearchL(*textCriteria, sortPref);
        CleanupStack::PopAndDestroy(textCriteria);
        
        CleanupStack::PushL(operation);
        matchIterTester->RunTestL(iDbSearcher, operation, iExpectedLmResult);
        CleanupStack::Pop(operation); // ownership transferred
        }
    else
        {
        CPosLmCatNameCriteria* criteria = CPosLmCatNameCriteria::NewLC();
        criteria->SetSearchPatternL(KSearchPattern);
        operation = iDbSearcher->StartCategorySearchL(*criteria, CPosLmCategoryManager::ECategorySortOrderNameAscending);
        CleanupStack::PopAndDestroy(criteria);
        
        CleanupStack::PushL(operation);
        matchIterTester->RunTestL(iDbSearcher, operation, iExpectedCatResult);
        CleanupStack::Pop(operation); // ownership transferred
        }        
    
    TInt error = matchIterTester->Result();
    if (error != KErrNone)
        {
        TPtrC msg;
        matchIterTester->GetErrorMsg(msg);
        
        iLog->Log(msg);
        User::Leave(error);
        }
        
    CleanupStack::PopAndDestroy(matchIterTester);
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CPosTp130::InitExpectedResultsL( const TDesC& aDbName, RIdArray& aArray, TBool aSearchLm )
    {    
    iLog->Log( aDbName );
    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL( aDbName );
    CleanupStack::PushL( db );
    
    if ( db->IsInitializingNeeded() )
    	{
    	ExecuteAndDeleteLD( db->InitializeL() );
    	}
    
	CPosLmCategoryManager* catman = CPosLmCategoryManager::NewL( *db );
    CleanupStack::PushL( catman );

    RPointerArray<CNamedLmItem> items;
    CleanupStack::PushL( TCleanupItem( ResetAndDestroyNamedLmItem, &items ) );
    
    CPosLmItemIterator* iter = NULL;
    if ( aSearchLm )
    	{
    	iter = db->LandmarkIteratorL();
    	}
    else
    	{
        iter = catman->CategoryIteratorL();
    	}
    CleanupStack::PushL( iter );
    
    TPosLmItemId id = iter->NextL();
    while ( id != KPosLmNullItemId )
    	{
    	CNamedLmItem* item = new (ELeave) CNamedLmItem;
        CleanupStack::PushL( item );
        
        item->iId = id;
        
    	TPtrC name;
        if ( aSearchLm )
        	{
        	CPosLandmark* lm = db->ReadLandmarkLC( id );
        	lm->GetLandmarkName( name );
        	item->iName = name.AllocL();
        	CleanupStack::PopAndDestroy( lm );
        	}
        else
        	{
        	CPosLandmarkCategory* cat = catman->ReadCategoryLC( id );
        	cat->GetCategoryName( name );
        	item->iName = name.AllocL();
        	CleanupStack::PopAndDestroy( cat );
        	}
        
        if ( item->iName->MatchC( KSearchPattern ) != KErrNotFound ) 
        	{
	        items.AppendL( item );
	        CleanupStack::Pop( item );
        	}
        else
        	{
        	CleanupStack::PopAndDestroy( item );
        	}
        id = iter->NextL();
    	}
    CleanupStack::PopAndDestroy( iter );
    
    TLinearOrder<CNamedLmItem> order( CNamedLmItem::CompareByName );
    items.Sort( order );
    
    aArray.Reset();
    for( TInt i = 0; i < items.Count(); i++ )
    	{
    	aArray.AppendL( items[i]->iId );
        iLog->Log( _L("id: %d, name = '%S'"), items[i]->iId, items[i]->iName );
    	}
    
    CleanupStack::PopAndDestroy(); // items
    CleanupStack::PopAndDestroy( catman );
    CleanupStack::PopAndDestroy( db );
    }

// ================= CMatchIteratorTester MEMBER FUNCTIONS =======================

// Constructor
CMatchIteratorTester::CMatchIteratorTester(
		CDesCArray* aDbUris, CStifLogger* aLog )
: CActive(CActive::EPriorityStandard), 
iDbUris(aDbUris), iLog( aLog )
    {
    CActiveScheduler::Add(this);
    }

// Destructor
CMatchIteratorTester::~CMatchIteratorTester() 
    {
    Cancel();
    delete iErrorMsg;
    delete iOperation;
    iIterators.ResetAndDestroy();
    }

// ---------------------------------------------------------
// CMatchIteratorTester::NewLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CMatchIteratorTester* CMatchIteratorTester::NewLC(
	CDesCArray* aDbUris, CStifLogger* aLog)
    {
    CMatchIteratorTester* self = 
    	new (ELeave) CMatchIteratorTester(aDbUris, aLog);
    CleanupStack::PushL(self);
    return self;
    }

// ---------------------------------------------------------
// CMatchIteratorTester::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CMatchIteratorTester::RunL()
    {       
    iResult = iStatus.Int();    
    VerifyIteratorsL(iResult);
    
    if (iStatus.Int() == KPosLmOperationNotComplete)
        {        
        iOperation->NextStep(iStatus, iProgress);
        SetActive();
        }
    else
        {
        CActiveScheduler::Stop();
        }
    }
    
// ---------------------------------------------------------
// CMatchIteratorTester::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CMatchIteratorTester::DoCancel()
    {
    delete iOperation;
    iOperation = NULL;
    }
    
// ---------------------------------------------------------
// CMatchIteratorTester::RunError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CMatchIteratorTester::RunError(TInt aError)
    {
    iResult = aError;
    CActiveScheduler::Stop();
    return KErrNone;
    }
    
// ---------------------------------------------------------
// CMatchIteratorTester::RunTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CMatchIteratorTester::RunTestL(
    CPosLmMultiDbSearch* aMultiSearcher,
    CPosLmOperation* aSearchOperation,
    RPointerArray<RIdArray>& aExpectedResult)
    {
    iMultiSearcher = aMultiSearcher;
    iExpectedResult = &aExpectedResult;
    iOperation = aSearchOperation;
    
    // Initialize iIterators
    for (TInt i = 0; i < iMultiSearcher->NumOfDatabasesToSearch(); i++)
        {
        CPosLmItemIterator* iterator = iMultiSearcher->MatchIteratorL(i);
        CleanupStack::PushL(iterator);
        User::LeaveIfError(iIterators.Append(iterator));
        CleanupStack::Pop(iterator);
        }
    
    iOperation->NextStep(iStatus, iProgress);
    SetActive();
    CActiveScheduler::Start();
    }
    
// ---------------------------------------------------------
// CMatchIteratorTester::Result
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CMatchIteratorTester::Result() const 
    {
    return iResult;
    }        

// ---------------------------------------------------------
// CMatchIteratorTester::GetErrorMsg
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CMatchIteratorTester::GetErrorMsg(TPtrC& aMsg) const
    {
    aMsg.Set(*iErrorMsg);
    }
    
// ---------------------------------------------------------
// CMatchIteratorTester::VerifyIteratorsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CMatchIteratorTester::VerifyIteratorsL(TInt aResult)
    {
    for (TInt dbIndex = 0; dbIndex < iMultiSearcher->NumOfDatabasesToSearch(); dbIndex++)
        {
        CPosLmItemIterator* iterator = iMultiSearcher->MatchIteratorL(dbIndex);
        CleanupStack::PushL(iterator);
        CompareIteratorsL(*iterator, *iIterators[dbIndex]);
        delete iIterators[dbIndex];
        iIterators[dbIndex] = iterator;
        CleanupStack::Pop(iterator);
        
        // Verify matches are correct
        TBuf<50> buf;
        iterator->Reset();
        TPosLmItemId id = iterator->NextL();
        while (id != KPosLmNullItemId)
            {
            if ((*(*iExpectedResult)[dbIndex]).Find(id) == KErrNotFound)
                {
                buf.Format(_L("Cannot find match %d in expected result"), id );
                SetErrorAndLeaveL(buf);
                }
            id = iterator->NextL();
            }
            
        if (aResult == KErrNone) // Search is completed - result should be sorted ascending
            {
            RIdArray idArray;
            CleanupClosePushL(idArray);
            TInt nrOfItems = iterator->NumOfItemsL();
            iterator->GetItemIdsL(idArray, 0, nrOfItems);
            for (TInt i = 0; i < nrOfItems; i++)
                {
                TPosLmItemId id1 = idArray[i];
                TPosLmItemId id2 = (*(*iExpectedResult)[dbIndex])[i];
                if (id1 != id2)
                    {
                    buf.Format(_L("Sorted result differs, %d != %d"), id1, id2);
                    SetErrorAndLeaveL(buf);
                    }
                }
                
            TInt expectedLength = (*(*iExpectedResult)[dbIndex]).Count();
            if (nrOfItems != expectedLength)
                {
                buf.Format(_L("Length of arrays doesnät match, %d != %d"), nrOfItems, expectedLength);
                SetErrorAndLeaveL(buf);
                }
            CleanupStack::PopAndDestroy(&idArray);
            }
        }
    }
    
// ---------------------------------------------------------
// CMatchIteratorTester::CompareIteratorsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CMatchIteratorTester::CompareIteratorsL(
    CPosLmItemIterator& aNewIter, 
    CPosLmItemIterator& aOldIter)
    {
    TInt oldIterLength = aOldIter.NumOfItemsL();
    TInt newIterLength = aNewIter.NumOfItemsL();
    
    if (newIterLength < oldIterLength)
        {
        SetErrorAndLeaveL(_L("aNewIter is shorter than aOldIter"));
        }
        
    // Verify that all the items in the old iterator can be found in the new one
    if (newIterLength > 0)
        {
        RArray<TPosLmItemId> newItemIds;
        CleanupClosePushL(newItemIds);
        aNewIter.GetItemIdsL(newItemIds, 0, newIterLength);
        
        aOldIter.Reset();
        TPosLmItemId id = aOldIter.NextL();
        while (id != KPosLmNullItemId)
            {
            TInt err = newItemIds.Find(id);
            if (err == KErrNotFound)
                {
                SetErrorAndLeaveL(_L("aNewIter doesn't match aOldIter"));
                }
            id = aOldIter.NextL();
            }
            
        CleanupStack::PopAndDestroy(&newItemIds);
        }
    }

// ---------------------------------------------------------
// CMatchIteratorTester::SetErrorAndLeaveL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CMatchIteratorTester::SetErrorAndLeaveL(const TDesC& aErrMsg)
    {    
    delete iErrorMsg; 
    iErrorMsg = NULL;
    iErrorMsg = aErrMsg.AllocL();
    User::Leave(KErrGeneral);
    }
    
//  End of File
