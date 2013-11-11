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
#include    "CCLFDbItemContainer.h"
#include    <mdeobject.h> 
#include    <f32file.h>
#include    "CLFPanics.h"

const TInt KCLFContainerArrayGranularity( 25 );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCLFContainerItem::CCLFContainerItem
// -----------------------------------------------------------------------------
//
CCLFContainerItem::CCLFContainerItem( const CMdEObject* aObject )
    : iReferenceCount( 0 ), iMdEObject( aObject ), iDeprecated( EFalse ),
      iItemId( aObject->Id() )
    {
    }

// -----------------------------------------------------------------------------
// CCLFContainerItem::CCLFContainerItem
// -----------------------------------------------------------------------------
//
CCLFContainerItem::CCLFContainerItem()
    : iReferenceCount( 0 ), iMdEObject( NULL ), iDeprecated( EFalse ),
      iItemId( 0 )
	{
	}

// -----------------------------------------------------------------------------
// CCLFContainerItem::NewLC
// -----------------------------------------------------------------------------
//
CCLFContainerItem* CCLFContainerItem::NewLC( const CMdEObject* aObject )
    {
    CCLFContainerItem* self = new ( ELeave ) CCLFContainerItem( aObject );
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCLFContainerItem::~CCLFContainerItem
// Destructor
// -----------------------------------------------------------------------------
//
CCLFContainerItem::~CCLFContainerItem()
    {
    delete iMdEObject;
    iMdEObject = NULL;
    }

// -----------------------------------------------------------------------------
// CCLFContainerItem::Compare
// -----------------------------------------------------------------------------
//
TInt CCLFContainerItem::Compare( const CCLFContainerItem& aFirst,
					             const CCLFContainerItem& aSecond )
	{
	return aFirst.iItemId - aSecond.iItemId;
	}

// -----------------------------------------------------------------------------
// CCLFDbItemContainer::TFindHelper::TFindHelper
// -----------------------------------------------------------------------------
//
CCLFDbItemContainer::TFindHelper::TFindHelper()
    : iItemId( 0 ), 
      iIndex( 0 )
    {
    }

// -----------------------------------------------------------------------------
// CCLFDbItemContainer::TFindHelper::Reset
// -----------------------------------------------------------------------------
//
void CCLFDbItemContainer::TFindHelper::Reset()
    {
    iItemId = 0;
    iIndex = 0;
    }

// -----------------------------------------------------------------------------
// CCLFDbItemContainer::TFindHelper::Set
// -----------------------------------------------------------------------------
//
void CCLFDbItemContainer::TFindHelper::Set( TCLFItemId aItemId, 
                                                   TInt aIndex )
    {
    iItemId = aItemId;
    iIndex = aIndex;
    }

// -----------------------------------------------------------------------------
// CCLFDbItemContainer::CCLFDbItemContainer
// -----------------------------------------------------------------------------
//
CCLFDbItemContainer::CCLFDbItemContainer()
    : iItemArray( KCLFContainerArrayGranularity ),
      iArraySorter( CCLFContainerItem::Compare )
    {
    }

// -----------------------------------------------------------------------------
// CCLFDbItemContainer::NewL
// -----------------------------------------------------------------------------
//
CCLFDbItemContainer* CCLFDbItemContainer::NewL()
    {
    return new( ELeave ) CCLFDbItemContainer;
    }

// -----------------------------------------------------------------------------
// CCLFDbItemContainer::~CCLFDbItemContainer
// Destructor
// -----------------------------------------------------------------------------
//
CCLFDbItemContainer::~CCLFDbItemContainer()
    {
    iItemArray.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CCLFDbItemContainer::ItemById
// -----------------------------------------------------------------------------
//
CCLFContainerItem* CCLFDbItemContainer::ItemById( const TCLFItemId aItemId ) const
    {
    CCLFContainerItem* item = NULL;
    TInt index( FindItem( aItemId, item ) );
    while ( index >= 0 )
        {
		if( !( item->iDeprecated ) )
			{
			return item;
			}
        index = NextItem( item );
        }
    return NULL;
    }

// -----------------------------------------------------------------------------
// CCLFDbItemContainer::ReleaseItem
// -----------------------------------------------------------------------------
//
void CCLFDbItemContainer::ReleaseItem( CCLFContainerItem* aItem )
    {
    const TInt index( iItemArray.Find( aItem ) );
    if ( index >= 0 )
        {
        aItem->iReferenceCount--;
        __ASSERT_DEBUG ( aItem->iReferenceCount >= 0,
                         User::Panic( KCLFPanicText, ECLFNullPointer ) );
        if ( aItem->iReferenceCount <= 0 )
            {
            iItemArray.Remove( index );
            delete aItem;
            aItem = NULL;
            }
        }
    }

// -----------------------------------------------------------------------------
// CCLFDbItemContainer::SetDeprecatedItems
// -----------------------------------------------------------------------------
//
void CCLFDbItemContainer::SetDeprecatedItems(
                                    const RArray<TCLFItemId>& aItemIdArray )
    {
    const TInt count( aItemIdArray.Count() );
    for ( TInt i = 0 ; i < count ; ++i )
        {
        CCLFContainerItem* item = NULL;
        TInt index( FindItem( aItemIdArray[i], item ) );
        while ( index >= 0 )
            {
            item->iDeprecated = ETrue;
            index = NextItem( item );
            }
        }
    }

// -----------------------------------------------------------------------------
// CCLFDbItemContainer::FindItem
// -----------------------------------------------------------------------------
//
TInt CCLFDbItemContainer::FindItem( TCLFItemId aItemId, 
                                    CCLFContainerItem*& aItem ) const
    {
    iFindHelper.Reset();
	iFindItem.SetItemId( aItemId );
    const TInt index ( iItemArray.SpecificFindInOrder( 
                                        &iFindItem, 
                                        iArraySorter, 
                                        EArrayFindMode_First ) );
	if ( index >= 0 )
		{
    	iFindHelper.Set( aItemId, index );
		aItem = iItemArray[index];
		return index;
		}
    return KErrNotFound;
    
    }

// -----------------------------------------------------------------------------
// CCLFDbItemContainer::NextItem
// -----------------------------------------------------------------------------
//
TInt CCLFDbItemContainer::NextItem( CCLFContainerItem*& aItem ) const
    {
    const TInt count( iItemArray.Count() );
    if ( iFindHelper.iIndex < count )
        {
        CCLFContainerItem* item = iItemArray[ iFindHelper.iIndex ];
        if ( iFindHelper.iItemId == item->iItemId )
            {
            iFindHelper.iIndex++;
            aItem = item;
            return iFindHelper.iIndex - 1;
            }
        }
    return KErrNotFound;
    }

//  End of File
