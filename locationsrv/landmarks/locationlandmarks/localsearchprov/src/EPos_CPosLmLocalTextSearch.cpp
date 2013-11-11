/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Class for handling text search.
*
*
*/


#include <EPos_CPosLmTextCriteria.h>
#include <epos_landmarkdatabasestructure.h>
#include <epos_poslmdatabaseutility.h>
#include <EPos_CPosLandmarkSearch.h>
#include <epos_poslmlongtextcolhandler.h>

#include "epos_cposlmnameindexiterator.h"
#include "EPos_CPosLmLocalTextSearch.h"
#include "epos_poslmdisplaydatahandler.h"

_LIT( KPosLmSqlSelectWhere,          "SELECT %S FROM %S WHERE " );
_LIT( KPosLmSqlAppendAnd,            " AND " );
_LIT( KPosLmSqlAppendEqualsUint,     "%S=%u" );
_LIT( KPosLmSqlAppendOr,             " OR " );
_LIT( KPosLmSqlAppendStartParanthesis, "(" );
_LIT( KPosLmSqlAppendEndParanthesis, ")" );
_LIT( KPosLmSqlAppendStringLike,     "%S LIKE '%S'" );

const TInt KPosLmSqlSelectStatementMaxLen       = 256;
const TInt KPosLmSqlFieldTypeStatementMaxLen    = 50;
const TInt KPosLmTextSearchGranularity = 100;
const TInt KPosLmSqlAppendAndOrLen = 6;
const TInt KPosLmSqlLikeStatementLen = 50;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalTextSearch::CPosLmLocalTextSearch(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmLocalSortPref& aSortPref,
    TInt aMaxNumOfMatches)
