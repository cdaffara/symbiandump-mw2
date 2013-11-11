/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   XDM Engine settings API
*
*/



#include <flogger.h>
#include <centralrepository.h>
#include "XdmSettingsApi.h"
#include "XdmSettingsProperty.h"
#include "XdmSettingsCollection.h"

// ---------------------------------------------------------
// TXdmSettingsApi::CreateCollectionL
//
// ---------------------------------------------------------
//
EXPORT_C TInt TXdmSettingsApi::CreateCollectionL( const CXdmSettingsCollection& aCollection )
    {
    #ifdef _DEBUG
        WriteToLog( _L8( "TXdmSettingsApi::CreateCollectionL()" ) );
    #endif
    TInt error = KErrArgument;
    CRepository* repository = CRepository::NewL( KCRUidXdmEngine );
    CleanupStack::PushL( repository );
    TInt row = LastRow( repository );
    TInt count = aCollection.Count();
    __ASSERT_DEBUG( count > 0, User::Leave( KErrArgument ) );
    for( TInt i = 0;i < count;i++ )
        {
        const CXdmSettingsProperty& prop = aCollection.Property( i );
        TInt32 column = prop.PropertyName();
        //Don't let the client set the settings ID, since
        //this might create conflicting IDs in the repository
        if( column != KXdmSettingsId )
            {
            //Now merge row & column
            TInt32 newKey = ( row * 0x100 ) | column;
            error = repository->Create( newKey, prop.PropertyValue() );
            #ifdef _DEBUG
                HBufC8* buf = HBufC8::NewLC( prop.PropertyValue().Length() );
                buf->Des().Copy( prop.PropertyValue() );
                TPtrC8 des( buf->Des() );
                WriteToLog( _L8( " New key [0x%08x] created - Name: %d  Value: %S" ), newKey, column, &des );
                CleanupStack::PopAndDestroy();  //buf
            #endif
            }
        }
    error = CreateUniqueIdL( repository, row );
    #ifdef _DEBUG
        WriteToLog( _L8( " New settings ID created - ID: %d  Error: %d" ), row, error );
    #endif
    CleanupStack::PopAndDestroy();  //repository
    return error == KErrNone ? row : error;
    }

// ---------------------------------------------------------
// TXdmSettingsApi::RemoveCollectionL
//
// ---------------------------------------------------------
//
EXPORT_C void TXdmSettingsApi::RemoveCollectionL( TInt aSettingsId )
    {
    #ifdef _DEBUG
        WriteToLog( _L8( "TXdmSettingsApi::RemoveCollectionL() - ID: %d" ), aSettingsId );
    #endif
    CRepository* repository = CRepository::NewL( KCRUidXdmEngine );
    CleanupStack::PushL( repository );
    TInt row = FindRowL( aSettingsId, repository );
    if( row >= KErrNone )
        {
        TInt32 key = -1;
        TInt error = KErrNone;
        for( TInt i = 0;i < KXdmPropertyCount;i++ )
            {
            key = ( row * 0x100 ) | i;
            error = repository->Delete( key );
            #ifdef _DEBUG
                WriteToLog( _L8( " Key %x deleted - Error: %d" ), key, error );
            #endif
            }
        }
    else User::Leave( KErrNotFound );
    CleanupStack::PopAndDestroy();
    }
    
