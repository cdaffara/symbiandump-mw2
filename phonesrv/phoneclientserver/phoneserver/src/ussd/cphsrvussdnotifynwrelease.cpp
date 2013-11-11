/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Ussd Handler Receive.
*
*/



// INCLUDE FILES
#include "cphsrvussdnotifynwrelease.h" 
#include "mphsrvussdnetworkobserver.h" 
#include "phsrvdebuginfo.h" 

// CONSTANTS
const TInt KPhSrvUssdReceiverPriority = CActive::EPriorityLow + 2;



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvUssdNotifyNWRelease::CPhSrvUssdNotifyNWRelease
// 
// C++ constructor
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
CPhSrvUssdNotifyNWRelease::CPhSrvUssdNotifyNWRelease(
    MPhSrvUssdNetworkObserver& aObserver,
    RMobileUssdMessaging& aUssdMessaging)
    :    CActive(KPhSrvUssdReceiverPriority), 
      
         iObserver( aObserver ),
         iUssdMessaging( aUssdMessaging ),
         iReturnResultPckg ( iReturnResult )
         
    {
    CActiveScheduler::Add( this );
    
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdNotifyNWRelease::~CPhSrvUssdNotifyNWRelease
// 
// C++ Destructor
// -----------------------------------------------------------------------------
//
CPhSrvUssdNotifyNWRelease::~CPhSrvUssdNotifyNWRelease()
    {
    Cancel();
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdNotifyNWRelease::ConstructL
// 
// Symbian OS 2nd phase constructor
// -----------------------------------------------------------------------------
//
void CPhSrvUssdNotifyNWRelease::ConstructL()
    {
    // Start notify a client of a MT USSD release message to the mobile device
    NotifyNetworkRelease();
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdNotifyNWRelease::StartReceiving
// 
// Start receiving a message
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
TInt CPhSrvUssdNotifyNWRelease::NotifyNetworkRelease()
    {
    if ( IsActive() )
        {
        return KErrInUse;
        } 
    _DPRINT( 4, "PhSrv.NotifyNetworkRelease" );
    iUssdMessaging.NotifyNetworkRelease( iStatus, iReturnResultPckg );
    SetActive();
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdNotifyNWRelease::RunL
// 
// Called when a message is received
// -----------------------------------------------------------------------------
//
void CPhSrvUssdNotifyNWRelease::RunL()
    {
    TInt error = iStatus.Int();
    if ( error == KErrCancel )
        {
        return;
        }
    _DPRINT( 4, "PhSrv.NotifyNetworkRelease:RunL" );
    iObserver.UssdNetworkObserverHandleNotifyNWReleaseL( 
        iReturnResult,
        error ); 
    NotifyNetworkRelease();      
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdNotifyNWRelease::DoCancel
// 
// Cancel a pending request
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
void CPhSrvUssdNotifyNWRelease::DoCancel()
    {
    iUssdMessaging.CancelAsyncRequest( EMobileUssdMessagingNotifyNetworkRelease );
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdNotifyNWRelease::RunError
// 
// Called when RunL leaves
// -----------------------------------------------------------------------------
//
TInt CPhSrvUssdNotifyNWRelease::RunError( TInt aError )
    {
    
    TRAP_IGNORE( iObserver.UssdNetworkObserverHandleNotifyNWReleaseL( 
         iReturnResult, aError ) );
        
    return KErrNone;
    }


//  End of File  
