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
#include "FT_CPosTp131.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkSearch.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLMItemIterator.h>
#include <EPos_CPosLmDisplayData.h>
#include <EPos_CPosLmDisplayItem.h>
#include <EPos_TPosLMSortPref.h>
#include "FT_CSearchResult.h"
#include <LbsPosition.h>
#include "FT_LandmarkConstants.h"
     
// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp131::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp131::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    iSearchResults.ResetAndDestroy();
        
    //CHECK TR   Delete of landmarks search - ESLI-5WNG9N
    delete iDatabase;
    iDatabase = NULL;
    
    iLandmarkSearch->UnsetDisplayData();
    delete iDisplayData;
    iDisplayData = NULL;

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    
    }

// ---------------------------------------------------------
// CPosTp131::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp131::StartL()
    {
    iErrorsFound = KErrNone;
    // copy test db to the private path
    _LIT(KTestPath, "c:\\system\\test\\testdata\\");

    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);

    TBuf<150> srcPath;

    srcPath.Append(KTestPath);
    srcPath.Append(KTp131TestDb);

    TInt er = fileMan->Copy(srcPath, KLmTp131DefaultDbPath,
            CFileMan::EOverWrite);
    if (er != KErrNone)
        iLog->Log(_L("Error when copying file"));

    CleanupStack::PopAndDestroy(fileMan);
    
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    // This db must exist
    manager->SetDefaultDatabaseUriL(KTp131TestDb);
    CleanupStack::PopAndDestroy(manager);

    iDatabase = CPosLandmarkDatabase::OpenL(KTp131TestDb);
    if (iDatabase->IsInitializingNeeded())
        {
        TRAPD( err, ExecuteAndDeleteLD( iDatabase->InitializeL() ) );
        AssertTrueSecL(err == KErrNone, _L("Init db failed"));
        }

    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    AppendSearchResultsL();

    iLog->Log(_L("Testing search with a defined sortorder"));
    SearchWithSortOrderL(iSearchResults);

    // Test Partial landmarks
    iLog->Log(_L("Testing search with a defined sortorder for partial landmarks"));
	SearchPartialLandmarksL(iSearchResults);
           
    iLog->Log(_L("Testing reset of DisplayData"));
    CheckResetL(iSearchResults);

    iLog->Log(_L("Testing cancel and displaydata"));
    CancelTestL(iSearchResults);
    
    // Test DisplayDataItem, create Landmark type
    TUint dbIndex = 0;
    CPosLandmark* lm = CPosLandmark::NewLC();
    lm->SetLandmarkNameL(_L("Tp131Landmarkname"));
    CPosLmDisplayItem* item = CPosLmDisplayItem::NewL(lm, dbIndex);
    CleanupStack::PushL(item);
    
    CPosLmDisplayItem::TDisplayItemType type = item->DisplayItemType();
    AssertTrueSecL(type == CPosLmDisplayItem::ELandmarkItem, _L("Wrong TDisplayItemType"));
    CleanupStack::PopAndDestroy(item);
    CleanupStack::Pop(lm); // lm deleted when deleting displaydata
        
    // Test DisplayDataItem, create Category type
    CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
    category->SetCategoryNameL(_L("Tp131CategoryName"));
    item = CPosLmDisplayItem::NewL(category, dbIndex);
    CleanupStack::PushL(item);
    type = item->DisplayItemType();
    
    AssertTrueSecL(type == CPosLmDisplayItem::ECategoryItem, _L("Wrong TDisplayItemType"));
    CleanupStack::PopAndDestroy(item);
    CleanupStack::Pop(category); // category deleted when deleting displaydata
    
    if (iErrorsFound != KErrNone) 
    	{
    	iLog->Log(_L("Errors were found in TP131"));
    	User::Leave(-1);
    	}

    }
    	
