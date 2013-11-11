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
* Description:  ClearScreen handler is responsible for clearing screen when
*                DisplayText has sent immediate response and it is still active
*                when any other command starts to execute.
*
*/



// INCLUDE FILES
#include    "MSatUtils.h"
#include    "SatSTypes.h"
#include    "CClearScreenHandler.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CClearScreenHandler::CClearScreenHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CClearScreenHandler::CClearScreenHandler( MSatUtils& aUtils ) :
    iUtils( aUtils )
    {
    LOG( SIMPLE, "DISPLAYTEXT: \
        CClearScreenHandler::CClearScreenHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CClearScreenHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CClearScreenHandler::ConstructL()
    {
    LOG( SIMPLE, "DISPLAYTEXT: CClearScreenHandler::ConstructL calling" )

    // Set immediate response to false.
    iImmediateResponse = EFalse;

    // List all events we should listen.
    iUtils.RegisterL( this, MSatUtils::ESetUpMenuExecuting );
    iUtils.RegisterL( this, MSatUtils::ESelectItemExecuting );
    iUtils.RegisterL( this, MSatUtils::ESetUpCallExecuting );
    iUtils.RegisterL( this, MSatUtils::EDisplayTextExecuting );
    iUtils.RegisterL( this, MSatUtils::ELaunchBrowserExecuting );
    iUtils.RegisterL( this, MSatUtils::EGetInputExecuting );
    iUtils.RegisterL( this, MSatUtils::EGetInkeyExecuting );
    iUtils.RegisterL( this, MSatUtils::ESendDtmfExecuting );
    iUtils.RegisterL( this, MSatUtils::ESendSsExecuting );
    iUtils.RegisterL( this, MSatUtils::ESendUssdExecuting );
    iUtils.RegisterL( this, MSatUtils::EPlayToneExecuting );
    iUtils.RegisterL( this, MSatUtils::ERefreshExecuting );
    iUtils.RegisterL( this, MSatUtils::ECallControlExecuting );
    iUtils.RegisterL( this, MSatUtils::ESendSmExecuting );
    iUtils.RegisterL( this, MSatUtils::EMoSmControlExecuting );

    LOG( SIMPLE, "DISPLAYTEXT: CClearScreenHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CClearScreenHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CClearScreenHandler* CClearScreenHandler::NewL( MSatUtils& aUtils )
    {
    LOG( SIMPLE, "DISPLAYTEXT: CClearScreenHandler::NewL calling" )

    CClearScreenHandler* self =
        new ( ELeave ) CClearScreenHandler( aUtils );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "DISPLAYTEXT: CClearScreenHandler::NewL exiting" )
    return self;
    }


// Destructor
CClearScreenHandler::~CClearScreenHandler()
    {
    LOG( SIMPLE,
        "DISPLAYTEXT: CClearScreenHandler::~CClearScreenHandler calling" )

    // Unregister this as an event observer
    iUtils.UnregisterEventObserver( this );

    LOG( SIMPLE,
        "DISPLAYTEXT: CClearScreenHandler::~CClearScreenHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CClearScreenHandler::UpdateImmediateState
// Updates immediate response state.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CClearScreenHandler::UpdateImmediateState( TBool aState )
    {
    LOG( SIMPLE,
        "DISPLAYTEXT: CClearScreenHandler::UpdateImmediateState calling" )

    iImmediateResponse = aState;

    LOG( SIMPLE,
        "DISPLAYTEXT: CClearScreenHandler::UpdateImmediateState exiting" )
    }

// -----------------------------------------------------------------------------
// CClearScreenHandler::Event
// Notification of an event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CClearScreenHandler::Event( TInt aEvent )
    {
    LOG2( SIMPLE, "DISPLAYTEXT: CClearScreenHandler::Event calling, \
          aEvent: %d", aEvent )
    LOG2( SIMPLE, "DISPLAYTEXT: CClearScreenHandler::Event, \
          iImmediateResponse: %d", iImmediateResponse )
    // We don't have to do anything if immediate response is not active.
    if ( iImmediateResponse )
        {
        switch ( aEvent )
            {
            case MSatUtils::ESetUpMenuExecuting:
            case MSatUtils::ESelectItemExecuting:
            case MSatUtils::ESetUpCallExecuting:
            case MSatUtils::EDisplayTextExecuting:
            case MSatUtils::ELaunchBrowserExecuting:
            case MSatUtils::EGetInputExecuting:
            case MSatUtils::EGetInkeyExecuting:
            case MSatUtils::ESendDtmfExecuting:
            case MSatUtils::ESendSsExecuting:
            case MSatUtils::ESendUssdExecuting:
            case MSatUtils::EPlayToneExecuting:
            case MSatUtils::ERefreshExecuting:
            case MSatUtils::ECallControlExecuting:
            case MSatUtils::ESendSmExecuting:
            case MSatUtils::EMoSmControlExecuting:
                {
                ClearScreen();
                break;
                }

            default:
                {
                // Do nothing.
                break;
                }
            }
        }

    LOG( SIMPLE, "DISPLAYTEXT: CClearScreenHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// CClearScreenHandler::ClearScreen
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CClearScreenHandler::ClearScreen()
    {
    LOG( SIMPLE, "DISPLAYTEXT: CClearScreenHandler::ClearScreen calling" )
    LOG2( SIMPLE, "DISPLAYTEXT: CClearScreenHandler::ClearScreen, \
          iImmediateResponse: %d", iImmediateResponse )
    // Clear the screen if immediate response is active.
    //lint -e{961} Else block meaningless
    if ( iImmediateResponse )
        {
        iUtils.NotifyUiEvent(
            ESatSClearScreenEvent, ESatEventNone, KErrNone );
        iImmediateResponse = EFalse;

        iUtils.NotifyEvent( MSatUtils::ESustainedTextRemoved );
        }

    LOG( SIMPLE, "DISPLAYTEXT: CClearScreenHandler::ClearScreen exiting" )
    }

//  End of File
