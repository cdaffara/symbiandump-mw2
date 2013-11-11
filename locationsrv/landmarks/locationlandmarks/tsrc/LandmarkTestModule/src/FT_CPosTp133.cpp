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
#include "FT_CPosTp133.h"
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

#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_CPosLmMultiDbSearch.h>
     
// CONSTANTS
const TInt KNrOfDatabases = 5;
_LIT(KTp133DbFile, "TP133Test.ldb");
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp133::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp133::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    iSearchResults.ResetAndDestroy();

    delete iDatabase;
    iDatabase = NULL;

    delete iDisplayData;
    iDisplayData = NULL;

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    
    delete iMultiLandmarkSearch;
    iMultiLandmarkSearch = NULL;
    
    iJustNowSearchResults->Reset();
    
    TRAPD(err, RemoveAllLmDatabasesL());
    if (err != KErrNone) iLog->Log(_L("Problem when removing all lm databases"));
    ReleaseLandmarkResources();
    }

// ---------------------------------------------------------
// CPosTp133::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp133::StartL()
    {
    CDesCArray* dbUris = NULL;
    // PrepareDatabases
    RemoveAllLmDatabasesL();
    // Get the list of Dbs remaining after removing the possible ones.
    CPosLmDatabaseManager* dbMan = CPosLmDatabaseManager::NewL();
        CleanupStack::PushL(dbMan);
        dbUris = dbMan->ListDatabasesLC();
        
        TInt initialDbCount = dbUris->Count();
        CleanupStack::PopAndDestroy(dbUris);
    CopyTestDbFileL(KDb20);
    CopyTestDbFileL(KDb40);
    CopyTestDbFileL(KDb60);
    CopyTestDbFileL(KDb80);
    CopyTestDbFileL(KDb105);
        
	iDisplayData = NULL;
	iLandmarkSearch = NULL;
	       
     dbUris = dbMan->ListDatabasesLC();
    CleanupStack::Pop(dbUris);
    CleanupStack::PopAndDestroy(dbMan);
    CleanupStack::PushL(dbUris);
        
    AssertTrueSecL(dbUris->Count() == initialDbCount+KNrOfDatabases, _L("Wrong number of test databases!"));
    
    // Use this db as a template for how the result from multiple db search should be sorted
	// used mostly in LandmarksSortL
    CopyTestDbFileL(KTp133DbFile);
	iDatabase = CPosLandmarkDatabase::OpenL(KTp133DbFile);
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

 	AppendSearchResultsL();
 	// Get only the list of dburi in which this test case operations need to be performed
 	TInt dbUriCount = dbUris->Count();
 	
 	for ( TInt i= 0;i < dbUriCount;i++)
 	    {
    iLog->Log((*dbUris)[i]);
    // Action to be performed only on the newly added dbs in this test case
        TPtrC dbUri((*dbUris)[i]);
            if ( (dbUri != KDb20Uri) && (dbUri != KDb40Uri) && (dbUri != KDb60Uri) &&
                   ( dbUri != KDb80Uri) && (dbUri != KDb105Uri) )
                {
                dbUris->Delete(i);
                }

 	    }
    DoSearchL(iSearchResults, dbUris, ESynchronous);
    
    DoSearchL(iSearchResults, dbUris, EAsynchronous);
    
    // Test Partial landmarks
    iLog->Log(_L("Testing search with a defined sortorder for partial landmarks"));
    
   	// Only do search for index KWhatToSearchFor in lmDbSearchResult.txt
	// index 11 is MLFW
	TInt KWhatToSearchFor = 11;
	SearchPartialLandmarksL(KWhatToSearchFor, iSearchResults, dbUris);
		
	// index 20 is x
	KWhatToSearchFor = 20;
	SearchPartialLandmarksL(KWhatToSearchFor, iSearchResults, dbUris);
	
	iLog->Log(_L("Testing search for partial landmarks without name"));
    SearchPartialWithoutNameL(iSearchResults, dbUris);
	
    iLog->Log(_L("Testing reset of DisplayData"));
    CheckResetL(iSearchResults, dbUris);
    
    iLog->Log(_L("Testing cancel and displaydata, cancel immeditely"));
    CancelTestL(iSearchResults, dbUris, EFalse);
    
    iLog->Log(_L("Testing cancel and displaydata, cancel in callback"));
    CancelTestL(iSearchResults, dbUris, ETrue);
    
    CleanupStack::PopAndDestroy(dbUris);
    if (iErrorsFound != KErrNone) 
    	{
    	iLog->Log(_L("Errors were found in TP133"));
         User::Leave(-1);
    	
   		}
    }

