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

#include "wdshutdownobserver.h"
#include "mdscommoninternal.h"

// Print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// ---------------------------------------------------------------------------
// CWDShutdownObserver::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CWDShutdownObserver* CWDShutdownObserver::NewL( MWDShutdownObserver& aObserver/*, const TUid& aKeyCategory */)
    { 
    CWDShutdownObserver* self = new( ELeave )CWDShutdownObserver( aObserver/*, aKeyCategory */);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CWDShutdownObserver::CWDShutdownObserver()
// ---------------------------------------------------------------------------
//
CWDShutdownObserver::CWDShutdownObserver( MWDShutdownObserver& aObserver/*, const TUid& aKeyCategory */)
    : CActive( CActive::EPriorityStandard ), iObserver( aObserver )/*, iKeyCategory( aKeyCategory )*/
    {       
    }

// ---------------------------------------------------------------------------
// CWDShutdownObserver::ConstructL()
// ---------------------------------------------------------------------------
//
void CWDShutdownObserver::ConstructL()
    {
    PRINT(_L("CWDShutdownObserver::ConstructL()"));
    
    // attach to the property    
    User::LeaveIfError( iProperty.Attach(KHarvesterPSShutdown,KShutdown,EOwnerThread) );
    
    CActiveScheduler::Add( this );
    
    // wait for the previously attached property to be updated
    iProperty.Subscribe(iStatus);        
    
    SetActive();
    
    PRINT(_L("CWDShutdownObserver::ConstructL() end"));
    }

// ---------------------------------------------------------------------------
// CWDShutdownObserver::~CWDShutdownObserver()
// ---------------------------------------------------------------------------
//
CWDShutdownObserver::~CWDShutdownObserver()
    {
    PRINT(_L("CWDShutdownObserver::~CWDShutdownObserver()"));
    Cancel();
    iProperty.Close();
    PRINT(_L("CWDShutdownObserver::~CWDShutdownObserver() end"));
    }

// ---------------------------------------------------------------------------
// CWDShutdownObserver::RunL()
// ---------------------------------------------------------------------------
//
void CWDShutdownObserver::RunL()
    {
    
    PRINT(_L("CWDShutdownObserver::RunL()"));
    
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
        iObserver.ShutdownNotification();
        }
    else
        {
        iObserver.RestartNotification();
        }
    }

TInt CWDShutdownObserver::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CWDShutdownObserver::DoCancel()
// ---------------------------------------------------------------------------
//
void CWDShutdownObserver::DoCancel()
    {
    iProperty.Cancel();
    }

// End of file
