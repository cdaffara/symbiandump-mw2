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
#include <lbttriggeringsystemsettingschangeeventnotifier.h>
#include "lbterrors.h"




// ================= LOCAL FUNCTIONS ========================
 
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CLbtTriggeringSystemSettingsChangeEventNotifier::CLbtTriggeringSystemSettingsChangeEventNotifier
//
// Default constructor
// ---------------------------------------------------------
//
CLbtTriggeringSystemSettingsChangeEventNotifier::CLbtTriggeringSystemSettingsChangeEventNotifier(RLbt& aLbt,
                                                 MLbtTriggeringSystemSettingsChangeEventObserver& aObserver,
                                                 TInt aPriority ):CActive(aPriority),
                                                 iLbt(aLbt),iObserver(aObserver)
    {
    CActiveScheduler::Add(this);
    }


// ---------------------------------------------------------------------------
// CLbtTriggeringSystemSettingsChangeEventNotifier::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLbtTriggeringSystemSettingsChangeEventNotifier::ConstructL()
    {
    
    }

// ---------------------------------------------------------------------------
// CLbtTriggeringSystemSettingsChangeEventNotifier::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtTriggeringSystemSettingsChangeEventNotifier* CLbtTriggeringSystemSettingsChangeEventNotifier::NewL(RLbt& aLbt,MLbtTriggeringSystemSettingsChangeEventObserver& aObserver,
            TInt aPriority )
    {
    __ASSERT_ALWAYS(aLbt.SubSessionHandle(), User::Panic(KLbtClientPanicCategory, ELbtServerBadHandle));
    CLbtTriggeringSystemSettingsChangeEventNotifier* self = new (ELeave) CLbtTriggeringSystemSettingsChangeEventNotifier(aLbt,aObserver,aPriority);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtTriggeringSystemSettingsChangeEventNotifier::~CLbtTriggeringSystemSettingsChangeEventNotifier()
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtTriggeringSystemSettingsChangeEventNotifier::~CLbtTriggeringSystemSettingsChangeEventNotifier()
    {
    if (IsActive())      
      	Cancel();
    }


// ---------------------------------------------------------
// CLbtTriggeringSystemSettingsChangeEventNotifier::Start
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CLbtTriggeringSystemSettingsChangeEventNotifier::Start()
    {
    iStatus = KRequestPending;
	iLbt.NotifyTriggeringSystemSettingChange(iSettings,iStatus);
	SetActive();
    }
    

// ---------------------------------------------------------
// CLbtTriggeringSystemSettingsChangeEventNotifier::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtTriggeringSystemSettingsChangeEventNotifier::RunL()
    {
    TInt error = iStatus.Int();
    if( error != KErrCancel )
        {
        Start();
        }
    if( error == KErrNone)
        {
        iObserver.TriggeringSystemSettingsChangedL(iSettings);
        }    
    }

// ---------------------------------------------------------
// CLbtTriggeringSystemSettingsChangeEventNotifier::RunError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CLbtTriggeringSystemSettingsChangeEventNotifier::RunError(TInt /*aError*/)
    {
    return KErrNone;
    }

// ---------------------------------------------------------
// CLbtTriggeringSystemSettingsChangeEventNotifier::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtTriggeringSystemSettingsChangeEventNotifier::DoCancel()
    {
    iLbt.CancelNotifyTriggeringSystemSettingChange();
    }
    
//  End of File

