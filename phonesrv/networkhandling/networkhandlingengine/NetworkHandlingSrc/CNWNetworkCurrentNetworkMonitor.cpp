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
* Description:  This module contains the implementation of CNWNetworkCurrentNetworkMonitor
*                class member functions.
*
*/


//  INCLUDE FILES
#include    "CNWNetworkCurrentNetworkMonitor.h"
#include    "NWPanic.pan"
#include    "NWLogger.h"
#include    "cnwnetworkcurrentnetworkreceiver.h"


// ================= MEMBER FUNCTIONS =======================================

// ----------------------------------------------------------------------------
// CNWNetworkCurrentNetworkMonitor::CNWNetworkCurrentNetworkMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkCurrentNetworkMonitor::CNWNetworkCurrentNetworkMonitor(
        CNWMessageHandler& aOwner,     // Reference to class owner
        RMobilePhone& aPhone,       // Reference to mobile phone
        TNWInfo& aNetworkInfo, // Reference to network info struct
        RMmCustomAPI& aCustomAPI,       // Reference to custom API
        TNWInterInfo& aInterNetworkInfo )
        : CNWNetworkMonitorBase( aOwner,
                                 aPhone,
                                 aNetworkInfo,
                                 aCustomAPI ),
          iNetworkInfoPckg( iNetworkInfo ),//lint !e1403
          iInterNetworkInfo( aInterNetworkInfo )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkCurrentNetworkMonitor::\
        CNWNetworkCurrentNetworkMonitor() Begin." );
    
    CActiveScheduler::Add( this );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkCurrentNetworkMonitor::\
        CNWNetworkCurrentNetworkMonitor() End." );
    }

// Destructor
CNWNetworkCurrentNetworkMonitor::~CNWNetworkCurrentNetworkMonitor(
        // None.
        )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkCurrentNetworkMonitor::\
        ~CNWNetworkCurrentNetworkMonitor() Begin." );
    
    Cancel();
    delete iCurrentNetworkReceiver;
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkCurrentNetworkMonitor::\
        ~CNWNetworkCurrentNetworkMonitor() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkCurrentNetworkMonitor::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CNWNetworkCurrentNetworkMonitor* CNWNetworkCurrentNetworkMonitor::NewL( 
        CNWMessageHandler& aOwner, 
        RMobilePhone& aPhone, 
        TNWInfo& aNetworkInfo, // Reference to network info struct
        RMmCustomAPI& aCustomAPI,       // Reference to custom API
        TNWInterInfo& aInterNetworkInfo )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkCurrentNetworkMonitor::NewL() Begin." );
    
    CNWNetworkCurrentNetworkMonitor* self = 
        new( ELeave ) CNWNetworkCurrentNetworkMonitor( aOwner,
                                                           aPhone, 
                                                           aNetworkInfo,
                                                           aCustomAPI,
                                                           aInterNetworkInfo );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkCurrentNetworkMonitor::NewL() End." );
    
    return self;
    }
// ----------------------------------------------------------------------------
// CNWNetworkCurrentNetworkMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CNWNetworkCurrentNetworkMonitor::ConstructL()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkCurrentNetworkMonitor::ConstructL() Begin." );
    
    iCurrentNetworkReceiver = new ( ELeave )
       CNWNetworkCurrentNetworkReceiver( 
               *this, iPhone, iNetworkInfoPckg, iLocationArea );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkCurrentNetworkMonitor::ConstructL() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkCurrentNetworkMonitor::DoCancel
