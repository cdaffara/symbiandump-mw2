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
#include "cphsrvussdreceivehandler.h" 
#include "mphsrvussdnetworkobserver.h" 


// CONSTANTS
const TInt KPhSrvUssdReceiverPriority = CActive::EPriorityLow + 2;



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvUssdReceiveHandler::CPhSrvUssdReceiveHandler
// 
// C++ constructor
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
CPhSrvUssdReceiveHandler::CPhSrvUssdReceiveHandler(
    MPhSrvUssdNetworkObserver& aObserver,
    RMobileUssdMessaging& aUssdMessaging)
    :    CActive(KPhSrvUssdReceiverPriority), 
         iAutoReceive( ETrue ), // the value is hardcoded here.
         iObserver( aObserver ),
         iUssdMessaging( aUssdMessaging ),
         iMsgAttributesPckg( iMsgAttributes )
    {
    CActiveScheduler::Add( this );
    iMsgData.Zero();
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdReceiveHandler::~CPhSrvUssdReceiveHandler
// 
// C++ Destructor
// -----------------------------------------------------------------------------
//
CPhSrvUssdReceiveHandler::~CPhSrvUssdReceiveHandler()
    {
    Cancel();
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdReceiveHandler::ConstructL
// 
// Symbian OS 2nd phase constructor
// -----------------------------------------------------------------------------
//
void CPhSrvUssdReceiveHandler::ConstructL()
    {
    // Start receiving automatically if wanted.
    if ( iAutoReceive )
        {
        User::LeaveIfError( StartReceiving() );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdReceiveHandler::StartReceiving
// 
// Start receiving a message
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
TInt CPhSrvUssdReceiveHandler::StartReceiving()
    {
    if ( IsActive() )
        {
        return KErrInUse;
        } 

    iUssdMessaging.ReceiveMessage( iStatus, iMsgData , iMsgAttributesPckg );
    SetActive();
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdReceiveHandler::RunL
// 
// Called when a message is received
// -----------------------------------------------------------------------------
//
void CPhSrvUssdReceiveHandler::RunL()
    {
    TInt error = iStatus.Int();
    if ( error == KErrCancel )
        {
        return;
        }
    iObserver.UssdNetworkObserverHandleReceivedEventL( 
        iMsgData, 
        iMsgAttributes, 
        error );
    
    if ( iAutoReceive && error != KErrCancel && error != KErrNotSupported )
        {
        StartReceiving();
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdReceiveHandler::DoCancel
// 
// Cancel a pending request
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
void CPhSrvUssdReceiveHandler::DoCancel()
    {
    iUssdMessaging.CancelAsyncRequest( EMobileUssdMessagingReceiveMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdReceiveHandler::RunError
// 
// Called when RunL leaves
// -----------------------------------------------------------------------------
//
TInt CPhSrvUssdReceiveHandler::RunError( TInt aError )
    {
    // Shows an error note.
    TRAP_IGNORE(  iObserver.UssdNetworkObserverHandleReceivedEventL( 
        iMsgData, iMsgAttributes , aError ) );
    
    if ( iAutoReceive )
        {
        StartReceiving();
        }
    
    return KErrNone;
    }


//  End of File  
