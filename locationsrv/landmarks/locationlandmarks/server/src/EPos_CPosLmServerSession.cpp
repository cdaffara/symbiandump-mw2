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
* Description:  Landmarks Server session class.
*
*/


#include <epos_cposlmsettingshandler.h>
#include <epos_poslmdatabaseutility.h>
#include <EPos_CPosLmDbRegistry.h>
#include <EPos_CPosLmDatabaseIterator.h>
#include <EPos_HPosLmDatabaseInfo.h>
#include <epos_cposlmdiskutilities.h>

#include "epos_lmdebug.h"
#include "EPos_CPosLmServer.h"
#include "EPos_PosLmServerCommon.h"
#include "EPos_LmServerGlobal.h"
#include "EPos_LandmarksServerPanics.h"
#include "EPos_CPosLmSubsessionBase.h"
#include "EPos_CPosLmSubsessionRegistry.h"
#include "EPos_CPosLmLocalAccessSubsession.h"
#include "EPos_CPosLmEventListenerSubsession.h"
#include "EPos_CPosLmLocalDbManSubsession.h"
#include "EPos_CPosLmServerSession.h"

const TInt KArrayGranularity  = 5;
const TChar KDriveUnspecified = 0;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmServerSession::CPosLmServerSession(
    CPosLmServer& aServer) :
    CSession2(),
    iLmServer(aServer)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::ConstructL()
    {
    iLmServer.LeaveIfBackupIsRunningL();
    // This call will also cancel any server shutdown in progress.

    iLmServer.IncrementSessions();
    iDecrementSessions = ETrue;

    iSubsessionRegistry = CPosLmSubsessionRegistry::NewL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmServerSession* CPosLmServerSession::NewL(
    CPosLmServer& aServer)
    {
    CPosLmServerSession* self =
        new( ELeave ) CPosLmServerSession(aServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmServerSession::~CPosLmServerSession()
    {
    iDbInfoArray.ResetAndDestroy();
    iDbInfoArray.Close();
    delete iDesArray;
    iLmServer.RemoveSessionMessages(this);

    if (iDecrementSessions)
        {
        iLmServer.DecrementSessions();
        }
    delete iSubsessionRegistry;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmServerSession::HasWriteLock(
    const TDesC& aUri)
    {
    TBool hasLock = EFalse;

    TInt count = iSubsessionRegistry->Count();
    for (TInt i = 0; i < count && !hasLock; i++)
        {
        CPosLmSubsessionBase* subsession =
            iSubsessionRegistry->SubsessionFromIndex(i);
        if (subsession->SubsessionType() ==
            CPosLmSubsessionBase::EPosLocalAccessSubsession)
            {
            hasLock = (static_cast<CPosLmLocalAccessSubsession*>(subsession))->
                HasWriteLock(aUri);
            }
        }

    return hasLock;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmServerSession::HasAnyLock(
    const TDesC& aUri)
    {
    TBool hasLock = EFalse;

    TInt count = iSubsessionRegistry->Count();
    for (TInt i = 0; i < count && !hasLock; i++)
        {
        CPosLmSubsessionBase* subsession =
            iSubsessionRegistry->SubsessionFromIndex(i);
        if (subsession->SubsessionType() ==
            CPosLmSubsessionBase::EPosLocalAccessSubsession)
            {
            hasLock = (static_cast<CPosLmLocalAccessSubsession*>(subsession))->
                HasAnyLock(aUri);
            }
        }

    return hasLock;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::HandleOperationEventL(
    TPosLmEvent aEvent,
    const TDesC& aUri,
    TBool aCheckUri)
    {
    TInt count = iSubsessionRegistry->Count();
    for (TInt i = 0; i < count; i++)
        {
        CPosLmSubsessionBase* subsession =
            iSubsessionRegistry->SubsessionFromIndex(i);
        if (subsession->SubsessionType() ==
            CPosLmSubsessionBase::EPosLocalAccessSubsession)
            {
            (static_cast<CPosLmLocalAccessSubsession*>(subsession))->
                HandleOperationEventL(aEvent, aUri, aCheckUri);
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::HandleOperationEventL(
    TPosLmDatabaseEvent aEvent,
    const TDesC& aUri)
    {
    TInt count = iSubsessionRegistry->Count();
    for (TInt i = 0; i < count; i++)
        {
        CPosLmSubsessionBase* subsession =
            iSubsessionRegistry->SubsessionFromIndex(i);
        if (subsession->SubsessionType() ==
            CPosLmSubsessionBase::EPosEventListenerSubsession)
            {
            (static_cast<CPosLmEventListenerSubsession*>(subsession))->
                HandleOperationEventL(aEvent, aUri);
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::CompleteOutstandingEventRequest(
    TInt aError)
    {
    TInt count = iSubsessionRegistry->Count();
    for (TInt i = 0; i < count; i++)
        {
        CPosLmSubsessionBase* subsession =
            iSubsessionRegistry->SubsessionFromIndex(i);
        if (subsession->SubsessionType() ==
            CPosLmSubsessionBase::EPosEventListenerSubsession)
            {
            (static_cast<CPosLmEventListenerSubsession*>(subsession))->
                CompleteOutstandingEventRequest(aError);
            }
        else if (subsession->SubsessionType() ==
            CPosLmSubsessionBase::EPosLocalAccessSubsession)
            {
            (static_cast<CPosLmLocalAccessSubsession*>(subsession))->
                CompleteOutstandingEventRequest(aError);
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::ServiceL(
    const RMessage2& aMessage)
    {
    iLmServer.LeaveIfBackupIsRunningL();

    TRAPD(err, DispatchMessageL(aMessage));

    if (err != KErrNone)
        {
        LOG2("ServerSession::ServiceL: message %d failed with %d", aMessage.Function(), err);
        LmServerGlobal::Complete(aMessage, err);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::DispatchMessageL(
    const RMessage2& aMessage)
    {
    switch (aMessage.Function())
        {
        // subsession management
        case EPosLmServerRegisterLocalAccessSubsession:
            CreateLocalAccessSubsessionL(aMessage);
            break;
        case EPosLmServerRegisterEventListenerSubsession:
            CreateEventListenerSubsessionL(aMessage);
            break;
        case EPosLmServerRegisterLocalDbManSubsession:
            CreateLocalDbManSubsessionL(aMessage);
            break;
        case EPosLmServerUnregisterSubsession:
            CloseSubsession(aMessage);
            break;

        // server API
        case EPosLmServerRegisterDatabase:
            RegisterDatabaseL(aMessage);
            break;
        case EPosLmServerUnregisterDatabase:
            UnregisterDatabaseL(aMessage);
            break;
        case EPosLmServerUnregisterAllDatabases:
            UnregisterAllDatabasesL(aMessage);
            break;
        case EPosLmServerModifyDatabaseSettings:
            ModifyDatabaseSettingsL(aMessage);
            break;
        case EPosLmServerReadDatabaseSettings:
            ReadDatabaseSettingsL(aMessage);
            break;
        case EPosLmServerListDatabaseUris:
            ListDatabaseUrisL(aMessage);
            break;
        case EPosLmServerRemainingDatabaseUris:
            RemainingDatabaseUrisL(aMessage);
            break;
        case EPosLmServerListDatabaseInfo:
            ListDatabaseInfoL(aMessage);
            break;
        case EPosLmServerRemainingDatabaseInfo:
            RemainingDatabaseInfoL(aMessage);
            break;
        case EPosLmServerReportDbEvent:
            ReportEventL(aMessage);
            break;
        case EPosLmServerFreeResources:
            FreeResources(aMessage);
            break;
        default:
            ForwardToSubsessionL(aMessage);
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::AddSubsessionToRegistryL(
    CPosLmSubsessionBase* aSubsession,
    const RMessage2& aMessage)
    {
    TInt subsessionHandle = iSubsessionRegistry->AddInstanceL(aSubsession);

    // Note: Nothing must leave in the remainder of this method, since
    // there is double ownership of the subsession object.

    // Set the client subsession identifier.
    TPckg<TInt> handlePackage(subsessionHandle);

    if (LmServerGlobal::Write(aMessage, 3, handlePackage) != KErrNone)
        {
        // Don't bother about the return code. This client has already been
        // panicked.
        iSubsessionRegistry->CloseSubsession(subsessionHandle);
        }
    else
        {
        LmServerGlobal::Complete(aMessage, KErrNone);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::CloseSubsession(
    const RMessage2& aMessage)
    {
    LmServerGlobal::Complete(aMessage,
        iSubsessionRegistry->CloseSubsession(aMessage.Int3()));
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::ForwardToSubsessionL(
    const RMessage2& aMessage)
    {
    CPosLmSubsessionBase* subsession =
        iSubsessionRegistry->SubsessionFromHandleL(aMessage.Int3());

    if (!subsession)
        {
        User::Leave(KErrNotSupported);
        }

    subsession->ServiceL(aMessage);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::CreateLocalAccessSubsessionL(
    const RMessage2& aMessage)
    {
    CPosLmLocalAccessSubsession* subsession =
        CPosLmLocalAccessSubsession::NewLC(&iLmServer);
    AddSubsessionToRegistryL(subsession, aMessage);
    CleanupStack::Pop(subsession); // Ownership transferred
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::CreateEventListenerSubsessionL(
    const RMessage2& aMessage)
    {
    CPosLmEventListenerSubsession* subsession =
        CPosLmEventListenerSubsession::NewLC(&iLmServer);
    AddSubsessionToRegistryL(subsession, aMessage);
    CleanupStack::Pop(subsession); // Ownership transferred
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::CreateLocalDbManSubsessionL(
    const RMessage2& aMessage)
    {
    CPosLmLocalDbManSubsession* subsession =
        CPosLmLocalDbManSubsession::NewLC(&iLmServer);
    AddSubsessionToRegistryL(subsession, aMessage);
    CleanupStack::Pop(subsession); // Ownership transferred
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::RegisterDatabaseL(const RMessage2& aMessage)
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
        TUint8* dbInfoPtr = const_cast<TUint8*>(buf->Ptr());
        HPosLmDatabaseInfo* dbInfo =
            reinterpret_cast<HPosLmDatabaseInfo*>(dbInfoPtr);

        TChar drive = GetSystemDriveChar();
        if (dbInfo->DatabaseDrive() != 0)
            {
            drive = dbInfo->DatabaseDrive();
            }

        CPosLmDbRegistry* reg = iLmServer.GetRegistryL(drive, ETrue);
        if (reg)
            {
            reg->RegisterDatabaseL(dbInfo->DatabaseUri(), dbInfo->Settings());
            User::LeaveIfError(reg->Compact());
            }
        LmServerGlobal::Complete(aMessage, KErrNone);
        }
    CleanupStack::PopAndDestroy(buf);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::UnregisterDatabaseL(const RMessage2& aMessage)
    {
    TInt uriLength = aMessage.GetDesLength(KPosLmServerUriArg);
    TInt driveLength = aMessage.GetDesLength(KPosLmServerDriveArg);

    if (uriLength < 0 || driveLength < 0)
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    HBufC* uri = HBufC::NewLC(uriLength);
    TPtr uriPtr = uri->Des();
    TChar drive;
    TPckg<TChar> drivePckg(drive);

    if (LmServerGlobal::Read(
        aMessage, KPosLmServerUriArg, uriPtr) == KErrNone &&
        LmServerGlobal::Read(
            aMessage, KPosLmServerDriveArg, drivePckg) == KErrNone)
        {
        if (drive == 0)
            {
            drive = GetSystemDriveChar();
            }

        CPosLmDbRegistry* reg = iLmServer.GetRegistryL(drive);
        if (reg)
            {
            reg->UnregisterDatabaseL(*uri);
            User::LeaveIfError(reg->Compact());
            }
        LmServerGlobal::Complete(aMessage, KErrNone);
        }

    CleanupStack::PopAndDestroy(uri);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::UnregisterAllDatabasesL(const RMessage2& aMessage)
    {
    TInt protocolLength = aMessage.GetDesLength(KPosLmServerProtocolArg);
    if (protocolLength < 0)
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    HBufC* protocol = HBufC::NewLC(protocolLength);
    TPtr protPtr = protocol->Des();

    if (LmServerGlobal::Read(
        aMessage, KPosLmServerProtocolArg, protPtr) != KErrNone)
        {
        CleanupStack::PopAndDestroy(protocol);
        return;
        }

    TDriveList driveList;
    User::LeaveIfError(iLmServer.FileSession().DriveList(driveList));

    for (TInt driveNumber = EDriveA; driveNumber <= EDriveY; driveNumber++)
        {
        if ( driveList[driveNumber] && !( driveList[driveNumber] & KDriveAttRemote ) ) // avoid remote drives
            {
	        TDriveInfo drvInfo;
	        TInt err = iLmServer.FileSession().Drive( drvInfo, driveNumber );

            if ( !err && drvInfo.iType != EMediaNotPresent )
	            {
	            TDriveUnit unit(driveNumber);
	            TChar drive((unit.Name())[0]);
	            CPosLmDbRegistry* reg = iLmServer.GetRegistryL(drive);
	            if (reg)
	                {
	                reg->UnregisterAllL(*protocol);
	                User::LeaveIfError(reg->Compact());
	                }
	            }
			}
        }

    LmServerGlobal::Complete(aMessage, KErrNone);
    CleanupStack::PopAndDestroy(protocol);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::ModifyDatabaseSettingsL(const RMessage2& aMessage)
    {
    TInt uriLength = aMessage.GetDesLength(KPosLmServerUriArg);
    TInt driveLength = aMessage.GetDesLength(KPosLmServerDriveArg);
    TInt settingsLength = aMessage.GetDesLength(KPosLmServerDbSettingsArg);

    if (uriLength < 0 || driveLength < 0 || settingsLength < 0 )
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    HBufC* uri = HBufC::NewLC(uriLength);
    TPtr uriPtr = uri->Des();
    TChar drive;
    TPckg<TChar> drivePckg(drive);
    TPosLmDatabaseSettings settings;
    TPckg<TPosLmDatabaseSettings> settingsPckg(settings);

    if (LmServerGlobal::Read(
            aMessage, KPosLmServerUriArg, uriPtr) == KErrNone &&
        LmServerGlobal::Read(
            aMessage, KPosLmServerDriveArg, drivePckg) == KErrNone &&
        LmServerGlobal::Read(
            aMessage, KPosLmServerDbSettingsArg, settingsPckg) == KErrNone)
        {
        if (drive == 0)
            {
            drive = GetSystemDriveChar();
            }

        CPosLmDbRegistry* reg = iLmServer.GetRegistryL(drive);
        if (!reg)
            {
            User::Leave(KErrNotFound);
            }
        reg->ModifyDatabaseSettingsL(*uri, settings);

        User::LeaveIfError(reg->Compact());
        LmServerGlobal::Complete(aMessage, KErrNone);
        }

    CleanupStack::PopAndDestroy(uri);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::ReadDatabaseSettingsL(const RMessage2& aMessage)
    {
    TInt dbInfoLength = aMessage.GetDesLength(KPosLmServerDbInfoArg);
    if (dbInfoLength < 0)
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    HBufC8* buf = HBufC8::NewLC(dbInfoLength);
    TPtr8 ptr = buf->Des();

    if (LmServerGlobal::Read(aMessage, KPosLmServerDbInfoArg, ptr) == KErrNone)
        {
        TUint8* pointer = const_cast<TUint8*>(buf->Ptr());
        HPosLmDatabaseInfo* dbInfo =
            reinterpret_cast<HPosLmDatabaseInfo*>(pointer);

        TChar drive = GetSystemDriveChar();
        if (dbInfo->DatabaseDrive() != 0)
            {
            drive = dbInfo->DatabaseDrive();
            }

        CPosLmDbRegistry* reg = iLmServer.GetRegistryL(drive);
        if (!reg)
            {
            User::Leave(KErrNotFound);
            }

        TPtrC uri = dbInfo->DatabaseUri();
        reg->ReadDatabaseSettingsL(uri, dbInfo->Settings());

        if (LmServerGlobal::Write(aMessage, KPosLmServerDbInfoArg, *buf)
            == KErrNone)
            {
            LmServerGlobal::Complete(aMessage, KErrNone);
            }
        }
    CleanupStack::PopAndDestroy(buf);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::ListDatabaseUrisL(const RMessage2& aMessage)
    {
    TInt protocolLength = aMessage.GetDesLength(KPosLmServerProtocolArg);
    TInt bufferLength = aMessage.GetDesMaxLength(KPosLmServerBufferArg);

    if (protocolLength < 0 || bufferLength < 0)
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    HBufC* protocol = HBufC::NewLC(protocolLength);
    TPtr protPtr = protocol->Des();

    if (LmServerGlobal::Read(
        aMessage, KPosLmServerProtocolArg, protPtr) != KErrNone)
        {
        CleanupStack::PopAndDestroy(protocol);
        return;
        }

    HBufC8* buffer = HBufC8::NewLC(bufferLength);
    TPtr8 bufPtr = buffer->Des();
    TInt allocLength = 0;
    TDriveList driveList;

    User::LeaveIfError(iLmServer.FileSession().DriveList(driveList));
    for (TInt driveNumber = EDriveA; driveNumber <= EDriveZ; driveNumber++)
        {
        if ( driveList[driveNumber] && !( driveList[driveNumber] & KDriveAttRemote ) ) // avoid remote drives
            {
            TDriveInfo drvInfo;
            TInt err = iLmServer.FileSession().Drive( drvInfo, driveNumber );

            if ( !err && drvInfo.iType != EMediaNotPresent )
                {
                TDriveUnit unit(driveNumber);
                TChar drive((unit.Name())[0]);
                CPosLmDbRegistry* reg = iLmServer.GetRegistryL(drive);
                if (reg)
                    {
                    CPosLmDatabaseIterator* iter = reg->ListDatabasesL(*protocol);
                    CleanupStack::PushL(iter);

                    while (iter->NextItemL())
                        {
                        TPtrC uri = iter->UriL();
                        TPckgC<TInt> intPckg(uri.Length());

                        TInt pckgLength = intPckg.Length();
                        TInt nextItemLength = pckgLength + uri.Length() * 2;

                        if ((nextItemLength + buffer->Length()) >
                            bufPtr.MaxLength())
                            {
                            if (!iDesArray)
                                {
                                iDesArray =
                                    new (ELeave) CDesCArrayFlat(KArrayGranularity);
                                }
                            iDesArray->AppendL(uri);
                            allocLength += nextItemLength;
                            }
                        else
                            {
                            TInt uriSize = uri.Size();
                            TPtr8 uriPtr(reinterpret_cast<TUint8*>(
                                const_cast<TUint16*>(uri.Ptr())), uriSize, uriSize);

                            bufPtr.Append(intPckg);
                            bufPtr.Append(uriPtr);
                            }
                        }
                    CleanupStack::PopAndDestroy(iter);
                    }
                }
            }
        }

    TPckg<TInt> allocPckg(allocLength);
    if (LmServerGlobal::Write(
            aMessage, KPosLmServerBufferArg, *buffer) == KErrNone &&
        LmServerGlobal::Write(
            aMessage, KPosLmServerAllocLengthArg, allocPckg) == KErrNone)
        {
        LmServerGlobal::Complete(aMessage, KErrNone);
        }

    CleanupStack::PopAndDestroy(2, protocol); //buffer
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::RemainingDatabaseUrisL(const RMessage2& aMessage)
    {
    if ( !iDesArray || (iDesArray && iDesArray->Count() == 0) )
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    TInt bufferLength = aMessage.GetDesMaxLength(KPosLmServerBufferArg);

    if (bufferLength < 0)
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    HBufC8* buffer = HBufC8::NewLC(bufferLength);
    TPtr8 bufPtr = buffer->Des();

    TInt sumUriSize = 0;
    for (TInt i = 0; i < iDesArray->Count(); i++)
        {
        TInt uriLength = (*iDesArray)[i].Length();
        TPckgC<TInt> intPckg(uriLength);

        sumUriSize += intPckg.Size() + (*iDesArray)[i].Size();
        if (sumUriSize > bufferLength)
            {
            iDesArray->Reset();
            CleanupStack::PopAndDestroy(buffer);
            PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
            return;
            }

        TInt uriSize = (*iDesArray)[i].Size();
        TPtr8 uriPtr(reinterpret_cast<TUint8*>(
            const_cast<TUint16*>((*iDesArray)[i].Ptr())), uriSize, uriSize);

        bufPtr.Append(intPckg);
        bufPtr.Append(uriPtr);
        }

    if (LmServerGlobal::Write(
            aMessage, KPosLmServerBufferArg, *buffer) == KErrNone)
        {
        LmServerGlobal::Complete(aMessage, KErrNone);
        iDesArray->Reset();
        delete iDesArray;
        iDesArray = NULL;
        }
    CleanupStack::PopAndDestroy(buffer);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::ListDatabaseInfoL(const RMessage2& aMessage)
    {
    TInt protocolLength = aMessage.GetDesLength(KPosLmServerProtocolArg);
    TInt bufferLength = aMessage.GetDesMaxLength(KPosLmServerBufferArg);

    if (protocolLength < 0 || bufferLength < 0)
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    HBufC* protocol = HBufC::NewLC(protocolLength);
    TPtr protPtr = protocol->Des();

    if (LmServerGlobal::Read(
        aMessage, KPosLmServerProtocolArg, protPtr) != KErrNone)
        {
        CleanupStack::PopAndDestroy(protocol);
        return;
        }

    HBufC8* buffer = HBufC8::NewLC(bufferLength);
    TPtr8 bufPtr = buffer->Des();

    TInt allocLength = 0;
    TInt sumInfoSize = 0;
    TDriveList driveList;

    User::LeaveIfError(iLmServer.FileSession().DriveList(driveList));
    for (TInt driveNumber = EDriveA; driveNumber <= EDriveZ; driveNumber++)
        {
        if ( driveList[driveNumber] && !( driveList[driveNumber] & KDriveAttRemote ) ) // avoid remote drives
            {
            TDriveInfo drvInfo;
            TInt err = iLmServer.FileSession().Drive( drvInfo, driveNumber );

            if ( !err && drvInfo.iType != EMediaNotPresent )
                {
                TDriveUnit unit(driveNumber);
                TChar drive((unit.Name())[0]);
                CPosLmDbRegistry* reg = iLmServer.GetRegistryL(drive);
                if (reg)
                    {
                    CPosLmDatabaseIterator* iter = reg->ListDatabasesL(*protocol);
                    CleanupStack::PushL(iter);

                    while (iter->NextItemL())
                        {
                        HPosLmDatabaseInfo* dbInfo =
                            HPosLmDatabaseInfo::NewLC(iter->UriL());
                        iter->GetSettingsL(dbInfo->Settings());

                        sumInfoSize += dbInfo->Size();
                        if (sumInfoSize > bufPtr.MaxLength())
                            {
                            iDbInfoArray.AppendL(dbInfo);
                            allocLength += dbInfo->Size();
                            CleanupStack::Pop(dbInfo);
                            }
                        else
                            {
                            TUint8* pointer = reinterpret_cast<TUint8*>(dbInfo);
                            bufPtr.Append(pointer, dbInfo->Size());
                            CleanupStack::PopAndDestroy(dbInfo);
                            }
                        }
                    CleanupStack::PopAndDestroy(iter);
                    }
                }
            }
        }

    TPckg<TInt> allocPckg(allocLength);
    if (LmServerGlobal::Write(
            aMessage, KPosLmServerBufferArg, *buffer) == KErrNone &&
        LmServerGlobal::Write(
            aMessage, KPosLmServerAllocLengthArg, allocPckg) == KErrNone)
        {
        LmServerGlobal::Complete(aMessage, KErrNone);
        }

    CleanupStack::PopAndDestroy(2, protocol); //buffer
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::RemainingDatabaseInfoL(const RMessage2& aMessage)
    {
    if (iDbInfoArray.Count() == 0)
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    TInt bufferLength = aMessage.GetDesMaxLength(KPosLmServerBufferArg);

    if (bufferLength < 0)
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    HBufC8* buffer = HBufC8::NewLC(bufferLength);

    TInt infoLength = 0;
    for (TInt i = 0; i < iDbInfoArray.Count(); i++)
        {
        HPosLmDatabaseInfo* dbInfo = iDbInfoArray[i];
        TUint8* pointer = reinterpret_cast<TUint8*>(dbInfo);

        infoLength += dbInfo->Size();
        if (infoLength > bufferLength)
            {
            iDbInfoArray.ResetAndDestroy();
            CleanupStack::PopAndDestroy(buffer);
            PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
            return;
            }

        buffer->Des().Append(pointer, dbInfo->Size());
        }

    if (LmServerGlobal::Write(
            aMessage, KPosLmServerBufferArg, *buffer) == KErrNone)
        {
        LmServerGlobal::Complete(aMessage, KErrNone);
        }

    iDbInfoArray.ResetAndDestroy();
    CleanupStack::PopAndDestroy(buffer);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::ReportEventL(const RMessage2& aMessage)
    {
    TInt uriLength = aMessage.GetDesLength(KPosLmServerUriArg);
    TInt dbEventLength = aMessage.GetDesLength(KPosLmServerDbEventArg);

    if (uriLength < 0 || dbEventLength < 0)
        {
        PanicClient(aMessage, EPosUnableToReadOrWriteDataToClient);
        return;
        }

    HBufC* uri = HBufC::NewLC(uriLength);
    TPtr uriPtr = uri->Des();

    TPckgBuf<TPosLmDatabaseEvent> event;

    if (LmServerGlobal::Read(
            aMessage, KPosLmServerUriArg, uriPtr) == KErrNone &&
        LmServerGlobal::Read(
            aMessage, KPosLmServerDbEventArg, event) == KErrNone)
        {
        iLmServer.ForwardEventToAllSessionsL(event(), uri->Des());
        LmServerGlobal::Complete(aMessage, KErrNone);
        }
    CleanupStack::PopAndDestroy(uri);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmServerSession::FreeResources(const RMessage2& aMessage)
    {
    delete iDesArray;
    iDesArray = NULL;
    iDbInfoArray.ResetAndDestroy();
    LmServerGlobal::Complete(aMessage, KErrNone);
    }

TChar CPosLmServerSession::GetSystemDriveChar()
{
	return 'A' + RFs::GetSystemDrive();
	
}
