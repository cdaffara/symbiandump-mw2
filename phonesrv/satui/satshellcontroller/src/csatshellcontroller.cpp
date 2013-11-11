/*
* Copyright (c) 2003-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*     This file contains the SAT Shell Controller class that is responsible
*     for the actual implementation of the following functionality:
*       - add SAT UI application to Desk (or similar)
*       - remove SAT UI application from Desk (or similar)
*       - bring SAT UI application to foreground if it already is up
*           and running
*       - launching the SAT UI application
*       - launching the Browser
*       - resolving default access point
*       - bring Browser to the foreground
*
*
*/


//  INCLUDE FILES
#include <w32std.h>             // RWsSession
#include <apaid.h>              // TApaAppInfo
#include <apacmdln.h>           // CApaCommandLine
#include <apgcli.h>             // RApaLsSession
#include <apgtask.h>            // TApaTaskList
#include <apgwgnam.h>           // CApaWindowGroupName
#include <MenuSatInterface.h>   // CMenuSATInterface
#include <DocumentHandler.h>    // KWmlcHandler
#include "csatshellcontroller.h"

// Browser Cen Rep Keys.
#ifdef __SERIES60_NATIVE_BROWSER
//    #include <BrowserUiSDKCRKeys.h> Commented out to allow sat server boot TB10.1 
//    #include <BrowserUiInternalCRKeys.h>
#endif // __SERIES60_NATIVE_BROWSER
#include "tflogger.h"

// CONSTANTS
const TUid KUidSatUi = { 0x101f4ce0 };
_LIT( KFour, "4" );                     // Browser parameter.
_LIT( KFive, "5" );                     // Browser parameter.
_LIT( KSpace, " " );                    // Used as Browser parameter.
static const TUid KEmptyUid = { KErrNotFound };
static const TUid KUidBrowser = { KWmlcHandler };
const TInt KTimerTime( 2000000 );       // 2 seconds.
const TInt KBrowserParamAndTwoSpaces( 3 );
#ifndef __SERIES60_NATIVE_BROWSER
    const TUid KCRUidBrowser = { 0x10008D39 };
    const TUint32 KBrowserDefaultAccessPoint( 0x0000000E );
#endif // __SERIES60_NATIVE_BROWSER

// ================= MEMBER FUNCTIONS =======================================

// C++ default constructor can NOT contain any code that
// might leave.
//
CSatShellController::CSatShellController():
    iUidWmlBrowser( TUid::Uid( KWmlcHandler ))
    {
    TFLOGSTRING( "CSatShellController::CSatShellController called-exit" )
    }

// Symbian OS constructor
void CSatShellController::ConstructL()
    {
    TFLOGSTRING( "CSatShellController::ConstructL called" )

    // Timer is used to get the Browser application to the foreground.
    iTimer = CPeriodic::NewL( EPriorityNormal );

    TFLOGSTRING( "CSatShellController::ConstructL exit" )
    }

// Two-phased constructor.
CSatShellController* CSatShellController::NewL()
    {
    TFLOGSTRING( "CSatShellController::NewL called" )

    CSatShellController* self = new ( ELeave ) CSatShellController;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    TFLOGSTRING( "CSatShellController::NewL exit" )
    return self;
    }


// Destructor
CSatShellController::~CSatShellController()
    {
    TFLOGSTRING( "CSatShellController::~CSatShellController called" )

    // Frees resources.
    if ( iTimer )
        {
        iTimer->Cancel();
        }
    delete iTimer;

    TFLOGSTRING( "CSatShellController::~CSatShellController exit" )
    }

// ---------------------------------------------------------
// CSatShellController::AddSatUiL
// Adds SAT UI Client application to phone's
// Application Shell (or similar).
// ---------------------------------------------------------
//
void CSatShellController::AddSatUiL(
    TDesC& aName )
    {
    TFLOGSTRING( "CSatShellController::AddSatUiL(name) called" )

    iSimAppName = aName;

    CMenuSATInterface* menu = new ( ELeave ) CMenuSATInterface;
    menu->MakeSatUiVisible( ETrue, aName );
    delete menu;

    TFLOGSTRING( "CSatShellController::AddSatUiL exit" )
    }

//------------------------------------------------------------
// CSatShellController::AddSatUiL
// Adds SAT UI Client application to phone's
// Application Shell (or similar).
// ---------------------------------------------------------
//
void CSatShellController::AddSatUiL(
    TDesC& aName, TUint8 aIconId )
    {
    TFLOGSTRING( "CSatShellController::AddSatUiL(name,icon) called" )

    iSimAppName = aName;

    CMenuSATInterface* menu = new ( ELeave ) CMenuSATInterface;
    menu->MakeSatUiVisible( ETrue, aName, aIconId );
    delete menu;

    TFLOGSTRING( "CSatShellController::AddSatUiL exit" )
    }

