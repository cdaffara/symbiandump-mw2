/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Class to handle system backup events.
*
*
*/


// INCLUDE FILES
#include <connect/sbdefs.h>
#include "EPos_CPosLmBackupListener.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmBackupListener::CPosLmBackupListener
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmBackupListener::CPosLmBackupListener()
    : CActive(EPriorityLow)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmBackupListener::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmBackupListener::ConstructL(MPosLmBackupObserver* aObserver)
    {
    TUid categoryUid;
    categoryUid.iUid = KUidSystemCategoryValue;
    User::LeaveIfError(iProperty.Attach(categoryUid,
                                        conn::KUidBackupRestoreKey));
    CActiveScheduler::Add(this);

    // initial subscription and process current property value
    RunL();

    // Set this value last in the method, so the code in RunL
    // doesn't call the callback method while in construction.
    iObserver = aObserver;
    }

// Two-phased constructor
CPosLmBackupListener* CPosLmBackupListener::NewL(
                MPosLmBackupObserver* aObserver
                )
    {
    CPosLmBackupListener* self =
        new (ELeave) CPosLmBackupListener;
    CleanupStack::PushL(self);
    self->ConstructL(aObserver);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmBackupListener::~CPosLmBackupListener()
    {
    iObserver = NULL;
    Cancel();
    iProperty.Close();
    }

// -----------------------------------------------------------------------------
// CPosLmBackupListener::IsBackupRunning
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPosLmBackupListener::IsBackupRunning()
    {
    TBool backupIsRunning = EFalse;
    TInt value = 0;
    TInt err = iProperty.Get(value);

    if (err == KErrNone)
        {
        // Use mask to get rid of unwanted info
        TInt partType = conn::KBURPartTypeMask & value;

        // The values EBURUnset or EBURNormal mean
        // that no backup/restore is ongoing.
        if (partType == conn::EBURUnset || partType == conn::EBURNormal)
            {
            backupIsRunning = EFalse;
            }
        else
            {
            backupIsRunning = ETrue;
            }
        }
    else if (err == KErrNotFound)
        {
        // If property is not found, assume that no backup is in progress
        backupIsRunning = EFalse;
        }
    else
        {
        // Some other error occured. In this case,
        // assume that backup is running.
        backupIsRunning = ETrue;
        }

    return backupIsRunning;
    }

// -----------------------------------------------------------------------------
// CPosLmBackupListener::RunL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmBackupListener::RunL()
    {
    // resubscribe before processing new value to prevent missing updates.
    iProperty.Subscribe(iStatus);
    SetActive();

    // Only send event if backup/restore is started.
    // (Don't send when finished.)
    if (IsBackupRunning() && iObserver)
        {
        iObserver->HandleBackupEvent();
        }
    }

// -----------------------------------------------------------------------------
// CPosLmBackupListener::DoCancel
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmBackupListener::DoCancel()
    {
    iProperty.Cancel();
    }

// End of File
