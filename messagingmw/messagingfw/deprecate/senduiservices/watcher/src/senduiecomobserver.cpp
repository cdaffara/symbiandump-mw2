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
* Description:   Listens Ecom events. Requests service update, 
*                - if SendUI Ecom service count changes
*                - has changed since previous boot.
*
*/




#include <e32base.h>
#include <e32property.h>
#include <ecom.h>

#include "senduiecomobserver.h"
#include "SendUiPrivateCRKeys.h"
#include "senduiproperty.h"
#include "senduiwatcher.h"
#include "centralrepository.h"
#include <SendingServiceInterfaceUids.hrh>

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CSendUiEcomObserver* CSendUiEcomObserver::NewL()
    {
    CSendUiEcomObserver* self = new ( ELeave ) CSendUiEcomObserver();
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CSendUiEcomObserver::~CSendUiEcomObserver()
    {
    Cancel();
    }
// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CSendUiEcomObserver::ConstructL() 
    {
    CActiveScheduler::Add( this );
    RunL(); 
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSendUiEcomObserver::CSendUiEcomObserver()
    : CActive( CActive::EPriorityStandard ), iEcomSession( REComSession::OpenL())
    {
    }

// ---------------------------------------------------------------------------
// From class CActive
// ---------------------------------------------------------------------------
//
void CSendUiEcomObserver::DoCancel()
    {
    iEcomSession.CancelNotifyOnChange( iStatus );
    }

// ---------------------------------------------------------------------------
// From class CActive
// ---------------------------------------------------------------------------
//
void CSendUiEcomObserver::RunL()
    {
    iEcomSession.NotifyOnChange( iStatus);
    CheckForUpdateL();
    SetActive();
    }

// ---------------------------------------------------------------------------
// CheckForUpdateL
// Requests service list update if SendUI Ecom service count has changed.
// ---------------------------------------------------------------------------
//
void CSendUiEcomObserver::CheckForUpdateL()
    {
    RImplInfoPtrArray implInfoArray ;
    REComSession::ListImplementationsL(
    TUid::Uid( KSendUiSendingServiceInterface ),implInfoArray );
    TInt newEcomCount = implInfoArray.Count();
    implInfoArray.ResetAndDestroy();
    
    CRepository* repository = CRepository::NewLC( KCRUidSendUi );
    TInt oldEcomCount = -1;
    // request service list update if sendui ecom service count has change.
    if ( repository->Get( KKeySendUiServiceEcomCount, oldEcomCount ) != KErrNone 
        || oldEcomCount != newEcomCount )
        {
        repository->Set( KKeySendUiServiceEcomCount, newEcomCount );
        User::LeaveIfError( 
            RProperty::Set(KPSUidSendUiProperty, KSendUiUpdateRequest, KSendUiUpdateRequested ) );
        }
    CleanupStack::PopAndDestroy( repository );
    }

// end of file
