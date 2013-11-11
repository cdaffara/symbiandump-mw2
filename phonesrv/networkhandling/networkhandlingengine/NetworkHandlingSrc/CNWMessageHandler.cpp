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
* Description:  This module contains the implementation of CNWMessageHandler
*
*/



// INCLUDE FILES
#include    <featmgr.h>
#include    <nwhandlingengine.h>
#include    "CNWMessageHandler.h"
#include    "CNWNetworkCurrentNetworkMonitor.h"
#include    "CNWNetworkRegistrationStatusMonitor.h"
#include    "CNWProgrammableOperatorNameCommand.h"
#include    "CNWServiceProviderNameCommand.h"
#include    "CNWNetworkModeMonitor.h"
#include    "CNWNetworkProviderNameCommand.h"
#include    "CNWSubscriberIdCommand.h"
#include    "NWLogger.h"
#ifdef RD_PHONE_NG
#include    "cnwnetworkselectionsettingmonitor.h"
#endif // RD_PHONE_NG
#include    "CNWDynamicCapsMonitor.h"
// CONSTANTS
const TInt KNWNetworkMonitorCurrent = 0;
const TInt KNWNetworkMonitorMode = 1;
const TInt KNWNetworkMonitorRegistration = 2;
#ifdef RD_PHONE_NG
const TInt KNWNetworkMonitorSettingSelection = 3;
const TInt KNWNetworkMonitorDynamicCaps = 4;
#else // RD_PHONE_NG
const TInt KNWNetworkMonitorDynamicCaps = 3;
#endif // RD_PHONE_NG

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWMessageHandler::CNWMessageHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
EXPORT_C CNWMessageHandler::CNWMessageHandler( 
        CNWSession& aNetworkData,
        MNWMessageObserver& aMessageObserver,
        TNWInfo& aNWInfo )
        : iNetworkData( aNetworkData ),
          iMessageObserver( aMessageObserver ),
          iNetworkInfo( aNWInfo )
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWMessageHandler::CNWMessageHandler() Begin" );
    
    // initialise current network info
    iNetworkInfo.iRegistrationStatus = ENWRegistrationUnknown;
    iNetworkInfo.iNetworkMode = ENWModeUnknown;
    iNetworkInfo.iStatus = ENWStatusUnknown;
    iNetworkInfo.iCountryCode = KNullDesC;
    iNetworkInfo.iNetworkId = KNullDesC;
    iNetworkInfo.iDisplayTag = KNullDesC;
    iNetworkInfo.iShortName = KNullDesC;
    iNetworkInfo.iLongName = KNullDesC;
    iNetworkInfo.iViagIndicatorType = ENWViagIndicatorTypeNone;
    iNetworkInfo.iViagTextTag = KNullDesC;
    iNetworkInfo.iMCNIndicatorType = ENWMCNIndicatorTypeNone;
    iNetworkInfo.iMCNName = KNullDesC;
    iNetworkInfo.iSPName = KNullDesC;
    iNetworkInfo.iServiceProviderNameDisplayReq = RMobilePhone::KDisplaySPNNotRequired;
    iNetworkInfo.iNPName = KNullDesC;
    iNetworkInfo.iOperatorNameInfo.iType = RMmCustomAPI::EOperatorNameHighestPriority;
    iNetworkInfo.iOperatorNameInfo.iName = KNullDesC;
    iNetworkInfo.iNoServerMode = ENWNotRegisteredOutOfRange;
    iNetworkInfo.iVoicePrivacyStatus = ENWVoicePrivacyStatusUnknown;
    iNetworkInfo.iPLMNField = KNullDesC;
#ifdef RD_PHONE_NG
    iNetworkInfo.iSelectionSetting = ENWNetworkSelectionUnknown;
#endif // RD_PHONE_NG
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWMessageHandler::CNWMessageHandler() End" );
    }
