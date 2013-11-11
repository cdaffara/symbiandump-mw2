/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Utility class to central repository database.
*
*/


#include <centralrepository.h>

#include "cenrepdatabaseutil.h"
#include "cenrepdatabaselogger.h"


const TInt KNoEntryId = 0; // Not allowed ID for entries. Deleted rows
                           // are marked with KNoEntryId

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CCenRepDatabaseUtil::CCenRepDatabaseUtil( TUint32 aStartKey,
                                      TUint32 aColIncrement,
                                      TUint32 aColMask,
                                      TUint32 aIdCounterKey,
                                      TInt aColCount )
    : iStartKey( aStartKey ),
      iColIncrement( aColIncrement ),
      iColMask( aColMask ),
      iIdCounterKey( aIdCounterKey ),
      iColCount( aColCount )
    {
    }
    
// ---------------------------------------------------------------------------
// 2nd phase constructor
// ---------------------------------------------------------------------------
//
void CCenRepDatabaseUtil::ConstructL( TUid aUid )
    {
    if( iColIncrement - 1 + iColMask != KMaxTUint )
        {
        User::Leave( KErrArgument );
        }
        
    iRepository = CRepository::NewL( aUid );

    // Create semaphore name from repository uid
    TUidName uidName = aUid.Name();
    TInt err = iSemaphore.OpenGlobal( uidName );
    if ( err != KErrNone )
        {
        User::LeaveIfError( iSemaphore.CreateGlobal( uidName, 1 ) );
        }
    
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CCenRepDatabaseUtil* CCenRepDatabaseUtil::NewL( TUid aUid, 
                                                     TUint32 aStartKey, 
                                                     TUint32 aColIncrement, 
                                                     TUint32 aColMask,
                                                     TUint32 aIdCounterKey,
                                                     TInt aColCount )
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::NewL - IN" );
    
    CCenRepDatabaseUtil* self = CCenRepDatabaseUtil::NewLC( 
                                                        aUid, 
                                                        aStartKey, 
                                                        aColIncrement, 
                                                        aColMask, 
                                                        aIdCounterKey, 
                                                        aColCount );
    CleanupStack::Pop( self );
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::NewL - OUT" );
    
    return self;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CCenRepDatabaseUtil* CCenRepDatabaseUtil::NewLC( TUid aUid, 
                                                      TUint32 aStartKey, 
                                                      TUint32 aColIncrement, 
                                                      TUint32 aColMask,
                                                      TUint32 aIdCounterKey,
                                                      TInt aColCount )
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::NewLC - IN" );
    
    CCenRepDatabaseUtil* self = new( ELeave ) CCenRepDatabaseUtil( 
                                                                aStartKey,
                                                                aColIncrement,
                                                                aColMask,
                                                                aIdCounterKey,
                                                                aColCount );
    CleanupStack::PushL( self );
    self->ConstructL( aUid );
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::NewLC - OUT" );
    
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CCenRepDatabaseUtil::~CCenRepDatabaseUtil()
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::~CCenRepDatabaseUtil - IN" );
    
    iSemaphore.Close();
    delete iRepository;
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::~CCenRepDatabaseUtil - OUT" );
    }

// ---------------------------------------------------------------------------
// Begins transaction in repository.
// ---------------------------------------------------------------------------
//
EXPORT_C void CCenRepDatabaseUtil::BeginTransactionL()
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::BeginTransactionL - IN" );
    
    iSemaphore.Wait();
    TCleanupItem cleanup( CCenRepDatabaseUtil::ReleaseSemaphore, this );
    CleanupStack::PushL( cleanup );
    
    User::LeaveIfError( 
        iRepository->StartTransaction( CRepository::EReadWriteTransaction ) );
    
    iRepository->CleanupRollbackTransactionPushL(); // if leave happens,
                                                    // only roll back, no delete
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::BeginTransactionL - OUT" );                                                
    }

// ---------------------------------------------------------------------------
// Commits changes in repository.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCenRepDatabaseUtil::CommitTransaction()
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::CommitTransaction - IN" );
    
    TUint32 temp;
    
    TInt retval = iRepository->CommitTransaction( temp );
    
    iSemaphore.Signal();

    CleanupStack::Pop( 2 ); // semaphore and repository
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::CommitTransaction - OUT" );

    return retval;
    }

