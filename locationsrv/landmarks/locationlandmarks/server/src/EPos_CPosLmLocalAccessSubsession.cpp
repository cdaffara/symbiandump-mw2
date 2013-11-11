/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: subsession class for operations related to local access.
*
*
*/


#include <e32base.h>
#include <s32mem.h>

#include <epos_poslmdatabaseutility.h>
#include <epos_cposlmdiskutilities.h>
#include <epos_cposlmlocaldbaccess.h>

#include "epos_lmdebug.h"
#include "EPos_CPosLmServer.h"
#include "EPos_CPosLmOperationManager.h"
#include "EPos_PosLmServerCommon.h"
#include "EPos_LmServerGlobal.h"
#include "EPos_LandmarksServerPanics.h"
#include "EPos_CPosLmEventHandler.h"
#include "EPos_PosLmDatabaseManager.h"
#include "epos_cposlmindexmanager.h"
#include "EPos_CPosLmNameIndex.h"
#include "EPos_CPosLmActiveCompacter.h"
#include "EPos_CPosLmLocalAccessSubsession.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalAccessSubsession::CPosLmLocalAccessSubsession(
    CPosLmServer* aLmServer) :
    CPosLmSubsessionBase(EPosLocalAccessSubsession, aLmServer)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalAccessSubsession* CPosLmLocalAccessSubsession::NewLC(
    CPosLmServer* aLmServer)
    {
    CPosLmLocalAccessSubsession* self =
        new (ELeave) CPosLmLocalAccessSubsession(aLmServer);
    CleanupClosePushL(*self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalAccessSubsession::~CPosLmLocalAccessSubsession()
    {
    delete iCompacter;
    delete iDbAccess;
    delete iEventHandler;

    // release database
    iHasWriteLock = EFalse;
    iNoOfReadLocks = 0;
    if ( iUsingNameIndex && Server() && iDbUri )
        {
        Server()->IndexManager().RemoveIndexUser( *iDbUri );
        iUsingNameIndex = EFalse;
        }
    delete iDbUri;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalAccessSubsession::HasWriteLock( const TDesC& aUri )
    {
    if (iHasWriteLock && aUri == *iDbUri)
        {
        return ETrue;
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalAccessSubsession::HasAnyLock( const TDesC& aUri ) 
    {
    if ((iHasWriteLock || iNoOfReadLocks > 0) && aUri == *iDbUri)
        {
        return ETrue;
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::HandleOperationEventL(
    TPosLmEvent aEvent,
    const TDesC& aUri,
    TBool aCheckUri)
    {
    // Check if the event is dedicated to the database, which this subsession is
    // connected to.
    if (!aCheckUri && iEventHandler && iDbUri ||
        iEventHandler && iDbUri && aUri.Compare(*iDbUri) == 0)
        {
        iEventHandler->HandleIncomingEventL(aEvent);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::CompleteOutstandingEventRequest(
    TInt aError)
    {
    if (iEventHandler)
        {
        iEventHandler->CompleteAnyOutStandingRequest(aError);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::ReleaseCompactLock()
    {
    delete iCompacter;
    iCompacter = NULL;
    iHasWriteLock = EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::ServiceL(
    const RMessage2& aMessage)
    {
#if 0 // #ifdef _DEBUG
    RThread client;
    aMessage.Client( client );
    LOG3("LocalSession::ServiceL in: function %d (client SID 0x%X, thread 0x%X)",
        aMessage.Function(), aMessage.SecureId().iId, client.Id() );
#endif    
    
    switch (aMessage.Function())
        {
        case EPosLmServerRegisterSession:
            if (iDbUri)
                {
                PanicClient(aMessage, EPosClientHasAlreadyRegistered);
                return;
                }
            RegisterUriL(aMessage);
            break;
        case EPosLmServerInitializeSync:
        case EPosLmServerInitializeAsync:
        case EPosLmServerInitializeCancel:
        case EPosLmServerLangSwitchSync:
        case EPosLmServerLangSwitchAsync:
        case EPosLmServerLangSwitchCancel:
        case EPosLmServerRecoverSync:
        case EPosLmServerRecoverAsync:
        case EPosLmServerRecoverCancel:
            if (!iDbUri)
                {
                PanicClient(aMessage, EPosClientHasNotRegistered);
                return;
                }
            Server()->OperationManagerL(aMessage.Function())->AddRequestL(
                aMessage, *iDbUri, this);
            break;
        case EPosLmServerDbLock:
            if (!iDbUri)
                {
                PanicClient(aMessage, EPosClientHasNotRegistered);
                return;
                }
            DatabaseLockL(aMessage);
            break;
        case EPosLmServerIndexTransaction:
            if (!iDbUri)
                {
                PanicClient(aMessage, EPosClientHasNotRegistered);
                return;
                }
            HandleDataTransactionL(aMessage);
            break;
        case EPosLmServerReportLmEvent:
            if (!iDbUri)
                {
                PanicClient(aMessage, EPosClientHasNotRegistered);
                return;
                }
            ForwardEventToAllSessionsL(aMessage);
            break;
        case EPosLmServerUpdateNameIndex:
            if (!iDbUri)
                {
                PanicClient(aMessage, EPosClientHasNotRegistered);
                return;
                }
            HandleUpdateLandmarkIndexL( aMessage );
            break;
        case EPosLmServerListenForLmEvent:
            // No check of iDbUri needed here
            User::LeaveIfError(Server()->StartSettingsListenerIfNeeded());
            if (!iEventHandler)
                {
                iEventHandler = CPosLmEventHandler::NewL();
                }
            iEventHandler->HandleRequestForEventL(aMessage);
            break;
        case EPosLmServerCancelListenForLmEvent:
            // No check of iDbUri needed here
            delete iEventHandler;
            iEventHandler = NULL;
            LmServerGlobal::Complete(aMessage, KErrNone);
            break;
        case EPosLmServerCreateDefaultDb:
            // No check of iDbUri needed here
            CreateDefaultDatabaseL(aMessage);
            break;
        case EPosLmServerOpenNameIndex:
        case EPosLmServerCloseNameIndex:
            if ( !iDbUri )
                {
                PanicClient( aMessage, EPosClientHasNotRegistered );
                return;
                }
            HandleUseNameIndexL( aMessage );
            break;
        case EPosLmServerReadSortedIds:
            if (!iDbUri)
                {
                PanicClient(aMessage, EPosClientHasNotRegistered);
                return;
                }
            HandleReadSortedIdsL( aMessage );
            break;
        case EPosLmServerReadNameIndex:
            if (!iDbUri)
                {
                PanicClient(aMessage, EPosClientHasNotRegistered);
                return;
                }
            HandleReadSortedLandmarksL( aMessage );
            break;
        case EPosLmServerNameIndexStatus:
            if (!iDbUri)
                {
                PanicClient(aMessage, EPosClientHasNotRegistered);
                return;
                }
            HandleNameIndexStatusL( aMessage );
            break;
        default:
            User::Leave(KErrNotSupported);
            break;
        }
#if 0 // #ifdef _DEBUG
    LOG("LocalSession::ServiceL out");
#endif
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::RegisterUriL(
    const RMessage2& aMessage)
    {
    // Validate length of client message.
    TInt desLength = aMessage.GetDesLength(KPosLmServerUriArg);
    if (desLength < 0)
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    // Read URI from client message.
    HBufC* dbUri = HBufC::NewLC(desLength);
    TPtr uriPtr = dbUri->Des();
    if (LmServerGlobal::Read(aMessage, KPosLmServerUriArg, uriPtr) != KErrNone)
        {
        CleanupStack::PopAndDestroy(dbUri);
        return;
        }

    // Lowercase the URI to be able to match different cases of the same URI.
    uriPtr.LowerCase();

    TInt err = PosLmDatabaseUtility::ValidateDatabaseUri(uriPtr);
    if (err == KErrArgument)
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        CleanupStack::PopAndDestroy(dbUri);
        return;
        }
    else
        {
        User::LeaveIfError(err);
        }

    // iDbUri is set last as it is used to validate if the uri has been
    // registered properly.
    CleanupStack::Pop(dbUri);
    iDbUri = dbUri;
    
    LmServerGlobal::Complete(aMessage, KErrNone);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::DatabaseLockL(
    const RMessage2& aMessage)
    {
    TBool setLockOn = static_cast<TBool>(aMessage.Int0());
    TBool writeLock = static_cast<TBool>(aMessage.Int1());

    if (setLockOn)
        {
        if (!Server()->CanAcquireLock(writeLock, *iDbUri))
            {
            User::Leave(KErrLocked);
            }

        if (writeLock)
            {
            iHasWriteLock = ETrue;
            }
        else
            {
            iNoOfReadLocks++;
            }
        }
    else if (writeLock)
        {
        if (!iHasWriteLock)
            {
            PanicClient(aMessage, EPosLmDbNotLocked);
            return;
            }
        iHasWriteLock = EFalse;
        }
    else
        {
        if (iNoOfReadLocks == 0)
            {
            PanicClient(aMessage, EPosLmDbNotLocked);
            return;
            }
        iNoOfReadLocks--;
        }

    LmServerGlobal::Complete(aMessage, KErrNone);

    if (!setLockOn && writeLock) // releasing write lock
        {
        TRAP_IGNORE( CompactIfNeededL() );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::HandleDataTransactionL( const RMessage2& aMessage )
    {
    if ( Server()->IndexManager().Find( *iDbUri ) >= 0 )
        {
        CPosLmNameIndex& index = Server()->IndexManager().Index( *iDbUri );
        User::LeaveIfError( index.Status() );
        switch( aMessage.Int0() )
            {
            case EPosLmServerTransactionStart:
                index.StartTransaction();
                break;
            case EPosLmServerTransactionCommit:
                index.CommitTransaction();
                break;
            case EPosLmServerTransactionRollback:
                index.RollbackTransaction();
                break;
            default:
                PanicClient( aMessage, EPosUnableToReadOrWriteDataToClient );
            }
        }

    LmServerGlobal::Complete( aMessage, KErrNone );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::ForwardEventToAllSessionsL(
    const RMessage2& aMessage)
    {
    TPckgBuf<TPosLmEvent> event;

    TInt desLength = aMessage.GetDesLength(KPosLmServerEventArg);
    if (desLength != event.Length())
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    if (LmServerGlobal::Read(aMessage, KPosLmServerEventArg, event) != KErrNone)
        {
        return;
        }

    // Validate that the event type is a valid enum value.
    TPosLmEventType eventType = event().iEventType;
    if (!(eventType == EPosLmEventUnknownChanges ||
        eventType == EPosLmEventNewDefaultDatabaseLocation ||
        eventType == EPosLmEventMediaRemoved ||
        (eventType >= EPosLmEventLandmarkUnknownChanges &&
        eventType <= EPosLmEventLandmarkUpdated) ||
        (eventType >= EPosLmEventCategoryUnknownChanges &&
        eventType <= EPosLmEventCategoryUpdated)))
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    Server()->ForwardEventToAllSessionsL(event(), *iDbUri, ETrue);

    LmServerGlobal::Complete(aMessage, KErrNone);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::CreateDefaultDatabaseL(
    const RMessage2& aMessage)
    {
    TInt desLength = aMessage.GetDesLength(KPosLmServerUriArg);
    if (desLength < 0)
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    HBufC* msgUri = HBufC::NewLC(desLength);
    TPtr uriPtr = msgUri->Des();

    if (LmServerGlobal::Read(aMessage, KPosLmServerUriArg, uriPtr) == KErrNone)
        {
        HBufC* uri = PosLmDatabaseUtility::CreateDatabaseUriL(*msgUri);
        CleanupStack::PushL(uri);

        // To remove a warning, the event must be default constructed.
        TPosLmDatabaseEvent event =
            {
            EPosLmDbDatabaseRegistered,
            {0,0,0,0,0,0,0,0}
            };

        CheckDiskSizeL(*uri);
        TPckgBuf<TBool> replaceDb;

        desLength = aMessage.GetDesLength(KPosLmServerDbReplaceArg);
        if (desLength != replaceDb.Length())
            {
            PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
            CleanupStack::PopAndDestroy(2, msgUri);
            return;
            }

        if (LmServerGlobal::Read(
            aMessage, KPosLmServerDbReplaceArg, replaceDb) != KErrNone)
            {
            CleanupStack::PopAndDestroy(2, msgUri);
            return;
            }

        PosLmDatabaseManager::CreateDatabaseL(uri->Des(), ETrue, replaceDb());

        Server()->ForwardEventToAllSessionsL(event, uri->Des());
        LmServerGlobal::Complete(aMessage, KErrNone);
        CleanupStack::PopAndDestroy(uri);
        }

    CleanupStack::PopAndDestroy(msgUri);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::CheckDiskSizeL(
    const TDesC& aUri)
    {
    CPosLmDiskUtilities* diskUtilities = CPosLmDiskUtilities::NewL();
    CleanupStack::PushL(diskUtilities);

    diskUtilities->DiskSpaceBelowCriticalLevelL(
        diskUtilities->EstimatedDiskSizeOfEmptyDatabase(),
        (aUri)[KPosLocalDbFileProtocol().Length()]);

    CleanupStack::PopAndDestroy(diskUtilities);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::CompactIfNeededL()
    {
    if (!iDbAccess)
        {
        CPosLmLocalDbAccess* dbAccess =
            CPosLmLocalDbAccess::NewL(CPosLmLocalDbAccess::EServerCompactLevel);
        CleanupStack::PushL(dbAccess);
        User::LeaveIfError(dbAccess->OpenDatabaseL(*iDbUri)); // Will be trapped
        CleanupStack::Pop(dbAccess);
        iDbAccess = dbAccess;
        }

    if (iDbAccess->IsCompactNeededL())
        {
        iCompacter = CPosLmActiveCompacter::NewL(*this, *iDbAccess);
        iHasWriteLock = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::HandleReadSortedIdsL( const RMessage2& aMessage )
    {
    AssertIndexLoadedL();

    TPosLmServerReadArrayParam param;
    TPckg<TPosLmServerReadArrayParam> pack( param );
    if ( LmServerGlobal::Read( aMessage, KPosLmServerIdArrayParam, pack ) != KErrNone )
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }
    
    CPosLmNameIndex& index = Server()->IndexManager().Index( *iDbUri );
    User::LeaveIfError( index.Status() );

    TPosLmItemId* ids = new (ELeave) TPosLmItemId[ param.iCount ];
    CleanupStack::PushL( ids );
    TPtrC8 data( (TUint8*) ids, sizeof( TPosLmItemId ) * param.iCount );

    TInt count = index.GetSortedIds( param.iFirst, param.iCount, ids, param.iRemainder );
    if ( count >= 0 )
        {
        param.iActualCount = count;
        param.iMinBufferNeeded = 0;
        User::LeaveIfError( LmServerGlobal::Write( aMessage, KPosLmServerIdArrayParam, pack ) );
        User::LeaveIfError( LmServerGlobal::Write( aMessage, KPosLmServerIdArray, data ) );
        }
    else
        {
        User::Leave( count ); // error code
        }
    CleanupStack::PopAndDestroy( ids );
    LmServerGlobal::Complete( aMessage, count );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::HandleReadSortedLandmarksL( const RMessage2& aMessage )
    {
    AssertIndexLoadedL();

    TPosLmServerReadArrayParam param;
    TPckg<TPosLmServerReadArrayParam> pack( param );
    if ( LmServerGlobal::Read( aMessage, 0, pack ) != KErrNone )
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    param.iMinBufferNeeded = 0;
    param.iActualCount = 0;
    param.iRemainder = 0;

    TInt bufferSize = aMessage.GetDesLengthL( 1 );
    
    CPosLmNameIndex& index = Server()->IndexManager().Index( *iDbUri );
    User::LeaveIfError( index.Status() );

    CBufFlat* buffer = CBufFlat::NewL( bufferSize );
    CleanupStack::PushL( buffer );
    
    RBufWriteStream out;
    out.Open( *buffer );
    CleanupClosePushL( out );

    if ( param.iCount > 0 ) // in ascending order
        {
        GetSortedLandmarksL( index, out, param, bufferSize );
        }
    else
        {
        GetSortedLandmarksReverseL( index, out, param, bufferSize );
        }
    
    out.CommitL();
    
    User::LeaveIfError( LmServerGlobal::Write( aMessage, 0, pack ) );
    User::LeaveIfError( LmServerGlobal::Write( aMessage, 1, buffer->Ptr( 0 ) ) );
    LmServerGlobal::Complete( aMessage, KErrNone );

    CleanupStack::PopAndDestroy( &out );
    CleanupStack::PopAndDestroy( buffer );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::GetSortedLandmarksL( 
    CPosLmNameIndex& aNameIndex,
    RWriteStream& aOut,
    TPosLmServerReadArrayParam& aParam,
    TInt aMaxSize )
    {
    TInt start = Max( aParam.iFirst, 0 );
    TInt last = Min( aParam.iFirst + aParam.iCount, aNameIndex.Count() );

    TInt leftInBuffer = aMaxSize;
    TInt i = start;
    for ( ; i < last ; i++ ) 
        {
        const CPosLmNameIndex::CItem& item = aNameIndex.Item( i );
        if ( !ExportIndexItemL( item, aOut, leftInBuffer, aParam ) )
            break;
        }

    aParam.iRemainder = aNameIndex.Count() - i;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::GetSortedLandmarksReverseL( 
    CPosLmNameIndex& aNameIndex,
    RWriteStream& aOut,
    TPosLmServerReadArrayParam& aParam,
    TInt aMaxSize )
    {
    TInt start = Min( aParam.iFirst, aNameIndex.Count() - 1 );
    TInt last = Max( aParam.iFirst + aParam.iCount, -1 ); // iCount is negative

    TInt leftInBuffer = aMaxSize;
    TInt i = start;
    for ( ; i > last ; i-- ) 
        {
        const CPosLmNameIndex::CItem& item = aNameIndex.Item( i );
        if ( !ExportIndexItemL( item, aOut, leftInBuffer, aParam ) )
            break;
        }

    aParam.iRemainder = i+1;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalAccessSubsession::ExportIndexItemL( 
    const CPosLmNameIndex::CItem& aItem,
    RWriteStream& aOut,
    TInt& aLeftInBuffer,
    TPosLmServerReadArrayParam& aParam )
    {
    TInt itemSize = aItem.Size();
    if ( itemSize <= aLeftInBuffer )
        {
        aItem.ExternalizeL( aOut );
        aLeftInBuffer -= itemSize;
        aParam.iActualCount++;
        return ETrue;
        }
    else 
        { 
        if ( itemSize > aLeftInBuffer )
            {
            aParam.iMinBufferNeeded = itemSize;
            }
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::HandleUpdateLandmarkIndexL( const RMessage2& aMessage )
    {
    //LOG1("UpdateIndex: function %d", aMessage.Int0() );
    AssertIndexLoadedL();
    switch ( aMessage.Int0() )
        {
        case EPosLmServerAddLandmarks:
        case EPosLmServerUpdateLandmarks:
            HandleLandmarksAddedOrUpdatedL( aMessage );
            break;
        case EPosLmServerRemoveLandmarks:
            HandleLandmarksRemovedL( aMessage );
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::HandleLandmarksAddedOrUpdatedL( const RMessage2& aMessage )
    {
    CPosLmNameIndex& index = Server()->IndexManager().Index( *iDbUri );
    User::LeaveIfError( index.Status() );

    HBufC8* buf = LmServerGlobal::CopyClientBuffer8LC( aMessage, 1 );
    RDesReadStream readStream( *buf );

    TPosLmItemId id = KPosLmNullItemId;

    // Update index
    TInt lmCount = readStream.ReadInt32L();
    for ( TInt i = 0; i < lmCount; i++ )
        {
        id = readStream.ReadInt32L();
        TInt nameLen = readStream.ReadInt32L();
        
        HBufC* lmBuf = HBufC::NewLC( nameLen );
        
        TPtr name( lmBuf->Des() );
        readStream.ReadL( name, nameLen );
        
        CleanupStack::Pop( lmBuf );//ownership of lmBuf is transferred in the call to UpdateL/InsertL
		if ( aMessage.Int0() == EPosLmServerUpdateLandmarks ) 
            {
//coverity[freed_arg : FALSE]
            index.UpdateL( id, lmBuf );
            }
        else
            {
            index.InsertL( id, lmBuf );
            }
//coverity[pass_freed_arg : FALSE]
        }
    
    readStream.Close();
    CleanupStack::PopAndDestroy( buf );

    LmServerGlobal::Complete( aMessage, KErrNone );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::HandleLandmarksRemovedL( const RMessage2& aMessage )
    {
    HBufC8* buf = LmServerGlobal::CopyClientBuffer8LC( aMessage, 1 );
    if ( buf->Size() % ( sizeof( TPosLmItemId ) ) != 0 )
        {
        User::Leave( KErrArgument );
        }

    TInt lmCount = buf->Size() / ( sizeof( TPosLmItemId ) );
    const TPosLmItemId* ids = reinterpret_cast<const TPosLmItemId*>( buf->Ptr() );

    // Update index
    CPosLmNameIndex& index = Server()->IndexManager().Index( *iDbUri );
    User::LeaveIfError( index.Status() );
    for ( TInt i = 0; i < lmCount; i++ )
        {
        index.Remove( ids[i] );
        }
    CleanupStack::PopAndDestroy( buf );
    
    LmServerGlobal::Complete( aMessage, KErrNone );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::HandleNameIndexStatusL( const RMessage2& aMessage )
    {
    AssertIndexLoadedL();
    CPosLmNameIndex& index = Server()->IndexManager().Index( *iDbUri );
    TInt status = index.Status();
    if ( status < 0 )
        LmServerGlobal::Complete( aMessage, status );
    else
        LmServerGlobal::Complete( aMessage, index.Count() );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::HandleUseNameIndexL(
    const RMessage2& aMessage )
    {
    switch ( aMessage.Function() )
        {
        case EPosLmServerOpenNameIndex:
            {
            if ( iUsingNameIndex )
                {
                PanicClient( aMessage, EPosClientHasAlreadyRegistered );
                return;
                }
            // Register new user of lm index
            Server()->IndexManager().AddIndexUserL( *iDbUri );
            iUsingNameIndex = ETrue;
            break;
            }
        case EPosLmServerCloseNameIndex:
            {
            if ( !iUsingNameIndex )
                {
                PanicClient( aMessage, EPosClientHasNotRegistered );
                return;
                }
            Server()->IndexManager().RemoveIndexUser( *iDbUri );
            iUsingNameIndex = EFalse;
            break;
            }
        default:
            break;
        }
    LmServerGlobal::Complete( aMessage, KErrNone );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAccessSubsession::AssertIndexLoadedL()
    {
    if ( Server()->IndexManager().Find( *iDbUri ) < 0 )
        {
        User::Leave( KErrNotFound );
        }
    }
