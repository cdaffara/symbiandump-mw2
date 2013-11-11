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
* Description:  implementation of playback session
*
*/

#include <mpxsession.h>
#include "mpxsessionretry.h"


// ============================== CONSTRUCTOR =================================

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
//
EXPORT_C RMPXSession::RMPXSession()
    {
    iSessionRetry = new RMPXSessionRetry();
    }

EXPORT_C RMPXSession::~RMPXSession()
    {
    delete iSessionRetry;
    }

// ============================== MEMBER FUNCTIONS ============================

// ----------------------------------------------------------------------------
// Return the version info
// ----------------------------------------------------------------------------
//
EXPORT_C TVersion RMPXSession::Version() const
    { 
    return iSessionRetry->Version();
    }

// ----------------------------------------------------------------------------
// Connect to server
// ----------------------------------------------------------------------------
//
EXPORT_C TInt RMPXSession::Connect(const TDesC& aServer, 
                                   const TDesC& aImage, 
                                   const TVersion& aVersion)
    {
    return iSessionRetry->Connect(aServer, aImage, aVersion); 
    }

// ----------------------------------------------------------------------------
// Reconnect to server
// ----------------------------------------------------------------------------
//
EXPORT_C TInt RMPXSession::Reconnect() const
    {
    return iSessionRetry->Reconnect();
    }

// -----------------------------------------------------------------------------
// Send a message
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RMPXSession::SendReceiveL(TInt aFunction) const
    {
    return iSessionRetry->SendReceiveL(aFunction);
    }

// -----------------------------------------------------------------------------
// Send a message
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RMPXSession::SendReceiveL(TInt aFunction,
                                        const TIpcArgs& aArgs) const
    {
    return iSessionRetry->SendReceiveL(aFunction, aArgs);
    }

// -----------------------------------------------------------------------------
// Send a message
// -----------------------------------------------------------------------------
//
EXPORT_C void RMPXSession::SendReceive(TInt aFunction,
                                       TRequestStatus& aStatus) const
    {
    iSessionRetry->SendReceive(aFunction, aStatus);        
    }

// -----------------------------------------------------------------------------
// Send a message
// -----------------------------------------------------------------------------
//
EXPORT_C void RMPXSession::SendReceive(TInt aFunction,
                                       const TIpcArgs& aArgs,
                                       TRequestStatus& aStatus) const
    {
    iSessionRetry->SendReceive(aFunction, aArgs, aStatus);    
    }

// -----------------------------------------------------------------------------
// Close session.
// -----------------------------------------------------------------------------
//
EXPORT_C void RMPXSession::Close()
    {
    iSessionRetry->Close();
    }
