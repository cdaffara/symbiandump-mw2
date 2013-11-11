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
*                CNWNetworkProviderNameCommand class
*
*/



// INCLUDE FILES
#include    "CNWMessageHandler.h"
#include    "CNWNetworkProviderNameCommand.h"
#include    "NWLogger.h"


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWNetworkProviderNameCommand::CNWNetworkProviderNameCommand
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkProviderNameCommand::CNWNetworkProviderNameCommand(
            CNWMessageHandler& aOwner,
            RMmCustomAPI& aCustomAPI,
            TNWInfo& aNetworkInfo )
            : CActive( EPriorityStandard ), 
              iOwner( aOwner ),
              iCustomAPI( aCustomAPI ), 
              iNetworkInfo( aNetworkInfo )
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkProviderNameCommand::\
        CNWNetworkProviderNameCommand() Begin" );
    
    CActiveScheduler::Add( this );
    
    NWLOGSTRING(KNWOBJECT, 
        "NW: CNWNetworkProviderNameCommand::\
        CNWNetworkProviderNameCommand() End");
    }

    
// Destructor
CNWNetworkProviderNameCommand::~CNWNetworkProviderNameCommand()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkProviderNameCommand::\
        ~CNWNetworkProviderNameCommand() Begin" );
    
    Cancel();
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkProviderNameCommand::\
        ~CNWNetworkProviderNameCommand() End" );
    }


// ----------------------------------------------------------------------------
// CNWNetworkProviderNameCommand::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CNWNetworkProviderNameCommand* CNWNetworkProviderNameCommand::NewL(
                                                CNWMessageHandler& aOwner,
                                                RMmCustomAPI& aCustomAPI,
                                                TNWInfo& aNetworkInfo )
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkProviderNameCommand::NewL() Begin" );
    
    CNWNetworkProviderNameCommand* self = new( 
            ELeave ) CNWNetworkProviderNameCommand( 
                    aOwner, aCustomAPI, aNetworkInfo );
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkProviderNameCommand::NewL() Begin" );
    return self;
    }

// ----------------------------------------------------------------------------
// CNWNetworkProviderNameCommand::DoCancel
// Cancels the pending async. command.
// ----------------------------------------------------------------------------
//
void CNWNetworkProviderNameCommand::DoCancel()
    {
    NWLOGSTRING( KNWREQEND,
        "NW: CNWNetworkProviderNameCommand::DoCancel() Begin" );
    
    iCustomAPI.CancelAsyncRequest( ECustomGetNetworkProviderNameIPC );
    
    NWLOGSTRING( KNWREQEND,
        "NW: CNWNetworkProviderNameCommand::DoCancel() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkProviderNameCommand::RunL
// Receives the completion of the pending async. command
// ----------------------------------------------------------------------------
//
void CNWNetworkProviderNameCommand::RunL()
    {
    NWLOGSTRING2( KNWREQEND, 
        "NW: CNWNetworkProviderNameCommand::RunL() Begin: \
        Error value: %I )", iStatus.Int() );
    
    if ( iStatus == KErrNone )  
        {
        UpdateNetworkInfo();
        iOwner.SendMessage( MNWMessageObserver::ENWMessageNetworkProviderNameChange );
        NWLOGSTRING2( KNWINT, 
            "NW: CNWNetworkProviderNameCommand::RunL: Network provider \
             name:%S", &iNetworkInfo.iNPName ); 
        }
    else
        {
        NWLOGSTRING( KNWERROR,
            "NW: CNWNetworkProviderNameCommand::RunL, Network Provider \
            Name not found!" );
        iOwner.SendErrorMessage( MNWMessageObserver::ENWGetNetworkProviderName, iStatus.Int() );
        }
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkProviderNameCommand::RunL() End" );
    }
// ----------------------------------------------------------------------------
// CNWNetworkProviderNameCommand::IssueRequest
// Starts the async. network provider name query. The return of the query is
// returned the RunL function.
// ----------------------------------------------------------------------------
//
void CNWNetworkProviderNameCommand::IssueRequest()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkProviderNameCommand::IssueRequest() Begin" );
    
    if ( !IsActive() )
        {
        NWLOGSTRING( KNWREQOUT,
            "NW: CNWNetworkProviderNameCommand::GetNetworkProviderName: \
            Request RMmCustomAPI::GetNetworkProviderName" );
        iCustomAPI.GetNetworkProviderName( iStatus, iNetworkProviderName );
        SetActive();
        }
    else 
        {
        NWLOGSTRING( KNWERROR, 
            "NW: CNWNetworkProviderNameCommand::GetNetworkProviderName: \
            Already active" );
        }
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkProviderNameCommand::IssueRequest() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkProviderNameCommand::UpdateNetworkInfo
// Updates the network provider name to the network info struct.
// ----------------------------------------------------------------------------
//
void CNWNetworkProviderNameCommand::UpdateNetworkInfo()
    { 
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkProviderNameCommand::UpdateNetworkInfo() Begin" );
    
    iNetworkInfo.iNPName.Copy( iNetworkProviderName.Ptr(), 
        iNetworkProviderName.Length() );
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkProviderNameCommand::UpdateNetworkInfo() End" );
    }


//  End of File  
