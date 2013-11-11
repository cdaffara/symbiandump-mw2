/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CNWNetworkRegistrationStatusMonitor 
*                class member functions.
*
*/


//  INCLUDE FILES
#include    <featmgr.h>
#include    "CNWNetworkRegistrationStatusMonitor.h"
#include    "CNWMessageHandler.h"
#include    <networkhandlingdomainpskeys.h>
#include    "NWPanic.pan"
#include    "NWLogger.h"
#include    "CNWNetworkRegistrationStatusReceiver.h"

// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CNWNetworkRegistrationStatusMonitor::CNWNetworkRegistrationStatusMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkRegistrationStatusMonitor::CNWNetworkRegistrationStatusMonitor( 
        CNWMessageHandler& aOwner, // ref. to owner class
        RMobilePhone& aPhone,  // ref. to mobile phone
        TNWInfo& aNetworkInfo, // ref. to network info struct 
        RMmCustomAPI& aCustomAPI )    //ref. to customAPI
         : CNWNetworkMonitorBase( aOwner,
                                  aPhone,
                                  aNetworkInfo,
                                  aCustomAPI )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkRegistrationStatusMonitor::\
        CNWNetworkRegistrationStatusMonitor() Begin." );
    
    CActiveScheduler::Add( this );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkRegistrationStatusMonitor::\
        CNWNetworkRegistrationStatusMonitor() End." );
    }
    
    
// ----------------------------------------------------------------------------
// CNWNetworkRegistrationStatusMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CNWNetworkRegistrationStatusMonitor::ConstructL()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkRegistrationStatusMonitor::\
        ConstructL() Begin." );
    
    FeatureManager::InitializeLibL();
    RProperty::TType type( RProperty::EInt );
    TSecurityPolicy readPolicy( ECapability_None );
    TSecurityPolicy writePolicy( ECapabilityWriteDeviceData );
    TInt error = iProperty.Define( KPSUidNetworkInfo, 
                                   KNWRegistrationStatus, 
                                   type, 
                                   readPolicy, 
                                   writePolicy );
                                   
    if ( (error != KErrNone) && (error != KErrAlreadyExists) )
        {
        NWLOGSTRING2( KNWERROR, 
            "NW: CNWNetworkRegistrationStatusMonitor::ConstructL(), Defining\
            property failed, error code = %d .", error );
        
        User::Leave(error);
        }
    // Create CNWNetworkRegistrationStatusReceiver object.
    iNetworkRegistrationReceiver = 
        CNWNetworkRegistrationStatusReceiver::NewL( 
                *this, iPhone, iRegistrationStatus );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkRegistrationStatusMonitor::ConstructL(), Attach\
        to P&S registration status property." );
    
    // Attach to P&S registration status property
    User::LeaveIfError( iProperty.Attach( 
            KPSUidNetworkInfo, KNWRegistrationStatus ) );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkRegistrationStatusMonitor::\
        ConstructL() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkRegistrationStatusMonitor::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CNWNetworkRegistrationStatusMonitor* CNWNetworkRegistrationStatusMonitor::NewL( 
        CNWMessageHandler& aOwner, 
        RMobilePhone& aPhone, 
        TNWInfo& aNetworkInfo, 
        RMmCustomAPI& aCustomAPI )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkRegistrationStatusMonitor::NewL() Begin." );
    
    CNWNetworkRegistrationStatusMonitor* self = 
        new( ELeave ) CNWNetworkRegistrationStatusMonitor( aOwner,
                                                           aPhone, 
                                                           aNetworkInfo,
                                                           aCustomAPI );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkRegistrationStatusMonitor::NewL() End." );
    
    return self;
    }
    

