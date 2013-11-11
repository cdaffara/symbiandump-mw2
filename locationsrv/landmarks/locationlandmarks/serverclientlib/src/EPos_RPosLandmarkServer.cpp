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
* Description: Implementation of Client API for the landmark server.
*
*
*/


// INCLUDE FILES
#include <badesca.h>
#include <epos_poslmservercommon.h>
#include "EPos_RPosLandmarkServer.h"

// CONSTANTS
const TInt KDefaultMessageSlots = 4;
const TInt KPosLmNumberOfTrials = 5;
const TUid KLandmarksServerUid = { 0x101fdf81 };
const TInt KGranularity = 10;
const TInt KPreAllocLengthURI = 1024;
const TInt KPreAllocLengthDbInfo = 2048;

_LIT(KPosLandmarksServerImg, "EPosLmServer");



// ============================ MEMBER FUNCTIONS ===============================

// C++ default constructor can NOT contain any code, that
// might leave.
//
EXPORT_C RPosLandmarkServer::RPosLandmarkServer() :
    RSessionBase(),
    iExtension(NULL)
    {
    }

// -----------------------------------------------------------------------------
// RPosLandmarkServer::Connect
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RPosLandmarkServer::Connect()
    {
    TBool retrying = ETrue;
    TInt counter = KPosLmNumberOfTrials;
    TInt err;


    while (retrying && counter > 0)
        {
        err = CreateSession(
            KPosLandmarksServerName,
            TVersion(
                KPosLmServerMajorVersionNumber,
                KPosLmServerMinorVersionNumber,
                KPosLmServerBuildVersionNumber),
            KDefaultMessageSlots);

        if (err != KErrNotFound && err != KErrServerTerminated)
            {
            // Session is up.
            retrying = EFalse;
            }
        else if (--counter > 0)
            {
            err = StartServer();

            if (err != KErrAlreadyExists && err != KErrNone)
                {
                // Server returned error code other than
                // KErrAlreadyExists or KErrNone. The client should
                // receive this code.
                retrying = EFalse;
                }
            }
        }

    return err;
    }

// -----------------------------------------------------------------------------
// RPosLandmarkServer::Close
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RPosLandmarkServer::Close()
    {
    RSessionBase::Close();
    }

// -----------------------------------------------------------------------------
// RPosLandmarkServer::ListDatabasesLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CDesCArray* RPosLandmarkServer::ListDatabasesLC(
    const TDesC& aProtocol)
    {
    TInt allocLength = 0;
    CDesCArray* array = new (ELeave) CDesCArrayFlat(KGranularity);
    CleanupStack::PushL(array);

    CleanupStack::PushL(TCleanupItem(*FreeRsc, this));

    HBufC8* buffer = SendMessageLC(EPosLmServerListDatabaseUris,
        aProtocol, KPreAllocLengthURI, allocLength);

    PopulateArrayL(array, buffer);
    CleanupStack::PopAndDestroy(buffer);
    CleanupStack::Pop(); //FreeRsc

    if (allocLength > 0)
        {
        buffer = SendMessageLC(EPosLmServerRemainingDatabaseUris,
                KNullDesC, allocLength, allocLength);
        PopulateArrayL(array, buffer);
        CleanupStack::PopAndDestroy(buffer);
        }
    return array;
    }

// -----------------------------------------------------------------------------
// RPosLandmarkServer::ListDatabasesL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RPosLandmarkServer::ListDatabasesL(
    RPointerArray<HPosLmDatabaseInfo>& aDatabaseInfoArray,
    const TDesC& aProtocol)
    {
    CleanupStack::PushL(TCleanupItem(*FreeRsc, this));

    TInt allocLength = 0;
    HBufC8* buffer = SendMessageLC(EPosLmServerListDatabaseInfo,
        aProtocol, KPreAllocLengthDbInfo, allocLength);

    PopulateArrayL(aDatabaseInfoArray, buffer);
    CleanupStack::PopAndDestroy(buffer);
    CleanupStack::Pop(); //FreeRsc

    if (allocLength > 0)
        {
        buffer = SendMessageLC(EPosLmServerRemainingDatabaseInfo,
                KNullDesC, allocLength, allocLength);
        PopulateArrayL(aDatabaseInfoArray, buffer);
        CleanupStack::PopAndDestroy(buffer);
        }
    }

// -----------------------------------------------------------------------------
// RPosLandmarkServer::RegisterDatabase
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RPosLandmarkServer::RegisterDatabase(
    HPosLmDatabaseInfo& aDatabaseInfo)
    {
    TInt size = aDatabaseInfo.Size();
    TPtr8 ptr(reinterpret_cast<TUint8*>(&aDatabaseInfo), size, size);

    return SendReceive(EPosLmServerRegisterDatabase, TIpcArgs(&ptr));
    }

// -----------------------------------------------------------------------------
// RPosLandmarkServer::UnregisterDatabase
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RPosLandmarkServer::UnregisterDatabase(
    const TDesC& aDatabaseUri,
    TChar aDriveLetter)
    {
    TPckgC<TChar> drivePckg(aDriveLetter);
    return SendReceive(EPosLmServerUnregisterDatabase,
        TIpcArgs(&aDatabaseUri, &drivePckg));
    }

// -----------------------------------------------------------------------------
// RPosLandmarkServer::UnregisterAllDatabases
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RPosLandmarkServer::UnregisterAllDatabases(
    const TDesC& aProtocol)
    {
    return SendReceive(EPosLmServerUnregisterAllDatabases,
        TIpcArgs(&aProtocol));
    }

