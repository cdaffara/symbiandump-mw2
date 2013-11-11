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

#include "FT_CPosTp35.h"
#include <f32file.h>
#include <e32math.h>
#include <e32math.h>

#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkSearch.h>
#include <EPos_CPosLMItemIterator.h>
#include <EPos_CPosLmDatabaseManager.h>
#include "FT_LandmarkConstants.h" 

// CONSTANTS
const TInt KMaxFieldLength = 80;
const TInt KDistanceFieldLength = 24;
const TInt KMaxNearestLineLength = 1024;

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp35::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp35::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    delete iDatabase;
    iDatabase = NULL;
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;

    iNearestSearchResults.ResetAndDestroy();
    }

// ---------------------------------------------------------
// CPosTp35::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp35::StartL()
    {
    RemoveDefaultDbL();
    RemoveAllLmDatabasesL();
    iDatabase = UseNearestSearchGeneratedDbFileL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
    
    AppendNearestSearchResultsL();
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    
    iLog->Log(_L("Testing search with criterias read from LmDbNearestResult.txt syncronously"));
    SearchL();

    iLog->Log(_L("Testing search with criterias read from LmDbNearestResult.txt asyncronously"));
    SearchL(ETrue);

    iLog->Log(_L("Testing search and cancel"));
    SearchAndCancelL();

    iLog->Log(_L("Testing search and cancel in callback"));
    SearchAndCancelL(ETrue);

    iLog->Log(_L("Testing search with a sortorder"));
    SearchWithSortOrderL();

    iLog->Log(_L("Testing error codes"));
    _LIT(KStartCatErr, "StartCategorySearch should leave with KErrArgument with nearest criteria");
    _LIT(KNaNErr, "Max distance should be NaN by default");

    TCoordinate coord(12, 13);
    CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(coord);
    AssertTrueSecL(Math::IsNaN(nearestCriteria->MaxDistance()), KNaNErr);

    TRAPD(err, iLandmarkSearch->StartCategorySearchL(*nearestCriteria, CPosLmCategoryManager::ECategorySortOrderNone));
    AssertTrueSecL(err == KErrArgument, KStartCatErr);
    
    CleanupStack::PopAndDestroy(nearestCriteria);
        
    TCoordinate pos(55, 18);    
    nearestCriteria = CPosLmNearestCriteria::NewLC(pos, ETrue);
    nearestCriteria->SetMaxDistance(1);
    RunAsyncOperationLD(iLandmarkSearch->StartLandmarkSearchL(*nearestCriteria, EFalse));
    
    _LIT(KReturned, "Returned no. of matches: %d");
    TBuf<100> info;
    info.Format(KReturned, iLandmarkSearch->NumOfMatches());
    iLog->Log(info);
        
    CleanupStack::PopAndDestroy(nearestCriteria);
    
    nearestCriteria = CPosLmNearestCriteria::NewLC(pos, ETrue);
    nearestCriteria->SetMaxDistance(300);
    RunAsyncOperationLD(iLandmarkSearch->StartLandmarkSearchL(*nearestCriteria, EFalse));
    
    info.Format(KReturned, iLandmarkSearch->NumOfMatches());
    iLog->Log(info);
    CleanupStack::PopAndDestroy(nearestCriteria);

    }

// ---------------------------------------------------------
// CPosTp35::UseNearestSearchGeneratedDbFileL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CPosTp35::UseNearestSearchGeneratedDbFileL()
    {    
    _LIT(KDbFile, "eposlmnearestsearch.ldb");
    CopyTestDbFileL(KDbFile);
    
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);
    // This db must exist
    manager->SetDefaultDatabaseUriL(KDbFile);
    CleanupStack::PopAndDestroy(manager);
    
    return CPosLandmarkDatabase::OpenL(KDbFile);
    }

