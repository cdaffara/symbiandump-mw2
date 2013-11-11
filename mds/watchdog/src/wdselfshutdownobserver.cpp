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
* Description: Shutdown Observer
*
*/
#include <e32base.h>

#include "wdselfshutdownobserver.h"
#include "mdscommoninternal.h"

// Print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// ---------------------------------------------------------------------------
// CWDSelfShutdownObserver::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CWDSelfShutdownObserver* CWDSelfShutdownObserver::NewL( MWDSelfShutdownObserver& aObserver/*, const TUid& aKeyCategory */)
    { 
    CWDSelfShutdownObserver* self = new( ELeave )CWDSelfShutdownObserver( aObserver/*, aKeyCategory */);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CWDSelfShutdownObserver::CWDSelfShutdownObserver()
// ---------------------------------------------------------------------------
//
CWDSelfShutdownObserver::CWDSelfShutdownObserver( MWDSelfShutdownObserver& aObserver/*, const TUid& aKeyCategory */)
    : CActive( CActive::EPriorityStandard ), iObserver( aObserver )/*, iKeyCategory( aKeyCategory )*/
    {       
    }

// ---------------------------------------------------------------------------
// CWDSelfShutdownObserver::ConstructL()
// ---------------------------------------------------------------------------
//
void CWDSelfShutdownObserver::ConstructL()
    {
    PRINT(_L("CWDSelfShutdownObserver::ConstructL()"));
    
    TInt error = RProperty::Define(KWatchdogPSShutdown, KShutdown,RProperty::EInt,KAllowAllPolicy,KPowerMgmtPolicy);

    // attach to the property    
    User::LeaveIfError( iProperty.Attach(KWatchdogPSShutdown,KShutdown,EOwnerThread) );
    
    CActiveScheduler::Add( this );
    
    // wait for the previously attached property to be updated
    iProperty.Subscribe(iStatus);        
    
    SetActive();
    
    PRINT(_L("CWDSelfShutdownObserver::ConstructL() end"));
    }

// ---------------------------------------------------------------------------
// CWDSelfShutdownObserver::~CWDSelfShutdownObserver()
// ---------------------------------------------------------------------------
//
CWDSelfShutdownObserver::~CWDSelfShutdownObserver()
    {
    PRINT(_L("CWDSelfShutdownObserver::~CWDSelfShutdownObserver()"));
    Cancel();
    iProperty.Close();
    PRINT(_L("CWDSelfShutdownObserver::~CWDSelfShutdownObserver() end"));
    }

// ---------------------------------------------------------------------------
// CWDSelfShutdownObserver::RunL()
// ---------------------------------------------------------------------------
//
void CWDSelfShutdownObserver::RunL()
    {
    
    PRINT(_L("CWDSelfShutdownObserver::RunL()"));
    
    // resubscribe before processing new value to prevent missing updates
    iProperty.Subscribe(iStatus);
    SetActive();
    
    // retrieve the Uid of the package going to be updated
    TInt value = 0;
    const TInt err = iProperty.Get(value);
    User::LeaveIfError(err);

    // observer callback
    if (value)
        {
        iObserver.SelfShutdownNotification();
        }
    
    }

TInt CWDSelfShutdownObserver::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CWDSelfShutdownObserver::DoCancel()
// ---------------------------------------------------------------------------
//
void CWDSelfShutdownObserver::DoCancel()
    {
    iProperty.Cancel();
    }

// End of file