:   CPosLmLocalSearchHandler( aDatabase, aCriteria.CriteriaType(), aSortPref, aMaxNumOfMatches ),
    iSearchStatus( ESearchNotStarted ),
    iMatchIndexes( KPosLmTextSearchGranularity ),
    iFieldMatches( KPosLmTextSearchGranularity )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalTextSearch::ConstructL(
    const CPosLmSearchCriteria& aCriteria,
    CPosLmDisplayData* aDisplayData )
    {
    const CPosLmTextCriteria& criteria =
        reinterpret_cast<const CPosLmTextCriteria&>( aCriteria );

    ValidateCriteriaL( criteria );

    iAttributes = criteria.AttributesToSearch();
    criteria.GetPositionFieldsToSearchL( iFields );
    iTextPattern.AppendL(criteria.Text().AllocL());

    // Check if the text should be searched in attributes or in position fields
    // or in both.
    if ( iFields.Count() == 0 && iAttributes == CPosLandmark::ENoAttribute )
        {
        iPositionFieldSearch = ETrue;
        iAttributeSearch = ETrue;
        iAttributes = CPosLandmark::ELandmarkName | CPosLandmark::EDescription;
        }
    else
        {
        if ( iFields.Count() > 0 )
            {
            iPositionFieldSearch = ETrue;
            }

        if ( iAttributes != CPosLandmark::ENoAttribute )
            {
            iAttributeSearch = ETrue;
            }
        }

    if ( aDisplayData )
        {
        SetDisplayDataL( aDisplayData );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalTextSearch* CPosLmLocalTextSearch::NewL(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmLocalSortPref& aSortPref,
    CPosLmDisplayData* aDisplayData,
    TInt aMaxNumOfMatches)
    {
    CPosLmLocalTextSearch* self = new (ELeave) CPosLmLocalTextSearch(
        aDatabase, aCriteria, aSortPref, aMaxNumOfMatches);
    CleanupStack::PushL(self);
    self->ConstructL(aCriteria, aDisplayData);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalTextSearch::~CPosLmLocalTextSearch()
    {
    iFields.Close();
    iTextPattern.ResetAndDestroy();
    iFieldMatches.Close();
    iMatchIndexes.Close();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalTextSearch::ValidateCriteriaL(
    const CPosLmTextCriteria& aCriteria)
    {
    // Validate that a text string is set.
    if (aCriteria.Text().Size() == 0)
        {
        User::Leave(KErrArgument);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalTextSearch::IsValidForLandmarkSearch() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalTextSearch::NextStepL( TReal32& aProgress )
    {
    if ( iSearchStatus == ESearchNotStarted )
    	{
    	PrepareSearchL();
    	}

    switch ( iSearchStatus )
        {
        case EPrepareUnsortedSearch:
        case EPrepareUnsortedFieldSearch:
        case EUnsortedAttributeSearch:
        case EUnsortedFieldSearch:
            UnsortedSearchL();
            break;
        case EPrepareSortedLimitedSearch:
        case EPrepareSortedLimitedAttributeSearch:
        case ESortedLimitedAttributeSearch:
        case ESortedLimitedFieldSearch:
            SortedLimitedSearchL();
            break;
        case EPrepareUnlimitedSearch:
        case EPrepareUnlimitedFieldSearch:
        case EUnlimitedAttributeSearch:
        case EUnlimitedFieldSearch:
           	SortedUnlimitedSearchL();
            break;
        default:
            break;
        }
    
    return SearchStepCompletedL( iSearchStatus == ESearchCompleted, aProgress );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalTextSearch::PrepareSearchL()
    {
    if ( SortPref().SortType() == TPosLmLocalSortPref::ELandmarkSorting )
        {
        iSortSearch = ETrue;
        if ( MaxNumOfMatches() == KPosLmMaxNumOfMatchesUnlimited )
            {
            iSearchStatus = EPrepareUnlimitedSearch;
            }
        else
            {
            iSearchStatus = EPrepareSortedLimitedSearch;
            }
        }
    else
        {
        iSortSearch = EFalse;
        iSearchStatus = EPrepareUnsortedSearch;
        }

    TInt totalRowCount = 0;
    if ( iPositionFieldSearch )
        {
        totalRowCount += GetRowCountL( KPosLmLandmarkFieldTable );
        }

    if ( iAttributeSearch )
        {
        totalRowCount += GetRowCountL( KPosLmLandmarkTable );
        }

    if ( totalRowCount == 0 )
        {
        iSearchStatus = ESearchCompleted;
        }
    else
        {
        PrepareCalculateProgress( totalRowCount );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
HBufC* CPosLmLocalTextSearch::CreatePosFieldQueryLC()
    {
    TInt ln = 0;
    for(int i=0; i< iTextPattern.Count(); i++)
    	ln += iTextPattern[i]->Length() + KPosLmSqlAppendAndOrLen + KPosLmSqlLikeStatementLen;
    	
    TInt maxLength = 
        KPosLmSqlSelectStatementMaxLen + 
        ln +
        (iFields.Count() * KPosLmSqlFieldTypeStatementMaxLen);
    
    HBufC* query = HBufC::NewLC( maxLength );
    query->Des().Format( KPosLmSqlSelectWhere,
                         &KPosLmLandmarkIdCol,
                         &KPosLmLandmarkFieldTable );

    if ( iFields.Count() != 0 )
        {
        // Include position field types in query
        query->Des().Append( KPosLmSqlAppendStartParanthesis );

        TInt i;
        for ( i = 0; i < iFields.Count() - 1; i++ )
            {
            query->Des().AppendFormat( KPosLmSqlAppendEqualsUint,
                                       &KPosLmFieldTypeCol,
                                       iFields[i] );
            query->Des().Append( KPosLmSqlAppendOr );
            }

        // last field
        query->Des().AppendFormat( KPosLmSqlAppendEqualsUint,
                                   &KPosLmFieldTypeCol,
                                   iFields[i] );
        query->Des().Append( KPosLmSqlAppendEndParanthesis );
        
        query->Des().Append( KPosLmSqlAppendAnd );
        }

	query->Des().Append( KPosLmSqlAppendStartParanthesis );
	
    for(int i=0; i< iTextPattern.Count() - 1; i++)
	{
		HBufC* textPattern = 
    	PosLmDatabaseUtility::EmbedSingleQuoteCharactersLC( *iTextPattern[i] );

     	query->Des().AppendFormat( 
        	KPosLmSqlAppendStringLike,
        	&KPosLmFieldStringCol,
        	textPattern );
        if(iCompositionType == 0)
        	query->Des().Append( KPosLmSqlAppendAnd );
        else
        	query->Des().Append( KPosLmSqlAppendOr );
        
        CleanupStack::PopAndDestroy( textPattern );
	}

	//last pattern
	HBufC* textPattern = PosLmDatabaseUtility::EmbedSingleQuoteCharactersLC( *iTextPattern[iTextPattern.Count() - 1] );
	query->Des().AppendFormat( 
        	KPosLmSqlAppendStringLike,
        	&KPosLmFieldStringCol,
        	textPattern );
    query->Des().Append( KPosLmSqlAppendEndParanthesis );
    CleanupStack::PopAndDestroy( textPattern );
    
    return query;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
HBufC* CPosLmLocalTextSearch::CreateAttributeQueryLC()
    {
    TInt ln = 0;
    for(int i=0; i< iTextPattern.Count(); i++)
    	ln += iTextPattern[i]->Length() + KPosLmSqlAppendAndOrLen + KPosLmSqlLikeStatementLen;
    	
    TInt maxLength = KPosLmSqlSelectStatementMaxLen + 2 * ln;

    HBufC* query = HBufC::NewLC( maxLength );
    query->Des().Format( 
        KPosLmSqlSelectWhere, &KPosLmSqlAll, &KPosLmLandmarkTable );

    

    TInt conditions = 0;
    if ( iAttributes & CPosLandmark::ELandmarkName )
        {
        query->Des().Append( KPosLmSqlAppendStartParanthesis );
        for(int i=0; i< iTextPattern.Count() - 1; i++ )
        {
        	HBufC* textPattern = PosLmDatabaseUtility::EmbedSingleQuoteCharactersLC( *iTextPattern[i] );
            query->Des().AppendFormat(KPosLmSqlAppendStringLike, &KPosLmNameCol, textPattern );
	        if(iCompositionType == 0)
	        	query->Des().Append( KPosLmSqlAppendAnd );
	        else
	        	query->Des().Append( KPosLmSqlAppendOr );
	        
	        CleanupStack::PopAndDestroy( textPattern );
        }
        
        //last pattern
        HBufC* textPattern = PosLmDatabaseUtility::EmbedSingleQuoteCharactersLC( *iTextPattern[iTextPattern.Count() - 1] );
        query->Des().AppendFormat(KPosLmSqlAppendStringLike, &KPosLmNameCol, textPattern );
        CleanupStack::PopAndDestroy( textPattern );
        conditions++;
        query->Des().Append( KPosLmSqlAppendEndParanthesis );
        }

    if ( iAttributes & CPosLandmark::EDescription )
        {
        if ( conditions )
        {
        query->Des().Append( KPosLmSqlAppendOr );
        }
        query->Des().Append( KPosLmSqlAppendStartParanthesis );
		for(int i=0; i< iTextPattern.Count() - 1; i++ )
        {
        	HBufC* textPattern = PosLmDatabaseUtility::EmbedSingleQuoteCharactersLC( *iTextPattern[i] );
            query->Des().AppendFormat( 
            KPosLmSqlAppendStringLike, &KPosLmNDescCol, textPattern );
	        if(iCompositionType == 0)
	        	query->Des().Append( KPosLmSqlAppendAnd );
	        else
	        	query->Des().Append( KPosLmSqlAppendOr );
	        
	        CleanupStack::PopAndDestroy( textPattern );
        }
        
        //last pattern
        HBufC* textPattern = PosLmDatabaseUtility::EmbedSingleQuoteCharactersLC( *iTextPattern[iTextPattern.Count() - 1] );
        query->Des().AppendFormat(KPosLmSqlAppendStringLike, &KPosLmNDescCol, textPattern );
        CleanupStack::PopAndDestroy( textPattern );
        query->Des().Append( KPosLmSqlAppendEndParanthesis );
        conditions++;
        }
    
    
    if ( conditions < 1 )
        {
        User::Leave( KErrArgument );
        }

    return query;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalTextSearch::PrepareFieldSearchL( RDbView& aView )
    {
    HBufC* query = CreatePosFieldQueryLC();
    PrepareViewL( aView, *query, EDbCompareCollated );
    CleanupStack::PopAndDestroy( query );
    }

// -----------------------------------------------------------------------------
// Sequence of this case:
// 1) Search LM table, results added to the end
// 2) Search Fields table, results added to the end
// Search stops when all records are analyzed, or once max number of matches reached
// -----------------------------------------------------------------------------
//
void CPosLmLocalTextSearch::UnsortedSearchL()
	{
	if ( iSearchStatus == EPrepareUnsortedSearch )
		{
	    if ( iAttributeSearch )
			{
	        HBufC* query = CreateAttributeQueryLC();
	        PrepareViewL( iView, *query );
	        CleanupStack::PopAndDestroy( query );
			iSearchStatus = EUnsortedAttributeSearch;
			}
	    else
	    	{
            iSearchStatus = EPrepareUnsortedFieldSearch;
	    	}
		}

    if ( iSearchStatus ==  EPrepareUnsortedFieldSearch )
        {
        if ( iPositionFieldSearch )
            {
            PrepareFieldSearchL( iView );
            iSearchStatus = EUnsortedFieldSearch;
            }
        else
            {
            iSearchStatus = ESearchCompleted;
            }
        }

    if ( iSearchStatus == EUnsortedAttributeSearch )
        {
        if ( SearchViewL( iView, EPosLmLcLandmarkIdCol ) )
            {
            iView.Close();
            iSearchStatus = EPrepareUnsortedFieldSearch; 
            }
        }
    
    if ( iSearchStatus == EUnsortedFieldSearch )
        {
        if ( SearchViewL( iView, EPosLmLcLandmarkIdCol, EFalse ) )
            {
            iView.Close();
            iSearchStatus = ESearchCompleted;
            }
        }
    
    if ( IsMaxMatchesFound() )
        {
        iSearchStatus = ESearchCompleted;
        }
	}


// -----------------------------------------------------------------------------
// Sequence of this case:
// 1) Use LM index to search for attributes. Save offset of found item in the Index
//  to an array. CurrentMatches should be built using this array. Keep all Index
//  items until search is completed. Insert to display data using same offset.
// 2) Search in Fields table and insert results as in 1.
// -----------------------------------------------------------------------------
//
void CPosLmLocalTextSearch::SortedUnlimitedSearchL()
    {
    if ( iSearchStatus == EPrepareUnlimitedSearch )
        {
        PrepareLandmarkIndexL();
        iLandmarkIndex->SetMode( CPosLmNameIndexIterator::EModeFull );

        if ( iAttributeSearch )
            {
            iSearchStatus = EUnlimitedAttributeSearch;
            }
        else
            {
            iSearchStatus = EPrepareUnlimitedFieldSearch;
            }
        }
    
    if ( iSearchStatus == EPrepareUnlimitedFieldSearch )
       {
       if ( iPositionFieldSearch )
           {
           LoadFullLandmarkIndexL();
           PrepareFieldSearchL( iView );
           iSearchStatus = EUnlimitedFieldSearch;
           }
       else
           {
           iSearchStatus = ESearchCompleted;
           }
       }

    if ( iSearchStatus == EUnlimitedAttributeSearch )
        {
        EvaluateLandmarkIndexL();
        TInt ddMatches = DisplayDataCount();
        while ( !iLandmarkIndex->AtEnd() && !BreakOnDisplayData( ddMatches ) )
            {
            const RPosLmLocalNameIndex::CIndexItem& item = iLandmarkIndex->Current();

            TBool match = EFalse;
            // check name
            if ( iAttributes & CPosLandmark::ELandmarkName && MatchText(item.Name()))
                 //item.Name().MatchC( *iTextPattern[0] ) >= 0 )
                {
                match = ETrue;
                if ( MatchesPreviousMatches( item.Id() ) )
                    {
                    if ( HasDisplayData() )
                        {
                        AddMatchL( item.Id() );
                        }
                    else
                        {
                        AddMatchIndexL( iLandmarkIndex->CurrentPosition() );
                        }
                    }
                }
            
            // check description
            if ( !match && iAttributes & CPosLandmark::EDescription )
                {
                if ( MatchesPreviousMatches( item.Id() ) && 
                    GetLandmarkRowL( item.Id() ) )
                    {
                    HBufC* desc = PosLmLongTextColHandler::ReadFromLongTextColumnLC( 
                        iTable, EPosLmLcDescriptionCol );
                    
                    if ( MatchText(*desc))//desc->MatchC( *iTextPattern ) >= 0 )
                        {
                        if ( HasDisplayData() )
                            {
                            AddMatchL( item.Id(), &iTable );
                            }
                        else
                            {
                            AddMatchIndexL( iLandmarkIndex->CurrentPosition(), &iTable );
                            }
                        match = ETrue;
                        }
                    
                    CleanupStack::PopAndDestroy( desc );
                    }
                }
            iLandmarkIndex->Next();
            }

        if ( !MoreInLandmarkIndex() )
            {
            iSearchStatus = EPrepareUnlimitedFieldSearch;
            }
        }    

    if ( iSearchStatus == EUnlimitedFieldSearch )
        {
        EvaluateViewL( iView );
        TInt ddMatches = DisplayDataCount();
        while ( iView.AtRow() && !BreakOnDisplayData( ddMatches ) )
            {
            GetRowL( iView );
            TPosLmItemId id = iView.ColUint32( 1 );
            if ( MatchesPreviousMatches( id ) && 
                 iCurrentMatches.Find( id ) == KErrNotFound ) 
                {
                if ( HasDisplayData() )
                    {
                    // landmark names are in display data,
                    // use them to sort new item
                    AddMatchL( id );
                    }
                else
                    {
                    // find this ID in sorted landmarks index
                    TInt pos = iLandmarkIndex->Find( id ); 
                    if ( pos >= 0 )
                        {
                        InsertMatchIndexL( pos );
                        }
                    }
                }
            iView.NextL();
            }

        if ( !MoreInView( iView ) )
            {
            iView.Close();
            iSearchStatus = ESearchCompleted;
            }
        }
    }

// -----------------------------------------------------------------------------
// iMatchIndexes contains indexes to iLandmarkIndex. Matches are sorted in the same
// way as is iLandmarkIndex, therefore this array is sorted itself.
// -----------------------------------------------------------------------------
//
void CPosLmLocalTextSearch::InsertMatchIndexL( TUint aIndex, RDbRowSet* aDataRow )
    {
    TInt pos = KErrNotFound;
    TInt err = iMatchIndexes.FindInOrder( aIndex, pos );
    if ( err != KErrNotFound )
        {
        return; // this item is already used
        }
    else
        {
        iMatchIndexes.InsertL( aIndex, pos );
        }
    
    TPosLmItemId id = iLandmarkIndex->Item( aIndex ).Id();
    iCurrentMatches.InsertL( id, pos );

    if ( HasDisplayData() )
        {
        CPosLmDisplayItem* item = CreateItemLC( id, aDataRow );
        PosLmDisplayDataHandler::InsertItemL( DisplayData(), item, pos );
        CleanupStack::Pop( item );
        }
    }

// -----------------------------------------------------------------------------
// iMatchIndexes is always sorted ascending (because RArray<TInt> does not support
// otherwise), but iLandmarkIndex is ordered according to sort preference.
// -----------------------------------------------------------------------------
//
void CPosLmLocalTextSearch::AddMatchIndexL( TUint aIndex, RDbRowSet* aDataRow )
    {
    if ( iMatchIndexes.FindInOrder( aIndex ) >= 0 )
        {
        return; // this item is already used
        }
    iMatchIndexes.AppendL( aIndex );
    
    TPosLmItemId id = iLandmarkIndex->Item( aIndex ).Id();
    iCurrentMatches.AppendL( id );

    if ( HasDisplayData() )
        {
        CPosLmDisplayItem* item = CreateItemLC( id, aDataRow );
        TInt pos = iCurrentMatches.Count(); // add to the end
        PosLmDisplayDataHandler::InsertItemL( DisplayData(), item, pos );
        CleanupStack::Pop( item );
        }
    }

// -----------------------------------------------------------------------------
// The trick of this case is that results cannot disappear from "current matches"
// (intermediate iterator of matches). Therefore results by fields cannot be 
// postponed: need to search for fields first and collect results until 
// search by index is started.
// -----------------------------------------------------------------------------
//
void CPosLmLocalTextSearch::SortedLimitedSearchL()
    {
    TInt err = KErrNone;
    
    if ( iSearchStatus == EPrepareSortedLimitedSearch )
        {
        if ( iPositionFieldSearch )
            {
            PrepareFieldSearchL( iView );
            iSearchStatus = ESortedLimitedFieldSearch;
            }
        else
            {
            iSearchStatus = EPrepareSortedLimitedAttributeSearch;
            }
        }

    if ( iSearchStatus == EPrepareSortedLimitedAttributeSearch )
        {
        if ( iAttributeSearch )
            {
            PrepareLandmarkIndexL();
            iSearchStatus = ESortedLimitedAttributeSearch;
            }
        else
            {
            iSearchStatus = ESearchCompleted;
            }
        }

    if ( iSearchStatus == ESortedLimitedFieldSearch )
        {
        EvaluateViewL( iView );

        while ( iView.AtRow() )
            {
            GetRowL( iView );
            err = iFieldMatches.InsertInOrder( iView.ColUint32( 1 ) ); // Lm ID
            if ( err && err != KErrAlreadyExists )
                {
                User::Leave( err );
                }
            iView.NextL();
            }
        if ( !MoreInView( iView ) )
            {
            iView.Close();
            iSearchStatus = EPrepareSortedLimitedAttributeSearch;
            }
        }
    
    if ( iSearchStatus == ESortedLimitedAttributeSearch )
        {
        EvaluateLandmarkIndexL();
        
        TInt ddMatches = DisplayDataCount();
        while ( !iLandmarkIndex->AtEnd() && !IsMaxMatchesFound() && !BreakOnDisplayData( ddMatches ) )
            {
            const RPosLmLocalNameIndex::CIndexItem& item = iLandmarkIndex->Current();

            TBool match = EFalse;
            // check name
            if ( iAttributes & CPosLandmark::ELandmarkName && MatchText(item.Name()))
                 //item.Name().MatchC( *iTextPattern ) >= 0 )
                {
                AddMatchL( item.Id() );
                match = ETrue;
                }
            
            // check description
            if ( !match && iAttributes & CPosLandmark::EDescription )
                {
                if ( GetLandmarkRowL( item.Id() ) )
                    {
                    HBufC* desc = PosLmLongTextColHandler::ReadFromLongTextColumnLC( 
                        iTable, EPosLmLcDescriptionCol );
                    
                    if ( MatchText(*desc))//desc->MatchC( *iTextPattern ) >= 0 )
                        {
                        AddMatchL( item.Id(), &iTable );
                        match = ETrue;
                        }
                    
                    CleanupStack::PopAndDestroy( desc );
                    }
                }
            
            // check fields
            if ( !match && iPositionFieldSearch &&
                 iFieldMatches.FindInOrder( item.Id() ) >= 0 )
                {
                AddMatchL( item.Id() );
                match = ETrue;
                }

            iLandmarkIndex->Next();
            }

        if ( !MoreInLandmarkIndex() || IsMaxMatchesFound())
            {
            iSearchStatus = ESearchCompleted;
            }
        }
    }

CPosLmLocalTextSearch* CPosLmLocalTextSearch::NewL(
            CPosLmLocalDatabase& aDatabase,
            const RPointerArray<CPosLmSearchCriteria>& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            CPosLmDisplayData* aDisplayData,
            TInt aMaxNumOfMatches )
{
	CPosLmLocalTextSearch* self = new (ELeave) CPosLmLocalTextSearch(
        aDatabase, *aCriteria[0], aSortPref, aMaxNumOfMatches);
    CleanupStack::PushL(self);
    self->ConstructL(aCriteria, aDisplayData);
    CleanupStack::Pop(self);
    return self;
}

void CPosLmLocalTextSearch::ConstructL(
            const RPointerArray<CPosLmSearchCriteria>& aCriteria,
            CPosLmDisplayData* aDisplayData )
{
	for(int i=0; i < aCriteria.Count(); i++)
	{
		const CPosLmTextCriteria& criteria =
	        reinterpret_cast<const CPosLmTextCriteria&>( *aCriteria[i] );

	    ValidateCriteriaL( criteria );

	    iAttributes |= criteria.AttributesToSearch();
	    criteria.GetPositionFieldsToSearchL( iFields );
	    TBool aFound = EFalse;
	    for(int j=0; j<iTextPattern.Count(); j++ )
	    {
	    	if(criteria.Text().CompareC(iTextPattern[j]->Des()) == 0)
	    	{
	    		aFound = ETrue;
	    		break;
	    	}
	    }
	    if(!aFound)
	    	iTextPattern.AppendL(criteria.Text().AllocL());
	}
	
	
    // Check if the text should be searched in attributes or in position fields
    // or in both.
    if ( iFields.Count() == 0 && iAttributes == CPosLandmark::ENoAttribute )
        {
        iPositionFieldSearch = ETrue;
        iAttributeSearch = ETrue;
        iAttributes = CPosLandmark::ELandmarkName | CPosLandmark::EDescription;
        }
    else
        {
        if ( iFields.Count() > 0 )
            {
            iPositionFieldSearch = ETrue;
            }

        if ( iAttributes != CPosLandmark::ENoAttribute )
            {
            iAttributeSearch = ETrue;
            }
        }

    if ( aDisplayData )
        {
        SetDisplayDataL( aDisplayData );
        }
    
}

void CPosLmLocalTextSearch::SetCompositionType(TInt atype)
{
	iCompositionType = atype;
}

 TBool CPosLmLocalTextSearch::MatchText(TPtrC text)
 {
 	if(iCompositionType == 1)
 	{
 		for(int i =0; i<iTextPattern.Count(); i++)
 		{
 		if(text.MatchC(*(iTextPattern[i])) != KErrNotFound)
 			return ETrue;
 		}
 	  return EFalse;
 	}
 	else
 	{
 		for(int i =0; i<iTextPattern.Count(); i++)
 		{
 		if(text.MatchC(*(iTextPattern[i])) == KErrNotFound)
 			return EFalse;
 		}
 	  return ETrue;	
 	}
 	
 	
 	
 }
 