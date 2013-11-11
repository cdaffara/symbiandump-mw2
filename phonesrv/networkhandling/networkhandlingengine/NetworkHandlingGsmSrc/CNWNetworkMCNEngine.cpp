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
* Description:  This module contains the implementation of CNWNetworkMcnEngine
*                class member functions.
*
*/

// INCLUDE FILES
#include "CNWGsmSessionImplementation.h"
#include "CNWGsmMessageHandler.h"
#include "CNWNetworkMCNEngine.h"
#include "CNWNetworkMcnTimer.h"
#include "NWPanic.pan"
#include "NWLogger.h"
#include <cmcn.h>
#include <cmcntopicarray.h>
#include <centralrepository.h>
#include <settingsinternalcrkeys.h>

// CONSTANTS
const TUint KNWzero = 0;
const TInt KMCNOn = 1;
const TInt KMCNOff = 0;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWNetworkMcnEngine::CNWNetworkMcnEngine
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkMcnEngine::CNWNetworkMcnEngine(
        CNWGsmMessageHandler& aMessageHandler,
        TNWInfo& aNetworkInfo, // ref. to network info struct
        RMmCustomAPI& aCustomAPI,       //ref. to customAPI
        CNWGsmNetworkCellReselectionHandler& aCellReselectionHandler )
        : CActive( EPriorityStandard ),
          iGsmMessageHandler( aMessageHandler ),
          iNWNetworkInfo( aNetworkInfo ),
          iCustomAPI( aCustomAPI ),
          iCellReselectionHandler( aCellReselectionHandler )
    {
    NWLOGSTRING( KNWINT, 
        "NW:CNWNetworkMcnEngine::CNWNetworkMcnEngine() Begin" );
    
    iComparedCellId = KNWzero;  //new cellId that will be used to comparization
    iListenMCNDisplay = EFalse; //Boolean indicating if to listen MCN Display area
    
    CActiveScheduler::Add( this );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkMcnEngine::CNWNetworkMcnEngine() End");
    }

// ----------------------------------------------------------------------------
// CNWNetworkMcnEngine::NewL
// Symbian two phased constructor.
// ----------------------------------------------------------------------------
//
CNWNetworkMcnEngine* CNWNetworkMcnEngine::NewL(
        CNWGsmMessageHandler& aMessageHandler,     
        TNWInfo& aNetworkInfo, // ref. to network info struct
        RMmCustomAPI& aCustomAPI,       // ref. to customAPI
        CNWGsmNetworkCellReselectionHandler& aCellReselectionHandler )
    {
    NWLOGSTRING( KNWINT, 
        "NW:CNWNetworkMcnEngine::NewL() Begin" );
    
    CNWNetworkMcnEngine* self = new (ELeave) CNWNetworkMcnEngine(
                                aMessageHandler,
                                aNetworkInfo,
                                aCustomAPI,
                                aCellReselectionHandler );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    NWLOGSTRING( KNWINT, 
        "NW:CNWNetworkMcnEngine::NewL() Begin" );
    
    return self;
    }