// ---------------------------------------------------------
// CPosTp133::DoSearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp133::DoSearchL(const RPointerArray<CSearchResult>& aSearchResults, 
							CDesCArray* aDbUris, TExecutionMode aExecutionMode)
    {
    iMultiLandmarkSearch = CPosLmMultiDbSearch::NewL(*aDbUris);
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
    TBuf<150> buf;
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();	
    
    // Dont search for all search criteras (see excel file)
	// takes just too long time on target (approx 1 hour for this test on target)
	// therefore start at j=2
#ifdef __WINS__	
	for(TInt j=0; j<aSearchResults.Count(); j++)
#else
    for(TInt j=2; j<aSearchResults.Count(); j++)
#endif
        {   
        textCriteria->SetTextL(aSearchResults[j]->TextCriteria());
        textCriteria->SetAttributesToSearch(aSearchResults[j]->Attributes());
        if (aSearchResults[j]->PositionFields().Find(803) != KErrNotFound)
            {
            // Stupid fix, since the multi dbs have not been re-generated against correct LBSFieldsid.h
            // they contain the 802 field (instead of 803) so append this id as well 
            RArray<TUint> arr;
            iLog->Log(_L("Found 803"));
            arr = aSearchResults[j]->PositionFields();
            arr.Append(802);
            textCriteria->SetPositionFieldsToSearchL(arr);
            }
        else
            {
            textCriteria->SetPositionFieldsToSearchL(aSearchResults[j]->PositionFields());
            }
        
        iLog->Log(_L(">>>>>>Searching for: "));
        iLog->Log(aSearchResults[j]->TextCriteria());

		// Sort ascended and descended
		for (TInt sorted=0;sorted<2;sorted++)
			{
			iDisplayData = CPosLmDisplayData::NewL();
	    	iMultiLandmarkSearch->SetDisplayData(*iDisplayData);
			
			switch (sorted)
	        	{
                case 0:
                iLog->Log(_L("******Sorting EAscending*******"));
                iAscendedSorted = ETrue;
                sortPref = TPosLmSortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
                break;
                case 1:
                iLog->Log(_L("**********Sorting EDescending**********"));
                iAscendedSorted = EFalse;
                sortPref = TPosLmSortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
                break;
                }

            iJustNowSearchResults = &aSearchResults[j]->SearchResult();
			LandmarksSortL(*iJustNowSearchResults, sortPref);
			iLastNrOfDisplayData = 0;
			iOperation = iMultiLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref, aSearchResults[j]->Redefined());
			switch (aExecutionMode)
	        	{
                case ESynchronous:
                	iLog->Log(_L("ESynchronous"));
                    ExecuteAndDeleteLD(iOperation);
                    break;
                case EAsynchronous:
                	iLog->Log(_L("EAsynchronous"));
                	// Also check progress here
                    ExecuteLmOpL();
                    break;
                }
			
			
			// Check for any errors when multi searching
			if (iMultiLandmarkSearch->NumOfSearchErrors() != 0) 
			{
				iLog->Log(_L("Errors after multi db search"));
         		User::Leave(-1);
			}
		
			TInt number = iDisplayData->Count();
			buf.Zero();
			buf.Format(_L("Nr in DisplayData: %d Nr in result %d"), number, iJustNowSearchResults->Count());
			iLog->Log(buf);
			
			AssertTrueSecL(iDisplayData->Count() == iJustNowSearchResults->Count(),_L("Mismatch in number of matches"));
			
	        for (TInt i=0;i<iDisplayData->Count();i++)
	            {
	            CPosLmDisplayItem& item = iDisplayData->DisplayItem(i);
	            TInt lmId = item.Landmark().LandmarkId();
	            CPosLandmark* target = iDatabase->ReadLandmarkLC((*iJustNowSearchResults)[i]);
	            TPtrC sourceName, targetName;

	            item.Landmark().GetLandmarkName(sourceName);
	            target->GetLandmarkName(targetName);
				buf.Zero();
			
	            if (sourceName.CompareC(targetName) != 0)
	            	{
	            	
	            		iLog->Log(_L("Mismatch between landmarks"));
         				User::Leave(-1);
	            	}
				CleanupStack::PopAndDestroy(target);
	            }
			
	    	iMultiLandmarkSearch->UnsetDisplayData();
		    delete iDisplayData;iDisplayData=NULL;
		    }
	    }
   	CleanupStack::PopAndDestroy(textCriteria);
	
	delete iMultiLandmarkSearch;
	iMultiLandmarkSearch = NULL;
    }
    