// ----------------------------------------------------------------------------
// CNWMessageHandler::BaseConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CNWMessageHandler::BaseConstructL()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWMessageHandler::BaseConstructL() Begin" );
    
    // initialise internal network info
    iInterNetworkInfo.iSubscriberId = KNullDesC;
    iInterNetworkInfo.iCellId = 0;
    iInterNetworkInfo.iLac = 0;
    iInterNetworkInfo.iAreaKnown = ETrue;

    //Connect ETel server
    NWLOGSTRING( KNWREQOUT, 
        "NW: CNWMessageHandler::ConstructL > rtelserver::connect" );
    User::LeaveIfError( iServer.Connect( 128 ) );
    
    // Load ETel TSY module
    NWLOGSTRING( KNWREQOUT,
        "NW: CNWMessageHandler::ConstructL > rtelserver::loadphonemodule" );
    User::LeaveIfError( iServer.LoadPhoneModule( KMmTsyModuleName ) );
    
    // Set this session to receive detailed errors
    NWLOGSTRING( KNWREQOUT,
        "NW: CNWMessageHandler::ConstructL > rtelserver::\
        setextendederrorgranularity" );
    User::LeaveIfError( iServer.SetExtendedErrorGranularity( 
            RTelServer::EErrorExtended ));

    //This function opens a phone subsession by name, 
    //and starts the modem initialisation process.
    NWLOGSTRING( KNWREQOUT, 
        "NW: CNWMessageHandler::ConstructL > rmobilephone::open" );
    User::LeaveIfError( iPhone.Open( iServer, KMmTsyPhoneName ) );
    
    //This function opens a phone subsession by name, 
    //and starts the modem initialisation process.
    NWLOGSTRING( KNWREQOUT, 
        "NW: CNWMessageHandler::ConstructL > rmmcustomapi::open" );
    User::LeaveIfError( iCustomAPI.Open( iPhone ) );

    // Create CNWNetworkCurrentNetworkMonitor object 
    // and insert it to monitor container.
    CNWNetworkCurrentNetworkMonitor* tempCurrentNetworkMonitor = 
        CNWNetworkCurrentNetworkMonitor::NewL( *this, 
                                                    iPhone, 
                                                    iNetworkInfo, 
                                                    iCustomAPI, 
                                                    iInterNetworkInfo );
                                                    
    CleanupStack::PushL( tempCurrentNetworkMonitor );
    User::LeaveIfError( iMonitorContainer.Insert( 
                            tempCurrentNetworkMonitor, 
                            KNWNetworkMonitorCurrent ) );
                            
    CleanupStack::Pop( tempCurrentNetworkMonitor );

    // Create CNWNetworkModeMonitor object and insert it to monitor container.
    CNWNetworkModeMonitor* tempNetworkModeMonitor = 
        CNWNetworkModeMonitor::NewL( *this, iPhone, iNetworkInfo, iCustomAPI );
    CleanupStack::PushL( tempNetworkModeMonitor );
    
    User::LeaveIfError( iMonitorContainer.Insert( 
                                tempNetworkModeMonitor, 
                                KNWNetworkMonitorMode ) );
                                
    CleanupStack::Pop( tempNetworkModeMonitor );
    
    // Create CNWNetworkRegistrationStatusMonitor object 
    // and insert it to monitor container.
    CNWNetworkRegistrationStatusMonitor* tempNetworkRegMonitor = 
        CNWNetworkRegistrationStatusMonitor::NewL( 
                *this, iPhone, iNetworkInfo, iCustomAPI );
        
    CleanupStack::PushL( tempNetworkRegMonitor );
    
    User::LeaveIfError( iMonitorContainer.Insert( 
                                    tempNetworkRegMonitor, 
                                    KNWNetworkMonitorRegistration ) );
                                    
    CleanupStack::Pop( tempNetworkRegMonitor );   
    // Create NetworkProviderName command
    iNetworkProviderNameCommand = CNWNetworkProviderNameCommand::NewL( *this, 
                                                                    iCustomAPI, 
                                                                    iNetworkInfo );

#ifdef RD_PHONE_NG 
    // Create CNWNetworkSelectionSettingMonitor object 
    // and insert it to monitor container.
    CNWNetworkSelectionSettingMonitor* tempNetworkSelectionSettingMonitor = 
        CNWNetworkSelectionSettingMonitor::NewL( 
                *this, iPhone, iNetworkInfo, iCustomAPI );
    CleanupStack::PushL( tempNetworkSelectionSettingMonitor );
    
    User::LeaveIfError( iMonitorContainer.Insert( 
                                tempNetworkSelectionSettingMonitor, 
                                KNWNetworkMonitorSettingSelection ) );
                                
    CleanupStack::Pop( tempNetworkSelectionSettingMonitor );
