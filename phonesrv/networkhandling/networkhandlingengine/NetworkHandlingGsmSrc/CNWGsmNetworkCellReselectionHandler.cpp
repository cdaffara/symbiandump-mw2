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
* Description:  This module contains the implementation of CNWGsmNetworkCellReselectionHandler
*                class member functions.
*
*/



// INCLUDE FILES
#include "CNWNetworkViagBaseEngine.h"
#include "CNWNetworkMCNEngine.h"
#include "CNWGsmNetworkCellReselectionHandler.h"
#include "NWLogger.h"

#include <CbsServerInternalPSKeys.h>




// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWGsmNetworkCellReselectionHandler::CNWGsmNetworkCellReselectionHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWGsmNetworkCellReselectionHandler::CNWGsmNetworkCellReselectionHandler(
        CNWGsmMessageHandler& aOwner,
        RMobilePhone& aPhone,
        TNWInfo& aNetworkInfo,
        RMmCustomAPI& aCustomAPI,
        TNWInterInfo& aInterNetworkInfo)
        : CActive( EPriorityStandard ),
        iOwner( aOwner ),
        iPhone( aPhone ),
        iNWNetworkInfo( aNetworkInfo ),
        iCustomAPI ( aCustomAPI ),
        iInterNetworkInfo( aInterNetworkInfo )
    {
    NWLOGSTRING( KNWMESOUT, 
        "NW: CNWGsmMessageHandler::CNWGsmNetworkCellReselectionHandler()\
        Begin" );
    
    iMcnState = EFalse;
    CActiveScheduler::Add( this );
    
    NWLOGSTRING( KNWMESOUT, 
        "NW: CNWGsmMessageHandler::CNWGsmNetworkCellReselectionHandler()\
        End" );
    }
// ----------------------------------------------------------------------------
// CNWGsmNetworkCellReselectionHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CNWGsmNetworkCellReselectionHandler::ConstructL()
    {
    NWLOGSTRING( KNWMESOUT, "NW: CNWGsmMessageHandler::ConstructL() Begin" );

    // Attach to startup property
    User::LeaveIfError( 
        iProperty.Attach( KPSUidCellBroadcast, KCbsServerStarted ) );

    TInt err = iProperty.Get( iMcnState );
    if ( err != KErrNone )
        {
        iMcnState = 0;
        NWLOGSTRING2( KNWERROR,
            "NW: CNWGsmNetworkCellReselectionHandler::ConstructL(), \
            RProperty.Get FAILED: %d", err );
        }
    NWLOGSTRING( KNWMESOUT, 
       "NW: CNWGsmNetworkCellReselectionHandler::ConstructL(),\
        RProperty::Get() called" );
   
    if ( iMcnState )
        {
        // Establish the session for CBS messages.
        // No error handling, if leaves. Session creation will be try again 
        // when CBS server is ready.
        if ( !iMcn )
            {
            NWLOGSTRING( KNWOBJECT, 
                "NW: CNWGsmNetworkCellReselectionHandler::ConstructL(),\
                Mcn hasn't been created yet." );
            iMcn = CMcn::NewL();
            }
        }
    else
        {
        NotifyCbsServerStateChanged();
        }

    iViagEngine = CNWNetworkViagBaseEngine::NewL( 
                        *this,
                        iOwner,
                        iCustomAPI,
                        iNWNetworkInfo,
                        iInterNetworkInfo
                        );
             
    iMcnEngine = CNWNetworkMcnEngine::NewL(
                iOwner,
                iNWNetworkInfo,
                iCustomAPI,
                *this );
                
    // Start SatRefresh handler if CBS server is already started.
    if ( iMcn )
        {
        iViagEngine->StartupReadyL();
        }
    
    NWLOGSTRING( KNWMESOUT, "NW: CNWGsmMessageHandler::ConstructL() End " );
    }

// ----------------------------------------------------------------------------
// CNWGsmNetworkCellReselectionHandler::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CNWGsmNetworkCellReselectionHandler* CNWGsmNetworkCellReselectionHandler::NewL(
        CNWGsmMessageHandler& aOwner,
        RMobilePhone& aPhone,
        TNWInfo& aNetworkInfo,
        RMmCustomAPI& aCustomAPI,
        TNWInterInfo& aInterNetworkInfo )
    {
    NWLOGSTRING( KNWMESOUT, "NW: CNWGsmMessageHandler::NewL() Begin" );
    
    CNWGsmNetworkCellReselectionHandler* self = 
        new (ELeave) CNWGsmNetworkCellReselectionHandler ( aOwner,
                                                           aPhone,
                                                           aNetworkInfo,
                                                           aCustomAPI,
                                                           aInterNetworkInfo );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    NWLOGSTRING( KNWMESOUT, "NW: CNWGsmMessageHandler::NewL() End" );
    return self;
    }

    
// Destructor
CNWGsmNetworkCellReselectionHandler::~CNWGsmNetworkCellReselectionHandler()
    {
    NWLOGSTRING( KNWMESOUT, "NW: CNWGsmMessageHandler::\
        ~CNWGsmNetworkCellReselectionHandler() Begin " );
    
    Cancel();
    delete iMcnEngine;
    delete iViagEngine;
    if ( iMcn )
        {
        delete iMcn;
        }
  
    iProperty.Close();
    
    NWLOGSTRING( KNWMESOUT, "NW: CNWGsmMessageHandler::\
        ~CNWGsmNetworkCellReselectionHandler() End " );
    }


