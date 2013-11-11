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
* Description: 
*
*/


// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <ecom.h>
#include <StifParser.h>
#include <StifTestEventInterface.h>
#include <Stiftestinterface.h>
#include <mediacollectionmanager.h>
#include "CollectionManagerTest.h"

#include "mdsutils.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCollectionManagerTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CCollectionManagerTest::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CCollectionManagerTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CCollectionManagerTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 

        // engine		
        ENTRY( "CreateCollectionManager", CCollectionManagerTest::CreateCollectionManagerL ),
        ENTRY( "GetCollectionInfos", CCollectionManagerTest::GetCollectionInfosL ),
        ENTRY( "CollectionInfoById", CCollectionManagerTest::CollectionInfoByIdL ),
        ENTRY( "CreateCollection", CCollectionManagerTest::CreateCollectionL ),
        ENTRY( "DeleteCollection", CCollectionManagerTest::DeleteCollectionL ),
        ENTRY( "RenameCollection", CCollectionManagerTest::RenameCollectionL ),
        ENTRY( "AddToCollection", CCollectionManagerTest::AddToCollectionL ),
        ENTRY( "RemoveFromCollectionL", CCollectionManagerTest::RemoveFromCollectionL ),
        ENTRY( "GetCollectionItemArray", CCollectionManagerTest::GetCollectionItemArrayL ),
        ENTRY( "GetCollectionIdByItem", CCollectionManagerTest::GetCollectionIdByItemL ),
        ENTRY( "AddCollectionObserver", CCollectionManagerTest::AddCollectionObserverL ),
        ENTRY( "RemoveCollectionObserver", CCollectionManagerTest::RemoveCollectionObserverL ),
        ENTRY( "GetDriveById", CCollectionManagerTest::GetDriveByIdL ),
        ENTRY( "EndTestSession", CCollectionManagerTest::EndTestSessionL )
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CCollectionManagerTest::CollectionChangedL
// -----------------------------------------------------------------------------
//
void CCollectionManagerTest::CollectionChangedL( const TArray<TInt>& /*aCollectionIdArray*/ )
    {
    // This is not going to be called
    }    

// -----------------------------------------------------------------------------
// CCollectionManagerTest::CollectionManagerErrorL
// -----------------------------------------------------------------------------
//
void CCollectionManagerTest::CollectionManagerErrorL( TInt /*aError*/ )
    {
    // This is not going to be called
    }    


// -----------------------------------------------------------------------------
// CCollectionManagerTest::CreateCollectionManagerLL
// -----------------------------------------------------------------------------
//
TInt CCollectionManagerTest::CreateCollectionManagerL( CStifItemParser& /* aItem */ )
    {
    iCollectionManager = MediaCollectionManagerFactory::CreateCollectionManagerL( );
    
    return KErrNone;
    }    

