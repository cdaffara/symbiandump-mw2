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
* Description:  Implementation of the class RMcnSubSession.
*
*/


// INCLUDE FILES
#include "CbsServerConstants.h"
#include <cbsmcnpanic.h>
#include <rcbsmcnsession.h>
#include <rmcnsubsession.h>
#include <ccbsmcnlistener.h>
#include <cmcntopicarray.h>
#include "CbsLogger.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// RMcnSubSession::RMcnSubSession
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
RMcnSubSession::RMcnSubSession()
    : iConnected( EFalse )
    {    //lint !e1928    
    }

// Destructor
RMcnSubSession::~RMcnSubSession()
    {
    Close();
    }

// -----------------------------------------------------------------------------
// RMcnSubSession::Open
// Creates a MCN subsession to CbsServer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RMcnSubSession::Open( 
    RCbsMcnSession& aMcnSession )
    {
    CBSLOGSTRING("CBSMCNCLIENT: >>> RMcnSubSession::Open()");

    if ( !iConnected )
        {
        const TIpcArgs args( TIpcArgs::ENothing );
        TInt result( CreateSubSession( aMcnSession, EMcnCreateSubsession, args ) );

        if ( result == KErrNone )
            {            
            CBSLOGSTRING2("CBSMCNCLIENT: RMcnSubSession::Open(): Subsession created, result: %d", result );
            iConnected = ETrue;
            } 
        else
            {
            iConnected = EFalse;
            }
        CBSLOGSTRING2("CBSMCNCLIENT: RMcnSubSession::Open(), returning %d.", result );        
        return result;
        }
    CBSLOGSTRING("CBSMCNCLIENT: <<< RMcnSubSession::Open(), returning KErrGeneral.");    
    return KErrGeneral;       
    }

// -----------------------------------------------------------------------------
// RMcnSubSession::Close
// Closes the subsession.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RMcnSubSession::Close()
    {
    CBSLOGSTRING("CBSMCNCLIENT: >>> RMcnSubSession::Close()");

    if ( iConnected ) 
        {
        RSubSessionBase::CloseSubSession( EMcnCloseSubsession );
        iConnected = EFalse;
        }
    CBSLOGSTRING("CBSMCNCLIENT: <<< RMcnSubSession::Close()");
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// RMcnSubSession::ReadMcnMessage
// Request next subscribed MCN message to be delivered to this session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RMcnSubSession::ReadMcnMessage( 
    TRequestStatus& aStatus, 
    TPckg<TCbsMcnMessage>& aMcnPckg )
    {
    __ASSERT_DEBUG( iConnected, CbsMcnPanic( EMcnNotConnected ) );    
    const TIpcArgs args( &aMcnPckg );

    SendReceive( EMcnNotifyOnChange, args, aStatus );  // asynchronous
    }

// -----------------------------------------------------------------------------
// RMcnSubSession::ReadMcnMessageCancel
// Cancels an outstanding request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RMcnSubSession::ReadMcnMessageCancel() 
    {
    __ASSERT_DEBUG( iConnected, CbsMcnPanic( EMcnNotConnected ) );
    const TIpcArgs args( TIpcArgs::ENothing );
    return SendReceive( EMcnNotifyOnChangeCancel, args );
    }

// -----------------------------------------------------------------------------
// RMcnSubSession::RegisterL
// Subscribes topics given in aArray.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RMcnSubSession::RegisterL( 
    const CMcnTopicArray& aArray )
    {
    CBSLOGSTRING("CBSMCNCLIENT: >>> RMcnSubSession::RegisterL()");

    __ASSERT_DEBUG( iConnected, CbsMcnPanic( EMcnNotConnected ) );
    // Store topic numbers into a descriptor.
    TUint entries( 0 );
    aArray.EnumerateEntries( entries );
    TUint number( 0 );

    // Request registration, pass topic numbers one at time to server-side.
    
    for( TUint i = 0; i < entries; i++ )
        {
        aArray.GetTopicNumber( i, number );
        const TIpcArgs args( number );        

        CBSLOGSTRING2("CBSMCNCLIENT: RMcnSubSession::RegisterL(): Passing topic number to server: %d", number );
        User::LeaveIfError( SendReceive( EMcnSubscribeTopic, args ) );        
        CBSLOGSTRING("CBSMCNCLIENT: RMcnSubSession::RegisterL(): Topic number passed to server OK." );
        }

    // Tell the server this was enough.
    const TIpcArgs args2( TIpcArgs::ENothing );
    User::LeaveIfError( SendReceive( EMcnNoMoreSubscriptions, args2 ) );

    CBSLOGSTRING("CBSMCNCLIENT: <<< RMcnSubSession::RegisterL()");
    }

// -----------------------------------------------------------------------------
// RMcnSubSession::ClearSubscriptionsL
// Clears topic subscriptions of this session in CbsServer
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RMcnSubSession::ClearSubscriptionsL()
    {
    CBSLOGSTRING("CBSMCNCLIENT: >>> RMcnSubSession::ClearSubscriptionsL()");

    __ASSERT_DEBUG( iConnected, CbsMcnPanic( EMcnNotConnected ) );
    const TIpcArgs args( TIpcArgs::ENothing );
    User::LeaveIfError( SendReceive( EMcnClearSubscriptions, args ) );

    CBSLOGSTRING("CBSMCNCLIENT: <<< RMcnSubSession::ClearSubscriptionsL(), cleared OK.");    
    }

//  End of File  
