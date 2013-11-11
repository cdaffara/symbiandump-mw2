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
* Description:  Implementation of the class RCbsMcnSession.
*
*/


// INCLUDE FILES

#include "CbsServerConstants.h"
#include <cbsmcnpanic.h>
#include <cmcn.h>
#include <rcbsmcnsession.h>
#include <ccbsmcnlistener.h>
#include <cmcntopicarray.h>
#include "CbsLogger.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// RCbsMcnSession::RCbsMcnSession
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
RCbsMcnSession::RCbsMcnSession()
    : iConnected( EFalse )
    { //lint !e1928 !e1926
    }                               
    
// Destructor
RCbsMcnSession::~RCbsMcnSession()
    {
    Close();
    }

// -----------------------------------------------------------------------------
// RCbsMcnSession::GetCellInfo
// Retrieves the current MCN message from CBS server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsMcnSession::GetCellInfo( 
    TDes& aMcnMessage ) const
    {
    return GetInfoMessage( aMcnMessage, KCellInfoTopic );
    }

// -----------------------------------------------------------------------------
// RCbsMcnSession::GetInfoMessage
// Retrieves the current HomeZone message from CBS server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsMcnSession::GetInfoMessage( 
    TDes& aMessage, 
    const TInt aTopicNumber ) const
    {
    __ASSERT_DEBUG( iConnected, CbsMcnPanic( EMcnNotConnected ) );

    const TIpcArgs args( &aMessage, aTopicNumber );
    TInt result( SendReceive( EMcnGetInfoMessage, args ) );
    return result;
    }

// -----------------------------------------------------------------------------
// RCbsMcnSession::ReadMcnMessage
// Request next subscribed MCN message to be delivered to this session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCbsMcnSession::ReadMcnMessage( 
    TRequestStatus& aStatus, 
    TPckg<TCbsMcnMessage>& aMcnPckg ) 
    {
    __ASSERT_DEBUG( iConnected, CbsMcnPanic( EMcnNotConnected ) );
    iSubSession.ReadMcnMessage( aStatus, aMcnPckg );
    }

// -----------------------------------------------------------------------------
// RCbsMcnSession::ReadMcnMessageCancel
// Cancels an outstanding request. Passed to the subsession object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsMcnSession::ReadMcnMessageCancel() 
    {
    __ASSERT_DEBUG( iConnected, CbsMcnPanic( EMcnNotConnected ) );
    return iSubSession.ReadMcnMessageCancel();
    }

// -----------------------------------------------------------------------------
// RCbsMcnSession::RegisterL
// Subscribes topics given in aArray.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCbsMcnSession::RegisterL( 
    const CMcnTopicArray& aArray )
    {
    CBSLOGSTRING("CBSMCNCLIENT: >>> RCbsMcnSession::RegisterL()");

    __ASSERT_DEBUG( iConnected, CbsMcnPanic( EMcnNotConnected ) );
    iSubSession.RegisterL( aArray );
    
    CBSLOGSTRING("CBSMCNCLIENT: <<< RCbsMcnSession::RegisterL()");
    }

// -----------------------------------------------------------------------------
// RCbsMcnSession::ClearSubscriptionsL
// Clears all topic subscriptions of this session in CbsServer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCbsMcnSession::ClearSubscriptionsL()
    {
    CBSLOGSTRING("CBSMCNCLIENT: >>> RCbsMcnSession::ClearSubscriptionsL()");

    __ASSERT_DEBUG( iConnected, CbsMcnPanic( EMcnNotConnected ) );
    iSubSession.ClearSubscriptionsL();

    CBSLOGSTRING("CBSMCNCLIENT: <<< RCbsMcnSession::ClearSubscriptionsL()");
    }

// -----------------------------------------------------------------------------
// RCbsMcnSession::Connect
// Establishes a connection with CbsServer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsMcnSession::Connect()
    {
    CBSLOGSTRING("CBSMCNCLIENT: >>> RCbsMcnSession::Connect()");

    // Create a session to the server.
    TInt result = CreateSession( KCbsServerName,
       Version(), KCbsServerDefaultSlots );

    CBSLOGSTRING2("CBSMCNCLIENT: RCbsMcnSession::Connect(): CreateSession() result: %d", result );    

    if ( result == KErrNone )
        {
        TInt error = iSubSession.Open( *this );
        if ( !error )
            {
            iConnected = ETrue;
            CBSLOGSTRING("CBSMCNCLIENT: RCbsMcnSession::Connect(): Subsession opened.");            
            }        
        CBSLOGSTRING2("CBSMCNCLIENT: RCbsMcnSession::Connect(): Subsession opening failed, error: %d", error );        
        } 
    else
        {
        iConnected = EFalse;
        CBSLOGSTRING2("CBSMCNCLIENT: RCbsMcnSession::Connect(): CreateSession failed, errorCode: %d", result );
        }

    CBSLOGSTRING2("CBSMCNCLIENT: <<< RCbsMcnSession::Connect(), iConnected: %d", iConnected );    
    return result;
    }

// -----------------------------------------------------------------------------
// RCbsMcnSession::Close
// Closes the connection with CbsServer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCbsMcnSession::Close()
    {
    CBSLOGSTRING("CBSMCNCLIENT: >>> RCbsMcnSession::Close()");

    if ( iConnected ) 
        {
        iSubSession.Close();

        const TIpcArgs args( TIpcArgs::ENothing );
        SendReceive( EMcnCloseSession, args );
        RSessionBase::Close();
        iConnected = EFalse;

        CBSLOGSTRING("CBSMCNCLIENT: RCbsMcnSession::Close(): Connected -> subsession closed.");        
        }
    CBSLOGSTRING("CBSMCNCLIENT: <<< RCbsMcnSession::Close()");
    }

// -----------------------------------------------------------------------------
// RCbsMcnSession::Version
// Returns version information of this dll.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TVersion RCbsMcnSession::Version() const
    {
    // Create version and return it.
    return TVersion( KCbsServerVersionMajor,
                     KCbsServerVersionMinor,
                     KCbsServerVersionBuild );
    }
    
//  End of File  
