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
* Description:  Handles SimSessionEnd command
*
*/



// INCLUDE FILES
#include    "MSatApi.h"
#include    "MSatUiSession.h"
#include    "MSatSUiClientHandler.h"
#include    "CSimSessionEndHandler.h"
#include    "SatLog.h"

// CONSTANTS
const TInt KSimSessionEndDelay = 60000000;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSimSessionEndHandler::CSimSessionEndHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSimSessionEndHandler::CSimSessionEndHandler() :
    CSatCommandHandler()
    {
    LOG( SIMPLE, "SIMSESSIONEND: \
        CSimSessionEndHandler::CSimSessionEndHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CSimSessionEndHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSimSessionEndHandler::ConstructL()
    {
    LOG( SIMPLE, "SIMSESSIONEND: CSimSessionEndHandler::ConstructL calling" )

    iTimer = CPeriodic::NewL( EPriorityStandard );

    iUtils->RegisterL( this, MSatUtils::EDelaySimSessionEnd );
    iUtils->RegisterL( this, MSatUtils::EDestroySimSessionEndTimer );
    iUtils->RegisterL( this, MSatUtils::ESessionTerminatedByUser );
    iUtils->RegisterL( this, MSatUtils::ESimSessionEndCallBack );
    iUtils->RegisterL( this, MSatUtils::ESustainedTextInDisplay );
    iUtils->RegisterL( this, MSatUtils::ESustainedTextRemoved );
    iUtils->RegisterL( this, MSatUtils::ESatUiLaunched );

    LOG( SIMPLE, "SIMSESSIONEND: CSimSessionEndHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSimSessionEndHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSimSessionEndHandler* CSimSessionEndHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "SIMSESSIONEND: CSimSessionEndHandler::NewL calling" )

    CSimSessionEndHandler* self = new( ELeave ) CSimSessionEndHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SIMSESSIONEND: CSimSessionEndHandler::NewL exiting" )
    return self;
    }


// Destructor
CSimSessionEndHandler::~CSimSessionEndHandler()
    {
    LOG( SIMPLE,
        "SIMSESSIONEND: CSimSessionEndHandler::~CSimSessionEndHandler calling" )

    Cancel();

    if ( iTimer )
        {
        iTimer->Cancel();
        }
    delete iTimer;

    LOG( SIMPLE,
        "SIMSESSIONEND: CSimSessionEndHandler::~CSimSessionEndHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSimSessionEndHandler::TimerCallback
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSimSessionEndHandler::TimerCallback( TAny* aSimSessionEndHandler )
    {
    LOG( SIMPLE, "SIMSESSIONEND: CSimSessionEndHandler::TimerCallback calling" )

    CSimSessionEndHandler* handler =
        static_cast<CSimSessionEndHandler*>( aSimSessionEndHandler );
        
    if ( handler )
        {
        LOG( SIMPLE, 
        "SIMSESSIONEND: CSimSessionEndHandler::TimerCallback handler true" )
        handler->iTimer->Cancel();
        handler->HandleCommand();
        }

    LOG( SIMPLE, "SIMSESSIONEND: CSimSessionEndHandler::TimerCallback exiting" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSimSessionEndHandler::Event
// Waits for indication of user wanting to close the sim session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSimSessionEndHandler::Event( TInt aEvent )
    {
    LOG( SIMPLE, "SIMSESSIONEND: CSimSessionEndHandler::Event calling" )

    switch ( aEvent )
        {
        case MSatUtils::ESatUiLaunched:
            {
            LOG( NORMAL, 
            "SIMSESSIONEND: CSimSessionEndHandler::Event UI is launched" )
            // New UI session, reset flag
            iSimSessionTerminatedByUser = EFalse;
            break;
            }

        case MSatUtils::ESessionTerminatedByUser:
            {
            LOG( NORMAL, 
            "SIMSESSIONEND: CSimSessionEndHandler::Event \
            ESessionTerminatedByUser" )
            // Next SimSessionEnd will close the ui session.
            iSimSessionTerminatedByUser = ETrue;
            break;
            }

        case MSatUtils::EDelaySimSessionEnd:
            {
            LOG( NORMAL, 
            "SIMSESSIONEND: CSimSessionEndHandler::Event \
            EDelaySimSessionEnd" )
            iTimer->Cancel();
            TCallBack callback( TimerCallback, this );
            iTimer->Start( KSimSessionEndDelay, KSimSessionEndDelay, callback );
            break;
            }

        case MSatUtils::EDestroySimSessionEndTimer:
            {
            LOG( NORMAL, 
            "SIMSESSIONEND: CSimSessionEndHandler::Event \
            EDestroySimSessionEndTimer" )
            iTimer->Cancel();
            break;
            }

        case MSatUtils::ESimSessionEndCallBack:
            {
            LOG( NORMAL, 
            "SIMSESSIONEND: CSimSessionEndHandler::Event \
            ESimSessionEndCallBack" )
            // Forced Sim session end.
            iTimer->Cancel();
            HandleCommand();
            break;
            }

        case MSatUtils::ESustainedTextInDisplay:
            {
            LOG( NORMAL, "SIMSESSIONEND: CSimSessionEndHandler::Event \
            ESustainedTextInDisplay" )
            // Indication of sustained text
            iSustainedTextActive = ETrue;
            break;
            }

        case MSatUtils::ESustainedTextRemoved:
            {
            LOG( NORMAL, "SIMSESSIONEND: CSimSessionEndHandler::Event \
            ESustainedTextRemoved" )
            // Indication of sustained text cleared
            iSustainedTextActive = EFalse;
            // If last sim session end is pending because of sustained text, 
            // execute it now
            if ( iSimSessionEndPending )
                {
                LOG( NORMAL, 
                "SIMSESSIONEND: CSimSessionEndHandler::Event \
                iSimSessionEndPending" )
                HandleCommand();
                }

            break;
            }

        default:
            {
            LOG2( NORMAL, 
            "SIMSESSIONEND: CSimSessionEndHandler::Event \
            Unexpected event: %i", aEvent )
            CSatCommandHandler::Event( aEvent );
            break;
            }
        }

    LOG( SIMPLE, "SIMSESSIONEND: CSimSessionEndHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// CSimSessionEndHandler::DoCancel
// Cancels the sat request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSimSessionEndHandler::DoCancel()
    {
    LOG( SIMPLE, "SIMSESSIONEND: CSimSessionEndHandler::DoCancel calling" )

    iUtils->USatAPI().NotifySimSessionEndCancel();

    LOG( SIMPLE, "SIMSESSIONEND: CSimSessionEndHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSimSessionEndHandler::IssueUSATRequest
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSimSessionEndHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE,
        "SIMSESSIONEND: CSimSessionEndHandler::IssueUSATRequest calling" )

    iUtils->USatAPI().NotifySimSessionEnd( aStatus );

    LOG( SIMPLE,
        "SIMSESSIONEND: CSimSessionEndHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// CSimSessionEndHandler::CommandAllowed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CSimSessionEndHandler::CommandAllowed()
    {
    LOG( SIMPLE, "SIMSESSIONEND: \
        CSimSessionEndHandler::CommandAllowed calling - exiting" )
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CSimSessionEndHandler::NeedUiSession
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CSimSessionEndHandler::NeedUiSession()
    {
    LOG( SIMPLE, "SIMSESSIONEND: \
        CSimSessionEndHandler::NeedUiSession calling - exiting" )
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSimSessionEndHandler::HandleCommand
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSimSessionEndHandler::HandleCommand()
    {
    LOG( SIMPLE, "SIMSESSIONEND: CSimSessionEndHandler::HandleCommand calling" )

    // Note that the framework has destroyed the simsession
    // end timer, when HandleCommand is called.
    if ( !iSimSessionTerminatedByUser && !iSustainedTextActive )
        {
        LOG( SIMPLE, 
        "SIMSESSIONEND: CSimSessionEndHandler::HandleCommand \
        ESimSessionEndExecuting" )
        iUtils->NotifyEvent( MSatUtils::ESimSessionEndExecuting );
        }

    MSatUiSession* uiSession = iUtils->SatUiHandler().UiSession();
    if ( uiSession )
        {
        // If sustained Display text is on screen, cannot close UI
        if ( !iSustainedTextActive && ( iSimSessionTerminatedByUser ||
            !iUtils->SatUiHandler().UiLaunchedByUser() ) )
            {
            iSimSessionTerminatedByUser = EFalse;
            iSimSessionEndPending = EFalse;
            // Number of executing command handlers should be 0.
            TInt expectedCommCount = 0;

            // If SimSessionEnd is executing ie. not active, it has to take
            // into account when checking executing command handlers.
            if ( !IsActive() )
                {
                LOG( SIMPLE, 
                "SIMSESSIONEND: CSimSessionEndHandler::HandleCommand \
                isn't Active" )
                expectedCommCount = 1;
                }

            LOG2( NORMAL, "SIMSESSIONEND: Expected commands: %i",
                expectedCommCount )
            // SimSession end is currently executing, therefore there is one
            // executing command.
            if ( expectedCommCount ==
                 iUtils->NumberOfExecutingCommandHandlers() )
                {
                LOG( SIMPLE, 
                "SIMSESSIONEND: CSimSessionEndHandler::HandleCommand \
                expectedCommCount==iUtils->NumberOfExecutingCommandHandlers()" )
                // Close the ui session only if there are no
                // executing commands.
                uiSession->CloseUiSession();
                }
            }
        else
            {
            if ( iSustainedTextActive )
                {
                LOG( SIMPLE, 
                "SIMSESSIONEND: CSimSessionEndHandler::HandleCommand \
                iSustainedTextActive true" )
                iSimSessionEndPending = ETrue;
                }
            }
        }

    // Renew the request, because the sim session end does not send
    // terminal response, which would renew the request automatically.
    Start();

    LOG( SIMPLE, "SIMSESSIONEND: CSimSessionEndHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// CSimSessionEndHandler::UiLaunchFailed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSimSessionEndHandler::UiLaunchFailed()
    {
    // This command does not launch ui client.
    LOG( SIMPLE,
        "SIMSESSIONEND: CSimSessionEndHandler::UiLaunchFailed calling-exiting" )
    }

//  End of File
