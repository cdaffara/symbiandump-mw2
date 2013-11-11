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
* Description:   Notifies SendUI to refresh its local sending service list
*                if list in Central Repository is updated.
*
*/



#include <e32base.h>
#include <e32property.h>

#include "propertyobserver.h"
#include "senduiproperty.h"
#include "senduiimpl.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CPropertyObserver* CPropertyObserver::NewL( CSendUiImpl* aParent)
    {
    CPropertyObserver* self = new ( ELeave ) CPropertyObserver( aParent );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPropertyObserver::~CPropertyObserver()
    {
    iPropertyUpdateOperation.Close();
    Cancel();
    }
// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CPropertyObserver::ConstructL()
    {
   
    User::LeaveIfError( iPropertyUpdateOperation.Attach( KPSUidSendUiProperty, KSendUiUpdateOperation  ));
    iPropertyUpdateOperation.Subscribe( iStatus );
    SetActive();

    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPropertyObserver::CPropertyObserver( CSendUiImpl* aParent )
    : CActive( CActive::EPriorityStandard ),
    iParent( aParent)
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// From class CActive
// ---------------------------------------------------------------------------
//
void CPropertyObserver::DoCancel()
    {
    iPropertyUpdateOperation.Close();
    }

// ---------------------------------------------------------------------------
// From class CActive
// ---------------------------------------------------------------------------
//
void CPropertyObserver::RunL()
    {
    TInt value;
    if ( iPropertyUpdateOperation.Get( value ) != KErrNotFound 
         && value == KSendUiUpdateOperationDone )
        {
        iParent->GetServicesL();
        }
    iPropertyUpdateOperation.Subscribe( iStatus );
    SetActive();
    }

