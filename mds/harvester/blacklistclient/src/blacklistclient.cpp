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
#include "blacklistclient.h"
#include "blacklistcommon.h"
#include "mdcserializationbuffer.h"
#include "blacklistitem.h"
#include "harvesterlog.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "blacklistclientTraces.h"
#endif



// ---------------------------------------------------------------------------
// RBlacklistClient::RBlacklistClient()
// ---------------------------------------------------------------------------
//
EXPORT_C RBlacklistClient::RBlacklistClient() : RSessionBase (),
    iSessionOk( EFalse )
    {
    }

// ---------------------------------------------------------------------------
// RBlacklistClient::~RBlacklistClient()
// ---------------------------------------------------------------------------
//
EXPORT_C RBlacklistClient::~RBlacklistClient()
    {
    WRITELOG( "RBlacklistClient::~RBlacklistClient - begin" );
    OstTrace0( TRACE_NORMAL, RBLACKLISTCLIENT_RBLACKLISTCLIENT, "RBlacklistClient::~RBlacklistClient -begin" );
    
    RSessionBase::Close(); 
    iBlacklistMemoryTable.ResetAndDestroy();
    iBlacklistMemoryTable.Close();
	iBlacklistChunk.Close();
	iFs.Close();

    WRITELOG( "RBlacklistClient::~RBlacklistClient - end" );
    OstTrace0( TRACE_NORMAL, DUP1_RBLACKLISTCLIENT_RBLACKLISTCLIENT, "RBlacklistClient::~RBlacklistClient - end" );
    
    }

// ---------------------------------------------------------------------------
// RBlacklistClient::Connect()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RBlacklistClient::Connect()
    {
    WRITELOG( "RBlacklistClient::Connect - begin" );
    OstTrace0( TRACE_NORMAL, RBLACKLISTCLIENT_CONNECT, "RBlacklistClient::Connect - begin" );
    
    iSessionOk = EFalse;
    
    TInt error = iFs.Connect();
    
    if( error != KErrNone )
        {
        return error;
        }
      
    error = StartServer();

    if ( error == KErrNone || error == KErrAlreadyExists )
        {
        WRITELOG( "RBlacklistClient::Connect() - creating session" );
        error = CreateSession( KBlacklistServerName, Version() );
        }
    
    if( error == KErrNone )
        {
        iSessionOk = ETrue;
        }
    else
        {
        iFs.Close();
        }
    
    WRITELOG( "RBlacklistClient::Connect - end" );
    OstTrace0( TRACE_NORMAL, DUP1_RBLACKLISTCLIENT_CONNECT, "RBlacklistClient::Connect - end" );
    
    return error;
    }

// ---------------------------------------------------------------------------
// RBlacklistClient::Version()
// ---------------------------------------------------------------------------
//
TVersion RBlacklistClient::Version() const
    {
    WRITELOG( "RBlacklistClient::Version - begin" );
    OstTrace0( TRACE_NORMAL, RBLACKLISTCLIENT_VERSION, "RBlacklistClient::Version - begin" );
    
    return TVersion( KBlacklistServerMajorVersion, KBlacklistServerMinorVersion,
    	KBlacklistServerBuildVersion );

    }

// ---------------------------------------------------------------------------
// RBlacklistClient::StartServer()
// ---------------------------------------------------------------------------
//
TInt RBlacklistClient::StartServer()
    {
    WRITELOG( "RBlacklistClient::StartServer - begin" );
    OstTrace0( TRACE_NORMAL, RBLACKLISTCLIENT_STARTSERVER, "RBlacklistClient::StartServer - begin" );
    
    RProcess server;
    TInt error = server.Create( KBlacklistServerExe, KNullDesC );
    if( error != KErrNone )
        {
        return error;
        }
    
    // start server and wait for signal before proceeding    
    TRequestStatus status;
    server.Rendezvous( status );
    if ( status.Int() != KRequestPending )
        {
        server.Kill( 0 );
        }
    else
        {
        server.Resume();
        }

    User::WaitForRequest( status );
    error = server.ExitType() == EExitPanic ? KErrCommsBreak : status.Int();
    server.Close();

    WRITELOG( "RBlacklistClient::StartServer - end" );
    OstTrace0( TRACE_NORMAL, DUP1_RBLACKLISTCLIENT_STARTSERVER, "RBlacklistClient::StartServer - end" );
    
    return error;
    }

