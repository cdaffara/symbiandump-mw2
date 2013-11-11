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
#include    "CCLFDefaultOperation.h"
#include    <MCLFSortingStyle.h>
#include    <MCLFModifiableItem.h>
#include    <ContentListingFactory.h>
#include    <collate.h>
#include    <badesca.h>
#include    "MGDebugPrint.h"

// for CleanupResetAndDestroyPushL
#include <mmf/common/mmfcontrollerpluginresolver.h>

// CONSTANTS
const TInt KCLFGroupedItemArrayGranularity( 4 );
const TInt KCLFSortingStyleArrayGranularity( 3 );

// CLASS DECLARATION

/**
*  Internal helper class for sorting
*/
class TCLFSortingItem
    {
    public:
        TCLFSortingItem()
            : iData( KNullDesC ),
              iIntData( 0 ),
              iTimeData( 0 ),
              iItem( NULL )
            {
            }

        TPtrC iData;
        TInt32 iIntData;
        TTime iTimeData;
        MCLFItem* iItem;
    };

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CompareAlphaAscending
// -----------------------------------------------------------------------------
//
TInt CompareAlphaAscending( const TCLFSortingItem& aFirst,
                            const TCLFSortingItem& aSecond )
    {
    // get the standard method
    TCollationMethod m = *Mem::CollationMethodByIndex( 0 );
    m.iFlags |= TCollationMethod::EIgnoreNone | TCollationMethod::EFoldCase;
    return aFirst.iData.CompareC( aSecond.iData, 3, &m  );
    }

// -----------------------------------------------------------------------------
// CompareIntAscending
// -----------------------------------------------------------------------------
//
TInt CompareIntAscending( const TCLFSortingItem& aFirst,
                          const TCLFSortingItem& aSecond )
    {
    return aFirst.iIntData - aSecond.iIntData;
    }

// -----------------------------------------------------------------------------
// CompareTimeAscending
// -----------------------------------------------------------------------------
//
TInt CompareTimeAscending( const TCLFSortingItem& aFirst,
                           const TCLFSortingItem& aSecond )
    {
    if ( aFirst.iTimeData > aSecond.iTimeData )
        {
        return 1;
        }
    else if ( aFirst.iTimeData < aSecond.iTimeData )
        {
        return -1;
        }
    return 0;
    }

// -----------------------------------------------------------------------------
// AppendItemsArrayL
// -----------------------------------------------------------------------------
//
void AppendItemsToArrayL( const TArray<MCLFItem*>& aSourceArray,
                          RPointerArray<MCLFItem>& aDestArray )
    {
    for( TInt i = aSourceArray.Count() - 1; i >=0; i--)
        {
        aDestArray.AppendL( aSourceArray[ i ] );
        }
    }

// -----------------------------------------------------------------------------
// AppendItemsToArrayL
// append aSourceArray items to aDestArray
// -----------------------------------------------------------------------------
//
void AppendItemsToArrayL( const TArray<MCLFItem*>& aSourceArray,
                          RPointerArray<MCLFItem>& aDestArray,
                          TCLFUndefinedItemPosition aPosition,
                          TInt& aItemIndex,
                          TInt& aLastAddedItemCount )
    {
    if( aPosition == ECLFSortingStyleUndefinedFirst )
        {
        aItemIndex = aItemIndex - aLastAddedItemCount;
        }

    aLastAddedItemCount = aSourceArray.Count();
    for( TInt i = 0 ; i < aLastAddedItemCount ; ++i )
        {
        aDestArray.InsertL( aSourceArray[i], aItemIndex );
        ++aItemIndex;
        }
    }

// -----------------------------------------------------------------------------
// AppendSortingItemsArrayL
// -----------------------------------------------------------------------------
//
void AppendSortingItemsToArrayL( const TArray<TCLFSortingItem>& aSourceArray,
                                 RPointerArray<MCLFItem>& aDestArray )
    {
    const TInt count( aSourceArray.Count() );
    for( TInt i = 0 ; i < count ; ++i )
        {
        aDestArray.AppendL( aSourceArray[i].iItem );
        }
    }

