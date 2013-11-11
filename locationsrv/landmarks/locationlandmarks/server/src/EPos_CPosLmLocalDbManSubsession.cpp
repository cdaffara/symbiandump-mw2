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
* Description: Subsession class for local database management operations
*
*
*/


#include <EPos_TPosLmDatabaseEvent.h>
#include <epos_cposlmdiskutilities.h>
#include <epos_poslmdatabaseutility.h>
#include <EPos_HPosLmDatabaseInfo.h>
#include <EPos_CPosLmDbRegistry.h>
#include "EPos_CPosLmLocalDbManSubsession.h"
#include "EPos_CPosLmServer.h"
#include "EPos_PosLmServerCommon.h"
#include "EPos_LmServerGlobal.h"
#include "EPos_PosLmDatabaseManager.h"
#include "EPos_LandmarksServerPanics.h"

//CONSTANTS

/*   Comment on KPosMaxUriLength:
*    KPosMaxUriLength = KDbMaxName + 9;
*    KDbMaxName is the maximum length for a DBMS name: 64 characters.
*    Length of "file://c:" is 9.
*/
const TInt KPosMaxUriLength = 73;

// -----------------------------------------------------------------------------
// CPosLmLocalDbManSubsession::CPosLmLocalDbManSubsession
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmLocalDbManSubsession::CPosLmLocalDbManSubsession(
    CPosLmServer* aLmServer) :
    CPosLmSubsessionBase(EPosLocalDbManProviderSubsession, aLmServer)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalDbManSubsession::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmLocalDbManSubsession::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalDbManSubsession::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalDbManSubsession* CPosLmLocalDbManSubsession::NewLC(
    CPosLmServer* aLmServer)
    {
    CPosLmLocalDbManSubsession* self =
        new (ELeave) CPosLmLocalDbManSubsession(aLmServer);
    CleanupClosePushL(*self);
    self->ConstructL();
    return self;
    }