// ---------------------------------------------------------------------------
// RBlacklistClient::RemoveFromDBL()
// ---------------------------------------------------------------------------
//
void RBlacklistClient::RemoveFromDBL( const TDesC& aUri, TUint32 aMediaId ) const
    {
    WRITELOG( "RBlacklistClient::RemoveFromDBL - begin" );
    OstTrace0( TRACE_NORMAL, RBLACKLISTCLIENT_REMOVEFROMDBL, "RBlacklistClient::RemoveFromDBL - begin" );
    
    TPckgBuf<TUint32> mediaIdPckg( aMediaId );
    
    TIpcArgs ipcArgs;
    ipcArgs.Set( 1, &aUri );
    ipcArgs.Set( 2, &mediaIdPckg );
 
    const TInt err = SendReceive( EBlacklistRemoveFromDB, ipcArgs );
    User::LeaveIfError( err );

    WRITELOG( "RBlacklistClient::RemoveFromDBL - end" );
    OstTrace0( TRACE_NORMAL, DUP1_RBLACKLISTCLIENT_REMOVEFROMDBL, "RBlacklistClient::RemoveFromDBL end" );
    
    }

// ---------------------------------------------------------------------------
// RBlacklistClient::DoLoadBlacklistL()
// ---------------------------------------------------------------------------
//
void RBlacklistClient::DoLoadBlacklistL( TInt& aHandle ) const
    {
    WRITELOG( "RBlacklistClient::DoLoadBlacklistL - begin" );
    OstTrace0( TRACE_NORMAL, RBLACKLISTCLIENT_DOLOADBLACKLISTL, "RBlacklistClient::DoLoadBlacklistL - begin" );
    
    TPckgBuf<TInt> handleBuf;
    TIpcArgs ipcArgs;
    ipcArgs.Set( 1, &handleBuf );
    const TInt err = SendReceive( EGetBlacklistData, ipcArgs );
    User::LeaveIfError( err );
    aHandle = handleBuf();

    WRITELOG( "RBlacklistClient::DoLoadBlacklistL - end" );
    OstTrace0( TRACE_NORMAL, DUP1_RBLACKLISTCLIENT_DOLOADBLACKLISTL, "RBlacklistClient::DoLoadBlacklistL - end" );
    
    } 

// ---------------------------------------------------------------------------
// RBlacklistClient::LoadBlacklistL()
// ---------------------------------------------------------------------------
//
EXPORT_C void RBlacklistClient::LoadBlacklistL()
    {
    WRITELOG( "RBlacklistClient::LoadBlacklistL - begin" );
    OstTrace0( TRACE_NORMAL, RBLACKLISTCLIENT_LOADBLACKLISTL, "RBlacklistClient::LoadBlacklistL - begin" );
    
    if ( !iSessionOk )
        {
        return;
        }

    // delete old data
    if( iBlacklistMemoryTable.Count() > 0 )
        {
        iBlacklistMemoryTable.ResetAndDestroy();
        }
    
    // Get handle to data
    TInt handle( 0 );
    DoLoadBlacklistL( handle );
    
   	// create memory chunk
   	HBufC*  name = HBufC::NewLC( 32 );
	*name = KBlacklistChunkName;
	name->Des().AppendNum( handle );
	iBlacklistChunk.Close();

	User::LeaveIfError ( iBlacklistChunk.OpenGlobal( *name, ETrue ) );

	CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( iBlacklistChunk.Base(), iBlacklistChunk.Size() );

	if ( buffer->Size() == 0 )
	    {
	    User::Leave( KErrNotFound );
	    }
	
	// First get list count
	TUint32 listCount ( 0 );
	buffer->ReceiveL( listCount );
	
	TInt64 modified ( 0 );
	TUint32 mediaId ( 0 );
    HBufC* uri = NULL;
    
    for( TInt i = listCount - 1; i >=0; i-- )
        {
        // get modified and media id
	    buffer->ReceiveL( modified );
	    buffer->ReceiveL( mediaId );
        
        //Get uri
        uri = buffer->ReceiveDes16L();
       	CleanupStack::PushL( uri );
       	AddToMemoryTableL( modified, *uri, mediaId );
       	CleanupStack::PopAndDestroy( uri );
       	uri = NULL;
        }
	
	CleanupStack::PopAndDestroy( buffer );
	CleanupStack::PopAndDestroy( name );

    WRITELOG( "RBlacklistClient::LoadBlacklistL - end" );
    OstTrace0( TRACE_NORMAL, DUP1_RBLACKLISTCLIENT_LOADBLACKLISTL, "RBlacklistClient::LoadBlacklistL - end" );
    
    }

