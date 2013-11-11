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
*                CNWServiceProviderNameCommand class
*
*/



// INCLUDE FILES
#include    "CNWMessageHandler.h"
#include    "CNWServiceProviderNameCommand.h"
#include    "NWPanic.pan"
#include    "NWLogger.h"


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWServiceProviderNameCommand::CNWServiceProviderNameCommand
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWServiceProviderNameCommand::CNWServiceProviderNameCommand
    (
    CNWMessageHandler& aOwner,                 // ref. to class owner
    RMobilePhone& aPhone,                   // ref. to mobile phone
    TNWInfo& aNetworkInfo          // ref. to network info struct
    ) : CActive( EPriorityStandard ), 
        iOwner( aOwner ),
        iPhone( aPhone ), 
        iNWNetworkInfo( aNetworkInfo ),
        iServiceProviderNamePckg( iParamServiceProviderName ),
        iAllowSending( ETrue )
    { //lint !e1403
    NWLOGSTRING(KNWOBJECT,
        "NW: CNWServiceProviderNameCommand::\
        CNWServiceProviderNameCommand() Begin");
    
    CActiveScheduler::Add( this );

    NWLOGSTRING(KNWOBJECT,
        "NW: CNWServiceProviderNameCommand::\
        CNWServiceProviderNameCommand() End");
    }

    
// Destructor
CNWServiceProviderNameCommand::~CNWServiceProviderNameCommand()
    {
    NWLOGSTRING(KNWOBJECT,
        "NW: CNWServiceProviderNameCommand::\
        ~CNWServiceProviderNameCommand() Begin");
    
    Cancel();
    
    NWLOGSTRING(KNWOBJECT,
        "NW: CNWServiceProviderNameCommand::\
        ~CNWServiceProviderNameCommand() End");
    }


// ----------------------------------------------------------------------------
// CNWServiceProviderNameCommand::DoCancel
// Cancels the pending async. command.
// Cancels the pending async. command. The active command is
// resolved from iNetworkServiceRequest member.
// ----------------------------------------------------------------------------
//
void CNWServiceProviderNameCommand::DoCancel()
    {
    NWLOGSTRING(KNWOBJECT,
        "NW: CNWServiceProviderNameCommand::DoCancel() Begin");
    
    iPhone.CancelAsyncRequest( EMobilePhoneGetServiceProviderName );
    
    NWLOGSTRING(KNWOBJECT,
        "NW: CNWServiceProviderNameCommand::DoCancel() End");
    }

// ----------------------------------------------------------------------------
// CNWServiceProviderNameCommand::IssueRequest
// Starts the async. GetServiceProviderName query. 
// The return of the query is returned the RunL function.
// ----------------------------------------------------------------------------
//
void CNWServiceProviderNameCommand::IssueRequest()
    {
    NWLOGSTRING(KNWOBJECT,
        "NW: CNWServiceProviderNameCommand::IssueRequest() Begin");
    
    if ( !IsActive() )
        {
        iPhone.GetServiceProviderName( iStatus, iServiceProviderNamePckg );
        NWLOGSTRING( KNWREQOUT, 
            "NW: CNWServiceProviderNameCommand::GetServiceProviderName: \
            Request issued");
        SetActive();
        }
    else 
        {
        NWLOGSTRING( KNWERROR, 
            "NW: CNWServiceProviderNameCommand::GetServiceProviderName: \
            Already active");
        }
    
    NWLOGSTRING(KNWOBJECT,
        "NW: CNWServiceProviderNameCommand::IssueRequest() End");
    }

// ----------------------------------------------------------------------------
// CNWServiceProviderNameCommand::UpdateNetworkInfo
// Updates the Custom API type network info to NW type.
// ----------------------------------------------------------------------------
//
void CNWServiceProviderNameCommand::UpdateNetworkInfo()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWServiceProviderNameCommand::UpdateNetworkInfo() Begin" );

    iNWNetworkInfo.iSPName.Copy( iParamServiceProviderName.iSPName.Ptr(), 
        iParamServiceProviderName.iSPName.Length() );
    iNWNetworkInfo.iPLMNField.Copy( iParamServiceProviderName.iPLMNField.Ptr(),
        iParamServiceProviderName.iPLMNField.Length() );
    iNWNetworkInfo.iServiceProviderNameDisplayReq = 
        static_cast<TNWDisplayRequirementsFlags>( 
                iParamServiceProviderName.iDisplayReq );

    NWLOGSTRING(KNWOBJECT,
        "NW: CNWServiceProviderNameCommand::UpdateNetworkInfo() End" );
    }

// ----------------------------------------------------------------------------
// CNWServiceProviderNameCommand::RunL
// Receives the completion of the pending async. command
// ----------------------------------------------------------------------------
//

void CNWServiceProviderNameCommand::RunL()
    {
    NWLOGSTRING2(KNWOBJECT,
        "NW: CNWServiceProviderNameCommand::RunL() Begin,\
        iStatus.Int() = %d", iStatus.Int() );

    if ( iStatus != KErrNone )  
        {
        NWLOGSTRING( KNWERROR,
            "NW: CNWServiceProviderNameCommand::RunL, Service Provider \
            Name not found!" );
        iOwner.SendErrorMessage( MNWMessageObserver::ENWGetServiceProviderName, iStatus.Int() );
        }
    else
        {
        UpdateNetworkInfo();
        if ( iAllowSending )
            {
            iOwner.SendMessage( MNWMessageObserver::ENWMessageServiceProviderNameChange );
            NWLOGSTRING2( KNWINT, 
                "NW: CNWServiceProviderNameCommand::RunL: Service provider \
                name:%S", &iNWNetworkInfo.iSPName ); 
            }
        }
    
    NWLOGSTRING(KNWOBJECT,
        "NW: CNWServiceProviderNameCommand::RunL() End" );
    }
    
// ----------------------------------------------------------------------------
// CNWServiceProviderNameCommand::AllowSendMessage
// Denies or allows sending of ENWMessageServiceProviderNameChange. If Refresh
// is ongoing this value is set EFalse. Default is ETrue. 
// ----------------------------------------------------------------------------
//
void CNWServiceProviderNameCommand::SetAllowSendMessage( TBool aAllowSending )
    {
    NWLOGSTRING2( KNWOBJECT,
        "NW: CNWServiceProviderNameCommand::AllowSendMessage() Begin,\
        aAllowSending = %d", aAllowSending );
    
    iAllowSending = aAllowSending;
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWServiceProviderNameCommand::AllowSendMessage() End" );
    }


//  End of File  
