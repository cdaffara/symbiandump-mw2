/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
 * Description: client-side interface implementation for server session.
 *  
 */

#include "contactresolversession.h"
#include "locprivacyinternal.h"
#include "locprivacycommon.h"
#include "locutilsdebug.h"

#include <lbs/epos_cposrequestor.h>
#include <lbs/EPos_RPosRequestorStack.h>
#include <e32cmn.h>
#include <s32strm.h>
#include <s32mem.h>

//This determines the number of outstanding requests the client may have with the server at any one time. 
//The maximum number of slots is 255.
TInt KDefaultMessageSlots = 255;
const TUid KServerUid3 =
    {
    0x101f7a86
    };
_LIT(KServerFilename, "locnotificationserver.exe");

// -----------------------------------------------------------------------------
// RContactResolverSession::RContactResolverSession()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C RContactResolverSession::RContactResolverSession() :
    RSessionBase()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// RContactResolverSession::ResolveRequestorsL()
// Issues a request for the time to the server.
// -----------------------------------------------------------------------------
//
EXPORT_C void RContactResolverSession::ResolveRequestorsL(RPointerArray<
        CPosRequestor>& aRequestors)
    {

     LOCUTILSDEBUG( "+RContactResolverSession::ResolveRequestorsL" )
     
    RPosRequestorStack* requestors = new (ELeave) RPosRequestorStack;
    CleanupStack::PushL(requestors);

    //-------------------------------------------------------------
    // getting size from the server in first IPC
    CBufFlat* buffer = CBufFlat::NewL(512);
    CleanupStack::PushL(buffer);
    RBufWriteStream writeStream;
    writeStream.Open(*buffer);
    CleanupClosePushL(writeStream);

    TInt count = aRequestors.Count();
    for (TInt i = 0; i < count; ++i)
        {
        requestors->Append(aRequestors[i]);
        }
    requestors->ExternalizeL(writeStream);
    writeStream.CommitL();

    TPtr8 ptr = buffer->Ptr(0);

    TIpcArgs args;
    TInt size = 0;
    TPckg<TInt> sizePkg(size);
    args.Set(0, &sizePkg);
    args.Set(1, &ptr);

    TInt in = SendReceive(ELocPrivacyGetSize, args);

    CleanupStack::PopAndDestroy(&writeStream);
    CleanupStack::PopAndDestroy(buffer);
    CleanupStack::PopAndDestroy(requestors);
    //-------------------------------------------------------------
    //-------------------------------------------------------------
    // allocating the buffer of the size obtained in the first IPC
    // and getting the data from the server in the 2nd IPC

    // This call waits for the server to complete the request before
    // proceeding. When it returns, the new time will be in aTime.


    CBufFlat* buffer1 = CBufFlat::NewL(512);
    CleanupStack::PushL(buffer1);
    buffer1->ResizeL(size);
    
    TPtr8 bufPtr = buffer1->Ptr(0);
    TIpcArgs ipcArgs;
    ipcArgs.Set(0, &bufPtr);
    in = SendReceive(ELocPrivacyResolve, ipcArgs);
    
    //-------------------------------------------------------------

    RBufReadStream readStream;
    readStream.Open(*buffer1);
    CleanupClosePushL(readStream);
    RPosRequestorStack* requestors2 = new (ELeave) RPosRequestorStack;
    CleanupStack::PushL(requestors2);
    requestors2->InternalizeL(readStream);
    TInt cnt = requestors2->Count();
    aRequestors.Reset();
    for (TInt i = 0; i < cnt; ++i)
        {
        CPosRequestor * entry = requestors2->operator [](i);
        aRequestors.Append(entry);
        }
    CleanupStack::PopAndDestroy(requestors2);
    CleanupStack::PopAndDestroy(&readStream);
    CleanupStack::PopAndDestroy(buffer1);
 
    LOCUTILSDEBUG( "-RContactResolverSession::ResolveRequestorsL" )
    }

// -----------------------------------------------------------------------------
// RContactResolverSession::Connect()
// Connects to the server and create a session.
// -----------------------------------------------------------------------------

EXPORT_C TInt RContactResolverSession::Connect()
    {
    LOCUTILSDEBUG( "+RContactResolverSession::Connect" )
    
    TInt error = StartServer();

    if (KErrNone == error || KErrAlreadyExists == error )
        {
        error = CreateSession(KLocPrivacyServerName, Version(),
                KDefaultMessageSlots);
        }
    LOCUTILSDEBUG( "-RContactResolverSession::Connect" )
    return error;
    }

// -----------------------------------------------------------------------------
// StartServer()
// Starts the server if it is not already running
// -----------------------------------------------------------------------------
//
TInt RContactResolverSession::StartServer()
    {
    LOCUTILSDEBUG( "+RContactResolverSession::StartServer" )
    
    TInt result;

    TFindServer findServer(KLocPrivacyServerName);
    TFullName name;

    result = findServer.Next(name);
    if (result == KErrNone)
        {
        // Server already running
        return KErrNone;
        }

    result = CreateServerProcess();
    if (result != KErrNone)
        {
        return result;
        }
    LOCUTILSDEBUG( "-RContactResolverSession::StartServer" )
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CreateServerProcess()
// Creates a server process
// -----------------------------------------------------------------------------
//
TInt RContactResolverSession::CreateServerProcess()
    {
    LOCUTILSDEBUG( "+RContactResolverSession::CreateServerProcess" )
    
    TInt result;

    const TUidType serverUid(KNullUid, KNullUid, KServerUid3);

    RProcess server;
    TRequestStatus status;
    result = server.Create(KServerFilename, KNullDesC, serverUid);

    if (result != KErrNone)
        {
        server.Close();
        return result;
        }

    server.Rendezvous(status);

    if (status != KRequestPending)
        {
        User::WaitForRequest(status);
        server.Kill(KErrNone);
        server.Close();
        return (status.Int());
        }
    else
        {
        server.Resume();
        }

    User::WaitForRequest(status);
    server.Close();

    if (status != KErrNone)
        {
        return (status.Int());
        }

    LOCUTILSDEBUG( "-RContactResolverSession::CreateServerProcess" )
    
    return KErrNone;
    }

TVersion RContactResolverSession::Version() const
    {
    LOCUTILSDEBUG( "RContactResolverSession::Version" )
    
    return TVersion(KLocPrivacyServerMajorVersionNumber,
            KLocPrivacyServerMinorVersionNumber,
            KLocPrivacyServerBuildVersionNumber);
    }

EXPORT_C void RContactResolverSession::Close()
    {
    LOCUTILSDEBUG( "+RContactResolverSession::Close" )
    
    RSessionBase::Close();
    	
    LOCUTILSDEBUG( "-RContactResolverSession::Close" )
    }