// ---------------------------------------------------------
// CSatShellController::RemoveSatUiL
// Removes SAT UI Client application from phone's
// Application Shell (or similar).
// ---------------------------------------------------------
//

void CSatShellController::RemoveSatUiL()
    {
    TFLOGSTRING( "CSatShellController::RemoveSatUiL called" )

    CMenuSATInterface* menu = new ( ELeave ) CMenuSATInterface;
    menu->MakeSatUiVisible( EFalse, iSimAppName );
    delete menu;

    TFLOGSTRING( "CSatShellController::RemoveSatUiL exit" )
    }

// ---------------------------------------------------------
// CSatShellController::LaunchSatUiL
// Launches SAT UI Client application.
// ---------------------------------------------------------
//
void CSatShellController::LaunchSatUiL()
    {
    TFLOGSTRING( "CSatShellController::LaunchSatUiL called" )

    RApaLsSession rapaLsSession;
    User::LeaveIfError( rapaLsSession.Connect() );
    CleanupClosePushL( rapaLsSession );
    TThreadId id( static_cast<TInt64>( 0 ) );
    TApaAppInfo appInfo;

    User::LeaveIfError( rapaLsSession.GetAppInfo( appInfo, KUidSatUi ) );
    CApaCommandLine* cmdLine = CApaCommandLine::NewLC();
    cmdLine->SetExecutableNameL( appInfo.iFullName );
    cmdLine->SetCommandL( EApaCommandOpen );
    rapaLsSession.StartApp( *cmdLine, id );

    CleanupStack::PopAndDestroy( cmdLine );
    CleanupStack::PopAndDestroy( &rapaLsSession );

    TFLOGSTRING( "CSatShellController::LaunchSatUiL exit" )
    }


// ---------------------------------------------------------
// CSatShellController::BringBrowserToForeground
// Brings the Browser to foreground.
// ---------------------------------------------------------
//
void CSatShellController::BringBrowserToForeground() const
    {
    TFLOGSTRING( "CSatShellController::BringBrowserToForeground called" )

    BringApplicationToForeground( iUidWmlBrowser );

    TFLOGSTRING( "CSatShellController::BringBrowserToForeground exit" )
    }

// ---------------------------------------------------------
// CSatShellController::BringBrowserToForegroundAfterPeriod
// Brings the Browser to foreground after time has passed.
// ---------------------------------------------------------
//
void CSatShellController::BringBrowserToForegroundAfterPeriod()
    {
    TFLOGSTRING(
        "CSatShellController::BringBrowserToForegroundAfterPeriod called" )

    if ( !iTimer->IsActive() )
        {
        TFLOGSTRING(
            "CSatShellController::BringBrowserToForegroundAfterPeriod \
             start iTimer" )
        iTimer->Start( KTimerTime,
                       KTimerTime,
                       TCallBack( TimerCompleted, this ) );
        }

    TFLOGSTRING(
        "CSatShellController::BringBrowserToForegroundAfterPeriod exit" )
    }

// ---------------------------------------------------------
// CSatShellController::BringSatUiToForeground
// Brings the SAT UI application to foreground.
// ---------------------------------------------------------
//
void CSatShellController::BringSatUiToForeground()
    {
    TFLOGSTRING( "CSatShellController::BringSatUiToForeground called" )

    CheckSatUiStatus();
    BringApplicationToForeground( KUidSatUi );

    TFLOGSTRING( "CSatShellController::BringSatUiToForeground exit" )
    }

// ---------------------------------------------------------
// CSatShellController::BringApplicationToForeground
// Brings the SAT UI application to foreground.
// ---------------------------------------------------------
//
void CSatShellController::BringApplicationToForeground(
    const TUid& aAppUid ) const
    {
    TFLOGSTRING( "CSatShellController::BringApplicationToForeground called" )

    RWsSession wsSession;
    // Open the WS server session.
    if ( KErrNone == wsSession.Connect() )
        {
        TFLOGSTRING( "CSatShellController::BringApplicationToForeground \
            open WS server session" )
        TApaTaskList tasklist( wsSession );
        // Find the task with uid
        TApaTask task = tasklist.FindApp( aAppUid );
        task.BringToForeground();
        wsSession.Close();
        }

    TFLOGSTRING( "CSatShellController::BringApplicationToForeground exit" )
    }

