/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CLF API test 
*
*/

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <s32mem.h>
#include <ecom.h>
#include <StifParser.h>
#include <StifTestEventInterface.h>
#include <Stiftestinterface.h>
#include "ContentListingFrameworkTest.h"

// the header for the tested class
#include <ContentListingFactory.h>
#include <MCLFChangedItemObserver.h>
#include <MCLFContentListingEngine.h>
#include <MCLFCustomGrouper.h>
#include <MCLFCustomSorter.h>
#include <MCLFItem.h>
#include <MCLFItemListModel.h>
#include <MCLFModifiableItem.h>
#include <MCLFOperationObserver.h>
#include <MCLFPostFilter.h>
#include <MCLFSortingStyle.h>
#include <ContentListingFrameworkTest.rsg>
#include <collate.h>
#include <pathInfo.h>
#include <MCLFProcessObserver.h>
#include <CLFContentListingExtended.hrh>

// CONSTANTS
#ifdef __WINSCW__
_LIT( KTestResourceFile, "z:\\resource\\ContentListingFrameworkTest.rsc" );
#else
_LIT( KTestResourceFile, "c:\\resource\\ContentListingFrameworkTest.rsc" );
#endif
_LIT( KTestFileNameBase, "CLFTestFiles\\TestFile" );
_LIT( KTestFileExt, ".txt" );

const TCLFFieldId KMultibleSortingTestField1 = 0x80000001;
const TCLFFieldId KMultibleSortingTestField2 = 0x80000002;
const TCLFFieldId KMultibleSortingTestField3 = 0x80000003;
const TCLFFieldId KMultibleSortingTestField4 = 0x80000004;
const TCLFFieldId KMultibleSortingTestField5 = 0x80000005;
const TCLFFieldId KMultibleSortingTestField6 = 0x80000006;

const TInt KCLFUpdateFoldersSemanticId = 0x1000;

// ---------------------------------------------------------------------------
// class CMGXAsyncCallback
// ---------------------------------------------------------------------------
//
class CCLFAsyncCallback : public CBase
    {
    public:
        ~CCLFAsyncCallback()
            {
            if ( iActiveWait.IsStarted() )
                {
                iActiveWait.AsyncStop();
                }
            }
    public:
        static void AfterL( TTimeIntervalMicroSeconds32 aInterval )
            {
            CCLFAsyncCallback* self = new( ELeave ) CCLFAsyncCallback();
            CleanupStack::PushL( self );
            CPeriodic* periodic = CPeriodic::NewL( CActive::EPriorityIdle );
            CleanupStack::PushL( periodic );
            TCallBack callBack( CallBackL, self );
            TTimeIntervalMicroSeconds32 interval( 1000000 );
            periodic->Start( aInterval, aInterval, callBack );
            self->iActiveWait.Start();
            periodic->Cancel();
            CleanupStack::PopAndDestroy( 2 ); // periodic, self
            }

        static TInt CallBackL( TAny* aObject )
            {
            CCLFAsyncCallback* self = reinterpret_cast< CCLFAsyncCallback* >( aObject );
            if ( self->iActiveWait.IsStarted() )
                {
                self->iActiveWait.AsyncStop();
                }
            return EFalse;
            }
            
    private:
        CActiveSchedulerWait iActiveWait;
    };

// ---------------------------------------------------------------------------
// class TTestOperationObserver
// ---------------------------------------------------------------------------
//
class TTestOperationObserver : public MCLFOperationObserver
    {
    public:
        TTestOperationObserver()
            // set invalid values
            : iOperationEvent( TCLFOperationEvent( -1 ) ), iError( 1 ), iWait( NULL )
            {}
        void HandleOperationEventL( TCLFOperationEvent aOperationEvent,
                                    TInt aError )
            {
            iError = aError;
            iOperationEvent = aOperationEvent;
            if( iWait &&
                iWait->IsStarted() )
                {
                iWait->AsyncStop();
                }
            }
        TCLFOperationEvent iOperationEvent;
        TInt iError;
        CActiveSchedulerWait* iWait;

    };

// ---------------------------------------------------------------------------
// class TTestCustomSorter
// ---------------------------------------------------------------------------
//
class TTestCustomSorter : public MCLFCustomSorter
    {
    public:
        TTestCustomSorter() : iSortItems( EFalse )
            {
            }
        void SortItemsL( RPointerArray<MCLFItem>& /*aItemArray*/ )
            {
            //aItemArray;
            iSortItems = ETrue;
            }
        TBool iSortItems;

    };

// ---------------------------------------------------------------------------
// class TTestCustomGrouper
// ---------------------------------------------------------------------------
//
class TTestCustomGrouper : public MCLFCustomGrouper
    {
    public:
        TTestCustomGrouper() : iGroupCount( 2 ), iCopyItems( EFalse ), iModifiableItems( NULL )
            {
            }
        void GroupItemsL( const TArray<MCLFItem*>& /*aSourceList*/,
                          RPointerArray<MCLFItem>& aGroupedList )
            {
            //aSourceList;
            if( iCopyItems )
                {
                TInt count( iModifiableItems->Count() );
                for( TInt i = 0 ; i < count ; ++i )
                    {
                    aGroupedList.AppendL( (*iModifiableItems)[i] );
                    }
                }
            else if( iModifiableItems )
                {
                iModifiableItems->ResetAndDestroy();
                for( TInt i = 0 ; i < iGroupCount ; ++i )
                    {
                    MCLFModifiableItem* item = ContentListingFactory::NewModifiableItemLC();
                    iModifiableItems->AppendL( item );
                    CleanupStack::Pop(); // item
                    aGroupedList.AppendL( item );
                    }
                }
            }
        TInt iGroupCount;
        TBool iCopyItems;
        RPointerArray<MCLFModifiableItem>* iModifiableItems;
    };

// ---------------------------------------------------------------------------
// class TTestPostFilter
// ---------------------------------------------------------------------------
//
class TTestPostFilter : public MCLFPostFilter
    {
    public:
        TTestPostFilter() : iShouldFilterCount( 5 ), iAllFilter( EFalse ), iFilteredCount( 0 )
            {
            }

        void FilterItemsL( const TArray<MCLFItem*>& aItemList,
                           RPointerArray<MCLFItem>& aFilteredItemList )
            {
            iFilteredCount = 0;
            if( iAllFilter )
                {
                iFilteredCount = aItemList.Count();
                return;
                }
            for( TInt i = 0 ; i < aItemList.Count() ; ++i )
                {
                if( i < iShouldFilterCount  )
                    {
                    iFilteredCount++;
                    }
                else
                    {
                    aFilteredItemList.AppendL( aItemList[i] );
                    }
                }
            }
        TInt iShouldFilterCount;
        TBool iAllFilter;
        TInt iFilteredCount;

    };

// ---------------------------------------------------------------------------
// class TTestChangedItemObserver
// ---------------------------------------------------------------------------
//
class TTestChangedItemObserver : public MCLFChangedItemObserver
    {
    public:
        TTestChangedItemObserver()
            : iHandleItemChange( EFalse ),
              iLastError( KErrNone ),
              iChangedArray( NULL ),
              iWait( NULL )      
            {
            }
        void HandleItemChangeL( const TArray<TCLFItemId>& aItemIDArray )
            {
            iHandleItemChange = ETrue;
            if( iChangedArray )
                {
                iChangedArray->Reset();
                for( TInt i = 0 ; i < aItemIDArray.Count() ; ++i )
                    {
                    iChangedArray->AppendL( aItemIDArray[i] );
                    }
                }
            if( iWait && iWait->IsStarted() )
                {
                iWait->AsyncStop();
                }

            }
        void HandleError( TInt aError )
            {
            iLastError = aError;
            if( iWait && iWait->IsStarted() )
                {
                iWait->AsyncStop();
                }
            }
        TInt iHandleItemChange;
        TInt iLastError;
        RArray<TCLFItemId>* iChangedArray;
        CActiveSchedulerWait* iWait;
    };

// ---------------------------------------------------------------------------
// class TTestCLFProcessObserver
// ---------------------------------------------------------------------------
//
class TTestCLFProcessObserver : public MCLFProcessObserver
    {
    public:
        TTestCLFProcessObserver()
            : iStartEvent( EFalse ), iEndEvent( EFalse )
            {}
        void HandleCLFProcessEventL( TCLFProcessEvent aProcessEvent )
            {
            switch ( aProcessEvent )
                {
                case ECLFUpdateStart:
                    {
                    iStartEvent = ETrue;
                    break;
                    }
                case ECLFUpdateStop:
                    {
                    iEndEvent = ETrue;
                    break;
                    }
                default:
                    {
                    User::Panic( _L("CLF module test"), 1 );
                    }
                }
            }
        void Reset()
            {
            iStartEvent = EFalse;
            iEndEvent = EFalse;
            }
        TBool iStartEvent;
        TBool iEndEvent;
    };