// ---------------------------------------------------------------------------
// Rollback changes in repository.
// ---------------------------------------------------------------------------
//
EXPORT_C void CCenRepDatabaseUtil::RollbackTransaction()
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::RollbackTransaction - IN" );
    
    CleanupStack::Pop( 2 ); // rollback transaction item and semaphore

    iRepository->RollbackTransaction();
    iSemaphore.Signal();
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::RollbackTransaction - OUT" );
    }
    
// ---------------------------------------------------------------------------
// Add new entry to cenrep.
// ---------------------------------------------------------------------------
//
EXPORT_C void CCenRepDatabaseUtil::AddEntryL( 
    TInt& aEntryId,
    const RIpAppPropArray& aArray )
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::AddEntryL - IN" );
    
    CreateEntryIdL( aEntryId );
    TUint32 idKey = GetNewIdKeyL();
    
    // Add entry Id
    SetOrCreateKeyL( idKey, aEntryId );
    
    UpdatePropertiesL( idKey, aArray );
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::AddEntryL - OUT" );
    }

// ---------------------------------------------------------------------------
// Find entry by ID.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCenRepDatabaseUtil::FindEntryL( 
    TInt aEntryId,
    RIpAppPropArray& aArray )
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::FindEntryL - IN" );
    
    RKeyArray keys;
    CleanupClosePushL( keys );

    // Find all rows where ID is aServiceId (entry row + each property row)
    TInt err = iRepository->FindEqL( iStartKey, iColMask, aEntryId, keys );

    TInt count = keys.Count();

    if ( KErrNone == err && count > 0 )
        {
        TUint32 idKey = keys[ 0 ];

        for ( TInt i = 1; i <= iColCount; i++ )
            {
            RBuf value;
            value.CreateL( KCenRepMaxDesLength );
            value.CleanupClosePushL();
            TUint32 propertyKey = idKey + i * iColIncrement;
            TInt errProperty = iRepository->Get( propertyKey, value );
            if( errProperty == KErrNone && value.Compare( KNullDesC ) != 0 )
                {
                CCenRepDatabaseProperty* property = CCenRepDatabaseProperty::NewLC();
                property->SetName( propertyKey - idKey + iStartKey );
                User::LeaveIfError( property->SetValue( value ) );
                User::LeaveIfError( aArray.Append( property ) );
                CleanupStack::Pop( property );
                }
                
            CleanupStack::PopAndDestroy( &value );
            }
        }

    CleanupStack::PopAndDestroy( &keys );
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::FindEntryL - OUT" );
    
    return err;

    }

// ---------------------------------------------------------------------------
// Update entry in cenrep.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCenRepDatabaseUtil::UpdateEntryL( 
    TInt aEntryId, 
    const RIpAppPropArray& aArray )
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::UpdateEntryL - IN" );
    
    RKeyArray keys;
    CleanupClosePushL( keys );

    // Find rows where ID is located
    TInt err = iRepository->FindEqL( iStartKey, iColMask, aEntryId, keys );

    TInt count = keys.Count();

    if ( KErrNone == err && count > 0 )
        {
        TUint32 idKey = keys[ 0 ];
        UpdatePropertiesL( idKey, aArray );
        }

    CleanupStack::PopAndDestroy( &keys );
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::UpdateEntryL - OUT" );
    
    return err;
    }

// ---------------------------------------------------------------------------
// Delete entry from cenrep.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCenRepDatabaseUtil::DeleteEntryL( TInt aEntryId )
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::DeleteEntryL - IN" );
    
    RKeyArray keys;
    CleanupClosePushL( keys );

    // Find all rows where ID is aServiceId (entry row + each property row)
    TInt err = iRepository->FindEqL( iStartKey, iColMask, aEntryId, keys );

    TInt count = keys.Count();

    if ( KErrNone == err && count > 0 )
        {
        TUint32 idKey = keys[ 0 ];
        SetOrCreateKeyL( idKey, KNoEntryId );

        for ( TInt i = 1; i <= iColCount; i++ )
            {
            TUint32 propertyKey = idKey + i * iColIncrement;
            SetOrCreateKeyL( propertyKey, KNullDesC );
            }
        }

    CleanupStack::PopAndDestroy( &keys );
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::DeleteEntryL - OUT" );
    
    return err;
    }

