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
* Description:  This module contains the implementation of 
                 CNWNetworkSelectionSettingMonitor class 
*                member functions.
*
*/


//  INCLUDE FILES
#include    "CNWMessageHandler.h"
#include    "cnwnetworkselectionsettingmonitor.h"
#include    <nwhandlingengine.h>
#include    "NWPanic.pan"
#include    "NWLogger.h"



// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CNWNetworkSelectionSettingMonitor::CNWNetworkSelectionSettingMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkSelectionSettingMonitor::CNWNetworkSelectionSettingMonitor
        ( 
        CNWMessageHandler& aOwner, // ref. to class owner
        RMobilePhone& aPhone,   // ref. to mobile phone
        TNWInfo& aNetworkInfo, // ref. to network info struct 
        RMmCustomAPI& aCustomAPI )    //ref. to customAPI
        : CNWNetworkMonitorBase( aOwner,
                                 aPhone,
                                 aNetworkInfo,
                                 aCustomAPI ),
           iNetworkSelectionPckg( iNetworkSelection )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSelectionSettingMonitor::\
        CNWNetworkSelectionSettingMonitor() Begin" );
    
    CActiveScheduler::Add( this );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSelectionSettingMonitor::\
        CNWNetworkSelectionSettingMonitor() End" );
    }


// ----------------------------------------------------------------------------
// CNWNetworkSelectionSettingMonitor::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CNWNetworkSelectionSettingMonitor* 
            CNWNetworkSelectionSettingMonitor::NewL( 
        CNWMessageHandler& aOwner, 
        RMobilePhone& aPhone, 
        TNWInfo& aNetworkInfo, 
        RMmCustomAPI& aCustomAPI )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSelectionSettingMonitor::NewL() Begin" );
    
    CNWNetworkSelectionSettingMonitor* self = 
        new( ELeave ) CNWNetworkSelectionSettingMonitor( aOwner,
                                             aPhone, 
                                             aNetworkInfo,
                                             aCustomAPI );

    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSelectionSettingMonitor::NewL() End" );
    
    return self;
    }

// Destructor
CNWNetworkSelectionSettingMonitor::~CNWNetworkSelectionSettingMonitor()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSelectionSettingMonitor::\
        ~CNWNetworkSelectionSettingMonitor() Begin" );
    
    Cancel();
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSelectionSettingMonitor::\
        ~CNWNetworkSelectionSettingMonitor() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkSelectionSettingMonitor::DoCancel
// Cancels the pending async. command. 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkSelectionSettingMonitor::DoCancel()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSelectionSettingMonitor::DoCancel() Begin" );
    
    iPhone.CancelAsyncRequest( 
           EMobilePhoneNotifyNetworkSelectionSettingChange );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSelectionSettingMonitor::DoCancel() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkSelectionSettingMonitor::RunL
// Receives the completion of the pending async. command, saves possible values 
// from async. function call and sends notification to CNWSession object.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkSelectionSettingMonitor::RunL()
    {
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkSelectionSettingMonitor::RunL() Begin,\
        iStatus.Int() = %d", iStatus.Int() );

    if ( iStatus != KErrNone )
        {
        NWLOGSTRING2( KNWERROR,
            "NW: Net.Sel.Set.Monitor.RunL, Unknown error, iStatus = %d",
               iStatus.Int() );
        iIsInitialised = EFalse;  // Ensure we get current net info.
        }
    else
        {
        UpdateNetworkSelectionSetting();
        iIsInitialised = ETrue;
        }
    // Check if monitor can be restarted.
    if ( iStatus != KErrCancel && iStatus != KErrServerTerminated )
        {
        IssueRequest();
        }

    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSelectionSettingMonitor::RunL() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkSelectionSettingMonitor::IssueRequest
// Executes the async. NotifyNetworkSelectionSettingChange function.
// If the network selection setting monitor is not yet initialised, 
// gets the initial Setting with sync. GetNetworkSelectionSetting function. 
// Then starts monitoring the setting changes.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
void CNWNetworkSelectionSettingMonitor::IssueRequest()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSelectionSettingMonitor::IssueRequest() Begin" );
    
    if ( !IsActive() )
        {
         // Start monitoring mode changes.
        NWLOGSTRING( KNWREQOUT, "NW: Net.Sel.Set.Monitor.IssueRequest ->" );
        iPhone.NotifyNetworkSelectionSettingChange( 
             iStatus, iNetworkSelectionPckg );
        SetActive();
        
        // Get initial value for the mode, if not initialised yet.
        if ( !iIsInitialised )
            {
            iIsInitialised = ETrue;
            TInt error = iPhone.GetNetworkSelectionSetting( 
                                          iNetworkSelectionPckg );
            if ( error == KErrNone ) 
                {
                UpdateNetworkSelectionSetting();
                NWLOGSTRING( KNWREQOUT, 
                    "NW: Net.Sel.Set.Monitor.IssueRequest <-" ); 
                }
            else
                {
                NWLOGSTRING2( KNWERROR, 
                    "NW: Net.Sel.Set.Monitor.IssueRequest,Error = %d", error );
                NWLOGSTRING( KNWREQOUT, 
                    "NW: Net.Sel.Set.Monitor.IssueRequest <-" );
                }
            }
        }
    else 
        {
        NWLOGSTRING( KNWERROR, 
         "NW: Net.Sel.Set.Monitor.IssueRequest Already active <-" );
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSelectionSettingMonitor::IssueRequest() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkSelectionSettingMonitor::UpdateNetworkSelectionSetting
// Update network selection setting
// ----------------------------------------------------------------------------
void CNWNetworkSelectionSettingMonitor::UpdateNetworkSelectionSetting()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSelectionSettingMonitor::\
        UpdateNetworkSelectionSetting() Begin" );
    
    iNetworkSelection = iNetworkSelectionPckg();
    NWLOGSTRING2( KNWERROR, 
        "NW: Net.Sel.Set.Monitor setting = %d", iNetworkSelection.iMethod );
    switch ( iNetworkSelection.iMethod ) 
        {
        case RMobilePhone::ENetworkSelectionUnknown:
            {
            iNWNetworkInfo.iSelectionSetting = ENWNetworkSelectionUnknown;
            break;
            }
        case RMobilePhone::ENetworkSelectionAutomatic:
            { 
            iNWNetworkInfo.iSelectionSetting = ENWNetworkSelectionAutomatic;
            break; 
            }
        case RMobilePhone::ENetworkSelectionManual:
            { 
            iNWNetworkInfo.iSelectionSetting = ENWNetworkSelectionManual;
            break;
            }
        default:
            {
            iNWNetworkInfo.iSelectionSetting = ENWNetworkSelectionUnknown;
            break;
            }
        }
    NWLOGSTRING( KNWERROR, 
        "NW: Net.Sel.Set.Monitor send ENWMessageNetworkSelectionSettingChange" );   
    iOwner.SendMessage ( 
        MNWMessageObserver::ENWMessageNetworkSelectionSettingChange );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkSelectionSettingMonitor::\
        UpdateNetworkSelectionSetting() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkSelectionSettingMonitor::RunError
// ----------------------------------------------------------------------------
//
TInt CNWNetworkSelectionSettingMonitor::RunError( TInt /*aError*/ )
    {
    NWLOGSTRING( KNWERROR, 
        "NW: CNWNetworkSelectionSettingMonitor::RunError() Called" );
   
    return KErrNone;
    }


//  End of File 