// ---------------------------------------------------------
// CPosTp35::SearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp35::SearchL(const TBool& aSync)
    {
   
    for(TInt i=0; i<iNearestSearchResults.Count(); i++)
        {
        TCoordinate coord(iNearestSearchResults[i]->iLatitude, iNearestSearchResults[i]->iLongitude);
        iLog->Log(_L("SearchL"));
        CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(coord, iNearestSearchResults[i]->iUseCoverageRadius);
        nearestCriteria->SetMaxDistance(iNearestSearchResults[i]->iMaxDistance);
        
        TBuf<100> pelle;
        pelle.Format(_L("StartSearchOperationL: Searching for Lat %f Long %f"), iNearestSearchResults[i]->iLatitude, iNearestSearchResults[i]->iLongitude);
		iLog->Log(pelle);

        if (iNearestSearchResults[i]->iMaxSearchResult > 0)
            {
            iLandmarkSearch->SetMaxNumOfMatches(iNearestSearchResults[i]->iMaxSearchResult);
            }
        else
            {
            iLandmarkSearch->SetMaxNumOfMatches(KPosLmMaxNumOfMatchesUnlimited);
            }

        if (aSync)
            {
            RunAsyncOperationLD(iLandmarkSearch->StartLandmarkSearchL(*nearestCriteria, iNearestSearchResults[i]->Redefined()));
            }
        else
            {
            ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(*nearestCriteria, iNearestSearchResults[i]->Redefined()));
            }
        
        if (iNearestSearchResults[i]->Redefined() && i > 0)
            {
            iLog->Log(_L("Redefined"));
            
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
        
        CheckSearchResultL(*iNearestSearchResults[i]); 
    
        CleanupStack::PopAndDestroy(nearestCriteria);   
        }
    }

// ---------------------------------------------------------
// CPosTp35::CheckSearchResultL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//

//void CPosTp35::CheckSearchResultL(CNearestSearchResult& aSearchResult)
void CPosTp35::CheckSearchResultL(CNearestSearchResultTp35& aSearchResult)
    {    
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
    
    TUint expectedNoMatches = aSearchResult.iDistances.Count();
    if (aSearchResult.iMaxSearchResult != 0 && aSearchResult.iMaxSearchResult < aSearchResult.iDistances.Count())
        {
        expectedNoMatches = aSearchResult.iMaxSearchResult;
        }
    
    if (expectedNoMatches != iLandmarkSearch->NumOfMatches())
        {
        _LIT(KExpected, "Expected no. of matches: %d");
        _LIT(KReturned, "Returned no. of matches: %d");
        TBuf<100> info;
        info.Format(KExpected, expectedNoMatches);
        iLog->Log(info);
        info.Format(KReturned, iLandmarkSearch->NumOfMatches());
        iLog->Log(info);
       
        
        iLog->Log(KNumOfMatchesErr);
        User::Leave(-1);
        }

    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
    
    TInt index=0;
    TPosLmItemId id = iter->NextL();
    while (id != KPosLmNullItemId)
        {
        if (aSearchResult.iDistances[index].iId != id)
            {
            // if the distance is equal to the distance to the landmark  
            // before or after then no error. 
            // This to be able to use a databse with landmarks with the same position.
            
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
                _LIT(KUnexpectedItemId, "Unexpected item id");    
                _LIT(KExpected, "Expected item id: %d");
                _LIT(KReturned, "Returned item id: %d");
                TBuf<100> info;
                info.Format(KExpected, aSearchResult.iDistances[index].iId);
                iLog->Log(info);
                info.Format(KReturned, id);
                iLog->Log(info);

                TBuf<800> info2;
                aSearchResult.Print(info2);
                iLog->Log(info2);

                
                iLog->Log(KUnexpectedItemId);
        		User::Leave(-1);
        
                }
            }
        _LIT(KReturned, "Returned id: %d or id: %d");
        TBuf<100> info;
        info.Format(KReturned, aSearchResult.iDistances[index].iId, id);
        //iLog->Log(info);
                
        id = iter->NextL();
        index++;
        }
    
    CleanupStack::PopAndDestroy(iter);
  
    }

