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
* Description: Implementation of RPosLmDbManClientSession
*
*
*/


// INCLUDE FILES
#include    "EPos_RPosLmDbManClientSession.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RPosLmDbManClientSession::NotifyDatabaseEvent
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RPosLmDbManClientSession::NotifyDatabaseEvent(
    CPosLmDatabaseManager::TDbEvent& aEvent,
    TRequestStatus& aStatus)
    {
    iEventPtr.Set(reinterpret_cast<TUint8*>(&aEvent),
        sizeof(CPosLmDatabaseManager::TDbEvent),
        sizeof(CPosLmDatabaseManager::TDbEvent));

    SendReceive(EPosLmServerListenForEvent, TIpcArgs(&iEventPtr), aStatus);
    }

// -----------------------------------------------------------------------------
// RPosLmDbManClientSession::CancelNotifyDatabaseEvent
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RPosLmDbManClientSession::CancelNotifyDatabaseEvent()
    {
    return SendReceive(EPosLmServerCancelListenForEvent,
                        TIpcArgs(TIpcArgs::ENothing));
    }

//  End of File