void SerializeL( const MDesCArray& aDataArray, CBufBase& aBuffer )
    {
    const TInt count( aDataArray.MdcaCount() );
    RBufWriteStream writeStream( aBuffer );
    CleanupClosePushL( writeStream );
    writeStream.WriteInt32L( count );
    for( TInt i = 0 ; i < count ; ++i )
        {
        const TDesC& des = aDataArray.MdcaPoint( i );
        TInt length( des.Length() );
        writeStream.WriteInt32L( length );
        writeStream.WriteL( des, length );
        }
    CleanupStack::PopAndDestroy( &writeStream );
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CContentListingFrameworkTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CContentListingFrameworkTest::Delete() 
    {
    }

// -----------------------------------------------------------------------------
// CCollectionManagerTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CContentListingFrameworkTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 

        // engine		
        ENTRY( "BaseSetup", CContentListingFrameworkTest::BaseSetupL ),
        ENTRY( "SortingStyleResourceSetup", CContentListingFrameworkTest::SortingStyleResourceSetupL ),
        ENTRY( "CreateModelSetup", CContentListingFrameworkTest::CreateModelSetupL ),
        ENTRY( "CreateModelFromResourceSetup", CContentListingFrameworkTest::CreateModelFromResourceSetupL ),
        ENTRY( "ListModelSetup", CContentListingFrameworkTest::ListModelSetupL ),
        ENTRY( "ListModelSetupFromResource", CContentListingFrameworkTest::ListModelSetupFromResourceL ),
        ENTRY( "ListModelAllFileItemsSetup", CContentListingFrameworkTest::ListModelAllFileItemsSetupL ),
        ENTRY( "EngineTestSetup", CContentListingFrameworkTest::EngineTestSetupL ),
        ENTRY( "SortingStyleTestSetup", CContentListingFrameworkTest::SortingStyleTestSetupL ),
        ENTRY( "SortingStyleResourceTestSetup", CContentListingFrameworkTest::SortingStyleResourceTestSetupL ),
        ENTRY( "ModifiableItemTestSetup", CContentListingFrameworkTest::ModifiableItemTestSetupL ),
        ENTRY( "ItemTestSetup", CContentListingFrameworkTest::ItemTestSetupL ),
        ENTRY( "MultibleSortingSetup", CContentListingFrameworkTest::MultibleSortingSetupL ),
        ENTRY( "MultibleSortingResourceSetup", CContentListingFrameworkTest::MultibleSortingResourceSetupL ),
        ENTRY( "Teardown", CContentListingFrameworkTest::Teardown ),
        ENTRY( "CreateEngineTest", CContentListingFrameworkTest::CreateEngineTestL ),
        ENTRY( "CreateModifiableItemTest", CContentListingFrameworkTest::CreateModifiableItemTestL ),
        ENTRY( "CreateSortignStyleTest", CContentListingFrameworkTest::CreateSortignStyleTestL ),
        ENTRY( "CreateSortignStyleFromResourceTest", CContentListingFrameworkTest::CreateSortignStyleFromResourceTestL ),
        ENTRY( "CreateListModelTest", CContentListingFrameworkTest::CreateListModelTestL ),
        ENTRY( "CreateListModelFromResourceTest", CContentListingFrameworkTest::CreateListModelFromResourceTestL ),
        ENTRY( "UpdateItemsTest", CContentListingFrameworkTest::UpdateItemsTestL ),
        ENTRY( "UpdateItemsWithIdTest", CContentListingFrameworkTest::UpdateItemsWithIdTestL ),
        ENTRY( "UpdateItemsWithOpaqueDataFolderTest", CContentListingFrameworkTest::UpdateItemsWithOpaqueDataFolderTestL ),
        ENTRY( "SortingStyleResourceTest", CContentListingFrameworkTest::SortingStyleResourceTestL ),
        ENTRY( "SortingStyleOrderingTest", CContentListingFrameworkTest::SortingStyleOrderingTestL ),
        ENTRY( "SortingStyleDataTypeTest", CContentListingFrameworkTest::SortingStyleDataTypeTestL ),
        ENTRY( "SortingStyleUndefinedItemPositionTest", CContentListingFrameworkTest::SortingStyleUndefinedItemPositionTestL ),
        ENTRY( "SortingStyleFieldTest", CContentListingFrameworkTest::SortingStyleFieldTestL ),
        ENTRY( "RefreshTest", CContentListingFrameworkTest::RefreshTestL ),
        ENTRY( "SetSortingStyleTest", CContentListingFrameworkTest::SetSortingStyleTestL ),
        ENTRY( "SetCustomSorterTest", CContentListingFrameworkTest::SetCustomSorterTestL ),
        ENTRY( "GroupingTest", CContentListingFrameworkTest::GroupingTestL ),
        ENTRY( "SetPostFilterTest", CContentListingFrameworkTest::SetPostFilterTestL ),
        ENTRY( "SetWantedMimeTypesTest", CContentListingFrameworkTest::SetWantedMimeTypesTestL ),
        ENTRY( "SetWantedMediaTypesTest", CContentListingFrameworkTest::SetWantedMediaTypesTestL ),
        ENTRY( "SetWantedMediaAndMimeTypesTest", CContentListingFrameworkTest::SetWantedMediaAndMimeTypesTestL ),
        ENTRY( "MultibleSortingTest", CContentListingFrameworkTest::MultibleSortingTestL ),
        ENTRY( "ModelItemsChangedTest", CContentListingFrameworkTest::ModelItemsChangedTestL ),
        ENTRY( "ItemFieldTest", CContentListingFrameworkTest::ItemFieldTestL ),
        ENTRY( "MIFieldTest", CContentListingFrameworkTest::MIFieldTestL ),
        };
    
    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

/**
 * Assistance methods
 */

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::ResourceL
// ---------------------------------------------------------------------------
//
void CContentListingFrameworkTest::ResourceL( TInt aResourceId )
    {
    delete iDataBuffer;
    iDataBuffer = NULL;
    iDataBuffer = iResourceFile.AllocReadL( aResourceId );
    iResourceReader.SetBuffer( iDataBuffer );
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::SortingStyleResourceL
// ---------------------------------------------------------------------------
//
void CContentListingFrameworkTest::SortingStyleResourceL()
    {
    ResourceL( R_SORTING_STYLE );
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::ListModelResourceL
// ---------------------------------------------------------------------------
//
void CContentListingFrameworkTest::ListModelResourceL()
    {
    ResourceL( R_LIST_MODEL );
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CreateNewFileL
// ---------------------------------------------------------------------------
//
void CContentListingFrameworkTest::CreateNewFileL( TInt aNumber, TDes& aFileName )
    {
    aFileName.Copy( PathInfo::PhoneMemoryRootPath() );
    aFileName.Append( KTestFileNameBase );
    TBuf<125> buf;
    buf.Num( aNumber );
    aFileName.Append( buf );
    aFileName.Append( KTestFileExt );

    RFile file;
    BaflUtils::EnsurePathExistsL( iFs, aFileName );
    TInt error( file.Replace( iFs, aFileName, EFileShareAny | EFileWrite ) );
    if( error == KErrNone )
        {
        error = file.Write( _L8("Test data") );
        }
    file.Close();
    User::LeaveIfError( error );
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::FindTestFileIdL
// ---------------------------------------------------------------------------
//
TCLFItemId CContentListingFrameworkTest::FindTestFileIdL( TInt aNumber )
    {
    TCLFItemId id( 0 );
    MCLFItemListModel* model = iEngine->CreateListModelLC( *iTestObserver );
    iMimeTypeArray->AppendL( _L("*") );
    
    iTestObserver->iWait = &iWait;
    model->SetWantedMimeTypesL( *iMimeTypeArray );
    model->RefreshL();
    iWait.Start();

    TFileName testFileName( PathInfo::PhoneMemoryRootPath() );
    testFileName.Append( KTestFileNameBase );
    TBuf<125> buf;
    buf.Num( aNumber );
    testFileName.Append( buf );
    testFileName.Append( KTestFileExt );
    for( TInt i = 0 ; i < model->ItemCount() ; ++i )
        {
        const MCLFItem& item = model->Item( i );
        TPtrC fileName;
        if( item.GetField( ECLFFieldIdFileNameAndPath, fileName ) != KErrNone )
            {
            continue;
            }
        if( fileName.CompareF( testFileName ) == 0 )
            {
            id = item.ItemId();
            break;
            }
        }
    CleanupStack::PopAndDestroy(); // model
    return id;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CheckFileNameShortingL
// ---------------------------------------------------------------------------
//
TBool CContentListingFrameworkTest::CheckFileNameShortingL()
    {
    TCollationMethod m = *Mem::CollationMethodByIndex( 0 );
    m.iFlags |= TCollationMethod::EIgnoreNone | TCollationMethod::EFoldCase;

    for( TInt i = 0 ; i < iListModel->ItemCount() -1 ; ++i )
        {
        const MCLFItem& item = iListModel->Item( i );
        const MCLFItem& item1 = iListModel->Item( i + 1 );
        TPtrC name;
        TPtrC name1;

        if( item.GetField( ECLFFieldIdFileName, name ) != KErrNone ||
            item1.GetField( ECLFFieldIdFileName, name1 ) != KErrNone )
            {
            return EFalse;
            }
        if( name.CompareC( name1, 3, &m ) > 0 )
            {
            return EFalse;
            }
        }
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CheckFileSizeShortingL
// ---------------------------------------------------------------------------
//
TBool CContentListingFrameworkTest::CheckFileSizeShortingL()
    {
    for( TInt i = 0 ; i < iListModel->ItemCount() -1 ; ++i )
        {
        const MCLFItem& item = iListModel->Item( i );
        const MCLFItem& item1 = iListModel->Item( i + 1 );
        TInt32 size;
        TInt32 size1;
        if( item.GetField( ECLFFieldIdFileSize, size ) != KErrNone ||
            item1.GetField( ECLFFieldIdFileSize, size1 ) != KErrNone )
            {
            return EFalse;
            }
        if( size < size1 )
            {
            return EFalse;
            }
        }
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CheckMultibleSortingShortingL
// ---------------------------------------------------------------------------
//
TBool CContentListingFrameworkTest::CheckMultibleSortingShortingL()
    {
    const MCLFItem* item = &( iListModel->Item( 0 ) );
    TInt32 data( 0 );
    if( item->GetField( KMultibleSortingTestField2, data ) != KErrNone ||
        data != 5 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 1 ) );
    if( item->GetField( KMultibleSortingTestField2, data ) != KErrNone ||
        data != 4 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 2 ) );
    if( item->GetField( KMultibleSortingTestField2, data ) != KErrNone ||
        data != 3 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 3 ) );
    if( item->GetField( KMultibleSortingTestField3, data ) != KErrNone ||
        data != 6 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 4 ) );
    if( item->GetField( KMultibleSortingTestField3, data ) != KErrNone ||
        data != 7 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 5 ) );
    if( item->GetField( KMultibleSortingTestField3, data ) != KErrNone ||
        data != 8 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 6 ) );
    if( item->GetField( KMultibleSortingTestField4, data ) != KErrNone ||
        data != 9 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 7 ) );
    if( item->GetField( KMultibleSortingTestField4, data ) != KErrNone ||
        data != 10 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 8 ) );
    if( item->GetField( KMultibleSortingTestField4, data ) != KErrNone ||
        data != 11 )
        {
        return EFalse;
        }
// unsorted start
    item = &( iListModel->Item( 9 ) );
    if( item->GetField( KMultibleSortingTestField6, data ) != KErrNone ||
        ( data < 15 || data > 17 ) )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 10 ) );
    if( item->GetField( KMultibleSortingTestField6, data ) != KErrNone ||
        ( data < 15 || data > 17 ) )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 11 ) );
    if( item->GetField( KMultibleSortingTestField6, data ) != KErrNone ||
        ( data < 15 || data > 17 ) )
        {
        return EFalse;
        }
// unsorted end
    item = &( iListModel->Item( 12 ) );
    if( item->GetField( KMultibleSortingTestField5, data ) != KErrNone ||
        data != 12 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 13 ) );
    if( item->GetField( KMultibleSortingTestField5, data ) != KErrNone ||
        data != 13 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 14 ) );
    if( item->GetField( KMultibleSortingTestField5, data ) != KErrNone ||
        data != 14 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 15 ) );
    if( item->GetField( KMultibleSortingTestField1, data ) != KErrNone ||
        data != 0 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 16 ) );
    if( item->GetField( KMultibleSortingTestField1, data ) != KErrNone ||
        data != 1 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 17 ) );
    if( item->GetField( KMultibleSortingTestField1, data ) != KErrNone ||
        data != 2 )
        {
        return EFalse;
        }

    return ETrue;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CheckMultibleSortingShorting2L