#endif    
    // Create SubscriberIdCommand command
    iSubscriberIdCommand = CNWSubscriberIdCommand::NewL( 
            *this, iPhone, iInterNetworkInfo );

    // Create ProgrammableOperatorLogo command
    iProgrammableOperatorNameCommand =
        new ( ELeave ) CNWProgrammableOperatorNameCommand( 
                *this, iCustomAPI, iNetworkInfo );

    // Create ServiceProviderName handler
    iServiceProviderNameCommand =
        new ( ELeave ) CNWServiceProviderNameCommand( 
                *this, iPhone, iNetworkInfo );

    // Start initialising network handling module.
    iMonitorContainer[ KNWNetworkMonitorRegistration ]->Initialise();

    // Create CNWDynamicCapsMonitor object and insert it to monitor container.
    // Will be used to hide alpha tag when CS registration is unsuccessful
    FeatureManager::InitializeLibL();
    TInt err( KErrNone );
    if( FeatureManager::FeatureSupported( 
            KFeatureIdFfDisplayNetworkNameAfterCsRegistration ))
        {
        TRAP(err, 
            CNWDynamicCapsMonitor* tempDynamicCapsMonitor =
                CNWDynamicCapsMonitor::NewL( *this, iPhone, iNetworkInfo, 
                                             iCustomAPI );
            CleanupStack::PushL( tempDynamicCapsMonitor );

            User::LeaveIfError( iMonitorContainer.Insert( 
                                        tempDynamicCapsMonitor,
                                        KNWNetworkMonitorDynamicCaps ) );
            CleanupStack::Pop( tempDynamicCapsMonitor ); );
        if( err == KErrNone )
            {
            iMonitorContainer
                [ KNWNetworkMonitorDynamicCaps ]->Initialise();
            }
        }
    FeatureManager::UnInitializeLib();
    User::LeaveIfError(err);
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWMessageHandler::BaseConstructL() End" );
    }


// Destructor
EXPORT_C CNWMessageHandler::~CNWMessageHandler()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWMessageHandler::~CNWMessageHandler() Begin" );
    
    delete iNetworkProviderNameCommand;
    delete iSubscriberIdCommand;
    delete iServiceProviderNameCommand;
    delete iProgrammableOperatorNameCommand;

    // This deletes all monitors.
    iMonitorContainer.ResetAndDestroy( );

    iCustomAPI.Close();
    iPhone.Close();
    if ( iServer.Handle() )
        {
        iServer.UnloadPhoneModule( KMmTsyModuleName );
        iServer.Close();
        }
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWMessageHandler::~CNWMessageHandler() End" );
    }