// ----------------------------------------------------------------------------
// CNWNetworkMcnEngine::ConstructL
// Symbian default constructor can leave.
// ----------------------------------------------------------------------------
//
void CNWNetworkMcnEngine::ConstructL()
    {
    NWLOGSTRING( KNWINT, 
        "NW:CNWNetworkMcnEngine::ConstructL() Begin" );
    
    iTimer = CNWNetworkMcnTimer::NewL();
    
    // Connecting and initialization
    iRepository = CRepository::NewL( KCRUidNetworkSettings );
    
    // Get the value from Central Repository
    iRepository->Get( KSettingsMcnDisplay, iListenMCNDisplay );
    NWLOGSTRING( KNWMESOUT, 
        "NW: CNWNetworkMCNEngine::ConstructL: CRepository->Get() called" );
    NotifyMcnDisplayChanged();
    // Reset initial values
    ResetIndicatorValues();
    
    NWLOGSTRING( KNWINT, 
        "NW:CNWNetworkMcnEngine::ConstructL() Begin" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkMcnEngine::~CNWNetworkMcnEngine
// Destructor
// ----------------------------------------------------------------------------
//
CNWNetworkMcnEngine::~CNWNetworkMcnEngine()
    {
    NWLOGSTRING( KNWINT, 
        "NW:CNWNetworkMcnEngine::~CNWNetworkMcnEngine() Begin" );
    
    Cancel();
    
    if( iMcn )
        {
        if( iCBSState == EListeningMCN )
            {
            iMcn->Unregister( this );
            NWLOGSTRING( KNWMESOUT, 
                "NW: CNWNetworkMCNEngine::~CNWNetworkMcnEngine: \
                iMcn->Unregister() called" );
            }
        iMcn = NULL; // CNWGsmNetworkCellReselectionHandler has ownership of iMcn.
        }

    delete iRepository;    
    delete iTimer;
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkMcnEngine::~CNWNetworkMcnEngine() End");
    }

// ----------------------------------------------------------------------------
// CNWNetworkMcnEngine::CellInfoChanged
// Gets called when a message of a subscribed topic is received from the 
// network.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CNWNetworkMcnEngine::CellInfoChanged(
    const TCbsMcnMessage& aMcnMessage ) //MCN message received from the network
    {
    NWLOGSTRING3( KNWOBJECT, 
        "NW: CNWNetworkMcnEngine::CellInfoChanged() Begin,\
        aMcnMessage.iTopicNumber = %d, aMcnMessage.iNetworkMode = %d ",
        aMcnMessage.iTopicNumber, aMcnMessage.iNetworkMode );
    
    TInt errorCode = HandleCBSMessages( aMcnMessage );
    if ( errorCode == KErrNone )
        {
        iGsmMessageHandler.SendMessage( 
            MNWMessageObserver::ENWMessageCurrentCellInfoMessage );
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkMcnEngine::CellInfoChanged() End");
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CNWNetworkMcnEngine::SharedDataCallBack
// This is a static callback function which is called when a Shared Data's 
// notify event completes.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
// 
TInt CNWNetworkMcnEngine::SharedDataCallBack(
        TAny* aAny )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkMcnEngine::SharedDataCallBack() Begin");
    
    CNWNetworkMcnEngine* self = static_cast<CNWNetworkMcnEngine*>( aAny );
    self->GetMcnDisplayValue();
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkMcnEngine::SharedDataCallBack() End");
    return KErrNone;                                     
    }

// ----------------------------------------------------------------------------
// CNWNetworkMcnEngine::GetMcnDisplayValue
// Description: This function reads MCN display value from Shared Data or from 
// Central Repository and decides if to listen for MCN display info changes. 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
// 
void CNWNetworkMcnEngine::GetMcnDisplayValue()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkMcnEngine::GetMcnDisplayValue() Begin");
    
    TInt uid( 0 );
    // Get the value from Central Repository
    iRepository->Get( KSettingsMcnDisplay, uid );
    NWLOGSTRING( KNWMESOUT, 
        "NW: CNWNetworkMCNEngine::GetMcnDisplayValue: \
        CRepository->Get() called" );
  
    //when cell info display value changes to
    if( uid == KMCNOn )
        {
        NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkMCNEngine::GetMcnDisplayValue: MCN changed to ON" );
        iListenMCNDisplay = ETrue;
        StartIndicatorHandling();
        }
    else if( uid == KMCNOff )
        {
        NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkMCNEngine::GetMcnDisplayValue: MCN changed to OFF");
        // cancel timer and unregister
        iTimer->Cancel();
        if( iMcn )
            {
            iMcn->Unregister( this );
            NWLOGSTRING( KNWMESOUT, 
                "NW: CNWNetworkMCNEngine::GetMcnDisplayValue: \
                iMcn->Unregister() called");
            }
        iState = EIdle;
        iCBSState = EListeningNone;
        iListenMCNDisplay = EFalse;
        
        ResetIndicatorValues();

        NWLOGSTRING( KNWINT, "NW: CNWNetworkMCNEngine::MCN Displaying");
        iGsmMessageHandler.SendMessage( 
            MNWMessageObserver::ENWMessageCurrentCellInfoMessage );
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkMcnEngine::GetMcnDisplayValue() End");
    }

// ----------------------------------------------------------------------------
// CNWNetworkMcnEngine::CellReselection
// Is called by CNWNetworkCurrentNetworkMonitor in case of cell
// re-selection.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkMcnEngine::CellReselection()
    {
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkMCNEngine::CellReselection() Begin" );
    
    StartIndicatorHandling();
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkMcnEngine::CellReselection() End");
    }

// ----------------------------------------------------------------------------
// CNWNetworkMcnEngine::RegisterToCBS
// Registers to listening CBS messages.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkMcnEngine::RegisterToCBSL(
    TUint16 aTopic )
    {
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkMCNEngine::RegisterToCBSL() Begin" );
    
    // Specify topics with CMcnTopicArray.
    CMcnTopicArray* topics = CMcnTopicArray::NewL();
    CleanupStack::PushL( topics );
    topics->AddCbTopicL( aTopic );      // MCN Info (cell info)

    iMcn = iCellReselectionHandler.GetMcnPtr();

    if( iMcn )
        {
        NWLOGSTRING( KNWMESOUT, 
            "NW: CNWNetworkMCNEngine::RegisterToCBS: \
            iMcn->RegisterL() called" );
        // Register. Parameter observer is an instance of CCbsMcnObserver.
        iMcn->RegisterL( this, *topics );
        NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkMCNEngine::RegisterToCBS: \
            Registered to MCN messages" );
        }
    else
        {
        NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkMcnEngine::RegisterToCBSL: iMcn = NULL" );
        }

    CleanupStack::PopAndDestroy( topics );
    
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkMCNEngine::RegisterToCBSL() End " );
    }   
// ----------------------------------------------------------------------------
// CNWNetworkMcnEngine::StartIndicatorHandling
// Handles MCN display.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkMcnEngine::StartIndicatorHandling()
    {
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkMCNEngine::StartIndicatorHandling() Begin" );
    
    if( iListenMCNDisplay )
        {
        NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkMCNEngine::StartIndicatorHandling: MCN ON" );
        if( iCBSState != EListeningMCN )
            {
            TRAPD( err, RegisterToCBSL( KNWMcnMsgId ) );
            if( err != KErrNone )
                {
                NWLOGSTRING( KNWERROR, 
                    "NW: CNWNetworkMcnEngine::StartIndicatorHandling: \
                    Couldn't register to CBS messages ! ");
                ResetIndicatorValues();
                iGsmMessageHandler.SendMessage( 
                    MNWMessageObserver::ENWMessageCurrentCellInfoMessage );
                }
            else
                {
                if ( iMcn )
                    {
                    iCBSState = EListeningMCN;
                    }
                }
            }

        //Put timer on and wait for max 5 seconds if there is
        //MCN display active, else wait 120secs.
        //Set the timer to 120secs is used to wait the message arrival.
        //If past 120secs, it may be a network problem.
        //So no need to wait any more, cancel the timer to save resource
        if( iNWNetworkInfo.iMCNIndicatorType == ENWMCNIndicatorTypeActive )
            {
            iTimer->Cancel();
            iTimer->After( KWaitTimeToClearOldMcnIndicator, this );
            iState=ECbsWaitingToClearMcnIndicator;
            NWLOGSTRING( KNWINT, 
                "NW: CNWNetworkMCNEngine::StartIndicatorHandling: \
                Started 5s timer" );
            }
        else
            {
            iTimer->Cancel();
            iTimer->After( KWaitTimeForNewMcn, this );
            iState=ECbsWaitingForNewMcn;
            NWLOGSTRING( KNWINT, 
                "NW: CNWNetworkMCNEngine::StartIndicatorHandling: Started\
                 120s timer" );
            }
        }
    else if( iNWNetworkInfo.iMCNIndicatorType == ENWMCNIndicatorTypeActive )
        {
        NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkMCNEngine::StartIndicatorHandling: MCN Displaying");
        ResetIndicatorValues();
        iGsmMessageHandler.SendMessage( 
            MNWMessageObserver::ENWMessageCurrentCellInfoMessage );
        }
    
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkMCNEngine::StartIndicatorHandling() Begin" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkMcnEngine::HandleCBSMessages
// Handles CBS messages.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CNWNetworkMcnEngine::HandleCBSMessages(
        const TCbsMcnMessage& aMcnMessage )//MCN message received from the network.
    {
    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkMCNEngine::HandleCBSMessages() Begin,\
        aMcnMessage.iTopicNumber = %d ", aMcnMessage.iTopicNumber );
    
    //Retrieve CBS message data
    TMcnMessage mcnInfo = aMcnMessage.iBuffer;
    TInt length = mcnInfo.Length();
    TInt err=KErrNone;

    //MCN Display message, check that we really are listening it
    if( aMcnMessage.iTopicNumber == KNWMcnMsgId && iCBSState == EListeningMCN )
        {
        if ( length > KCbsMcnMessageMaxLength )
            {
            NWLOGSTRING( KNWINT, 
                "NW: CNWNetworkMCNEngine::HandleCBSMessages() End,\
                Incorrect message" );
            //incorrect message. Nothing to do
            return KErrArgument;
            }
        
        NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkMCNEngine::HandleCBSMessages: \
            MCN message arrived" );
        // cancel timer and unregister
        iTimer->Cancel();
        iState = EIdle;
        if( iMcn )
            {
            iMcn->Unregister( this );
            NWLOGSTRING( KNWMESOUT, 
                "NW: CNWNetworkMCNEngine::HandleCBSMessages: \
                iMcn.Unregister() called" );
            }
        iCBSState = EListeningNone;

        if ( length > KNWMCNNameLength )
            {
            iNWNetworkInfo.iMCNName.Copy( mcnInfo.Left( KNWMCNNameLength ) );
            }
        else
            {
            iNWNetworkInfo.iMCNName.Copy( mcnInfo );
            }
        
        iNWNetworkInfo.iMCNIndicatorType = ENWMCNIndicatorTypeActive;
        NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkMCNEngine::HandleCBSMessages: MCN display updated");
        }
   
    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkMCNEngine::HandleCBSMessages() End,\
        err = %d ", err );
    return err;
    }

// ----------------------------------------------------------------------------
// CNWNetworkMcnEngine::HandleTimeOutL
// Handles timeout.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkMcnEngine::HandleTimeOut()
    {
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkMCNEngine::HandleTimeOut() Begin" );
    
    switch ( iState )
        {
        case ECbsWaitingToClearMcnIndicator:
            {
            //Sends clear indicator to UI and then restarts
            //timer for 115 more seconds
            ResetIndicatorValues();
            iGsmMessageHandler.SendMessage( 
                MNWMessageObserver::ENWMessageCurrentCellInfoMessage );
            iTimer->Cancel();
            iTimer->After( KWaitTimeForNewMcnAfterClearingOld, this );
            iState=ECbsWaitingForNewMcn;
            NWLOGSTRING( KNWINT, 
                "NW: CNWNetworkMCNEngine::HandleTimeOutL: 115s timer started" );
            }
            break;
        case ECbsWaitingForNewMcn:
            {
            NWLOGSTRING( KNWINT, 
                "NW: CNWNetworkMCNEngine::HandleTimeOutL: \
                115/120s timer expired" );
            if( iMcn )
                {
                //Unregisters when 120/115sec timer expires
                iMcn->Unregister( this );
                NWLOGSTRING( KNWMESOUT, 
                    "NW: CNWNetworkMCNEngine::HandleTimeOutL: \
                    iMcn.Unregister() called" );
                }
            ResetIndicatorValues();
            iCBSState=EListeningNone;
            iState=EIdle;
            }
            break;
        case EIdle:
            break;
        default:
            __ASSERT_DEBUG( !iState, Panic( ENWPanicInvalidState ) );
            break;
        }
    
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkMCNEngine::HandleTimeOut() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkMcnEngine::ResetIndicatorValues
// Resets Viag's and MCN display's TPEInfo values.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkMcnEngine::ResetIndicatorValues()
    {
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkMCNEngine::ResetIndicatorValues() Begin" );
    
    iNWNetworkInfo.iMCNIndicatorType = ENWMCNIndicatorTypeNone;
    iNWNetworkInfo.iMCNName.Zero();
    
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkMCNEngine::ResetIndicatorValues() End" );
    }
    
  // --------------------------------------------------------------------------
// CNWNetworkMcnEngine::DoCancel
// Cancels the pending async. command.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkMcnEngine::DoCancel()
    {
    NWLOGSTRING( KNWINT, "NW: CNWNetworkMcnEngine::DoCancel() Begin" );
    // Cancel all the asynchronous service requests
    iRepository->NotifyCancel( KSettingsMcnDisplay );
    
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkMCNEngine::DoCancel() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkMcnEngine::RunL
// Receives the completion of the pending async. command
// ----------------------------------------------------------------------------
//
void CNWNetworkMcnEngine::RunL()
    {
    NWLOGSTRING2( KNWREQEND, 
        "NET CNWNetworkMcnEngine::RunL() Begin,\
        iStatus.Int() =  %I ", iStatus.Int() );
    
    if ( iStatus == NCentralRepositoryConstants::KUnspecifiedKey || 
         iStatus == KSettingsMcnDisplay )  
        {
        NWLOGSTRING( KNWINT, "NW CNWNetworkMcnEngine::RunL: NotifyRequest !" );
        // Get the current value from Central Repository
        GetMcnDisplayValue();
        NotifyMcnDisplayChanged();   
        }
    else
        {
        NWLOGSTRING ( KNWERROR, 
            "NW: CNWNetworkMcnEngine::RunL: NOTIFYREQUEST FAILED!" );
        }
    
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkMCNEngine::RunL() End" );
    }  

// ----------------------------------------------------------------------------
// CNWNetworkMcnEngine::NotifyMcnDisplayChanged
// Calls NotifyRequest from Central Repository 
// ----------------------------------------------------------------------------
//
void CNWNetworkMcnEngine::NotifyMcnDisplayChanged()
    {
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkMCNEngine::NotifyMcnDisplayChanged() Begin " );
    
    iRepository->NotifyRequest( KSettingsMcnDisplay, iStatus );
    NWLOGSTRING( KNWREQOUT,
        "NW CNWNetworkMcnEngine::NotifyMcnDisplayChanged: \
        CRepository::NotifyRequest" );
    SetActive();
    
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkMCNEngine::NotifyMcnDisplayChanged() End " );
    }


//  End of File 
