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
* Description: provides functionality common to all server operations
*
*
*/


#include <EPos_Landmarks.h>
#include "EPos_CPosLmServerOperation.h"
#include "EPos_LandmarksServerPanics.h"
#include "EPos_MPosLmServerOperationObserver.h"
#include "EPos_PosLmServerCommon.h"
#include "EPos_LmServerGlobal.h"

const TInt KDrivePosition = 7;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmServerOperation::CPosLmServerOperation(
    CPosLmServer& aServer,
    MPosLmServerOperationObserver* aObserver ) :
    CActive(EPriorityIdle),
    iServer( aServer ),
    iObserver(aObserver),
    iProgress(0),
    iStatusFlag(KPosLmOperationNotComplete)
    {
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerOperation::BaseConstructL(
    const TDesC& aUri)
    {
    iDb = CPosLmLocalDbAccess::NewL();
    User::LeaveIfError(iDb->OpenDatabaseL(aUri));

    iDiskUtilities = CPosLmDiskUtilities::NewL();

    iDbDrive = aUri[KDrivePosition];
    iDbUri = aUri.AllocL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmServerOperation::~CPosLmServerOperation()
    {
    Cancel();
    if (iStatusFlag == KErrNone)
        {
        iStatusFlag = KErrServerTerminated;
        }
    CompleteAndRemoveMessagesFromSyncWaitList();
    CompleteAndRemoveMessagesFromAsyncWaitList();
    iSyncWaitList.Close();
    iAsyncWaitList.Close();
    delete iDiskUtilities;
    delete iDb;
    iClientSessions.Close();
    delete iDbUri;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmServerOperation::RunError( TInt aError )
    {
    iStatusFlag = aError;
    CompleteAndRemoveMessagesFromSyncWaitList();
    CompleteAndRemoveMessagesFromAsyncWaitList();
    iObserver->HandleOperationStatus(this, iStatusFlag);

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerOperation::DoCancel()
    {
    iStatusFlag = KErrCancel;
    CompleteAndRemoveMessagesFromSyncWaitList();
    CompleteAndRemoveMessagesFromAsyncWaitList();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerOperation::AddToSyncWaitListL(
    const RMessage2& aMessage,
    TAny* aSession)
    {
    User::LeaveIfError(iSyncWaitList.Append(Message(aMessage, aSession)));
    if (iClientSessions.Find(static_cast<TUint*>(aSession)) == KErrNotFound)
        {
        User::LeaveIfError(iClientSessions.Append(
                    static_cast<TUint*>(aSession)));
        }

    if ( iStatusFlag != KPosLmOperationNotComplete )
        {
        NotifyClients();
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerOperation::AddToAsyncWaitListL(
    const RMessage2& aMessage,
    TAny* aSession )
    {
    TPckgBuf<TReal32> progress;
    TInt desMaxLength = aMessage.GetDesMaxLength(KPosLmServerProgressArg);
    if (desMaxLength != progress.Length())
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }
    User::LeaveIfError(iAsyncWaitList.Append(Message(aMessage, aSession)));
    if (iClientSessions.Find(static_cast<TUint*>(aSession)) == KErrNotFound)
        {
        User::LeaveIfError(iClientSessions.Append(
            static_cast<TUint*>(aSession)));
        }
    
    if ( iStatusFlag != KPosLmOperationNotComplete )
        {
        // this client should only be informed about operation
        // status (because it has not been yet) and removed from
        // operation's clients
        NotifyClients();
        RemoveSessionMessages( aSession );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerOperation::RemoveFromAsyncWaitList(
    const RMessage2& aMessage,
    TAny* aSession)
    {
    RemoveFromWaitList(iAsyncWaitList, aSession);
    LmServerGlobal::Complete(aMessage, KErrNone);

    TInt position = iClientSessions.Find(static_cast<TUint*>(aSession));
    if (position != KErrNotFound)
        {
        iClientSessions.Remove(position);
        }
    iObserver->HandleOperationStatus( this, iStatusFlag );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerOperation::RemoveSessionMessages(
    TAny* aSession)
    {
    RemoveFromWaitList(iAsyncWaitList, aSession);
    RemoveFromWaitList(iSyncWaitList, aSession);

    TInt position = iClientSessions.Find(static_cast<TUint*>(aSession));
    if (position != KErrNotFound)
        {
        iClientSessions.Remove(position);
        }
    iObserver->HandleOperationStatus( this, iStatusFlag );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerOperation::CompleteAndRemoveMessagesFromAsyncWaitList()
    {
    for (TInt i = 0; i < iAsyncWaitList.Count(); i++)
        {
        Write(iAsyncWaitList[i].iMsg);
        LmServerGlobal::Complete(iAsyncWaitList[i].iMsg, iStatusFlag);
        }

    iAsyncWaitList.Reset();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerOperation::CompleteAndRemoveMessagesFromSyncWaitList()
    {
    for (TInt i = 0; i < iSyncWaitList.Count(); i++)
        {
        LmServerGlobal::Complete(iSyncWaitList[i].iMsg, iStatusFlag);
        }

    iSyncWaitList.Reset();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerOperation::Write(
    const RMessage2& aMessage)
    {
    // It is not necessary to return the error code, because the client will be
    // panicked if the write fails. Hence, the client won't receive the error
    // anyway.
    TPckgBuf<TInt> statusFlag(iStatusFlag);
    TInt err = LmServerGlobal::Write(aMessage, KPosLmServerStatusArg,
        statusFlag);

    if (err == KErrNone)
        {
        TPckgBuf<TReal32> progress(iProgress);
        err = LmServerGlobal::Write(aMessage, KPosLmServerProgressArg,
            progress);

        if (err == KErrNone && iStatusFlag == KErrNone)
            {
            // Reset init flag on client side
            TPckgBuf<TUint> initFlag(ENoInit);
            LmServerGlobal::Write(aMessage, KPosLmServerInitFlagArg, initFlag);
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerOperation::NotifyClients()
    {
    // Check if there are any clients left listening for status
    if (iClientSessions.Count() == 0)
        {
        Cancel();
        iObserver->HandleOperationStatus(this, iStatusFlag);
        return;
        }

    CompleteAndRemoveMessagesFromAsyncWaitList();

    if ( iStatusFlag == KErrNone )
        {
        CompleteAndRemoveMessagesFromSyncWaitList();
        iObserver->HandleOperationStatus(this, iStatusFlag);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerOperation::CompleteSelf()
    {
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status, KErrNone);
    SetActive();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerOperation::RemoveFromWaitList(
    RArray<TPosMessage>& aWaitList,
    TAny* aSession)
    {
    for (TInt i = 0; i < aWaitList.Count(); i++)
        {
        if (aSession == aWaitList[i].iSession)
            {
            LmServerGlobal::Complete(aWaitList[i].iMsg, KErrCancel);
            aWaitList.Remove(i--);
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmServerOperation::TPosMessage CPosLmServerOperation::Message(
    const RMessage2& aMessage,
    TAny* aSession)
    {
    TPosMessage msg;
    msg.iSession = aSession;
    msg.iMsg = aMessage;

    return msg;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmServerOperation::Status() const
    {
    return iStatusFlag;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmServerOperation::IsRegistered( TAny* aSession ) const
    {
    return iClientSessions.Find( static_cast<TUint*>( aSession ) ) != KErrNotFound;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmServerOperation::HasRequests( TAny* aSession ) const
    {
    for ( TInt i = 0; i < iAsyncWaitList.Count(); i++ )
        {
        if ( iAsyncWaitList[i].iSession == aSession )
            return ETrue;
        }
    for ( TInt i = 0; i < iSyncWaitList.Count(); i++ )
        {
        if ( iSyncWaitList[i].iSession == aSession )
            return ETrue;
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmServerOperation::HasClients() const
    {
    return iClientSessions.Count() > 0;;
    }