// ---------------------------------------------------------
// CPosTp35::SearchAndCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp35::SearchAndCancelL(const TBool& aInCallback)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");

    // Test search with one criteria
    const TInt KIndex=0;

    TCoordinate coord(iNearestSearchResults[KIndex]->iLatitude, iNearestSearchResults[KIndex]->iLongitude);
        
    CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(coord, iNearestSearchResults[KIndex]->iUseCoverageRadius);
    
    iLandmarkSearch->SetMaxNumOfMatches(KPosLmMaxNumOfMatchesUnlimited);
    if (aInCallback)
        {
        RunAsyncOperationAndCancelInCallbackLD(iLandmarkSearch->StartLandmarkSearchL(
                        *nearestCriteria, 
                        iNearestSearchResults[KIndex]->Redefined()));
        }
    else
        {
        RunAsyncOperationAndCancelLD(iLandmarkSearch->StartLandmarkSearchL(
                        *nearestCriteria, 
                        iNearestSearchResults[KIndex]->Redefined()));
        }

    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
 
    if (aInCallback)
        {
        if (iter->NumOfItemsL() != iLandmarkSearch->NumOfMatches())
            {
            iLog->Log(_L("Iterators no. of items is not equal to LandmarksSearch no. matches"));
            _LIT(KExpected, "Iter. of matches: %d");
            _LIT(KReturned, "LandmarksSearch no. of matches: %d");
            TBuf<100> info;
            info.Format(KExpected, iter->NumOfItemsL());
            iLog->Log(info);
            info.Format(KReturned, iLandmarkSearch->NumOfMatches());
            iLog->Log(info);
            
            iLog->Log(KNumOfMatchesErr);
        	User::Leave(-1);
        
            }
        
        TBuf<100> info2;
        _LIT(KNof, "No. of matches found before cancelled the operation = %d");
        info2.Format(KNof, iter->NumOfItemsL());
        iLog->Log(info2);

        // FrBo: No longer sure that result is returned when cancelling
        //AssertTrueSecL(iter->NumOfItemsL() != 0, KCancelErr);
        }
        
    CleanupStack::PopAndDestroy(2, nearestCriteria);
    
    }

// ---------------------------------------------------------
// CPosTp35::SearchWithSortOrderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp35::SearchWithSortOrderL()
    {
    // Only verifies that search does not fail when only landmarks with no name in the db

    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed with a sortorder");
   
    // Test search with one criteria which return all landmarks in db
    const TInt KIndex=0;

    TCoordinate coord(iNearestSearchResults[KIndex]->iLatitude, iNearestSearchResults[KIndex]->iLongitude);
        
    CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(coord, iNearestSearchResults[KIndex]->iUseCoverageRadius);
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
   
    iLandmarkSearch->SetMaxNumOfMatches(KPosLmMaxNumOfMatchesUnlimited);
   
    for (TInt i=0; i<4; i++)
        {
        if(i==0)
            {
            ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(
                        *nearestCriteria,
                        sortPref,
                        iNearestSearchResults[KIndex]->Redefined()));
            }
        else if (i==1)
            {
            sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
            ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(
                        *nearestCriteria, 
                        sortPref,
                        iNearestSearchResults[KIndex]->Redefined()));
            }
        //**** async
        else if (i==2)
            {
            RunAsyncOperationLD(iLandmarkSearch->StartLandmarkSearchL(
                        *nearestCriteria, 
                        sortPref,
                        iNearestSearchResults[KIndex]->Redefined()));
            }
        else if (i==3)
            {
            sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
            
            RunAsyncOperationLD(iLandmarkSearch->StartLandmarkSearchL(
                        *nearestCriteria,
                        sortPref,
                        iNearestSearchResults[KIndex]->Redefined()));
            }
        AssertTrueSecL((TUint)iNearestSearchResults[KIndex]->iDistances.Count()== iLandmarkSearch->NumOfMatches(), KNumOfMatchesErr);    
        }

    CleanupStack::PopAndDestroy(nearestCriteria);
    }

// ---------------------------------------------------------
// CPosTp35::AppendAreaSearchResultsL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CPosTp35::AppendNearestSearchResultsL()
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
            //CNearestSearchResult* searchResult = CNearestSearchResult::NewL(line, row);
            CNearestSearchResultTp35* searchResult = CNearestSearchResultTp35::NewL(line, row, iFileSession);
            CleanupStack::PushL(searchResult);
       
            User::LeaveIfError(iNearestSearchResults.Append(searchResult));
            CleanupStack::Pop(searchResult);
                   
            }
        }
    CleanupStack::PopAndDestroy(&file);    
    }

