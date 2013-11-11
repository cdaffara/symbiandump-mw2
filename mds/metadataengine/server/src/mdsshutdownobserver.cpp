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

#include "mdsshutdownobserver.h"
#include "mdscommoninternal.h"

// Print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// ---------------------------------------------------------------------------
// CMDSShutdownObserver::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CMDSShutdownObserver* CMDSShutdownObserver::NewL( MMDSShutdownObserver& aObserver/*, const TUid& aKeyCategory */)
    { 
    CMDSShutdownObserver* self = new( ELeave )CMDSShutdownObserver( aObserver/*, aKeyCategory */);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CMDSShutdownObserver::CMDSShutdownObserver()
// ---------------------------------------------------------------------------
//
CMDSShutdownObserver::CMDSShutdownObserver( MMDSShutdownObserver& aObserver/*, const TUid& aKeyCategory */)
    : CActive( CActive::EPriorityUserInput ), iObserver( aObserver ), iValue( KErrNone )/*, iKeyCategory( aKeyCategory )*/
    {       
    }

// ---------------------------------------------------------------------------
// CMDSShutdownObserver::ConstructL()
// ---------------------------------------------------------------------------
//
void CMDSShutdownObserver::ConstructL()
    {
#ifdef _DEBUG
    PRINT(_L("CMDSShutdownObserver::ConstructL()"));
#endif
    
    // attach to the property    
    User::LeaveIfError( iProperty.Attach(KMdSPSShutdown,KShutdown,EOwnerThread) );
    
    CActiveScheduler::Add( this );
    
    // wait for the previously attached property to be updated
    iProperty.Subscribe(iStatus);        
    
    SetActive();

#ifdef _DEBUG
    PRINT(_L("CMDSShutdownObserver::ConstructL() end"));
#endif
    }

// ---------------------------------------------------------------------------
// CMDSShutdownObserver::~CMDSShutdownObserver()
// ---------------------------------------------------------------------------
//
CMDSShutdownObserver::~CMDSShutdownObserver()
    {
#ifdef _DEBUG
    PRINT(_L("CMDSShutdownObserver::~CMDSShutdownObserver()"));
#endif
    Cancel();
    iProperty.Close();
#ifdef _DEBUG
    PRINT(_L("CMDSShutdownObserver::~CMDSShutdownObserver() end"));
#endif
    }

// ---------------------------------------------------------------------------
// CMDSShutdownObserver::RunL()
// ---------------------------------------------------------------------------
//
void CMDSShutdownObserver::RunL()
    {
    // resubscribe before processing new value to prevent missing updates
    iProperty.Subscribe(iStatus);
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

TInt CMDSShutdownObserver::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMDSShutdownObserver::DoCancel()
// ---------------------------------------------------------------------------
//
void CMDSShutdownObserver::DoCancel()
    {
    iProperty.Cancel();
    }


// ---------------------------------------------------------------------------
// CMDSShutdownObserver::UpdateInProgress()
// ---------------------------------------------------------------------------
//
TBool CMDSShutdownObserver::UpdateInProgress()
    {
    return iValue > 0 ? ETrue : EFalse;
    }
	
// End of file
