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
* Description: The main server side class of Landmarks Server.
*
*/


#include <e32svr.h>
#include <epos_cposlmresourcereader.h>
#include <eposlmserver.rsg>
#include "EPos_CPosLmDbRegistry.h"
#include "EPos_CPosLmServerSession.h"
#include "EPos_CPosLmServerSettingsHandler.h"
#include "EPos_PosLmServerCommon.h"
#include "EPos_CPosLmServerDelayedShutdown.h"
#include "EPos_LmServerGlobal.h"
#include "EPos_LandmarksServerPanics.h"
#include "EPos_CPosLmBackupListener.h"
#include "EPos_LmServerGlobal.h"
#include "epos_cposlmindexmanager.h"
#include "EPos_CPosLmServer.h"

_LIT(KPosResourceFile, "\\Private\\101fdf81\\eposlmserver.rsc");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmServer::CPosLmServer(TInt aPriority)
    : CPolicyServer(aPriority, KPolicy)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServer::ConstructL()
    {
    iBackupListener = CPosLmBackupListener::NewL(this);
    LeaveIfBackupIsRunningL();

    iServerSettingsHandler = CPosLmServerSettingsHandler::NewL(*this);
    User::LeaveIfError(iServerSettingsHandler->StartListening());
    iShutdown = CPosLmServerDelayedShutdown::NewL();

    CPosLmResourceReader* resources =
        CPosLmResourceReader::NewLC(KPosResourceFile);
    iSrvShutdownDelay = resources->ReadInt32L(R_POS_LM_SERVER_SHUTDOWN_DELAY);

    iDbPath = resources->ReadHBufCL(R_POS_LM_SERVER_DB_PATH_FULL);
    CleanupStack::PopAndDestroy(resources);

    iIndexManager = CPosLmIndexManager::NewL( *this );
    
    User::LeaveIfError(iFs.Connect());
    StartL(KPosLandmarksServerName);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmServer* CPosLmServer::NewL()
    {
    CPosLmServer* self = new (ELeave) CPosLmServer(EPriorityHigh);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmServer::~CPosLmServer()
    {
    Cancel();
    iManagers.DeleteAll();
    delete iIndexManager;
    delete iServerSettingsHandler;
    delete iShutdown;
    CloseAllRegistries();
    iRegistryArray.Close();
    delete iDbPath;
    delete iBackupListener;
    iFs.Close();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmServer::RunError(
    TInt aError)
    {
    Message().Complete(aError);
    ReStart();
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServer::HandleManagerFinished()
    {
    for (TInt i = 0; i < iManagers.Count(); i++)
        {
        if (iManagers[i] && iManagers[i]->Count() == 0)
            {
            // Manager is not used any more, so delete it.
            delete iManagers[i];
            iManagers[i] = NULL;
            }
        }
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CPosLmServer::HandleBackupEvent()
    {
    CloseAllRegistries();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CSession2* CPosLmServer::NewSessionL(
    const TVersion& aVersion, const RMessage2& /*aMessage*/) const
    {
    if (iNumSessions == 0)
        {
        // OK to call cancel even if shutdown is not started
        iShutdown->Cancel();
        }

    // Check we're the right version
    TVersion version(KPosLmServerMajorVersionNumber,
                     KPosLmServerMinorVersionNumber,
                     KPosLmServerBuildVersionNumber);
    if (!User::QueryVersionSupported(version, aVersion))
        {
        User::Leave(KErrNotSupported);
        }

    // Make a new session
    CPosLmServerSession* newSession = CPosLmServerSession::NewL(
        *(const_cast<CPosLmServer*> (this)));

    return newSession;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPolicyServer::TCustomResult CPosLmServer::CustomSecurityCheckL(
    const RMessage2& aMsg,
    TInt& /*aAction*/,
    TSecurityInfo& /*aMissing*/)
    {
    switch (aMsg.Function())
        {
        case EPosLmServerDbLock:
            if (static_cast<TBool>(aMsg.Int1()))
                {
                // Write lock
                if (aMsg.HasCapability(ECapabilityReadUserData,
                    ECapabilityWriteUserData,
                    __PLATSEC_DIAGNOSTIC_STRING(
                        "Landmarks: Attempt made to fetch database write lock."
                        )))
                    {
                    return EPass;
                    }
                }
            else
                {
                // Read lock
                if (aMsg.HasCapability(ECapabilityReadUserData,
                    __PLATSEC_DIAGNOSTIC_STRING(
                    "Landmarks: Attempt made to fetch database read lock."
                    )))
                    {
                    return EPass;
                    }
                }
            break;
        default:
            break;
        }

    return EFail;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmOperationManager* CPosLmServer::OperationManagerL( TInt aFunction )
    {
    // Initialize to dummy value
    CPosLmOperationManager::TPosOperationManagerType type =
        CPosLmOperationManager::EPosMaxNumberOfManagers;

    switch (aFunction)
        {
        case EPosLmServerInitializeSync:
        case EPosLmServerInitializeAsync:
        case EPosLmServerInitializeCancel:
        case EPosLmServerLangSwitchSync:
        case EPosLmServerLangSwitchAsync:
        case EPosLmServerLangSwitchCancel:
        case EPosLmServerRecoverSync:
        case EPosLmServerRecoverAsync:
        case EPosLmServerRecoverCancel:
            type = CPosLmOperationManager::EPosInitializerManager;
            break;
        default:
            User::Leave(KErrArgument);
            break;
        }

    if (!iManagers[type])
        {
        iManagers[type] = CPosLmOperationManager::NewL( *this, type, this );
        }

    return iManagers[type];
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServer::IncrementSessions()
    {
    iNumSessions++;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServer::DecrementSessions()
    {
    // This panic handles an internal error.
    __ASSERT_DEBUG(iNumSessions > 0,
        PanicServer(EPosLmServerPanicSessionsCountInconsistency));

    if (--iNumSessions == 0)
        {
        iManagers.DeleteAll(); // Stops all initialize operations
        iManagers.Reset();
        CloseAllRegistries();

        if (!(iShutdown->IsActive()))
            {
            iShutdown->Start(iSrvShutdownDelay);
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmServer::StartSettingsListenerIfNeeded()
    {
    if (iServerSettingsHandler->IsActive())
        {
        return KErrNone;
        }

    return iServerSettingsHandler->StartListening();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmServer::CanAcquireLock(
    TBool aWriteLock,
    const TDesC& aUri)
    {
    iSessionIter.SetToFirst();
    CPosLmServerSession* session =
        static_cast<CPosLmServerSession*>(iSessionIter++);

    // check locks from every session/subsession
    while (session)
        {
        if ((aWriteLock && session->HasAnyLock(aUri)) ||
            (!aWriteLock && session->HasWriteLock(aUri)))
            {
            return EFalse;
            }
        session = static_cast<CPosLmServerSession*>(iSessionIter++);
        }
    
    // check index manager as well
    if ( aWriteLock && IndexManager().HasAnyLock( aUri ) ||
        (!aWriteLock && IndexManager().HasWriteLock( aUri )))
        {
        return EFalse;
        }
    
    return ETrue;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServer::RemoveSessionMessages(
    TAny* aSession)
    {
    for (TInt i = 0; i < iManagers.Count(); i++)
        {
        if (iManagers[i])
            {
            iManagers[i]->RemoveSessionMessages(aSession);
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServer::ForwardEventToAllSessionsL(
    TPosLmDatabaseEvent aEvent,
    const TDesC& aUri)
    {
    // For all database management sessions, call HandleOperationEventL()
    iSessionIter.SetToFirst();
    CPosLmServerSession* session =
        static_cast<CPosLmServerSession*>(iSessionIter++);

    while (session)
        {
        session->HandleOperationEventL(aEvent, aUri);
        session = static_cast<CPosLmServerSession*>(iSessionIter++);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServer::ForwardEventToAllSessionsL(
    TPosLmEvent aEvent,
    const TDesC& aUri,
    TBool aCheckUri)
    {
     // For all Local Access sessions, call HandleOperationEventL()
    iSessionIter.SetToFirst();
    CPosLmServerSession* session =
        static_cast<CPosLmServerSession*>(iSessionIter++);

    while (session)
        {
        session->HandleOperationEventL(aEvent, aUri, aCheckUri);
        session = static_cast<CPosLmServerSession*>(iSessionIter++);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServer::CompleteOutstandingEventRequests(
    TInt aError)
    {
    // For all Local Access sessions, call HandleOperationEventL()
    iSessionIter.SetToFirst();
    CPosLmServerSession* session =
        static_cast<CPosLmServerSession*>(iSessionIter++);

    while (session)
        {
        session->CompleteOutstandingEventRequest(aError);
        session = static_cast<CPosLmServerSession*>(iSessionIter++);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmDbRegistry* CPosLmServer::GetRegistryL(TChar aDrive, TBool aCreateDb)
    {
    LeaveIfBackupIsRunningL();

    aDrive.UpperCase();

    HBufC* buf = HBufC::NewLC(iDbPath->Length() + 1);
    buf->Des().Append(aDrive);
    buf->Des().Append(*iDbPath);

    //Return the registry if it is already opened for the certain drive.
    for (TInt i = 0; i < iRegistryArray.Count(); i++)
        {
        TRegistryInfo regInfo = iRegistryArray[i];
        if (regInfo.iDrive == aDrive)
            {
            CleanupStack::PopAndDestroy(buf);
            return regInfo.iRegistry;
            }
        }

    CPosLmDbRegistry* dbReg = CPosLmDbRegistry::NewL();
    CleanupStack::PushL(dbReg);

    TInt err = dbReg->Open(iFs, *buf);
    if (err == KErrNotFound ||
        err == KErrPathNotFound ||
        err == KErrCorrupt)
        {
        if (!aCreateDb)
            {
            CleanupStack::PopAndDestroy(2, buf); //dbReg
            return NULL;
            }

        TInt driveNr;
        User::LeaveIfError(RFs::CharToDrive(aDrive, driveNr));
        err = iFs.CreatePrivatePath(driveNr);
        if (err == KErrNone)
            {
            iFs.SetSessionToPrivate(driveNr);
            }
        dbReg->CreateL(iFs, *buf);
        }
    else if (!aCreateDb && (err == KErrNotReady || err == KErrAccessDenied))
        {
        // If media is not present or access denied and
        // not create the db, otherwise leave with err
        CleanupStack::PopAndDestroy(2, buf); //dbReg
        return NULL;
        }
    else
        {
        User::LeaveIfError(err);
        }

    TRegistryInfo regInfo;
    regInfo.iDrive = aDrive;
    regInfo.iRegistry = dbReg;

    User::LeaveIfError(iRegistryArray.Append(regInfo));

    CleanupStack::Pop(dbReg);
    CleanupStack::PopAndDestroy(buf);
    return dbReg;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
RFs& CPosLmServer::FileSession()
    {
    return iFs;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServer::LeaveIfBackupIsRunningL()
    {
    if (iBackupListener->IsBackupRunning())
        {
        User::Leave(KErrNotReady);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmIndexManager& CPosLmServer::IndexManager()
    {
    return *iIndexManager;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServer::CloseAllRegistries()
    {
    for (TInt i = 0; i < iRegistryArray.Count(); i++)
        {
        delete iRegistryArray[i].iRegistry;
        iRegistryArray[i].iRegistry = NULL;
        }
    iRegistryArray.Reset();
    }