// -----------------------------------------------------------------------------
// AppendSortingItemsArrayReverseL
// -----------------------------------------------------------------------------
//
void AppendSortingItemsToArrayReverseL(
                        const TArray<TCLFSortingItem>& aSourceArray,
                        RPointerArray<MCLFItem>& aDestArray )
    {
    for( TInt i = aSourceArray.Count() - 1 ; i > -1 ; --i )
        {
        aDestArray.AppendL( aSourceArray[i].iItem );
        }
    }

// -----------------------------------------------------------------------------
// MakeSortingItemsL
// Add/Make items to sorting item array and undefined items to
// undefined item array
// -----------------------------------------------------------------------------
//
void MakeSortingItemsL( RArray<TCLFSortingItem>& aSortingItemArray,
                        RPointerArray<MCLFItem>& aUndefinedItemArray,
                        const TArray<MCLFItem*>& aItemArray,
                        const TArray<TCLFFieldId> aSortFields,
                        TCLFItemDataType aSortingDataType )
    {
    const TInt sortingFieldsCount( aSortFields.Count() );
    const TInt count( aItemArray.Count() );
    for( TInt i = 0 ; i < count ; ++i )
        {
        MCLFItem* seItem = aItemArray[i];
        TCLFSortingItem sortingItem;
        TInt fieldIdIndex( -1 );
        TInt error( KErrNone );
        do
            {
            ++fieldIdIndex;
            TCLFFieldId fieldId( aSortFields[fieldIdIndex] );
            switch( aSortingDataType )
                {
                case ECLFItemDataTypeDesC:
                    {
                    error = seItem->GetField( fieldId, sortingItem.iData );
                    break;
                    }
                case ECLFItemDataTypeTInt32:
                    {
                    error = seItem->GetField( fieldId, sortingItem.iIntData );
                    break;
                    }
                case ECLFItemDataTypeTTime:
                default:
                    {
                    error = seItem->GetField( fieldId, sortingItem.iTimeData );
                    break;
                    }
                }
            } while( ( error != KErrNone ) &&
                     ( fieldIdIndex < ( sortingFieldsCount - 1 ) ) );
        if( error == KErrNone )
            {
            sortingItem.iItem = seItem;
            aSortingItemArray.AppendL( sortingItem );
            }
        else
            { // field not found -> undefined item
            aUndefinedItemArray.AppendL( seItem );
            }
        }
    }

// -----------------------------------------------------------------------------
// SortSortingItemsL
// Select correct sortign operation
// -----------------------------------------------------------------------------
//
void SortSortingItemsL( RArray<TCLFSortingItem>& aItemArray,
                        TCLFItemDataType aSortingDataType )
    {
    TLinearOrder<TCLFSortingItem>* sorter = NULL;

    switch ( aSortingDataType )
        {
        case ECLFItemDataTypeDesC:
            {
            sorter = new ( ELeave ) TLinearOrder<TCLFSortingItem>
                        ( CompareAlphaAscending );
            break;
            }
        case ECLFItemDataTypeTInt32:
            {
            sorter = new ( ELeave ) TLinearOrder<TCLFSortingItem>
                        ( CompareIntAscending );
            break;
            }
        case ECLFItemDataTypeTTime:
        default:
            {
            sorter = new ( ELeave ) TLinearOrder<TCLFSortingItem>
                        ( CompareTimeAscending );
            break;
            }
        }

    aItemArray.Sort( *sorter );
    delete sorter;
    sorter = NULL;
    }