// -----------------------------------------------------------------------------
// CCollectionManagerTest::GetCollectionInfosL
// -----------------------------------------------------------------------------
//
TInt CCollectionManagerTest::GetCollectionInfosL( CStifItemParser& /* aItem */ )
	{
    _LIT( KMsg1, "Enter GetCollectionInfosL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
	
    TInt error( KErrNone );
    
    if( iCollectionManager )
        {
        RPointerArray<MMediaCollectionInfo> collectionInfoArray;
        TRAP( error, iCollectionManager->GetCollectionInfosL( collectionInfoArray ) );
        if( error != KErrNotSupported && error != KErrNone )
            {
            return error;
            }
        error = KErrNone;
        const TInt infoArrayCount( collectionInfoArray.Count() );
        if( infoArrayCount > 0 )
            {
            error = KErrUnknown;
            }
        }
    else
        {
        error = KErrUnknown;
        }
	
    _LIT( KMsg2, "Exit GetCollectionInfosL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    
    return error;
    }

// -----------------------------------------------------------------------------
// CCollectionManagerTest::CollectionInfoByIdL
// -----------------------------------------------------------------------------
//
TInt CCollectionManagerTest::CollectionInfoByIdL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "Enter CollectionInfoByIdL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    TInt collectionID( KErrNotFound );
    TInt error( KErrNone );
    MMediaCollectionInfo* mci = NULL;
    if( !iCollectionManager )
        {
        return KErrUnknown;
        }
    
    TRAP( error, mci = iCollectionManager->CollectionInfoByIdLC( collectionID ) );
    if( error != KErrNotSupported && error != KErrNone )
        {
        return error;
        }
    
    if( mci )
        {
        return KErrCorrupt;
        }
    
    TRAP( error, mci = iCollectionManager->CollectionInfoByIdLC( 0 ) );
    if( error != KErrNotSupported && error != KErrNone )
        {
        return error;
        }
    
    if( mci )
        {
        return KErrCorrupt;
        }
    
    _LIT( KMsg2, "Exit CollectionInfoByIdL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCollectionManagerTest::CreateCollectionL
// -----------------------------------------------------------------------------
//
TInt CCollectionManagerTest::CreateCollectionL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter CreateCollectionL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    if( !iCollectionManager )
        {
        return KErrUnknown;
        }
    
    _LIT( KTestAlbumName, "TestAlbum" );
    HBufC* collectionName = KTestAlbumName().AllocL();
    RArray<TInt> types;
    types.AppendL( 1 );
    TInt32 properties = EMCPhoneMemoryCollection;

    TInt collectionId( KErrNotFound );
    TInt error( KErrNone );
    TRAP( error, collectionId = iCollectionManager->CreateCollectionL( *collectionName, types.Array(), properties ) );
    if( error != KErrNotSupported && error != KErrNone )
        {
        delete collectionName;
        collectionName = NULL;
        return error;
        }

    properties = EMCMmcCollection;
    TRAP( error, collectionId = iCollectionManager->CreateCollectionL( *collectionName, types.Array(), properties ) );
    if( error != KErrNotSupported && error != KErrNone )
        {
        delete collectionName;
        collectionName = NULL;
        return error;
        }

    MMediaCollectionInfo* mci = NULL;
    TRAP( error, iCollectionManager->CollectionInfoByIdLC( collectionId ) );
    if( error != KErrNotSupported && error != KErrNone )
        {
        delete collectionName;
        collectionName = NULL;
        return error;
        }

    if( mci )
        {
        delete collectionName;
        collectionName = NULL;
        return KErrCorrupt;
        }

    delete collectionName;
    collectionName = NULL;
    
    _LIT( KMsg2, "Exit CreateCollectionL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
	
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCollectionManagerTest::DeleteCollectionL
// -----------------------------------------------------------------------------
//
TInt CCollectionManagerTest::DeleteCollectionL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter DeleteCollectionL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    if( !iCollectionManager )
        {
        return KErrUnknown;
        }
    
    TInt collectionId( KErrNotFound );
    TInt error( KErrNone );
    TRAP( error, iCollectionManager->DeleteCollectionL( collectionId ) );
    if( error != KErrNotSupported && error != KErrNone )
        {
        return error;
        }

    collectionId = 0;
    TRAP( error, iCollectionManager->DeleteCollectionL( collectionId ) );
    if( error != KErrNotSupported && error != KErrNone )
        {
        return error;
        }
    
    _LIT( KMsg2, "Exit DeleteCollectionL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCollectionManagerTest::RenameCollectionL
// -----------------------------------------------------------------------------
//
TInt CCollectionManagerTest::RenameCollectionL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter RenameCollectionL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    if( !iCollectionManager )
        {
        return KErrUnknown;
        }
    
    TInt collectionId( KErrNotFound );
    TInt error( KErrNone );
    
    _LIT( KTestAlbumName, "NewTestAlbum" );
    HBufC* collectionNewName = KTestAlbumName().AllocL();
    
    TRAP( error, iCollectionManager->RenameCollectionL( collectionId, *collectionNewName ) );
    if( error != KErrNotSupported && error != KErrNone )
        {
        delete collectionNewName;
        collectionNewName = NULL;
        return error;
        }

    MMediaCollectionInfo* mci = NULL;
    
    TRAP( error, mci= iCollectionManager->CollectionInfoByIdLC( collectionId ) );
    if( error != KErrNotSupported && error != KErrNone )
        {
        delete collectionNewName;
        collectionNewName = NULL;
        return error;
        }
    
    if( mci )
        {
        delete collectionNewName;
        collectionNewName = NULL;
        return KErrCorrupt;
        }
    
    delete collectionNewName;
    collectionNewName = NULL;
    
    _LIT( KMsg2, "Exit RenameCollectionL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
	
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCollectionManagerTest::AddToCollectionL
// -----------------------------------------------------------------------------
//
TInt CCollectionManagerTest::AddToCollectionL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter AddToCollectionL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );

    if( !iCollectionManager )
        {
        return KErrUnknown;
        }
    
    const TInt itemCount( 500 );
    CDesCArray* newItemArray = new (ELeave) CDesCArrayFlat( itemCount );

    for( TInt i = 0 ; i < itemCount ; ++i )
        {
        TBuf<40> b;
        b.Num( i );
        b.Append( _L("TestItem") );
        newItemArray->AppendL( b );
        }
    
    TInt collectionId( KErrNotFound );
    TInt error( KErrNone );
    TRAP( error, iCollectionManager->AddToCollectionL( collectionId, *newItemArray, ETrue ) );
    if( error != KErrNotSupported && error != KErrNone )
        {
        delete newItemArray;
        newItemArray = NULL;
        return error;
        }

    TRAP( error, iCollectionManager->AddToCollectionL( collectionId, *newItemArray, EFalse ) );
    if( error != KErrNotSupported && error != KErrNone )
        {
        delete newItemArray;
        newItemArray = NULL;
        return error;
        }

    delete newItemArray;
    newItemArray = NULL;

    _LIT( KMsg2, "Exit AddToCollectionL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCollectionManagerTest::RemoveFromCollectionL
// -----------------------------------------------------------------------------
//
TInt CCollectionManagerTest::RemoveFromCollectionL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter RemoveFromCollectionL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );

    if( !iCollectionManager )
        {
        return KErrUnknown;
        }
    
    const TInt itemCount( 500 );
    CDesCArray* removeItemArray = new (ELeave) CDesCArrayFlat( itemCount );

    for( TInt i = 0 ; i < itemCount ; ++i )
        {
        TBuf<40> b;
        b.Num( i );
        b.Append( _L("TestItem") );
        removeItemArray->AppendL( b );
        }

    TInt collectionId( KErrNotFound );
    TInt error( KErrNone );
    TRAP( error, iCollectionManager->RemoveFromCollectionL( collectionId, *removeItemArray ) );
    if( error != KErrNotSupported && error != KErrNone )
        {
        delete removeItemArray;
        removeItemArray = NULL;
        return error;
        }

    delete removeItemArray;
    removeItemArray = NULL;
    
    _LIT( KMsg2, "Exit RemoveFromCollectionL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCollectionManagerTest::RemoveFromAllCollectionsL
// -----------------------------------------------------------------------------
//
TInt CCollectionManagerTest::RemoveFromAllCollectionsL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "Enter RemoveFromAllCollectionsL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    if( !iCollectionManager )
        {
        return KErrUnknown;
        }
    
    const TInt itemCount( 500 );
    CDesCArray* removeItemArray = new (ELeave) CDesCArrayFlat( itemCount );
    CleanupStack::PushL( removeItemArray );

    for( TInt i = 0 ; i < itemCount ; ++i )
        {
        TBuf<40> b;
        b.Num( i );
        b.Append( _L("TestItem") );
        removeItemArray->AppendL( b );
        }
    
    TInt error( KErrNone );
    TRAP( error, iCollectionManager->RemoveFromAllCollectionsL( *removeItemArray ) );
    if( error != KErrNotSupported && error != KErrNone )
        {
        CleanupStack::PopAndDestroy( removeItemArray );
        removeItemArray = NULL;
        return error;
        }
    
    CleanupStack::PopAndDestroy( removeItemArray );
    removeItemArray = NULL;
    
    _LIT( KMsg2, "Exit RemoveFromAllCollectionsL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCollectionManagerTest::GetCollectionItemArrayL
// -----------------------------------------------------------------------------
//
TInt CCollectionManagerTest::GetCollectionItemArrayL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "Enter GetCollectionItemArrayL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
   
    if( !iCollectionManager )
        {
        return KErrUnknown;
        }
    
    CDesCArraySeg* collectionItemArray = new (ELeave) CDesCArraySeg( 4 );
    CleanupStack::PushL( collectionItemArray );

    TInt collectionId( KErrNotFound );
    TInt error( KErrNone );
    TRAP( error, iCollectionManager->GetCollectionItemArrayL( collectionId, *collectionItemArray ) );
    if( error != KErrNotSupported && error != KErrNone )
        {
        CleanupStack::PopAndDestroy(collectionItemArray);
        return error;
        }
    
    if( collectionItemArray->Count() > 0 )
        {
        CleanupStack::PopAndDestroy(collectionItemArray);
        return KErrUnknown;
        }
    
    CleanupStack::PopAndDestroy(collectionItemArray);
    
    _LIT( KMsg2, "Exit GetCollectionItemArrayL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCollectionManagerTest::GetCollectionIdByItemL
// -----------------------------------------------------------------------------
//
TInt CCollectionManagerTest::GetCollectionIdByItemL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter GetCollectionIdByItemL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );

    if( !iCollectionManager )
        {
        return KErrUnknown;
        }
    
    _LIT( KTestItemName, "GetCollectionIdByItemTestItem");
    HBufC* getCollectionIdByItemTestItemName = KTestItemName().AllocL();
    
    RArray<TInt> idArray;
    CleanupClosePushL( idArray );
    TInt error( KErrNone );
    TRAP( error, iCollectionManager->GetCollectionIdByItemL( *getCollectionIdByItemTestItemName, idArray ) );
    if( error != KErrNotSupported && error != KErrNone )
        {
        CleanupStack::PopAndDestroy(&idArray);
        delete getCollectionIdByItemTestItemName;
        getCollectionIdByItemTestItemName = NULL;
        return error;
        }

    const TInt idArrayCount( idArray.Count() );
    if( idArrayCount > 0 )
        {
        CleanupStack::PopAndDestroy(&idArray);
        delete getCollectionIdByItemTestItemName;
        getCollectionIdByItemTestItemName = NULL;
        return KErrUnknown;
        }

    CleanupStack::PopAndDestroy(&idArray);
	
    delete getCollectionIdByItemTestItemName;
    getCollectionIdByItemTestItemName = NULL;
    
    _LIT( KMsg2, "Enter GetCollectionIdByItemL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCollectionManagerTest::AddCollectionObserverL
// -----------------------------------------------------------------------------
//
TInt CCollectionManagerTest::AddCollectionObserverL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter AddCollectionObserverL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );

    if( !iCollectionManager )
        {
        return KErrUnknown;
        }
    
    TInt err( KErrNone );
    TRAP( err, iCollectionManager->AddCollectionObserverL( *this ) );
    if( err != KErrNotSupported && err != KErrNone )
        {
        return err;
        }
    
    _LIT( KMsg2, "Exit AddCollectionObserverL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCollectionManagerTest::RemoveCollectionObserverL
// -----------------------------------------------------------------------------
//
TInt CCollectionManagerTest::RemoveCollectionObserverL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter RemoveCollectionObserverL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
 
    if( !iCollectionManager )
        {
        return KErrUnknown;
        }
    
    TInt err( KErrNone );
    TRAP( err, iCollectionManager->AddCollectionObserverL( *this ) );
    if( err != KErrNotSupported && err != KErrNone )
        {
        return err;
        }
    
    TRAP( err, iCollectionManager->RemoveCollectionObserverL( *this ) );
    if( err != KErrNotSupported && err != KErrNone )
        {
        return err;
        }
    
    _LIT( KMsg2, "Exit RemoveCollectionObserverL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
	
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCollectionManagerTest::GetDriveByIdL
// -----------------------------------------------------------------------------
//
TInt CCollectionManagerTest::GetDriveByIdL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter GetDriveByIdL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    if( !iCollectionManager )
        {
        return KErrUnknown;
        }
    
    TInt collectionId( KErrNotFound );

    TInt32 drive( 100 );
    TInt error( KErrNone );
    
    TRAP( error, drive = iCollectionManager->GetDriveByIdL( collectionId ) );
    if( error != KErrNotSupported && error != KErrNone )
        {
        return error;
        }
    
    if( drive != 100 )
        {
        return KErrUnknown;
        }
    
    _LIT( KMsg2, "Exit GetDriveByIdL" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCollectionManagerTest::EndTestSessionL
// -----------------------------------------------------------------------------
//
TInt CCollectionManagerTest::EndTestSessionL( CStifItemParser& /* aItem */ )
    {
    delete iCollectionManager;
    iCollectionManager = NULL;
    
    return KErrNone;
    }   

//  [End of File] - Do not remove
