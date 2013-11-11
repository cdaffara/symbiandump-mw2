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
* Description:  Implementation of the BufTypeSupEventAO class.
*
*/


#include "BufTypeSupEventAO.h"

using namespace multimedia;


// ---------------------------------------------------------------------------
// Contructor
// ---------------------------------------------------------------------------
CBufTypeSupEventAO::CBufTypeSupEventAO(MBufferingTypesSupportedObserver& aObserver )
: CActive(CActive::EPriorityStandard),
iObserver(aObserver),
iError(KErrNone)
    {
    CActiveScheduler::Add(this);
    }


// ---------------------------------------------------------------------------
// Second Phase Constructor
// ---------------------------------------------------------------------------
void CBufTypeSupEventAO::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// NewL method
// ---------------------------------------------------------------------------
CBufTypeSupEventAO* CBufTypeSupEventAO::NewL( MBufferingTypesSupportedObserver& aObserver )
    {
    CBufTypeSupEventAO* self = new (ELeave)CBufTypeSupEventAO( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
CBufTypeSupEventAO::~CBufTypeSupEventAO()
    {
    Cancel();
    }

// ---------------------------------------------------------------------------
// SetActive the Object if not already Active
// ---------------------------------------------------------------------------
void CBufTypeSupEventAO::SetActive()
    {
    if (!IsActive())
        {
        CActive::SetActive();
        }
    }

// ---------------------------------------------------------------------------
// Error Code Stored upon Request Completion
// ---------------------------------------------------------------------------
TInt CBufTypeSupEventAO::Error()
    {
    return iError;
    }

// ---------------------------------------------------------------------------
// RunL called when Request Completed
// ---------------------------------------------------------------------------
void CBufTypeSupEventAO::RunL()
    {
    // Save the error code
    iError = iStatus.Int();
    // Signal the observer that this request is serviced
    iObserver.BufferingTypesSupportedChanged();
    }

// ---------------------------------------------------------------------------
// Cancelling the Active Object
// ---------------------------------------------------------------------------
void CBufTypeSupEventAO::DoCancel()
    {
    // No impl
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status,KErrCancel);
    }

// ---------------------------------------------------------------------------
// Error Occured in the RunL
// ---------------------------------------------------------------------------
TInt CBufTypeSupEventAO::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }

// End of File