// ---------------------------------------------------------
// CNearestSearchResult::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//CNearestSearchResult* CNearestSearchResult::NewL(const TDesC& aDes, const TInt& aIndex) 
CNearestSearchResultTp35* CNearestSearchResultTp35::NewL(
    const TDesC& aDes, 
    const TInt& aIndex,
    RFs& aFileSession) 
    {
    CNearestSearchResultTp35* self = new(ELeave) CNearestSearchResultTp35( aFileSession );
    CleanupStack::PushL(self);
    self->ConstructL(aDes, aIndex);
    CleanupStack::Pop();
    return self;
    }

// C++ default constructor can NOT contain any code, that
// might leave.
//

CNearestSearchResultTp35::CNearestSearchResultTp35(RFs& aFileSession)
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
void CNearestSearchResultTp35::ConstructL(const TDesC& aDes, const TInt& aIndex) 
    {
    iDatabase = CPosLandmarkDatabase::OpenL();

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

CNearestSearchResultTp35::~CNearestSearchResultTp35()
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
void CNearestSearchResultTp35::AppendDistancesL(const TInt& aIndex)
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
void CNearestSearchResultTp35::ParseDistanceL(const TDesC& aDes, 
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
                            TSearchResultTp35 searchResult(aId, TReal32(dist));
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
//TReal64 CNearestSearchResult::Latitude() const
TReal64 CNearestSearchResultTp35::Latitude() const
    {
    return iLatitude;
    }

// ---------------------------------------------------------
// CNearestSearchResult::Latitude 
//
// (other items were commented in a header).
// ---------------------------------------------------------
//   
//TReal64 CNearestSearchResult::Longitude() const
TReal64 CNearestSearchResultTp35::Longitude() const
    {
    return iLongitude;
    }

// ---------------------------------------------------------
// CNearestSearchResult::UseCoverageRadius 
//
// (other items were commented in a header).
// ---------------------------------------------------------
//           
//TBool CNearestSearchResult::UseCoverageRadius() const
TBool CNearestSearchResultTp35::UseCoverageRadius() const
    {
    return iUseCoverageRadius;
    }

// ---------------------------------------------------------
// CNearestSearchResult::MaxDistance
//
// (other items were commented in a header).
// ---------------------------------------------------------
//           

TReal32 CNearestSearchResultTp35::MaxDistance() const
    {   
    return iMaxDistance;
    }

// ---------------------------------------------------------
// CNearestSearchResult::ErrorCode 
//
// (other items were commented in a header).
// ---------------------------------------------------------
//   
//TInt CNearestSearchResult::ErrorCode() const
TInt CNearestSearchResultTp35::ErrorCode() const
    {   
    return iErrorCode;
    }

// ---------------------------------------------------------
// InternalLandmarksSortL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//   
TInt InternalSort(const TSearchResultTp35& aSource, const TSearchResultTp35& aTarget)
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
void CNearestSearchResultTp35::SortDistances() 
    {   
    TLinearOrder<TSearchResultTp35> order(InternalSort);
    iDistances.Sort(order);
    }

// ---------------------------------------------------------
// CNearestSearchResult::Print
//
// (other items were commented in a header).
// ---------------------------------------------------------
//   
void CNearestSearchResultTp35::Print(TDes& aPrint)
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
void CNearestSearchResultTp35::PrintDistances(TDes& aPrint)
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
TSearchResultTp35::TSearchResultTp35()
    {
    TRealX nan;
    nan.SetNaN();
    iDistance=nan;
    iId=0;
    } 

TSearchResultTp35::TSearchResultTp35(const TPosLmItemId& aId, const TReal32& aDistance) : 
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
TReal64 TSearchResultTp35::Distance() const
    {
    return iDistance;     
    }

// ---------------------------------------------------------
// TSearchResult::Id
//
// (other items were commented in a header).
// ---------------------------------------------------------
//  
TPosLmItemId TSearchResultTp35::Id() const
    {
    return iId;
    }

//  End of File
