/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Operation for reading partial landmarks.
*
*
*/


#include    <eposlmasyncops.rsg>
#include    <EPos_LandmarksErrors.h>
#include    <epos_cposlmresourcereader.h>
#include    <EPos_PosLmImplExtension.h>
#include    <epos_poslmlandmarkhandler.h>
#include    <epos_cposlmlocaldbaccess.h>

#include    "EPos_LocalLandmarks.h"
#include    "epos_cposlmlocaldatabase.h"
#include    "EPos_RPosLmLocalAccessSubsession.h"
#include    "EPos_CPosLmLocalGetSortedIdsOp.h"

const TInt KGranularity = 128;
const TReal32 KOperationDone = 1;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalGetSortedIdsOp::CPosLmLocalGetSortedIdsOp(
    CPosLmLocalDatabase& aDb, TPosLmSortPref aSortPref ) :
    CPosLmLocalOperation(aDb),
    iSortPref( aSortPref ),
    iStatusFlag( KPosLmOperationNotComplete )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalGetSortedIdsOp::ConstructL()
    {
    BaseConstructL();
    
    iIdArray = new (ELeave) CArrayFixSeg<TPosLmItemId>( KGranularity );

    ReadInfoFromResourceFileL();
    AquireLockL( CPosLmLocalDatabase::EReadLock );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalGetSortedIdsOp* CPosLmLocalGetSortedIdsOp::NewL(
    CPosLmLocalDatabase& aDb,
    TPosLmSortPref aSortPref )
    {
    CPosLmLocalGetSortedIdsOp* self = new (ELeave)
        CPosLmLocalGetSortedIdsOp( aDb, aSortPref );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalGetSortedIdsOp::~CPosLmLocalGetSortedIdsOp()
    {
    if ( iIdArray )
        {
        delete iIdArray;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalGetSortedIdsOp::NextStepL( TReal32& aProgress )
    {
    TPosLmItemId* ids = new (ELeave) TPosLmItemId[ iIdBatchSize ];
    CleanupStack::PushL( ids );

    TInt remainder( 0 );
    TInt result = iDb->NameIndex().ReadSortedIds( ids, iCurrent, iIdBatchSize, remainder );

    if ( result < 0 )
        {
        User::Leave( result );
        }
    else // result >= 0
        {
        for ( int i = 0; i < result ; i++)
            {
            iIdArray->AppendL( ids[i] );
            }
        iCurrent += result;
        }

    if ( remainder <= 0 ) // all retrieved
        {
        iStatusFlag = KErrNone;
        aProgress = KOperationDone;
        ReleaseLock();
        }
    else
        {
        aProgress = TReal32( iCurrent ) / TReal32( iCurrent + remainder );
        }

    CleanupStack::PopAndDestroy( ids );
    return iStatusFlag;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalGetSortedIdsOp::HandleError( TInt& aError )
    {
    iStatusFlag = aError;
    ReleaseLock();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalGetSortedIdsOp::GetResultL( RArray<TPosLmItemId>& aIdArray )
    {
    aIdArray.Reset();
    aIdArray.ReserveL( iIdArray->Count() );
    if ( iSortPref.SortOrder() == TPosLmSortPref::EAscending )
        {
        for ( TInt i = 0; i < iIdArray->Count(); i++ )
            {
            aIdArray.AppendL( (*iIdArray)[i] );
            }
        }
    else
        {
        for ( TInt i = iIdArray->Count() - 1; i >= 0; i-- )
            {
            aIdArray.AppendL( (*iIdArray)[i] );
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalGetSortedIdsOp::ReadInfoFromResourceFileL()
    {
    CPosLmResourceReader* resources =
        CPosLmResourceReader::NewLC(KPosLocalSuboperationResourceFile);

    iIdBatchSize = resources->ReadInt32L( R_POS_LM_LOCAL_GET_SORTED_IDS_OP );

    if ( iIdBatchSize <= 0 )
        {
        Panic(KPosLandmarksClientPanic,
            EPosInvalidValueSpecifiedInResourceFile );
        }

    CleanupStack::PopAndDestroy( resources );
    }

