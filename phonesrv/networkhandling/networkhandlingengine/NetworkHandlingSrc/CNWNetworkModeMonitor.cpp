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
* Description:  This module contains the implementation of CNWNetworkModeMonitor class 
*                member functions.
*
*/


//  INCLUDE FILES
#include    "CNWMessageHandler.h"
#include    "CNWNetworkModeMonitor.h"
#include    <nwhandlingengine.h>
#include    "NWPanic.pan"
#include    "NWLogger.h"



// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CNWNetworkModeMonitor::CNWNetworkModeMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkModeMonitor::CNWNetworkModeMonitor
        ( 
        CNWMessageHandler& aOwner, // ref. to class owner
        RMobilePhone& aPhone,   // ref. to mobile phone
        TNWInfo& aNetworkInfo, // ref. to network info struct 
        RMmCustomAPI& aCustomAPI )    //ref. to customAPI
        : CNWNetworkMonitorBase( aOwner,
                                 aPhone,
                                 aNetworkInfo,
                                 aCustomAPI )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkModeMonitor::CNWNetworkModeMonitor() Begin." );
    
    CActiveScheduler::Add( this );
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkModeMonitor::CNWNetworkModeMonitor() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkModeMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CNWNetworkModeMonitor::ConstructL()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkModeMonitor::ConstructL() Begin." );
    
    RProperty::TType type( RProperty::EInt );
    TSecurityPolicy readPolicy( ECapability_None );
    TSecurityPolicy writePolicy( ECapabilityWriteDeviceData );
    TInt error = RProperty::Define( KPSUidNetworkInfo, 
                                    KNWTelephonyNetworkMode, 
                                    type, 
                                    readPolicy, 
                                    writePolicy );
                                    
    //An error of KErrAlready exists should be ignored, as this only
    //indicates that some other code int system is also interested in the 
    //value and has created the property.
    if ( (error != KErrNone) && (error != KErrAlreadyExists) )
        {
        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkModeMonitor::CNWNetworkModeMonitor(), error\
            code = %d.", error );
        User::Leave(error);
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkModeMonitor::ConstructL() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkModeMonitor::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CNWNetworkModeMonitor* CNWNetworkModeMonitor::NewL( 
        CNWMessageHandler& aOwner, 
        RMobilePhone& aPhone, 
        TNWInfo& aNetworkInfo, 
        RMmCustomAPI& aCustomAPI )
    {
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkModeMonitor::NewL() Begin." );
    CNWNetworkModeMonitor* self = 
        new( ELeave ) CNWNetworkModeMonitor( aOwner,
                                             aPhone, 
                                             aNetworkInfo,
                                             aCustomAPI );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkModeMonitor::NewL() End." );
    return self;
    }

// Destructor
CNWNetworkModeMonitor::~CNWNetworkModeMonitor()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkModeMonitor::~CNWNetworkModeMonitor start." );
    
    Cancel();
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkModeMonitor::~CNWNetworkModeMonitor complete." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkModeMonitor::DoCancel
// Cancels the pending async. command. 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkModeMonitor::DoCancel()
    {
    NWLOGSTRING( KNWREQEND, 
        "NW: CNWNetworkModeMonitor::DoCancel: \
        Request RMobilePhone::CancelAsyncRequest(\
        EMobilePhoneNotifyModeChange)" );
    
    iPhone.CancelAsyncRequest( EMobilePhoneNotifyModeChange );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkModeMonitor::DoCancel() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkModeMonitor::RunL
// Receives the completion of the pending async. command, saves possible 
// values from async. function call and sends notification to CNWSession object.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkModeMonitor::RunL()
    {
    NWLOGSTRING2( KNWREQEND, 
        "CNWNetworkModeMonitor::RunL() Begin, iStatus = %d", iStatus.Int());

    if ( iStatus != KErrNone )
        {
        NWLOGSTRING2( KNWERROR,
            "NW: CNWNetworkModeMonitor::RunL, Unknown error situation, \
            iStatus = %d", iStatus.Int() );
        iIsInitialised = EFalse;  // Ensure we get current net info.
        }
    else
        {
        UpdateNetworkInfo();
        UpdateNetworkMode();
        iOwner.SendMessage ( MNWMessageObserver::ENWMessageNetworkModeChange );
        iOwner.SendMessage( MNWMessageObserver::ENWMessageNetworkInfoChange );
        iIsInitialised = ETrue;
        }
    // Check if monitor can be restarted.
    if ( iStatus != KErrCancel && iStatus != KErrServerTerminated )
        {
        IssueRequest();
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkModeMonitor::RunL() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkModeMonitor::UpdateNetworkInfo
// Updates MMEtel network mode value to NW value and assigns the value to
// the TNWInfo structure.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkModeMonitor::UpdateNetworkInfo()
    {
    NWLOGSTRING2( KNWREQEND, 
        "CNWNetworkModeMonitor::UpdateNetworkInfo() Begin, iNetworkMode = %d",
        iNetworkMode );
    
    switch ( iNetworkMode ) 
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
            __ASSERT_DEBUG( !iNetworkMode, Panic( ENWPanicInvalidMode ) );
            }
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkModeMonitor::UpdateNetworkInfo() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkModeMonitor::IssueRequest
// Executes the async. NotifyModeChange function.
// If the network mode monitor is not yet initialised, gets the initial
// mode with sync. GetCurrentModen function. Then starts monitoring
// the mode changes.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
void CNWNetworkModeMonitor::IssueRequest()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkModeMonitor::IssueRequest() Begin." );
    
    if ( !IsActive() )
        {
         // Start monitoring mode changes.
        NWLOGSTRING( KNWREQOUT, 
            "NW: CNWNetworkModeMonitor::IssueRequest: Request RMobilePhone::\
            NotifyModeChange" );
        iPhone.NotifyModeChange( iStatus, iNetworkMode );
        SetActive();
        
        // Get initial value for the mode, if not initialised yet.
        if ( !iIsInitialised )
            {
            iIsInitialised = ETrue;
            TInt error = iPhone.GetCurrentMode( iNetworkMode );
            if ( error ) 
                {
                NWLOGSTRING2( KNWERROR, 
                    "NW: CNWNetworkModeMonitor::IssueRequest > RMobilePhone::\
                    GetCurrentMode, ErrorCode = %d", error );
                return;
                }
            else
                {
                UpdateNetworkMode();
                NWLOGSTRING2( KNWREQEND, "CNWNetworkModeMonitor::IssueRequest(), iNetworkMode = %d", iNetworkMode );
                }
            }
        }
    else 
        {
        NWLOGSTRING( KNWERROR, 
            "NW: CNWNetworkModeMonitor::IssueRequest: Already active" );
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkModeMonitor::IssueRequest() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkModeMonitor::UpdateNetworkModeL
// Update networkMode PS key
// ----------------------------------------------------------------------------
void CNWNetworkModeMonitor::UpdateNetworkMode()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkModeMonitor::UpdateNetworkMode() Begin." );
    
    TInt networkMode( ENWNetworkModeUnknown );

    switch ( iNetworkMode ) 
        {
        case RMobilePhone::ENetworkModeUnknown:
        case RMobilePhone::ENetworkModeUnregistered:
            {
            networkMode = ENWNetworkModeUnknown;
            break;
            }
        case RMobilePhone::ENetworkModeGsm:
            { 
            networkMode = ENWNetworkModeGsm;
            break; 
            }
        case RMobilePhone::ENetworkModeAmps:
        case RMobilePhone::ENetworkModeCdma95:
        case RMobilePhone::ENetworkModeCdma2000:
            { 
            networkMode = ENWNetworkModeCdma;
            break;
            }
        case RMobilePhone::ENetworkModeWcdma:
        case RMobilePhone::ENetworkModeTdcdma:
            { 
            networkMode = ENWNetworkModeWcdma;
            break;
            }
        default:
            {
            networkMode = ENWNetworkModeUnknown;
            __ASSERT_DEBUG( !iNetworkMode, Panic( ENWPanicInvalidMode ) );
            }
        }

    TInt err = RProperty::Set( 
            KPSUidNetworkInfo, KNWTelephonyNetworkMode, networkMode );

    if ( err != KErrNone )
        {
        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkModeMonitor::UpdateNetworkMode(), error code = \
            %d.", err );
        
        RProperty::Delete( KPSUidNetworkInfo, KNWTelephonyNetworkMode );
        } 
    else
        {
        NWLOGSTRING( KNWREQIN, 
            "NW: CNWNetworkModeMonitor::RunL: RProperty::Set() called" );
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkModeMonitor::UpdateNetworkMode() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkModeMonitor::RunError
// ----------------------------------------------------------------------------
//
TInt CNWNetworkModeMonitor::RunError( TInt /*aError*/ )
    {
    NWLOGSTRING( KNWERROR, "NW:CNWNetworkModeMonitor::RunL() Called\
        : rproperty::set error" );
   
    return KErrNone;
    }

//  End of File 
