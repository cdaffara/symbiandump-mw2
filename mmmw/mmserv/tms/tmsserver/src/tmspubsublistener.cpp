/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Implements the class TMSPubSubListener
 *
 */

#include "tmspubsublistener.h"
#include "tmspubsubobserver.h"

using namespace TMS;

// -----------------------------------------------------------------------------
// TMSPubSubListener::TMSPubSubListener
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TMSPubSubListener::TMSPubSubListener(const TUid aUid, const TInt aKey,
        TMSPubSubObserver* aObserver) :
    CActive(CActive::EPriorityStandard),
    iUid(aUid),
    iId(aKey),
    iObserver(aObserver)
    {
    }

// -----------------------------------------------------------------------------
// TMSPubSubListener::~TMSPubSubListener
// Destructor.
// -----------------------------------------------------------------------------
//
TMSPubSubListener::~TMSPubSubListener()
    {
    Cancel();
    iProperty.Close();
    }

// -----------------------------------------------------------------------------
// TMSPubSubListener::RunL
// From CActive.
// -----------------------------------------------------------------------------
//
void TMSPubSubListener::RunL()
    {
    const TRequestStatus status(iStatus);
    StartListening();
    iObserver->HandleNotifyPSL(iUid, iId, status);
    }

// -----------------------------------------------------------------------------
// TMSPubSubListener::DoCancel
// From CActive.
// -----------------------------------------------------------------------------
//
void TMSPubSubListener::DoCancel()
    {

    iProperty.Cancel();
    }

// -----------------------------------------------------------------------------
// TMSPubSubListener::RunError
// From CActive.
// -----------------------------------------------------------------------------
//
TInt TMSPubSubListener::RunError(TInt /*aError*/)
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// TMSPubSubListener::NewL
// -----------------------------------------------------------------------------
//
TMSPubSubListener* TMSPubSubListener::NewL(const TUid aUid, const TInt aKey,
        TMSPubSubObserver* aObserver)
    {

    TMSPubSubListener* self = new (ELeave) TMSPubSubListener(aUid, aKey,
            aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// TMSPubSubListener::StartListening
// -----------------------------------------------------------------------------
//
void TMSPubSubListener::StartListening()
    {
    iProperty.Subscribe(iStatus);
    SetActive();
    }

// -----------------------------------------------------------------------------
// TMSPubSubListener::ConstructL
// -----------------------------------------------------------------------------
//
void TMSPubSubListener::ConstructL()
    {
    if (iObserver)
        {
        CActiveScheduler::Add(this);
        }

    User::LeaveIfError(iProperty.Attach(iUid, iId, EOwnerThread));

    if (iObserver)
        {
        StartListening();
        }
    }

// -----------------------------------------------------------------------------
// TMSPubSubListener::Get
// Read integer value.
// -----------------------------------------------------------------------------
//
TInt TMSPubSubListener::Get(TInt& aVal)
    {
    return iProperty.Get(iUid, iId, aVal);
    }

// -----------------------------------------------------------------------------
// TMSPubSubListener::Get
// Read binary value.
// -----------------------------------------------------------------------------
//
TInt TMSPubSubListener::Get(TDes8& aVal)
    {
    return iProperty.Get(iUid, iId, aVal);
    }

// -----------------------------------------------------------------------------
// TMSPubSubListener::Get
// Read string value.
// -----------------------------------------------------------------------------
//
TInt TMSPubSubListener::Get(TDes16& aVal)
    {
    return iProperty.Get(iUid, iId, aVal);
    }

// -----------------------------------------------------------------------------
// TMSPubSubListener::Set
// Sets integer value.
// -----------------------------------------------------------------------------
//
TInt TMSPubSubListener::Set(TInt aVal)
    {
    return iProperty.Set(iUid, iId, aVal);
    }

// End of File