// -----------------------------------------------------------------------------
// SortItemsBySortingStyleL
// sort aItemArray with aSortingStyle
// add undefined items to aUndefinedItemArray
// -----------------------------------------------------------------------------
//
void SortItemsBySortingStyleL( RPointerArray<MCLFItem>& aItemArray,
                               MCLFSortingStyle& aSortingStyle,
                               RPointerArray<MCLFItem>& aUndefinedItemArray )
    {
    RArray<TCLFFieldId> sortingFields;
    CleanupClosePushL( sortingFields );
    aSortingStyle.GetFieldsL( sortingFields );
    const TInt sortingFieldsCount( sortingFields.Count() );
    if( sortingFieldsCount )  // sort only if there are sorting fields
        {
        TCLFItemDataType sortingDataType( aSortingStyle.SortingDataType() );

        RArray<TCLFSortingItem> sortingArray;
        CleanupClosePushL( sortingArray );
        MakeSortingItemsL( sortingArray,
                           aUndefinedItemArray,
                           aItemArray.Array(),
                           sortingFields.Array(),
                           sortingDataType );

        SortSortingItemsL( sortingArray, sortingDataType );

        aItemArray.Reset();
        if( aSortingStyle.Ordering() == ECLFOrderingAscending )
            {
            AppendSortingItemsToArrayL( sortingArray.Array(),
                                        aItemArray );
            }
        else
            {
            AppendSortingItemsToArrayReverseL( sortingArray.Array(),
                                               aItemArray );
            }

        CleanupStack::PopAndDestroy( &sortingArray ); // sortingArray.Close
        }
    CleanupStack::PopAndDestroy( &sortingFields ); // sortingFields.Close
    }


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCLFDefaultOperation::CCLFDefaultOperation
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCLFDefaultOperation::CCLFDefaultOperation()
    : iSortingStyle( NULL ),
      iSortingStyleArray( KCLFSortingStyleArrayGranularity ),
      iGrouping( ECLFNoGrouping ),
      iGroupedItemList( KCLFGroupedItemArrayGranularity )
    {
    }

// -----------------------------------------------------------------------------
// CCLFDefaultOperation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCLFDefaultOperation* CCLFDefaultOperation::NewL()
    {
    return new( ELeave ) CCLFDefaultOperation;
    }

// -----------------------------------------------------------------------------
// CCLFDefaultOperation::NewL
// Destructor
// -----------------------------------------------------------------------------
//
CCLFDefaultOperation::~CCLFDefaultOperation()
    {
    iGroupedItemList.ResetAndDestroy();
    iSortingStyleArray.Close();
    }


// -----------------------------------------------------------------------------
// CCLFDefaultOperation::SetSortingStyle
// -----------------------------------------------------------------------------
//
void CCLFDefaultOperation::SetSortingStyle( MCLFSortingStyle* aSortingStyle )
    {
    iSortingStyle = aSortingStyle;
    iSortingStyleArray.Reset();
    }

// -----------------------------------------------------------------------------
// CCLFDefaultOperation::AppendSortingStyleL
// -----------------------------------------------------------------------------
//
void CCLFDefaultOperation::AppendSortingStyleL(
                                            MCLFSortingStyle& aSortingStyle )
    {
    iSortingStyleArray.AppendL( &aSortingStyle );
    }

// -----------------------------------------------------------------------------
// CCLFDefaultOperation::SetGrouping
// -----------------------------------------------------------------------------
//
void CCLFDefaultOperation::SetGrouping( TCLFGrouping aGrouping )
    {
    iGrouping = aGrouping;
    }

// -----------------------------------------------------------------------------
// CCLFDefaultOperation::FilterItemsL
// -----------------------------------------------------------------------------
//
void CCLFDefaultOperation::FilterItemsL( const TArray<MCLFItem*>& aItemList,
                                   RPointerArray<MCLFItem>& aFilteredItemList )
    {
    AppendItemsToArrayL( aItemList, aFilteredItemList );
    }