// ---------------------------------------------------------
// CPosTp131::ChechPartialLandmarkL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp131::CheckPartialLandmarkL(const CPosLandmark& aLandmark, 
                                     const CPosLandmark::TAttributes& aAttr, const RArray<TUint>& aReqFields)
    {
    _LIT(KPartialIdErr, "Incorrect Id when reading from a Partial landmark");
    _LIT(KPartialCategoryInfoErr, "Incorrect Category info when reading from a Partial landmark");

    TPosLmItemId id = aLandmark.LandmarkId();
    AssertTrueSecL(id != 0, KPartialIdErr); 
    
    AssertTrueSecL(aLandmark.IsPartial(), _L("Landmark is not partial"));
    
    TInt err;
    TPtrC name;
    err = aLandmark.GetLandmarkName(name);
    // Name is always included
    CheckPartialErrorCodeL(aAttr & CPosLandmark::ELandmarkName, err); 
    
    TReal32 coverageR;     
    err = aLandmark.GetCoverageRadius(coverageR);
    CheckPartialErrorCodeL(aAttr & CPosLandmark::ECoverageRadius, err);
    
    TPtrC desc;
    err = aLandmark.GetLandmarkDescription(desc); 
    CheckPartialErrorCodeL(aAttr & CPosLandmark::EDescription, err);
    
    TPtrC iconName;
    TInt iconIndex;
    TInt iconMaskIndex;
    err = aLandmark.GetIcon(iconName, iconIndex, iconMaskIndex); 
    CheckPartialErrorCodeL(aAttr & CPosLandmark::EIcon, err);
    
    RArray<TPosLmItemId> categories;
    CleanupClosePushL(categories);

    aLandmark.GetCategoriesL(categories);
    if (aAttr & CPosLandmark::ECategoryInfo)
        {
        AssertTrueSecL(categories.Count() != 0, KPartialCategoryInfoErr);
        }
    else
        {
        AssertTrueSecL(categories.Count() == 0, KPartialCategoryInfoErr);
        }
    CleanupStack::PopAndDestroy(&categories);

    TLocality loc;
    err = aLandmark.GetPosition(loc); 
    CheckPartialErrorCodeL(aAttr & CPosLandmark::EPosition, err);
    
    // Check PositionFields    
    // Check fields from EPositionFieldCompassCapabilitiesBegin up to EPositionFieldMediaCapabilitiesBegin+100 
    // should be enough to also cover URL
    for (TInt i=EPositionFieldCompassCapabilitiesBegin; i<EPositionFieldMediaCapabilitiesBegin+100; i++) 
     	{
     	TBool avail = aLandmark.IsPositionFieldAvailable((TUint16)i);
     	TInt found = aReqFields.Find(i);
     	if (avail && found != KErrNotFound)
     		{
     		TPtrC value;
     		err = aLandmark.GetPositionField((TUint16)aReqFields[found], value);
     		AssertTrueSecL(err == KErrNone, _L("Could not retrieve positionfield value"), err);
     		iLog->Log(value);
     		}
     	else if (!avail && found != KErrNotFound || avail && found == KErrNotFound)
     		{
			// Fields 405 and 802 are not always set     		
     		if (i!=405 && i!=802)
     			{
		 		TBuf<100> buf;
		 		buf.Format(_L("ERROR: Mismatch in positionfields, id %d were not found"), i);
		 		iLog->Log(buf);
		 		iErrorsFound++;
		 		}
     		}
     	}
    }

// ---------------------------------------------------------
// CPosTp131::ChechPartialErrorCodeL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp131::CheckPartialErrorCodeL(const TBool aPartial, const TInt aErr)
    {
    _LIT(KPartialLmErr, "Incorrect error code returned when reading from a Partial landmark");

    if (aPartial)
        {
        AssertTrueSecL(aErr == KErrNone, KPartialLmErr); 
        }
    else
        {
        AssertTrueSecL(aErr == KErrNotFound, KPartialLmErr);
        }
    }