// ----------------------------------------------------------------------------
// CNWGsmNetworkCellReselectionHandler::HandleCellReselection
// Checks if we got Viag SIM card in ME. 
// SubscriberId is checked in the CNWNetworkCommand-class. Viag IMSI has fixed
// prefix which we compare here.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWGsmNetworkCellReselectionHandler::HandleCellReselection( )
    {
    NWLOGSTRING( KNWMESOUT, "NW: CNWGsmMessageHandler::\
        HandleCellReselection() Begin " );

    if( iViagEngine )
        {
        iViagEngine->StopTimer();
        iViagEngine->CellReselection();
        }
    iMcnEngine->CellReselection();

    NWLOGSTRING( KNWMESOUT, "NW: CNWGsmMessageHandler::\
        HandleCellReselection() End " );
    }

// ----------------------------------------------------------------------------
// CNWGsmNetworkCellReselectionHandler::GetMcnPtr
// Returns the pointer to CMcn object.
// ----------------------------------------------------------------------------
//
CMcn* CNWGsmNetworkCellReselectionHandler::GetMcnPtr()
    {
    return iMcn;
    }

// ----------------------------------------------------------------------------
// CNWGsmNetworkCellReselectionHandler::DoCancel
// Cancels the pending async. command.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWGsmNetworkCellReselectionHandler::DoCancel()
    {
    NWLOGSTRING( KNWMESOUT, "NW: CNWGsmMessageHandler::DoCancel() Begin " );
    
    iProperty.Cancel();   
    
    NWLOGSTRING( KNWMESOUT, "NW: CNWGsmMessageHandler::DoCancel() End " );
    }

// ----------------------------------------------------------------------------
// CNWGsmNetworkCellReselectionHandler::RunL
// Receives the completion of the NotifyCbsServerStateChanged command.
// ----------------------------------------------------------------------------
//
void CNWGsmNetworkCellReselectionHandler::RunL()
    {
    NWLOGSTRING( KNWREQEND, 
        "NET CNWGsmNetworkCellReselectionHandler::RunL() Begin" );
    NWLOGSTRING2( KNWREQEND, 
        "NW: CNWGsmNetworkCellReselectionHandler::RunL(), error value of\
        istatus = %d ", iStatus.Int() );
    
    if ( iStatus == KErrNone )  
        {
        NWLOGSTRING( KNWINT, 
            "NW CNWGsmNetworkCellReselectionHandler::RunL: NotifyRequest !" );
        //Establish the session for CBS messages.
        if ( !iMcn )
            {
            iMcn = CMcn::NewL();
            iViagEngine->StartupReadyL();
            iMcnState = ETrue;
            }
        }
    else
        {
        NWLOGSTRING ( KNWERROR, 
            "NW: CNWGsmNetworkCellReselectionHandler::RunL: \
            NOTIFYREQUEST FAILED!" );
        }
    
    NWLOGSTRING( KNWREQEND, 
        "NET CNWGsmNetworkCellReselectionHandler::RunL() End " );
    }


// ----------------------------------------------------------------------------
// CNWGsmNetworkCellReselectionHandler::RunError
// ----------------------------------------------------------------------------
//
TInt CNWGsmNetworkCellReselectionHandler::RunError( TInt /* aError */ )
    {
    // Just ignore any error and continue without
    // any handling to allow smooth execution. 
    NWLOGSTRING( KNWOBJECT, 
        "NET CNWGsmNetworkCellReselectionHandler::RunError() called " );
    return KErrNone;            
    }


// ----------------------------------------------------------------------------
// CNWGsmNetworkCellReselectionHandler::NotifyCbsServerStateChanged
// Calls notify request from Publish and Subscribe or SharedData
// ----------------------------------------------------------------------------
//
void CNWGsmNetworkCellReselectionHandler::NotifyCbsServerStateChanged()
    {
    NWLOGSTRING( KNWREQOUT, 
        "NET CNWGsmNetworkCellReselectionHandler::\
        NotifyCbsServerStateChanged() Begin " );
    
    iProperty.Subscribe( iStatus );
    NWLOGSTRING( KNWREQOUT,
        "NW CNWGsmNetworkCellReselectionHandler::NotifyCbsServerStateChanged()\
         > RProperty::Subscribe" );
    SetActive();

    NWLOGSTRING( KNWREQOUT, 
        "NET CNWGsmNetworkCellReselectionHandler::\
        NotifyCbsServerStateChanged() End " );
    }

// ----------------------------------------------------------------------------
// CNWGsmNetworkCellReselectionHandler::HandleUpdateReadingStatus
// Updates Operator name's read status
// ----------------------------------------------------------------------------
//
void CNWGsmNetworkCellReselectionHandler::UpdateReadingStatus( 
        const TNWRead& aElementFile, TBool aReadStatus )
    {
    NWLOGSTRING2( KNWREQOUT, 
        "NET CNWGsmNetworkCellReselectionHandler::\
        UpdateReadingStatus() Begin, aReadStatus = %d ",
        aReadStatus );
    
    iViagEngine->UpdateEFReadingState( aElementFile, aReadStatus );
    
    NWLOGSTRING( KNWREQOUT, 
        "NET CNWGsmNetworkCellReselectionHandler::\
        UpdateReadingStatus() End " );
    }

//  End of File  