// -----------------------------------------------------------------------------
// RPosLandmarkServer::ModifyDatabaseSettings
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RPosLandmarkServer::ModifyDatabaseSettings(
    const TDesC& aDatabaseUri,
    const TPosLmDatabaseSettings& aDatabaseSettings,
    TChar aDriveLetter)
    {
    TPckgC<TChar> drivePckg(aDriveLetter);
    TPckgC<TPosLmDatabaseSettings> settingsPckg(aDatabaseSettings);

    return SendReceive(EPosLmServerModifyDatabaseSettings,
        TIpcArgs(&aDatabaseUri, &drivePckg, &settingsPckg));
    }

// -----------------------------------------------------------------------------
// RPosLandmarkServer::GetDatabaseInfo
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RPosLandmarkServer::GetDatabaseInfo(
    HPosLmDatabaseInfo& aDatabaseInfo)
    {
    TInt size = aDatabaseInfo.Size();
    TPtr8 ptr(reinterpret_cast<TUint8*>(&aDatabaseInfo), size, size);

    return SendReceive(
        EPosLmServerReadDatabaseSettings, TIpcArgs(&ptr));
    }

// -----------------------------------------------------------------------------
// RPosLandmarkServer::ReportEvent
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RPosLandmarkServer::ReportEvent(
    const TPosLmDatabaseEvent& aEvent,
    const TDesC& aDatabaseUri)
    {
    TPckgC<TPosLmDatabaseEvent> eventPckg(aEvent);

    return SendReceive(EPosLmServerReportDbEvent,
        TIpcArgs(&aDatabaseUri, &eventPckg));
    }

// -----------------------------------------------------------------------------
// RPosLandmarkServer::StartServer
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RPosLandmarkServer::StartServer()
    {
    TRequestStatus started;

    const TUidType serverUid(KNullUid, KNullUid, KLandmarksServerUid);

    /**
    EPOC is easy, we just create a new server process. Simultaneous launching
    of two such processes should be detected when the second one attempts to
    create the server object, failing with KErrAlreadyExists.
    **/
    RProcess server;
    TInt ret = server.Create(KPosLandmarksServerImg, KNullDesC, serverUid);

    if (ret != KErrNone)
        {
        return ret;
        }

    server.Rendezvous(started);
    //
    // logon OK - start the server
    server.Resume();

    User::WaitForRequest(started);  // wait for start or death

    server.Close();

    return started.Int();
    }

// -----------------------------------------------------------------------------
// RPosLandmarkServer::SendMessageLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC8* RPosLandmarkServer::SendMessageLC(
    TInt aFunction,
    const TDesC& aDes,
    TInt aLength,
    TInt& aAllocLength)
    {
    HBufC8* buffer = HBufC8::NewLC(aLength);
    TPtr8 bufPtr = buffer->Des();

    TPckg<TInt> allocPckg(aAllocLength);
    User::LeaveIfError(SendReceive(aFunction,
        TIpcArgs(&aDes, &bufPtr, &allocPckg)));
    return buffer;
    }

// -----------------------------------------------------------------------------
// RPosLandmarkServer::PopulateArrayL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RPosLandmarkServer::PopulateArrayL(
    CDesCArray* aArray,
    HBufC8* aBuffer)
    {
    if (aBuffer->Length() > 0)
        {
        TUint8* bufPointer = const_cast<TUint8*>(aBuffer->Ptr());
        const TUint8* endPointer = bufPointer + aBuffer->Size();
        while (bufPointer < endPointer)
            {
            TPckgBuf<TInt> intPckg;
            intPckg.Copy(bufPointer, intPckg.MaxLength());

            TInt uriLength = intPckg();
            TInt uriSize = uriLength * 2;
            bufPointer += intPckg.Size();
            HBufC* uri = HBufC::NewLC(uriSize);
            uri->Des().SetLength(uriLength);

            TPtr8 uriPtr(reinterpret_cast<TUint8*>(
                const_cast<TUint16*>(uri->Ptr())), uriSize, uriSize);
            uriPtr.Copy(bufPointer, uriSize);
            bufPointer += uriSize;

            aArray->AppendL(*uri);
            CleanupStack::PopAndDestroy(uri);
            }
        }
    }

// -----------------------------------------------------------------------------
// RPosLandmarkServer::PopulateArrayL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RPosLandmarkServer::PopulateArrayL(
    RPointerArray<HPosLmDatabaseInfo>& aDatabaseInfoArray,
    HBufC8* aBuffer)
    {
    if (aBuffer->Length() > 0)
        {
        TInt currentLength = 0;
        TInt bufferLength = aBuffer->Length();
        do  {
            TUint8* infoPtr =
                const_cast<TUint8*>(aBuffer->Ptr() + currentLength);
            HPosLmDatabaseInfo* dbInfo = HPosLmDatabaseInfo::NewLC(
                *(reinterpret_cast<HPosLmDatabaseInfo*>(infoPtr)));

            aDatabaseInfoArray.AppendL(dbInfo);
            currentLength += dbInfo->Size();
            CleanupStack::Pop(dbInfo);
            } while (currentLength < bufferLength);
        }
    }

// -----------------------------------------------------------------------------
// RPosLandmarkServer::FreeResources
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RPosLandmarkServer::FreeResources()
    {
    SendReceive(EPosLmServerFreeResources, TIpcArgs(TIpcArgs::ENothing));
    }

// -----------------------------------------------------------------------------
// RPosLandmarkServer::FreeRsc
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RPosLandmarkServer::FreeRsc(TAny* aParam)
    {
    reinterpret_cast<RPosLandmarkServer*>(aParam)->FreeResources();
    }

//  End of File