// ---------------------------------------------------------
// CPosTp131::SearchPartialLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp131::SearchPartialLandmarksL(const RPointerArray<CSearchResult>& aSearchResults)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect");
    _LIT(KNumOfMatchesErr2, "No. of display data matches is incorrect");
    _LIT(KSortorderErr, "The sortorder is incorrect");
   
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);  
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    
    // Set DisplayData
	iDisplayData = CPosLmDisplayData::NewL();
    iLandmarkSearch->SetDisplayData(*iDisplayData);
        
	// Only do search for index KWhatToSearchFor in lmDbSearchResult.txt
	// index 11 is MLFW
	TInt KWhatToSearchFor = 11;
    textCriteria->SetTextL(aSearchResults[KWhatToSearchFor]->TextCriteria());
    textCriteria->SetAttributesToSearch(aSearchResults[KWhatToSearchFor]->Attributes());
    textCriteria->SetPositionFieldsToSearchL(aSearchResults[KWhatToSearchFor]->PositionFields()); 

    RArray<TUint> reqFields;
    CleanupClosePushL(reqFields);
    
    iLog->Log(aSearchResults[KWhatToSearchFor]->TextCriteria());
    CPosLmPartialReadParameters* partialParam = CPosLmPartialReadParameters::NewLC();
    
    CPosLmPartialReadParameters* fakePartialParam = CPosLmPartialReadParameters::NewLC();
    reqFields.Append(EPositionFieldStreet);
    fakePartialParam->SetRequestedPositionFields(reqFields);
    reqFields.Reset();

	// Dont search for all search criteras 
	// takes just too long time on target (approx 1 hour for this test on target)
	        	
	// Search for all combinations from 0 - CPosLandmark::EDescription
	// Ex attr=0, attr=1, attr=2, attr=3, .....attr=63,
#ifdef __WINS__	
    TUint max = CPosLandmark::EDescription;
    max <<= 1;
    max -= 1;
#else
    TUint max = CPosLandmark::ECategoryInfo;
    max <<= 1;
    max -= 1;
#endif
    
    TUint attr=0;
    for (attr=0; attr<=max; attr++)
        {
        partialParam->SetRequestedAttributes(attr);
        reqFields.Reset();
        
        if (attr == 5)
        	{
        	// Add EPositionFieldCountry
        	iLog->Log(_L("Also cover EPositionFieldCountry"));
        	reqFields.Append(EPositionFieldCountry);
        	}
        else if (attr == 12)
        	{
        	// Add EPositionFieldCity, EPositionFieldStreet, EPositionFieldCountry
        	iLog->Log(_L("Also cover EPositionFieldCity, EPositionFieldStreet, EPositionFieldCountry"));
        	reqFields.Append(EPositionFieldCity);
        	reqFields.Append(EPositionFieldStreet);
        	reqFields.Append(EPositionFieldCountry);
        	
        	}
        else if (attr == 32)
        	{
        	// Add EPositionFieldMediaLinksStart, EPositionFieldDistrict
        	iLog->Log(_L("Also cover EPositionFieldMediaLinksStart, EPositionFieldDistrict"));
        	reqFields.Append(EPositionFieldMediaLinksStart);
        	reqFields.Append(EPositionFieldDistrict);
        	}
        	
		iDisplayData->UnsetPartialReadParameters();
        partialParam->SetRequestedPositionFields(reqFields);
    	iDisplayData->SetPartialReadParametersL(*partialParam);
    	
        for (TInt i=0; i<2; i++)
            {
            switch (i)
                {
                case 0:
                	{
                	//**** async
                	//iLog->Log(_L("*****Async, EDescending"));
                	sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
                    iAscendedSorted = EFalse;
                    iOperation = iLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref, aSearchResults[KWhatToSearchFor]->Redefined());
                    // Test that if SetPartialReadParametersL is called after search has started it has no affect
                    iDisplayData->SetPartialReadParametersL(*fakePartialParam);
                    ExecuteLmOpL();
                    }
                    break;
                case 1:
                	{
                	partialParam->SetRequestedPositionFields(reqFields);
    				iDisplayData->SetPartialReadParametersL(*partialParam);
    	
                    sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
                    //iLog->Log(_L("*****Async EAscending"));
                    iAscendedSorted = ETrue;
                    iOperation = iLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref, aSearchResults[KWhatToSearchFor]->Redefined());
                    ExecuteLmOpL();
                    }
                    break;
                }
            
            CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
            CleanupStack::PushL(iter);
            
            RArray<TPosLmItemId>* searchResults = &aSearchResults[KWhatToSearchFor]->SearchResult();
            if ((TUint)searchResults->Count() != iLandmarkSearch->NumOfMatches())
                {
                TBuf<50> buf;
                buf.Append(_L("KNumOfMatchesError when searching for "));
                buf.Append(aSearchResults[KWhatToSearchFor]->TextCriteria());
                iLog->Log(buf);
               
               iLog->Log(KNumOfMatchesErr);
               User::Leave(-1);
               
                }
                
            // Check correct displayData
            AssertTrueSecL(iter->NumOfItemsL() == iDisplayData->Count(), KNumOfMatchesErr2);
            LandmarksSortL(*searchResults, sortPref);
            
            TInt ii=0;
            TPosLmItemId id = iter->NextL();
                   
            for (TInt i=0;i<iDisplayData->Count();i++)
                {
                CPosLmDisplayItem& item = iDisplayData->DisplayItem(i);
                TInt lmId = item.Landmark().LandmarkId();

                CPosLandmark* target = iDatabase->ReadLandmarkLC((*searchResults)[ii]);
                TPtrC sourceName, targetName;
                item.Landmark().GetLandmarkName(sourceName);
                target->GetLandmarkName(targetName);
                
                if (lmId != (*searchResults)[ii])
                    {
                    // If found multiple match -> "Mölndal" etc
                    //Check if same name then don't Leave
                    AssertTrueSecL(sourceName.CompareC(targetName) == 0,KSortorderErr, lmId);
                    }
                else
                    {
                    //CompareLandmarksL(item.Landmark(), *target);
                    // Add some check of partial landmarks here
                    AssertTrueSecL(sourceName.CompareC(targetName) == 0,KSortorderErr, lmId);
                    
                    // Landmarkname is always included when sorting
                    CheckPartialLandmarkL(item.Landmark() , attr | CPosLandmark::ELandmarkName, reqFields);
                    }

                    CleanupStack::PopAndDestroy(target);
                id = iter->NextL();

                ++ii;
                }
            CleanupStack::PopAndDestroy(iter);
            iDisplayData->Reset();
            } 
        } 
        
    iLandmarkSearch->UnsetDisplayData();
    CleanupStack::PopAndDestroy(fakePartialParam);
    CleanupStack::PopAndDestroy(partialParam);
    CleanupStack::PopAndDestroy(&reqFields);
    CleanupStack::PopAndDestroy(textCriteria);
    delete iDisplayData;
    iDisplayData = NULL;
    }

