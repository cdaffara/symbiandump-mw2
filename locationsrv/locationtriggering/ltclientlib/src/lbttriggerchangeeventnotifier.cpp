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
#include <lbttriggerchangeeventnotifier.h>
#include "lbterrors.h"




// ================= LOCAL FUNCTIONS ========================
 
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CLbtTriggerChangeEventNotifier::CLbtTriggerChangeEventNotifier
//
// Default constructor
// ---------------------------------------------------------
//
CLbtTriggerChangeEventNotifier::CLbtTriggerChangeEventNotifier(RLbt& aLbt,
                                MLbtTriggerChangeEventObserver& aObserver,
                                TInt aPriority ):CActive(aPriority),
                                iLbt(aLbt),iObserver(aObserver)
    {
    CActiveScheduler::Add(this);
    }


// ---------------------------------------------------------------------------
// CLbtTriggerChangeEventNotifier::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLbtTriggerChangeEventNotifier::ConstructL()
    {
    
    }

// ---------------------------------------------------------------------------
// CLbtTriggerChangeEventNotifier::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtTriggerChangeEventNotifier* CLbtTriggerChangeEventNotifier::NewL(RLbt& aLbt,
                                MLbtTriggerChangeEventObserver& aObserver,
                                TInt aPriority)
    {
    __ASSERT_ALWAYS(aLbt.SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
    CLbtTriggerChangeEventNotifier* self = new (ELeave) CLbtTriggerChangeEventNotifier(aLbt,aObserver,aPriority);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// ---------------------------------------------------------------------------
// CLbtTriggerChangeEventNotifier::~CLbtTriggerChangeEventNotifier()
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtTriggerChangeEventNotifier::~CLbtTriggerChangeEventNotifier()
    {
    if (IsActive())      
      	Cancel();
    }


// ---------------------------------------------------------
// CLbtTriggerChangeEventNotifier::Start
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CLbtTriggerChangeEventNotifier::Start()
    {
    iStatus = KRequestPending;
	iLbt.NotifyTriggerChangeEvent(iChangeEvent,iStatus);
	SetActive();
    }
    

// ---------------------------------------------------------
// CLbtTriggerChangeEventNotifier::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtTriggerChangeEventNotifier::RunL()
    {
    TInt error = iStatus.Int();
    if( error != KErrCancel )
        {
        Start();
        }
    if( error == KErrNone )
        {
        iObserver.TriggerChangedL(iChangeEvent);
        }    
    }

// ---------------------------------------------------------
// CLbtTriggerChangeEventNotifier::RunError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CLbtTriggerChangeEventNotifier::RunError(TInt /*aError*/)
    {
    return KErrNone;
    }

// ---------------------------------------------------------
// CLbtTriggerChangeEventNotifier::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtTriggerChangeEventNotifier::DoCancel()
    {
    iLbt.CancelNotifyTriggerChangeEvent();
    }

//  End of File