// ---------------------------------------------------------------------------
// RBlacklistClient::AddToMemoryTableL()
// ---------------------------------------------------------------------------
//
void RBlacklistClient::AddToMemoryTableL( const TInt64& aModified,
        const TDesC& aUri, const TUint32 aMediaId )
    {
    WRITELOG( "RBlacklistClient::AddToMemoryTableL - begin" );
    OstTrace0( TRACE_NORMAL, RBLACKLISTCLIENT_ADDTOMEMORYTABLEL, "RBlacklistClient::AddToMemoryTableL - begin" );
    
    CBlacklistItem* item = CBlacklistItem::NewL( aModified, aUri, aMediaId );
    
    const TInt err = iBlacklistMemoryTable.Append( item ); // ownership is transferred
    if ( err != KErrNone )
        {
        delete item;
        item = NULL;
        }


    WRITELOG( "RBlacklistClient::AddToMemoryTableL - end" );
    OstTrace0( TRACE_NORMAL, DUP1_RBLACKLISTCLIENT_ADDTOMEMORYTABLEL, "RBlacklistClient::AddToMemoryTableL - end" );
    
    }

// ---------------------------------------------------------------------------
// RBlacklistClient::RemoveFromMemoryTableL()
// ---------------------------------------------------------------------------
//
void RBlacklistClient::RemoveFromMemoryTableL( const TDesC& aUri, const TUint32 aMediaId )
    {
    WRITELOG( "RBlacklistClient::RemoveFromMemoryTableL - begin" );
    OstTrace0( TRACE_NORMAL, RBLACKLISTCLIENT_REMOVEFROMMEMORYTABLEL, "RBlacklistClient::RemoveFromMemoryTableL - begin" );
    
    const TInt index = GetListIndex( aUri, aMediaId );
    if ( index >= 0 )
        {
        CBlacklistItem* item = iBlacklistMemoryTable[index];
        delete item;
        item = NULL;
        iBlacklistMemoryTable.Remove( index );
        }

    WRITELOG( "RBlacklistClient::RemoveFromMemoryTableL - end" );
    OstTrace0( TRACE_NORMAL, DUP1_RBLACKLISTCLIENT_REMOVEFROMMEMORYTABLEL, "RBlacklistClient::RemoveFromMemoryTableL - end" );
    
    }
 

// ---------------------------------------------------------------------------
// RBlacklistClient::IsBlacklistedL()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool RBlacklistClient::IsBlacklistedL( const TDesC& aUri, TUint32 aMediaId, TTime /*aLastModifiedTime*/ )
    {
    WRITELOG( "RBlacklistClient::IsBlacklistedL - begin" );
    OstTrace0( TRACE_NORMAL, RBLACKLISTCLIENT_ISBLACKLISTEDL, "RBlacklistClient::IsBlacklistedL - begin" );
    

    if ( !iSessionOk )
        {
        return EFalse;
        }
    
    const TInt index = GetListIndex( aUri, aMediaId );
    if ( index >= 0 )
        {
        TEntry entry;
        const TInt errorcode = iFs.Entry( aUri, entry );
        
        if ( errorcode != KErrNone )
            {
            return EFalse;
            }
        
        TTime fileLastModified( 0 );
        fileLastModified = entry.iModified;
    
        TInt64 modified( 0 );
        modified = iBlacklistMemoryTable[index]->Modified();
        
        if( modified > 0 )
            {
            if ( modified == fileLastModified.Int64() )
                {
                WRITELOG( "RBlacklistClient::IsBlacklistedL - file is blacklisted, modification time is the same" );
                OstTrace0( TRACE_NORMAL, DUP1_RBLACKLISTCLIENT_ISBLACKLISTEDL, "RBlacklistClient::IsBlacklistedL - file is blacklisted, modification time is the same" );
                
                return ETrue;
                }
            else
                {
                // file might be different, so remove from blacklist
                // and act like it wasn't found
                RemoveFromMemoryTableL( aUri, aMediaId );
                
                // Remove from server DB
                RemoveFromDBL( aUri, aMediaId );
                }
            }
        else
            {
            WRITELOG( "RBlacklistClient::IsBlacklistedL - file is blacklisted, no modification time found" );
            OstTrace0( TRACE_NORMAL, DUP2_RBLACKLISTCLIENT_ISBLACKLISTEDL, "RBlacklistClient::IsBlacklistedL - file is blacklisted, no modification time found" );
            
            return ETrue;
            }
        
        }
   
    WRITELOG( "RBlacklistClient::IsBlacklistedL - end" );
    OstTrace0( TRACE_NORMAL, DUP3_RBLACKLISTCLIENT_ISBLACKLISTEDL, "RBlacklistClient::IsBlacklistedL - end" );
    
    return EFalse;
    }

