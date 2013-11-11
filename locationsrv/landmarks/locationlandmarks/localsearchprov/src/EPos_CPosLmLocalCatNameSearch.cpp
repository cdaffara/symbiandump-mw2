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
* Description: Class for handling searching for landmark categories with a certain name.
*
*
*/


#include    <EPos_CPosLmCatNameCriteria.h>
#include    <epos_landmarkdatabasestructure.h>
#include    <epos_poslmdatabaseutility.h>
#include    "EPos_CPosLmLocalCatNameSearch.h"

_LIT(KPosLmSqlSelectStringLike, "SELECT %S FROM %S WHERE %S LIKE '%S'");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalCatNameSearch::CPosLmLocalCatNameSearch(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmLocalSortPref& aSortPref,
    TInt aMaxNumOfMatches)
:   CPosLmLocalSearchHandler(
        aDatabase, aCriteria.CriteriaType(), aSortPref, aMaxNumOfMatches),
    iSearchStatus(EPosSearchNotStarted)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCatNameSearch::ConstructL(
    const CPosLmSearchCriteria& aCriteria,
    CPosLmDisplayData* aDisplayData)
    {
    const CPosLmCatNameCriteria& criteria =
        static_cast<const CPosLmCatNameCriteria&>(aCriteria);

    ValidateCriteriaL(criteria);

    iCriteria = CPosLmCatNameCriteria::NewLC();
    CleanupStack::Pop(iCriteria);

    HBufC* embeddedText = PosLmDatabaseUtility::EmbedSingleQuoteCharactersLC(
        criteria.SearchPattern());
    iCriteria->SetSearchPatternL(*embeddedText);
    CleanupStack::PopAndDestroy(embeddedText);

    if (aDisplayData)
        {
        SetDisplayDataL(aDisplayData);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalCatNameSearch* CPosLmLocalCatNameSearch::NewL(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmLocalSortPref& aSortPref,
    CPosLmDisplayData* aDisplayData,
    TInt aMaxNumOfMatches)
    {
    CPosLmLocalCatNameSearch* self = new (ELeave) CPosLmLocalCatNameSearch(
        aDatabase, aCriteria, aSortPref, aMaxNumOfMatches);
    CleanupStack::PushL(self);
    self->ConstructL(aCriteria, aDisplayData);
    CleanupStack::Pop(self);

    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalCatNameSearch::~CPosLmLocalCatNameSearch()
    {
    delete iCriteria;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCatNameSearch::ValidateCriteriaL(
    const CPosLmCatNameCriteria& aCriteria)
    {
    if (aCriteria.SearchPattern().Length() == 0)
        {
        User::Leave(KErrArgument);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalCatNameSearch::IsValidForCategorySearch() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalCatNameSearch::NextStepL(
    TReal32& aProgress)
    {
    if (iSearchStatus == EPosSearchNotStarted)
        {
        PrepareSearchL();
        }

    if (iSearchStatus == EPosCatNameSearch)
        {
        SearchL();
        }

    return SearchStepCompletedL(
        iSearchStatus == EPosSearchCompleted, aProgress);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCatNameSearch::PrepareSearchL()
    {
    TInt numRowsInTable = GetRowCountL( KPosLmCategoryTable );

    if ( numRowsInTable == 0 )
        {
        iSearchStatus = EPosSearchCompleted;
        }
    else
        {
        iSearchStatus = EPosCatNameSearch;
        PrepareCalculateProgress( numRowsInTable );
        PrepareQueryL();
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCatNameSearch::PrepareQueryL()
    {
    HBufC* query = HBufC::NewLC( KPosLmSqlStatementMaxLen );

    TPtrC searchPattern = iCriteria->SearchPattern();
    if ( HasDisplayData() )
    	{
    	// request all fields
	    query->Des().Format(
	    	KPosLmSqlSelectStringLike,
	        &KPosLmSqlAll,
	        &KPosLmCategoryTable,
	        &KPosLmCategoryNameCol,
	        &searchPattern );
    	}
    else
    	{
    	// request only ID column
	    query->Des().Format(
	    	KPosLmSqlSelectStringLike,
	        &KPosLmCategoryIdCol,
	        &KPosLmCategoryTable,
	        &KPosLmCategoryNameCol,
	        &searchPattern );
    	}

    if ( !HasDisplayData() && SortPref().SortType() ==
        TPosLmLocalSortPref::ECategorySorting )
        {
        AppendSortToQueryL( query );
        }

    PrepareViewL( iView, *query );
    CleanupStack::PopAndDestroy( query );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCatNameSearch::SearchL()
    {
    if ( SearchViewL( iView, 1 ) || IsMaxMatchesFound() )
        {
        iView.Close();
        iSearchStatus = EPosSearchCompleted;
        }
    }

