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
* Description:  Inline implementation of message monitor
*
*/

#include <mpxlog.h>
#include "mpxsession.h"
#include "mpxmessagemonitor.h"

const TInt KMPXGetNextMessage=0;
const TInt KMPXCancelGetNextMessage=1;

// ============================== MEMBER FUNCTIONS ============================
// ----------------------------------------------------------------------------
// Two phases constructor
// ----------------------------------------------------------------------------
// 
EXPORT_C CMPXMessageMonitor* CMPXMessageMonitor::NewL(
    const RMPXSession& aSession,
    MMPXMessageObserver& aObserver)
    {
    CMPXMessageMonitor* self = new(ELeave)CMPXMessageMonitor(aSession, aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//    
EXPORT_C CMPXMessageMonitor::~CMPXMessageMonitor() 
    {
    MPX_FUNC_EX("CMPXMessageMonitor::~CMPXMessageMonitor");
    Cancel();
    }

// ----------------------------------------------------------------------------
// C++ constructor
// ----------------------------------------------------------------------------
//    
CMPXMessageMonitor::CMPXMessageMonitor(
    const RMPXSession& aSession,
    MMPXMessageObserver& aObserver)
:   CActive(EPriorityHigh),
    iSession(aSession),
    iObserver(aObserver)
    {
    CActiveScheduler::Add(this);
    }


// ----------------------------------------------------------------------------
// 2nd phase construction: 
// ----------------------------------------------------------------------------
//    
void CMPXMessageMonitor::ConstructL()
    {
    MPX_FUNC_EX("CMPXMessageMonitor::ConstructL");
    GetNextMessage();
    }

// ----------------------------------------------------------------------------
// Start to monitor messages
//
// ----------------------------------------------------------------------------
//    
EXPORT_C void CMPXMessageMonitor::GetNextMessage()
    {
    MPX_FUNC_EX("CMPXMessageMonitor::GetNextMessage");
    if (!IsActive())
        {
        MPX_DEBUG1("CMPXMessageMonitor::GetNextMessage send request");
        iSession.SendReceive(KMPXGetNextMessage, 
                             TIpcArgs(&iMsgDataPkg, &iErrorPkg),
                             iStatus);
        SetActive();
        }
    }
 
// ----------------------------------------------------------------------------
// From CActive: a new message must have arrived so retrieve it and pass back
// to observer, then wait for next message
// ----------------------------------------------------------------------------
//    
EXPORT_C void CMPXMessageMonitor::RunL()
    {
    MPX_FUNC_EX("CMPXMessageMonitor::RunL");
    
    // Try again in case server is down for IAD
    if (iStatus == KErrDied || iStatus == KErrServerTerminated)
        {
        MPX_DEBUG1("-->CMPXMessageMonitor::RunL Reconnecting session for IAD");
        
        // attempt to bring the server back up
        if (iSession.Reconnect() != KErrNone)
            {            
            MPX_DEBUG1("-->CMPXMessageMonitor::RunL Reconnect Failed");
            
            User::LeaveIfError(iStatus.Int());
            }
        }
    else
        {
        User::LeaveIfError(iStatus.Int());
        iObserver.MessageReceived(iMsgDataPkg(), iErrorPkg());
        }
    }

// ----------------------------------------------------------------------------
// From CActive: no more messages are required 
// ----------------------------------------------------------------------------
//     
EXPORT_C void CMPXMessageMonitor::DoCancel()
    {
    MPX_FUNC_EX("CMPXMessageMonitor::DoCancel");
    if (IsActive())
        {
        TRAP_IGNORE(iSession.SendReceiveL(KMPXCancelGetNextMessage));
        }
    }

// ----------------------------------------------------------------------------
// Handle leave in the RunL() 
// ----------------------------------------------------------------------------
//     
EXPORT_C TInt CMPXMessageMonitor::RunError(TInt aError)
    {
    MPX_DEBUG3("-->CMPXMessageMonitor::RunError 0x%08x, error %d", this, aError);
    if (KErrCancel!=aError && KErrServerTerminated!=aError)
        {
        GetNextMessage();
        }
    MPX_DEBUG3("<--CMPXMessageMonitor::RunError 0x%08x, error %d", this, aError);
    return KErrNone;
    }

// End of file

