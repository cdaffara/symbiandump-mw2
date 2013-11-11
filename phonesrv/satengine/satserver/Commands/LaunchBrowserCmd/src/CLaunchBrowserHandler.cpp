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
* Description:  Handles LaunchBrowser command
*
*/



#include    <MSatShellController.h>
#include    <cmmanager.h>
#include    <cmconnectionmethod.h>
#include    <cmconnectionmethoddef.h>
#include    <cmpluginpacketdatadef.h>
#include    <cmdestination.h>

#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "CLaunchBrowserHandler.h"
#include    "SatLog.h"
#include    "MSatSystemState.h"

// The pause constant for checking if BrowserApp has exited.
// This value may be adjusted to ensure the time sequence
const TInt KSatSShortLoopTimeout = 800000; //0.8 sec

// How many times the browser status is checked, when waiting for
// browser shutdown. Total time
// used to check the exiting of browser app is
// KSatSWaitCount * KSatSShortLoopTimeout.
const TInt KSatSWaitCount = 10;

// Invalid PDP type
const TInt KSatInvalidPdpType = -1;

#ifdef _DEBUG
// Class name for error information.
//lint -e752 Accessed in __ASSERT_DEBUG, but lint will not notice it.
_LIT( KLaunchBrowserHandlerName, "CLaunchBrowserHandler" );
#endif

const TInt KSatMaxIPv4AddressLength( 15 );
const TInt KSatMaxIPv6AddressLength( 39 );
const TInt KSatMinIPv4AddressLength( 7 );
const TInt KSatMinIPv6AddressLength( 7 );
const TInt KSatMaxIPv4SegmentCounter( 3 );
const TInt KSatMaxIPv4SegmentLength( 3 );
const TInt KSatIPv4Broadcast( 255 );
const TInt KSatIPv4Network( 0 );
const TInt KSatMaxIPv6SegmentCounter( 7 );
const TInt KSatMaxIPv6SegmentLength( 4 );
const TUint32 KSatLaunchBrowserProxyPortNumber( 80 );
const TUint32 KSatMaxProxyPortNumber( 9999 );
const TUint32 KSatMinProxyPortNumber( 0 );
// Granularity for getting connection methods from CM session
const TUint32 KSatCMGranularity( 5 );

_LIT( KSatAccessPointName, "SAT Launch Browser" );
_LIT( KSatGprsAccessPointName, "internet" );
_LIT( KSatIPv4Delimiter, "." );
_LIT( KSatIPv6Delimiter, ":" );

