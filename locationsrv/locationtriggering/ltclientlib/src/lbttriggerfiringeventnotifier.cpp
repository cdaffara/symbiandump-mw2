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
* Description:  location triggering server client interface
*
*/


// INCLUDE FILES
#include <e32base.h>
#include "lbttriggerfiringeventnotifier.h"
#include "lbterrors.h"





// ================= LOCAL FUNCTIONS ========================
 
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CLbtTriggerFiringEventNotifier::CLbtTriggerFiringEventNotifier
//
// Default constructor
// ---------------------------------------------------------
//
CLbtTriggerFiringEventNotifier::CLbtTriggerFiringEventNotifier(RLbt& aLbt,
                                MLbtTriggerFiringEventObserver& aObserver,
                                TInt aPriority):CActive(aPriority),
                                iLbt(aLbt),iObserver(aObserver)
    {
    CActiveScheduler::Add(this);
    }


// ---------------------------------------------------------------------------
// CLbtTriggerFiringEventNotifier::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLbtTriggerFiringEventNotifier::ConstructL()
    {
    
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFiringEventNotifier::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtTriggerFiringEventNotifier* CLbtTriggerFiringEventNotifier::NewL(RLbt& aLbt,MLbtTriggerFiringEventObserver& aObserver,
            TInt aPriority )
    {
    __ASSERT_ALWAYS(aLbt.SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
    CLbtTriggerFiringEventNotifier* self = new (ELeave) CLbtTriggerFiringEventNotifier(aLbt,aObserver,aPriority);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFiringEventNotifier::~CLbtTriggerFiringEventNotifier()
// Destructor
// ---------------------------------------------------------------------------
// 
EXPORT_C CLbtTriggerFiringEventNotifier::~CLbtTriggerFiringEventNotifier()
    {
    if (IsActive())      
      	Cancel();
    }


// ---------------------------------------------------------
// CLbtTriggerFiringEventNotifier::Start
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CLbtTriggerFiringEventNotifier::Start()
    {
    iStatus = KRequestPending;
	iLbt.NotifyTriggerFired(iFiringEvent,iStatus);
	SetActive();
    }

// ---------------------------------------------------------
// CLbtTriggerFiringEventNotifier::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtTriggerFiringEventNotifier::RunL()
    {
    TInt error = iStatus.Int();
    if( error != KErrCancel )
        {
        Start();
        }
    if( error == KErrNone )
        {
        iObserver.TriggerFiredL(iFiringEvent);
        }
    }

// ---------------------------------------------------------
// CLbtTriggerFiringEventNotifier::RunError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CLbtTriggerFiringEventNotifier::RunError(TInt /*aError*/)
    {
    return KErrNone;
    }

// ---------------------------------------------------------
// CLbtTriggerFiringEventNotifier::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtTriggerFiringEventNotifier::DoCancel()
    {
    iLbt.CancelNotifyTriggerFired();
    }

//  End of File