// Destructor
CPosLmLocalDbManSubsession::~CPosLmLocalDbManSubsession()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalDbManSubsession::ServiceL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalDbManSubsession::ServiceL(
    const RMessage2& aMessage)
    {
    switch (aMessage.Function())
        {
        case EPosLmServerCreateDatabase:
        case EPosLmServerDeleteDatabase:
            HandleDbManagementOperationL(aMessage);
            break;
        case EPosLmServerCopyDatabase:
            HandleDbCopyOperationL(aMessage);
            break;
        default:
            User::Leave(KErrNotSupported);
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalDbManSubsession::HandleDbManagementOperationL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalDbManSubsession::HandleDbManagementOperationL(
    const RMessage2& aMessage)
    {
    TInt dbInfoLength = aMessage.GetDesLength(KPosLmServerDbInfoArg);
    if (dbInfoLength < 0)
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    HBufC8* buf = HBufC8::NewLC(dbInfoLength);
    TPtr8 bufPtr = buf->Des();
    if (LmServerGlobal::Read(
        aMessage, KPosLmServerDbInfoArg, bufPtr) == KErrNone)
        {
        // To remove a warning, the event must be default constructed.
        TPosLmDatabaseEvent event =
            {
            EPosLmDbDatabaseRegistered,
            {0,0,0,0,0,0,0,0}
            };

        TUint8* dbInfoPtr = const_cast<TUint8*>(buf->Ptr());
        HPosLmDatabaseInfo* dbInfo =
            reinterpret_cast<HPosLmDatabaseInfo*>(dbInfoPtr);


        // verify that dbInfo size is the same as the transferred descriptor
        if(bufPtr.Length() != dbInfo->Size())
            {
            PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
            CleanupStack::PopAndDestroy(buf);
            return;
            }

        HBufC* uri =
            PosLmDatabaseUtility::CreateDatabaseUriL(dbInfo->DatabaseUri());
        CleanupStack::PushL(uri);

        CPosLmDbRegistry* reg =
            Server()->GetRegistryL(dbInfo->DatabaseDrive(), ETrue);
        if (!reg)
            {
            User::Leave(KErrGeneral);
            }

        if (aMessage.Function() == EPosLmServerCreateDatabase)
            {
            // create on disk
            CheckDiskSizeL(*uri);
            PosLmDatabaseManager::CreateDatabaseL(*uri, EFalse);

            // register
            if (dbInfo->Settings().IsAttributeSet(
                TPosLmDatabaseSettings::EName))
                {
                reg->RegisterDatabaseL(*uri, dbInfo->Settings());
                User::LeaveIfError(reg->Compact());
                }
            }
        else
            {
            // a local db URI may not exceed KPosMaxUriLength
            if (uri->Length() > KPosMaxUriLength)
                {
                User::Leave(KErrArgument);
                }

            event.iEventType = EPosLmDbDatabaseUnregistered;

            // unregister
            reg->UnregisterDatabaseL(*uri);

            // delete from disk
            PosLmDatabaseManager::DeleteDatabaseL(*uri);
            }

        Server()->ForwardEventToAllSessionsL(event, *uri);
        CleanupStack::PopAndDestroy(uri);
        LmServerGlobal::Complete(aMessage, KErrNone);
        }

    CleanupStack::PopAndDestroy(buf);
    }


// -----------------------------------------------------------------------------
// CPosLmLocalDbManSubsession::HandleDbCopyOperationL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalDbManSubsession::HandleDbCopyOperationL(
    const RMessage2& aMessage)
    {
    TInt srcDesLength = aMessage.GetDesLength(KPosLmServerSourceUriArg);
    TInt trgDesLength = aMessage.GetDesLength(KPosLmServerTargetUriArg);
    if (srcDesLength < 0 || trgDesLength < 0)
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    HBufC* srcUri = HBufC::NewLC(srcDesLength);
    HBufC* trgUri = HBufC::NewLC(trgDesLength);
    TPtr srcUriPtr = srcUri->Des();
    TPtr trgUriPtr = trgUri->Des();

    if (LmServerGlobal::Read(aMessage,
                             KPosLmServerSourceUriArg,
                             srcUriPtr) == KErrNone &&

        LmServerGlobal::Read(aMessage,
                             KPosLmServerTargetUriArg,
                             trgUriPtr) == KErrNone)
        {

        HBufC* sourceUri = PosLmDatabaseUtility::CreateDatabaseUriL(*srcUri);
        CleanupStack::PushL(sourceUri);
        HBufC* targetUri = PosLmDatabaseUtility::CreateDatabaseUriL(*trgUri);
        CleanupStack::PushL(targetUri);

        CheckDiskSizeForCopyL(*sourceUri, *targetUri);

        PosLmDatabaseManager::CopyDatabaseL(*sourceUri, *targetUri);

        // To remove a warning, the event must be default constructed.
        TPosLmDatabaseEvent event =
            {
            EPosLmDbDatabaseRegistered,
            {0,0,0,0,0,0,0,0}
            };

        Server()->ForwardEventToAllSessionsL(event, *targetUri);
        CleanupStack::PopAndDestroy(2, sourceUri); // targetUri

        LmServerGlobal::Complete(aMessage, KErrNone);
        }

    CleanupStack::PopAndDestroy(2, srcUri); // trgUri
    }

// -----------------------------------------------------------------------------
// CPosLmLocalDbManSubsession::CheckDiskSizeL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalDbManSubsession::CheckDiskSizeL(
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
// CPosLmLocalDbManSubsession::CheckDiskSizeForCopyL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalDbManSubsession::CheckDiskSizeForCopyL(
    const TDesC& aSourceUri, const TDesC& aTargetUri)
    {
        // check size of existing database
    RDbs dbSession;
    RDbNamedDatabase db;
    CleanupClosePushL(dbSession);
    CleanupClosePushL(db);

    User::LeaveIfError(dbSession.Connect());
    TPtrC sourceFile = aSourceUri;
    PosLmDatabaseUtility::RemoveProtocolFromUriL(sourceFile);
    User::LeaveIfError(db.Open(dbSession, sourceFile, KPosLmDbSecureFormat));

    TInt err = db.UpdateStats();
    if (err == KErrLocked)
        {
        err = KErrInUse;
        }
    User::LeaveIfError(err);

    TInt size = db.Size().iSize;

    CleanupStack::PopAndDestroy(2, &dbSession);  // db

    CPosLmDiskUtilities* diskUtilities = CPosLmDiskUtilities::NewL();
    CleanupStack::PushL(diskUtilities);

    diskUtilities->DiskSpaceBelowCriticalLevelL(size,
        (aTargetUri)[KPosLocalDbFileProtocol().Length()]);

    CleanupStack::PopAndDestroy(diskUtilities);
    }

// End of File