// ---------------------------------------------------------
// CPosTp131::SearchWithSortOrderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp131::SearchWithSortOrderL(const RPointerArray<CSearchResult>& aSearchResults)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect");
    _LIT(KNumOfMatchesErr2, "No. of display data matches is incorrect");
    _LIT(KSortorderErr, "The sortorder is incorrect");
   
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);  
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    
    // Set DisplayData
	iDisplayData = CPosLmDisplayData::NewL();
    iLandmarkSearch->SetDisplayData(*iDisplayData);

	CPosLmPartialReadParameters* partialParam = CPosLmPartialReadParameters::NewLC();
	
	RArray<TUint> reqFields;
    CleanupClosePushL(reqFields);

	// Test that UnsetPartialReadParameters works	
	reqFields.Append(EPositionFieldCountry);
	partialParam->SetRequestedPositionFields(reqFields);
    iDisplayData->SetPartialReadParametersL(*partialParam);
    CleanupStack::PopAndDestroy(&reqFields);
    CleanupStack::PopAndDestroy(partialParam);
    iDisplayData->UnsetPartialReadParameters();
    
	// Dont search for all search criteras (see excel file)
	// takes just too long time on target (approx 1 hour for this test on target)
	// therefore start at j=7
#ifdef __WINS__	
    for(TInt j=2; j<aSearchResults.Count(); j++)
#else
    for(TInt j=7; j<aSearchResults.Count(); j++)
