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
* Description: Implementation of CPosLmNameIndex class
*
*
*/


#include <e32base.h>
#include <d32dbms.h>
#include <EPos_Landmarks.h>
#include <EPos_CPosLandmark.h>
#include <epos_cposlmlocaldbaccess.h>
#include <epos_landmarkdatabasestructure.h>
#include <epos_poslmlandmarkhandler.h>
#include <epos_cposlmdiskutilities.h>
#include <epos_poslmserverutility.h>
#include <epos_poslmlongtextcolhandler.h>

#include "EPos_PosLmDatabaseManager.h"
#include "epos_lmdebug.h"
#include "EPos_CPosLmNameIndex.h"

// ============================ LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

//--------------------------------------------------------------------
// CLmNameIndex::CIndexItem
//--------------------------------------------------------------------
//
CPosLmNameIndex::CIndexItem::CIndexItem()
: iId ( KPosLmNullItemId )
    {
    }

CPosLmNameIndex::CIndexItem::CIndexItem( TPosLmItemId aId )
: iId ( aId )
    {
    }

CPosLmNameIndex::CIndexItem::CIndexItem( TPosLmItemId aId, HBufC* aName )
: iId ( aId ), iName( aName )
    {
    }

CPosLmNameIndex::CIndexItem::~CIndexItem()
    {
    delete iName;
    }

CPosLmNameIndex::CIndexItem* CPosLmNameIndex::CIndexItem::NewLC( TPosLmItemId aId, const TDesC& aName )
    {
    CIndexItem* self = new (ELeave) CIndexItem( aId );
    CleanupStack::PushL( self );
    self->iName = aName.AllocL();
    return self;
    }

CPosLmNameIndex::CIndexItem* CPosLmNameIndex::CIndexItem::NewLC( RReadStream& aIn )
    {
    TPosLmItemId id = aIn.ReadUint32L();
    TInt len = aIn.ReadUint32L();
    if ( id < 1 || len < 0 ) User::Leave( KErrCorrupt );
    
    HBufC* name = HBufC::NewLC( len );
    TPtr ptr = name->Des();
    aIn.ReadL( ptr, len );
    
    CIndexItem* self = new (ELeave) CIndexItem( id, name );
    CleanupStack::Pop( name );
    
    CleanupStack::PushL( self );
    return self;
    }

TPtrC CPosLmNameIndex::CIndexItem::Name() const
    {
    if ( iName )
        return *iName;
    else
        return KNullDesC();
    }

void CPosLmNameIndex::CIndexItem::ExternalizeL( RWriteStream& aOut ) const
    {
    aOut.WriteUint32L( iId );
    aOut.WriteUint32L( Name().Length() );
    aOut.WriteL( Name() );
    // state is not stored, it is used only for index verification
    }

TInt CPosLmNameIndex::CIndexItem::Size() const
    {
    return 
        sizeof( TUint32 )       // id size
        + sizeof( TUint32 )     // string length size
        + Name().Size();        // string size
    }

TInt CPosLmNameIndex::CIndexItem::CompareById( const CIndexItem& aLeft, const CIndexItem& aRight )
    {
    return aLeft.Id() - aRight.Id();
    }

//--------------------------------------------------------------------
// CPosLmNameIndex::TLmIndexNameKey
//--------------------------------------------------------------------
//
CPosLmNameIndex::TLmIndexNameKey::TLmIndexNameKey() :
    TKeyArrayFix( 0, ECmpCollated )
    {
    // the second parameter - comparison type - is used
    // by TKeyArrayFix::Compare()
    // the first parameter - offset of the key - is not used: 
    // overridden At() returns pointer to key not using offsets
    }

TAny* CPosLmNameIndex::TLmIndexNameKey::At( TInt anIndex ) const
    {
    TAny* any = TKeyArrayFix::At( anIndex );
    CIndexItem** ptr = static_cast<CIndexItem**>( any );
    return (*ptr)->NamePtr();
    }