// ---------------------------------------------------------------------------
// Find property and its value by ID.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCenRepDatabaseUtil::FindPropertyL( TInt aEntryId, 
                    TUint32 aPropertyName,
                    CCenRepDatabaseProperty& aProperty )
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::FindPropertyL - IN" );
    
    RKeyArray keys;
    CleanupClosePushL( keys );
    TBool found = EFalse;

    // Find all rows where ID is aServiceId (entry row + each property row)
    TInt err = iRepository->FindEqL( iStartKey, iColMask, aEntryId, keys );

    TInt count = keys.Count();

    if ( KErrNone == err && count > 0 )
        {
        TUint32 idKey = keys[ 0 ];

        for ( TInt i = 1; i <= iColCount; i++ )
            {
            TUint32 propertyKey = idKey + i * iColIncrement;
            TUint32 propertyName = propertyKey - idKey + iStartKey;
            if( propertyName == aPropertyName )
                {
                RBuf value;
                value.CreateL( KCenRepMaxDesLength );
                value.CleanupClosePushL();
                
                err = iRepository->Get( propertyKey, value );
                if( err == KErrNone )
                    {
                    if( value.Compare( KNullDesC ) != 0 )
                        {
                        aProperty.SetName( propertyName );
                        User::LeaveIfError( aProperty.SetValue( value ) );
                        found = ETrue;
                        }
                    }
                
                CleanupStack::PopAndDestroy( &value );
                break;
                }
            }
        }

    if( found )
        {
        err = KErrNone;
        }
    else
        {
        if( err == KErrNone )
            {
            err = KErrNotFound;
            }
        }
    CleanupStack::PopAndDestroy( &keys );
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::FindPropertyL - OUT" );
    
    return err;
    }

// ---------------------------------------------------------------------------
// Adds or updates properties to/in cenrep.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCenRepDatabaseUtil::AddOrUpdatePropertiesL( 
    TInt aEntryId, 
    const RIpAppPropArray& aArray )
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::AddOrUpdatePropertiesL - IN" );
    TBool updatedOrCreated = EFalse;
    RKeyArray keys;
    CleanupClosePushL( keys );

    // Find row maching to given ID
    TInt err = iRepository->FindEqL( iStartKey, iColMask, aEntryId, keys );

    if ( KErrNone == err && keys.Count() > 0 )
        {
        TUint32 idKey = keys[ 0 ];

        TInt count = aArray.Count();

        HBufC* data = HBufC::NewLC( KCenRepMaxDesLength );
        TPtr ptr = data->Des();
        
        // Add properties one by one
        for( TInt i = 0; i < count; i++ )
            {
            CCenRepDatabaseProperty* property = aArray[i];
            TUint32 propertyName = property->GetName();
            TUint32 key = idKey - iStartKey + propertyName;
            

            TInt errProperty = iRepository->Get( key, ptr );
            if( errProperty || ( ptr != property->GetDesValue() ) )
                {
                updatedOrCreated = ETrue;
                SetOrCreateKeyL( key, property->GetDesValue() );
                }
            
            }
        
        CleanupStack::PopAndDestroy( data );
        }

    CleanupStack::PopAndDestroy( &keys );
    
    if( !err && !updatedOrCreated )
        {
        CENREPDATABASELOG( "CCenRepDatabaseUtil::AddOrUpdatePropertiesL - Not updated or created" );
        err = KErrCancel;
        }
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::AddOrUpdatePropertiesL - OUT" );
    return err;
    }

// ---------------------------------------------------------------------------
// Return count of entries in cenrep.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCenRepDatabaseUtil::EntryCountL( TInt& aCount )
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::EntryCountL - IN" );
    
    RKeyArray keys;
    CleanupClosePushL( keys );

    // Find all rows where ID not equals to KNoEntryId
    TInt err = iRepository->FindNeqL( iStartKey, iColMask, KNoEntryId, keys );

    TInt count = keys.Count();

    if ( KErrNone == err && count > 0 )
        {
        aCount = keys.Count();
        }
    else
        {
        aCount = 0;    
        }

    CleanupStack::PopAndDestroy( &keys );
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::EntryCountL - OUT" );

    return err;
    }

// ---------------------------------------------------------------------------
// Find all ids of entries in cenrep.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCenRepDatabaseUtil::FindEntryIdsL( RArray<TInt>& aEntryIds )
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::FindEntryIdsL - IN" );
    
    aEntryIds.Reset();
    
    RKeyArray keys;
    CleanupClosePushL( keys );

    // Find all entry rows from ID column. Deleted rows are not returned.
    TInt err = iRepository->FindNeqL( iStartKey, iColMask, KNoEntryId, keys );

    TInt count = keys.Count();

    if ( KErrNone == err && count > 0 )
        {
        for( TInt i = 0; i < count; i++ )
            {
            TUint32 key = keys[i];
            TInt entryId = 0;
            User::LeaveIfError( iRepository->Get( key, entryId ) );
            User::LeaveIfError( aEntryIds.Append( entryId ) );
            }
        }

    CleanupStack::PopAndDestroy( &keys );
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::FindEntryIdsL - OUT" );
    
    return err;
    }