#endif
        {   
        textCriteria->SetTextL(aSearchResults[j]->TextCriteria());
        textCriteria->SetAttributesToSearch(aSearchResults[j]->Attributes());
        textCriteria->SetPositionFieldsToSearchL(aSearchResults[j]->PositionFields()); 

        iLog->Log(aSearchResults[j]->TextCriteria());
        
        // Dont test sync search
        for (TInt i=2; i<6; i++)
            {
            switch (i)
                {
                //**** sync
                case 0:
                	{
                	sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
                    ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(
                        *textCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined()));
                    }
                    break;
                case 1:
                	{
                    sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
                    ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(
                        *textCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined()));
                    }
                    break;
                case 2:
                	{
                	//**** async
                	//iLog->Log(_L("*****Async, EDescending"));
                	sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
                    iAscendedSorted = EFalse;
                    //iLog->Log(_L("Start"));
                    iOperation = iLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref, aSearchResults[j]->Redefined());
                    ExecuteLmOpL();
                    //iLog->Log(_L("Done"));
                    }
                    break;
                case 3:
                	{
                    sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
                    //iLog->Log(_L("*****Async EAscending"));
                    iAscendedSorted = ETrue;
                    iOperation = iLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref, aSearchResults[j]->Redefined());
                    ExecuteLmOpL();
                    }
                    break;
                //**** asych with User::WaitForRequest()
                case 4:
                    {
                    //iLog->Log(_L("*****User::WaitForRequest() EDescending"));
                    sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
                    CPosLmOperation* op = iLandmarkSearch->StartLandmarkSearchL(
                        *textCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined());
                    CleanupStack::PushL(op);
                    RunAsyncOperationByWaitForReqL(op);
                    CleanupStack::PopAndDestroy(op);
                    }
                    break;
                case 5:
                    {
                    //iLog->Log(_L("*****User::WaitForRequest() EAscending"));
                    sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
                    CPosLmOperation* op = iLandmarkSearch->StartLandmarkSearchL(
                        *textCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined());
                    CleanupStack::PushL(op);
                    RunAsyncOperationByWaitForReqL(op);
                    CleanupStack::PopAndDestroy(op);
                    }
                    break;
                }
            
        CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
        CleanupStack::PushL(iter);
        
        RArray<TPosLmItemId>* searchResults = &aSearchResults[j]->SearchResult();
        if ((TUint)searchResults->Count() != iLandmarkSearch->NumOfMatches())
            {
            TBuf<50> buf;
            buf.Append(_L("KNumOfMatchesError when searching for "));
            buf.Append(aSearchResults[j]->TextCriteria());
            iLog->Log(buf);
            
            iLog->Log(KNumOfMatchesErr);
            User::Leave(-1);
            }
            
        // Check correct displayData
        AssertTrueSecL(iter->NumOfItemsL() == iDisplayData->Count(), KNumOfMatchesErr2);
        LandmarksSortL(*searchResults, sortPref);
        
        TInt ii=0;
        TPosLmItemId id = iter->NextL();
               
        for (TInt i=0;i<iDisplayData->Count();i++)
            {
            CPosLmDisplayItem& item = iDisplayData->DisplayItem(i);
            TInt lmId = item.Landmark().LandmarkId();

            CPosLandmark* target = iDatabase->ReadLandmarkLC((*searchResults)[ii]);
            TPtrC sourceName, targetName;
            item.Landmark().GetLandmarkName(sourceName);
            target->GetLandmarkName(targetName);
            
            if (lmId != (*searchResults)[ii])
                {
                // If found multiple match -> "Mölndal" etc
                //Check if same name then don't Leave
                AssertTrueSecL(sourceName.CompareC(targetName) == 0,KSortorderErr, lmId);
                }
            else
                {
                CompareLandmarksL(item.Landmark(), *target);
                }

            CleanupStack::PopAndDestroy(target);
            id = iter->NextL();

            ++ii;
            }
        CleanupStack::PopAndDestroy(iter);
        iDisplayData->Reset();
        }
        }
    iLandmarkSearch->UnsetDisplayData();
    CleanupStack::PopAndDestroy(textCriteria);
    delete iDisplayData;
    iDisplayData = NULL;
    }