// ----------------------------------------------------------------------------
// CNWMessageHandler::SendMessage
// Reroutes messages to the Client object
// The function controls the starting of the monitors other than registration 
// status monitor.Other monitors are starter when registration status message 
// is received and phone has not been registered.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
EXPORT_C void CNWMessageHandler::SendMessage(
    MNWMessageObserver::TNWMessages aMessage ) // send message
    {
    NWLOGSTRING( KNWMESOUT,
        "NW: CNWMessageHandler::SendMessage() Begin");
    
    if ( aMessage == MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange )
        {
        if ( !iIsRegistered && iNetworkInfo.iRegistrationStatus != 
                ENWNotRegisteredSearching )
            {
            // Start other monitors only after we are successfully 
            // registered to network.
            NWLOGSTRING( KNWMESOUT,
                "NW: CNWMessageHandler::SendMessage  Start monitors");
            iIsRegistered = ETrue;
#ifdef RD_PHONE_NG 
            iMonitorContainer[ KNWNetworkMonitorSettingSelection ]->Initialise();
#endif // RD_PHONE_NG   
            iMonitorContainer[ KNWNetworkMonitorCurrent ]->Initialise();
            iMonitorContainer[ KNWNetworkMonitorMode ]->Initialise();
            }

        // If terminal is registered to network get serviceprovidername
        // else clean serviceprovidername information.
        if ( iNetworkInfo.iRegistrationStatus == ENWRegisteredOnHomeNetwork ||
             iNetworkInfo.iRegistrationStatus == ENWRegisteredRoaming )
            {
            // Start command querys
            iSubscriberIdCommand->IssueRequest();
            iNetworkProviderNameCommand->IssueRequest();
            iServiceProviderNameCommand->IssueRequest();
            iProgrammableOperatorNameCommand->IssueRequest();
            }
        else
            {
            iNetworkInfo.iSPName = KNullDesC;
            // Invalidate programmable name in TNWInfo
            iMessageObserver.HandleNetworkMessage( 
                MNWMessageObserver::ENWMessageProgrammableOperatorInfoUpdating );
            }
        }
    else if ( aMessage == MNWMessageObserver::ENWMessageNetworkInfoChange )
        {
        if ( iPreviousStatus != iNetworkInfo.iStatus )
            {
            // Invalidate programmable name in TNWInfo
            iMessageObserver.HandleNetworkMessage( 
                MNWMessageObserver::ENWMessageProgrammableOperatorInfoUpdating );
            iPreviousStatus = iNetworkInfo.iStatus;                 
            }
        iProgrammableOperatorNameCommand->IssueRequest();
        }           
    else if ( aMessage == MNWMessageObserver::ENWMessageProgrammableOperatorInfoUpdating  )
        {
        iProgrammableOperatorNameCommand->IssueRequest();
        }
    else if ( aMessage == MNWMessageObserver::ENWMessageNetworkProviderNameUpdating   )
        {
        iNetworkProviderNameCommand->IssueRequest();
        }
    else if ( aMessage == MNWMessageObserver::ENWMessageServiceProviderNameUpdating  )
        {
        iServiceProviderNameCommand->IssueRequest();
        iServiceProviderNameCommand->SetAllowSendMessage( ETrue );
        }
    else if ( aMessage == MNWMessageObserver::ENWMessageServiceProviderNameChange )
        {
        HandleUpdateReadingStatus( ESPNEFRead, ETrue );
        }
    else if ( aMessage == MNWMessageObserver::ENWMessageNetworkProviderNameChange )
        {
        HandleUpdateReadingStatus( ENPNEFRead, ETrue );
        }
    else if ( aMessage == MNWMessageObserver::ENWMessageProgrammableOperatorInfoChange )
        {
        HandleUpdateReadingStatus( EProgEFRead, ETrue );
        }
    else if ( aMessage == MNWMessageObserver::ENWMessageAllowRefresh )
        {
        iServiceProviderNameCommand->SetAllowSendMessage( EFalse );
        }


    #ifdef TEF_LOGGING_ENABLED
        TPtrC messageName = GetNameByMessage( aMessage );
        NWLOGSTRING2( KNWINT, 
            "NW: CNWMessageHandler::SendMessage, messageName=%s",
            messageName.Ptr( ) );
    #endif
    if ( aMessage != MNWMessageObserver::ENWMessageAllowRefresh )
        {
        iMessageObserver.HandleNetworkMessage( aMessage );
        }
    
    NWLOGSTRING( KNWMESOUT,
        "NW: CNWMessageHandler::SendMessage() End");
    }

#ifdef TEF_LOGGING_ENABLED

