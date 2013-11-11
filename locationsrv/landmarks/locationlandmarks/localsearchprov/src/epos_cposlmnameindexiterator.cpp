/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPosLmNameIndexIterator class
*
*
*/


#include <e32base.h>
#include "epos_rposlmlocalnameindex.h"
#include "epos_cposlmnameindexiterator.h"

#ifdef _DEBUG
#include <e32debug.h>
#endif

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmNameIndexIterator::CPosLmNameIndexIterator( 
    RPosLmLocalNameIndex& aIndex,
    TBool aAscending )
:   iIndex( aIndex ), iMode( EModePartial ), iAscending( aAscending )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmNameIndexIterator* CPosLmNameIndexIterator::NewL( 
    RPosLmLocalNameIndex& aIndex,
    TBool aAscending )
    {
    CPosLmNameIndexIterator* self = new (ELeave) CPosLmNameIndexIterator( aIndex, aAscending );
    self->First();
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmNameIndexIterator::~CPosLmNameIndexIterator()
    {
    iSortedLandmarks.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmNameIndexIterator::HasMore() const
    {
    return iLeftInIndex > 0;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmNameIndexIterator::LoadMoreL( TInt aCount )
    {
    if ( iMode == EModePartial )
        {
        DiscardProcessedItems();
        }
    
    TInt oldCount = iSortedLandmarks.Count();
    if ( iAscending )
        {
        iIndex.ReadSortedLandmarksL( iNextToLoad, aCount, iSortedLandmarks, iLeftInIndex );
        iNextToLoad += iSortedLandmarks.Count() - oldCount; // this is actual number of loaded items
        }
    else
        {
        iIndex.ReadSortedLandmarksL( iNextToLoad, -aCount, iSortedLandmarks, iLeftInIndex );
        iNextToLoad -= iSortedLandmarks.Count() - oldCount;
        }
    iLeftInIndex = Max( 0, iLeftInIndex );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmNameIndexIterator::DiscardProcessedItems()
    {
    if ( iCurrent >= 0 )
        {
        TInt last = Min( iSortedLandmarks.Count(), iCurrent );
        for ( int i = last - 1; i >= 0; --i )
            {
            delete iSortedLandmarks[i];
            iSortedLandmarks.Remove( i );
            }
        iCurrent = 0;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmNameIndexIterator::Count() const
    {
    return iAscending ? iNextToLoad : TotalCount() - (iNextToLoad+1);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmNameIndexIterator::TotalCount() const
    {
    return iIndex.Count();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmNameIndexIterator::LoadedCount() const
    {
    return iSortedLandmarks.Count();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmNameIndexIterator::First()
    {
    iLeftInIndex = iIndex.Count();
    iNextToLoad = iAscending ? 0 : iLeftInIndex-1;
    iSortedLandmarks.ResetAndDestroy();
    iCurrent = 0;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmNameIndexIterator::Next()
    {
    if ( iCurrent < iSortedLandmarks.Count() )
        {
        iCurrent++;
        return iCurrent < iSortedLandmarks.Count();
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmNameIndexIterator::AtEnd() const
    {
    return iCurrent == iSortedLandmarks.Count(); 
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmNameIndexIterator::CurrentPosition() const
    {
    return Count() - iSortedLandmarks.Count() + iCurrent;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
const RPosLmLocalNameIndex::CIndexItem& CPosLmNameIndexIterator::Current() const
    {
    ASSERT( iCurrent >= 0 && iCurrent < iSortedLandmarks.Count() );
    return *(iSortedLandmarks[ iCurrent ]);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmNameIndexIterator::SetMode( TIndexMode aMode )
    {
    iMode = aMode; 
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmNameIndexIterator::TIndexMode CPosLmNameIndexIterator::Mode() const
    {
    return iMode;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
const RPosLmLocalNameIndex::CIndexItem& CPosLmNameIndexIterator::Item( TInt aIndex ) const
    {
    ASSERT( aIndex >= 0 && aIndex < iSortedLandmarks.Count() );
    return *(iSortedLandmarks[ aIndex ]);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmNameIndexIterator::Find( TPosLmItemId aLmid ) const
    {
    for ( int i = 0; i < iSortedLandmarks.Count(); ++i )
        {
        if ( iSortedLandmarks[i]->Id() == aLmid )
            {
            return i;
            }
        }
    return KErrNotFound;
    }