//--------------------------------------------------------------------
// CPosLmNameIndex::TLmIndexIdKey
//--------------------------------------------------------------------
//
CPosLmNameIndex::TLmIndexIdKey::TLmIndexIdKey() :
    TKeyArrayFix( 0, ECmpTUint32 )
    {
    }

TAny* CPosLmNameIndex::TLmIndexIdKey::At( TInt anIndex ) const
    {
    TAny* any = TKeyArrayFix::At( anIndex );
    CIndexItem** ptr = static_cast<CIndexItem**>( any );
    return (*ptr)->IdPtr();
    }

//--------------------------------------------------------------------
// CPosLmNameIndex
//--------------------------------------------------------------------
//

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
CPosLmNameIndex::CPosLmNameIndex( CPosLmLocalDbAccess& aDbAccess )
:   iDbAccess( aDbAccess ), iStatus( KErrNotReady )
    {
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
CPosLmNameIndex::~CPosLmNameIndex()
    {
    Reset();
    delete iArray;
    delete iKeyItem;
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::Reset()
    {
    StopEvaluation();
    iArray->ResetAndDestroy();
    iDataSize = 0;
    iStatus = KErrNotReady;
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
CPosLmNameIndex* CPosLmNameIndex::NewL( CPosLmLocalDbAccess& aDbAccess )
    {
    CPosLmNameIndex* self = NewLC( aDbAccess );
    CleanupStack::Pop( self );
    return self;
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
CPosLmNameIndex* CPosLmNameIndex::NewLC( CPosLmLocalDbAccess& aDbAccess )
    {   
    CPosLmNameIndex* self = new (ELeave) CPosLmNameIndex( aDbAccess );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::ConstructL()
    {
    const TInt KGranularity = 100;
    iArray = new (ELeave) CArrayPtrSeg<CIndexItem>( KGranularity );
    iKeyItem = new (ELeave) CIndexItem;
    
    iDbAccess.GetDatabase( iDatabase );

    iTimeStamp.UniversalTime();
    TRAPD( err, LoadL() );
    if ( err )
        {
        // need to re-generate index
        iStatus = KErrNotReady;
        }
    else
    	{
    	//if no need to re-generate then set all the landmarks to valid state
    	for(int i =0; i < iArray->Count(); i++)
    	{
    		CIndexItem *item = iArray->At(i);
    		item->SetValid();
    	}
    	iStatus = KErrNone;
    	}
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
TReal32 CPosLmNameIndex::Evaluate()
    {
    TReal32 progress = 1.0;
    if ( iStatus )
        {
        TRAPD( err, DoEvaluateL( progress ) );
        if ( err )
            {
            StopEvaluation();
            iStatus = err;
            }
        else if ( progress == 1.0 ) // evaluation completed
            {
            StopEvaluation();
            iStatus = KErrNone;
            }
        }
    return progress;
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::StopEvaluation()
	{
	if ( iIsTableOpen )
		{
	    iTable.Close();
	    iIsTableOpen = EFalse;
		}
    iIdSortedArray.Reset();
    RemoveInvalidItems();
	}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::DoEvaluateL( TReal32& aProgress )
    {
    const TInt KRowsPerStep( 100 );

    if ( !iIsTableOpen )
    	{
        TInt err = iTable.Open( iDatabase, KPosLmLandmarkTable, RDbRowSet::EReadOnly );
        User::LeaveIfError( err );
        iIsTableOpen = ETrue;

        if ( iTable.FirstL() )
            {
            //   Reset(); 
            // Don't reset what's loaded - try to check the index with
            // contents of the database and update accordingly. 
            // Reason: if changes are small (most probable case) this job 
            // is faster than rebuilding new index from scratch
            CreateByIdSortingL();
            iTablePosition = 0;
            iStatus = KErrNotReady;
            }
        else
            {
            // no records in the table
    	    iTable.Close();
    	    iIsTableOpen = EFalse;
            aProgress = 1.0;
            return;
            }
    	}
    
    TInt numRows = KRowsPerStep;
    while( !iTable.AtEnd() && numRows-- > 0 )
        {
        iTable.GetL();

        TPosLmItemId id = iTable.ColUint32( EPosLmLcLandmarkIdCol );

        HBufC* name = PosLmServerUtility::ReadFromLongTextColumnLC( 
        	iTable, EPosLmLcNameCol );

        // If the item already exists in the index, mark it as verified
        // otherwise - add to index
        CIndexItem* item = ItemById( id );
        // when comparing contents of the index with database
        // it is ok to use binary comparison 
        if ( !item || item->Name().Compare( *name ) != 0 ) 
            {
            // the ID has been removed or changed in database
            if ( item )
                {
                // the original item has been changed
                item->SetInvalid();
                }
			
			CleanupStack::Pop( name );//ownership of name is transferred in the call to DoInsertL	
				
            // add new item for this ID
            // here comparison must be done using collation
            CIndexItem* newItem = DoInsertL( id, name ); // takes onwership of name
            newItem->SetValid();
            
            TLinearOrder<CIndexItem> order( CIndexItem::CompareById );
            iIdSortedArray.InsertInOrder( newItem, order );
            }
        else
            {
            // this item already exists in the index
            item->SetValid();
            CleanupStack::PopAndDestroy( name );
            }
				    //coverity[check_return :FALSE]
    //coverity[unchecked_value :FALSE]
        iTable.NextL();
        iTablePosition++;
        }
    
    // return progress;
    TInt count = iTable.CountL();
    if ( count > 0 )
        {
        aProgress = TReal32( iTablePosition ) / TReal32( count );
        }
    else
        {
        aProgress = 1.0;
        }
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
CArrayFixSeg<TPosLmItemId>* CPosLmNameIndex::GetSortedIdsLC() const
    {
    return GetSortedIdsLC( 0, Count() );
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
CArrayFixSeg<TPosLmItemId>* CPosLmNameIndex::GetSortedIdsLC( TInt aFirst, TInt aCount ) const
    {
    if ( iStatus == KErrNone )
        {
        const TInt KGranularity( 512 );
        CArrayFixSeg<TPosLmItemId>* array = new (ELeave) CArrayFixSeg<TPosLmItemId>( KGranularity );
        CleanupStack::PushL( array );
        TInt first = Max( 0, aFirst );
        TInt last = Min( first + aCount, iArray->Count() );
        for ( TInt i = first; i < last; i++ )
            {
            CIndexItem* item = iArray->At(i);
            array->AppendL( item->Id() );
            }
        return array;
        }
    else
        {
        User::Leave( iStatus );
        return NULL;
        }
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
TInt CPosLmNameIndex::GetSortedIds( 
    TInt aFirst, TInt aCount, 
    TPosLmItemId* aTarget, TInt& aReminder ) const
    {
    if ( aFirst < 0 || aCount < 0 || aTarget == NULL )
        return KErrArgument;
    
    if ( iStatus == KErrNone )
        {
        TInt first = Min( Max( aFirst, 0 ), iArray->Count() );
        TInt last = Max( 0, Min( aFirst + aCount, iArray->Count() ) );
        for ( TInt i = first; i < last; i++ )
            {
            CIndexItem* item = iArray->At( i );
            *(aTarget + i-first ) = item->Id();
            }
        aReminder = Count() - last;
        return last - first;
        }
    else
        {
        return iStatus;
        }
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::InsertL( const CPosLandmark& aLandmark )
    {
    TPtrC name;
    aLandmark.GetLandmarkName( name );
    InsertL( aLandmark.LandmarkId(), name );
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::InsertL( TPosLmItemId aLmid, const TDesC& aName )
    {
    HBufC* name = aName.AllocLC();
    CleanupStack::Pop( name );
    InsertL( aLmid, name );
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::InsertL( TPosLmItemId aLmid, HBufC* aName )
    {
	//coverity[ alloc_fn : FALSE ]
   DoInsertL( aLmid, aName );

    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
CPosLmNameIndex::CIndexItem* CPosLmNameIndex::DoInsertL( TPosLmItemId aLmid, HBufC* aName )
    {
	CleanupStack::PushL( aName );//ownership of aName is transferred to this class
    if ( Find( aLmid ) >= 0 ) // finds valid item
        {
        User::Leave( KErrAlreadyExists ); // duplicate ID found
        }
    
    CIndexItem* landmark = new (ELeave) CIndexItem( aLmid, aName ); // takes ownership of aName
    CleanupStack::Pop( aName );
	CleanupStack::PushL( landmark );
    
    TLmIndexNameKey key;
    iArray->InsertIsqAllowDuplicatesL( landmark, key );

    CleanupStack::Pop( landmark );
    iDataSize += landmark->Size();
    
    if ( !iInTransaction )
        {
        landmark->SetValid();
        }
    // otherwise item remains in Temp state
    
    return landmark;
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::AppendL( TPosLmItemId aLmid, HBufC* aName )
    {
	//Ownership of aName is transferred in this call
	CleanupStack::PushL( aName );
    CIndexItem* landmark = new (ELeave) CIndexItem( aLmid, aName );
    CleanupStack::Pop( aName );

    CleanupStack::PushL( landmark );
    AppendL( landmark );
    CleanupStack::Pop( landmark );
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::AppendL( CIndexItem* aLandmark )
    {
    ASSERT( aLandmark );
    iArray->AppendL( aLandmark );
    iDataSize += aLandmark->Size();
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::Remove( TPosLmItemId aLmid )
    {
    TInt index = Find( aLmid );
    if ( index >= 0 )
        {
        DoRemove( index );
        }
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::DoRemove( TInt aIndex )
    {
    ASSERT( aIndex >= 0 && aIndex < iArray->Count() );

    CIndexItem* entry = iArray->At( aIndex );
    if ( !iInTransaction )
        {
        iDataSize -= entry->Size();
        ASSERT( iDataSize >= 0 );
        iArray->Delete( aIndex );
		delete entry;
        }
    else
        {
        entry->SetInvalid();
        }
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::UpdateL( TPosLmItemId aId, const TDesC& aName )
    {
    HBufC* name = aName.AllocLC();
	CleanupStack::Pop( name );//ownership of name is transferred in the call to UpdateL
//coverity[freed_arg : FALSE]
    UpdateL( aId, name );
//coverity[pass_freed_arg : FALSE]
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::UpdateL( TPosLmItemId aLmid, HBufC* aName )
    {
    TInt index = Find( aLmid );
    if ( index >= 0 )
        {
        CIndexItem* entry = iArray->At( index );
        // if name is exactly same, no need to change anything
        if ( entry->Name().Compare( *aName ) != 0 )
            {
            DoRemove( index );
            InsertL( aLmid, aName );
            }
        else
            {
            delete aName;
            }
        }
    else
        {
        delete aName;
        }
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
TInt CPosLmNameIndex::Find( TPosLmItemId aLmid ) const
    {
    for ( TInt i = 0; i < iArray->Count(); i++ )
        {
        CIndexItem* entry = iArray->At(i);
        if ( entry->Id() == aLmid && entry->IsValid() )
            {
            return i;
            }
        }
    return KErrNotFound;
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
TInt CPosLmNameIndex::Count() const
    {
    return iArray->Count();
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
TInt CPosLmNameIndex::DataSize() const
    {
    return 
        sizeof( TUint32 )       // total amount of items
        + iDataSize;
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
const CPosLmNameIndex::CItem& CPosLmNameIndex::Item( TInt aIndex ) const
    {
    return *( iArray->At( aIndex ) );
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::ExternalizeL( RWriteStream& aOut ) const
    {
    aOut.WriteInt32L( iArray->Count() );
    for ( TInt i = 0; i < iArray->Count(); i++ ) 
        {
        CIndexItem* item = iArray->At( i );
        item->ExternalizeL( aOut );
        }
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::InternalizeL( RReadStream& aIn )
    {
    Reset();
    TInt32 count = aIn.ReadInt32L();
    if ( count < 0 ) User::Leave( KErrCorrupt );
    for ( TInt i = 0; i < count; i++ ) 
        {
        CIndexItem* item = CIndexItem::NewLC( aIn );
		CleanupStack::Pop( item ); //Ownership of item is transferred in the AppendL call
        AppendL( item ); // array takes ownership
        }
    iStatus = KErrNone;
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::LoadL()
    {
    // Skip index loading for empty databases
    TInt numLandmarks = PosLmServerUtility::TotalLandmarkCountL( iDbAccess );
    if ( numLandmarks == 0 )
        {
        iTimeStamp.UniversalTime();
        iStatus = KErrNone;
        return;
        }
    
    RDbTable table;
    TInt err = table.Open( iDatabase, KPosLmIndexTable, RDbRowSet::EReadOnly );
    if ( err )
        {
        LOG("NameIndex::LoadL: index table not found"); 
        User::Leave( err );
        }
    CleanupClosePushL( table );

    table.FirstL();
    if ( table.AtEnd() )
        {
        LOG("NameIndex::LoadL: index not found"); 
        User::Leave( KErrNotFound  );
        }

    table.GetL();

    // verify that index is valid for current language
    if ( !table.IsColNull( EPosLmIncLanguageCol ) )
        {
        TLanguage lang = (TLanguage) table.ColInt32( EPosLmIncLanguageCol );
        if ( User::Language() != lang ) 
            {
            LOG2("NameIndex::LoadL: index not valid, lang %d, current lang %d", 
                lang, User::Language() );
            User::Leave( KErrCorrupt );
            }
        }
    else
        {
        LOG("NameIndex::LoadL: index lang not found"); 
        User::Leave( KErrCorrupt );
        }

    // read the index
    if ( !table.IsColNull( EPosLmIncIndexDataCol ) )
        {
        RDbColReadStream readStream;
        readStream.OpenL( table, EPosLmIncIndexDataCol );
        CleanupClosePushL( readStream );
        InternalizeL( readStream );
        CleanupStack::PopAndDestroy( &readStream );

        // basic check for the index
        if ( Count() != numLandmarks )
            {
            LOG2("NameIndex::LoadL: index not valid, count %d, landmarks in db %d", 
                Count(), numLandmarks );
            User::Leave( KErrCorrupt );
            }
        }
    else
        {
        LOG("NameIndex::LoadL: index data not found"); 
        User::Leave( KErrCorrupt );
        }

    // read the time stamp
    iTimeStamp.UniversalTime();
    if ( !table.IsColNull( EPosLmIncTimestampCol ) )
        {
        iTimeStamp = table.ColTime( EPosLmIncTimestampCol );
        }
    else
        {
        LOG("NameIndex::LoadL: index timestamp not found"); 
        User::Leave( KErrCorrupt );
        }

    CleanupStack::PopAndDestroy ( &table );

    // index is valid
    iStatus = KErrNone;
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::SaveL( TChar aDrive )
    {
    RDbTable table;
    TInt err = table.Open( iDatabase, KPosLmIndexTable, RDbRowSet::EUpdatable );
    if ( err == KErrNotFound )
        {
        PosLmDatabaseManager::CreateIndexTableL( iDatabase );
        err = table.Open( iDatabase, KPosLmIndexTable, RDbRowSet::EUpdatable );
        }
    User::LeaveIfError( err );
    CleanupClosePushL( table );

    TInt currentSize = 0;
    table.FirstL();
    if ( table.AtEnd() )
        {
        table.InsertL();
        }
    else
        {
        table.GetL();
        currentSize = table.ColSize( EPosLmIncIndexDataCol ); 
        table.UpdateL();
        }
    
    if ( currentSize < DataSize() )
        {
        // check disk size
        CPosLmDiskUtilities* diskUtilities = CPosLmDiskUtilities::NewL();
        CleanupStack::PushL( diskUtilities );
    
        TInt bytesToWrite = DataSize() - currentSize;
        diskUtilities->DiskSpaceBelowCriticalLevelL( bytesToWrite, aDrive );
    
        CleanupStack::PopAndDestroy( diskUtilities );
        }
    
    // current language
    table.SetColL( EPosLmIncLanguageCol, User::Language() );
    
    // index data
    RDbColWriteStream writeStream;
    writeStream.OpenL( table, EPosLmIncIndexDataCol );
    CleanupClosePushL( writeStream );
    ExternalizeL( writeStream );
    CleanupStack::PopAndDestroy( &writeStream );
    
    // index timestamp
    TTime now;
    now.UniversalTime();
    table.SetColL( EPosLmIncTimestampCol, now );

#ifdef _DEBUG  
    TBuf<64> mtime;
    now.FormatL( mtime, KPosLmTimeFormat );
    LOG1( "NameIndex: Saving index timestamp %S", &mtime); 
#endif    

    table.PutL();
    CleanupStack::PopAndDestroy ( &table );
    
    iTimeStamp = now;
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
CPosLmNameIndex::CIndexItem* CPosLmNameIndex::ItemById( TPosLmItemId aId ) const
    {
    iKeyItem->SetId( aId );

    TLinearOrder<CIndexItem> order( CIndexItem::CompareById );
    TInt index = iIdSortedArray.FindInOrder( iKeyItem, order );
    if ( index >=0 )
        return iIdSortedArray[index];
    else
        return NULL;
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::CreateByIdSortingL()
    {
    iIdSortedArray.Reset();
    iIdSortedArray.ReserveL( iArray->Count() );
    
    for ( TInt i = 0; i < iArray->Count(); i++ ) 
        {
        CIndexItem* item = iArray->At( i );
        TLinearOrder<CIndexItem> order( CIndexItem::CompareById );
        iIdSortedArray.InsertInOrder( item, order );
        }
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::RemoveInvalidItems()
    {
    for ( TInt i = iArray->Count() - 1; i >= 0 ; i-- ) 
        {
        CIndexItem* item = iArray->At( i );
        if ( !item->IsValid() )
            {
            iArray->Delete( i );
			delete item;
            }
        }
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::StartTransaction()
    {
    ASSERT( !iInTransaction );
    ASSERT( iStatus == KErrNone );
    iInTransaction = ETrue;
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::CommitTransaction()
    {
    ASSERT( iInTransaction );
    ASSERT( iStatus == KErrNone );
    // delete items marked as deleted
    // and validate items marked as temporary
    for ( TInt i = iArray->Count() - 1; i >= 0 ; i-- ) 
        {
        CIndexItem* item = iArray->At( i );
        if ( item->IsTemp() )
            {
            item->SetValid();
            }
        else if ( !item->IsValid() )
            {
            iArray->Delete( i );
			delete item;
            }
        }
    iInTransaction = EFalse;
    }

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//
void CPosLmNameIndex::RollbackTransaction()
    {
    ASSERT( iInTransaction );
    ASSERT( iStatus == KErrNone );
    // delete temp items
    // and validate items marked as invalid
    for ( TInt i = iArray->Count() - 1; i >= 0 ; i-- ) 
        {
        CIndexItem* item = iArray->At( i );
        if ( item->IsTemp() )
            {
            iArray->Delete( i );
			delete item;
            }
        else if ( !item->IsValid() )
            {
            item->SetValid();
            }
        }
    iInTransaction = EFalse;
    }