// ---------------------------------------------------------
// CSatShellController::SetSatUiToBackground
// Sets SAT UI Application to background if needed.
// ---------------------------------------------------------
//
void CSatShellController::SetSatUiToBackground() const
    {
    TFLOGSTRING( "CSatShellController::SetSatUiToBackground called" )
    
    if ( iSetSatUiToBackground )
        {
        RWsSession wsSession;
        // Open the WS server session.
        if ( KErrNone == wsSession.Connect() )
            {
            TFLOGSTRING( "CSatShellController::SetSatUiToBackground \
                open WS server session" )
            TApaTaskList tasklist( wsSession );
            // Find the task with uid
            TApaTask task = tasklist.FindApp( KUidSatUi );
            task.SendToBackground();
            wsSession.Close();
            }            
        }
                    
    TFLOGSTRING( "CSatShellController::SetSatUiToBackground exit" )                    
    }

// ---------------------------------------------------------
// CSatShellController::CycleSatUiBackwards
// Brings the SAT UI application to foreground.
// ---------------------------------------------------------
//
void CSatShellController::CycleSatUiBackwards()
    {
    TFLOGSTRING( "CSatShellController::BringSatUiToForeground called" )

    RWsSession wsSession;
    // Open the WS server session.
    if ( KErrNone == wsSession.Connect() )
        {
        TFLOGSTRING( "CSatShellController::CycleSatUiBackwards \
            open WS server session" )
        TApaTaskList taskList( wsSession );
        // Finds topmost application.
        TApaTask task = taskList.FindByPos(0);
        iTimer->Cancel();

        CApaWindowGroupName* name = 0;
        TRAPD( err,
            name = CApaWindowGroupName::NewL( wsSession, task.WgId() ) );

        if ( name )
            {
            // Check is that Browser is foremost application.
            if ( !err && ( name->AppUid() == KUidBrowser ) )
                {
                TFLOGSTRING(
                    "CSatShellController::BSUTF Browser is in foreground" )
                }
            else
                {
                TFLOGSTRING(
                    "CSatShellController::BSUTF Browser isn't in foreground" )
                TApaTask browserTask = taskList.FindApp( KUidBrowser );
                browserTask.BringToForeground();
                iTimer->Start(
                    KTimerTime, KTimerTime, TCallBack( TimerCompleted, this ) );
                }

            delete name;
            }

        wsSession.Close();
        }

    TFLOGSTRING( "CSatShellController::BringSatUiToForeground exit" )
    }

// ---------------------------------------------------------
// CSatShellController::TimerCompleted
// Callback function. Completes after predefined
// time has passed
// ---------------------------------------------------------
//
TInt CSatShellController::TimerCompleted( TAny* aObject )
    {
    TFLOGSTRING( "CSatShellController::TimerCompleted called" )

    if ( aObject )
        {
        STATIC_CAST( CSatShellController*, aObject )->CycleSatUiBackwards();
        }

    TFLOGSTRING( "CSatShellController::TimerCompleted exit" )
    return KErrNone;
    }

// ---------------------------------------------------------
// CSatShellController::LaunchBrowserL
// Launch XHTML-browser
// Browser launched with parameter
// "4" + "<Space>" + "<Url>" + "<Space>" + "<UID of App>" or "5"
// More information see Browser API Specification Document
// ---------------------------------------------------------
//
TInt CSatShellController::LaunchBrowserL( const TDesC& aUrl,
     TUid aAccessPointUid )
    {
    TFLOGSTRING( "CSatShellController::LaunchBrowserL called" )

    TInt errorCode( KErrNone );
    HBufC* param = HBufC::NewLC( aUrl.Length() + KMaxUidName +
        KBrowserParamAndTwoSpaces );
    
    TFLOGSTRING2( "CSatShellController::LaunchBrowserL length of aUrl: %d", \
        aUrl.Length() )
    if ( aUrl.Length() > 0 )
        {
        param->Des().Copy( KFour );
        param->Des().Append( KSpace );
        param->Des().Append( aUrl );
        TFLOGSTRING2( "CSatShellController::LaunchBrowserL \
                      aAccessPointUid: %d", aAccessPointUid )
        // Create script for Browser if not empty accesspoint UID.
        if ( aAccessPointUid != KEmptyUid )
            {
            TFLOGSTRING( "CSatShellController::LaunchBrowserL not empty \
                accesspoint" )
            // Uid is in decimal format
            param->Des().Append( KSpace );
            param->Des().AppendNum( aAccessPointUid.iUid );
            }

        }
    else
        {
        param->Des().Copy( KFive );
        }

    User::LeaveIfError( iWsSession.Connect() );

    TFLOGSTRING2( "CSatShellController::LaunchBrowserL \
                   param string: %S", param )
    TApaTaskList taskList( iWsSession );
    TApaTask task = taskList.FindApp( iUidWmlBrowser );

    // If browser already open.
    if ( task.Exists() )
        {
        TFLOGSTRING( "CSatShellController::LaunchBrowserL browser open" )
        HBufC8* param8 = HBufC8::NewLC( param->Length() );
        param8->Des().Append( *param );
        errorCode = task.SendMessage( TUid::Uid( 0 ), param8->Des() );
        CleanupStack::PopAndDestroy( param8 );
        }
    else
        {
        TFLOGSTRING( "CSatShellController::LaunchBrowserL launch browser" )
        // Launch the Browser.
        TThreadId id( static_cast<TInt64>( 0 ) );
        RApaLsSession rapaLsSession;
        User::LeaveIfError( rapaLsSession.Connect() );
        errorCode = rapaLsSession.StartDocument( *param, iUidWmlBrowser, id );
        rapaLsSession.Close();
        }

    BringBrowserToForeground();

    iWsSession.Close();
    CleanupStack::PopAndDestroy( param );

    TFLOGSTRING2( "CSatShellController::LaunchBrowserL exit %d", errorCode )
    return errorCode;
    }

