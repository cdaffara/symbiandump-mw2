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
* Description: Landmarks Server's utilities
*
*
*/


#include "epos_landmarkdatabasestructure.h"
#include "epos_cposlmlocaldbaccess.h"
#include "epos_poslmserverutility.h"

const TInt KIdIndexStartSize = 1100;
const TInt KIdIndexGrowth = 9;

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt PosLmServerUtility::EstimatedDiskSizeOfIndex(
    TLmOperation aOperation,
    TInt aCountOfRecords )
    {
    ASSERT( aOperation == ECreateFieldsLmIdIndex || aOperation == ECreateCategoriesLmIdIndex );

    return KIdIndexStartSize + aCountOfRecords * KIdIndexGrowth;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool PosLmServerUtility::IndexExistsL(
    const RDbNamedDatabase& aDatabase,
    const TDesC& aTable,
    const TDesC& aIndex )
    {
    TBool found( EFalse );
    CDbIndexNames* indexes = aDatabase.IndexNamesL( aTable );
    for ( TInt i = 0; i < indexes->Count(); ++i ) 
        {
        if ( (*indexes)[i].Compare( aIndex ) == 0 )
            {
            found = ETrue;
            break;
            }
        }
    delete indexes;
    return found;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TUint PosLmServerUtility::GetTableRowCountL( 
    CPosLmLocalDbAccess& aDbAccess,
    const TDesC& aTableName )
    {
    RDbNamedDatabase db;
    aDbAccess.GetDatabase( db );
    
    RDbTable table;
    User::LeaveIfError( table.Open( db, aTableName, RDbRowSet::EReadOnly ) );
    CleanupClosePushL( table );
    TInt numRows = table.CountL();
    CleanupStack::PopAndDestroy( &table );

    if ( numRows < 0 )
        {
        User::Leave( KErrNotFound );
        }
    
    return numRows;
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TUint PosLmServerUtility::TotalLandmarkCountL( CPosLmLocalDbAccess& aDbAccess )
    {
    return PosLmServerUtility::GetTableRowCountL( aDbAccess, KPosLmLandmarkTable );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
HBufC* PosLmServerUtility::ReadFromLongTextColumnLC(
    RDbRowSet& aView,
    TDbColNo aCol )
    {
    TInt len = aView.ColLength( aCol );
    if ( len > 0 )
        {
        HBufC* text = HBufC::NewLC( len );
        TPtr ptr = text->Des();

        RDbColReadStream readStream;
        readStream.OpenL( aView, aCol );
        CleanupClosePushL( readStream );
        readStream.ReadL( ptr, len );
        CleanupStack::PopAndDestroy( &readStream );

        return text;
        }
    else
        {
        return KNullDesC().AllocLC();
        }
    }