// ---------------------------------------------------------
// CPosTp131::CheckResetL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp131::CheckResetL(const RPointerArray<CSearchResult>& aSearchResults)
    {
    iLog->Log(_L("CheckResetL"));
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    
    // Set DisplayData
    CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
	CleanupStack::PushL(displayData);
    iLandmarkSearch->SetDisplayData(*displayData);
        
    for(TInt j=0; j<aSearchResults.Count(); j++)
        {
		textCriteria->SetTextL(aSearchResults[j]->TextCriteria());
		textCriteria->SetAttributesToSearch(aSearchResults[j]->Attributes());
		textCriteria->SetPositionFieldsToSearchL(aSearchResults[j]->PositionFields());

		// Sync
		ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(*textCriteria, aSearchResults[j]->Redefined()));
		AssertTrueSecL(iLandmarkSearch->NumOfMatches() == displayData->Count(),_L("Wrong number of matches1"),j);
		displayData->Reset();
		AssertTrueSecL(displayData->Count() == 0,_L("DisplayData should have been resetted"));
		
		// Async
		RunAsyncOperationLD(iLandmarkSearch->StartLandmarkSearchL(*textCriteria, aSearchResults[j]->Redefined()));
		AssertTrueSecL(iLandmarkSearch->NumOfMatches() == displayData->Count(),_L("Wrong number of matches2"),j);
    
    	RArray<TPosLmItemId>* searchResults = &aSearchResults[j]->SearchResult();
        //AssertTrueSecL(iLandmarkSearch->NumOfMatches() == searchResults->Count(),_L("Wrong number of matches3"),j);
        if (iLandmarkSearch->NumOfMatches() != searchResults->Count())
        	{
	        TBuf<150> buf;
	        buf.Format(_L("Exp %d nr of matches, got %d, searching for: "), searchResults->Count(), iLandmarkSearch->NumOfMatches());
	        buf.Append(aSearchResults[j]->TextCriteria());
	        iLog->Log(buf);
        	}
        
        AssertTrueSecL(displayData->Count() == iLandmarkSearch->NumOfMatches(),_L("DisplayData matches is not correct"));
        
    }

	// Test unset of displaydata
	iLandmarkSearch->UnsetDisplayData();
    CleanupStack::PopAndDestroy(displayData);
    
    displayData = CPosLmDisplayData::NewL();
	CleanupStack::PushL(displayData);
    iLandmarkSearch->SetDisplayData(*displayData);
    iLandmarkSearch->UnsetDisplayData();
    
    textCriteria->SetTextL(aSearchResults[0]->TextCriteria());
	textCriteria->SetAttributesToSearch(aSearchResults[0]->Attributes());
	textCriteria->SetPositionFieldsToSearchL(aSearchResults[0]->PositionFields());
	ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(*textCriteria));
	AssertTrueSecL(displayData->Count() == 0,_L("No matches should be found in DisplayData"));
        
    iLandmarkSearch->SetDisplayData(*displayData);
    
    textCriteria->SetTextL(aSearchResults[0]->TextCriteria());
	textCriteria->SetAttributesToSearch(aSearchResults[0]->Attributes());
	textCriteria->SetPositionFieldsToSearchL(aSearchResults[0]->PositionFields());
	ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(*textCriteria));
	RArray<TPosLmItemId>* searchResults = &aSearchResults[0]->SearchResult();
   	AssertTrueSecL(iLandmarkSearch->NumOfMatches() == displayData->Count(),_L("Wrong number of matches4"));
    AssertTrueSecL(iLandmarkSearch->NumOfMatches() == searchResults->Count(),_L("Wrong number of matches5"));
    
    iLandmarkSearch->UnsetDisplayData();
    CleanupStack::PopAndDestroy(displayData);
	CleanupStack::PopAndDestroy(textCriteria);
    }