// ---------------------------------------------------------------------------
//
TBool CContentListingFrameworkTest::CheckMultibleSortingShorting2L()
    {
// check sorted
    const MCLFItem* item = &( iListModel->Item( 0 ) );
    TInt32 data( 0 );
    if( item->GetField( KMultibleSortingTestField5, data ) != KErrNone ||
        data != 12 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 1 ) );
    if( item->GetField( KMultibleSortingTestField5, data ) != KErrNone ||
        data != 13 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 2 ) );
    if( item->GetField( KMultibleSortingTestField5, data ) != KErrNone ||
        data != 14 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 3 ) );
    if( item->GetField( KMultibleSortingTestField6, data ) != KErrNone ||
        data != 15 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 4 ) );
    if( item->GetField( KMultibleSortingTestField6, data ) != KErrNone ||
        data != 16 )
        {
        return EFalse;
        }
    item = &( iListModel->Item( 5 ) );
    if( item->GetField( KMultibleSortingTestField6, data ) != KErrNone ||
        data != 17 )
        {
        return EFalse;
        }
// check unsorted
    for( TInt i = 6 ; i < 18 ; ++i )
        {
        item = &( iListModel->Item( i ) );
        if( item->GetField( KMultibleSortingTestField1, data ) != KErrNone )
            {
            if( item->GetField( KMultibleSortingTestField2, data ) != KErrNone )
                {
                if( item->GetField( KMultibleSortingTestField3, data ) != KErrNone )
                    {
                    if( item->GetField( KMultibleSortingTestField4, data ) != KErrNone )
                        {
                        return EFalse;
                        }
                    }
                }
            }
        if( data < 0 || data > 11 )
            {
            return EFalse;
            }
        }

    return ETrue;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CheckFileDateShortingL