// ---------------------------------------------------------------------------
// RBlacklistClient::GetListIndex()
// ---------------------------------------------------------------------------
//
TInt RBlacklistClient::GetListIndex( const TDesC& aUri, TUint32 aMediaId )
    {
    WRITELOG( "RBlacklistClient::GetListIndex - begin" );
    OstTrace0( TRACE_NORMAL, RBLACKLISTCLIENT_GETLISTINDEX, "RBlacklistClient::GetListIndex - begin" );
    
    for( TInt i = iBlacklistMemoryTable.Count() - 1; i >=0; i-- )
        {
        if ( iBlacklistMemoryTable[i]->Compare( aUri, aMediaId ) )
            {
            return i;
            }
        }

    WRITELOG( "RBlacklistClient::GetListIndex - end" );
    OstTrace0( TRACE_NORMAL, DUP1_RBLACKLISTCLIENT_GETLISTINDEX, "RBlacklistClient::GetListIndex - end" );
    
    return KErrNotFound;
    }


// ---------------------------------------------------------------------------
// RBlacklistClient::AddL()
// ---------------------------------------------------------------------------
//
EXPORT_C void RBlacklistClient::AddL( const TDesC& aUri, TUint32 aMediaId, TTime aLastModifiedTime ) const
    {
    WRITELOG( "RBlacklistClient::AddL - begin" );
    OstTrace0( TRACE_NORMAL, RBLACKLISTCLIENT_ADDL, "RBlacklistClient::AddL - begin" );
    
    TPckgC<TUint32> mediaIdPckg( aMediaId );
    TPckgC<TTime> lastModifiedTimePckg( aLastModifiedTime );

	TIpcArgs ipcArgs;
    ipcArgs.Set( 0, &mediaIdPckg );
    ipcArgs.Set( 1, &aUri );
    ipcArgs.Set( 2, &lastModifiedTimePckg );
 
    const TInt err = SendReceive( EBlacklistAdd, ipcArgs );
    User::LeaveIfError( err );

    WRITELOG( "RBlacklistClient::AddL - end" );
    OstTrace0( TRACE_NORMAL, DUP1_RBLACKLISTCLIENT_ADDL, "RBlacklistClient::AddL - end" );
    }


// ---------------------------------------------------------------------------
// RBlacklistClient::RemoveL()
// ---------------------------------------------------------------------------
//
EXPORT_C void RBlacklistClient::RemoveL( const TDesC& aUri, TUint32 aMediaId ) const
    {
    WRITELOG( "RBlacklistClient::RemoveL - begin" );
    OstTrace0( TRACE_NORMAL, RBLACKLISTCLIENT_REMOVEL, "RBlacklistClient::RemoveL - begin" );
    
    TPckgBuf<TUint32> mediaIdPckg( aMediaId );
    
    TIpcArgs ipcArgs;
    ipcArgs.Set( 1, &aUri );
    ipcArgs.Set( 2, &mediaIdPckg );
 
    const TInt err = SendReceive( EBlacklistRemove, ipcArgs );
    User::LeaveIfError( err );

    WRITELOG( "RBlacklistClient::RemoveL - end" );
    OstTrace0( TRACE_NORMAL, DUP1_RBLACKLISTCLIENT_REMOVEL, "RBlacklistClient::RemoveL - end" );
    }

// ---------------------------------------------------------------------------
// RBlacklistClient::CloseDBL()
// ---------------------------------------------------------------------------
//
EXPORT_C void RBlacklistClient::CloseDBL()
    {
    WRITELOG( "RBlacklistClient::CloseDBL - begin" );
    OstTrace0( TRACE_NORMAL, RBLACKLISTCLIENT_CLOSEDBL, "RBlacklistClient::CloseDBL - begin" );
    
    if ( !iSessionOk )
        {
        User::Leave( KErrDisconnected );
        }
    else
        {
        SendReceive( EBlacklistCloseDB );
        }

    WRITELOG( "RBlacklistClient::CloseDBL - end" );
    OstTrace0( TRACE_NORMAL, DUP1_RBLACKLISTCLIENT_CLOSEDBL, "RBlacklistClient::CloseDBL- end" );
    
    }

// ----------------------------------------------------------------------------------------
// Close
// ----------------------------------------------------------------------------------------
//
EXPORT_C void RBlacklistClient::Close()
    {
    WRITELOG( "RBlacklistClient::Close" );

    iFs.Close();
    iBlacklistChunk.Close();
    
    RSessionBase::Close();
    }

// End of File