// -----------------------------------------------------------------------------
// CSatShellController::ResolveDefaultAccesspoint
// Resolves Browser's default accesspoint.
// -----------------------------------------------------------------------------
//
TInt CSatShellController::ResolveDefaultAccesspoint()
    {
    TFLOGSTRING( "CSatShellController::ResolveDefaultAccesspoint called" )

    // Initialize with error value for wrong access point.
    TInt apInt( KErrNotFound );
/* Commented out to enable SAT Server boot,
add TB10.1 implementation later

    TInt err( KErrNone );
    CRepository* repository = NULL;

    // Which UID to monitor.
    TRAP( err, repository = CRepository::NewL( KCRUidBrowser ) );

    if ( ( KErrNone == err  ) && repository )
        {
        err = repository->StartTransaction(
            CRepository::EReadWriteTransaction );
        err = repository->Get( KBrowserDefaultAccessPoint, apInt );
        TFLOGSTRING2( "CSatShellController::ResolveDefaultAccesspoint \
        KBrowserDefaultAccessPoint: %d", apInt )
        }

    delete repository;
    repository = NULL;
*/
    TFLOGSTRING( "CSatShellController::ResolveDefaultAccesspoint exit" )
    return apInt;
    }

// -----------------------------------------------------------------------------
// CSatShellController::CheckSatUiStatus
// Check if SatUi is needed to set to background.
// -----------------------------------------------------------------------------
//
void CSatShellController::CheckSatUiStatus()
    {
    TFLOGSTRING( "CSatShellController::CheckSatUiStatus called" )
    
    // if SatUi task is found and SatUi is not in foreground flag is set ETrue
    iSetSatUiToBackground = EFalse;

    RWsSession wsSession;
    // Open the WS server session.
    if ( KErrNone == wsSession.Connect() )
        {
        TFLOGSTRING( "CSatShellController::CheckSatUiStatus \
            open WS server session" )
        TApaTaskList tasklist( wsSession );
        TApaTask satUiTask = tasklist.FindApp( KUidSatUi );
        
        if ( satUiTask.Exists() )
            {
            TFLOGSTRING(
                 "CSatShellController::CheckSatUiStatus task exists" )
            TApaTask topMostTask = tasklist.FindByPos( 0 );
            
            if ( topMostTask.ThreadId() != satUiTask.ThreadId() )
                {
                TFLOGSTRING(
                    "CSatShellController::CheckSatUiStatus set SatUI to BG" )
                iSetSatUiToBackground = ETrue;                    
                }
            }
            
        wsSession.Close();
        }
        
    TFLOGSTRING( "CSatShellController::CheckSatUiStatus exit" )
    }
    
// ================= OTHER EXPORTED FUNCTIONS ===============================

EXPORT_C MSatShellController* NewSatController()
    {
    TFLOGSTRING( "CSatShellController::NewSatController called" )

    MSatShellController* satController = NULL;
    TRAPD( err, satController = CSatShellController::NewL() )
    if ( KErrNone != err )
        {
        TFLOGSTRING2( "   CSatShellController::NewSatController \
            failed: %d", err )
        satController = NULL;
        }

    TFLOGSTRING( "CSatShellController::NewSatController exit" )
    return satController;
    }

//  End of File