// ---------------------------------------------------------------------------
// Delete given properties from entry.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCenRepDatabaseUtil::DeletePropertiesL( TInt aEntryId,
                        const RArray<TUint32>& aNameArray )
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::DeletePropertiesL - IN" );
    
    RKeyArray keys;
    CleanupClosePushL( keys );

    // Find row that matches to aEntryID
    TInt err = iRepository->FindEqL( iStartKey, iColMask, aEntryId, keys );

    if ( KErrNone == err && keys.Count() > 0 )
        {
        TUint32 idKey = keys[ 0 ];

        TInt count = aNameArray.Count();

        // delete properties one by one
        for( TInt i = 0; i < count; i++ )
            {
            TUint32 name = aNameArray[i];
            TUint32 key = idKey - iStartKey + name;
            SetOrCreateKeyL( key, KNullDesC );
            }
        }

    CleanupStack::PopAndDestroy( &keys );
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::DeletePropertiesL - OUT" );
    
    return err;
    }

// ---------------------------------------------------------------------------
// Find entry ids of given properties.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCenRepDatabaseUtil::FindEntryIdsFromPropertiesL( 
    const RIpAppPropArray& aPropertyArray, 
    RArray<TInt>& aEntryIds )
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::FindEntryIdsFromPropertiesL - IN" );
    
    TInt err = FindEntryIdsL( aEntryIds );
    if( err == KErrNone )
        {
        TInt count = aEntryIds.Count();
        TInt removed = 0;
        
        for( TInt i = 0; i < count; i++ )
            {
            TBool ok = EFalse;
            TUint32 entryId = aEntryIds[ i - removed ];
            CheckEntryHasPropertiesL( entryId, aPropertyArray, ok );
            
            if( !ok )
                {
                aEntryIds.Remove( i - removed );
                removed++;
                }
            }
        }
        
    CENREPDATABASELOG( "CCenRepDatabaseUtil::FindEntryIdsFromPropertiesL - OUT" );
        
    return err;
    }

// ---------------------------------------------------------------------------
// Release semaphore, this function is used in custom cleanup
// ---------------------------------------------------------------------------
//
void CCenRepDatabaseUtil::ReleaseSemaphore( TAny* aPtr )
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::ReleaseSemaphore - IN" );
    
    ASSERT ( aPtr );
    CCenRepDatabaseUtil* utils = static_cast<CCenRepDatabaseUtil*>( aPtr );
    utils->DoReleaseSemaphore();
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::ReleaseSemaphore - OUT" );
    }

// ---------------------------------------------------------------------------
// Release semaphore
// ---------------------------------------------------------------------------
//
void CCenRepDatabaseUtil::DoReleaseSemaphore()
    {
    CENREPDATABASELOG( "CCenRepDatabaseUtil::DoReleaseSemaphore - IN" );
    
    iSemaphore.Signal();
    
    CENREPDATABASELOG( "CCenRepDatabaseUtil::DoReleaseSemaphore - OUT" );
    }

// ---------------------------------------------------------------------------
// Set value of key or create a new one.
// ---------------------------------------------------------------------------
//
template<class T> void CCenRepDatabaseUtil::SetOrCreateKeyL( TUint32 aKey, 
    const T& aValue )
    {
    TInt err = iRepository->Set( aKey, aValue );

    if ( KErrNotFound == err )
        {
        err = iRepository->Create( aKey, aValue );
        }

    User::LeaveIfError( err );
    }

// ---------------------------------------------------------------------------
// Create new ID.
// ---------------------------------------------------------------------------
//
void CCenRepDatabaseUtil::CreateEntryIdL( TInt& aNewId )
    {
    TInt newId( KNoEntryId );

    TInt err = iRepository->Get( iIdCounterKey, newId );

    if ( KErrNone == err  )
        {
        if ( 0 < newId )
            {
            // Store next available ID
            err = iRepository->Set( iIdCounterKey, newId + 1 );
            }
        else
            {
            // Range of ID exceeded
            err = KErrOverflow;
            }
        }
    else if ( KErrNotFound == err )
        {
        // Create new key (should always be found)
        SetOrCreateKeyL( iIdCounterKey, KNoEntryId + 1 );
        }

    aNewId = newId;

    User::LeaveIfError( err );
    }