// ---------------------------------------------------------
// CPosTp133::CheckPartialLandmarkL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp133::CheckPartialLandmarkL(const CPosLandmark& aLandmark, 
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
    
    TBuf<100> buf;
    buf.Format(_L("Checking LM: %S"), &name);
    // iLog->Log(buf);
    
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
// CPosTp133::ChechPartialErrorCodeL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp133::CheckPartialErrorCodeL(const TBool aPartial, const TInt aErr)
    {
    _LIT(KPartialLmErr, "Incorrect error code returned when reading from a Partial landmark");
    _LIT(KPartialLmErr2, "Incorrect error code returned when reading from a non-Partial landmark");

    if (aPartial)
        {
        
        if (aErr != KErrNone)
        	{
        	iLog->Log(KPartialLmErr);
        	iErrorsFound++;
        	}
        }
    else
        {
        
        if (aErr != KErrNotFound)
        	{
        	iLog->Log(KPartialLmErr2);
        	iErrorsFound++;
        	}
        }
    }

// ---------------------------------------------------------
// CPosTp133::SearchPartialWithoutNameL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp133::SearchPartialWithoutNameL(const RPointerArray<CSearchResult>& aSearchResults,
									CDesCArray* aDbUris)
    {
    iLog->Log(_L("SearchPartialWithoutNameL"));
    // Search in all lm dbs
   	iMultiLandmarkSearch = CPosLmMultiDbSearch::NewL(*aDbUris);
   	// Set DisplayData
	iDisplayData = CPosLmDisplayData::NewL();
    iMultiLandmarkSearch->SetDisplayData(*iDisplayData);
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
    TBuf<150> buf;
    
    CPosLmPartialReadParameters* partialParam = CPosLmPartialReadParameters::NewLC();
    
    // Dont search for all search criteras (see excel file)
	// takes just too long time on target
	// therefore start at j=2
#ifdef __WINS__	
    for(TInt j=0; j<aSearchResults.Count(); j++)
#else
    for(TInt j=2; j<aSearchResults.Count(); j++)
#endif
        {   
        textCriteria->SetTextL(aSearchResults[j]->TextCriteria());
        textCriteria->SetAttributesToSearch(aSearchResults[j]->Attributes());
        
        
        if (aSearchResults[j]->PositionFields().Find(803) != KErrNotFound)
            {
            // Stupid fix, since the multi dbs have not been re-generated against correct LBSFieldsid.h
            // they contain the 802 field (instead of 803) so append this id as well
            RArray<TUint> arr;
            iLog->Log(_L("Found 803"));
            arr = aSearchResults[j]->PositionFields();
            arr.Append(802);
            textCriteria->SetPositionFieldsToSearchL(arr);
            }
        else
            {
            textCriteria->SetPositionFieldsToSearchL(aSearchResults[j]->PositionFields());
            }

        RArray<TUint> reqFields;
        CleanupClosePushL(reqFields);
    
        iLog->Log(aSearchResults[j]->TextCriteria());
        
        // Set position and descriptiom
        //partialParam->SetRequestedAttributes(CPosLandmark::EPosition);
        partialParam->SetRequestedAttributes(CPosLandmark::EDescription| CPosLandmark::EPosition);
        
    	iDisplayData->SetPartialReadParametersL(*partialParam);
        iLastNrOfDisplayData=0;
        iOperation = iMultiLandmarkSearch->StartLandmarkSearchL(*textCriteria, aSearchResults[j]->Redefined());
        ExecuteLmOpL();
            
        iLog->Log(_L("Search Done, check sorting"));
		// Check for any errors when multi searching
		if (iMultiLandmarkSearch->NumOfSearchErrors() != 0) 
		{
			iLog->Log(_L("Errors after multi db search"));
         	User::Leave(-1);
		}
		
		RArray<TPosLmItemId>* result = &aSearchResults[j]->SearchResult();
        
		TInt number = iDisplayData->Count();
		buf.Zero();
		buf.Format(_L("Nr in DisplayData: %d Nr in result %d"), number, result->Count());
		iLog->Log(buf);
		TInt nrOfNoPos = 0;
        AssertTrueSecL(iDisplayData->Count() == result->Count(),_L("Mismatch in number of matches"));		
        for (TInt i=0;i<iDisplayData->Count();i++)
            {
            CPosLmDisplayItem& item = iDisplayData->DisplayItem(i);
            TInt lmId = item.Landmark().LandmarkId();
                    	
        	TInt err;
            TPtrC name;
            err = item.Landmark().GetLandmarkName(name);
            AssertTrueSecL(err == KErrNotFound,_L("GetLandmarkName returned name"));
            TReal32 radius;
            err = item.Landmark().GetCoverageRadius(radius);
            AssertTrueSecL(err == KErrNotFound,_L("GetLandmarkName returned name"));
            TPtrC desc;
            err = item.Landmark().GetLandmarkDescription(desc);
            AssertTrueSecL(err == KErrNone,_L("GetLandmarkDescription returned error"));
            TLocality loc;
            err = item.Landmark().GetPosition(loc);
            
            // Should only be three landmarks without position
            if (err == KErrNotFound) nrOfNoPos++;

            }
            
        if (nrOfNoPos > 3)
            {
            iLog->Log(_L("ERROR: GetPosition returned error"));
            iErrorsFound++;
            }
        
        CleanupStack::PopAndDestroy(&reqFields);
        }
        
        
    CleanupStack::PopAndDestroy(partialParam);

    CleanupStack::PopAndDestroy(textCriteria);
    iMultiLandmarkSearch->UnsetDisplayData();
    delete iDisplayData;
    iDisplayData = NULL;
    delete iMultiLandmarkSearch;
    iMultiLandmarkSearch = NULL;
    
    }

