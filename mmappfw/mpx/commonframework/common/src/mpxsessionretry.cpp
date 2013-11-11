/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Client session retry implementation
*
*/

#include "mpxsessionretry.h"


enum { EFunctionType_SendReceive1, EFunctionType_SendReceive2};

// ============================== LOCAL FUNCTIONS =============================

// ----------------------------------------------------------------------------
// Start server process
// ----------------------------------------------------------------------------
//
LOCAL_C TInt StartServer(const TDesC& aImage)
    {
    RProcess server;
    
    TInt r = server.Create(aImage,KNullDesC);
    if (r != KErrNone)
        {
        return r;
        }
    
    TRequestStatus s;
    server.Rendezvous(s);
    if (s != KRequestPending)
        {
        server.Kill(0);
        }
    else
        {
        server.Resume();
        }
    
    User::WaitForRequest(s);    
    
    r = (server.ExitType()==EExitPanic) ? KErrGeneral : s.Int();
    server.Close();
    
    return r;
    }

// ============================== MEMBER FUNCTIONS ============================

// ----------------------------------------------------------------------------
// Return the version info
// ----------------------------------------------------------------------------
//
TVersion RMPXSessionRetry::Version() const
    { 
    return iVersion;
    }

// ----------------------------------------------------------------------------
// Connect to server
// ----------------------------------------------------------------------------
//
TInt RMPXSessionRetry::Connect(const TDesC& aServer, 
                               const TDesC& aImage, 
                               const TVersion& aVersion)
    {
    iVersion = aVersion;
    iServer = aServer;
    iImage = aImage;
        
    TInt r = RSessionBase::CreateSession(iServer, iVersion, 2);
    
    if (r == KErrNotFound || r == KErrServerTerminated)
        {
        r = Reconnect();
        }

    return r;
    }

// ----------------------------------------------------------------------------
// Reconnect to server
// ----------------------------------------------------------------------------
//
TInt RMPXSessionRetry::Reconnect()
    {
    // make sure the current session is closed; it does no harm to close
    // a session that is already closed
    Close();
    
    TInt r = StartServer(iImage);
        
    if (r == KErrNone || r == KErrAlreadyExists)
        {
        r = RSessionBase::CreateSession(iServer, iVersion, 2);
        }
    
    return r;
    }

// -----------------------------------------------------------------------------
// Send a message
// -----------------------------------------------------------------------------
//
TInt RMPXSessionRetry::SendReceiveL(TInt aFunction)
    {
    TInt retVal = RSessionBase::SendReceive(aFunction);
    
    if (KErrServerTerminated == retVal) 
        {        
        Reconnect(); // try again in case IAD took down the server to replace it
        retVal = RSessionBase::SendReceive(aFunction);
        }
    
    return User::LeaveIfError(retVal);
    }

// -----------------------------------------------------------------------------
// Send a message
// -----------------------------------------------------------------------------
//
TInt RMPXSessionRetry::SendReceiveL(TInt aFunction, const TIpcArgs& aArgs)
    {
    TInt retVal = RSessionBase::SendReceive(aFunction,aArgs);
    
    if (KErrServerTerminated == retVal) 
        {
        Reconnect(); // try again in case IAD took down the server to replace it
        retVal = RSessionBase::SendReceive(aFunction,aArgs);
        }    
    
    return User::LeaveIfError(retVal);
    }

// -----------------------------------------------------------------------------
// Send a message
// -----------------------------------------------------------------------------
//
void RMPXSessionRetry::SendReceive(TInt aFunction, 
                                   TRequestStatus& aStatus)
    {
    RSessionBase::SendReceive(aFunction, aStatus);
    }

// -----------------------------------------------------------------------------
// Send a message
// -----------------------------------------------------------------------------
//
void RMPXSessionRetry::SendReceive(TInt aFunction, 
                                   const TIpcArgs& aArgs,
                                   TRequestStatus& aStatus)
    {
    RSessionBase::SendReceive(aFunction, aArgs, aStatus);
    }
