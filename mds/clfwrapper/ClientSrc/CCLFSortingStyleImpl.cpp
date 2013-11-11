/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/



// INCLUDE FILES
#include    "CCLFSortingStyleimpl.h"
#include    <barsread.h>

// CONSTANTS
const TInt KCLFFieldIdArrayGranularity( 4 );
const TInt KCLFResourceVersion( 1 );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCLFSortingStyleImpl::CCLFSortingStyleImpl
// -----------------------------------------------------------------------------
//
CCLFSortingStyleImpl::CCLFSortingStyleImpl()
    : iFieldIDArray( KCLFFieldIdArrayGranularity ),
      iDataType( ECLFItemDataTypeNull ),
      iOrdering( ECLFOrderingAscending ),
      iUndefinedItemPosition( ECLFSortingStyleUndefinedEnd )
    {
    }

// -----------------------------------------------------------------------------
// CCLFSortingStyleImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CCLFSortingStyleImpl::ConstructL( TResourceReader& aResource )
    {
    const TInt version( aResource.ReadInt16() );
    if ( version != KCLFResourceVersion )
        {
        User::Leave( KErrNotSupported );
        }
    iOrdering = TCLFSortingStyleOrdering( aResource.ReadInt32() );
    iDataType = TCLFItemDataType( aResource.ReadInt32() );
    iUndefinedItemPosition = TCLFUndefinedItemPosition( aResource.ReadInt8() );
    const TInt count( aResource.ReadInt16() );
    for ( TInt i = 0 ; i < count ; ++i )
        {
        iFieldIDArray.AppendL( aResource.ReadInt32() );
        }
    }

// -----------------------------------------------------------------------------
// CCLFSortingStyleImpl::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCLFSortingStyleImpl* CCLFSortingStyleImpl::NewLC()
    {
    CCLFSortingStyleImpl* self = new( ELeave ) CCLFSortingStyleImpl;

    CleanupStack::PushL( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CCLFSortingStyleImpl::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCLFSortingStyleImpl* CCLFSortingStyleImpl::NewLC( TResourceReader& aResource )
    {
    CCLFSortingStyleImpl* self = new( ELeave ) CCLFSortingStyleImpl;

    CleanupStack::PushL( self );
    self->ConstructL( aResource );

    return self;
    }


// -----------------------------------------------------------------------------
// CCLFSortingStyleImpl::~CCLFSortingStyleImpl
// Destructor
// -----------------------------------------------------------------------------
//
CCLFSortingStyleImpl::~CCLFSortingStyleImpl()
    {
    iFieldIDArray.Close();
    }


// -----------------------------------------------------------------------------
// CCLFSortingStyleImpl::ResetL
// -----------------------------------------------------------------------------
//
void CCLFSortingStyleImpl::ResetL()
    {
    iFieldIDArray.Reset();
    iOrdering = ECLFOrderingAscending;
    iUndefinedItemPosition = ECLFSortingStyleUndefinedEnd;
    }

// -----------------------------------------------------------------------------
// CCLFSortingStyleImpl::AddFieldL
// -----------------------------------------------------------------------------
//
void CCLFSortingStyleImpl::AddFieldL( TCLFFieldId aFieldId )
    {
    iFieldIDArray.AppendL( aFieldId );
    }
    
// -----------------------------------------------------------------------------
// CCLFSortingStyleImpl::GetFieldsL
// -----------------------------------------------------------------------------
//
void CCLFSortingStyleImpl::GetFieldsL( RArray<TCLFFieldId>& aArray ) const
    {
    for( TInt i = iFieldIDArray.Count() - 1; i >=0; i--)
        {
        aArray.AppendL( iFieldIDArray[i] );
        }
    }

// -----------------------------------------------------------------------------
// CCLFSortingStyleImpl::SetOrdering
// -----------------------------------------------------------------------------
//
void CCLFSortingStyleImpl::SetOrdering( TCLFSortingStyleOrdering aOrdering )
    {
    iOrdering = aOrdering;
    }
    
// -----------------------------------------------------------------------------
// CCLFSortingStyleImpl::Ordering
// -----------------------------------------------------------------------------
//
TCLFSortingStyleOrdering CCLFSortingStyleImpl::Ordering() const
    {
    return iOrdering;
    }

// -----------------------------------------------------------------------------
// CCLFSortingStyleImpl::SetSortingDataType
// -----------------------------------------------------------------------------
//
void CCLFSortingStyleImpl::SetSortingDataType( TCLFItemDataType aDataType )
    {
    iDataType = aDataType;
    }

// -----------------------------------------------------------------------------
// CCLFSortingStyleImpl::SortingDataType
// -----------------------------------------------------------------------------
//
TCLFItemDataType CCLFSortingStyleImpl::SortingDataType() const
    {
    return iDataType;
    }

// -----------------------------------------------------------------------------
// CCLFSortingStyleImpl::SetUndefinedItemPosition
// -----------------------------------------------------------------------------
//
void CCLFSortingStyleImpl::SetUndefinedItemPosition(
                            TCLFUndefinedItemPosition aUndefinedItemPosition )
    {
    iUndefinedItemPosition = aUndefinedItemPosition;
    }
// -----------------------------------------------------------------------------
// CCLFSortingStyleImpl::UndefinedItemPosition
// -----------------------------------------------------------------------------
//
TCLFUndefinedItemPosition CCLFSortingStyleImpl::UndefinedItemPosition() const
    {
    return iUndefinedItemPosition;
    }
    
//  End of File