// Cancels the pending asynchronous command.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkCurrentNetworkMonitor::DoCancel()
    {
    NWLOGSTRING( KNWREQEND, 
            "NW: CNWNetworkCurrentNetworkMonitor::DoCancel() Begin: \
            Request RMobilePhone::CancelAsyncRequest( \
            EMobilePhoneNotifyCurrentNetworkChange )" );
    iPhone.CancelAsyncRequest( EMobilePhoneNotifyCurrentNetworkChange );
    
    NWLOGSTRING( KNWREQEND, 
        "NW: CNWNetworkCurrentNetworkMonitor::\
        DoCancel() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkCurrentNetworkMonitor::RunL
// Receives the completion of the pending asynchronous command,
// saves possible values from asynchronous function call and
// sends notification to CNWSession object.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkCurrentNetworkMonitor::RunL()
    {
    NWLOGSTRING2( KNWOBJECT, 
        "CNWNetworkCurrentNetworkMonitor::RunL() Begin, \
        iStatus = %d", iStatus.Int() );

    if ( iStatus == KErrNone )
        {
        iIsInitialised = ETrue;
        IssueRequest();
        UpdateNetworkInfo();
        }
        
    else
        {
        if ( !iIsInitialised && iStatus == KErrNotFound )
            {
            // Network is not ready, request notifications about changes.
            iNWNetworkInfo.iStatus = ENWStatusUnknown;
            iIsInitialised = ETrue;
            IssueRequest();
            }
        else
            {
            NWLOGSTRING2( KNWERROR,
               "NW: CNWNetworkCurrentNetworkMonitor::RunL, Unknown error \
               situation, iStatus = %d", iStatus.Int() );

            // Check if monitor can be restarted.
            if ( iStatus != KErrCancel && iStatus != KErrServerTerminated )
                {
                iIsInitialised = EFalse;  // Ensure we get current net info.
                IssueRequest();
                }
            else
                {
                NWLOGSTRING( KNWERROR,
                    "NW: CNWNetworkCurrentNetworkMonitor::RunL, KErrCancel \
                    or KErrServerTerminated" );
                }
            } 
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "CNWNetworkCurrentNetworkMonitor::RunL() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkCurrentNetworkMonitor::IssueRequest
// Initialises the current network info query to MMEtel. If the network info has
// not been queried before the GetCurrentNetwork function is called.
// Otherwise restarts observing network information changes.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkCurrentNetworkMonitor::IssueRequest()
    {
    NWLOGSTRING( KNWOBJECT, 
        "CNWNetworkCurrentNetworkMonitor::IssueRequest() Begin" );
    
    if ( !IsActive() )
        {
        NWLOGSTRING( KNWREQOUT, 
                "NW: CNWNetworkCurrentNetworkMonitor::IssueRequest > \
                RMobilePhone::NotifyCurrentNetworkChange" );
        iPhone.NotifyCurrentNetworkChange( 
                iStatus, iNetworkInfoPckg, iLocationArea );
        SetActive();
        if ( !iIsInitialised )
            {
            iCurrentNetworkReceiver->IssueRequest();
            }
        }
    else 
        {
        NWLOGSTRING( KNWERROR, 
            "NW: CNWNetworkCurrentNetworkMonitor::IssueRequest: Already active" );
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "CNWNetworkCurrentNetworkMonitor::IssueRequest() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkCurrentNetworkMonitor::UpdateNetworkInfo
// Convert MMEtel network types to client'network info. All common, Gsm and 
// WCdma network information are updated to the Client's TNWInfo struct.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkCurrentNetworkMonitor::UpdateNetworkInfo()
    {
    NWLOGSTRING( KNWOBJECT, 
        "CNWNetworkCurrentNetworkMonitor::UpdateNetworkInfo() Begin" );
    
    iNetworkInfo = iNetworkInfoPckg();
    //lint -e64
    iNWNetworkInfo.iNetworkId.Copy( iNetworkInfo.iNetworkId.Ptr(), 
        iNetworkInfo.iNetworkId.Length() );

    iNWNetworkInfo.iCountryCode.Copy( iNetworkInfo.iCountryCode.Ptr(), 
        iNetworkInfo.iCountryCode.Length() );

    iNWNetworkInfo.iDisplayTag.Copy( iNetworkInfo.iDisplayTag.Ptr(), 
        iNetworkInfo.iDisplayTag.Length() );

    iNWNetworkInfo.iShortName.Copy( iNetworkInfo.iShortName.Ptr(), 
        iNetworkInfo.iShortName.Length() );

    iNWNetworkInfo.iLongName.Copy( iNetworkInfo.iLongName.Ptr(), 
        iNetworkInfo.iLongName.Length() );

    iInterNetworkInfo.iCellId = iLocationArea.iCellId;
    iInterNetworkInfo.iLac = iLocationArea.iLocationAreaCode;
    iInterNetworkInfo.iAreaKnown = iLocationArea.iAreaKnown;
    //lint -restore

    
    NWLOGSTRING3( KNWOBJECT, 
        "CNWNetworkCurrentNetworkMonitor::UpdateNetworkInfo(),\
        iNetworkInfo.iMode = %d, iNetworkInfo.iStatus = %d",
        iNetworkInfo.iMode, iNetworkInfo.iStatus );
    
    // Match MMEtel network mode to NW values
    switch ( iNetworkInfo.iMode ) 
        {
        case RMobilePhone::ENetworkModeUnknown: 
            { 
            iNWNetworkInfo.iNetworkMode = ENWModeUnknown; 
            break;
            }
        case RMobilePhone::ENetworkModeUnregistered: 
            { 
            iNWNetworkInfo.iNetworkMode = ENWModeUnregistered; 
            break;
            }
        case RMobilePhone::ENetworkModeGsm: 
            { 
            iNWNetworkInfo.iNetworkMode = ENWModeGsm; 
            break; 
            }
        case RMobilePhone::ENetworkModeAmps:
            { 
            iNWNetworkInfo.iNetworkMode = ENWModeAmps;
            break; 
            }
        case RMobilePhone::ENetworkModeCdma95:
            { 
            iNWNetworkInfo.iNetworkMode = ENWModeCdma95;
            break; 
            }
        case RMobilePhone::ENetworkModeCdma2000: 
            { 
            iNWNetworkInfo.iNetworkMode = ENWModeCdma2000;
            break; 
            }
        case RMobilePhone::ENetworkModeWcdma:
            { 
            iNWNetworkInfo.iNetworkMode = ENWModeWcdma;
            break; 
            }
        case RMobilePhone::ENetworkModeTdcdma:
            { 
            iNWNetworkInfo.iNetworkMode = ENWModeTdcdma;
            break; 
            }
        default: 
            { 
            iNWNetworkInfo.iNetworkMode = ENWModeUnknown;
            __ASSERT_DEBUG( !iNetworkInfo.iMode, Panic( ENWPanicInvalidMode ) );
            }
        }//Lint !e788
    // Match MMEtel network status to NW values
    switch ( iNetworkInfo.iStatus ) 
        {
        case RMobilePhone::ENetworkStatusAvailable:
            {
            iNWNetworkInfo.iStatus = ENWStatusAvailable;
            break; 
            }
        case RMobilePhone::ENetworkStatusCurrent:
            {
            iNWNetworkInfo.iStatus = ENWStatusCurrent;
            break; 
            }
        case RMobilePhone::ENetworkStatusForbidden:
            {
            iNWNetworkInfo.iStatus = ENWStatusForbidden;
            break; 
            }
        case RMobilePhone::ENetworkStatusUnknown:
            {
            iNWNetworkInfo.iStatus = ENWStatusUnknown;
            break; 
            }
        default: 
            { 
            iNWNetworkInfo.iStatus = ENWStatusUnknown;
            __ASSERT_DEBUG( iNetworkInfo.iStatus, Panic( ENWPanicInvalidStatus ) );
            }
        }// switch
    iOwner.SendMessage( MNWMessageObserver::ENWMessageNetworkInfoChange );
    
    NWLOGSTRING( KNWOBJECT, 
        "CNWNetworkCurrentNetworkMonitor::UpdateNetworkInfo() End" );
    }

//  End of File