// -----------------------------------------------------------------------------
// CNWMessageHandler::GetNameByMessage
// Return message name, given the id of message.
// -----------------------------------------------------------------------------
//
TPtrC CNWMessageHandler::GetNameByMessage(
        MNWMessageObserver::TNWMessages aMessage ) const
    {
    switch ( aMessage )
        {
        case MNWMessageObserver::ENWMessageNetworkIndicatorChange:
            return _L("ENWMessageNetworkIndicatorChange");
        case MNWMessageObserver::ENWMessageNetworkInfoChange:
            return _L("ENWMessageNetworkInfoChange");
        case MNWMessageObserver::ENWMessageNetworkModeChange:
            return _L("ENWMessageNetworkModeChange");
        case MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange:
            return _L("ENWMessageNetworkRegistrationStatusChange");
        case MNWMessageObserver::ENWMessageProgrammableOperatorInfoChange:
            return _L("ENWMessageProgrammableOperatorInfoChange");
        case MNWMessageObserver::ENWMessageNetworkProviderNameChange:
            return _L("ENWMessageNetworkProviderNameChange");
        case MNWMessageObserver::ENWMessageServiceProviderNameChange:
            return _L("ENWMessageServiceProviderNameChange");
        case MNWMessageObserver::ENWMessageNetworkCellReselection:
            return _L("ENWMessageNetworkCellReselection");
        case MNWMessageObserver::ENWMessageNetworkConnectionFailure:
            return _L("ENWMessageNetworkConnectionFailure");
        case MNWMessageObserver::ENWMessageCurrentCellInfoMessage:
            return _L("ENWMessageCurrentCellInfoMessage");
        case MNWMessageObserver::ENWMessageCurrentHomeZoneMessage:
            return _L("ENWMessageCurrentHomeZoneMessage");
        case MNWMessageObserver::ENWMessageNetworkEmergencyMode:
            return _L("ENWMessageNetworkEmergencyMode");
        case MNWMessageObserver::ENWMessageVoicePrivacyStatusChange:
            return _L("ENWMessageVoicePrivacyStatusChange");
        case MNWMessageObserver::ENWMessageStartSystemSearchRequestCompleteOk:
            return _L("ENWMessageStartSystemSearchRequestCompleteOk");
        case MNWMessageObserver::ENWMessageStartSystemSearchRequestCompleteFail:
            return _L("ENWMessageStartSystemSearchRequestCompleteFail");
        case MNWMessageObserver::ENWMessageStopProtocolStackRequestCompleteOk:
            return _L("ENWMessageStopProtocolStackRequestCompleteOk");
        case MNWMessageObserver::ENWMessageStopProtocolStackRequestCompleteFail:
            return _L("ENWMessageStopProtocolStackRequestCompleteFail");
        case MNWMessageObserver::ENWMessageProgrammableOperatorInfoUpdating:
            return _L("ENWMessageProgrammableOperatorInfoUpdating"); 
        case MNWMessageObserver::ENWMessageNetworkProviderNameUpdating:
            return _L("ENWMessageNetworkProviderNameUpdating"); 
        case MNWMessageObserver::ENWMessageServiceProviderNameUpdating:
            return _L("ENWMessageServiceProviderNameUpdating"); 
        case MNWMessageObserver::ENWMessageDynamicCapsChange:
            {
            return _L("ENWMessageDynamicCapsChange");
            break;
            }
        default:
            NWLOGSTRING2( KNWINT, 
                "NW: CNWMessageHandler::GetNameByMessage, unknown message = %d",
                 static_cast<TInt>( aMessage ) );
            return _L("Unknown message");
        } //lint !e788
    }
#endif  // Help function for logging.


// ----------------------------------------------------------------------------
// CNWMessageHandler::HandleSubscriberIdChange
// Empty implementation for Handle Subscriber Id Change
// ----------------------------------------------------------------------------
//
void CNWMessageHandler::HandleSubscriberIdChange()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWMessageHandler::HandleSubscriberIdChange() Called");
    }

// ----------------------------------------------------------------------------
// CNWMessageHandler::SendErrorMessage
// Reroutes error messages to the Client object.
// ----------------------------------------------------------------------------
//
void CNWMessageHandler::SendErrorMessage( 
    MNWMessageObserver::TNWOperation aOperation,
    TInt aErrorCode )
    {
    NWLOGSTRING3( KNWMESOUT,
        "NW: CNWMessageHandler::SendErrorMessage() Begin,\
        aOperation = %d, aErrorCode = %d ",
        aOperation, aErrorCode );
    
    if ( aOperation == MNWMessageObserver::ENWGetNetworkProviderName )
        {
        HandleUpdateReadingStatus( ENPNEFRead, ETrue );
        }
    else if ( aOperation == MNWMessageObserver:: ENWGetProgrammableOperatorName )
        {
        HandleUpdateReadingStatus( EProgEFRead, ETrue );
        }
    else if ( aOperation == MNWMessageObserver::ENWGetServiceProviderName )
        {
        HandleUpdateReadingStatus( ESPNEFRead, ETrue );
        }
    
    iMessageObserver.HandleNetworkError( aOperation, aErrorCode );
    
    NWLOGSTRING( KNWMESOUT,
        "NW: CNWMessageHandler::SendMessage() End");
    }

// ----------------------------------------------------------------------------
// CNWMessageHandler::HandleUpdateReadingStatus
// Empty implementation for Handle SIM Reading status Change
// ----------------------------------------------------------------------------
//
void CNWMessageHandler::HandleUpdateReadingStatus(  
        const TNWRead& /*aElementFile*/, TBool /*aReadStatus*/ )
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWMessageHandler::HandleUpdateReadingStatus() Called");
    }
//  End of File  