// ---------------------------------------------------------
// CPosTp131::CancelTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp131::CancelTestL(const RPointerArray<CSearchResult>& aSearchResults)
    {
    iLog->Log(_L("CancelTestL"));
	CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
	
	// Cancel search operation and verify that the displayData result is sorted correctly
	TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
	
    for (TInt pp=0;pp<2;pp++)
    	{
    	if (pp == 0) 
    		{
    		//iLog->Log(_L("Sorting Descending"));
    		sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
    		}
    	else {
    		sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
    		//iLog->Log(_L("Sorting Ascending"));
    		}
    		
	    // Set DisplayData
	    iLandmarkSearch->UnsetDisplayData();
	    delete iDisplayData;
	    iDisplayData = NULL;
	    CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
		CleanupStack::PushL(displayData);
	    iLandmarkSearch->SetDisplayData(*displayData);
	        
	    for(TInt j=0; j<aSearchResults.Count(); j++)
	        {
			textCriteria->SetTextL(aSearchResults[j]->TextCriteria());
			textCriteria->SetAttributesToSearch(aSearchResults[j]->Attributes());
			textCriteria->SetPositionFieldsToSearchL(aSearchResults[j]->PositionFields());

			TBuf<150> buf;
		    buf.Append(aSearchResults[j]->TextCriteria());
		    //iLog->Log(buf);
		        
			// Cancel operation in callback
			RunAsyncOperationAndCancelInCallbackLD(iLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref, EFalse));
			AssertTrueSecL(iLandmarkSearch->NumOfMatches() == displayData->Count(),_L("Wrong number of matches after cancel"),j);
			
			buf.Zero();
			buf.Format(_L("Search returned %d nr of matches, displayData %d nr of matches"),iLandmarkSearch->NumOfMatches(), displayData->Count());
			//iLog->Log(buf);
			
			// Check sort order below
			CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
	        CleanupStack::PushL(iter);
	        
	        if (iter->NumOfItemsL()>0)
	        	{
	        	RArray<TPosLmItemId> arr;
	        	CleanupClosePushL(arr);
	        	iter->GetItemIdsL(arr, 0, iter->NumOfItemsL());
	        
	        	// Sort matchiterator
	        	LandmarksSortL(arr, sortPref);
			    
			    TInt index=0;
				TInt itemId = displayData->NewItemIndex();
			    while (itemId != KPosLmNoNewItems)
			        {
			        CPosLmDisplayItem& item = displayData->DisplayItem(itemId);
			        
			        AssertTrueSecL(item.DisplayItemType() == CPosLmDisplayItem::ELandmarkItem, 
			        	_L("Wrong item returned, only ELandmarkItem should be returned"));
			        
			        AssertTrueSecL(item.DatabaseIndex() == 0, _L("Wrong DatabaseIndex, should be 0"));
			    	
				    TInt lmId = item.Landmark().LandmarkId();
				    
				    
	//		    	AssertTrueSecL(arr[index] == lmId , _L("Incorrect sortorder"));
			    	CPosLandmark* lm = iDatabase->ReadLandmarkLC(arr[index]);
			    	
				    if ( arr[index] != lmId )
				        {
			    	TPtrC sourceName;
				    item.Landmark().GetLandmarkName(sourceName);
					TPtrC name2;
					lm->GetLandmarkName(name2);
                        iLog->Log( _L("CancelTestL: #%02d, exp %d, act %d, '%S', '%S'"), 
                            index, arr[index], lmId, &name2, &sourceName );
					
                        if ( sourceName.CompareC( name2 ) != 0 )
                            {
                            AssertTrueSecL( arr[index] == lmId, _L("Incorrect sort order") );
                            }
				        }
				    else
				        {
					CompareLandmarksL(*lm, item.Landmark());
				        }
					
			        itemId = displayData->NewItemIndex();
			        index++;
			        CleanupStack::PopAndDestroy(lm);
			        }
			    CleanupStack::PopAndDestroy(&arr);       
	        	}
	        CleanupStack::PopAndDestroy(iter);
			}
			iLandmarkSearch->UnsetDisplayData();
	    	CleanupStack::PopAndDestroy(displayData);
	    }
		CleanupStack::PopAndDestroy(textCriteria);
	}

// ---------------------------------------------------------
// Tp131InternalLandmarksSortL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//	
TInt Tp131InternalLandmarksSortL(const CPosLandmark& aSource, const CPosLandmark& aTarget)
    {
    TPtrC sourceName, targetName;
    
    User::LeaveIfError(aSource.GetLandmarkName(sourceName));  
    User::LeaveIfError(aTarget.GetLandmarkName(targetName));
    
    return sourceName.CompareC(targetName);
    }
    	