// ---------------------------------------------------------
// TXdmSettingsApi::CreatePropertyL
//
// ---------------------------------------------------------
//   
EXPORT_C void TXdmSettingsApi::CreatePropertyL( TInt aSettingsId,
                                                const TDesC& aPropertyValue,
                                                TXdmSettingsProperty aPropertyName )
    {
    #ifdef _DEBUG
        WriteToLog( _L8( "TXdmSettingsApi::UpdatePropertyL()" ) );
        WriteToLog( _L8( "  Property:           %d" ), aPropertyName );
        WriteToLog( _L8( "  Property set ID:    %d" ), aSettingsId );
        WriteToLog( _L8( "  Property value:     %S" ), &aPropertyValue );
    #endif
    TInt error = KErrNotFound;
    CRepository* repository = CRepository::NewL( KCRUidXdmEngine );
    CleanupStack::PushL( repository );
    TInt row = FindRowL( aSettingsId, repository );
    if( row >= 0 )      
        {
        TInt32 key = ( row * 0x100 ) | aPropertyName;
        error = repository->Create( key, aPropertyValue );
        if( error != KErrNone )
            {
            #ifdef _DEBUG
                WriteToLog( _L8( "  Create() failed with %d" ), error );
            #endif
            User::Leave( error );
            }
        }
    else                            //No keys or too many
        {
        #ifdef _DEBUG
            WriteToLog( _L8( "  Property set not found - Error: %d" ), row );
        #endif
        User::Leave( error );
        } 
    CleanupStack::PopAndDestroy();  //repository
    }

// ---------------------------------------------------------
// TXdmSettingsApi::UpdatePropertyL
//
// ---------------------------------------------------------
//   
EXPORT_C void TXdmSettingsApi::UpdatePropertyL( TInt aSettingsId,
                                                const TDesC& aPropertyValue,
                                                TXdmSettingsProperty aPropertyName )

    {
    #ifdef _DEBUG
        HBufC8* value = HBufC8::NewLC( aPropertyValue.Length() );
        TPtr8 prop( value->Des() );
        prop.Copy( aPropertyValue ); 
        WriteToLog( _L8( "TXdmSettingsApi::UpdatePropertyL()" ) );
        WriteToLog( _L8( "  Settings ID:      %d" ), aSettingsId );
        WriteToLog( _L8( "  Property name:    %d" ), aPropertyName );
        WriteToLog( _L8( "  Property value:   %S" ), &prop );
        CleanupStack::PopAndDestroy();  //value
    #endif
    TInt error = KErrAlreadyExists;
    CRepository* repository = CRepository::NewL( KCRUidXdmEngine );
    CleanupStack::PushL( repository );
    RArray<TUint32> keys = FindByIdL( aSettingsId, repository );
    CleanupClosePushL( keys );
    TInt count = keys.Count();
    if( count == 1 )      
        {
        TInt32 row = ( keys[0] & 0xFFFFFF00 ) / 0x100;
        TInt32 key = ( row * 0x100 ) | aPropertyName;
        #ifdef _DEBUG
            WriteToLog( _L8( "  Key to update:    0x%08x" ), key );
        #endif
        error = repository->Set( key, aPropertyValue );
        if( error != KErrNone )
            {
            #ifdef _DEBUG
                WriteToLog( _L8( "  Set() failed with %d, try Create()" ), error );
            #endif
            error = repository->Create( key, aPropertyValue );
            #ifdef _DEBUG
                WriteToLog( _L8( "  Create() completed with %d" ), error );
            #endif
            }
        }
    else                            //No keys or too many
        {
        #ifdef _DEBUG
            TBuf<32> errBuf;
            count > 0 ? errBuf.Append( _L( "Too many results" ) ) :
                        errBuf.Append( _L( "No results found" ) );
            WriteToLog( _L8( "  * Error - %S: %d" ), &errBuf, count );
        #endif
        TInt error = count > 0 ? KErrGeneral : KErrNotFound;
        User::Leave( error );
        } 
    CleanupStack::PopAndDestroy( 2 );  //keys, repository
    }