// ---------------------------------------------------------
// CPosTp133::SearchPartialLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp133::SearchPartialLandmarksL(TInt aWhatIndexToSerrachFor, const RPointerArray<CSearchResult>& aSearchResults,
									CDesCArray* aDbUris)
    {   
   	// Search in all lm dbs
   	iMultiLandmarkSearch = CPosLmMultiDbSearch::NewL(*aDbUris);
   	// Set DisplayData
	iDisplayData = CPosLmDisplayData::NewL();
    iMultiLandmarkSearch->SetDisplayData(*iDisplayData);
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
    TBuf<150> buf;
    
    textCriteria->SetTextL(aSearchResults[aWhatIndexToSerrachFor]->TextCriteria());
    textCriteria->SetAttributesToSearch(aSearchResults[aWhatIndexToSerrachFor]->Attributes());
    textCriteria->SetPositionFieldsToSearchL(aSearchResults[aWhatIndexToSerrachFor]->PositionFields()); 

    RArray<TUint> reqFields;
    CleanupClosePushL(reqFields);
    
    iLog->Log(aSearchResults[aWhatIndexToSerrachFor]->TextCriteria());
    CPosLmPartialReadParameters* partialParam = CPosLmPartialReadParameters::NewLC();
    
    CPosLmPartialReadParameters* fakePartialParam = CPosLmPartialReadParameters::NewLC();
    reqFields.Append(EPositionFieldStreet);
    fakePartialParam->SetRequestedPositionFields(reqFields);
    reqFields.Reset();
        	
	// Search for all combinations from 0 - CPosLandmark::EDescription
	// Ex attr=0, attr=1, attr=2, attr=3, .....attr=63,
    TUint max = CPosLandmark::EDescription;
    max <<= 1;
    max -= 1;
    
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
        	
        partialParam->SetRequestedPositionFields(reqFields);
    	iDisplayData->SetPartialReadParametersL(*partialParam);
    	
        for (TInt i=0; i<2; i++)
            {
            switch (i)
                {
                case 0:
                	{
                	//**** async
                	iLog->Log(_L("*****Async, EDescending"));
                	sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
                    iAscendedSorted = EFalse;
                    iLastNrOfDisplayData=0;
                    iOperation = iMultiLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref, aSearchResults[aWhatIndexToSerrachFor]->Redefined());
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
                    iLog->Log(_L("*****Async EAscending"));
                    iAscendedSorted = ETrue;
                    iLastNrOfDisplayData=0;
                    iOperation = iMultiLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref, aSearchResults[aWhatIndexToSerrachFor]->Redefined());
                    ExecuteLmOpL();
                    }
                    break;
                }
                
                // Sort correct result
				iJustNowSearchResults = &aSearchResults[aWhatIndexToSerrachFor]->SearchResult();
				LandmarksSortL(*iJustNowSearchResults, sortPref);                
                //
             	iLog->Log(_L("Search Done, check sorting"));
				// Check for any errors when multi searching
				if (iMultiLandmarkSearch->NumOfSearchErrors() != 0)
				{
					iLog->Log(_L("Errors after multi db search"));
         			User::Leave(-1);
				}
				
				TInt number = iDisplayData->Count();
				buf.Zero();
				buf.Format(_L("Nr in DisplayData: %d Nr in result %d"), number, iJustNowSearchResults->Count());
				iLog->Log(buf);
				
				AssertTrueSecL(iDisplayData->Count() == iJustNowSearchResults->Count(),_L("Mismatch in number of matches"));
				
		        for (TInt i=0;i<iDisplayData->Count();i++)
		            {
		            CPosLmDisplayItem& item = iDisplayData->DisplayItem(i);
		            TInt lmId = item.Landmark().LandmarkId();
		            CPosLandmark* target = iDatabase->ReadLandmarkLC((*iJustNowSearchResults)[i]);
		            TPtrC sourceName, targetName;

		            item.Landmark().GetLandmarkName(sourceName);
		            target->GetLandmarkName(targetName);
					buf.Zero();
				
		            if (sourceName.CompareC(targetName) != 0)
		            	{
		            
		            	iLog->Log(_L("Mismatch between landmarks"));
         				User::Leave(-1);
		            	}
					CleanupStack::PopAndDestroy(target);
					
					// Landmarkname is always included when sorting
                	CheckPartialLandmarkL(item.Landmark() , attr | CPosLandmark::ELandmarkName, reqFields);
		            }
                
        } 
        } 
        
    CleanupStack::PopAndDestroy(fakePartialParam);
    CleanupStack::PopAndDestroy(partialParam);
    CleanupStack::PopAndDestroy(&reqFields);
    CleanupStack::PopAndDestroy(textCriteria);
    iMultiLandmarkSearch->UnsetDisplayData();
    delete iDisplayData;
    iDisplayData = NULL;
    delete iMultiLandmarkSearch;
    iMultiLandmarkSearch = NULL;
    }


