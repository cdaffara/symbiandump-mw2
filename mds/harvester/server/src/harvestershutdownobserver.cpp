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
* Description: 
*
*/

#include <e32base.h>

#include "harvestershutdownobserver.h"
#include "mdscommoninternal.h"
#include "harvesterlog.h"

// ---------------------------------------------------------------------------
// CHarvesterShutdownObserver::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CHarvesterShutdownObserver* CHarvesterShutdownObserver::NewL( MHarvesterShutdownObserver& aObserver/*, const TUid& aKeyCategory */)
    { 
    CHarvesterShutdownObserver* self = new( ELeave )CHarvesterShutdownObserver( aObserver/*, aKeyCategory */);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CHarvesterShutdownObserver::CHarvesterShutdownObserver()
// ---------------------------------------------------------------------------
//
CHarvesterShutdownObserver::CHarvesterShutdownObserver( MHarvesterShutdownObserver& aObserver/*, const TUid& aKeyCategory */)
    : CActive( CActive::EPriorityUserInput ), iObserver( aObserver ), iValue( KErrNone )/*, iKeyCategory( aKeyCategory )*/
    {       
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// CHarvesterShutdownObserver::ConstructL()
// ---------------------------------------------------------------------------
//
void CHarvesterShutdownObserver::ConstructL()
    {
    WRITELOG( "CHarvesterShutdownObserver::ConstructL()" );
    
    const TInt error = RProperty::Define(KHarvesterPSShutdown, KShutdown,RProperty::EInt,KAllowAllPolicy,KPowerMgmtPolicy);
    
    WRITELOG1( "CHarvesterShutdownObserver::ConstructL, defineError = %d", error );
    
    // attach to the property    
    User::LeaveIfError( iProperty.Attach(KHarvesterPSShutdown,KShutdown,EOwnerThread) );
    
    // wait for the previously attached property to be updated
    iProperty.Subscribe(iStatus);       
    
    SetActive();
    
    WRITELOG("CHarvesterShutdownObserver::ConstructL() end");
    }

// ---------------------------------------------------------------------------
// CHarvesterShutdownObserver::~CHarvesterShutdownObserver()
// ---------------------------------------------------------------------------
//
CHarvesterShutdownObserver::~CHarvesterShutdownObserver()
    {
    WRITELOG( "CHarvesterShutdownObserver::~CHarvesterShutdownObserver()" );
    Cancel();
    iProperty.Close();
    WRITELOG( "CHarvesterShutdownObserver::~CHarvesterShutdownObserver() end" );
    }

// ---------------------------------------------------------------------------
// CHarvesterShutdownObserver::RunL()
// ---------------------------------------------------------------------------
//
void CHarvesterShutdownObserver::RunL()
    {
    // resubscribe before processing new value to prevent missing updates
    iProperty.Subscribe( iStatus );
    SetActive();
    
    // retrieve the Uid of the package going to be updated
    TInt value = 0;
    const TInt err = iProperty.Get(value);
    User::LeaveIfError(err);

	iValue = value;

    // observer callback
    if (value)
        {
        iObserver.ShutdownNotification();
        }
    else
        {
        iObserver.RestartNotification();
        }
    }

TInt CHarvesterShutdownObserver::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CHarvesterShutdownObserver::DoCancel()
// ---------------------------------------------------------------------------
//
void CHarvesterShutdownObserver::DoCancel()
    {
    iProperty.Cancel();
    }

// ---------------------------------------------------------------------------
// CHarvesterShutdownObserver::UpdateInProgress()
// ---------------------------------------------------------------------------
//
TBool CHarvesterShutdownObserver::UpdateInProgress()
    {
    return iValue > 0 ? ETrue : EFalse;
    }

// End of file