// ---------------------------------------------------------------------------
// Get next available key for ID.
// ---------------------------------------------------------------------------
//
TUint32 CCenRepDatabaseUtil::GetNewIdKeyL()
    {
    RKeyArray keys;
    CleanupClosePushL( keys );
    TUint32 idKey( 0 );

    // Reuse keys which value is KNoEntryId
    TInt err = iRepository->FindEqL( iStartKey, iColMask, KNoEntryId, keys );

    if ( KErrNone == err && 0 < keys.Count() )
        {
        // return id column key from first deleted row
        idKey = keys[0];
        }
    else if ( KErrNotFound == err )
        {
        keys.Close();
        TInt err1 = iRepository->FindL( iStartKey, iColMask, keys );

        if( KErrNone == err1  )
            {
            // return new
            TInt count( keys.Count() );
            idKey = keys[count-1] + 1;
            }
        else if ( KErrNotFound == err1 )
            {
            idKey = iStartKey;
            }
        else
            {
            User::Leave( err1 );
            }
        }
    else
        {
        User::Leave( err );
        }

    CleanupStack::PopAndDestroy( &keys );

    return idKey;
    }
    
// ---------------------------------------------------------------------------
// Update given properties to cenrep.
// ---------------------------------------------------------------------------
//
void CCenRepDatabaseUtil::UpdatePropertiesL(
    TUint32 aIdKey,
    const RIpAppPropArray& aArray )
    {
    TInt count = aArray.Count();

    // Create all keys for this entry
    RKeyArray propertyKeys;
    CleanupClosePushL( propertyKeys );
    for( TInt k = 1; k <= iColCount; k++ )
        {
        User::LeaveIfError( propertyKeys.Append( aIdKey + k * iColIncrement ) );
        }
    
    // Add properties one by one
    for( TInt i = 0; i < count; i++ )
        {
        CCenRepDatabaseProperty* property = aArray[i];
        TUint32 propertyName = property->GetName();
        TUint32 key = aIdKey - iStartKey + propertyName;
        SetOrCreateKeyL( key, property->GetDesValue() );
        
        // Get only non-set keys
        TInt index = propertyKeys.Find( key );
        if( index != KErrNotFound )
            {
            propertyKeys.Remove( index );
            }
        }
    
    // set non-set properties one by one
    count = propertyKeys.Count();
    for( TInt j = 0; j < count; j++ )
        {
        TUint32 key = propertyKeys[j];
        SetOrCreateKeyL( key, KNullDesC() );
        }
    
    CleanupStack::PopAndDestroy( &propertyKeys );    
    }

// ---------------------------------------------------------------------------
// Check if entry has these properties.
// ---------------------------------------------------------------------------
//
void CCenRepDatabaseUtil::CheckEntryHasPropertiesL( 
    TInt aEntryId, 
    const RIpAppPropArray& aPropertyArray, 
    TBool& aOk )
    {
    RIpAppPropArray allArray;
    TCleanupItem cleanup( CCenRepDatabaseUtil::CleanupPointerArray, &allArray );
    CleanupStack::PushL( cleanup );
    
    TInt err = FindEntryL( aEntryId, allArray );
    if( err == KErrNone )
        {
        aOk = ETrue;
        TInt count = aPropertyArray.Count();
        TInt allCount = allArray.Count();
        
        for( TInt i = 0; i < count; i++ )
            {
            CCenRepDatabaseProperty* property = aPropertyArray[i];
            TUint32 name = property->GetName();
            const TDesC& value = property->GetDesValue();
            TBool found = EFalse;
            for( TInt j = 0; j < allCount; j++ )
                {
                CCenRepDatabaseProperty* allProperty = allArray[j];
                if( name == allProperty->GetName() &&
                    value.Compare( allProperty->GetDesValue() ) == 0 )
                    {
                    found = ETrue;
                    break;
                    }
                }
            if( !found )
                {
                aOk = EFalse;
                break;
                }
            }
        }
    else
        {
        aOk = EFalse;
        }

    CleanupStack::PopAndDestroy( &allArray );
    }

// ---------------------------------------------------------------------------
// Cleanup array of properties.
// ---------------------------------------------------------------------------
//
void CCenRepDatabaseUtil::CleanupPointerArray(  TAny* aPointer )
    {
    RIpAppPropArray* array = static_cast<RIpAppPropArray*>( aPointer );
    array->ResetAndDestroy();
    }
    