// ---------------------------------------------------------
// CPosTp133::CheckResetL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp133::CheckResetL(const RPointerArray<CSearchResult>& aSearchResults, CDesCArray* aDbUris)
    {
    iMultiLandmarkSearch = CPosLmMultiDbSearch::NewL(*aDbUris);
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
    TBuf<150> buf;
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();	
    
	iDisplayData = CPosLmDisplayData::NewL();
	iMultiLandmarkSearch->SetDisplayData(*iDisplayData);
	iAscendedSorted = ETrue;
    
	for(TInt j=0; j<aSearchResults.Count(); j++)
        {
        textCriteria->SetTextL(aSearchResults[j]->TextCriteria());
        textCriteria->SetAttributesToSearch(aSearchResults[j]->Attributes());
        
        if (aSearchResults[j]->PositionFields().Find(803) != KErrNotFound)
            {
            // Stupid fix, since the multi dbs have not been re-generated against correct LBSFieldsid.h
            // they contain the 802 field (instead of 803) so append this id as well
            RArray<TUint> arr;
            iLog->Log(_L("Found 803"));
            arr = aSearchResults[j]->PositionFields();
            arr.Append(802);
            textCriteria->SetPositionFieldsToSearchL(arr);
            }
        else
            {
            textCriteria->SetPositionFieldsToSearchL(aSearchResults[j]->PositionFields());
            }
        
        iLog->Log(_L(">>>>>>Searching for: "));
        iLog->Log(aSearchResults[j]->TextCriteria());
			
        RArray<TPosLmItemId>* result = &aSearchResults[j]->SearchResult();
        
		LandmarksSortL(*result, sortPref);
		iLastNrOfDisplayData = 0;
		iOperation = iMultiLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref, aSearchResults[j]->Redefined());
		ExecuteAndDeleteLD(iOperation);
		
		iLastNrOfDisplayData = 0;
		iDisplayData->Reset();
		AssertTrueSecL(iDisplayData->Count() == 0,_L("DisplayData should have been resetted"));
		
		iOperation = iMultiLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref, aSearchResults[j]->Redefined());
        // Also check progress here
        ExecuteLmOpL();
		AssertTrueSecL(iDisplayData->Count() == result->Count(),_L("Mismatch in number of matches"));						
		}
		
	iMultiLandmarkSearch->UnsetDisplayData();
	iDisplayData->Reset();
	iMultiLandmarkSearch->SetDisplayData(*iDisplayData);
	AssertTrueSecL(iDisplayData->Count() == 0,_L("DisplayData should have been resetted"));
	
	// Unset display data
	iMultiLandmarkSearch->UnsetDisplayData();
	
	textCriteria->SetTextL(aSearchResults[0]->TextCriteria());
    textCriteria->SetAttributesToSearch(aSearchResults[0]->Attributes());
    textCriteria->SetPositionFieldsToSearchL(aSearchResults[0]->PositionFields());
    
    // Check that no result is found in displaydata
    iOperation = iMultiLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref, aSearchResults[0]->Redefined());
	ExecuteAndDeleteLD(iOperation);
	AssertTrueSecL(iDisplayData->Count() == 0,_L("No matches should be found in DisplayData1"));
	
	iMultiLandmarkSearch->SetDisplayData(*iDisplayData);
	iOperation = iMultiLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref, aSearchResults[0]->Redefined());
	ExecuteAndDeleteLD(iOperation);
	
	// Check that result is found in displaydata
	RArray<TPosLmItemId>* searchResults = &aSearchResults[0]->SearchResult();
   	AssertTrueSecL(iMultiLandmarkSearch->TotalNumOfMatches() == iDisplayData->Count(),_L("Wrong number of matches1"));
    AssertTrueSecL(iMultiLandmarkSearch->TotalNumOfMatches() == searchResults->Count(),_L("Wrong number of matches2"));
				
   	CleanupStack::PopAndDestroy(textCriteria);
   	
   	iMultiLandmarkSearch->UnsetDisplayData();
	delete iDisplayData;iDisplayData=NULL;
	
	delete iMultiLandmarkSearch;
    iMultiLandmarkSearch = NULL;
    }

