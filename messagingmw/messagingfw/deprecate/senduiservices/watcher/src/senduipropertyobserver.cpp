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
* Description:   Launches SendUi Service Resolver in the event of:
*                - refresh request.
*                - MTM amount has changed since previous boot
*                - Phone language has change since previous boot
*
*/




#include <e32base.h>
#include <e32property.h>

#include "senduipropertyobserver.h"
#include "senduiproperty.h"
#include "senduiwatcher.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CSendUiPropertyObserver* CSendUiPropertyObserver::NewL( CSendUiWatcher* aWatcher )
    {
    CSendUiPropertyObserver* self = new ( ELeave ) CSendUiPropertyObserver( aWatcher );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CSendUiPropertyObserver::~CSendUiPropertyObserver()
    {
    iPropertyUpdateRequest.Close();
    Cancel();
    }
// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CSendUiPropertyObserver::ConstructL()
    {
    CActiveScheduler::Add( this );
    
    User::LeaveIfError( 
        iPropertyUpdateRequest.Attach( KPSUidSendUiProperty, KSendUiUpdateRequest ) );

    iPropertyUpdateRequest.Subscribe( iStatus );

    SetActive();
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSendUiPropertyObserver::CSendUiPropertyObserver( CSendUiWatcher* aParent )
    : CActive( CActive::EPriorityStandard ),
    iParent( aParent )
    {
    }

// ---------------------------------------------------------------------------
// From class CActive
// ---------------------------------------------------------------------------
//
void CSendUiPropertyObserver::DoCancel()
    {
    iPropertyUpdateRequest.Close();
    }

// ---------------------------------------------------------------------------
// From class CActive
// ---------------------------------------------------------------------------
//
void CSendUiPropertyObserver::RunL()
    {
    TInt value;
    if ( ( iPropertyUpdateRequest.Get(value) ) != KErrNotFound 
        && value == KSendUiUpdateRequested )
        {
            iParent->HandleUpdateServicesL( );
        }
    iPropertyUpdateRequest.Subscribe( iStatus );
    SetActive();
    }

// end of file
