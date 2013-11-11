/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  harvester server session
*
*/

#include <e32std.h>
#include <e32svr.h>
#include <bamdesca.h>
#include <mpxlog.h>
#include <mpxuser.h>
#include <mpxcmn.h>
#include <mpxmedia.h>
#include "mpxharvestercommon.h"
#include "mpxharvestersession.h"
#include "mpxharvesterserver.h"
#include "mpxharvesterengine.h"

// ============================ LOCAL FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Panic client
// ----------------------------------------------------------------------------
//
LOCAL_C void PanicClient(const RMessage2 &aMessage,TInt aPanic)
    {
    _LIT(KTxtServer,"Harvester Server Session");
    aMessage.Panic(KTxtServer,aPanic);
    }

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CMPXHarvesterSession* CMPXHarvesterSession::NewL(
    CMPXHarvesterServer& aServer,
    CMPXHarvesterEngine& aEngine)
    {
    CMPXHarvesterSession* s = new(ELeave)CMPXHarvesterSession(aServer, aEngine);
    CleanupStack::PushL(s);
    s->ConstructL();
    CleanupStack::Pop(s);
    return s;
    }

// ----------------------------------------------------------------------------
// C++ constructor can NOT contain any code that might leave.
// ----------------------------------------------------------------------------
//
CMPXHarvesterSession::CMPXHarvesterSession(CMPXHarvesterServer& aServer,
                                           CMPXHarvesterEngine& aEngine)
    : iServer(aServer), iEngine(aEngine)
    {
    iServer.AddClient();
    }