// ---------------------------------------------------------
// CPosTp133::CancelTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp133::CancelTestL(const RPointerArray<CSearchResult>& aSearchResults, CDesCArray* aDbUris,
					TBool aCancelInCallback)
    {
    iMultiLandmarkSearch = CPosLmMultiDbSearch::NewL(*aDbUris);
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
    TBuf<150> buf;
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();	
    
    // Search for wildcard, index 0 in lmdbsearchresult.txt
	TInt KWhatToSearchForIndex = 0;
	
        textCriteria->SetTextL(aSearchResults[KWhatToSearchForIndex]->TextCriteria());
        textCriteria->SetAttributesToSearch(aSearchResults[KWhatToSearchForIndex]->Attributes());
        textCriteria->SetPositionFieldsToSearchL(aSearchResults[KWhatToSearchForIndex]->PositionFields()); 
        
        iLog->Log(_L(">>>>>>Searching for: "));
        iLog->Log(aSearchResults[KWhatToSearchForIndex]->TextCriteria());

		// Sort ascended and descended
		for (TInt sorted=0;sorted<2;sorted++)
			{
			iDisplayData = CPosLmDisplayData::NewL();
	    	iMultiLandmarkSearch->SetDisplayData(*iDisplayData);
			
			switch (sorted)
	        	{
                case 0:
                iLog->Log(_L("******Sorting EAscending*******"));
                iAscendedSorted = ETrue;
                sortPref = TPosLmSortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
                break;
                case 1:
                iLog->Log(_L("**********Sorting EDescending**********"));
                iAscendedSorted = EFalse;
                sortPref = TPosLmSortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
                break;
                }

            iJustNowSearchResults = &aSearchResults[KWhatToSearchForIndex]->SearchResult();
			LandmarksSortL(*iJustNowSearchResults, sortPref);
			iLastNrOfDisplayData = 0;
			
			iOperation = iMultiLandmarkSearch->StartLandmarkSearchL(*textCriteria, sortPref, aSearchResults[KWhatToSearchForIndex]->Redefined());

			switch (aCancelInCallback)
				{
					case EFalse:
					iLog->Log(_L("Cancel"));
					RunAsyncOperationAndCancelLD(iOperation);
					break;
					case ETrue:
					iLog->Log(_L("Cancel In Callback"));
					RunAsyncOperationAndCancelInCallbackLD(iOperation);
					break;
				}
			
			
			buf.Format(_L("TotalNumOfMatches returned %d matches and iDisplayData %d matches after cancel"), iMultiLandmarkSearch->TotalNumOfMatches(), iDisplayData->Count());
			iLog->Log(buf);
			
			iLog->Log(_L("After Cancel, check result"));
			// Check for any errors when multi searching
			// XXX Shouldnt error be KErrCancel
			if (iMultiLandmarkSearch->NumOfSearchErrors() != 0) iLog->Log(_L("Errors after cancel"));
			for (TInt i=0;i<iMultiLandmarkSearch->NumOfSearchErrors(); i++)
				{
	    		CPosLmMultiDbSearch::TSearchError searchErr;
	    		iMultiLandmarkSearch->GetSearchError(i, searchErr);
	    		
	    		buf.Format(_L("ErrorCode %d returned from db nr %d"),searchErr.iErrorCode, searchErr.iDatabaseIndex);
	    		iLog->Log(buf);
	    		
	    		}
			
			TInt number = iDisplayData->Count();
			buf.Zero();
			buf.Format(_L("Nr in DisplayData: %d Nr in result %d"), number, iJustNowSearchResults->Count());
			iLog->Log(buf);
			
			if (!aCancelInCallback)
				{
				if (iDisplayData->Count() != 0) 
					{
					iLog->Log(_L("No matches should be found when cancel before search started"));
         			User::Leave(-1);
					}
				}
			else
			{
			// XXX maybe check result here
			}
			
	    	iMultiLandmarkSearch->UnsetDisplayData();
		    delete iDisplayData;iDisplayData=NULL;
		    }
   	CleanupStack::PopAndDestroy(textCriteria);
	
	delete iMultiLandmarkSearch;
	iMultiLandmarkSearch = NULL;
	}

