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
* Description:  Base class for handling SAT commands.
*
*/



// INCLUDE FILES
#include    <ecom.h>
#include    "MSatSystemState.h"
#include    "CSatCommandHandler.h"
#include    "MSatUtils.h"
#include    "MSatApi.h"
#include    "EnginePanic.h"
#include    "MSatSUiClientHandler.h"
#include    "SatLog.h"

// Small delay for giving UI time to launch before starting to handle
// pending command
const TInt KTimerDelay =  100000;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatCommandHandler::CSatCommandHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CSatCommandHandler::CSatCommandHandler() :
    CActive( EPriorityStandard )
    {
    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::CSatCommandHandler calling" )

    CActiveScheduler::Add( this );

    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::CSatCommandHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandHandler::BaseConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
EXPORT_C void CSatCommandHandler::BaseConstructL( MSatUtils* aPtr )
    {
    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::BaseConstructL calling" )

    __ASSERT_ALWAYS( aPtr, PanicSatEngine( ESatEngineNullPointer ) );
    iUtils = aPtr;

    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::BaseConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatCommandHandler* CSatCommandHandler::NewL(
    const TUid& aImplUid,
    MSatUtils* aUtils )
    {
    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::NewL calling" )

    __ASSERT_ALWAYS( aUtils, PanicSatEngine( ESatEngineNullPointer ) );

    TAny* ptr = REComSession::CreateImplementationL( aImplUid,
        _FOFF( CSatCommandHandler, iDtorIDKey ), aUtils );

    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::NewL exiting" )
    return reinterpret_cast<CSatCommandHandler*>( ptr );
    }

// Destructor
EXPORT_C CSatCommandHandler::~CSatCommandHandler()
    {
    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::~CSatCommandHandler calling" )

    iUtils = NULL;
    REComSession::DestroyedImplementation( iDtorIDKey );

    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::~CSatCommandHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandHandler::Start
// Starts the waiting of SAT command, if not already active.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSatCommandHandler::Start()
    {
    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::Start calling" )

    if ( !IsActive() )
        {
        LOG( DETAILED, "SATENGINE: CSatCommandHandler::Start set active" )
        // Child class issues the request.
        IssueUSATRequest( iStatus );
        SetActive();

        // Not executing
        iIsExecuting = EFalse;
        iWaitingUiLaunch = EFalse;
        }

    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandHandler::DoHandleCommand
// Handles the SAT command, which has been notified from USAT api.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSatCommandHandler::DoHandleCommand()
    {
    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::DoHandleCommand calling" )

    if ( CommandAllowed() )
        {
        iUtils->NotifyEvent( MSatUtils::EDestroySimSessionEndTimer );
        if ( NeedUiSession() )
            {
            // If ui session is not availabe, SATUI has to be
            // launched.
            if ( NULL == iUtils->SatUiHandler().UiSession() )
                {
                LOG( SIMPLE, "SATENGINE: CSatCommandHandler::DoHandleCommand \
                     uisession not available" )
                TRAPD( err, iUtils->RegisterL(
                    this, MSatUtils::ESatUiLaunched );

                    // Try to launch the ui client.
                    iUtils->SatUiHandler().LaunchSatUiL();
                    ); // End of TRAPD

                if ( KErrNone != err )
                    {
                    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::\
                    DoHandleCommand KErrNone != err" )
                    // Ui launch failed, unregister event
                    iUtils->UnregisterEvent( this, MSatUtils::ESatUiLaunched );

                    // Notify derived class that ui launch failed.
                    UiLaunchFailed();
                    }
                else 
                    { 
                    LOG( SIMPLE,  
                    "SATENGINE: CSatCommandHandler::DoHandleCommand iWaitingUiLaunch")
                    iWaitingUiLaunch = ETrue; 
                    }
                }
            else
                {
                LOG( SIMPLE, "SATENGINE: CSatCommandHandler::DoHandleCommand \
                     uisession available" )
                // client session is available handle the command.
                HandleCommand();
                }
            }
        else
            {
            LOG( SIMPLE, "SATENGINE: CSatCommandHandler::DoHandleCommand \
                 don't need uisession" )
            // Command does not need ui client, so handle the command.
            HandleCommand();
            }
        }

    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::DoHandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandHandler::ClientResponse
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSatCommandHandler::ClientResponse()
    {
    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::ClientResponse panic" )
    PanicSatEngine( ESatUnexpectedHandleUiResponseCall );
    }

// -----------------------------------------------------------------------------
// CSatCommandHandler::Event
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSatCommandHandler::Event( TInt aEvent )
    {
    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::Event calling" )

    if ( MSatUtils::ESatUiLaunched == aEvent )
        {
        iUtils->UnregisterEvent( this, MSatUtils::ESatUiLaunched );
        // Check is command still waiting UI to start
        if ( !IsActive() )
            {
            LOG( SIMPLE, "SATENGINE: CSatCommandHandler::Event waiting UI" )
            // Start timer to give UI some time to execute
            iWaitingUiLaunch = ETrue;
            iDelayTimer.CreateLocal();
            iDelayTimer.After( iStatus, KTimerDelay );
            SetActive();
            }
        else
            {
            LOG( SIMPLE, "SATENGINE: CSatCommandHandler::Event close UI" )
            // UI is launched but command is not executing. UI must be closed.
            iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
            }
        }

    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandHandler::TerminalRsp
// Sends the terminal response to the SIM and renews the sat request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSatCommandHandler::TerminalRsp(
    RSat::TPCmd aPCmd, // Identifies the command sending terminal rsp.
    const TDesC8& aRsp ) // Data package of the response
    {
    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::TerminalRsp calling" )

    iUtils->NotifyEvent( MSatUtils::EDelaySimSessionEnd );
    iUtils->USatAPI().TerminalRsp( aPCmd, aRsp );
    Start();

    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::TerminalRsp exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandHandler::RunL
// If request is completed without error, starts handling the command.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSatCommandHandler::RunL()
    {
    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::RunL calling" )
    LOG2( SIMPLE,
            "SATENGINE:   RunL status: %d", iStatus.Int() )
    if ( iWaitingUiLaunch )
        {
        LOG( NORMAL, "  UI is launched. Starting to handle command" )
        iWaitingUiLaunch = EFalse;
        iDelayTimer.Close();
        HandleCommand();
        }
    else if ( KErrNone == iStatus.Int() )
        {
        // Executing only, if really executing
        iIsExecuting = ETrue;
        DoHandleCommand();
        }
    else if ( KErrNotSupported != iStatus.Int() )
        {        
        Start();
        }
    else
        {
        LOG( SIMPLE, "SATENGINE:   Request was unsupported" )
        }

    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCommandHandler::IsPhoneInIdleState
// Returns ETrue if using Dummy TSY
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSatCommandHandler::IsPhoneInIdleState()
    {
    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::IsPhoneInIdleState calling" )
    TBool isIdle( EFalse );

    TRAPD( err, ( isIdle = iUtils->SystemState().IsPhoneInIdleStateL() ) );

    if ( KErrNone != err )
        {
        LOG( SIMPLE, 
        "SATENGINE: CSatCommandHandler::IsPhoneInIdleState KErrNone != err" )
        isIdle = EFalse;
        }

    LOG2( SIMPLE, "SATENGINE: CSatCommandHandler::IsPhoneInIdleState \
          exiting, isIdle: %d", isIdle )
    return isIdle;
    }

// -----------------------------------------------------------------------------
// CSatCommandHandler::Panic
// Gives a panic.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSatCommandHandler::Panic(
    const TDesC& aCategory, TInt aReason ) const
    {
    LOG( SIMPLE, "SATENGINE: CSatCommandHandler::Panic" )
    User::Panic( aCategory, aReason );
    }

// -----------------------------------------------------------------------------
// CSatCommandHandler::IsCommandExecuting
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSatCommandHandler::IsCommandExecuting() const
    {
    LOG( DETAILED, "SATENGINE: CSatCommandHandler::IsCommandExecuting" )
    return iIsExecuting;
    }

//  End of File
