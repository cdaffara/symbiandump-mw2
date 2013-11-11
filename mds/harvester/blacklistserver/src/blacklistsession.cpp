/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
// USER INCLUDE
#include "blacklistsession.h"
#include "blacklistserver.h"
#include "blacklistcommon.h"

// ---------------------------------------------------------------------------
//  CBlacklistSession::CBlacklistSession()
// ---------------------------------------------------------------------------
//
CBlacklistSession::CBlacklistSession()
    {
    }

// ---------------------------------------------------------------------------
//  CBlacklistSession::CreateL()
// ---------------------------------------------------------------------------
//
void CBlacklistSession::CreateL()
    {
    Server().AddSession();
    }

// ---------------------------------------------------------------------------
//  CBlacklistSession::~CBlacklistSession()
// ---------------------------------------------------------------------------
//
CBlacklistSession::~CBlacklistSession()
    {
    Server().RemoveSession();
    }

// ---------------------------------------------------------------------------
//  CBlacklistSession::Server()
// ---------------------------------------------------------------------------
//
CBlacklistServer& CBlacklistSession::Server()
    {
    return *static_cast<CBlacklistServer*>( const_cast<CServer2*>(CSession2::Server()) );
    }

// ---------------------------------------------------------------------------
//  CBlacklistSession::ServiceL()
// ---------------------------------------------------------------------------
//
void CBlacklistSession::ServiceL( const RMessage2& aMessage )
    {
     switch ( aMessage.Function() )
        {
        case EGetBlacklistData:
            {
            LoadBlacklistDataL( aMessage );
            break;
            }    
        case EBlacklistAdd:
            {
            AddL( aMessage );
            break;
            }    
        case EBlacklistRemove:
            {
            RemoveL( aMessage );
            break;
            }    
        case EBlacklistRemoveFromDB:
            {
            RemoveFromDBL( aMessage );
            break;
            }
        case EBlacklistCloseDB:
            {
            CloseDB( aMessage );
            break;
            }
        default:
            {
            aMessage.Complete( KErrNotSupported );
            break;
            }    
        }
    }

// ---------------------------------------------------------------------------
//  CBlacklistSession::LoadBlacklistDataL()
// ---------------------------------------------------------------------------
//
void CBlacklistSession::LoadBlacklistDataL( const RMessage2& aMessage ) 
    {
    Server().OpenDatabaseL();
    TPckgBuf<TInt> handleBuf( Server().MemoryHandle() );
    aMessage.WriteL( 1, handleBuf );
    aMessage.Complete( KErrNone );
    }

// ---------------------------------------------------------------------------
//  CBlacklistSession::AddL()
// ---------------------------------------------------------------------------
//
void CBlacklistSession::AddL( const RMessage2& aMessage ) 
    {
    TUint32 mediaId ( 0 );
    TPckg<TUint32> mediaIdPckg( mediaId );
    aMessage.ReadL( 0, mediaIdPckg );
 
 	RBuf uri;
 	User::LeaveIfError( uri.Create( aMessage.GetDesLength( 1 ) ) );
    CleanupClosePushL( uri );
    aMessage.ReadL( 1, uri );
   	
   	TTime lastModifiedTime ( 0 );
    TPckg<TTime> lastModifiedTimePckg( lastModifiedTime );
    aMessage.ReadL( 2, lastModifiedTimePckg );

    Server().AddL( mediaId, uri, lastModifiedTime );

    CleanupStack::PopAndDestroy( &uri );

    aMessage.Complete( KErrNone );
    }

// ---------------------------------------------------------------------------
//  CBlacklistSession::RemoveL()
// ---------------------------------------------------------------------------
//
void CBlacklistSession::RemoveL( const RMessage2& aMessage ) 
    {
 	RBuf uri;
    User::LeaveIfError( uri.Create( aMessage.GetDesLength( 1 ) ) );
    CleanupClosePushL( uri );
    aMessage.ReadL( 1, uri );

    TUint32 mediaId ( 0 );
    TPckg<TUint32> mediaIdPckg( mediaId );
    aMessage.ReadL( 2, mediaIdPckg );
 
    Server().RemoveL( mediaId, uri );

    CleanupStack::PopAndDestroy( &uri );
   
    aMessage.Complete( KErrNone );
    }

// ---------------------------------------------------------------------------
//  CBlacklistSession::RemoveFromDBL()
// ---------------------------------------------------------------------------
//
void CBlacklistSession::RemoveFromDBL( const RMessage2& aMessage ) 
    {
 	RBuf uri;
 	User::LeaveIfError( uri.Create( aMessage.GetDesLength( 1 ) ) );
    CleanupClosePushL( uri );
    aMessage.ReadL( 1, uri );

    TUint32 mediaId ( 0 );
    TPckg<TUint32> mediaIdPckg( mediaId );
    aMessage.ReadL( 2, mediaIdPckg );

    Server().RemoveFromDBL( uri, mediaId ); 

    CleanupStack::PopAndDestroy( &uri );  

    aMessage.Complete( KErrNone );
    }

// ---------------------------------------------------------------------------
//  CBlacklistSession::CloseDB()
// ---------------------------------------------------------------------------
//
void CBlacklistSession::CloseDB( const RMessage2& aMessage ) 
    {
    Server().CloseDB();
    if( !aMessage.IsNull() )
        {
        aMessage.Complete( KErrNone );
        }
    }

// End of File
