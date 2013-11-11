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
* Description: This class is a subsession handle with operations related to
*  database management event listening.
*
*
*/



// INCLUDE FILES
#include <e32cmn.h>
#include <e32def.h>
#include <epos_poslmservercommon.h>
#include <epos_rposlandmarkserver.h>
#include "EPos_RPosLmDatabaseEventListener.h"


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// RPosLmDatabaseEventListener::RPosLmDatabaseEventListener
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
RPosLmDatabaseEventListener::RPosLmDatabaseEventListener()
    : iEventPtr(NULL, 0), iUriLength(0), iUriLengthPckg(iUriLength)
    {
    }

// -----------------------------------------------------------------------------
// RPosLmDatabaseEventListener::Open
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
TInt RPosLmDatabaseEventListener::Open(RPosLandmarkServer& aServer)
    {
    return CreateSubSession(aServer,
                            EPosLmServerRegisterEventListenerSubsession);
    }

// -----------------------------------------------------------------------------
// RPosLmDatabaseEventListener::Close
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void RPosLmDatabaseEventListener::Close()
    {
    CloseSubSession(EPosLmServerUnregisterSubsession);
    }

// ---------------------------------------------------------
// RPosLmDatabaseEventListener::NotifyDatabaseEvent
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void RPosLmDatabaseEventListener::NotifyDatabaseEvent(
    TPosLmDatabaseEvent& aEvent,
    TRequestStatus& aStatus)
    {
    iEventPtr.Set(reinterpret_cast<TUint8*>(&aEvent),
        sizeof(TPosLmDatabaseEvent),
        sizeof(TPosLmDatabaseEvent));
    SendReceive(EPosLmServerListenForDbEvent,
                TIpcArgs(&iEventPtr, &iUriLengthPckg), aStatus);
    }

// ---------------------------------------------------------
// RPosLmDatabaseEventListener::CancelNotifyDatabaseEvent
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt RPosLmDatabaseEventListener::CancelNotifyDatabaseEvent()
    {
    return SendReceive(EPosLmServerCancelListenForDbEvent,
                        TIpcArgs(TIpcArgs::ENothing));
    }

// ---------------------------------------------------------
// RPosLmDatabaseEventListener::DatabaseUriFromLastEventLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
HBufC* RPosLmDatabaseEventListener::DatabaseUriFromLastEventLC()
    {
    if (iUriLength == 0)
        {
        User::Leave(KErrNotFound);
        }

    HBufC* buffer = HBufC::NewLC(iUriLength);
    TPtr ptr = buffer->Des();

    iUriLength = 0;

    User::LeaveIfError(SendReceive(EPosLmServerUriFromLastEvent,
                                   TIpcArgs(&ptr)));

    return buffer;
    }



// End of File
