/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Container of event monitor classes
*
*/



// INCLUDE FILES
#include    <f32file.h>
#include    <syslangutil.h>
#include    "MSatEventMonitor.h"
#include    "MSatUtils.h"
#include    "MSatBIPUtils.h"
#include    "MSatSystemState.h"
#include    "TSatEventMonitorFactory.h"
#include    "CSatEventMonitorContainer.h"
//lint -e766 Used inside TRAP macro, lint misfunction.
#include    "EnginePanic.h"
#include    "TUSatAPI.h"
#include    "SatSOpcodes.h"
#include    "SatLog.h"

// CONSTANTS
const TUint KLangTextSize( 2 );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatEventMonitorContainer* CSatEventMonitorContainer::NewL(
    TUSatAPI& aUsatAPI,
    MSatUtils& aSatUtils )
    {
    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::NewL calling" )

    CSatEventMonitorContainer* self =
        new( ELeave )CSatEventMonitorContainer( aUsatAPI, aSatUtils );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( /* self */ );

    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::NewL exiting" )
    return self;
    }

// Destructor
CSatEventMonitorContainer::~CSatEventMonitorContainer()
    {
    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::\
        ~CSatEventMonitorContainer calling" )

    iEventMonitors = NULL;

    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::\
        ~CSatEventMonitorContainer exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::Event
// From MSatEventObserver
// -----------------------------------------------------------------------------
//
void CSatEventMonitorContainer::Event( TInt aEvent )
    {
    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::Event calling" )

    //lint -e{961} else block meaningless, event handled elsewhere.
    if ( MSatUtils::EBipCommandExecuting == aEvent )
        {
        LOG( SIMPLE, "EventMonitor: BIP Command Executing" )
        iBipExecuting = ETrue;
        }
    else if ( MSatUtils::EBipCommandDoneExecuting == aEvent )
        {
        LOG( SIMPLE, "EventMonitor: BIP Command NOT executing" )
        iBipExecuting = EFalse;

        if ( iEventPending )
            {
            iEventPending = EFalse;
            LOG( SIMPLE, "EventMonitor: Executing pending envelope" )
            EventDownload( iPendingEvent, iPendingFirstArg, iPendingSecondArg );
            }
        }

    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::Event exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::SetUpEventMonitors
// Updates all event monitors in list.
// -----------------------------------------------------------------------------
//
TInt CSatEventMonitorContainer::SetUpEventMonitors( TUint aEvents )
    {
    LOG2( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::SetUpEventMonitors calling %d",
        aEvents )
    const TInt monitorCount( iEventMonitors->Count() );
    TInt errCode = KErrNotSupported;
    LOG2( SIMPLE, "SATENGINE: CSatEventMonitorContainer::SetUpEventMonitors \
          monitorCount: %i", monitorCount )
    // Check is events supported
    if ( EventsSupported( aEvents ) )
        {
        LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::\
        SetUpEventMonitors events supported" )
        // Update all monitors from list
        for ( TInt index = 0; index < monitorCount; index++ )
            {
            MSatEventMonitor* monitor = iEventMonitors->At( index );
            __ASSERT_ALWAYS( monitor, PanicSatEngine( ESatEngineNullPointer ) );
            // Update monitor
            errCode = monitor->UpdateMonitor( aEvents );

            if ( KErrNone != errCode )
                {
                LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::\
                SetUpEventMonitors KErrNone != errCode" )
                // Error, cancel all monitors and end this loop
                CancelAllMonitors();
                index = monitorCount;
                }
            }
        }

    LOG2( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::SetUpEventMonitors exiting %d",
        errCode )

    return errCode;
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::CancelAllMonitors
// Cancels all monitors in list.
// -----------------------------------------------------------------------------
//
TInt CSatEventMonitorContainer::CancelAllMonitors()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::CancelAllMonitors calling" )

    const TInt monitorCount( iEventMonitors->Count() );
    LOG2( SIMPLE, "SATENGINE: CSatEventMonitorContainer::CancelAllMonitors \
          monitorCount: %i", monitorCount )
    // Cancel all monitors from list
    for ( TInt index = 0; index < monitorCount; index++ )
        {
        MSatEventMonitor* monitor = iEventMonitors->At( index );
        __ASSERT_ALWAYS( monitor, PanicSatEngine( ESatEngineNullPointer ) );
        // Cancel monitor
        monitor->CancelMonitor();
        }

    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::CancelAllMonitors exiting" )

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::LanguageSettingL
// Resolves language ID
// -----------------------------------------------------------------------------
//
void CSatEventMonitorContainer::LanguageSettingL( TInt& aLanguage )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::LanguageSettingL calling" )

    // Automatic Language setting is detected as 0.
    // There is no header file for this. This is only needed here.
    const TInt KAutomaticLanguageSetting( 0 );

    // Special treatment for automatic language setting.
    if ( KAutomaticLanguageSetting == aLanguage )
        {
        // Automatic language setting - Tries to take preferred
        // language from SIM card.

        // The following operations need a file server session.
        RFs fileServerSession;
        User::LeaveIfError( fileServerSession.Connect() );
        CleanupClosePushL( fileServerSession );

        // Automatic language setting
        // 1) look for SIM Preferred Language
        // 2) look if language is supported in phone
        //

        // Preferred language from SIM (Symbian coding).
        TInt simPreferredLanguageSymbianCoding;

        // Fetch SIM preferred language (Symbian coding) and see if it is
        // supported
        if ( KErrNone == SysLangUtil::RestoreSIMLanguage
                ( simPreferredLanguageSymbianCoding, &fileServerSession )
             && SysLangUtil::IsValidLanguage
                ( simPreferredLanguageSymbianCoding, &fileServerSession ) )
            {
            aLanguage = simPreferredLanguageSymbianCoding;
            LOG2( SIMPLE, "SATENGINE: CSatEventMonitorContainer::\
                LanguageSettingL SIM preferred language is supported %d",
                aLanguage )
            }
        else
            {
            // Either there is no SIM preferred language, or the SIM preferred
            // language is not supported. Then default language is used.
            SysLangUtil::GetDefaultLanguage( aLanguage, &fileServerSession );
            LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::\
                LanguageSettingL No SIM preferred language" )
            }

        CleanupStack::PopAndDestroy(); // fileServerSession
        }

    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::LanguageSettingL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::EventDownload
// Called when an event occurs. Handles that event and possibly sends
// event download to SIM.
// -----------------------------------------------------------------------------
//
void CSatEventMonitorContainer::EventDownload(
    RSat::TEventList aSingleEvent, TInt aFirstArg, TInt aSecArg )
    {
    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::EventDownload calling" )

    //lint -e{961} Else block meaningless
    if ( ( iBipExecuting || IsActive() ) && !iEventPending )
        {
        LOG( NORMAL, "EventMonitor: EventDownload BIP Command Executing" )
        // If there is event download currently active, we cannot send next
        // event download, so we have to store this event.
        iPendingEvent = aSingleEvent;
        iPendingFirstArg = aFirstArg;
        iPendingSecondArg = aSecArg;
        iEventPending = ETrue;
        }
    else if ( !iEventPending )
        {
        // No pending events, we can execute
        switch ( aSingleEvent )
            {
            case RSat::KLanguageSelection:
                {
                LOG( NORMAL, "SATENGINE:   Event LanguageSelection" )
                LanguageSelectionEvent( aFirstArg );
                break;
                }

            case RSat::KBrowserTermination:
                {
                LOG( NORMAL, "SATENGINE:   Event BrowserTermination" )
                BrowserTerminationEvent( aFirstArg );
                break;
                }

            case RSat::KDataAvailable:
                {
                LOG( NORMAL, "SATENGINE:   Event DataAvailable" )
                DataAvailableEvent( aFirstArg, aSecArg );
                break;
                }

            case RSat::KChannelStatus:
                {
                LOG( NORMAL, "SATENGINE:   Event ChannelStatus" )
                ChannelStatusEvent( aFirstArg, aSecArg );
                break;
                }

            // These events does not have any data to be sent
            // to SIM
            case RSat::KUserActivity:
            case RSat::KIdleScreenAvailable:
                {
                LOG( NORMAL, "SATENGINE:   Event UserActivity/IdleScreen" )
                RSat::TEventDownloadBaseV2 eventDownload;
                RSat::TEventDownloadBaseV2Pckg eventDownloadPckg(
                    eventDownload );
                eventDownload.iDataAvailable = EFalse;
                iSat.EventDownload( iStatus, aSingleEvent, eventDownloadPckg );
                iRemovableSingleEvent = aSingleEvent;
                break;
                }

            default:
                {
                LOG( NORMAL, "SATENGINE:   BadRequest" )
                __ASSERT_DEBUG( EFalse, PanicSatEngine( ESatSBadRequest ) );
                }
            }

        SetActive();
        }

    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::EventDownload exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::BipUtils
// Returns the interface for BIP Utils
// -----------------------------------------------------------------------------
//
MSatBIPEventNotifier& CSatEventMonitorContainer::BipNotifier()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::BipUtils calling-exiting" )
    return iSatUtils.BipEventNotifier();
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::SimRemoved
// Updates event monitor list
// -----------------------------------------------------------------------------
//
void CSatEventMonitorContainer::SimRemoved()
    {
    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::SimRemoved calling" )

    // If SIM is removed, all monitors must be cancelled.
    CancelAllMonitors();

    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::SimRemoved exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::GetLanguageString
// Gets the ISO639 language code.
// -----------------------------------------------------------------------------
//
void CSatEventMonitorContainer::GetLanguageString( const TInt aId, TDes& aText )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::GetLanguageString calling" )

    aText = KNullDesC;
    TBuf<KLangTextSize> lanText;
    lanText.Copy( KNullDesC );

    // Number of Symbian languages.
    TInt count( KNUMLANGSTRINGS );
    // Index for searching loop.
    TInt i( 0 );
    // Language structure for searching loop.
    SLangString language( KLangStrings[i] );

    // Loop languages until one is found
    do  {
        language = KLangStrings[i];
        // Finds the appropriate language and gives a code for that.
        if ( language.iId == aId )
            {
            LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::\
            GetLanguageString language.iId == aId" )
            // Current language code in the loop is appended to reference.
            lanText.Append(
                *( language.iString ) << 8 | *( language.iString + 1 ) );
            }
        i++;
        }
        while ( ( i <= count ) && ( language.iId != aId ) );

    aText.Copy( lanText );
    LOG2( SIMPLE,
    "SATENGINE: CSatEventMonitorContainer::GetLanguageString aId: %i", aId )
    LOG2( SIMPLE,
    "SATENGINE: CSatEventMonitorContainer::GetLanguageString aId: %S",
    &aText )
    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::GetLanguageString exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::RunL
// From CActive
// -----------------------------------------------------------------------------
//
void CSatEventMonitorContainer::RunL()
    {
    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::RunL calling" )

    if ( KErrCancel != iRemovableSingleEvent )
        {
        LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::RunL \
        KErrCancel != iRemovableSingleEvent" )
        //Remove event from list as these are not continuous events
        RemoveSingleEvent(
            static_cast<RSat::TEventList>( iRemovableSingleEvent ) );
        iRemovableSingleEvent = KErrCancel;
        }

    // Check the request status
    if ( KErrNone != iStatus.Int() )
        {
        LOG2( SIMPLE,
            "SATENGINE: CSatEventMonitorContainer::RunL with value %d",
            iStatus.Int() )
        }

    // If there are events pending, send now
    if ( iEventPending )
        {
        LOG( SIMPLE,
            "SATENGINE: CSatEventMonitorContainer::RunL Sending pending event" )
        iEventPending = EFalse;

        // Send event
        EventDownload( iPendingEvent, iPendingFirstArg, iPendingSecondArg );
        }

    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::DoCancel
// From CActive
// -----------------------------------------------------------------------------
//
void CSatEventMonitorContainer::DoCancel()
    {
    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::DoCancel calling" )

    CancelAllMonitors();

    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::CSatEventMonitorContainer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatEventMonitorContainer::CSatEventMonitorContainer( TUSatAPI& aUsatAPI,
    MSatUtils& aSatUtils ) :
    CActive ( CActive::EPriorityStandard ),
    iSat( aUsatAPI ),
    iSatUtils( aSatUtils )
    {
    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::\
        CSatEventMonitorContainer calling" )

    CActiveScheduler::Add( this );

    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::\
        CSatEventMonitorContainer exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatEventMonitorContainer::ConstructL()
    {
    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::ConstructL calling" )

    iEventMonitors = TSatEventMonitorFactory::CreateEventMonitorsL( *this );
    iRemovableSingleEvent = KErrCancel;

    iSatUtils.RegisterL( this, MSatUtils::EBipCommandExecuting );
    iSatUtils.RegisterL( this, MSatUtils::EBipCommandDoneExecuting );

    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::LanguageSelectionEvent
// Handles LanguageSelection event and sends event download
// -----------------------------------------------------------------------------
//
void CSatEventMonitorContainer::LanguageSelectionEvent( TInt aLanguageId )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::LanguageSelectionEvent calling" )

    // Special event download package,
    // used exclusively for Language Selection events
    RSat::TLanguageSelectionEventV2 languageDownload;
    RSat::TLanguageSelectionEventV2Pckg languageDownloadPckg(
        languageDownload );
    languageDownload.iDataAvailable = EFalse;

    // Resolve Language ID
    TRAPD( err, LanguageSettingL( aLanguageId ) );
    LOG2( SIMPLE, "SATENGINE: CSatEventMonitorContainer::\
          LanguageSelectionEvent err: %i", err )
    if ( KErrNone == err )
        {
        // Get Language string
        TBuf<1> languageTxt;
        GetLanguageString( aLanguageId, languageTxt );

        // If language is found, put it to event download information
        if ( languageTxt.Length() != 0 )
            {
            LOG( SIMPLE,
            "SATENGINE: CSatEventMonitorContainer::LanguageSelectionEvent \
            languageTxt.Length() != 0" )

            // Language selection event does have data.
            languageDownload.iDataAvailable = ETrue;

            // Integer argument is interpreted as language code.
            languageDownload.iLanguage =
                static_cast<TUint16>( languageTxt[0] );
            }
        }

    // Send EventDownload to SIM
    iSat.EventDownload(
        iStatus,
        RSat::KLanguageSelection,
        languageDownloadPckg );

    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::LanguageSelectionEvent exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::BrowserTerminationEvent
// Handles browser termination event and sends event download
// -----------------------------------------------------------------------------
//
void CSatEventMonitorContainer::BrowserTerminationEvent( TInt aCause )
    {
    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::\
        BrowserTerminationEvent calling" )

    // Special event download package,
    // used exclusively for Browser termination events.
    RSat::TBrowserTerminationEventV2 browserTermination;
    RSat::TBrowserTerminationEventV2Pckg browserTerminationPckg(
        browserTermination );

    // Language selection event does have data.
    browserTermination.iDataAvailable = ETrue;

    // Browser termination event does have data.
    browserTermination.iCause =
        static_cast<RSat::TBrowserTerminationCause>( aCause );

    // Send event download
    iSat.EventDownload(
        iStatus,
        RSat::KBrowserTermination,
        browserTerminationPckg );

    LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::\
        BrowserTerminationEvent exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::DataAvailableEvent
// Handles DataAvailable event and sends event download
// -----------------------------------------------------------------------------
//
void CSatEventMonitorContainer::DataAvailableEvent(
    const TInt aChannelId, const TInt aLength )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::DataAvailableEvent calling" )

    RSat::TDataAvailableEventV2 dataAvailable;
    RSat::TDataAvailableEventV2Pckg dataAvailablePckg( dataAvailable );
    RSat::TChannelStatus status;

    // BIP Utils will generate channel status
    TRAPD( err,
        status = iSatUtils.BipUtils().GenerateChannelStatusL( aChannelId, 0 ) );

    // Check error
    LOG2( SIMPLE, "SATENGINE: CSatEventMonitorContainer::\
            DataAvailableEvent ChannelStatus failed: %i", err )    
    if ( KErrNone == err )
        {
        // Put the parameters to data package
        dataAvailable.iDataAvailable = ETrue;
        // If received more than 0xFF, we return 0xFF, because that iLength can
        // contain only one byte.
        TInt8 length( 0 );

        if ( KSatBIPMoreThanMaxSize < aLength )
            {
            LOG( SIMPLE, "SATENGINE: CSatEventMonitorContainer::\
            DataAvailableEvent KSatBIPMoreThanMaxSize < aLength" )
            length = static_cast<TInt8>( KSatBIPMoreThanMaxSize );
            }
        else
            {
            length = static_cast<TInt8>( aLength );
            }

        dataAvailable.iLength = length;
        dataAvailable.iStatus = status;
        // Send EventDownload to SIM
        iSat.EventDownload(
            iStatus,
            RSat::KDataAvailable,
            dataAvailablePckg );
        }

    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::DataAvailableEvent exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::ChannelStatusEvent
// Handles ChannelStatus event and sends event download
// -----------------------------------------------------------------------------
//
void CSatEventMonitorContainer::ChannelStatusEvent( const TInt aChannelId,
    const TInt aStatus )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::ChannelStatusEvent calling" )

    RSat::TChannelStatusEventV2 channelStatus;
    RSat::TChannelStatusEventV2Pckg channelStatusPckg( channelStatus );
    RSat::TChannelStatus status;

    // BIP Utils will generate channel status
    TRAPD( err, status = iSatUtils.BipUtils().GenerateChannelStatusL(
        aChannelId, aStatus ) );

    // Check error
    LOG2( NORMAL, "SATENGINE: CSatEventMonitorContainer::\
            ChannelStatusEvent ChannelStatus failed: %i", err )    
    if ( KErrNone == err )
        {

        LOG2( SIMPLE, "SatServer, ChannelStatus envelope: %b", status[0] )
        // Put the parameters to data package
        channelStatus.iDataAvailable = ETrue;
        channelStatus.iStatus = status;
        // Send EventDownload to SIM
        iSat.EventDownload(
            iStatus,
            RSat::KChannelStatus,
            channelStatusPckg );
        }

    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::ChannelStatusEvent exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::EventsSupported
// Checks whether aEvents contains supported events only.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CSatEventMonitorContainer::EventsSupported( TUint aEvents ) const
    {
    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::EventsSupported calling" )

    // Events supported and handled by this class.
    const TUint eventsSupported =
        RSat::KUserActivity | RSat::KIdleScreenAvailable |
        RSat::KLanguageSelection | RSat::KBrowserTermination |
        RSat::KDataAvailable | RSat::KChannelStatus;

    // If aEvents has even one event that is not supported (not found from
    // eventsSuported), then EventsSupported returns EFalse.

    // First the eventsSupported is masked and bits are inverted. Then that mask
    // is compared to aEvents and if there is even one bit that is not supposed
    // to be there, function returns EFalse.

    const TBool supported( !( ~( eventsSupported ) & aEvents ) );

    LOG2( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::EventsSupported returning %i",
        supported )
    return supported;
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::WriteLanguageSelectionTime
// Sets time delay before reboot.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSatEventMonitorContainer::WriteLanguageSelectionTime( TInt aTime )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::WriteLanguageSelectionTime \
         calling-exiting" )
    return iSatUtils.SystemState().WriteLanguageSelectionTime( aTime );
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorContainer::RemoveSingleEvent
// Removes single event from list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatEventMonitorContainer::RemoveSingleEvent(
    RSat::TEventList aSingleEvent )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::RemoveSingleEvent calling" )

    const TInt monitorCount( iEventMonitors->Count() );
    LOG2( SIMPLE, "SATENGINE: CSatEventMonitorContainer::RemoveSingleEvent \
          monitorCount: %i", monitorCount )

    // Cancel all monitors from list
    for ( TInt index = 0; index < monitorCount; index++ )
        {
        MSatEventMonitor* monitor = iEventMonitors->At( index );
        __ASSERT_ALWAYS( monitor, PanicSatEngine( ESatEngineNullPointer ) );

        if ( monitor->IsMonitoringEvent( aSingleEvent ) )
            {
            // Cancel monitor
            monitor->CancelMonitor();
            }
        }

    LOG( SIMPLE,
        "SATENGINE: CSatEventMonitorContainer::RemoveSingleEvent exiting" )
    }

//  End of File