// ---------------------------------------------------------------------------
//
TBool CContentListingFrameworkTest::CheckFileDateShortingL()
    {
    for( TInt i = 0 ; i < iListModel->ItemCount() -1 ; ++i )
        {
        const MCLFItem& item = iListModel->Item( i );
        const MCLFItem& item1 = iListModel->Item( i + 1 );
        TTime date;
        TTime date1;
        if( item.GetField( ECLFFieldIdFileDate, date ) != KErrNone ||
            item1.GetField( ECLFFieldIdFileDate, date1 ) != KErrNone )
            {
            return EFalse;
            }
        if( date > date1 )
            {
            return EFalse;
            }
        }
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CheckFileTypesL
// ---------------------------------------------------------------------------
//
TBool CContentListingFrameworkTest::CheckFileTypesL( const MDesCArray& aMimeTypeArray,
                                            const TArray<TInt>& aMediaTypes )
    {
    for( TInt i = 0 ; i < iListModel->ItemCount() ; ++i )
        {
        TPtrC mimeType;
        TInt32 mediaType;
        TInt error = iListModel->Item( i ).GetField( ECLFFieldIdMimeType, mimeType );
        if( iListModel->Item( i ).GetField( ECLFFieldIdMediaType, mediaType ) != KErrNone )
            {
            return EFalse;
            }
        TBool mimeTypeVal( EFalse );
        TBool mediaTypeVal( EFalse );
        if( error == KErrNone )
            {
            mimeTypeVal = CheckMimeTypesL( aMimeTypeArray, mimeType );
            }
        mediaTypeVal = CheckMediaTypesL( aMediaTypes, TCLFMediaType( mediaType ) );
        if( !( mimeTypeVal || mediaTypeVal ) )
            {
            return EFalse;
            }
        }
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CheckMimeTypesL
// ---------------------------------------------------------------------------
//
TBool CContentListingFrameworkTest::CheckMimeTypesL( const MDesCArray& aMimeTypeArray,
                                            const TDesC& aMimeType )
    {
    for( TInt j = 0 ; j < aMimeTypeArray.MdcaCount() ; ++j )
        {
        if( aMimeTypeArray.MdcaPoint( j ).Match( aMimeType ) == KErrNotFound )
            {
            return ETrue;
            }
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CheckMediaTypesL
// ---------------------------------------------------------------------------
//
TBool CContentListingFrameworkTest::CheckMediaTypesL( const TArray<TInt>& aMediaTypes,
                                             TCLFMediaType aMediaType )
    {
    for( TInt j = 0 ; j < aMediaTypes.Count() ; ++j )
        {
        if( aMediaTypes[j] == aMediaType )
            {
            return ETrue;
            }
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::MakeOpaqueDataL
// ---------------------------------------------------------------------------
//
HBufC8* CContentListingFrameworkTest::MakeOpaqueDataL( const MDesCArray& aFiles )
    {
    CDesCArray* folderArray = new ( ELeave ) CDesCArraySeg( 8 );
    CleanupStack::PushL( folderArray );
    TInt count( aFiles.MdcaCount() );
    for( TInt i = 0 ; i < count ; ++i )
        {
        TPtrC folderPath( TParsePtrC(
                            aFiles.MdcaPoint( i ) ).DriveAndPath() );
        TInt tmp( 0 );
        if( folderArray->Find( folderPath, tmp, ECmpFolded ) != 0 )
            {
            folderArray->AppendL( folderPath );
            }
        }

    CBufBase* dynBuffer = CBufFlat::NewL( 64 );
    CleanupStack::PushL( dynBuffer );
    SerializeL( *folderArray, *dynBuffer );
    HBufC8* ret = dynBuffer->Ptr( 0 ).AllocL();
    CleanupStack::PopAndDestroy( 2, folderArray );
    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::MakeMultibleSortingItemsL
// ---------------------------------------------------------------------------
//
void CContentListingFrameworkTest::MakeMultibleSortingItemsL(
                        RPointerArray<MCLFModifiableItem>& aItemArray )
    {
    for( TInt i = 0 ; i < 18 ; ++i )
        {
        MCLFModifiableItem* item = ContentListingFactory::NewModifiableItemLC();
        aItemArray.AppendL( item );
        CleanupStack::Pop(); //item

        if( i < 3 )
            {
            item->AddFieldL( KMultibleSortingTestField1, i );
            }
        else if( i < 6 )
            {
            item->AddFieldL( KMultibleSortingTestField2, i );
            }
        else if( i < 9 )
            {
            item->AddFieldL( KMultibleSortingTestField3, i );
            }
        else if( i < 12 )
            {
            item->AddFieldL( KMultibleSortingTestField4, i );
            }
        else if( i < 15 )
            {
            item->AddFieldL( KMultibleSortingTestField5, i );
            }
        else
            {
            item->AddFieldL( KMultibleSortingTestField6, i );
            }
        }
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::FindItem
// ---------------------------------------------------------------------------
//
const MCLFItem* CContentListingFrameworkTest::FindItem( MCLFItemListModel& aModel, TCLFItemId aItemId )
    {
    for( TInt i = 0 ; i < aModel.ItemCount() ; ++i )
        {
        const MCLFItem& item = aModel.Item( i );
        if( item.ItemId() == aItemId )
            {
            return &item;
            }
        }
    return NULL;
    }

/**
 * Setup
 */

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::BaseSetupL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::BaseSetupL( CStifItemParser& /* aItem */ )
    {
    iFs.Close();
    iResourceFile.Close();
    
    User::LeaveIfError( iFs.Connect() );
    TFileName fileName( KTestResourceFile );
    BaflUtils::NearestLanguageFile( iFs, fileName );
    iResourceFile.OpenL( iFs, KTestResourceFile );
    iResourceFile.ConfirmSignatureL( 0 );

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::SortingStyleResourceSetupL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::SortingStyleResourceSetupL( CStifItemParser& aItem )
    {
    BaseSetupL( aItem );
    SortingStyleResourceL();

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CreateModelSetupL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::CreateModelSetupL( CStifItemParser& /* aItem */ )
    {
    iEngine = ContentListingFactory::NewContentListingEngineLC();
    CleanupStack::Pop(); //engine
    iTestObserver  = new (ELeave) TTestOperationObserver;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CreateModelFromResourceSetupL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::CreateModelFromResourceSetupL( CStifItemParser& aItem )
    {
    BaseSetupL( aItem );
    CreateModelSetupL( aItem );
    ListModelResourceL();

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::ListModelSetupL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::ListModelSetupL( CStifItemParser& aItem )
    {
    BaseSetupL( aItem );
    CreateModelSetupL( aItem );
    iListModel = iEngine->CreateListModelLC( *iTestObserver );
    CleanupStack::Pop(); //listModel

    iSortingStyle = ContentListingFactory::NewSortingStyleLC();
    CleanupStack::Pop(); // sortingStyle
    iSortingStyle1 = ContentListingFactory::NewSortingStyleLC();
    CleanupStack::Pop(); // sortingStyle
    iSortingStyle2 = ContentListingFactory::NewSortingStyleLC();
    CleanupStack::Pop(); // sortingStyle
    iSortingStyle3 = ContentListingFactory::NewSortingStyleLC();
    CleanupStack::Pop(); // sortingStyle

    iTestSorter = new (ELeave) TTestCustomSorter;
    iTestSorter1 = new (ELeave) TTestCustomSorter;
    iTestGrouper = new (ELeave) TTestCustomGrouper;
    iTestGrouper1 = new (ELeave) TTestCustomGrouper;
    iTestFilter = new (ELeave) TTestPostFilter;
    iTestFilter1 = new (ELeave) TTestPostFilter;
    iMimeTypeArray = new (ELeave) CDesCArrayFlat( 8 );
    iMimeTypeArray1 = new (ELeave) CDesCArrayFlat( 8 );
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::MultibleSortingSetupL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::MultibleSortingSetupL( CStifItemParser& aItem )
    {
    ListModelSetupL( aItem );
    MakeMultibleSortingItemsL( iModifiableItems );

// use custom grouper to make own items
    iTestGrouper->iModifiableItems = &iModifiableItems;
    iTestGrouper->iCopyItems = ETrue;
    iListModel->SetCustomGrouper( iTestGrouper );

// set sorters
    iSortingStyle->ResetL();
    iSortingStyle->SetSortingDataType( ECLFItemDataTypeTInt32 );
    iSortingStyle->AddFieldL( KMultibleSortingTestField1 );
    iSortingStyle->SetUndefinedItemPosition( ECLFSortingStyleUndefinedFirst );

    iSortingStyle1->ResetL();
    iSortingStyle1->SetSortingDataType( ECLFItemDataTypeTInt32 );
    iSortingStyle1->AddFieldL( KMultibleSortingTestField2 );
    iSortingStyle1->SetUndefinedItemPosition( ECLFSortingStyleUndefinedEnd );
    iSortingStyle1->SetOrdering( ECLFOrderingDescending );

    iSortingStyle2->ResetL();
    iSortingStyle2->SetSortingDataType( ECLFItemDataTypeTInt32 );
    iSortingStyle2->AddFieldL( KMultibleSortingTestField3 );
    iSortingStyle2->AddFieldL( KMultibleSortingTestField4 );
    iSortingStyle2->SetUndefinedItemPosition( ECLFSortingStyleUndefinedEnd );

    iSortingStyle3->ResetL();
    iSortingStyle3->SetSortingDataType( ECLFItemDataTypeTInt32 );
    iSortingStyle3->AddFieldL( KMultibleSortingTestField5 );
    iSortingStyle3->SetUndefinedItemPosition( ECLFSortingStyleUndefinedFirst );

    iListModel->SetSortingStyle( iSortingStyle );
    iListModel->AppendSecondarySortingStyleL( *iSortingStyle1 );
    iListModel->AppendSecondarySortingStyleL( *iSortingStyle2 );
    iListModel->AppendSecondarySortingStyleL( *iSortingStyle3 );

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::MultibleSortingResourceSetupL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::MultibleSortingResourceSetupL( CStifItemParser& aItem )
    {
    ListModelSetupL( aItem );
    delete iListModel;
    iListModel = NULL;
    ResourceL( R_LIST_MODEL_MULTIBLE );
    iListModel = iEngine->CreateListModelLC( *iTestObserver, iResourceReader );
    CleanupStack::Pop(); //listModel

// use custom grouper to make own items
    MakeMultibleSortingItemsL( iModifiableItems );
    iTestGrouper->iModifiableItems = &iModifiableItems;
    iTestGrouper->iCopyItems = ETrue;
    iListModel->SetCustomGrouper( iTestGrouper );

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::ListModelAllFileItemsSetupL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::ListModelAllFileItemsSetupL( CStifItemParser& aItem )
    {
    ListModelSetupL( aItem );
    iMediaTypeArray.AppendL( ECLFMediaTypeVideo );
    iMediaTypeArray.AppendL( ECLFMediaTypeImage );
    iMediaTypeArray.AppendL( ECLFMediaTypeSound );
    iMediaTypeArray.AppendL( ECLFMediaTypeMusic );
    iMediaTypeArray.AppendL( ECLFMediaTypeStreamingURL );
    iMediaTypeArray.AppendL( ECLFMediaTypePlaylist );
    iListModel->SetWantedMediaTypesL( iMediaTypeArray.Array() );
    iTestObserver->iWait = &iWait;
    iListModel->RefreshL();
    iWait.Start();
    iItemCount = iListModel->ItemCount();
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::ListModelSetupFromResourceL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::ListModelSetupFromResourceL( CStifItemParser& aItem )
    {
    BaseSetupL( aItem );
    CreateModelSetupL( aItem );
    ListModelResourceL();
    iListModel = iEngine->CreateListModelLC( *iTestObserver, iResourceReader );
    CleanupStack::Pop(); //listModel

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::EngineTestSetupL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::EngineTestSetupL( CStifItemParser& /* aItem */ )
    {
    User::LeaveIfError( iFs.Connect() );
    iTestObserver  = new (ELeave) TTestOperationObserver;
    iMimeTypeArray = new (ELeave) CDesCArrayFlat( 8 );
    iEngine = ContentListingFactory::NewContentListingEngineLC();
    CleanupStack::Pop(); //engine
    iChangedItemObserver = new (ELeave) TTestChangedItemObserver;
    iChangedItemObserver1 = new (ELeave) TTestChangedItemObserver;
    iTestCLFProcessObserver = new (ELeave) TTestCLFProcessObserver;
    iTestCLFProcessObserver1 = new (ELeave) TTestCLFProcessObserver;

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::SortingStyleTestSetupL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::SortingStyleTestSetupL( CStifItemParser& /* aItem */ )
    {
    iSortingStyle = ContentListingFactory::NewSortingStyleLC();
    CleanupStack::Pop(); // sortingStyle

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::SortingStyleResourceTestSetupL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::SortingStyleResourceTestSetupL( CStifItemParser& aItem )
    {
    SortingStyleResourceSetupL( aItem );
    iSortingStyle1 = ContentListingFactory::NewSortingStyleLC( iResourceReader );
    CleanupStack::Pop(); // sortingStyle

    ResourceL( R_SORTING_STYLE_EMPTY );
    iSortingStyle = ContentListingFactory::NewSortingStyleLC( iResourceReader );
    CleanupStack::Pop(); // sortingStyle

    ResourceL( R_SORTING_STYLE_UNDEFINEDITEM );
    iSortingStyle2 = ContentListingFactory::NewSortingStyleLC( iResourceReader );
    CleanupStack::Pop(); // sortingStyle

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::ModifiableItemTestSetupL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::ModifiableItemTestSetupL( CStifItemParser& /* aItem */ )
    {
    iModifiableItem = ContentListingFactory::NewModifiableItemLC();
    CleanupStack::Pop(); //item
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::ItemTestSetupL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::ItemTestSetupL( CStifItemParser& aItem )
    {
    TInt ret = KErrNone;

    ListModelSetupL( aItem );
    CreateNewFileL( 12, iFileName );
    iEngine->UpdateItemsL();
    iMimeTypeArray->Reset();
    iMimeTypeArray->AppendL( _L("*") );
    iListModel->SetWantedMimeTypesL( *iMimeTypeArray );

    iTestObserver->iWait = &iWait;
    iListModel->RefreshL();
    iWait.Start();

    for( TInt i = 0 ; i < iListModel->ItemCount() ; ++i )
        {
        const MCLFItem& item = iListModel->Item( i );
        TPtrC fn;
        item.GetField( ECLFFieldIdFileNameAndPath, fn );
        if( iFileName.CompareF( fn ) == 0 )
            {
            iItem = &item;
            }
        }

    if (!iItem)
        {
        ret = KErrUnknown;
        }
    return ret;
    }

/**
 * Teardown
 */

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::Teardown
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::Teardown( CStifItemParser& /* aItem */ )
    {
    iResourceFile.Close();
    iFs.Close();
    iMediaTypeArray.Reset();
    iMediaTypeArray.Close();
    iMediaTypeArray1.Reset();
    iMediaTypeArray1.Close();
    iUpdateItemIdArray.Reset();
    iUpdateItemIdArray.Close();
    iChangedArray.Reset();
    iChangedArray.Close();
    iModifiableItems.ResetAndDestroy();
    iModifiableItems.Close();
    
    delete iListModel;
    iListModel = NULL;
    delete iEngine;
    iEngine = NULL;
    delete iSortingStyle;
    iSortingStyle = NULL;
    delete iSortingStyle1;
    iSortingStyle1 = NULL;
    delete iSortingStyle2;
    iSortingStyle2 = NULL;
    delete iSortingStyle3;
    iSortingStyle3 = NULL;
    delete iDataBuffer;
    iDataBuffer = NULL;    
    delete iTestObserver;
    iTestObserver = NULL;
    delete iTestSorter;
    iTestSorter = NULL;
    delete iTestSorter1;
    iTestSorter1 = NULL;
    delete iTestGrouper;
    iTestGrouper = NULL;
    delete iTestGrouper1;
    iTestGrouper1 = NULL;
    delete iTestFilter;
    iTestFilter = NULL;
    delete iTestFilter1;
    iTestFilter1 = NULL;
    delete iMimeTypeArray;
    iMimeTypeArray = NULL;
    delete iMimeTypeArray1;
    iMimeTypeArray1 = NULL;
    delete iChangedItemObserver;
    iChangedItemObserver = NULL;
    delete iChangedItemObserver1;
    iChangedItemObserver1 = NULL;
    delete iOpaqueData;
    iOpaqueData = NULL;
    delete iModifiableItem;
    iModifiableItem = NULL;
    delete iTestCLFProcessObserver1;
    iTestCLFProcessObserver1 = NULL;
    delete iTestCLFProcessObserver;
    iTestCLFProcessObserver = NULL;
    
    TTimeIntervalMicroSeconds32 time = 1000000;
    TRAP_IGNORE( CCLFAsyncCallback::AfterL( time ) );

    return KErrNone;
    }

/**
 * Tests, construction
 */

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CreateEngineTestL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::CreateEngineTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    iEngine = ContentListingFactory::NewContentListingEngineLC();
    if (!iEngine)
        {
        ret = KErrUnknown;
        }
    CleanupStack::PopAndDestroy(); //engine
    iEngine = NULL;
    iEngine = ContentListingFactory::NewContentListingEngineLC();
    if (!iEngine)
        {
        ret = KErrUnknown;
        }
    CleanupStack::Pop(); // engine

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CreateModifiableItemTestL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::CreateModifiableItemTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    MCLFModifiableItem* item = NULL;
    item = ContentListingFactory::NewModifiableItemLC();
    if (!item)
        {
        ret = KErrUnknown;
        }
    CleanupStack::PopAndDestroy(); // item
    item = NULL;
    item = ContentListingFactory::NewModifiableItemLC();
    if (!item)
        {
        ret = KErrUnknown;
        }
    CleanupStack::Pop(); // item
    delete item;

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CreateSortignStyleTestL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::CreateSortignStyleTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    MCLFSortingStyle* sortingStyle = NULL;
    sortingStyle = ContentListingFactory::NewSortingStyleLC();
    if (!sortingStyle)
        {
        ret = KErrUnknown;
        }
    CleanupStack::PopAndDestroy(); // sortingStyle
    sortingStyle = NULL;
    sortingStyle = ContentListingFactory::NewSortingStyleLC();
    if (!sortingStyle)
        {
        ret = KErrUnknown;
        }
    CleanupStack::Pop(); // sortingStyle
    delete sortingStyle;

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CreateSortignStyleFromResourceTestL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::CreateSortignStyleFromResourceTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    MCLFSortingStyle* sortingStyle = NULL;
    sortingStyle = ContentListingFactory::NewSortingStyleLC( iResourceReader );
    if (!sortingStyle)
        {
        ret = KErrUnknown;
        }
    CleanupStack::PopAndDestroy(); // sortingStyle
    sortingStyle = NULL;

    SortingStyleResourceL(); // refresh resource reader
    sortingStyle = ContentListingFactory::NewSortingStyleLC( iResourceReader );
    if (!sortingStyle)
        {
        ret = KErrUnknown;
        }
    CleanupStack::Pop(); // sortingStyle
    delete sortingStyle;

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CreateListModelTestL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::CreateListModelTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    MCLFItemListModel* model = NULL;
    model = iEngine->CreateListModelLC( *iTestObserver );
    if (!model)
        {
        ret = KErrUnknown;
        }
    CleanupStack::PopAndDestroy();  //listModel
    model = NULL;

    model = iEngine->CreateListModelLC( *iTestObserver );
    if (!model)
        {
        ret = KErrUnknown;
        }
    CleanupStack::Pop(); // listModel
    delete model;

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::CreateListModelFromResourceTestL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::CreateListModelFromResourceTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    MCLFItemListModel* model = NULL;
    model = iEngine->CreateListModelLC( *iTestObserver, iResourceReader );
    if (!model)
        {
        ret = KErrUnknown;
        }
    CleanupStack::PopAndDestroy(); //listModel
    model = NULL;

    ListModelResourceL();
    model = iEngine->CreateListModelLC( *iTestObserver, iResourceReader );
    if (!model)
        {
        ret = KErrUnknown;
        }
    CleanupStack::Pop(); // listModel
    delete model;

    ResourceL( R_LIST_MODEL_INCORRECT_VERSION );
    TRAPD(error, iEngine->CreateListModelLC( *iTestObserver, iResourceReader ));
    if (error != KErrNotSupported)
        {
        ret = KErrUnknown;
        CleanupStack::Pop(); // listModel
        }

    return ret;
    }

/**
 * Tests, engine
 */

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::UpdateItemsTestL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::UpdateItemsTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    // MDS will do the updating automatically in the background,
    // thus only checking that the methods return correctly
    iChangedItemObserver->iWait = &iWait;
    iChangedItemObserver->iChangedArray = &iChangedArray;
    iEngine->AddChangedItemObserverL( *iChangedItemObserver );
    iEngine->AddCLFProcessObserverL( *iTestCLFProcessObserver );
    iEngine->AddCLFProcessObserverL( *iTestCLFProcessObserver1 );

    CreateNewFileL( 0, iFileName );
    CreateNewFileL( 1, iFileName );
    CreateNewFileL( 2, iFileName );
    CreateNewFileL( 3, iFileName );
    CreateNewFileL( 4, iFileName );
    CreateNewFileL( 5, iFileName );

// update server
// to avoid incorrect test result
    CreateNewFileL( 0, iFileName );
    iEngine->UpdateItemsL();

    ret = iChangedItemObserver->iLastError;

    iChangedArray.Reset();
    iEngine->RemoveCLFProcessObserver( *iTestCLFProcessObserver1 );
    iChangedItemObserver->iHandleItemChange = EFalse;
    iChangedItemObserver1->iHandleItemChange = EFalse;
    iTestCLFProcessObserver->Reset();
    iTestCLFProcessObserver1->Reset();
    iEngine->UpdateItemsL();

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::UpdateItemsWithIdTestL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::UpdateItemsWithIdTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    // MDS will do the updating automatically in the background,
    // thus only checking that the methods return correctly
    iChangedItemObserver->iWait = &iWait;
    iChangedItemObserver->iChangedArray = &iChangedArray;
    iEngine->AddChangedItemObserverL( *iChangedItemObserver );
    iEngine->AddChangedItemObserverL( *iChangedItemObserver1 );
    iEngine->AddCLFProcessObserverL( *iTestCLFProcessObserver );

    CreateNewFileL( 6, iFileName );
    CreateNewFileL( 7, iFileName );
    CreateNewFileL( 8, iFileName );

// update server
// to aTInt incorrect test result
    CreateNewFileL( 6, iFileName );
    iEngine->UpdateItemsL();
    
    if (iChangedItemObserver->iLastError != KErrNone)
        {
        ret = KErrUnknown;
        }

// start testing
// update by id
    TUint id1 = FindTestFileIdL( 7 );
    TUint id0 = FindTestFileIdL( 6 );
    TUint id2 = FindTestFileIdL( 8 );
    iUpdateItemIdArray.AppendL( id1 );
    iChangedItemObserver->iHandleItemChange = EFalse;
    iChangedItemObserver1->iHandleItemChange = EFalse;
    iTestCLFProcessObserver->Reset();
    iTestCLFProcessObserver1->Reset();
    CreateNewFileL( 6, iFileName );
    CreateNewFileL( 7, iFileName );
    iEngine->UpdateItemsL( iUpdateItemIdArray.Array() );
    
    if (iChangedItemObserver->iLastError != KErrNone)
        {
        ret = KErrUnknown;
        }

    iEngine->RemoveChangedItemObserver( *iChangedItemObserver1 );
    iEngine->AddCLFProcessObserverL( *iTestCLFProcessObserver1 );

    iChangedItemObserver->iHandleItemChange = EFalse;
    iChangedItemObserver1->iHandleItemChange = EFalse;
    iTestCLFProcessObserver->Reset();
    iTestCLFProcessObserver1->Reset();
    iUpdateItemIdArray.AppendL( id0 );
    iUpdateItemIdArray.AppendL( id2 );
    
// update server
// to aTInt incorrect test result
    iEngine->UpdateItemsL();
    
    if (iChangedItemObserver->iLastError != KErrNone)
        {
        ret = KErrUnknown;
        }
    
    CreateNewFileL( 6, iFileName );
    CreateNewFileL( 7, iFileName );
    CreateNewFileL( 8, iFileName );
    iChangedArray.Reset();
    
    iEngine->UpdateItemsL( iUpdateItemIdArray.Array() );

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::UpdateItemsWithOpaqueDataFolderTestL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::UpdateItemsWithOpaqueDataFolderTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    // start testing
    // update by opaque data
    // folders data
    iSemanticId = KCLFUpdateFoldersSemanticId;
    delete iOpaqueData;
    iOpaqueData = NULL;

    CDesCArray* fileArray = new (ELeave) CDesCArraySeg( 8 );
    iOpaqueData = MakeOpaqueDataL( *fileArray );
    // Calls internally same MDS method as when updating all data
    // thus only interested if this call leaves
    iEngine->UpdateItemsL( iSemanticId, *iOpaqueData );

    return ret;
    }

/**
 * Tests, list model
 */

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::RefreshTestL
// ---------------------------------------------------------------------------
//  
TInt CContentListingFrameworkTest::RefreshTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    iMimeTypeArray->Reset();
    iMimeTypeArray->AppendL( _L("*") );
    iListModel->SetWantedMimeTypesL( *iMimeTypeArray );

    iTestObserver->iWait = &iWait;
    iTestObserver->iError = 100;
    iListModel->RefreshL();
    iWait.Start();

    if (iTestObserver->iOperationEvent != ECLFRefreshComplete)
        {
        ret = KErrUnknown;
        }
    if (iTestObserver->iError != KErrNone)
        {
        ret = KErrUnknown;
        }
    if (iListModel->ItemCount() != 0)
        {
        ret = KErrUnknown;
        }

// cancel refresh
    iListModel->RefreshL();
    iListModel->CancelRefresh();

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::SetSortingStyleTestL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::SetSortingStyleTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    iListModel->SetSortingStyle( NULL );
    iListModel->RefreshL( ECLFRefreshAll );
    if (CheckFileNameShortingL())
        {
        ret = KErrUnknown;
        }

// file name sorting
    iSortingStyle->ResetL();
    iSortingStyle->SetSortingDataType( ECLFItemDataTypeDesC );
    iSortingStyle->AddFieldL( ECLFFieldIdFileName );
    iListModel->SetSortingStyle( iSortingStyle );
    iListModel->RefreshL( ECLFRefreshAll );
    if (!CheckFileNameShortingL())
        {
        ret = KErrUnknown;
        }

// file size sorting
    iSortingStyle1->ResetL();
    iSortingStyle1->SetOrdering( ECLFOrderingDescending );
    iSortingStyle1->SetSortingDataType( ECLFItemDataTypeTInt32 );
    iSortingStyle1->AddFieldL( ECLFFieldIdFileSize );
    iListModel->SetSortingStyle( iSortingStyle1 );
    iListModel->RefreshL( ECLFRefreshAll );
    if (CheckFileNameShortingL())
        {
        ret = KErrUnknown;
        }

    iListModel->SetSortingStyle( NULL );
    iListModel->RefreshL( ECLFRefreshAll );
    if (CheckFileNameShortingL() || CheckFileSizeShortingL() || CheckFileDateShortingL())
        {
        ret = KErrUnknown;
        }

// time sorting
    iSortingStyle->ResetL();
    iSortingStyle->SetSortingDataType( ECLFItemDataTypeTTime );
    iSortingStyle->AddFieldL( ECLFFieldIdFileDate );
    iListModel->SetSortingStyle( iSortingStyle );
    iListModel->RefreshL( ECLFRefreshAll );
    if (!CheckFileDateShortingL())
        {
        ret = KErrUnknown;
        }

// parameter test (time)
    iListModel->SetSortingStyle( NULL );
    iListModel->RefreshL( ECLFRefreshAll );
    if (CheckFileNameShortingL() || CheckFileSizeShortingL() || CheckFileDateShortingL())
        {
        ret = KErrUnknown;
        }

    iListModel->SetSortingStyle( iSortingStyle );
    iListModel->RefreshL( ECLFRefreshPostFilter );
    if (CheckFileNameShortingL() || CheckFileSizeShortingL() || CheckFileDateShortingL())
        {
        ret = KErrUnknown;
        }

    iListModel->RefreshL( ECLFRefreshGrouping );
    if (CheckFileNameShortingL() || CheckFileSizeShortingL() || CheckFileDateShortingL())
        {
        ret = KErrUnknown;
        }

    iListModel->RefreshL( ECLFRefreshSorting );
    if (CheckFileNameShortingL() || CheckFileSizeShortingL() || !CheckFileDateShortingL())
        {
        ret = KErrUnknown;
        }

// custom sorter (overwrite sorting style)
    iTestSorter->iSortItems = EFalse;
    iListModel->SetCustomSorter( iTestSorter );
    iListModel->RefreshL( ECLFRefreshAll );
    if (!(iTestSorter->iSortItems))
        {
        ret = KErrUnknown;
        }
    if (CheckFileNameShortingL() || CheckFileSizeShortingL() || CheckFileDateShortingL())
        {
        ret = KErrUnknown;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::SetCustomSorterTestL
// ---------------------------------------------------------------------------
//
TInt CContentListingFrameworkTest::SetCustomSorterTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    iTestSorter->iSortItems = EFalse;
    iTestSorter1->iSortItems = EFalse;

    iListModel->SetCustomSorter( NULL );
    iListModel->RefreshL( ECLFRefreshAll );
    if ((iTestSorter->iSortItems) || (iTestSorter1->iSortItems))
        {
        ret = KErrUnknown;
        }

    iTestSorter->iSortItems = EFalse;
    iTestSorter1->iSortItems = EFalse;

    iListModel->SetCustomSorter( iTestSorter );
    iListModel->RefreshL( ECLFRefreshAll );
    if (!(iTestSorter->iSortItems) || (iTestSorter1->iSortItems))
        {
        ret = KErrUnknown;
        }

    iTestSorter->iSortItems = EFalse;
    iTestSorter1->iSortItems = EFalse;

    iListModel->SetCustomSorter( iTestSorter1 );
    iListModel->RefreshL( ECLFRefreshAll );
    if ((iTestSorter->iSortItems) || !(iTestSorter1->iSortItems))
        {
        ret = KErrUnknown;
        }

    iTestSorter->iSortItems = EFalse;
    iTestSorter1->iSortItems = EFalse;

    iListModel->SetCustomSorter( NULL );
    iListModel->RefreshL( ECLFRefreshAll );
    if ((iTestSorter->iSortItems) || (iTestSorter1->iSortItems))
        {
       ret = KErrUnknown;
        }

    iTestSorter->iSortItems = EFalse;
    iTestSorter1->iSortItems = EFalse;

    iListModel->SetCustomSorter( iTestSorter );
    iListModel->RefreshL( ECLFRefreshAll );
    if (!(iTestSorter->iSortItems) || (iTestSorter1->iSortItems))
        {
        ret = KErrUnknown;
        }

    iTestSorter->iSortItems = EFalse;
    iTestSorter1->iSortItems = EFalse;

    iListModel->SetCustomSorter( iTestSorter1 );
    iListModel->RefreshL( ECLFRefreshAll );
    if ((iTestSorter->iSortItems) || !(iTestSorter1->iSortItems))
        {
        ret = KErrUnknown;
        }

// parameter test
    iTestSorter->iSortItems = EFalse;
    iTestSorter1->iSortItems = EFalse;

    iListModel->SetCustomSorter( NULL );
    iListModel->RefreshL( ECLFRefreshAll );
    if ((iTestSorter->iSortItems) || (iTestSorter1->iSortItems))
        {
        ret = KErrUnknown;
        }


    iTestSorter->iSortItems = EFalse;
    iTestSorter1->iSortItems = EFalse;

    iListModel->SetCustomSorter( iTestSorter );
    iListModel->RefreshL( ECLFRefreshPostFilter );
    if ((iTestSorter->iSortItems) || (iTestSorter1->iSortItems))
        {
        ret = KErrUnknown;
        }

    iListModel->RefreshL( ECLFRefreshGrouping );
    if ((iTestSorter->iSortItems) || (iTestSorter1->iSortItems))
        {
        ret = KErrUnknown;
        }

    iListModel->RefreshL( ECLFRefreshSorting );
    if (!(iTestSorter->iSortItems) || (iTestSorter1->iSortItems))
        {
        ret = KErrUnknown;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::GroupingTestL
// ---------------------------------------------------------------------------
//
TInt CContentListingFrameworkTest::GroupingTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    iTestGrouper->iModifiableItems = &iModifiableItems;
    iTestGrouper1->iModifiableItems = &iModifiableItems;

// No grouping
    iListModel->SetCustomGrouper( NULL );
    iListModel->RefreshL( ECLFRefreshAll );
    if (iListModel->ItemCount() != iItemCount)
        {
        ret = KErrUnknown;
        }

// couple groups
    iListModel->SetCustomGrouper( iTestGrouper );
    iListModel->RefreshL( ECLFRefreshAll );
    if (iListModel->ItemCount() != iModifiableItems.Count())
        {
        ret = KErrUnknown;
        }

// 0 groups
    iTestGrouper1->iGroupCount = 0;
    iListModel->SetCustomGrouper( iTestGrouper1 );
    iListModel->RefreshL( ECLFRefreshAll );
    if (iListModel->ItemCount() != iModifiableItems.Count())
        {
        ret = KErrUnknown;
        }

// No grouping
    iListModel->SetCustomGrouper( NULL );
    iListModel->RefreshL( ECLFRefreshAll );
    if (iListModel->ItemCount() != iItemCount)
        {
        ret = KErrUnknown;
        }
// Music album grouping
    iListModel->SetGroupingStyle( ECLFMusicAlbumGrouping );
    iListModel->RefreshL( ECLFRefreshAll );
    if (iListModel->ItemCount() == iItemCount)
        {
        ret = KErrUnknown;
        }
// No grouping
    iListModel->SetGroupingStyle( ECLFNoGrouping );
    iListModel->RefreshL( ECLFRefreshAll );
    if (iListModel->ItemCount() != iItemCount)
        {
        ret = KErrUnknown;
        }

// test parameters
    iTestGrouper->iGroupCount = 1000;
    iListModel->SetCustomGrouper( iTestGrouper );
    iListModel->RefreshL( ECLFRefreshPostFilter );
    if (iListModel->ItemCount() != iItemCount)
        {
        ret = KErrUnknown;
        }
    iListModel->RefreshL( ECLFRefreshSorting );
    if (iListModel->ItemCount() != iItemCount)
        {
        ret = KErrUnknown;
        }
    iListModel->RefreshL( ECLFRefreshGrouping );
    if (iListModel->ItemCount() != iModifiableItems.Count())
        {
        ret = KErrUnknown;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::SetPostFilterTestL
// ---------------------------------------------------------------------------
//
TInt CContentListingFrameworkTest::SetPostFilterTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

// no filter
    iListModel->SetPostFilter( NULL );
    iListModel->RefreshL( ECLFRefreshAll );
    if (iListModel->ItemCount() != iItemCount)
        {
        ret = KErrUnknown;
        }

// filter couple items
    iListModel->SetPostFilter( iTestFilter );
    iListModel->RefreshL( ECLFRefreshAll );
    if ( ( iListModel->ItemCount() + iTestFilter1->iFilteredCount ) == iItemCount )
        {
        ret = KErrUnknown;
        }

// filter all items
    iListModel->SetPostFilter( iTestFilter1 );
    iTestFilter1->iAllFilter = ETrue;
    iListModel->RefreshL( ECLFRefreshAll );
    if ( ( iListModel->ItemCount() + iTestFilter1->iFilteredCount ) != iItemCount )
        {
        ret = KErrUnknown;
        }

// no filter
    iListModel->SetPostFilter( NULL );
    iListModel->RefreshL( ECLFRefreshAll );
    if (iListModel->ItemCount() != iItemCount)
        {
        ret = KErrUnknown;
        }

// filter one item
    iListModel->SetPostFilter( iTestFilter );
    iTestFilter->iShouldFilterCount = 1;
    iListModel->RefreshL( ECLFRefreshAll );
    if ( ( iListModel->ItemCount() + iTestFilter1->iFilteredCount ) == iItemCount )
        {
        ret = KErrUnknown;
        }

// filter couple items
    iListModel->SetPostFilter( iTestFilter1 );
    iTestFilter1->iAllFilter = EFalse;
    iListModel->RefreshL( ECLFRefreshAll );
    if ( ( iListModel->ItemCount() + iTestFilter1->iFilteredCount ) != iItemCount )
        {
        ret = KErrUnknown;
        }

// test parameters
    iListModel->SetPostFilter( NULL );
    iListModel->RefreshL( ECLFRefreshAll );
    if (iListModel->ItemCount() != iItemCount)
        {
        ret = KErrUnknown;
        }
    iListModel->SetPostFilter( iTestFilter1 );
    iTestFilter1->iAllFilter = ETrue;
    iListModel->RefreshL( ECLFRefreshSorting );
    if (iListModel->ItemCount() != iItemCount)
        {
       ret = KErrUnknown;
        }
    iListModel->RefreshL( ECLFRefreshGrouping );
    if (iListModel->ItemCount() != iItemCount)
        {
        ret = KErrUnknown;
        }
    iListModel->RefreshL( ECLFRefreshPostFilter );
    if ( ( iListModel->ItemCount() + iTestFilter1->iFilteredCount ) != iItemCount )
        {
        ret = KErrUnknown;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::SetWantedMimeTypesTestL
// ---------------------------------------------------------------------------
//
TInt CContentListingFrameworkTest::SetWantedMimeTypesTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    iTestObserver->iWait = &iWait;

// list not defined (mimetype)
    iListModel->RefreshL();
    iWait.Start();
    if (iListModel->ItemCount() != 0)
        {
        ret = KErrUnknown;
        }

// couple mime types
    iMimeTypeArray->Reset();
    iMimeTypeArray->AppendL( _L( "audio/mpeg" ) );
    iMimeTypeArray->AppendL( _L( "audio/aac" ) );
    iMimeTypeArray->AppendL( _L( "audio/mp3" ) );
    iMimeTypeArray->AppendL( _L( "audio/x-mp3" ) );
    iMimeTypeArray->AppendL( _L( "audio/mp4" ) );
    iMimeTypeArray->AppendL( _L( "audio/3gpp" ) );
    iMimeTypeArray->AppendL( _L( "audio/m4a" ) );
    iMimeTypeArray->AppendL( _L( "audio/3gpp2" ) );
    iMimeTypeArray->AppendL( _L( "audio/mpeg4") );
    iListModel->SetWantedMimeTypesL( *iMimeTypeArray );
    iListModel->RefreshL();
    iWait.Start();
    if ( !CheckFileTypesL( *iMimeTypeArray, iMediaTypeArray.Array() ) )
        {
        ret = KErrUnknown;
        }

// unsupported mimetype
    iMimeTypeArray1->Reset();
    iMimeTypeArray1->AppendL( _L("__not_supported__")  );
    iListModel->SetWantedMimeTypesL( *iMimeTypeArray1 );
    iListModel->RefreshL();
    iWait.Start();
    if (iListModel->ItemCount() != 0)
        {
        ret = KErrUnknown;
        }

// empty mimetype list
    iMimeTypeArray1->Reset();
    iListModel->SetWantedMimeTypesL( *iMimeTypeArray1 );
    iListModel->RefreshL();
    iWait.Start();
    if (iListModel->ItemCount() != 0)
        {
        ret = KErrUnknown;
        }

// from resource (mimetype)
    iMimeTypeArray->Reset();
    iMimeTypeArray->AppendL( _L("image/*")  );
    iMimeTypeArray->AppendL( _L("audio/*")  );

    ResourceL( R_MIME_TYPE_ARRAY );
    iListModel->SetWantedMimeTypesL( iResourceReader );
    iListModel->RefreshL();
    iWait.Start();
    if ( !CheckFileTypesL( *iMimeTypeArray, iMediaTypeArray.Array() ) )
        {
        ret = KErrUnknown;
        }

    ResourceL( R_MIME_TYPE_ARRAY_EMPTY );
    iListModel->SetWantedMimeTypesL( iResourceReader );
    iListModel->RefreshL();
    iWait.Start();
    if (iListModel->ItemCount() != 0)
        {
        ret = KErrUnknown;
        }

// incorrect resource version
    ResourceL( R_MIME_TYPE_ARRAY_INCORRECT_VERSION );
    TRAPD(error, iListModel->SetWantedMimeTypesL( iResourceReader ));
    if (error != KErrNotSupported)
        {
        ret = KErrUnknown;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::SetWantedMediaTypesTestL
// ---------------------------------------------------------------------------
//
TInt CContentListingFrameworkTest::SetWantedMediaTypesTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    iTestObserver->iWait = &iWait;

// list not defined (mediatype)
    iListModel->RefreshL();
    iWait.Start();
    if (iListModel->ItemCount() != 0)
        {
        ret = KErrUnknown;
        }

// couple media types
    iMediaTypeArray.AppendL( ECLFMediaTypeVideo );
    iMediaTypeArray.AppendL( ECLFMediaTypeImage );
    iMediaTypeArray.AppendL( ECLFMediaTypeSound );
    iMediaTypeArray.AppendL( ECLFMediaTypeMusic );
    iMediaTypeArray.AppendL( ECLFMediaTypeStreamingURL );
    iMediaTypeArray.AppendL( ECLFMediaTypePlaylist );
    iMediaTypeArray.AppendL( TCLFMediaType( ECLFMediaTypeCollection ) );

    iListModel->SetWantedMediaTypesL( iMediaTypeArray.Array() );
    iListModel->RefreshL();
    iWait.Start();
    if ( !CheckFileTypesL( *iMimeTypeArray, iMediaTypeArray.Array() ) )
        {
        ret = KErrUnknown;
        }

// empty media type list list
    iListModel->SetWantedMediaTypesL( iMediaTypeArray1.Array() );
    iListModel->RefreshL();
    iWait.Start();
    if (iListModel->ItemCount() != 0)
        {
        ret = KErrUnknown;
        }

// media type list from resource
    iMediaTypeArray.Reset();
    iMediaTypeArray.AppendL( ECLFMediaTypeImage );
    iMediaTypeArray.AppendL( TCLFMediaType( ECLFMediaTypeCollection ) );
    ResourceL( R_MEDIA_TYPE_ARRAY );
    iListModel->SetWantedMediaTypesL( iResourceReader );
    iListModel->RefreshL();
    iWait.Start();
    if ( !CheckFileTypesL( *iMimeTypeArray, iMediaTypeArray.Array() ) )
        {
        ret = KErrUnknown;
        }

// empty media type list from resource
    ResourceL( R_MEDIA_TYPE_ARRAY_EMPTY );
    iListModel->SetWantedMediaTypesL( iResourceReader );
    iListModel->RefreshL();
    iWait.Start();
    if (iListModel->ItemCount() != 0)
        {
        ret = KErrUnknown;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::SetWantedMediaAndMimeTypesTestL
// ---------------------------------------------------------------------------
//
TInt CContentListingFrameworkTest::SetWantedMediaAndMimeTypesTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    iTestObserver->iWait = &iWait;

    iListModel->RefreshL();
    iWait.Start();
    if (iListModel->ItemCount() != 0)
        {
        ret = KErrUnknown;
        }

// couple types
    iMimeTypeArray->AppendL( _L( "audio/mpeg" ) );
    iMimeTypeArray->AppendL( _L( "audio/aac" ) );
    iMimeTypeArray->AppendL( _L( "audio/mp3" ) );
    iMimeTypeArray->AppendL( _L( "audio/x-mp3" ) );
    iMimeTypeArray->AppendL( _L( "audio/mp4" ) );
    iMimeTypeArray->AppendL( _L( "audio/3gpp" ) );
    iMimeTypeArray->AppendL( _L( "audio/m4a" ) );
    iMimeTypeArray->AppendL( _L( "audio/3gpp2" ) );
    iMimeTypeArray->AppendL( _L( "audio/mpeg4") );
    iListModel->SetWantedMimeTypesL( *iMimeTypeArray );

    iMediaTypeArray.AppendL( ECLFMediaTypeVideo );
    iListModel->SetWantedMediaTypesL( iMediaTypeArray.Array() );
    iListModel->RefreshL();
    iWait.Start();
    if ( !CheckFileTypesL( *iMimeTypeArray, iMediaTypeArray.Array() ) )
        {
        ret = KErrUnknown;
        }

// refresh again
    iListModel->RefreshL();
    iWait.Start();
    if ( !CheckFileTypesL( *iMimeTypeArray, iMediaTypeArray.Array() ) )
        {
        ret = KErrUnknown;
        }

// empty lists
    iMediaTypeArray.Reset();
    iMimeTypeArray->Reset();
    iListModel->SetWantedMediaTypesL( iMediaTypeArray.Array() );
    iListModel->SetWantedMimeTypesL( *iMimeTypeArray );
    iListModel->RefreshL();
    iWait.Start();
    if (iListModel->ItemCount() != 0)
        {
        ret = KErrUnknown;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::MultibleSortingTestL
// ---------------------------------------------------------------------------
//
TInt CContentListingFrameworkTest::MultibleSortingTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    iListModel->RefreshL( ECLFRefreshAll );
    if ( !CheckMultibleSortingShortingL() )
        {
        ret = KErrUnknown;
        }

// resort

    iSortingStyle->ResetL();
    iSortingStyle->SetSortingDataType( ECLFItemDataTypeTInt32 );
    iSortingStyle->AddFieldL( KMultibleSortingTestField6 );
    iSortingStyle->AddFieldL( KMultibleSortingTestField5 );
    iSortingStyle->SetUndefinedItemPosition( ECLFSortingStyleUndefinedEnd );
    iListModel->SetSortingStyle( iSortingStyle );
    iListModel->RefreshL( ECLFRefreshAll );
    if ( CheckMultibleSortingShortingL() )
        {
        ret = KErrUnknown;
        }

    return ret;
    }

/*
* Test model item(s) obsolate functionality
*/

// ---------------------------------------------------------------------------
// ModelItemsChangedTestL
// ---------------------------------------------------------------------------
//
TInt CContentListingFrameworkTest::ModelItemsChangedTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    const TInt newFileNumber( 13 );

// create test files
    CreateNewFileL( 9, iFileName );
    CreateNewFileL( 10, iFileName );
    CreateNewFileL( 11, iFileName );
    CreateNewFileL( newFileNumber, iFileName );
    User::LeaveIfError( iFs.Delete( iFileName ) );

// update server
// to aTInt incorrect test result
    CreateNewFileL( 0, iFileName );
    iEngine->UpdateItemsL();

// create list model with all files
    iMimeTypeArray->Reset();
    iMimeTypeArray->AppendL( _L("*") );
    iListModel->SetWantedMimeTypesL( *iMimeTypeArray );

    iTestObserver->iWait = &iWait;
    iTestObserver->iError = 100;
    iListModel->RefreshL();
    iWait.Start();  // wait until model is refreshed

    if ( iTestObserver->iOperationEvent != ECLFRefreshComplete
         || iTestObserver->iError != KErrNone
         || iListModel->ItemCount() != 0 )
        {
        ret = KErrUnknown;
        }

    const TInt listModelItemCount( iListModel->ItemCount() );
    const TCLFItemId testId( FindTestFileIdL( 0 ) );

// test with modified item
    CreateNewFileL( 0, iFileName );
    iEngine->UpdateItemsL();
    iWait.Start(); // wait until model outdated event is received

    if ( iTestObserver->iOperationEvent != ECLFModelOutdated
         || iTestObserver->iError != KErrNone
         || iListModel->ItemCount() != listModelItemCount
         || FindItem( *iListModel, testId ) == NULL )
        {
        ret = KErrUnknown;
        }

    iTestObserver->iError = 100;
    iListModel->RefreshL();
    iWait.Start();  // wait until model is refreshed
    if ( iTestObserver->iOperationEvent != ECLFRefreshComplete
         || iTestObserver->iError != KErrNone
         || iListModel->ItemCount() != listModelItemCount
         || FindItem( *iListModel, testId ) == NULL )
        {
        ret = KErrUnknown;
        }

// test with new item
    CreateNewFileL( newFileNumber, iFileName );
    iEngine->UpdateItemsL();
    iWait.Start(); // wait until model outdated event is received

    if ( iTestObserver->iOperationEvent != ECLFModelOutdated
         || iTestObserver->iError != KErrNone
         || iListModel->ItemCount() != listModelItemCount )
        {
        ret = KErrUnknown;
        }

    iTestObserver->iError = 100;
    iListModel->RefreshL();
    iWait.Start();  // wait until model is refreshed
    if ( iTestObserver->iOperationEvent != ECLFRefreshComplete
         || iTestObserver->iError != KErrNone
         || iListModel->ItemCount() != listModelItemCount + 1 )
        {
        ret = KErrUnknown;
        }

// delete file
    User::LeaveIfError( iFs.Delete( iFileName ) );
    iEngine->UpdateItemsL();
    iWait.Start(); // wait until model outdated event is received

    if ( iTestObserver->iOperationEvent != ECLFModelOutdated
         || iTestObserver->iError != KErrNone
         || iListModel->ItemCount() != listModelItemCount + 1 )
        {
        ret = KErrUnknown;
        }

    iTestObserver->iError = 100;
    iListModel->RefreshL();
    iWait.Start();  // wait until model is refreshed
    if ( iTestObserver->iOperationEvent != ECLFRefreshComplete
         || iTestObserver->iError != KErrNone
         || iListModel->ItemCount() != listModelItemCount )
        {
        ret = KErrUnknown;
        }

    return ret;
    }

/**
 * Tests, Modifiable item
 */

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::MIFieldTestL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::MIFieldTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    const TUint KTestField1 = 1;
    const TUint KTestField2 = 112312312;
    const TUint KTestField3 = 13333;
    const TUint KTestFieldIncorrect = 2;

    TTime time( 100 );
    TInt32 integer( 12 );
    TBuf<30> data( _L("data") );
    iModifiableItem->AddFieldL( KTestField1, time );
    iModifiableItem->AddFieldL( KTestField2, integer );
    iModifiableItem->AddFieldL( KTestField3, data );

// data type test
    if ( iModifiableItem->DataType( KTestField1 ) != ECLFItemDataTypeTTime
         || iModifiableItem->DataType( KTestField2 ) != ECLFItemDataTypeTInt32
         || iModifiableItem->DataType( KTestField3 ) != ECLFItemDataTypeDesC
         || iModifiableItem->DataType( KTestFieldIncorrect ) != ECLFItemDataTypeNull
         || iModifiableItem->DataType( ECLFFieldIdNull ) != ECLFItemDataTypeNull )
        {
        ret = KErrUnknown;
        }

// get field
    TTime time1( 0 );
    TInt32 integer1( 0 );
    TPtrC ptr;
    if ( iModifiableItem->GetField( KTestField1, time1 ) != KErrNone
         || time != time1 )
        {
        ret = KErrUnknown;
        }
    if ( iModifiableItem->GetField( KTestField2, integer1 ) != KErrNone
         || integer != integer1 )
        {
        ret = KErrUnknown;
        }
    if ( iModifiableItem->GetField( KTestField3, ptr ) != KErrNone
         || data != ptr )
        {
        ret = KErrUnknown;
        }

// incorrect field id
    if ( iModifiableItem->GetField( KTestFieldIncorrect, ptr ) != KErrNotFound
         || iModifiableItem->GetField( KTestFieldIncorrect, integer1 ) != KErrNotFound
         || iModifiableItem->GetField( KTestFieldIncorrect, time1 ) != KErrNotFound )
        {
        ret = KErrUnknown;
        }

// incorrect field type
    if ( iModifiableItem->GetField( KTestField1, ptr ) != KErrNotFound
         || iModifiableItem->GetField( KTestField3, integer1 ) != KErrNotFound
         || iModifiableItem->GetField( KTestField2, time1 ) != KErrNotFound )
        {
        ret = KErrUnknown;
        }

    if ( iModifiableItem->ItemId() == 0 )
        {
        ret = KErrUnknown;
        }

    return ret;
    }

/**
 * Tests, item
 */

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::ItemFieldTestL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::ItemFieldTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

// data type test
    if ( iItem->DataType( ECLFFieldIdFileDate ) != ECLFItemDataTypeTTime
         || iItem->DataType( ECLFFieldIdFileSize ) != ECLFItemDataTypeTInt32
         || iItem->DataType( ECLFFieldIdFileNameAndPath ) != ECLFItemDataTypeDesC
         || iItem->DataType( ECLFFieldIdNull ) != ECLFItemDataTypeNull )
        {
        ret = KErrUnknown;
        }

// get field
    TTime time1( 0 );
    TInt32 integer1( 0 );
    TPtrC ptr;
    TEntry entry;
    User::LeaveIfError( iFs.Entry( iFileName, entry ) );

    if ( iModifiableItem->GetField( ECLFFieldIdFileDate, time1 ) != KErrNone
         || entry.iModified != time1 )
        {
        ret = KErrUnknown;
        }
    if ( iModifiableItem->GetField( ECLFFieldIdFileSize, integer1 ) != KErrNone
         || entry.iSize != integer1 )
        {
        ret = KErrUnknown;
        }
    if ( iModifiableItem->GetField( ECLFFieldIdFileNameAndPath, ptr ) != KErrNone
         || iFileName != ptr )
        {
        ret = KErrUnknown;
        }

    TParsePtrC parse( iFileName );

    if ( iModifiableItem->GetField( ECLFFieldIdFileExtension, ptr ) != KErrNone
         || parse.Ext() != ptr )
        {
        ret = KErrUnknown;
        }
    if ( iModifiableItem->GetField( ECLFFieldIdFileName, ptr ) != KErrNone
         || parse.Name() != ptr )
        {
        ret = KErrUnknown;
        }
    if ( iModifiableItem->GetField( ECLFFieldIdPath, ptr ) != KErrNone
         || parse.Path() != ptr )
        {
        ret = KErrUnknown;
        }
    if ( iModifiableItem->GetField( ECLFFieldIdDrive, ptr ) != KErrNone
         || parse.Drive() != ptr )
        {
        ret = KErrUnknown;
        }

// incorrect field id
    if ( iItem->GetField( ECLFFieldIdNull, ptr ) != KErrNotFound
         || iItem->GetField( ECLFFieldIdNull, integer1 ) != KErrNotFound
         || iItem->GetField( ECLFFieldIdNull, time1 ) != KErrNotFound )
        {
        ret = KErrUnknown;
        }

// incorrect field type
    if ( iItem->GetField( ECLFFieldIdFileSize, ptr ) != KErrNotSupported
         || iItem->GetField( ECLFFieldIdFileDate, integer1 ) != KErrNotSupported
         || iItem->GetField( ECLFFieldIdFileNameAndPath, time1 ) != KErrNotSupported )
        {
        ret = KErrUnknown;
        }

    if ( iItem->ItemId() == 0 )
        {
        ret = KErrUnknown;
        }

    return ret;
    }

/**
 * Tests, Sorting style
 */

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::SortingStyleResourceTestL
// ---------------------------------------------------------------------------
//  
TInt CContentListingFrameworkTest::SortingStyleResourceTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    const TInt KArray1IdCount( 4 );
    const TInt KArray2IdCount( 1 );


    RArray<TCLFItemId> itemIdArray;
    CleanupClosePushL( itemIdArray );

    if ( iSortingStyle->Ordering() != ECLFOrderingAscending
         || iSortingStyle1->Ordering() != ECLFOrderingDescending
         || iSortingStyle2->Ordering() != ECLFOrderingDescending )
        {
        ret = KErrUnknown;
        }
    if ( iSortingStyle->SortingDataType() != ECLFItemDataTypeTInt32
         || iSortingStyle1->SortingDataType() != ECLFItemDataTypeDesC
         || iSortingStyle2->SortingDataType() != ECLFItemDataTypeDesC )
        {
        ret = KErrUnknown;
        }

    iSortingStyle->GetFieldsL( itemIdArray );

    if ( itemIdArray.Count() != 0 )
        {
        ret = KErrUnknown;
        }

    itemIdArray.Reset();
    iSortingStyle1->GetFieldsL( itemIdArray );

    if ( itemIdArray.Count() != KArray1IdCount )
        {
        ret = KErrUnknown;
        }

    itemIdArray.Reset();
    iSortingStyle2->GetFieldsL( itemIdArray );

    if ( itemIdArray.Count() != KArray2IdCount )
        {
        ret = KErrUnknown;
        }

    CleanupStack::PopAndDestroy( &itemIdArray ); // itemIdArray.Close

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::SortingStyleOrderingTestL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::SortingStyleOrderingTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    iSortingStyle->SetOrdering( ECLFOrderingAscending );
    if ( iSortingStyle->Ordering() != ECLFOrderingAscending )
        {
        ret = KErrUnknown;
        }

    iSortingStyle->SetOrdering( ECLFOrderingDescending );
    if ( iSortingStyle->Ordering() != ECLFOrderingDescending )
        {
        ret = KErrUnknown;
        }

    iSortingStyle->SetOrdering( ECLFOrderingAscending );
    if ( iSortingStyle->Ordering() != ECLFOrderingAscending )
        {
        ret = KErrUnknown;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::SortingStyleDataTypeTestL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::SortingStyleDataTypeTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    iSortingStyle->SetSortingDataType( ECLFItemDataTypeTInt32 );
    if ( iSortingStyle->SortingDataType() != ECLFItemDataTypeTInt32 )
        {
        ret = KErrUnknown;
        }

    iSortingStyle->SetSortingDataType( ECLFItemDataTypeDesC );
    if ( iSortingStyle->SortingDataType() != ECLFItemDataTypeDesC )
        {
        ret = KErrUnknown;
        }

    iSortingStyle->SetSortingDataType( ECLFItemDataTypeTTime );
    if ( iSortingStyle->SortingDataType() != ECLFItemDataTypeTTime )
        {
        ret = KErrUnknown;
        }

    iSortingStyle->SetSortingDataType( ECLFItemDataTypeNull );
    if ( iSortingStyle->SortingDataType() != ECLFItemDataTypeNull )
        {
        ret = KErrUnknown;
        }

    iSortingStyle->SetSortingDataType( ECLFItemDataTypeTInt32 );
    if ( iSortingStyle->SortingDataType() != ECLFItemDataTypeTInt32 )
        {
        ret = KErrUnknown;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::SortingStyleUndefinedItemPositionTestL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::SortingStyleUndefinedItemPositionTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    iSortingStyle->SetUndefinedItemPosition( ECLFSortingStyleUndefinedEnd );
    if ( iSortingStyle->UndefinedItemPosition() != ECLFSortingStyleUndefinedEnd )
        {
        ret = KErrUnknown;
        }

    iSortingStyle->SetUndefinedItemPosition( ECLFSortingStyleUndefinedFirst );
    if ( iSortingStyle->UndefinedItemPosition() != ECLFSortingStyleUndefinedFirst )
        {
        ret = KErrUnknown;
        }

    iSortingStyle->SetUndefinedItemPosition( ECLFSortingStyleUndefinedEnd );
    if ( iSortingStyle->UndefinedItemPosition() != ECLFSortingStyleUndefinedEnd )
        {
        ret = KErrUnknown;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// CContentListingFrameworkTest::SortingStyleFieldTestL
// ---------------------------------------------------------------------------
// 
TInt CContentListingFrameworkTest::SortingStyleFieldTestL( CStifItemParser& /* aItem */ )
    {
    TInt ret = KErrNone;

    RArray<TCLFItemId> itemIdArray;
    CleanupClosePushL( itemIdArray );

    iSortingStyle->GetFieldsL( itemIdArray );
    if ( itemIdArray.Count() != 0 )
        {
        ret = KErrUnknown;
        }

    iSortingStyle->AddFieldL( ECLFFieldIdFileName );
    iSortingStyle->AddFieldL( ECLFFieldIdCollectionId );
    iSortingStyle->AddFieldL( ECLFFieldIdCollectionName );
    iSortingStyle->AddFieldL( ECLFFieldIdArtist );

    iSortingStyle->GetFieldsL( itemIdArray );

    if ( itemIdArray.Count() != 4 )
        {
        ret = KErrUnknown;
        }

    itemIdArray.Reset();
    iSortingStyle->ResetL();
    iSortingStyle->GetFieldsL( itemIdArray );
    if ( itemIdArray.Count() != 0 )
        {
        ret = KErrUnknown;
        }

    CleanupStack::PopAndDestroy( &itemIdArray ); // itemIdArray.Close

    return ret;
    }

//  [End of File] - Do not remove