// ---------------------------------------------------------
// TXdmSettingsApi::RemovePropertyL
//
// ---------------------------------------------------------
//
EXPORT_C void TXdmSettingsApi::RemovePropertyL( TInt aSettingsId,
                                                TXdmSettingsProperty aPropertyName )
    {
    #ifdef _DEBUG
        WriteToLog( _L8( "TXdmSettingsApi::RemovePropertyL() - ID: %d" ), aSettingsId );
    #endif
    TInt error = KErrNotFound;
    CRepository* repository = CRepository::NewL( KCRUidXdmEngine );
    CleanupStack::PushL( repository );
    TInt row = FindRowL( aSettingsId, repository );
    if( row >= 0 )      
        {
        TInt32 key = ( row * 0x100 ) | aPropertyName;
        error = repository->Delete( key );
        if( error != KErrNone )
            {
            #ifdef _DEBUG
                WriteToLog( _L8( "  Delete() failed with %d" ), error );
            #endif
            User::Leave( error );
            }
        }
    else                            //No keys or too many
        {
        #ifdef _DEBUG
            WriteToLog( _L8( "  Property set not found - Error: %d" ), row );
        #endif
        User::Leave( error );
        } 
    CleanupStack::PopAndDestroy();  //repository
    }
    
// ---------------------------------------------------------
// TXdmSettingsApi::CreateUniqueIdL
//
// ---------------------------------------------------------
//
TInt TXdmSettingsApi::CreateUniqueIdL( CRepository* aRepository, TInt aRow )
    {
    TBuf<10> numBuf;
    numBuf.Zero();
    numBuf.AppendNum( aRow );
    TInt error = KErrNone;
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    const TInt32 KPartialIdMask = 0x000000FF;
    error = aRepository->FindEqL( KXdmSettingsId, KPartialIdMask, numBuf, keys );
    __ASSERT_DEBUG( error == KErrNotFound, User::Panic( _L( "CXdmSettingsApi" ), 1 ) );
    TInt32 newKey = ( aRow * 0x100 ) | KXdmSettingsId;
    error = aRepository->Create( newKey, numBuf );
    CleanupStack::PopAndDestroy();  //keys 
    return error;
    }
    
// ---------------------------------------------------------
// TXdmSettingsApi::LastRow
//
// ---------------------------------------------------------
//
TInt TXdmSettingsApi::LastRow( CRepository* aRepository )
    {
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    const TInt32 KPartialNameMask = 0x000000FF;
    TInt error = aRepository->FindL( KXdmSettingsId, KPartialNameMask, keys );
    TInt count = keys.Count();
    //Must check for existing IDs
    if( count > 0 )
        {
        TInt exists = FindRowL( count, aRepository );
        while( exists != KErrNotFound )
            {
            count++;
            exists = FindRowL( count, aRepository );
            }
        }
    CleanupStack::PopAndDestroy();  //keys
    return count;
    }

// ---------------------------------------------------------
// TXdmSettingsApi::FindRowL
//
// ---------------------------------------------------------
//
TInt TXdmSettingsApi::FindRowL( TInt aSettingsId,
                                CRepository* aRepository )
    {
    TUint32 rowNum = 0;
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    TBuf<10> numBuf;
    numBuf.AppendNum( aSettingsId );
    const TInt32 KPartialNameMask = 0x000000FF;
    TInt error = aRepository->FindEqL( KXdmSettingsId, KPartialNameMask, numBuf, keys );
    if( error == KErrNone && keys.Count() == 1 )
        {
        //Extract row part from the key
        TInt row = keys[0] & 0xFFFFFF00;
        //e.g. 0x00000400 => 0x00000004
        rowNum = row / 0x100;
        }
    CleanupStack::PopAndDestroy();  //keys
    return error == KErrNone ? rowNum : error;
    }
           
// ---------------------------------------------------------
// TXdmSettingsApi::FindByIdL
//
// ---------------------------------------------------------
//
RArray<TUint32> TXdmSettingsApi::FindByIdL( TInt aSettingsId, CRepository* aRepository )
    {
    const TInt32 KPartialKeyMask = 0x000000FF;
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    TBuf<15> numBuf;
    numBuf.AppendNum( aSettingsId );
    TInt error = aRepository->FindEqL( KXdmSettingsId, KPartialKeyMask, numBuf, keys );
    CleanupStack::Pop();  //keys
    return keys;
    }

