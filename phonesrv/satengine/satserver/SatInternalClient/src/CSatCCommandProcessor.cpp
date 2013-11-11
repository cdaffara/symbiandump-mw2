/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the client command processor class
*                that is  responsible for the active objects needed to
*                maintain that connection.
*
*/



// INCLUDE FILES
#include    <e32svr.h>
#include    "CSatCCommandProcessor.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatCCommandProcessor::CSatCCommandProcessor
// C++ default constructor cannot contain any code that
// might leave.
// -----------------------------------------------------------------------------
//
CSatCCommandProcessor::CSatCCommandProcessor(
    RSatUiSession* aSession ) :
    iSession( aSession )
    {
    }

// -----------------------------------------------------------------------------
// CSatCCommandProcessor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatCCommandProcessor::ConstructL()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCCommandProcessor::ConstructL calling" )

    // All the command handlers are constructed here.
    iDisplayTextHandler = CSatCDisplayTextHandler::NewL( iSession );
    iGetInkeyHandler = CSatCGetInkeyHandler::NewL( iSession );
    iGetInputHandler = CSatCGetInputHandler::NewL( iSession );
    iSetUpMenuHandler = CSatCSetUpMenuHandler::NewL( iSession );
    iSelectItemHandler = CSatCSelectItemHandler::NewL( iSession );
    iPlayToneHandler = CSatCPlayToneHandler::NewL( iSession );
    iActiveCommandHandler = CSatCActiveCommandHandler::NewL( iSession );
    iEventHandler = CSatCEventHandler::NewL( iSession );
    iQueryHandler = CSatCQueryHandler::NewL( iSession );
    iNotifyHandler = CSatCNotifyHandler::NewL( iSession );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCCommandProcessor::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCCommandProcessor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatCCommandProcessor* CSatCCommandProcessor::NewL(
    RSatUiSession* aSession )
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCCommandProcessor::NewL calling" )

    // Perform two-phase construction.
    CSatCCommandProcessor* self =
        new ( ELeave ) CSatCCommandProcessor( aSession );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCCommandProcessor::NewL exiting" )
    return self;
    }

// Destructor
CSatCCommandProcessor::~CSatCCommandProcessor()
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCCommandProcessor::~CSatCCommandProcessor calling" )

    // All the event handlers are deleted here.
    delete iDisplayTextHandler;
    delete iGetInkeyHandler;
    delete iGetInputHandler;
    delete iSetUpMenuHandler;
    delete iSelectItemHandler;
    delete iPlayToneHandler;
    delete iActiveCommandHandler;
    delete iEventHandler;
    delete iQueryHandler;
    delete iNotifyHandler;

    iSession = NULL;

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCCommandProcessor::~CSatCCommandProcessor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCCommandProcessor::Start
// Starts the active objects
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCCommandProcessor::Start()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCCommandProcessor::Start calling" )

    // Start all the event handlers
    iDisplayTextHandler->Start();
    iGetInkeyHandler->Start();
    iGetInputHandler->Start();
    iSetUpMenuHandler->Start();
    iSelectItemHandler->Start();
    iPlayToneHandler->Start();
    iQueryHandler->Start();
    iNotifyHandler->Start();
    iEventHandler->Start();

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCCommandProcessor::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCCommandProcessor::SetUpMenuHandler
// Returns a pointer to a Set Up menu command handler object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CSatCSetUpMenuHandler* CSatCCommandProcessor::SetUpMenuHandler()
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCCommandProcessor::SetUpMenuHandler calling-exiting" )
    return iSetUpMenuHandler;
    }

// -----------------------------------------------------------------------------
// CSatCCommandProcessor::ActiveCommandHandler
// Returns a pointer to a active command handler object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CSatCActiveCommandHandler* CSatCCommandProcessor::ActiveCommandHandler()
    {
    LOG( SIMPLE,
    "SATINTERNALCLIENT: CSatCCommandProcessor::ActiveCommandHandler calling-exiting" )
    return iActiveCommandHandler;
    }

//  End of File
