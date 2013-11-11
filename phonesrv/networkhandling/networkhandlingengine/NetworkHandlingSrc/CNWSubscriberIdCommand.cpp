/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains implementation of 
*                CNWSubscriberIdCommand class
*
*/



// INCLUDE FILES
#include    "CNWSubscriberIdCommand.h"
#include    "CNWMessageHandler.h"
#include    "NWLogger.h"


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWSubscriberIdCommand::CNWSubscriberIdCommand
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWSubscriberIdCommand::CNWSubscriberIdCommand(
            CNWMessageHandler& aOwner,
            RMobilePhone& aPhone,
            TNWInterInfo& aInterNetworkInfo )
            : CActive( EPriorityStandard ),
              iOwner( aOwner ),
              iPhone( aPhone ),
              iInterNetworkInfo( aInterNetworkInfo ),
              iSubscriberIdCounter( 0 )
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWSubscriberIdCommand::CNWSubscriberIdCommand() Begin" );
    
    CActiveScheduler::Add( this );

    NWLOGSTRING( KNWOBJECT,
        "NW: CNWSubscriberIdCommand::CNWSubscriberIdCommand() End" );
    }

    
// Destructor
CNWSubscriberIdCommand::~CNWSubscriberIdCommand()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWSubscriberIdCommand::~CNWSubscriberIdCommand() Begin" );
    
    Cancel();

    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWSubscriberIdCommand::~CNWSubscriberIdCommand() End");
    }


// ----------------------------------------------------------------------------
// CNWSubscriberIdCommand::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CNWSubscriberIdCommand* CNWSubscriberIdCommand::NewL(
                                        CNWMessageHandler& aOwner,
                                        RMobilePhone& aPhone,
                                        TNWInterInfo& aInterNetworkInfo )
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWSubscriberIdCommand::NewL() Begin" );
    
    CNWSubscriberIdCommand* self = new( ELeave ) CNWSubscriberIdCommand(
                                        aOwner,
                                        aPhone,
                                        aInterNetworkInfo );

    NWLOGSTRING( KNWOBJECT,
        "NW: CNWSubscriberIdCommand::NewL() Begin" );
    return self;
    }

// ----------------------------------------------------------------------------
// CNWSubscriberIdCommand::DoCancel
// Cancels the pending async. command.
// ----------------------------------------------------------------------------
//
void CNWSubscriberIdCommand::DoCancel()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWSubscriberIdCommand::DoCancel() Begin" );
    
    iPhone.CancelAsyncRequest( EMobilePhoneGetSubscriberId );
    
    NWLOGSTRING( KNWREQEND,
        "NW: CNWSubscriberIdCommand::DoCancel() End" );
    }

// ----------------------------------------------------------------------------
// CNWSubscriberIdCommand::RunL
// Receives the completion of the pending async. command
// ----------------------------------------------------------------------------
//
void CNWSubscriberIdCommand::RunL()
    {
    NWLOGSTRING2( KNWOBJECT,
        "NW: CNWSubscriberIdCommand::RunL() Begin, iStatus.Int() = \
        %d", iStatus.Int() );
    
    if ( iStatus != KErrNone )
        {
        NWLOGSTRING2( KNWERROR,
            "NW: CNWSubscriberIdCommand::RunL, Unknown error situation, \
            iStatus = %d", iStatus.Int() );
            // Subscriber Id is NOT necessary available when system is 
            // registered to network (= SIM State OK). (This might happen 
            // when PIN query is ON).Therefore we try to get subscriber 
            // id several times.
        if ( iSubscriberIdCounter < KMaxSubscriberIdRequests )
            {
            NWLOGSTRING2( KNWERROR, 
                "NW:CNWMessageHandler::SendMessage: New subscriber id\
                request. counter = %d", iSubscriberIdCounter );
            IssueRequest();
            iSubscriberIdCounter++;
            return;
            }
        }
    else
        {
        iOwner.HandleSubscriberIdChange();
        }
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWSubscriberIdCommand::RunL() End" );
    }

// ----------------------------------------------------------------------------
// CNWSubscriberIdCommand::IssueRequest
// Starts the async. subscriber ID query. The return of the query is returned 
// the RunL function.
// ----------------------------------------------------------------------------
//
void CNWSubscriberIdCommand::IssueRequest()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWSubscriberIdCommand::IssueRequest() Begin" );
    
    if ( !IsActive() )
        {
        NWLOGSTRING( KNWREQOUT,
            "NW: CNWSubscriberIdCommand::GetSubscriberId: Request \
            RMmCustomAPI::GetSubscriberId" );
        iPhone.GetSubscriberId( iStatus, iInterNetworkInfo.iSubscriberId );
        SetActive();
        }
    else 
        {
        NWLOGSTRING( KNWERROR, 
            "NW: CNWSubscriberIdCommand::GetSubscriberId: Already active" );
        }
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWSubscriberIdCommand::IssueRequest() End" );
    }


//  End of File  