// ---------------------------------------------------------
// Tp133InternalLandmarksSortL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//	
TInt Tp133InternalLandmarksSortL(const CPosLandmark& aSource, const CPosLandmark& aTarget)
    {
    TPtrC sourceName, targetName;
    
    User::LeaveIfError(aSource.GetLandmarkName(sourceName));  
    User::LeaveIfError(aTarget.GetLandmarkName(targetName));
    
    return sourceName.CompareC(targetName);
    }

// ---------------------------------------------------------
// CPosTp133::LandmarksSortL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp133::LandmarksSortL(RArray<TPosLmItemId>& aArray, const TPosLmSortPref& aSortPref)
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
    
    TLinearOrder<CPosLandmark> order(Tp133InternalLandmarksSortL);
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
// CPosTp133::ExecuteLmOpL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp133::ExecuteLmOpL()
    {
    CActiveLmOperation* operationWrapper = new (ELeave) CActiveLmOperation(iLog);
    CleanupStack::PushL(operationWrapper);
	
	// Start operation with this class as observer
	operationWrapper->Start(iOperation, this);        
    CActiveScheduler::Start();
    
    operationWrapper->CheckOperationL();
    operationWrapper->DeleteOperationD();

    CleanupStack::PopAndDestroy(operationWrapper);
    }

