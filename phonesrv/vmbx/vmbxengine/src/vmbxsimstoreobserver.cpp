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
* Description: Implementation of CVmbxSimStoreObserver class.
*
*/

// INCLUDE FILES
#include <etelmmcs.h>
#include "vmbxsimstoreobserver.h"
#include "vmbxsimhandler.h"
#include "vmbxlogger.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVmbxSimStoreObserver::CVmbxSimStoreObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVmbxSimStoreObserver::CVmbxSimStoreObserver( CVmbxSimHandler& aSimHandler,
                                             MVmbxSimStoreObserver& aObserver )
                                            : CActive( EPriorityStandard ),
                                            iSimHandler( aSimHandler ),
                                            iObserver( aObserver )
    {
    VMBLOGSTRING( "VMBX:CVmbxSimStoreObserver::CVmbxSimStoreObserver =>" );
    CActiveScheduler::Add( this );
    VMBLOGSTRING( "VMBX:CVmbxSimStoreObserver::CVmbxSimStoreObserver <=" );
    }

// -----------------------------------------------------------------------------
// CVmbxSimStoreObserver::~CVmbxSimStoreObserver
// Destructor
// -----------------------------------------------------------------------------
//
CVmbxSimStoreObserver::~CVmbxSimStoreObserver()
    {
    VMBLOGSTRING( "VMBX:CVmbxSimStoreObserver::~CVmbxSimStoreObserver =>" );
    Cancel();
    VMBLOGSTRING( "VMBX:CVmbxSimStoreObserver::~CVmbxSimStoreObserver <=" );
    }

// -----------------------------------------------------------------------------
// CVmbxSimStoreObserver::NewL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
CVmbxSimStoreObserver* CVmbxSimStoreObserver::NewL( 
    CVmbxSimHandler& aSimHandler, MVmbxSimStoreObserver& aObserver )
    {
    VMBLOGSTRING( "VMBX: CVmbxSimStoreObserver::NewL =>" );
    CVmbxSimStoreObserver* self = 
        new( ELeave ) CVmbxSimStoreObserver( aSimHandler, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVmbxSimStoreObserver::NewL <=" );
    return self;
    }

// -----------------------------------------------------------------------------
// CVmbxSimStoreObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVmbxSimStoreObserver::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmbxSimStoreObserver::ConstructL =>" );
    ActivateL();
    VMBLOGSTRING( "VMBX: CVmbxSimStoreObserver::ConstructL <=" );
    }

// -----------------------------------------------------------------------------
// CVmbxSimStoreObserver::Activate
//
// -----------------------------------------------------------------------------
//
void CVmbxSimStoreObserver::ActivateL()
   {
   VMBLOGSTRING( "VMBX:CVmbxSimStoreObserver::ActivateL =>" );
   if ( !IsActive() )
        {
        iSimHandler.NotifyStoreEvent( iStatus, iEvents, iIndex );
        SetActive();    
        }
    else
        {
        User::Leave( KErrInUse );
        }
   VMBLOGSTRING( "VMBX:CVmbxSimStoreObserver::ActivateL <=" );
   }

// -----------------------------------------------------------------------------
// CVmbxSimStoreObserver::RunL
// -----------------------------------------------------------------------------
//
void CVmbxSimStoreObserver::RunL()
    {
    VMBLOGSTRING( "VMBX:CVmbxSimStoreObserver::RunL =>" );
    // Activate the notication again
    ActivateL();
    iObserver.HandleSimStoreChanged();
    VMBLOGSTRING( "VMBX:CVmbxSimStoreObserver::RunL <=" );
    }

// -----------------------------------------------------------------------------
// CVmbxSimStoreObserver::DoCancel
// -----------------------------------------------------------------------------
//
void CVmbxSimStoreObserver::DoCancel()
    {
    VMBLOGSTRING( "VMBX:CVmbxSimStoreObserver::DoCancel =>" );
    if ( IsActive() )
        {
        iSimHandler.CancelAsyncRequest( EMobilePhoneStoreNotifyStoreEvent );
        }
    VMBLOGSTRING( "VMBX:CVmbxSimStoreObserver::DoCancel <=" );
    }

//End of file
