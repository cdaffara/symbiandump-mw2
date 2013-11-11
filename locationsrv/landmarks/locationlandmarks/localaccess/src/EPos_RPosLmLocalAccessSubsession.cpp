/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: This class is a subsession handle with operations related to
*  local access.
*
*
*/


#include <e32base.h>
#include <epos_poslmservercommon.h>
#include <epos_rposlandmarkserver.h>

#include "epos_rposlmlocalnameindex.h"
#include "EPos_RPosLmLocalAccessSubsession.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
RPosLmLocalAccessSubsession::RPosLmLocalAccessSubsession()
:   RSubSessionBase(),
    iEventPtr(NULL, 0)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt RPosLmLocalAccessSubsession::Open( RPosLandmarkServer& aServer )
    {
    return CreateSubSession(aServer,
                            EPosLmServerRegisterLocalAccessSubsession);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void RPosLmLocalAccessSubsession::Close()
    {
    CloseSubSession( EPosLmServerUnregisterSubsession );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void RPosLmLocalAccessSubsession::RegisterSessionL( const TDesC& aUri )
    {
    User::LeaveIfError(
        SendReceive(EPosLmServerRegisterSession, TIpcArgs(&aUri)));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void RPosLmLocalAccessSubsession::NotifyDatabaseEvent(
    TPosLmEvent &aEvent,
    TRequestStatus& aStatus)
    {
    iEventPtr.Set(reinterpret_cast<TUint8*>(&aEvent), sizeof(TPosLmEvent),
        sizeof(TPosLmEvent));

    SendReceive(EPosLmServerListenForLmEvent, TIpcArgs(&iEventPtr), aStatus);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt RPosLmLocalAccessSubsession::CancelNotifyDatabaseEvent()
    {
    return SendReceive(EPosLmServerCancelListenForLmEvent,
                        TIpcArgs(TIpcArgs::ENothing));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt RPosLmLocalAccessSubsession::ReportEvent(
    TPosLmEventType aEventType,
    TPosLmItemId aId)
    {
    //coverity[var_decl : FALSE]
    TPosLmEvent event;
    event.iEventType = aEventType;
    event.iLandmarkItemId = aId;
    TPckgBuf<TPosLmEvent> eventPckg(event);

    return SendReceive(EPosLmServerReportLmEvent, TIpcArgs(&eventPckg));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt RPosLmLocalAccessSubsession::SetDatabaseLock(
    CPosLmLocalDatabase::TLockType aLockType )
    {
    return SendReceive( EPosLmServerDbLock, TIpcArgs(
        ETrue, // Set lock
        aLockType == CPosLmLocalDatabase::EWriteLock ? ETrue : EFalse ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt RPosLmLocalAccessSubsession::ReleaseDatabaseLock(
    CPosLmLocalDatabase::TLockType aLockType )
    {
    return SendReceive( EPosLmServerDbLock, TIpcArgs(
        EFalse, // Release lock
        aLockType == CPosLmLocalDatabase::EWriteLock ? ETrue : EFalse ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void RPosLmLocalAccessSubsession::CreateDefaultDbL(
    const TDesC& aUri,
    TBool aReplaceDb)
    {
    TPckgC<TBool> pkg(aReplaceDb);
    User::LeaveIfError(SendReceive(EPosLmServerCreateDefaultDb,
        TIpcArgs(&aUri, &pkg)));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt RPosLmLocalAccessSubsession::InitializeDbSync( CPosLmLocalInitializeOp::TInitializeType /*aInitType*/ )
    {
    return SendReceive( EPosLmServerInitializeSync, TIpcArgs( TIpcArgs::ENothing ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void RPosLmLocalAccessSubsession::InitializeDbAsync(
    CPosLmLocalInitializeOp::TInitializeType /*aInitType*/,
    TPckgBuf<TInt>* aStatusPckg,
    TPckg<TReal32>* aProgressPckg,
    TPckg<TUint>* aInitFlagPckg,
    TRequestStatus& aStatus)
    {
    TIpcArgs args( aStatusPckg, aProgressPckg, aInitFlagPckg );
    SendReceive( EPosLmServerInitializeAsync, args, aStatus );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt RPosLmLocalAccessSubsession::CancelInitializeDbAsync( CPosLmLocalInitializeOp::TInitializeType /*aInitType*/ )
    {
    return SendReceive( EPosLmServerInitializeCancel, TIpcArgs( TIpcArgs::ENothing ) );
    }