// Destructor
CNWNetworkRegistrationStatusMonitor::~CNWNetworkRegistrationStatusMonitor()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkRegistrationStatusMonitor::\
        ~CNWNetworkRegistrationStatusMonitor() Begin." );
    
    Cancel();    
    delete iNetworkRegistrationReceiver;
    iProperty.Close();
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkRegistrationStatusMonitor::\
        ~CNWNetworkRegistrationStatusMonitor() End." );
    FeatureManager::UnInitializeLib();
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkRegistrationStatusMonitor::~CNWNetworkRegistrationStatusMonitor complete." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkRegistrationStatusMonitor::DoCancel
// Cancels the pending async. command.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkRegistrationStatusMonitor::DoCancel()
    {
    NWLOGSTRING( KNWREQEND, 
        "NW: CNWNetworkRegistrationStatusMonitor::DoCancel() Begin: Request\
        CancelAsyncRequest( EMobilePhoneNotifyNetwork\
        RegistrationStatusChange )" );
    
    iPhone.CancelAsyncRequest( 
            EMobilePhoneNotifyNetworkRegistrationStatusChange );
    
    NWLOGSTRING( KNWREQEND, 
        "NW: CNWNetworkRegistrationStatusMonitor::DoCancel() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkRegistrationStatusMonitor::RunL
// Receives the completion of the pending async. command,
// saves possible values from async.
// Function call and sends notification to CNWSession object.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkRegistrationStatusMonitor::RunL()
    {
    NWLOGSTRING2( KNWREQEND,
        "NW: CNWNetworkRegistrationStatusMonitor::RunL, \
        iStatus = %d", iStatus.Int() );

    if ( iStatus == KErrNone )
        {
        if ( !iRegistrationStatus == RMobilePhone::ERegistrationUnknown )
            {
            IssueRequest();
            // Cast MMETel types to NW types.
            UpdateNetworkInfo(); 
            // Update P&S property with registration status
            UpdateRegistrationStatusProperty();
            
            NWLOGSTRING3( KNWINT, 
                "NW: CNWNetworkRegistrationStatusMonitor::RunL, \
                iRegistrationStatus = %d (=ETel) %d (=NW)", 
                iRegistrationStatus, 
                iNWNetworkInfo.iRegistrationStatus );
            }
        else
            {
            IssueRequest();
            }
        }
        // Check if monitor can be restarted.
    else if ( iStatus != KErrCancel && iStatus != KErrServerTerminated )
        {
        NWLOGSTRING2( KNWERROR,
            "NW: CNWNetworkRegistrationStatusMonitor::RunL, Unknown error \
            situation, iStatus = %d", iStatus.Int() );
   
        if ( FeatureManager::FeatureSupported( KFeatureIdFfManualSelectionPopulatedPlmnList ) )
            {
            iOwner.SendErrorMessage( MNWMessageObserver::ENWNotifyNetworkRegistrationStatusChange, 
                                     iStatus.Int() );
            }
        
        IssueRequest();
        }
    else
        {
        NWLOGSTRING2( KNWERROR,
            "NW: CNWNetworkRegistrationStatusMonitor::RunL, Server \
            Terminated or request Cancelled, iStatus = %d", iStatus.Int() );
        }
    
    NWLOGSTRING( KNWREQEND, 
        "NW: CNWNetworkRegistrationStatusMonitor::RunL() End" );
    }


// ----------------------------------------------------------------------------
// CNWNetworkRegistrationStatusMonitor::IssueRequest
// Executes the async. NotifyNetworkRegistrationStatusChange function.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkRegistrationStatusMonitor::IssueRequest()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkRegistrationStatusMonitor::IssueRequest() Begin" );
    
    if ( !IsActive() )
        {
        iPhone.NotifyNetworkRegistrationStatusChange( 
                iStatus, iRegistrationStatus ); 
        SetActive();
        
        if ( !iIsInitialised )
            {
            NWLOGSTRING( KNWREQIN, 
                "NW:CNWNetworkRegistrationStatusMonitor::IssueRequest \
                >RMobilePhone::GetNetworkRegistrationStatus" );
            iNetworkRegistrationReceiver->IssueRequest();
            iIsInitialised = ETrue;
            }
        
        NWLOGSTRING( KNWREQIN, 
                "NW:CNWNetworkRegistrationStatusMonitor::IssueRequest \
                > RMobilePhone::NotifyNetworkRegistrationStatusChange" );
           
        }
    else 
        {
        NWLOGSTRING( KNWERROR, 
            "NW: CNWNetworkRegistrationStatusMonitor::IssueRequest: \
            Already active" );
        }
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkRegistrationStatusMonitor::IssueRequest() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkRegistrationStatusMonitor::UpdateNetworkInfo
// Castthe MMETel type network info to TNWInfo type.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkRegistrationStatusMonitor::UpdateNetworkInfo()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkRegistrationStatusMonitor::\
        UpdateNetworkInfo() Begin" );
    
    switch ( iRegistrationStatus )
        {
        case RMobilePhone::ENotRegisteredNoService:
            {
            iNWNetworkInfo.iRegistrationStatus = ENWNotRegisteredNoService;
            break;
            }
        case RMobilePhone::ENotRegisteredEmergencyOnly:
            {
            iNWNetworkInfo.iRegistrationStatus = ENWNotRegisteredEmergencyOnly;
            break;
            }
        case RMobilePhone::ENotRegisteredSearching:
            {
            iNWNetworkInfo.iRegistrationStatus = ENWNotRegisteredSearching;
            break;
            }
        case RMobilePhone::ERegisteredOnHomeNetwork:
            {
            iNWNetworkInfo.iRegistrationStatus = ENWRegisteredOnHomeNetwork;
            break;
            }
        case RMobilePhone::ERegisteredBusy:
            {
            iNWNetworkInfo.iRegistrationStatus = ENWRegisteredBusy;
            break;
            }
        case RMobilePhone::ERegistrationDenied:
            {
            iNWNetworkInfo.iRegistrationStatus = ENWRegistrationDenied;
            break;
            }
        case RMobilePhone::ERegisteredRoaming:
            {
            iNWNetworkInfo.iRegistrationStatus = ENWRegisteredRoaming;
            break;
            }
        case RMobilePhone::ERegistrationUnknown:
            {
            iNWNetworkInfo.iRegistrationStatus = ENWRegistrationUnknown;
            break;
            }
        default:
            {
            iNWNetworkInfo.iRegistrationStatus = ENWRegistrationUnknown;
            __ASSERT_DEBUG( !iRegistrationStatus, Panic( ENWPanicInvalidStatus ) );
            }
        }
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkRegistrationStatusMonitor::\
        UpdateNetworkInfo() End" );
    }
    
// ----------------------------------------------------------------------------
// CNWNetworkRegistrationStatusMonitor::UpdateRegistrationStatusProperty
// Updates Publish & Subscribe with the current registration status.
// ----------------------------------------------------------------------------
//
void CNWNetworkRegistrationStatusMonitor::UpdateRegistrationStatusProperty()
    {
    // KNWNetworkRegistrationStatus values map directly with 
    // TNWRegistrationStatus values
    NWLOGSTRING2( KNWREQOUT, 
        "NW:CNWNetworkRegistrationStatusMonitor::\
        UpdateRegistrationStatusProperty() Begin, rproperty::set \
        < status: %d", iNWNetworkInfo.iRegistrationStatus );

    TInt err = iProperty.Set( iNWNetworkInfo.iRegistrationStatus );   
    if ( err != KErrNone )
        {
        NWLOGSTRING2( KNWERROR, 
            "NW:CNWNetworkRegistrationStatusMonitor::\
            UpdateRegistrationStatusProperty: rproperty::set error: %d",
            err );
        }
    iOwner.SendMessage( 
            MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange );

    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkRegistrationStatusMonitor::\
        UpdateRegistrationStatusProperty() End" );
    }


//  End of File 