// -----------------------------------------------------------------------------
// CCLFDefaultOperation::GroupItemsL
// -----------------------------------------------------------------------------
//
void CCLFDefaultOperation::GroupItemsL( const TArray<MCLFItem*>& aSourceList,
                                        RPointerArray<MCLFItem>& aGroupedList )
    {
    iGroupedItemList.ResetAndDestroy();
    switch ( iGrouping )
        {
        case ECLFMusicAlbumGrouping:
            {
            DoMusicAlbumGroupingL( aSourceList, aGroupedList );
            break;
            }
        case ECLFNoGrouping:
        default:
            {
            AppendItemsToArrayL( aSourceList, aGroupedList );
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CCLFDefaultOperation::SortItemsL
// -----------------------------------------------------------------------------
//
void CCLFDefaultOperation::SortItemsL( RPointerArray<MCLFItem>& aItemArray )
    {
    if( iSortingStyle )
        {
        RPointerArray<MCLFItem> array;
        CleanupClosePushL( array );
        RPointerArray<MCLFItem> array1;
        CleanupClosePushL( array1 );
        RPointerArray<MCLFItem>* undefinedItemArray = &array;
        RPointerArray<MCLFItem>* newUndefinedItemArray = &array1;

        SortItemsBySortingStyleL( aItemArray,
                                  *iSortingStyle,
                                  *undefinedItemArray );
        TInt lastAddedItemCount( aItemArray.Count() );
        TInt nextItemIndex( lastAddedItemCount );
        TCLFUndefinedItemPosition position(
                                    iSortingStyle->UndefinedItemPosition() );

        const TInt secondarySorterCount( iSortingStyleArray.Count() );
        for( TInt i = 0 ; i < secondarySorterCount ; ++i )
            {
            MCLFSortingStyle& sortingStyle = *iSortingStyleArray[i];

            newUndefinedItemArray->Reset();
            // sort undefined items
            SortItemsBySortingStyleL( *undefinedItemArray,
                                      sortingStyle,
                                      *newUndefinedItemArray );

            // append sorted items to aItemArray
            AppendItemsToArrayL( undefinedItemArray->Array(),
                                 aItemArray,
                                 position,
                                 nextItemIndex,
                                 lastAddedItemCount );


            position = sortingStyle.UndefinedItemPosition();

            // change pointers
            RPointerArray<MCLFItem>* tmp = undefinedItemArray;
            undefinedItemArray = newUndefinedItemArray;
            newUndefinedItemArray = tmp;
            }


        // copy rest undefined items to correct position
        AppendItemsToArrayL( undefinedItemArray->Array(),
                             aItemArray,
                             position,
                             nextItemIndex,
                             lastAddedItemCount );

                                    // close undefined arrays
        CleanupStack::PopAndDestroy( 2, &array );
        }
        // else no sorting
    }


// -----------------------------------------------------------------------------
// CCLFDefaultOperation::DoMusicAlbumGroupingL
// -----------------------------------------------------------------------------
//
void CCLFDefaultOperation::DoMusicAlbumGroupingL(
                                    const TArray<MCLFItem*>& aSourceList,
                                    RPointerArray<MCLFItem>& aGroupedList )
    {
    CleanupResetAndDestroyPushL( aGroupedList );
    
    CDesCArray* tempAlbumNameArray =
                new (ELeave) CDesCArraySeg( KCLFGroupedItemArrayGranularity );
    CleanupStack::PushL( tempAlbumNameArray );

    const TInt count( aSourceList.Count() );
    for( TInt i = 0 ; i < count ; ++i )
        {
        const MCLFItem* item = aSourceList[i];
        TPtrC albumName;
        if( item->GetField( ECLFFieldIdAlbum, albumName ) == KErrNone )
            {
            TInt pos( 0 );
            if( tempAlbumNameArray->FindIsq( albumName, pos ) )
                {
                // not found
                tempAlbumNameArray->InsertIsqL( albumName );

                // make new item
                MCLFModifiableItem* newItem =
                                ContentListingFactory::NewModifiableItemLC();
                newItem->AddFieldL( ECLFFieldIdAlbum, albumName );
                iGroupedItemList.AppendL( newItem );    // takes ownership
                CleanupStack::Pop(); // newItem

                TPtrC artistName;
                if( item->GetField( ECLFFieldIdArtist, artistName ) == KErrNone )
                    {
                    newItem->AddFieldL( ECLFFieldIdArtist, artistName );
                    }

                // add new item to grouper list
                aGroupedList.AppendL( newItem );
                }
            }
        }
    CleanupStack::PopAndDestroy( tempAlbumNameArray );
    
    CleanupStack::Pop( &aGroupedList );
    }

//  End of File
