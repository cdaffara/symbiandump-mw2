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
* Description:  Notifier class implementation for SuplSettings
*
*/



// INCLUDE FILES
#include    <centralrepository.h>

#include "epos_csuplsettings.h"
#include "epos_suplsessionnotifier.h"
#include "epos_msuplsessionobserver.h"
#include "epos_csuplsettingsinternalcrkeys.h"


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplSessionNotifier::CSuplSessionNotifier( MSuplSessionObserver& aObserver )
    :CActive(CActive::EPriorityStandard),
    iObserver(aObserver)
    {
    
    }
    

// EPOC default constructor can leave.
void CSuplSessionNotifier::ConstructL()
    {
    CActiveScheduler::Add(this);
    iRepository = CRepository::NewL(KCRUidSuplSettings);
    
    // Start listening
    StartListening();
    }

// Two-phased constructor.
CSuplSessionNotifier* CSuplSessionNotifier::NewL(
    MSuplSessionObserver& aObserver)
    {
    CSuplSessionNotifier* self = 
        new (ELeave) CSuplSessionNotifier(aObserver);

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }
    

// Destructor
CSuplSessionNotifier::~CSuplSessionNotifier()
    {
    Cancel();
    
    delete iRepository;
    iRepository = NULL;
    
    }

// ---------------------------------------------------------
// CSuplSessionNotifier::RunL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionNotifier::RunL()
    {
     TInt event;
     iRepository->Get(KSuplSessionDBEvent, event);
     TReal sessionID;
     iRepository->Get(KSuplSessionDBChangedSessionId, sessionID);
     MSuplSessionObserver::TSuplSessionEventType eventType = (MSuplSessionObserver::TSuplSessionEventType)event;
     iObserver.HandleSuplSessionChangeL(eventType,sessionID);
     
     StartListening();
    }

// ---------------------------------------------------------
// CSuplSessionNotifier::DoCancel
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionNotifier::DoCancel()
    {
    iRepository->NotifyCancel(KSuplSessionDBEvent);
    }

// ---------------------------------------------------------
// CSuplSessionNotifier::RunError
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSuplSessionNotifier::RunError(TInt /*aError*/)
    {
    return KErrNone;
    }

// ---------------------------------------------------------
// CSuplSessionNotifier::StartListening
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionNotifier::StartListening()
    {
    iStatus = KRequestPending;

     // Request for notification for DB field change
    iRepository->NotifyRequest(KSuplSessionDBEvent, iStatus);
    SetActive();
    }
//  End of File