// ----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CMPXHarvesterSession::ConstructL()
    {
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CMPXHarvesterSession::~CMPXHarvesterSession()
    {
    iServer.RemoveClient();
    delete iDes;
    delete iDes8;
    delete iBuffer;
    delete iMedia;

    iMessages.ResetAndDestroy();
    iMessages.Close();
    }

// ----------------------------------------------------------------------------
// Service request
// ----------------------------------------------------------------------------
//
void CMPXHarvesterSession::ServiceL(const RMessage2& aMessage)
    {
    // Default ServiceErrorL() will complete the message if ServiceL() leaves
    DispatchMessageL(aMessage);
    }

// ----------------------------------------------------------------------------
// Handle Import Complete
// ----------------------------------------------------------------------------
//
void CMPXHarvesterSession::HandleImportCompleteL( CMPXMedia* aMedia,
                                                  TInt aErr,
                                                  RHvsMsg* aMsg )
    {
    TInt r(0);
    if( aErr == KErrNone )
        {
        // Keep media alive
        delete iMedia;
        iMedia = NULL;
        iMedia = CMPXMedia::NewL( *aMedia );

        // Serialize the media property to iBuffer
        ::CreateBufferL<CMPXMedia>( *aMedia, iBuffer );
        r = iBuffer->Size();
        }
    CompleteMessage( aMsg, aErr < 0 ? aErr : r );
    }

// ----------------------------------------------------------------------------
// Handle Export Complete
// ----------------------------------------------------------------------------
//
void CMPXHarvesterSession::HandleExportCompleteL( CMPXMedia* aMedia,
                                                  TInt aErr,
                                                  RHvsMsg* aMsg )
    {
    TInt r(0);
    if( aErr == KErrNone )
        {
         // Keep media alive
        delete iMedia;
        iMedia = NULL;

        iMedia = CMPXMedia::NewL( *aMedia );

        // Serialize the media property to iBuffer
        ::CreateBufferL<CMPXMedia>( *aMedia, iBuffer );
        r = iBuffer->Size();
        }
    CompleteMessage( aMsg, aErr < 0 ? aErr : r );
    }

// ----------------------------------------------------------------------------
// Handles task completion with error
// ----------------------------------------------------------------------------
//
void CMPXHarvesterSession::CompleteTask( RHvsMsg* aMsg, TInt aErr )
    {
    CompleteMessage( aMsg, aErr );
    }

// ----------------------------------------------------------------------------
// Message dispatcher
// ----------------------------------------------------------------------------
//
void CMPXHarvesterSession::DispatchMessageL(const RMessage2& aMessage)
    {
    MPX_DEBUG1(_L("CMPXHarvesterSession::DispatchMessageL <---"));

    TInt op(aMessage.Function());

    switch(op)
        {
        case EHvsServerGetBuffer:
            {
            ASSERT( iBuffer );
            aMessage.WriteL(0,iBuffer->Ptr(0));
            delete iBuffer;
            iBuffer = NULL;
            aMessage.Complete( KErrNone );
            break;
            }
        case EHvsServerScan:
            {
            DoScanL();
            aMessage.Complete( KErrNone );
            break;
            }
        case EHvsServerCancelScan:
            {
            aMessage.Complete( KErrNone );
            DoCancelScan();
            break;
            }
        case EHvsServerShutdown:
            {
            DoShutdown();
            aMessage.Complete( KErrNone );
            break;
            }
        case EHvsServerAddFile:
            {
            ReadDesL( aMessage, 0 ); // file path
            RHvsMsg* msg = SetAsyncL( aMessage, (TMPXHarvesterServerOp) op );
            DoAddFileL( msg );
            break;
            }
        case EHvsServerAddFileMedia:
            {
            TInt r = DoAddMediaPropL( aMessage );
            aMessage.Complete( r );
            break;
            }
        case EHvsServerGetMedia:
            {
            ReadDesL( aMessage, 0 ); // file path
            RHvsMsg* msg = SetAsyncL( aMessage, (TMPXHarvesterServerOp) op );
            DoGetMediaL( msg );
            break;
            }
        case EHvsServerGetColUid:
            {
            TInt r = DoGetColUidL( aMessage );
            aMessage.Complete( r );
            break;
            }
        case EHvsServerRemoveFile:
            {
            ReadDesL( aMessage, 0 ); // file path
            TPckgBuf<TInt> del;
            aMessage.Read( 1, del);

            TInt r = iEngine.RemoveFileL( *iDes, del() );
            aMessage.Complete( r );
            break;
            }
        case EHvsServerRemoveFiles:
            {
            DoRemoveMultipleFilesL( aMessage );
            aMessage.Complete( KErrNone );
            break;
            }

        case EHvsServerRemoveAllFiles:
            {
            iEngine.RemoveAllFilesL();
            aMessage.Complete( KErrNone );
            break;
            }
        case EHvsServerUpdateFile:
            {
            DoUpdateFileL( aMessage );
            aMessage.Complete( KErrNone );
            break;
            }
        case EHvsServerRenameFile:
            {
            DoRenameFileL( aMessage );
            aMessage.Complete( KErrNone );
            break;
            }
        case EHvsServerExportPlaylist:
            {
            // Async, complete later
            RHvsMsg* msg = SetAsyncL( aMessage, (TMPXHarvesterServerOp) op );
            TRAPD( err,  iEngine.ExportPlaylistL( msg, this ) );
            if( err != KErrNone )
                {
                CompleteMessage( msg, err );
                }
            break;
            }
        case EHvsServerImportPlaylist:
            {
            RHvsMsg* msg = SetAsyncL( aMessage, (TMPXHarvesterServerOp) op );
            TRAPD( err, iEngine.ImportPlaylistL( msg, this ) );
            if( err != KErrNone )
                {
                CompleteMessage( msg, err );
                }
            break;
            }
        case EHvsServerFindColID:
            {
            ReadDesL( aMessage, 0 ); // file path
            TInt r=iEngine.FindCollectionIdL( *iDes );
            aMessage.Complete( r );
            break;
            }
        case EHvsServerReqAttr:
            {
            TPckgBuf<TInt> playlistType;
            aMessage.Read(0, playlistType );
            TArray<TMPXAttribute> requiredAttributes=iEngine.RequiredAttributesL( playlistType() );
            ::CreateBufferL<TMPXAttribute>( requiredAttributes, iBuffer ); //lint !e1024 !e1703 !e1514 !e64
            aMessage.Complete( iBuffer->Size() );
            break;
            }
        case EHvsServerOptAttr:
            {
            TPckgBuf<TInt> playlistType;
            aMessage.Read(0, playlistType );
            TArray<TMPXAttribute> optionalAttributes=iEngine.OptionalAttributesL( playlistType() );
            ::CreateBufferL<TMPXAttribute>( optionalAttributes, iBuffer ); //lint !e1024 !e1703 !e1514 !e64
            aMessage.Complete( iBuffer->Size() );
            break;
            }
        case EHvsServerPlaylistFileExt:
            {
            TPckgBuf<TInt> playlistType;
            aMessage.Read(0, playlistType );
            const TDesC& fileExtension=iEngine.PlaylistFileExtensionL( playlistType() );
            aMessage.WriteL(1,fileExtension);
            aMessage.Complete( KErrNone );
            break;
            }
        case EHvsServerIsPlaylist:
            {
            ReadDesL( aMessage, 0 ); // file path
            TBool r=iEngine.IsPlaylistL( *iDes );
            aMessage.Complete( r );
            break;
            }
        case EHvsServerImportFile:
            {
            RHvsMsg* msg = SetAsyncL( aMessage, (TMPXHarvesterServerOp) op );
            TRAPD( err, iEngine.ImportFileL( msg, this ) );
            if( err != KErrNone )
                {
                CompleteMessage( msg, err );
                }
            break;
            }
        case  EHvsServerReCreateDB:
            {
            iEngine.RecreateDatabases();
            aMessage.Complete( KErrNone );
            break;
            }
        case EHvsServerCheckSystemEvent:
            {
            iEngine.CheckForSystemEvents();
            aMessage.Complete( KErrNone );
            break;
            }
        case EHvsServerCloseTransaction:
            {
            TRAPD( err, iEngine.CloseTransactionL() );
            aMessage.Complete( err );
            break;
            }
        default:
            PanicClient(aMessage, KErrNotFound);
            break;
        }

    MPX_DEBUG1(_L("CMPXHarvesterSession::DispatchMessageL --->"));
    }

// ----------------------------------------------------------------------------
// Start scanning for new files
// ----------------------------------------------------------------------------
//
void CMPXHarvesterSession::DoScanL()
    {
    MPX_DEBUG1(_L("CMPXHarvesterSession::DoScanL <---"));
    iEngine.ScanL();
    MPX_DEBUG1(_L("CMPXHarvesterSession::DoScanL --->"));
    }

// ----------------------------------------------------------------------------
// Cancel a scanning operation
// ----------------------------------------------------------------------------
//
void CMPXHarvesterSession::DoCancelScan()
    {
    MPX_DEBUG1(_L("CMPXHarvesterSession::DoCancelScanL <---"));
    iEngine.CancelScan();
    MPX_DEBUG1(_L("CMPXHarvesterSession::DoCancelScanL --->"));
    }

// ----------------------------------------------------------------------------
// Shutdown the server, only used for memory leak testing
// ----------------------------------------------------------------------------
//
void CMPXHarvesterSession::DoShutdown()
    {
    MPX_DEBUG1(_L("CMPXHarvesterSession::DoShutdown <---"));
    CActiveScheduler::Stop();
    MPX_DEBUG1(_L("CMPXHarvesterSession::DoShutdown --->"));
    }

// ----------------------------------------------------------------------------
// Handles adding a file
// ----------------------------------------------------------------------------
//
void CMPXHarvesterSession::DoAddFileL( RHvsMsg* aMsg )
    {
    MPX_DEBUG1(_L("CMPXHarvesterSession::DoAddFileL <---"));
    iEngine.AddFileL( aMsg ,*iDes, this );
    MPX_DEBUG1(_L("CMPXHarvesterSession::DoAddFileL <---"));
    }

// ----------------------------------------------------------------------------
// Handle adding a media property
// ----------------------------------------------------------------------------
//
TInt CMPXHarvesterSession::DoAddMediaPropL( const RMessage2& aMessage )
    {
    MPX_DEBUG1(_L("CMPXHarvesterSession::DoAddMediaPropL <---"));
    CMPXMedia* prop( NULL );
    ::NewFromMessageL<CMPXMedia>( aMessage, 0, prop );

    CleanupStack::PushL( prop );
    TInt r = iEngine.AddFileL( prop );
    CleanupStack::PopAndDestroy( prop );

    MPX_DEBUG1(_L("CMPXHarvesterSession::DoAddMediaPropL --->"));
    return r;
    }

// ----------------------------------------------------------------------------
// Handles media creation for a file
// ----------------------------------------------------------------------------
//
void CMPXHarvesterSession::DoGetMediaL( RHvsMsg* aMsg )
    {
    MPX_DEBUG1(_L("CMPXHarvesterSession::DoGetMediaL <---"));
    iEngine.GetMediaL( aMsg ,*iDes, this );
    MPX_DEBUG1(_L("CMPXHarvesterSession::DoGetMediaL --->"));
    }

// ----------------------------------------------------------------------------
// Handle getting collection id
// ----------------------------------------------------------------------------
//
TInt CMPXHarvesterSession::DoGetColUidL( const RMessage2& aMessage )
    {
    MPX_DEBUG1(_L("CMPXHarvesterSession::DoGetColUidL <---"));

    HBufC* buf = HBufC::NewLC(aMessage.GetDesLengthL(0));
    TPtr ptr = buf->Des();
    aMessage.Read(0,ptr);

    TInt r = iEngine.GetColUidL( *buf );
    CleanupStack::PopAndDestroy( buf );

    MPX_DEBUG1(_L("CMPXHarvesterSession::DoGetColUidL --->"));
    return r;
    }

// ----------------------------------------------------------------------------
// Remove multiple songs from the harvester db
// ----------------------------------------------------------------------------
//
void CMPXHarvesterSession::DoRemoveMultipleFilesL( const RMessage2& aMessage )
    {
    MPX_DEBUG1(_L("CMPXHarvesterSession::DoRemoveMultipleSongsL <---"));

    // Buffer with the array
    //
    CDesCArray* array( NULL );
    CBufBase* buffer( NULL );
    MPXUser::RecreateBufferL(aMessage.GetDesLengthL(0), buffer);
    CleanupStack::PushL( buffer );
    TPtr8 ptr = buffer->Ptr(0);
    User::LeaveIfError( aMessage.Read( 0, ptr ) );
    MPXUser::CreateFromBufferL( *buffer, array );
    CleanupStack::PopAndDestroy( buffer );

    if( array )
        {
        CleanupStack::PushL( array );

        // Check if we want to delete the files
        //
        TPckgBuf<TInt> del;
        aMessage.Read( 1, del );
        TPckgBuf<TInt> endTransaction;
        TInt error = aMessage.Read( 2, endTransaction );
        TInt endDbTransaction(1); // If endTranscation is not sent, assume True to commit the db.
        if ( !error )
            {
            endDbTransaction = endTransaction();
            }
        // Engine to do the work
        //
        iEngine.RemoveMultipleFilesL(*array, del(), endDbTransaction );

        CleanupStack::PopAndDestroy( array );
        }
    else
        {
        // Array was NULL;
        User::Leave( KErrArgument );
        }

    MPX_DEBUG1(_L("CMPXHarvesterSession::DoRemoveMultipleSongsL --->"));
    }

// ----------------------------------------------------------------------------
// Unpacks an RMessage2 and calls update functions in the engine
// ----------------------------------------------------------------------------
//
void CMPXHarvesterSession::DoUpdateFileL( const RMessage2& aMessage )
    {
     // Path of the object
    HBufC* buf = HBufC::NewLC(aMessage.GetDesLengthL(0));
    TPtr ptr = buf->Des();
    aMessage.Read(0,ptr);

    // New collection ID
    TPckgBuf<TInt> collection;
    aMessage.Read(1, collection );

    iEngine.UpdateFileL( *buf, collection() );
    CleanupStack::PopAndDestroy( buf );
    }

// ----------------------------------------------------------------------------
// Unpacks an RMessage2 and calls rename function in the engine
// ----------------------------------------------------------------------------
//
void CMPXHarvesterSession::DoRenameFileL( const RMessage2& aMessage )
    {
    // Path of the object
    HBufC* oldPath = HBufC::NewLC(aMessage.GetDesLengthL(0));
    TPtr oldPathPtr = oldPath->Des();
    aMessage.Read(0,oldPathPtr);

    // Path of the object
    HBufC* newPath = HBufC::NewLC(aMessage.GetDesLengthL(1));
    TPtr newPathPtr = newPath->Des();
    aMessage.Read(1,newPathPtr);

    // collection Id of the object
    TPckgBuf<TInt> collection;
    aMessage.Read(2, collection );

    iEngine.RenameFileL( *oldPath, *newPath, collection() );
    CleanupStack::PopAndDestroy( 2, oldPath ); // newPath, oldPath
    }

// ----------------------------------------------------------------------------
// Sets this message as asynchronous and adds it to internal list
// ----------------------------------------------------------------------------
//
RHvsMsg* CMPXHarvesterSession::SetAsyncL( const RMessage2& aMessage,
                                          TMPXHarvesterServerOp aOp )
    {
    RHvsMsg* msg = new(ELeave) RHvsMsg( aMessage, aOp );
    iMessages.Append( msg );
    return msg;
    }

// ----------------------------------------------------------------------------
// Complete an async message
// ----------------------------------------------------------------------------
//
void CMPXHarvesterSession::CompleteMessage( RHvsMsg*& aMessage,
                                            TInt aErr )
    {
    TInt count = iMessages.Count();
    for( TInt i=0; i<count; ++i )
        {
        if( aMessage == iMessages[i] )
            {
            // Ensures we complete the correct message!
            iMessages[i]->Message().Complete( aErr );
            delete iMessages[i];
            iMessages.Remove(i);
            break;
            }
        }
    }

// ----------------------------------------------------------------------------
// Reads a descriptor from the message, allocated into iDes
// ----------------------------------------------------------------------------
//
const TDesC& CMPXHarvesterSession::ReadDesL(const RMessage2& aMessage,
                                             TInt aMsgSlot)
    {
    TInt s=aMessage.GetDesLengthL(aMsgSlot);
    delete iDes;
    iDes=NULL;
    iDes=HBufC::NewL(s);
    TPtr p=iDes->Des();
    aMessage.ReadL(aMsgSlot,p);
    return *iDes;
    }

// END OF FILE