// ======== MEMBER FUNCTIONS ========
// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLaunchBrowserHandler* CLaunchBrowserHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::NewL calling" )

    CLaunchBrowserHandler* self = new( ELeave ) CLaunchBrowserHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::NewL exiting" )
    return self;
    }


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CLaunchBrowserHandler::~CLaunchBrowserHandler()
    {
    LOG( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::~CSatSLaunchBrowserHandler \
        calling" )

    if ( iAfterWait.IsStarted() )
        {
        iAfterWait.AsyncStop();
        }

    if ( iAfterTimer )
        {
        iAfterTimer->Cancel();
        delete iAfterTimer;
        iAfterTimer = NULL;
        }

    iWsSession.Close();

    Cancel();

    LOG( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::~CSatSLaunchBrowserHandler \
        exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatCommand.
// Response from the client.
// -----------------------------------------------------------------------------
//
void CLaunchBrowserHandler::ClientResponse()
    {
    LOG( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::ClientResponse calling" )

    // TPCmdResult
    iLaunchBrowserRsp.iGeneralResult = RSat::KLaunchBrowserError;

    // TAdditionalInfoType
    iLaunchBrowserRsp.iInfoType = RSat::KNoAdditionalInfo;
    iLaunchBrowserRsp.iAdditionalInfo.Zero();
    iLaunchBrowserRsp.SetPCmdNumber( iLaunchBrowserData.PCmdNumber() );

    // Perform the action that we just queried the user for.
    if ( iQueryRsp.iAccepted )
        {
        // Launch the Browser.
        TRAPD( err, LaunchBrowserL() );
        if ( KErrNone != err )
            {
            iLaunchBrowserRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            LOG2( SIMPLE, "LAUNCHBROWSER: \
                CLaunchBrowserHandler::ClientResponse error: %i", err )
            }
        else
            {
            LOG( SIMPLE,
                "LAUNCHBROWSER: CLBH::CR ESetUpMenuRequested registered" )
            TRAP_IGNORE( iUtils->RegisterL(
                this, MSatUtils::ESetUpMenuRequested ) )
            }
        }
    else // User denied this action
        {
        iLaunchBrowserRsp.iGeneralResult = RSat::KPCmdNotAcceptedByUser;
        LOG( SIMPLE, "LAUNCHBROWSER: \
            CLaunchBrowserHandler::ClientResponse not accepted" )
        if ( iQueryRsp.iSessionTerminatedByUser )
            {
            LOG( SIMPLE, "LAUNCHBROWSER: \
                 CLaunchBrowserHandler::ClientResponse close ui session" )
            // Next SimSession end will close the ui session
            iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
            }
        }
        
    // If command had icon data and was done succesfully, report that icon
    // was not shown
    // To be removed when icons are allowed in this command
    if ( ( RSat::KSuccess == iLaunchBrowserRsp.iGeneralResult ) &&
        iIconCommand )
        {
        LOG( SIMPLE, "LAUNCHBROWSER: \
                 CLaunchBrowserHandler::ClientResponse icon not display" )
        iLaunchBrowserRsp.iGeneralResult =
            RSat::KSuccessRequestedIconNotDisplayed;
        }

    TerminalRsp( RSat::ELaunchBrowser, iLaunchBrowserRspPckg );

    LOG( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::ClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Event notification
// -----------------------------------------------------------------------------
//
void CLaunchBrowserHandler::Event( TInt aEvent )
    {
    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::Event calling" )

    switch ( aEvent )
        {
        // SetUpMenu is listening by SAT Client.
        case MSatUtils::ESetUpMenuRequested:
            {
            LOG( NORMAL,
                "LAUNCHBROWSER::Event ESetUpMenuRequested catched and unreg" )
            iUtils->UnregisterEvent( this, MSatUtils::ESetUpMenuRequested );
            // Browser is brought to the top after short period of time.
            //iUtils->SatUiHandler().ShellController().
            //    BringBrowserToForegroundAfterPeriod();
            break;
            }

        default:
            {
            LOG2( NORMAL, "LAUNCHBROWSER:   Unexpected event: %i", aEvent )
            CSatCommandHandler::Event( aEvent );
            }
        }

    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CLaunchBrowserHandler::DoCancel()
    {
    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::DoCancel calling" )

    iUtils->USatAPI().NotifyLaunchBrowserCancel();

    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Requests the command notification.
// -----------------------------------------------------------------------------
//
void CLaunchBrowserHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iLaunchBrowserData) RSat::TLaunchBrowserV2();
    iQueryRsp.iAccepted = EFalse; // default

    iUtils->USatAPI().NotifyLaunchBrowser( aStatus, iLaunchBrowserPckg );

    LOG( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Precheck before executing the command.
// -----------------------------------------------------------------------------
//
TBool CLaunchBrowserHandler::CommandAllowed()
    {
    LOG( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::CommandAllowed calling" )

    iLaunchBrowserRsp.iGeneralResult = RSat::KLaunchBrowserError;
    iLaunchBrowserRsp.iInfoType = RSat::KNoAdditionalInfo;
    iLaunchBrowserRsp.iAdditionalInfo.Zero();

    RMobilePhone::TMobilePhoneRegistrationStatus registrationStatus(
        iUtils->SystemState().GetNetworkRegistrationStatus() );

    TBool commandAllowed( ETrue );

    if ( RSat::EBrowserIdNotSpecified == iLaunchBrowserData.iBrowserId )
        {
        // When get unknown browser id, such as RFU value, return
        // terminal response of command not understood.
        iLaunchBrowserRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
        commandAllowed = EFalse;
        LOG( SIMPLE, "LAUNCHBROWSER: \
            CLaunchBrowserHandler::CommandAllowed unknown browser id" )
        }
    else if ( ( RSat::EAlphaIdProvided !=
        iLaunchBrowserData.iAlphaId.iStatus ) &&
        ( RSat::ESelfExplanatory ==
            iLaunchBrowserData.iIconId.iQualifier ||
          RSat::ENotSelfExplanatory ==
            iLaunchBrowserData.iIconId.iQualifier ) )
        {
        iLaunchBrowserRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
        commandAllowed = EFalse;
        LOG( SIMPLE, "LAUNCHBROWSER: \
            CLaunchBrowserHandler::CommandAllowed icon without alpha id" )
        }
    else if (
        ( RMobilePhone::ERegisteredOnHomeNetwork != registrationStatus ) &&
        ( RMobilePhone::ERegisteredRoaming != registrationStatus ) )
        {
        iLaunchBrowserRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        iLaunchBrowserRsp.iInfoType = RSat::KMeProblem;
        iLaunchBrowserRsp.iAdditionalInfo.SetLength( 1 );
        iLaunchBrowserRsp.iAdditionalInfo[0] = RSat::KNoService;
        commandAllowed = EFalse;
        LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::CommandAllowed\
             MS is out of service (Net. Reg. status wrong)" )
        }
    else if ( RSat::EBrowserSelectionNotSet == iLaunchBrowserData.iBrowserSel )
        {
        iLaunchBrowserRsp.iGeneralResult = RSat::KCmdTypeNotUnderstood;
        commandAllowed = EFalse;
        LOG( SIMPLE, "LAUNCHBROWSER: \
            CLaunchBrowserHandler::CommandAllowed selection not set" )
        }
    else if ( RSat::ELaunchBrowserIfNotAlreadyLaunched ==
        iLaunchBrowserData.iBrowserSel )
        {
        if ( BrowserExists() )
            {
            iLaunchBrowserRsp.iInfoType = RSat::KMeProblem;
            iLaunchBrowserRsp.iAdditionalInfo.Append( 
                RSat::KBrowserUnavailable );
            commandAllowed = EFalse;
            LOG( SIMPLE, "LAUNCHBROWSER: \
                CLaunchBrowserHandler::CommandAllowed not launched" )
            }
        }
    else
        {
        LOG( SIMPLE,
            "LAUNCHBROWSER: CLaunchBrowserHandler::CommandAllowed ok" )
        }

    if ( !commandAllowed )
        {
        LOG( SIMPLE,
            "CLaunchBrowserHandler::CommandAllowed commandAllowed false" )
        iLaunchBrowserRsp.SetPCmdNumber( iLaunchBrowserData.PCmdNumber() );
        TerminalRsp( RSat::ELaunchBrowser, iLaunchBrowserRspPckg );
        }
    // Set icon command flag whether icon data was received and set qualifier
    // to no icon id
    // To be removed when icons are allowed in this command
    else if ( ( RSat::ESelfExplanatory ==
        iLaunchBrowserData.iIconId.iQualifier ) ||
        ( RSat::ENotSelfExplanatory ==
        iLaunchBrowserData.iIconId.iQualifier ) )
        {
         LOG( SIMPLE,
            "CLaunchBrowserHandler::CommandAllowed commandAllowed ENoIconId" )
        iIconCommand = ETrue;
        iLaunchBrowserData.iIconId.iQualifier = RSat::ENoIconId;
        }
    else
        {
        iIconCommand = EFalse;
        }

    LOG2( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::CommandAllowed exiting, \
         commandAllowed: %d", commandAllowed )
    return commandAllowed;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Answers for need of UI session.
// -----------------------------------------------------------------------------
//
TBool CLaunchBrowserHandler::NeedUiSession()
    {
    LOG( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::NeedUiSession calling" )

    const RSat::TAlphaId alphaId( iLaunchBrowserData.iAlphaId );

    iNeedUiSession = ETrue;

    if ( ( ( alphaId.iStatus == RSat::EAlphaIdProvided &&
        alphaId.iAlphaId.Length() == 0 ) ||
        alphaId.iStatus == RSat::EAlphaIdNull ) &&
        ( !iUtils->SystemState().IsConfirmSatOperationsOn() ) )
        {
        iNeedUiSession = EFalse;
        LOG( NORMAL,
            "LAUNCHBROWSER: CLaunchBrowserHandler::NeedUiSession no  \
            Ui session needed" )
        }

    if ( iUtils->CoverUiSupported() && iNeedUiSession )
        {
        LOG( NORMAL,
            "LAUNCHBROWSER: CLaunchBrowserHandler::NeedUiSession Ui session \
            needed" )
        TSatCommandData medEventData;
        medEventData.iPCmdNumber = RSat::ELaunchBrowser;
        medEventData.iAlphaId = iLaunchBrowserData.iAlphaId;

        if ( iUtils->SystemState().IsConfirmSatOperationsOn() )
            {
            LOG( NORMAL,
            "LAUNCHBROWSER: CLaunchBrowserHandler::NeedUiSession \
             KSatLongDuration" )
            medEventData.iDuration.iNumOfUnits = KSatLongDuration;
            }
        else
            {
            medEventData.iDuration.iNumOfUnits = KSatDefaultDuration;
            }

        medEventData.iDuration.iTimeUnit = RSat::ESeconds;
        medEventData.iIconID = iLaunchBrowserData.iIconId;
        TSatCommandPckg tPckg( medEventData );
        iUtils->RaiseSatEvent( tPckg );
        }

    LOG2( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::NeedUiSession exiting, \
         iNeedUiSession: %d", iNeedUiSession )
    return iNeedUiSession;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Called when USAT API notifies that command.
// -----------------------------------------------------------------------------
//
void CLaunchBrowserHandler::HandleCommand()
    {
    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::HandleCommand calling" )

    // Register service request handler for LaunchBrowser command,
    // If there is already service request for query, registering
    // updates command handler, so client responses comes to this
    // command handler
    TRAPD( err, iUtils->RegisterServiceRequestL(
        ESatSProactiveQuery,
        ESatSProactiveQueryResponse,
        this ) );

    if ( KErrNone != err )
        {
        iLaunchBrowserRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        iLaunchBrowserRsp.iInfoType = RSat::KMeProblem;
        iLaunchBrowserRsp.iAdditionalInfo.SetLength( 1 );
        iLaunchBrowserRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
        iLaunchBrowserRsp.SetPCmdNumber( iLaunchBrowserData.PCmdNumber() );
        TerminalRsp( RSat::ELaunchBrowser, iLaunchBrowserRspPckg );

        LOG2( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::HandleCommand error: %i", err )
        }
    else if ( iNeedUiSession )
        {
        iQueryData.iCommand = ESatSLaunchBrowserQuery;
        iQueryData.iQueryText = iLaunchBrowserData.iAlphaId.iAlphaId;
        iQueryData.iIconId.iIdentifier =
            iLaunchBrowserData.iIconId.iIdentifier;
        LOG2( SIMPLE, "CLaunchBrowserHandler::HandleCommand \
              iLaunchBrowserData.iIconId.iQualifier: %i", 
              iLaunchBrowserData.iIconId.iQualifier )
        switch ( iLaunchBrowserData.iIconId.iQualifier )
            {
            case RSat::ESelfExplanatory:
                {
                // Icon qualifier is self explanatory (to display instead
                // of the alpha id or text string).
                iQueryData.iIconId.iIconQualifier = ESatSelfExplanatory;
                break;
                }

            case RSat::ENotSelfExplanatory:
                {
                // Icon qualifier is not self explanatory.
                iQueryData.iIconId.iIconQualifier = ESatNotSelfExplanatory;
                break;
                }

            default:
                {
                // Icon qualifier not present
                iQueryData.iIconId.iIconQualifier = ESatENoIconId;
                break;
                }
            }

        if ( RSat::EAlphaIdNotPresent == iLaunchBrowserData.iAlphaId.iStatus )
            {
            LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::HandleCommand \
                 EAlphaIdNotPresent" )
            iQueryData.iAlphaIdStatus = ESatAlphaIdNotProvided;
            }
        else if ( RSat::EAlphaIdProvided ==
            iLaunchBrowserData.iAlphaId.iStatus )
            {
            LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::HandleCommand \
                 EAlphaIdProvided" )
            iQueryData.iAlphaIdStatus = ESatAlphaIdNotNull;
            }
        else
            {
            iQueryData.iAlphaIdStatus = ESatAlphaIdNull;
            }

        iQueryData.iSimApplicationName = iUtils->SatAppName();

        // Ask the user permission to launch browser. Reply will come
        // to ClientResponse() method.
        iUtils->SatUiHandler().UiSession()->SendCommand(
            &iQueryPckg,
            &iQueryRspPckg,
            ESatSProactiveQuery );
        }
    else // transparent launch
        {
        // Launch the Browser.
        TRAP( err, LaunchBrowserL() );

        if ( KErrNone != err )
            {
            iLaunchBrowserRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            LOG2( SIMPLE, "LAUNCHBROWSER: \
                CLaunchBrowserHandler::HandleCommand error: %i", err )
            }
        else
            {
            LOG( SIMPLE,
                "LAUNCHBROWSER: CLBH::CR ESetUpMenuRequested registered" )
            TRAP_IGNORE( iUtils->RegisterL(
                this, MSatUtils::ESetUpMenuRequested ) )
            }

        TerminalRsp( RSat::ELaunchBrowser, iLaunchBrowserRspPckg );
        }

    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Indicates the failure of launching ui client.
// -----------------------------------------------------------------------------
//
void CLaunchBrowserHandler::UiLaunchFailed()
    {
    LOG( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::UiLaunchFailed calling" )

    iLaunchBrowserRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
    iLaunchBrowserRsp.iInfoType = RSat::KMeProblem;
    iLaunchBrowserRsp.iAdditionalInfo.SetLength( 1 );
    iLaunchBrowserRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
    iLaunchBrowserRsp.SetPCmdNumber( iLaunchBrowserData.PCmdNumber() );
    TerminalRsp( RSat::ELaunchBrowser, iLaunchBrowserRspPckg );

    LOG( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::UiLaunchFailed exiting" )
    }
    
// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized, harmless.
CLaunchBrowserHandler::CLaunchBrowserHandler() :
    CSatCommandHandler(),
    iLaunchBrowserData(),
    iLaunchBrowserPckg( iLaunchBrowserData ),
    iLaunchBrowserRsp(),
    iLaunchBrowserRspPckg( iLaunchBrowserRsp ),
    iQueryData(),
    iQueryPckg( iQueryData ),
    iQueryRsp(),
    iQueryRspPckg( iQueryRsp ),
    iUidWmlBrowser( TUid::Uid( KWmlcHandler ) ),
    iAfterTimer( NULL ),
    // To be removed when icons are allowed in this command
    iIconCommand( EFalse )
    {
    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::CLaunchBrowserHandler \
        calling - exiting" )
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CLaunchBrowserHandler::ConstructL()
    {
    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::ConstructL calling" )

    // Open the Window server session.
    User::LeaveIfError( iWsSession.Connect() );

    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::ConstructL exiting" )
    }


// -----------------------------------------------------------------------------
// Checks if Browser application is launched.
// -----------------------------------------------------------------------------
//
TBool CLaunchBrowserHandler::BrowserExists()
    {
    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::BrowserExists calling" )
    TBool browserExists( EFalse );

    TApaTaskList taskList( iWsSession );
    TApaTask task = taskList.FindApp( iUidWmlBrowser );
    browserExists = task.Exists();

    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::BrowserExists exiting" )
    return browserExists;
    }

// -----------------------------------------------------------------------------
// The Launch Browser handler.
// -----------------------------------------------------------------------------
//
void CLaunchBrowserHandler::LaunchBrowserL()
    {
    LOG( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::LaunchBrowserL calling" )

    // Check the URL length.
    const TInt urlLen( iLaunchBrowserData.iUrl.Length() );

    // ID of the accesspoint in the communications database.
    TUid accesspointID( TUid::Uid( KErrNotFound ) );

    // If apId is given by SIM card, then we use that, otherwise,
    // get default apId from default connection method.
    TUint32 apId( 0 );
    // GCF is supported since S60 5.0
    apId = GetAccessPointByGatewayAddressL();
    if ( apId )
        {
        LOG2( SIMPLE, 
              "LAUNCHBROWSER: CLaunchBrowserHandler::LaunchBrowserL, \
              Get gcf AP id = %d", apId )
        accesspointID.iUid = static_cast<TInt32>( apId );
        }    
    
    // Get default AP if GCF ap not found
    if ( KErrNotFound == accesspointID.iUid )
        {
        LOG( SIMPLE, 
             "LAUNCHBROWSER: CLaunchBrowserHandler::LaunchBrowserL, \
             no gcf AP id" )
        apId = GetAccessPointByDefaultL();
        if ( apId )
            {
            LOG2( SIMPLE, 
                  "LAUNCHBROWSER: CLaunchBrowserHandler::LaunchBrowserL, \
                  default AP id = %d", apId )            
            accesspointID.iUid = static_cast<TInt32>( apId );
            }
        }
    
    LOG2( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::LaunchBrowserL, \
          accesspointID.iUid=%d", accesspointID.iUid )
    
    HBufC* param = NULL;
    // If the URL is given by SIM, then we use that
    if ( urlLen )
        {
        // Url is given by SIM, and we use that.
        param = HBufC::NewLC( urlLen );
        param->Des().Copy( iLaunchBrowserData.iUrl );
        LOG3( SIMPLE, 
              "LAUNCHBROWSER: CLaunchBrowserHandler::LaunchBrowserL,\
              url is given by SIM, url=%s, length=%d", 
              param, param->Des().Length() )
        }  
    // If no url from SIM, get default URL by selected access point.
    else if ( !param && accesspointID.iUid > 0 )
        {
        param = GetDefaultUrlByAccessPointLC( 
                static_cast<TUint32>( accesspointID.iUid) );
        LOG3( SIMPLE, 
              "LAUNCHBROWSER: CLaunchBrowserHandler::LaunchBrowserL,\
              url is given by IAP, url=%s, length=%d",
              param, param->Des().Length() )
        }
    else
        {
        param = KNullDesC().AllocLC();
        LOG( SIMPLE, 
             "LAUNCHBROWSER: CLaunchBrowserHandler::LaunchBrowserL no url" )
        }
    
    switch ( iLaunchBrowserData.iBrowserSel )
        {
        case RSat::ECloseExistingLaunchNewBrowserSession:
            {
            LOG( NORMAL, "LAUNCHBROWSER: CLaunchBrowserHandler:: \
                 LaunchBrowserL, ECloseExistingLaunchNewBrowserSession" )
            CloseBrowser();  // Close Browser Session if it exists.
            LaunchWithUrlL( param->Des(), accesspointID );
            break;
            }

        case RSat::ELaunchBrowserIfNotAlreadyLaunched:
            {
            LOG( NORMAL, "LAUNCHBROWSER: CLaunchBrowserHandler:: \
                 LaunchBrowserL, ELaunchBrowserIfNotAlreadyLaunched" )
            if ( BrowserExists() )
                {
                LOG( SIMPLE,"CLaunchBrowserHandler::LaunchBrowserL \
                     BrowserExists" )
                // Browser exists, error response returned.
                iLaunchBrowserRsp.iGeneralResult = RSat::KLaunchBrowserError;
                iLaunchBrowserRsp.iInfoType = RSat::KMeProblem;
                iLaunchBrowserRsp.iAdditionalInfo.Append( 
                    RSat::KBrowserUnavailable );
                }
            else
                {
                LaunchWithUrlL( param->Des(), accesspointID );
                }
            break;
            }

        case RSat::EUseExistingBrowser:
            {
            // SAT Shell Controller looks if browser already exists.
            LOG( NORMAL, "LAUNCHBROWSER: CLaunchBrowserHandler:: \
                 LaunchBrowserL,EUseExistingBrowser" )
            LaunchWithUrlL( param->Des(), accesspointID );
            break;
            }

        case RSat::EBrowserSelectionNotSet:
        default:
            __ASSERT_DEBUG( EFalse,
                Panic( KLaunchBrowserHandlerName, KErrBadName ) );
            break;
        }

    CleanupStack::PopAndDestroy( param );

    LOG( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::LaunchBrowserL exiting" )
    }

// --------------------------------------------------------------------------
// Closes the Browser application if it is active and waits until
// closing is complete.
// --------------------------------------------------------------------------
//
void CLaunchBrowserHandler::CloseBrowser()
    {
    LOG( SIMPLE, 
         "LAUNCHBROWSER: CLaunchBrowserHandler::CloseBrowser calling" )
    TApaTaskList taskList( iWsSession );
    TApaTask task = taskList.FindApp( iUidWmlBrowser );

    if ( task.Exists() )
        {
        task.SendSystemEvent( EApaSystemEventShutdown );

        TBool browserExists( ETrue );
        TInt waitCount( 0 );
        LOG2( SIMPLE, 
              "CLaunchBrowserHandler::CloseBrowser browserExists: %d",
              browserExists )
        // Make sure that closing is completed.
        while ( browserExists )
            {
            After( KSatSShortLoopTimeout );
            ++waitCount;

            TApaTask newTask = taskList.FindApp( iUidWmlBrowser );
            browserExists = newTask.Exists();

            if ( KSatSWaitCount <= waitCount )
                {
                newTask.KillTask();
                browserExists = EFalse;
                LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler:: \
                    CloseBrowser force kill task" )
                }
            }
        }

    LOG( SIMPLE, 
         "LAUNCHBROWSER: CLaunchBrowserHandler::CloseBrowser exiting" )
    }

// --------------------------------------------------------------------------
// Launches The Browser with URL.
// --------------------------------------------------------------------------
//
void CLaunchBrowserHandler::LaunchWithUrlL( const TDesC& /*aParam*/,
    const TUid& /*aAccessPointUid*/ )
    {
    LOG( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::LaunchWithUrlL calling" )
//    LOG3( SIMPLE,
//         "LAUNCHBROWSER: CLaunchBrowserHandler::LaunchWithUrlL\
//          url=%s, IAP=%d", &aParam, aAccessPointUid.iUid )        

    // Browser launching called.
//    TInt err = iUtils->SatUiHandler().ShellController().LaunchBrowserL(
//        aParam, aAccessPointUid );
    TInt err =  KErrNone;

    if ( KErrNone == err )
        {
        if ( !iQueryRsp.iRequestedIconDisplayed &&
            ( ( RSat::ESelfExplanatory ==
            iLaunchBrowserData.iIconId.iQualifier ) ||
            ( RSat::ENotSelfExplanatory ==
            iLaunchBrowserData.iIconId.iQualifier ) ) )
            {
            iLaunchBrowserRsp.iGeneralResult =
                RSat::KSuccessRequestedIconNotDisplayed;
            LOG( SIMPLE,
                "LAUNCHBROWSER: CLaunchBrowserHandler::LaunchWithUrlL \
                no icon shown" )
            }
        else
            {
            iLaunchBrowserRsp.iGeneralResult = RSat::KSuccess;
            LOG( SIMPLE,
                "LAUNCHBROWSER: CLaunchBrowserHandler::LaunchWithUrlL \
                command ok" )
            }
        }
    else
        {
        iLaunchBrowserRsp.iGeneralResult  = RSat::KLaunchBrowserError;
        iLaunchBrowserRsp.iInfoType       = RSat::KMeProblem;
        iLaunchBrowserRsp.iAdditionalInfo.Append( 
                RSat::KBrowserUnavailable );
        LOG2( SIMPLE, 
        "LAUNCHBROWSER: CLaunchBrowserHandler::LaunchWithUrlL error: %i", 
        err )
        }

    LOG( SIMPLE,
        "LAUNCHBROWSER: CLaunchBrowserHandler::LaunchWithUrlL exiting" )
    }

// --------------------------------------------------------------------------
// Starts timer. Function returns as the given time has elapsed or
// timer is cancelled.
// --------------------------------------------------------------------------
void CLaunchBrowserHandler::After(
    const TTimeIntervalMicroSeconds32& aDelay,
    const TTimeIntervalMicroSeconds32& aInterval )
    {
    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::After calling" )

    if ( !iAfterWait.IsStarted() )
        {
        TRAPD( err, iAfterTimer = CPeriodic::NewL( EPriorityStandard ) )
        LOG2( SIMPLE, 
        "LAUNCHBROWSER: CLaunchBrowserHandler::After err: %i", err )
        if ( ( KErrNone == err ) && iAfterTimer )
            {
            iAfterTimer->Start(
                aDelay, aInterval, TCallBack( DelayCallBack, this ) );
            iAfterWait.Start();

            delete iAfterTimer;
            iAfterTimer = NULL;
            }
        }

    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::After exiting" )
    }

// --------------------------------------------------------------------------
// Timer call back function
// --------------------------------------------------------------------------
TInt CLaunchBrowserHandler::DelayCallBack( TAny* aPtr )
    {
    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::DelayCallBack \
    calling" )
    CLaunchBrowserHandler* ptrThis =
        static_cast<CLaunchBrowserHandler*>( aPtr );

    if ( ptrThis && ( ptrThis->iAfterWait.IsStarted() ) )
        {
        LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::DelayCallBack \
             stop iAfterWait" )
        ptrThis->iAfterWait.AsyncStop();
        }
    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::DelayCallBack \
    exiting" )
    return ( EFalse );
    }

// --------------------------------------------------------------------------
// Create a new access point using the Access Point Engine.
// --------------------------------------------------------------------------
//
TUint32 CLaunchBrowserHandler::CreateAccessPointL( 
        const TDesC16& aApName,
        const TDesC16& aProxyServerAddr,
        const TUint32 aProxyPortNumber,
        const RPacketContext::TProtocolType& aIPvType )
    {
    LOG( SIMPLE, 
         "LAUNCHBROWSER: CLaunchBrowserHandler::CreateAccessPointL calling" )

    // Connection method interface.
    RCmConnectionMethod cm;
    // Connection method manager interface.
    RCmManager cmManager;
    // Used to exit connection methods go through loop
    TBool exitConnectionMethodsLoop( EFalse );
    
    cmManager.OpenLC();

    // Find if the access point with the same name is already exist.
    // If such access point exists, delete it and then create a new one.
     
    // Get the Connection Methods does not belogns to any destination
    RArray<TUint32> array = RArray<TUint32>( KSatCMGranularity );
    CleanupClosePushL( array );

    cmManager.ConnectionMethodL( array );

    // Go through the Connection Method list to find if there is matched one
     
    for( TInt i = 0; 
         ( i < array.Count() ) && !exitConnectionMethodsLoop; ++i)
        {
        cm = cmManager.ConnectionMethodL( array[i] );
        CleanupClosePushL( cm );
        
        // query the access point name
        HBufC * pa = NULL;
        pa = cm.GetStringAttributeL( CMManager::ECmName );
        CleanupStack::PushL(pa);
        
        if ( *pa == aApName )
            {
            // Find the matched connection method
            LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::\
                 CreateAccessPointL, find the existing ap" )
            cm.DeleteL();
            exitConnectionMethodsLoop = ETrue;
            }
        CleanupStack::PopAndDestroy( pa );
        CleanupStack::PopAndDestroy( &cm ); 
        } 
    CleanupStack::PopAndDestroy( &array );

    // Create a connection method without destination.
    cm = cmManager.CreateConnectionMethodL( KUidPacketDataBearerType );
    CleanupClosePushL( cm );
    
    // Fulfill connection method params with default setting from OCC.
    // We use trap to continue following operations even if leaving from 
    // FulfillConnnectionMethodL.
    TRAPD( errCode, FulfillConnnectionMethodL( cmManager, cm ) );
    if ( errCode != KErrNone )
        {
        LOG3( SIMPLE, "SATENGINE: CLaunchBrowserHandler::CreateAccessPointL"
            " error %d at line %d", errCode, __LINE__ )
        }
    // Set attributes of the connection method.
    cm.SetStringAttributeL( CMManager::ECmName, aApName );
    cm.SetIntAttributeL( CMManager::EPacketDataPDPType, aIPvType );
    cm.SetStringAttributeL( CMManager::ECmWapIPGatewayAddress, 
                            TPtrC( aProxyServerAddr ) );
    cm.SetStringAttributeL( CMManager::ECmProxyServerName, 
                            TPtrC( aProxyServerAddr ) );
    cm.SetIntAttributeL( CMManager::ECmProxyPortNumber, aProxyPortNumber );
        
    // Update the access point ID for use later.
    cm.UpdateL();

    TUint32 apId = cm.GetIntAttributeL( CMManager::ECmIapId );
    LOG2( SIMPLE, 
          "LAUNCHBROWSER: CLaunchBrowserHandler::CreateAccessPointL apId=%d",
          apId )

    CleanupStack::PopAndDestroy( &cm );
    CleanupStack::PopAndDestroy( &cmManager );
    
    LOG( SIMPLE, 
         "LAUNCHBROWSER: CLaunchBrowserHandler::CreateAccessPointL exiting" )
    
    return apId;
    }

// --------------------------------------------------------------------------
// Get a AP from the Connection Method Manager.
// --------------------------------------------------------------------------
TUint32 CLaunchBrowserHandler::GetAccessPointByGatewayAddressL()
    {
    LOG( SIMPLE, "LAUNCHBROWSER: \
        CLaunchBrowserHandler::GetAccessPointByGatewayAddressL calling" )
    // Access Point ID.
    TUint32 apId( 0 ); 
    // Unknown IP address type.
    TInt ipvType( 0 );
    // Get the gateway address.
    TPtrC gatewayAddrPtr( iLaunchBrowserData.iText );
    // Get the bearer list.
    TPtrC8 bearerListPtr( iLaunchBrowserData.iBearerList ); 

    // Proxy address.
    HBufC* proxyAddress = NULL;
    // Proxy port number.
    TUint32 proxyPortNumber( 0 );
    HBufC* pa = NULL;
    TUint32 pn( 0 );

    // Used to exit the connection method go through loop
    TBool exitConnectionMethodsLoop( EFalse );
    
    // SAT access point name.
    TPtrC satAPN( KNullDesC );
    
    // Use SAT application name as SAT APN.
    satAPN.Set( iUtils->SatAppName() );
    // Use default SAT APN if SAT application name is not available.
    if ( satAPN == KNullDesC )
        {
        LOG( SIMPLE, 
             "CLaunchBrowserHandler::GetAccessPointByGatewayAddressL\
             satAPN == KNullDesC" )
        satAPN.Set( KSatAccessPointName );
        }
        
    // Get the proxy address and port number
    proxyAddress = HBufC::NewLC( gatewayAddrPtr.Length() );
    TPtr proxyAddrPtr( proxyAddress->Des() );
    SeparatePortNumberFromAddress( gatewayAddrPtr, proxyAddrPtr, 
        proxyPortNumber );
    proxyAddrPtr.TrimAll();
    
    // Check the launch browser package for gateway setting.
    ipvType = ValidateGatewayAddress( proxyAddrPtr );
    
    if ( RPacketContext::EPdpTypeIPv4 == ipvType || \
         RPacketContext::EPdpTypeIPv6 == ipvType )
        {
        LOG2( SIMPLE, 
              "CLaunchBrowserHandler::GetAccessPointByGatewayAddressL \
              gatewayAddrPtr.Length(): %d", gatewayAddrPtr.Length() )
        // Check the launch browser package for bearer setting if the gateway
        // address is valid.
        LOG2( SIMPLE, 
              "LAUNCHBROWSER: CLaunchBrowserHandler:: \
              GetAccessPointByGatewayAddressL, IPvType is %d",
              ipvType)
        if ( gatewayAddrPtr.Length() )
            {
            // The bearer is GPRS.
            if ( bearerListPtr != KNullDesC8 && bearerListPtr.Length() > 0 
                    && RSat::KGprsBearer == bearerListPtr[0] )
                {
                
                // Go through the candidate connection methods to find 
                // the one matched the gateway address. If a matched one 
                // is found,using it as the access point for SIM service, 
                // else a new one is created.      
                RCmManager cmManager;
                cmManager.OpenLC();

                // Get the Connection Methods does not belongs to 
                // any destination
                RArray<TUint32> array = RArray<TUint32>( 
                                        KSatCMGranularity );
                CleanupClosePushL( array );

                cmManager.ConnectionMethodL( array );

                // Go through the Connection Method list to find 
                // if there is matched one
                 
                for( TInt i = 0; 
                     ( i < array.Count() ) && !exitConnectionMethodsLoop; 
                     ++i )
                    {
                    RCmConnectionMethod cm = cmManager.ConnectionMethodL(
                                                          array[i] );
                    CleanupClosePushL( cm );
                    
                    // query the Gateway proxy address 
                    // of the Connection Method
                    pa = cm.GetStringAttributeL( 
                         CMManager::ECmWapIPGatewayAddress );
                    CleanupStack::PushL(pa);
                    
                    // query the Gateway proxy port number of the Connection 
                    // Method
                    pn = cm.GetIntAttributeL( 
                            CMManager::ECmProxyPortNumber );
                    
                    if (( pn == proxyPortNumber ) && 
                        ( *pa == *proxyAddress ))
                        {
                        // Find the matched connection method
                        apId = cm.GetIntAttributeL( CMManager::ECmIapId );
                        LOG2( SIMPLE, 
                              "LAUNCHBROWSER: CLaunchBrowserHandler::\
                              GetAccessPointByGatewayAddressL, apId=%d",
                              apId )
                        exitConnectionMethodsLoop = ETrue;
                        }
                        
                    CleanupStack::PopAndDestroy( pa );
                    CleanupStack::PopAndDestroy( &cm );
                    }
            
                CleanupStack::PopAndDestroy( &array );
                CleanupStack::PopAndDestroy( &cmManager );
                
                LOG2( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler:: \
                      GetAccessPointByGatewayAddressL apId=%d", apId )
                           
                if ( !apId )
                    {
                    // Create a new AP if there is not a matched one.
                    apId = CreateAccessPointL( 
                            satAPN,
                            proxyAddrPtr,
                            proxyPortNumber,
                            static_cast<RPacketContext::TProtocolType>(
                                                              ipvType ) );
                    }
                }
            }
        }
    CleanupStack::PopAndDestroy( proxyAddress );
    
    LOG2( SIMPLE, 
          "LAUNCHBROWSER: CLaunchBrowserHandler::\
          GetAccessPointByGatewayAddressL exiting, apId=%d", apId )
    
    // return the AP.
    return apId;
    }

// --------------------------------------------------------------------------
// Get an Access Point by default.
// --------------------------------------------------------------------------
TUint32 CLaunchBrowserHandler::GetAccessPointByDefaultL()
    {
    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::\
         GetAccessPointByDefaultL calling" )
         
    TUint32 apId ( KErrNone );
    RCmManager cmManager;
    cmManager.OpenLC();
    RCmConnectionMethod cm;
    CleanupClosePushL( cm );
    
    // Get a default connection method
    TInt ret( KErrNotFound );
    GetDefConnMethodL( cmManager, cm, ret );
    if ( !ret )
        {
        LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::\
             GetAccessPointByDefaultL default connection found" )        
        apId = cm.GetIntAttributeL( CMManager::ECmIapId );
        }
    
    CleanupStack::PopAndDestroy( &cm );
    CleanupStack::PopAndDestroy( &cmManager );
    
    LOG2( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::\
          GetAccessPointByDefaultL exiting, apId=%d", apId )
    
    return apId;
    }

// --------------------------------------------------------------------------
// Get default URL by access point
// --------------------------------------------------------------------------
HBufC* CLaunchBrowserHandler::GetDefaultUrlByAccessPointLC( TUint32 aApId )
    {
    LOG2( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::\
         GetDefaultUrlByAccessPointLC calling, aApId=%d", aApId )
    
    HBufC* url = NULL;
    if ( aApId > 0 )
        {
        // Create CMManager session
        RCmManager cmManager;
        cmManager.OpenLC();
        
        // get the connection method by apId
        RCmConnectionMethod cm = cmManager.ConnectionMethodL( aApId );
        CleanupClosePushL( cm );
        
        // Get the default URL from default accesspoint
        // If the access point doesn't include the default URL,
        // the empty string is returned.
        url = cm.GetStringAttributeL( CMManager::ECmStartPage );
        
        CleanupStack::PopAndDestroy( &cm );
        CleanupStack::PopAndDestroy( &cmManager );

        CleanupStack::PushL( url );  
        }
    
    if ( !url )
        {
        url = KNullDesC().AllocLC();
        }
    
    LOG2( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::\
          GetDefaultUrlByAccessPointLC exiting, url=%s", url )
    return url;
    }

// --------------------------------------------------------------------------
// Convert a descriptor into a integer.
// --------------------------------------------------------------------------
TInt CLaunchBrowserHandler::ConvertDesToInt( const TDesC16& aDes )
    {
    LOG2( SIMPLE, "LAUNCHBROWSER: \
          CLaunchBrowserHandler::ConvertDesToInt calling,length of aDes:%d",
          aDes.Length() )

    TLex converter;
    TInt intValue( KErrNotFound );
    TBool digital( ETrue );

    if ( aDes != KNullDesC && aDes.Length() > 0 )
        {
        // The literal value of every element in the descriptor 
        // should be numerical.
        for ( TInt i = 0; i < aDes.Length() && digital; i++ )
            {
            TChar ch( aDes[i] );
            
            if ( !ch.IsDigit() )
                {
                LOG( SIMPLE, "LAUNCHBROWSER: \
                CLaunchBrowserHandler::ConvertDesToInt not digit" )
                digital = EFalse;
                }
            }
            
        if ( digital )
            {
            LOG( SIMPLE, "LAUNCHBROWSER: \
            CLaunchBrowserHandler::ConvertDesToInt digit" )
            // Converting.
            converter.Assign( aDes );
            converter.Val( intValue );
            }
        }

    LOG2( SIMPLE, "LAUNCHBROWSER: \
          CLaunchBrowserHandler::ConvertDesToInt exiting,intValue:%d", 
          intValue )

    return intValue;
    }

// --------------------------------------------------------------------------
// Check the validity of a descriptor in hex.
// --------------------------------------------------------------------------
TBool CLaunchBrowserHandler::ValidateHexadecimal( const TDesC16& aHex )
    {
    LOG( SIMPLE, "LAUNCHBROWSER: \
        CLaunchBrowserHandler::ValidateHexadecimal calling" )

    // The hexadecimal descriptor can be null.
    TInt length( aHex.Length() );
    TBool retVal( ETrue );
    LOG2( SIMPLE, "LAUNCHBROWSER: \
        CLaunchBrowserHandler::ValidateHexadecimal length:%d", length )

    // Validate characters one by one.
    for ( TInt i = 0; i < length && retVal; i++ )
        {
        TChar ch( aHex[i] );
        
        if ( !ch.IsHexDigit() )
            {
            LOG2( SIMPLE, "LAUNCHBROWSER: \
            CLaunchBrowserHandler::ValidateHexadecimal hex digit,i: %d",
            i )
            retVal = EFalse;
            }
        }

    LOG2( SIMPLE, "LAUNCHBROWSER: \
          CLaunchBrowserHandler::ValidateHexadecimal exiting, retVal:%d",
          retVal )
        
    return retVal;
    }

// --------------------------------------------------------------------------
// Check the validity of a IPv4 address. For example, 210.13.246.172.
// --------------------------------------------------------------------------
TBool CLaunchBrowserHandler::ValidateIPv4Address( 
        const TDesC16& aAddress )
    {
    LOG2( SIMPLE, "LAUNCHBROWSER: \
    CLaunchBrowserHandler::ValidateIPv4Address calling,aAddress: %s",
    &aAddress )

    // Separate the address to be three segments using dot and validate
    // each segment.
    TPtrC segPtr( aAddress );
    TBuf<KSatMaxIPv4SegmentLength> segment;
    TInt posInDes( KErrNone );
    TInt posInPtr( KErrNone );
    TInt segCounter( KErrNone );
    TBool first( ETrue );
    TBool retVal( ETrue );
    // Segment value in integer.
    TInt intValue( KErrNone );
        
    // Convert each segment into integer.
    while ( KErrNotFound != ( posInPtr = segPtr.Find( KSatIPv4Delimiter ) )
            && retVal )
        {
        
        if ( KSatMaxIPv4SegmentLength >= posInPtr && 0 < posInPtr )
            {
            LOG( SIMPLE, "LAUNCHBROWSER: \
            CLaunchBrowserHandler::ValidateIPv4Address posInPtr valid" )
            segCounter++;
            posInDes += posInPtr;
            segment = segPtr.Mid( 0, posInPtr );
            
            // The first integer should be > 0 and < 255, others should be 
            // >=0 and < 255.
            intValue = ConvertDesToInt( segment );
            LOG2( SIMPLE, "LAUNCHBROWSER: \
            CLaunchBrowserHandler::ValidateIPv4Address first:%d",first )
            if ( first )
                {
                first = EFalse;
                
                if ( !( KSatIPv4Network < intValue && 
                        KSatIPv4Broadcast > intValue ) )
                    {
                    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::\
                         ValidateIPv4Address intValue valid" )
                    retVal = EFalse;
                    }
                }
            else
                {
                
                if ( !( KSatIPv4Network <= intValue && 
                        KSatIPv4Broadcast > intValue ) )
                    {
                    LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::\
                         ValidateIPv4Address intValue range" )
                    retVal = EFalse;
                    }
                }
            // Skip dot.
            posInDes++;
            segPtr.Set( aAddress.Mid( posInDes, 
                aAddress.Length() - posInDes ) );
            }
        else
            {
            retVal = EFalse;
            }
        }
    
    // IPv4 address is consist of 4 segment
    if ( KSatMaxIPv4SegmentCounter == segCounter && retVal )
        {
        LOG( SIMPLE, "LAUNCHBROWSER: \
        CLaunchBrowserHandler::ValidateIPv4Address valid address" )
        // Validate the last segment.
        segment = segPtr;
        
        if ( KSatMaxIPv4SegmentLength < segment.Length() )
            {
            LOG( SIMPLE, "LAUNCHBROWSER: \
            CLaunchBrowserHandler::ValidateIPv4Address address invalid" )
            retVal = EFalse;
            }
            
        intValue = ConvertDesToInt( segment );
        
        if ( !( KSatIPv4Network < intValue && 
                KSatIPv4Broadcast > intValue ) )
            {
            LOG( SIMPLE, "LAUNCHBROWSER: \
            CLaunchBrowserHandler::ValidateIPv4Address intValue range" )
            retVal = EFalse;
            }
        }
    else
        {
        retVal = EFalse;
        }
        
    LOG2( SIMPLE, "LAUNCHBROWSER: \
        CLaunchBrowserHandler::ValidateIPv4Address exiting,retVal:%d",
        retVal )

    return retVal;
    }

// --------------------------------------------------------------------------
// Check the validity of a IPv6 address. For example,
// 3ffe:ffff:101::230:6eff:fe04:d9ff.
// --------------------------------------------------------------------------
TBool CLaunchBrowserHandler::ValidateIPv6Address( 
        const TDesC16& aAddress )
    {
    LOG2( SIMPLE, "LAUNCHBROWSER: \
    CLaunchBrowserHandler::ValidateIPv6Address calling,aAddress: %s",
    &aAddress )

    // Separate the address to be eight segments using colon and validate
    // each segment.
    TPtrC segPtr( aAddress );
    TBuf<KSatMaxIPv6SegmentLength> segment;
    TInt posInDes( KErrNone );
    TInt posInPtr( KErrNone );
    TInt segCounter( KErrNone );
    TBool first( ETrue );
    TBool retVal( ETrue );
    
    while ( KErrNotFound != ( posInPtr = segPtr.Find( KSatIPv6Delimiter ) )
            && retVal )
        {
        
        if ( KSatMaxIPv6SegmentLength >= posInPtr && 0 <= posInPtr )
            {
            LOG( SIMPLE, "LAUNCHBROWSER: \
            CLaunchBrowserHandler::ValidateIPv6Address posInPtr valid" )
            segCounter++;
            posInDes += posInPtr;
            segment = segPtr.Mid( 0, posInPtr );
            LOG2( SIMPLE, "LAUNCHBROWSER: \
            CLaunchBrowserHandler::ValidateIPv6Address first:%d",first )
            if ( first )
                {
                first = EFalse;
                
                if ( 0 >= segment.Length() )
                    {
                    retVal = EFalse;
                    }
                }
                
            if ( KSatMaxIPv6SegmentLength < segment.Length() )
                {
                LOG( SIMPLE, "LAUNCHBROWSER: \
                CLaunchBrowserHandler::ValidateIPv6Address address invalid" )
                retVal = EFalse;
                }
                
            if ( !ValidateHexadecimal( segment ) )
                {
                LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::\
                ValidateIPv6Address address invalid hex" )
                retVal = EFalse;
                }
            
            // Skip colon.
            posInDes++; 
            segPtr.Set( aAddress.Mid( posInDes, 
                aAddress.Length() - posInDes ) );
            }
        else 
            {
            retVal = EFalse;
            }
        }

    // IPv6 address is consist of 8 segment
    if ( KSatMaxIPv6SegmentCounter == segCounter && retVal )
        {
        LOG( SIMPLE, "LAUNCHBROWSER: \
        CLaunchBrowserHandler::ValidateIPv6Address valid address" )
        // Validate the last segment.
        segment = segPtr;
        
        if ( KSatMaxIPv6SegmentLength < segment.Length() )
            {
            LOG( SIMPLE, "LAUNCHBROWSER: \
            CLaunchBrowserHandler::ValidateIPv6Address address invalid" )
            retVal = EFalse;
            }
            
        if ( !ValidateHexadecimal( segment ) )
            {
            LOG( SIMPLE, "LAUNCHBROWSER: CLaunchBrowserHandler::\
            ValidateIPv6Address address invalid hex" )
            retVal = EFalse;
            }
        }
    else
        {
        retVal = EFalse;
        }
    
    LOG2( SIMPLE, "LAUNCHBROWSER: \
          CLaunchBrowserHandler::ValidateIPv6Address exiting,retVal:%d", 
          retVal )

    return retVal;
    }
    
// --------------------------------------------------------------------------
// Check the validity of a gateway IP address. Either it is IPv4 or IPv6. 
// For example, 210.13.246.172 or 3ffe:ffff:101::230:6eff:fe04:d9ff.
// --------------------------------------------------------------------------
TInt CLaunchBrowserHandler::ValidateGatewayAddress( 
        const TDesC16& aAddress )
    {
    LOG2( SIMPLE, "LAUNCHBROWSER: \
          CLaunchBrowserHandler::ValidateGatewayAddress calling,length of \
          aAddress: %d", aAddress.Length() )
    
    LOG2( SIMPLE, "LAUNCHBROWSER: \
    CLaunchBrowserHandler::ValidateGatewayAddress aAddress: %s", &aAddress )
    TInt ipvType( KSatInvalidPdpType );
    TInt length( aAddress.Length() );
    
    if ( length )
        {
        // The maximum length of a IP address is less than 39.
        if ( KSatMaxIPv4AddressLength >= length && 
                KSatMinIPv4AddressLength <= length )
            {
            LOG( SIMPLE, "LAUNCHBROWSER: \
            CLaunchBrowserHandler::ValidateGatewayAddress IPV4 address" )
            // aAddress is a IPv4 address.
            if ( ValidateIPv4Address( aAddress ) )
                {
                LOG( SIMPLE, "LAUNCHBROWSER: \
                CLaunchBrowserHandler::ValidateGatewayAddress valid IPV4 \
                address" )
                ipvType = RPacketContext::EPdpTypeIPv4;
                }
            }
        else if ( KSatMaxIPv6AddressLength >= length && 
                    KSatMinIPv6AddressLength <= length )
            {
            LOG( SIMPLE, "LAUNCHBROWSER: \
            CLaunchBrowserHandler::ValidateGatewayAddress IPV6 address" )
            // aAddress is a IPv6 address.
            if ( ValidateIPv6Address( aAddress ) )
                {
                LOG( SIMPLE, "LAUNCHBROWSER: \
                CLaunchBrowserHandler::ValidateGatewayAddress valid IPV6 \
                address" )
                ipvType = RPacketContext::EPdpTypeIPv6;
                }
            }
        }
    
    LOG2( SIMPLE, "LAUNCHBROWSER: \
        CLaunchBrowserHandler::ValidateGatewayAddress exiting,ipvType:%d",
        ipvType )

    // Returned IP address.
    return ipvType;
    }

// --------------------------------------------------------------------------
// Separate the port number from a address if it is existing. For example,
// 210.13.246.172:8080.
// --------------------------------------------------------------------------
void CLaunchBrowserHandler::SeparatePortNumberFromAddress( 
    const TDesC16& aAddress, 
    TDes16& aPureAddr,
    TUint32& aPortNumber )
    {
    LOG2( SIMPLE, "LAUNCHBROWSER: \
    CLaunchBrowserHandler::SeparatePortNumberFromAddress calling,\
    aAddress: %s", &aAddress )

    // The following three variables are used to locate the port number.
    TBool isSeparator( EFalse );
    TBool portNumberBegin( EFalse );
    TBool portNumberEnd( EFalse );
    TBool pureAddrEnd( EFalse );
    // Count delimiters in the address.
    TInt delimiterCounter( 0 );
    // Count symbols in a segment of the address.
    TInt symbolCounter( 0 );
    // Currsent position of the address descriptor.
    TInt curPos( 0 );
    TInt posOffset( 0 );
    // The position of the last valid character.
    TInt rightBound( 0 );
    // Digital port number.
    TUint32 portNumber( KSatLaunchBrowserProxyPortNumber );
    // The descriptor pointer for the address.
    TPtrC addrPtr;
    // The length of the address.
    TInt addrLen( 0 );
    
    // The address is a valid descriptor.
    if ( NULL != &aAddress &&
            KSatMinIPv4AddressLength <= aAddress.Length() )
        {
        addrPtr.Set( aAddress );
        addrLen = aAddress.Length();
        curPos = addrPtr.Find( KSatIPv4Delimiter );
        
        LOG2( SIMPLE, "CLaunchBrowserHandler::\
        SeparatePortNumberFromAddress curPos: %i", curPos )
        if ( KErrNotFound != curPos )
            {
            // Skip three delimiters in the IPv4 address.
            delimiterCounter++;
     
            while ( KSatMaxIPv4SegmentCounter > delimiterCounter
                        && curPos < addrLen )
                {
                curPos++;
                posOffset = addrPtr.Right( addrLen - curPos ).Find( 
                                KSatIPv4Delimiter );
                LOG2( SIMPLE, "CLaunchBrowserHandler::\
                SeparatePortNumberFromAddress posOffset: %i", posOffset )
                if ( KErrNotFound != posOffset )
                    {
                    delimiterCounter++;
                    curPos += posOffset;
                    }
                else
                    {
                    delimiterCounter = KSatMaxIPv4SegmentCounter;
                    }
                }

            // Locate the separator between the pure address and 
            // the port number.
            curPos++;
     
            while ( curPos < addrLen 
                &&  KSatMaxIPv4SegmentLength > symbolCounter 
                && !isSeparator )
                {
                TChar ch( addrPtr[curPos] );
     
                if ( ch.IsAlphaDigit() )
                    {
                    LOG( SIMPLE, "CLaunchBrowserHandler::\
                    SeparatePortNumberFromAddress AlphaDigit" )
                    symbolCounter++;
                    }
                else
                    {
                    isSeparator = ETrue;
                    }

                curPos++;
                }
            }
        else
            {
            curPos = addrPtr.Find( KSatIPv6Delimiter );
            LOG2( SIMPLE, "CLaunchBrowserHandler::\
            SeparatePortNumberFromAddress curPos: %i", curPos )
            if ( KErrNotFound != curPos )
                {
                // Skip seven delimiters in the IPv6 address.
                delimiterCounter++;
     
                while ( KSatMaxIPv6SegmentCounter > delimiterCounter
                            && curPos < addrLen )
                    {
                    curPos++;
                    posOffset = addrPtr.Right( addrLen - curPos ).Find( 
                                    KSatIPv6Delimiter );
                    LOG2( SIMPLE, "CLaunchBrowserHandler::\
                    SeparatePortNumberFromAddress posOffset: %i", posOffset )
                    if ( KErrNotFound != posOffset )
                        {
                        delimiterCounter++;
                        curPos += posOffset;
                        }
                    }

                // Locate the separator between the pure address and 
                // the port number.
                curPos++;
     
                while ( curPos < addrLen && 
                            KSatMaxIPv6SegmentLength > symbolCounter && 
                            !isSeparator )
                    {
                    TChar ch( addrPtr[curPos] );
     
                    if ( ch.IsAlphaDigit() )
                        {
                        LOG( SIMPLE, "CLaunchBrowserHandler::\
                        SeparatePortNumberFromAddress AlphaDigit" )
                        symbolCounter++;
                        }
                    else
                        {
                        isSeparator = ETrue;
                        }

                    curPos++;
                    }
                }
            }

        // Extract the pure address.
        while ( curPos < addrLen && KSatMinIPv4AddressLength < curPos 
                && !pureAddrEnd )
            {
            TChar ch( addrPtr[curPos] );
            
            if ( !ch.IsAlphaDigit() )
                {
                LOG( SIMPLE, "CLaunchBrowserHandler::\
                SeparatePortNumberFromAddress not AlphaDigit" )
                curPos--;
                }
            else
                {
                pureAddrEnd = ETrue;
                }
            }

        if ( curPos <= addrLen )
            {
            aPureAddr.Copy( addrPtr.Left( curPos + 1 ) );
            }

        // Extract the port number.
        // Locate the beginning of the port number.
        curPos++;
    
        while ( curPos < addrLen && !portNumberBegin)
            {
            TChar ch( addrPtr[curPos] );

            if ( ch.IsDigit() )
                {
                LOG( SIMPLE, "CLaunchBrowserHandler::\
                SeparatePortNumberFromAddress Digit" )
                portNumberBegin = ETrue;
                }
            else
                {
                curPos++;
                }
            }
        
        // Locate the end of the port number.    
        rightBound = addrLen;
        rightBound--;

        while ( rightBound > curPos && !portNumberEnd )
            {
            TChar ch( addrPtr[rightBound] );

            if ( ch.IsDigit() )
                {
                LOG( SIMPLE, "CLaunchBrowserHandler::\
                SeparatePortNumberFromAddress Digit" )
                portNumberEnd = ETrue;
                }
            else 
                {
                rightBound--;
                }
            }
        
        // Convert the port number and validate it.
        // Set the port number to default port number
        aPortNumber = portNumber; 
        if ( curPos < rightBound && rightBound < addrLen )
            {
            portNumber = ConvertDesToInt( 
                addrPtr.Mid( curPos, rightBound - curPos + 1 ) );

            if ( KSatMaxProxyPortNumber > portNumber &&
                    KSatMinProxyPortNumber < portNumber )
                {
                LOG( SIMPLE, "CLaunchBrowserHandler::\
                SeparatePortNumberFromAddress valid portNumber" )
                aPortNumber = portNumber;
                }
            }
        }
        
    LOG( SIMPLE, 
    "LAUNCHBROWSER: CLaunchBrowserHandler::SeparatePortNumberFromAddress \
    exiting" )
    }

// --------------------------------------------------------------------------
// Fulfill connecion method params with default params.
// --------------------------------------------------------------------------
void CLaunchBrowserHandler::FulfillConnnectionMethodL( 
    RCmManager& aCmManager,
    RCmConnectionMethod& aCm )
    {
    LOG( SIMPLE, "SATENGINE: CLaunchBrowserHandler::\
        FulfillConnnectionMethodL calling" )
    
    // The following 3 lines are used to preset the default parameters
    // to avoid doing nothing for them if leaving.
    aCm.SetStringAttributeL( CMManager::EPacketDataAPName, 
        KSatGprsAccessPointName );
    aCm.SetBoolAttributeL( CMManager::ECmIFPromptForAuth, EFalse );
    aCm.SetBoolAttributeL( CMManager::ECmDisablePlainTextAuth, EFalse );
        
    // Get default access point ID.
    RCmConnectionMethod defCm;
    CleanupClosePushL( defCm );
    
    TInt ret ( KErrNotFound );
    GetDefConnMethodL( aCmManager, defCm, ret );
    
    if ( !ret )
        {
        // Default AP found! Copy parameters from
        // default AP to new created AP.
        
        // Copy AP name.
        HBufC* strValue = defCm.GetStringAttributeL( 
            CMManager::EPacketDataAPName );
        LOG2( SIMPLE, "SATENGINE: CLaunchBrowserHandler::\
            FulfillConnnectionMethodL AP Name is %s", strValue )
        CleanupStack::PushL( strValue );
        aCm.SetStringAttributeL( CMManager::EPacketDataAPName, *strValue );
        CleanupStack::PopAndDestroy( strValue );
        
        // Copy authentic name
        strValue = defCm.GetStringAttributeL( CMManager::ECmIFAuthName );
        LOG2( SIMPLE, "SATENGINE: CLaunchBrowserHandler::\
            FulfillConnnectionMethodL Auch name is %s", strValue )
        CleanupStack::PushL( strValue );
        aCm.SetStringAttributeL( CMManager::ECmIFAuthName, *strValue );
        CleanupStack::PopAndDestroy( strValue );
        
        // Copy authentic pass
        strValue = defCm.GetStringAttributeL( CMManager::ECmIFAuthPass );
        LOG2( SIMPLE, "SATENGINE: CLaunchBrowserHandler::\
            FulfillConnnectionMethodL Auch pass is %s", strValue )
        CleanupStack::PushL( strValue );
        aCm.SetStringAttributeL( CMManager::ECmIFAuthPass, *strValue );
        CleanupStack::PopAndDestroy( strValue );
        
        // Copy default URL
        strValue = defCm.GetStringAttributeL( CMManager::ECmStartPage );
        LOG2( SIMPLE, "SATENGINE: CLaunchBrowserHandler::\
            FulfillConnnectionMethodL Start page pass is %s", strValue )
        CleanupStack::PushL( strValue );
        aCm.SetStringAttributeL( CMManager::ECmStartPage, *strValue );
        CleanupStack::PopAndDestroy( strValue );
        
        // Copy status if prompt for auth
        TBool bValue = defCm.GetBoolAttributeL( 
                CMManager::ECmIFPromptForAuth );
        aCm.SetBoolAttributeL( CMManager::ECmIFPromptForAuth, bValue );
        
        // Copy security status.
        bValue = defCm.GetBoolAttributeL( 
                CMManager::ECmDisablePlainTextAuth );
        aCm.SetBoolAttributeL( CMManager::ECmIFPromptForAuth, bValue );
        }
        
    CleanupStack::PopAndDestroy( &defCm );
    
    LOG( SIMPLE, "SATENGINE: CLaunchBrowserHandler::\
        FulfillConnnectionMethodL exiting" )
    }

// --------------------------------------------------------------------------
// Check the validity of offered bearer type
// --------------------------------------------------------------------------
void CLaunchBrowserHandler::ValidateBearerTypeL( RCmConnectionMethod& aCm, 
                                                 TInt& aIsValid )
    {
    LOG( SIMPLE, "SATENGINE: CLaunchBrowserHandler::\
         ValidateBearerTypeL calling" )
        
    TUint32 bearerType = aCm.GetIntAttributeL( 
        CMManager::ECmBearerType );
    LOG2( SIMPLE, "SATENGINE: CLaunchBrowserHandler::\
        ValidateBearerTypeL bearerType=0x%x", bearerType )
    
    if ( KUidPacketDataBearerType == bearerType )
        {
        LOG( SIMPLE, "SATENGINE: CLaunchBrowserHandler::\
            ValidateBearerTypeL bearer type is GPRS" )
        aIsValid = KErrNone;
        }
    
    LOG2( SIMPLE, "SATENGINE: CLaunchBrowserHandler::\
          ValidateBearerTypeL exiting, aIsValid=%d", aIsValid )
    }

// --------------------------------------------------------------------------
// Get default connection method
// --------------------------------------------------------------------------
void CLaunchBrowserHandler::GetDefConnMethodL( RCmManager& aCmManager,
                                               RCmConnectionMethod& aDefCm,
                                               TInt& aIsValid )
    {
    LOG( SIMPLE, "SATENGINE: CLaunchBrowserHandler::\
         GetDefConnMethodL calling" )   
         
    // Go through the default connection methods to find 
    // the one valid connection method.   
    TCmDefConnValue defConn;
    aCmManager.ReadDefConnL( defConn );
    
    LOG3( SIMPLE, "SATENGINE: CLaunchBrowserHandler::\
      GetDefConnMethodL DefConn type=%d, id=%d", 
      defConn.iType, defConn.iId )
    
    // Mark aIsFound as KErrNone if there is one valid 
    // connection method, otherwise KErrNotFound.
    aIsValid = KErrNotFound;
    
    // Default setting is a connection method
    if ( ECmDefConnConnectionMethod == defConn.iType )
        {
        LOG( SIMPLE, "SATENGINE: CLaunchBrowserHandler::\
             GetDefConnMethodL ECmDefConnConnectionMethod" )
        aDefCm = aCmManager.ConnectionMethodL( defConn.iId );
        // Check bearer type, leave if not GPRS
        ValidateBearerTypeL( aDefCm, aIsValid );
        }

    // Default setting is a destination
    else if ( ECmDefConnDestination == defConn.iType )
        {
        LOG( SIMPLE, "SATENGINE: CLaunchBrowserHandler::\
            GetDefConnMethodL ECmDefConnDestination" )
        RCmDestination defDes = aCmManager.DestinationL( defConn.iId );
        
        TInt desCount = defDes.ConnectionMethodCount();
        
        LOG2( SIMPLE, "SATENGINE: CLaunchBrowserHandler::\
            GetDefConnMethodL ECmDefConnDestination count=%d",
            desCount )
        
        for ( TInt i = 0; 
              desCount > 0 && aIsValid != KErrNone && i < desCount; i++ )
            {
            // Go through all the connection methods, 
            // until find a valid one.
            aDefCm = defDes.ConnectionMethodL( i );
            ValidateBearerTypeL( aDefCm, aIsValid );
            }
        }
    else
        {
        aIsValid = KErrNotFound;
        }
    LOG( SIMPLE, "SATENGINE: CLaunchBrowserHandler::GetDefConnMethodL exit" )
    }
// End Of File
