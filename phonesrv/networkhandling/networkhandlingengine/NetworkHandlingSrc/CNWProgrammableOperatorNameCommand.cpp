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
*                CNWProgrammableOperatorNameCommand class
*
*/



// INCLUDE FILES
#include    "CNWMessageHandler.h"
#include    "CNWProgrammableOperatorNameCommand.h"
#include    "NWLogger.h"


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWProgrammableOperatorNameCommand::CNWProgrammableOperatorNameCommand
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWProgrammableOperatorNameCommand::CNWProgrammableOperatorNameCommand(
    CNWMessageHandler& aOwner,                 // ref. to class owner
    RMmCustomAPI&   aCustomAPI,             // ref. to Custom API
    TNWInfo& aNetworkInfo )  // ref. to current network info struct
    : CActive( EPriorityStandard ), 
      iOwner( aOwner ),
      iCustomAPI( aCustomAPI ), 
      iNetworkInfo( aNetworkInfo )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWProgrammableOperatorNameCommand::\
        CNWProgrammableOperatorNameCommand() Begin" );
    
    CActiveScheduler::Add( this );

    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWProgrammableOperatorNameCommand::\
        CNWProgrammableOperatorNameCommand() End" );
    }

    
// Destructor
CNWProgrammableOperatorNameCommand::~CNWProgrammableOperatorNameCommand()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWProgrammableOperatorNameCommand::\
        ~CNWProgrammableOperatorNameCommand() Begin" );
    
    Cancel();
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWProgrammableOperatorNameCommand::\
        ~CNWProgrammableOperatorNameCommand() Begin" );
    }


// ----------------------------------------------------------------------------
// CNWNetworkCommand::DoCancel
// Cancels the pending async. command.
// Cancels the pending async. command. The active command is
// resolved from iNetworkServiceRequest member.
// ----------------------------------------------------------------------------
//
void CNWProgrammableOperatorNameCommand::DoCancel()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWProgrammableOperatorNameCommand::DoCancel() Begin" );
    
    iCustomAPI.CancelAsyncRequest( ECustomGetOperatorNameIPC );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWProgrammableOperatorNameCommand::DoCancel() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkCommand::IssueRequest
// Initialises the Programmable Operator Name query to Custom API. 
// ----------------------------------------------------------------------------
//
void CNWProgrammableOperatorNameCommand::IssueRequest()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWProgrammableOperatorNameCommand::IssueRequest() Begin" );
    
    if ( !IsActive() )
        {
        NWLOGSTRING( KNWREQOUT, 
            "NW: CNWProgrammableOperatorNameCommand::\
            GetProgrammableOperatorInfo: iCustomAPI.GetOperatorName() \
            called" );
        iCustomAPI.GetOperatorName( iStatus, iOperatorName );
        SetActive();
        }
    else 
        {
        NWLOGSTRING( KNWERROR, 
            "NW: CNWProgrammableOperatorNameCommand::\
            GetProgrammableOperatorName: Already active" );
        Cancel();
        NWLOGSTRING( KNWREQOUT, 
            "NW: CNWProgrammableOperatorNameCommand::\
            GetProgrammableOperatorInfo: IssueRequest \
            Call IssueRequest again" );
        IssueRequest();
        }

    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWProgrammableOperatorNameCommand::DoCancel() End" );
    }

// ----------------------------------------------------------------------------
// CNWProgrammableOperatorNameCommand::RunL
// Receives the completion of the pending async. command
// ----------------------------------------------------------------------------
//
void CNWProgrammableOperatorNameCommand::RunL()
    {
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWProgrammableOperatorNameCommand::RunL() Begin,\
        iStatus.Int() = %d", iStatus.Int() );

    if ( iStatus != KErrNone )
        {
        NWLOGSTRING( KNWINT, 
            "NW: CNWProgrammableOperatorNameCommand::RunL, Operator Name \
            Not Found!" );
        iOwner.SendErrorMessage( MNWMessageObserver:: ENWGetProgrammableOperatorName, iStatus.Int() );
        }
    else
        {
        UpdateNetworkInfo();
        }
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWProgrammableOperatorNameCommand::RunL() End" );
    }

// ----------------------------------------------------------------------------
// CNWProgrammableOperatorNameCommand::UpdateNetworkInfo
// Updates the Operator name to the network info struct.
// ----------------------------------------------------------------------------
//
void CNWProgrammableOperatorNameCommand::UpdateNetworkInfo()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWProgrammableOperatorNameCommand::UpdateNetworkInfo() Begin");

    iNetworkInfo.iOperatorNameInfo.iType = iOperatorName.iType;
    iNetworkInfo.iOperatorNameInfo.iName = iOperatorName.iName; 
    
    NWLOGSTRING3( KNWINT, 
        "NW: CNWProgrammableOperatorNameCommand::UpdateNetworkInfo(),\
        iNetworkInfo.iOperatorNameInfo.iType = %d, iName = %S", 
        iNetworkInfo.iOperatorNameInfo.iType,
        &(iNetworkInfo.iOperatorNameInfo.iName) );

    iOwner.SendMessage( 
        MNWMessageObserver::ENWMessageProgrammableOperatorInfoChange );

    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWProgrammableOperatorNameCommand::UpdateNetworkInfo() End");
    }

//  End of File  