// ---------------------------------------------------------
// CPosTp133::ExecuteLmOpInCallbackL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp133::ExecuteLmOpInCallbackL()
    {
        CActiveLmOperation* activeLmOperation = new(ELeave) CActiveLmOperation(iLog, iUseLogFromThreadIsDisabled);
    CleanupStack::PushL(activeLmOperation);

    TCallBack callback(CLandmarkTestProcedureBase::CancelTest, activeLmOperation);

    CPeriodic* periodicTimer = CPeriodic::NewL(CActive::EPriorityHigh); 
    CleanupStack::PushL(periodicTimer);

    periodicTimer->Start(1, 1, callback);
    activeLmOperation->SetCancelTest();
    // Start operation with this class as observer
    activeLmOperation->Start(iOperation, this);
    
    CActiveScheduler::Start();
    periodicTimer->Cancel();
    
    CleanupStack::PopAndDestroy(2, activeLmOperation);
    }
        
// ---------------------------------------------------------
// CPosTp133::NotifyProgress
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp133::NotifyProgress(TReal aProgress)
    {
    TBuf<100> buf;
    TInt progress = (TInt) (aProgress * 100);
    buf.Format(_L("Progress: %d"), progress);
    TInt nrOfMatches = 0;

    if (iDisplayData)
    	{
	    buf.Zero();
	    buf.Format(_L("Progress: %g DisplayData %d"), aProgress * 100, iDisplayData->Count());
	    
	    // Check that nr of matches never decreases
	    if (iLastNrOfDisplayData>iDisplayData->Count())
	    	{
	    	iLog->Log(_L("ERROR: Less nr of mathes than in last search"));
	    	iErrorsFound++;
	    	}
	    
	    if (iDisplayData->Count()>0)
			{
			TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
	        // Check nr of hits for each db using the iterator
			for (TInt i=0;i<KNrOfDatabases;i++)
				{
				CPosLmItemIterator* iter = iMultiLandmarkSearch->MatchIteratorL(i);
				CleanupStack::PushL(iter);
				if (iter->NumOfItemsL() > 0) 
					{
					buf.Zero();
					buf.Format(_L("Found %d nr of lm in db %d"), iter->NumOfItemsL(), i);
				
					nrOfMatches+=iter->NumOfItemsL();
		        	}
		        CleanupStack::PopAndDestroy(iter);
	        	}
   	        	
	        if (iAscendedSorted) sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
	        else sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
            	
            for (TInt i=0;i<iDisplayData->Count();i++)
	            {
	            // Check for all but first and last
	            if (i>0 && i<iDisplayData->Count()-1)
	            	{
	            	CPosLmDisplayItem& item = iDisplayData->DisplayItem(i);
	            	TInt lmId = item.Landmark().LandmarkId();
	            
	            	CPosLmDisplayItem& nextItem = iDisplayData->DisplayItem(i+1);
	            	TInt nextLmId = item.Landmark().LandmarkId();
	            	
	            	TPtrC name, nextLmName;
	            	item.Landmark().GetLandmarkName(name);
	            	nextItem.Landmark().GetLandmarkName(nextLmName);
	            
	            	TInt order = nextLmName.CompareC(name);
                    if (iAscendedSorted)
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
					} // if (i>0 && i<iDisplayData->Count()-1)
				} // for (TInt i=0;i<iDisplayData->Count();i++)
			} // if (iDisplayData->Count()>0)
			iLastNrOfDisplayData = iDisplayData->Count();
		} //if (iDisplayData)
	}

        
//  End of File
