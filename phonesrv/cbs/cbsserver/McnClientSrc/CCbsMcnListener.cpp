/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains Implementation of the class CCbsMcnListener member 
*                functions.
*
*/


// INCLUDE FILES

#include <ccbsmcnlistener.h>
#include <rcbsmcnsession.h>
#include <cmcn.h>
#include "CbsLogger.h"

// CONSTANTS

const TInt KMcnListenerPriority = CActive::EPriorityStandard;

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsMcnListener::CCbsMcnListener
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsMcnListener::CCbsMcnListener( 
    CMcn& aMcn, 
    RCbsMcnSession& aSession )
    : CActive( KMcnListenerPriority ), 
      iMcn( aMcn ), 
      iSession( aSession ),
      iMcnPckg( iMcnMessage )
    {
    }

// -----------------------------------------------------------------------------
// CCbsMcnListener::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsMcnListener::ConstructL()
    {    
    CBSLOGSTRING("CBSMCNCLIENT: >>> CCbsMcnListener::ConstructL()");

    CActiveScheduler::Add( this );
    Receive();

    CBSLOGSTRING("CBSMCNCLIENT: <<< CCbsMcnListener::ConstructL()");
    }

// -----------------------------------------------------------------------------
// CCbsMcnListener::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsMcnListener* CCbsMcnListener::NewL( 
    CMcn& aMcn, 
    RCbsMcnSession& aSession )
    {
    CCbsMcnListener* self = new ( ELeave ) CCbsMcnListener( aMcn, aSession );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

    
// Destructor
CCbsMcnListener::~CCbsMcnListener() 
    {
    Cancel();
    }
// -----------------------------------------------------------------------------
// CCbsMcnListener::RunL
// This method is called when an Mcn message is received.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMcnListener::RunL()
    {   
    CBSLOGSTRING2("CBSMCNCLIENT: >>> CCbsMcnListener::RunL(): iStatus: %d", iStatus.Int() );
    CBSLOGSTRING2("CBSMCNCLIENT: CCbsMcnListener::RunL(): Msg topic number: %d", iMcnMessage.iTopicNumber );
    CBSLOGSTRING2("CBSMCNCLIENT: CCbsMcnListener::RunL(): Msg network mode: %d", iMcnMessage.iNetworkMode );

    // MCN message and parameters received from the network 
    // and stored into iMcnMessage
    iMcn.RelayMessage( iMcnMessage );
    if ( iStatus.Int() != KErrServerTerminated && iStatus.Int() != KErrCancel )
        {
        Receive();
        }    
    
    CBSLOGSTRING("CBSMCNCLIENT: <<< CCbsMcnListener::RunL()");
    }

// -----------------------------------------------------------------------------
// CCbsMcnListener::DoCancel
// Cancels the outstanding Mcn request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMcnListener::DoCancel()
    {
    iSession.ReadMcnMessageCancel();
    }

// -----------------------------------------------------------------------------
// CCbsMcnListener::Receive
//  Receive an Mcn message from network.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMcnListener::Receive()
    {
    CBSLOGSTRING("CBSMCNCLIENT: >>> CCbsMcnListener::Receive()");

    iSession.ReadMcnMessage( iStatus, iMcnPckg );
     
    SetActive();
    CBSLOGSTRING("CBSMCNCLIENT: <<< CCbsMcnListener::Receive()");
    }

//  End of File  