// ---------------------------------------------------------
// CPosTp131::LandmarksSortL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp131::LandmarksSortL(RArray<TPosLmItemId>& aArray, const TPosLmSortPref& aSortPref)
    {
    
    RPointerArray<CPosLandmark> landmarks;
    CleanupClosePushL(landmarks);
    
    for (TInt i=0; i<aArray.Count(); i++)
        {
        CPosLandmark* lm = iDatabase->ReadLandmarkLC(aArray[i]);
        landmarks.Append(lm);
        CleanupStack::Pop(lm);
        }
    
    TLinearOrder<CPosLandmark> order(Tp131InternalLandmarksSortL);
    landmarks.Sort(order);
  
    aArray.Reset();

    if (aSortPref.SortOrder() == TPosLmSortPref::EAscending)
        {
        for (TInt j=0; j<landmarks.Count(); j++)
            {
            aArray.Append(landmarks[j]->LandmarkId());
            }
        }
    else
        {
        for (TInt k=landmarks.Count()-1; k>=0; k--)
            {
            aArray.Append(landmarks[k]->LandmarkId());
            }
        }
    landmarks.ResetAndDestroy();
    CleanupStack::PopAndDestroy(&landmarks);
    
    }

// ---------------------------------------------------------
// CPosTp131::ExecuteLmOpL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp131::ExecuteLmOpL()
    {
    CActiveLmOperation* operationWrapper = new (ELeave) CActiveLmOperation(iLog);
    CleanupStack::PushL(operationWrapper);
        
	operationWrapper->Start(iOperation, this);        
    CActiveScheduler::Start();
    
    operationWrapper->CheckOperationL();
    operationWrapper->DeleteOperationD();

    CleanupStack::PopAndDestroy(operationWrapper);
    
    }
    
// ---------------------------------------------------------
// CPosTp131::NotifyProgress
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp131::NotifyProgress(TReal aProgress)
    {

    TBuf<100> buf;

    TInt progress = (TInt) (aProgress * 100);
    buf.Format(_L("Progress: %d"), progress);
    
    
    if (iDisplayData)
    	{
    
		
		if (iDisplayData->Count()>0)
			{
			CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
        	CleanupStack::PushL(iter);
        	
        	RArray<TPosLmItemId> arr;
        	CleanupClosePushL(arr);
        	iter->GetItemIdsL(arr, 0, iter->NumOfItemsL());
        	TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);  
        	
        	if (iAscendedSorted) sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
        	else sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
        	// Sort matchiterator
        	LandmarksSortL(arr, sortPref);
        	
        	if (iter->NumOfItemsL() != iDisplayData->Count())
		    	{
		    	iLog->Log(_L("Error: Wrong number of items"));
		    	iErrorsFound++;
		    	}		    

			TInt itemId = iDisplayData->NewItemIndex();
		    while (itemId != KPosLmNoNewItems)
		        {
		        CPosLmDisplayItem& item = iDisplayData->DisplayItem(itemId);
		        
		        if (item.DisplayItemType() != CPosLmDisplayItem::ELandmarkItem)
            	{
            	iLog->Log(_L("Error: TDisplayItemType should be ELandmarkItem"));
            	iErrorsFound++;
            	}
            
            if (item.DatabaseIndex() != 0)
            	{
            	// When single search, index should always be 0
            	iLog->Log(_L("Error: Wrong DatabaseIndex, should be 0"));
            	iErrorsFound++;
            	}

				TPtrC lmName;
				item.Landmark().GetLandmarkName(lmName);
				TInt lmId = item.Landmark().LandmarkId();
				
				// Compare all landmarks from CPosLmDisplayItem and landmarks from MatchIteratorL
		        for (TInt i=0;i<iDisplayData->Count();i++)
			            {
			            CPosLmDisplayItem& item = iDisplayData->DisplayItem(i);
			            TPtrC sourceName;
			            item.Landmark().GetLandmarkName(sourceName);
						
						
						CPosLandmark* lm2 = iDatabase->ReadLandmarkLC(arr[i]);
						TPtrC name;
						lm2->GetLandmarkName(name);
					
						
						// Only compare names
						if (name.Compare(sourceName) != 0) 
							{
							iLog->Log(_L("ERROR: No match, name %S, sourceName %S"), &name, &sourceName);
							iErrorsFound++;
							}
						
						CleanupStack::PopAndDestroy(lm2);							
						}
				
		        itemId = iDisplayData->NewItemIndex();		        
		        }

		    CleanupStack::PopAndDestroy(&arr);
        	CleanupStack::PopAndDestroy(iter);
        	}
        }
	}
        
//  End of File