// ---------------------------------------------------------
// TXdmSettingsApi::FindByTypeL
//
// ---------------------------------------------------------
//
RArray<TUint32> TXdmSettingsApi::FindByTypeL( CRepository* aRepository,
                                              TXdmSettingsProperty aSingleProp )
    {
    const TInt32 KPartialKeyMask = 0x000000FF;
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    TInt error = aRepository->FindL( aSingleProp, KPartialKeyMask, keys );
    CleanupStack::Pop();  //keys
    return keys;
    }

// ----------------------------------------------------------
// TXdmSettingsApi::WriteToLog
// 
// ----------------------------------------------------------
//
void TXdmSettingsApi::WriteToLog( TRefByValue<const TDesC8> aFmt,... )                                 
    {
    VA_LIST list;
    VA_START( list, aFmt );
    TBuf8<KXdmSettingsLogBufferMaxSize> buf;
    buf.FormatList( aFmt, list );
    RFileLogger::Write( KXdmSettingsLogDir, KXdmSettingsLogFile, 
                        EFileLoggingModeAppend, buf );
    }

// ---------------------------------------------------------
// TXdmSettingsApi::CollectionNamesL
//
// ---------------------------------------------------------
//
EXPORT_C CDesCArray* TXdmSettingsApi::CollectionNamesLC( RArray<TInt>& aSettingIds )
    {
    #ifdef _DEBUG
        WriteToLog( _L8( "TXdmSettingsApi::CollectionNamesL()" ) );
    #endif
    TInt error = KErrNone;
    CDesCArrayFlat* propertySet = new ( ELeave ) CDesCArrayFlat( 10 );
    CleanupStack::PushL( propertySet );
    CRepository* repository = CRepository::NewL( KCRUidXdmEngine );
    CleanupStack::PushL( repository );
    RArray<TUint32> nameKeys = FindByTypeL( repository, EXdmPropName );
    CleanupClosePushL( nameKeys );
    RArray<TUint32> idKeys = FindByTypeL( repository, EXdmPropSettingsId );
    CleanupClosePushL( idKeys );
    #ifdef _DEBUG
        WriteToLog( _L8( "  ID key count: %d" ), idKeys.Count() );
        WriteToLog( _L8( "  Name key count: %d" ), nameKeys.Count() );
    #endif
    TInt count = nameKeys.Count();
    if( count > 0 )
        {
        TInt numId = 0; 
        HBufC* name = NULL;
        TBuf<10> identifier;
        for( TInt i = 0;i < count;i++ )
            {
            identifier.Zero();
            name = HBufC::NewLC( NCentralRepositoryConstants::KMaxUnicodeStringLength );
            TPtr desc( name->Des());
            error = repository->Get( nameKeys[i], desc );
            error = repository->Get( idKeys[i], identifier );
            if( error == KErrNone && desc.Length() > 0 && identifier.Length() > 0 )
                {
                #ifdef _DEBUG
                    HBufC8* eightName = HBufC8::NewLC( desc.Length() );
                    TPtr8 eightDesc( eightName->Des() );
                    eightDesc.Copy( desc );
                    WriteToLog( _L8( " Collection no. %d - Name: %S  ID: %S" ), i, &eightDesc, &identifier );
                    CleanupStack::PopAndDestroy();  //eightName
                #endif
                propertySet->AppendL( desc );
                TLex id( identifier );
                error = id.Val( numId );
                aSettingIds.Append( error == KErrNone ? numId : error );
                }
            else
                {
                #ifdef _DEBUG
                    WriteToLog( _L8( " Fetching of the name no. %d failed with: %d" ), i, error );
                #endif
                }
            CleanupStack::PopAndDestroy();  //name
            }
        }
    CleanupStack::PopAndDestroy( 3 );   //idKeys, nameKeys, repository
    return propertySet;
    }
                                            
// ---------------------------------------------------------
// TXdmSettingsApi::SettingsCollectionLC
//
// ---------------------------------------------------------
//
EXPORT_C CXdmSettingsCollection* TXdmSettingsApi::SettingsCollectionL( TInt aSettingsId )
    {
    #ifdef _DEBUG
        WriteToLog( _L8( "TXdmSettingsApi::SettingsCollectionL() - ID: %d" ), aSettingsId );
    #endif
    CXdmSettingsCollection* collection = NULL;
    CRepository* repository = CRepository::NewL( KCRUidXdmEngine );
    CleanupStack::PushL( repository );
    TInt row = FindRowL( aSettingsId, repository );
    if( row >= KErrNone )
        {
        TInt32 key = -1;
        HBufC* buf = NULL;
        TInt error = KErrNone;
        collection = new ( ELeave ) CXdmSettingsCollection();
        CleanupStack::PushL( collection );
        #ifdef _DEBUG
            WriteToLog( _L8( " Data in the specified colletion: " ) );
        #endif
        for( TInt i = 0;i < KXdmPropertyCount;i++ )
            {
            key = ( row * 0x100 ) | i;
            buf = HBufC::NewLC( NCentralRepositoryConstants::KMaxUnicodeStringLength );
            TPtr desc( buf->Des() );
            error = repository->Get( key, desc );
            if( error == KErrNone )
                {
                #ifdef _DEBUG
                    HBufC8* eight = HBufC8::NewLC( desc.Length() );
                    TPtr8 eightDesc( eight->Des() );
                    eightDesc.Copy( desc );
                    WriteToLog( _L8( "  Property %d [0x%08x]: %S" ), i, key, &eightDesc );
                    CleanupStack::PopAndDestroy();   //eight
                #endif
                collection->AppendL( desc, ( TXdmSettingsProperty )i );
                }
            CleanupStack::PopAndDestroy();  //buf
            }
        CleanupStack::Pop();  //collection
        }
    else
        {
        #ifdef _DEBUG
            WriteToLog( _L8( "  Could not find the collection, leaves with KErrNotFound" ) );
        #endif
        User::Leave( KErrNotFound );
        }
    CleanupStack::PopAndDestroy();  //repository
    return collection;
    }
        
// ---------------------------------------------------------
// TXdmSettingsApi::PropertyL
//
// ---------------------------------------------------------
//                                     
EXPORT_C HBufC* TXdmSettingsApi::PropertyL( TInt aSettingsId,
                                            TXdmSettingsProperty aSingleProp )
    {
    #ifdef _DEBUG
        WriteToLog( _L8( "TXdmSettingsApi::PropertyLC()" ) );
        WriteToLog( _L8( "  Property:           %d" ), &aSingleProp );
        WriteToLog( _L8( "  Property set ID:    %d" ), aSettingsId );
    #endif
    HBufC* buf = NULL;
    TInt error = KErrGeneral;
    CRepository* repository = CRepository::NewL( KCRUidXdmEngine );
    CleanupStack::PushL( repository );
    RArray<TUint32> keys = FindByIdL( aSettingsId, repository );
    CleanupClosePushL( keys );
    TInt count = keys.Count();
    if( count == 1 )                //This is the normal case
        {
        TInt32 row = ( keys[0] & 0xFFFFFF00 ) / 0x100;
        TInt32 key = ( row * 0x100 ) | aSingleProp;
        buf = HBufC::NewL( NCentralRepositoryConstants::KMaxUnicodeStringLength );
        TPtr desc( buf->Des() );
        error = repository->Get( key, desc );
        #ifdef _DEBUG
            WriteToLog( _L8( "  Error:    %d" ), error );
        #endif
        }
    else                            //No keys or too many
        {
        #ifdef _DEBUG
            TBuf8<32> errBuf;
            count > 0 ? errBuf.Append( _L8( "Too many results" ) ) :
                        errBuf.Append( _L8( "No results found" ) );
            WriteToLog( _L8( "  * Error - %S: %d" ), &errBuf, count );
        #endif
        error = count > 0 ? KErrGeneral : KErrNotFound;
        User::Leave( error );
        }
    CleanupStack::PopAndDestroy( 2 );  //keys, repository
    return buf;
    }
            
// End of File

