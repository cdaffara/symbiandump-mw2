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
* Description:  This is the handler for the SIM Application Toolkit
*                active commands.
*
*/



// INCLUDE FILES
#include    <e32base.h>
#include    <e32svr.h>
#include    "RSatUiSession.h"
#include    "MSatUiAdapter.h"
#include    "CSatCActiveCommandHandler.h"
#include    "CSatCCommandProcessor.h"
#include    "SatSOpcodes.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatCActiveCommandHandler::CSatCActiveCommandHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatCActiveCommandHandler::CSatCActiveCommandHandler(
    RSatUiSession* aSession ) :
    iSession( aSession ),
    iMenuSelectionData(),
    iMenuSelectionPckg( iMenuSelectionData )
    {
    }

// -----------------------------------------------------------------------------
// CSatCActiveCommandHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatCActiveCommandHandler* CSatCActiveCommandHandler::NewL(
    RSatUiSession* aSat )
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCActiveCommandHandler::NewL calling" )

    // Perform construction
    CSatCActiveCommandHandler* self =
        new ( ELeave ) CSatCActiveCommandHandler( aSat );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCActiveCommandHandler::NewL exiting" )
    return self;
    }

// Destructor
CSatCActiveCommandHandler::~CSatCActiveCommandHandler()
    {
    LOG( SIMPLE, 
    "SATINTERNALCLIENT: CSatCActiveCommandHandler::~CSatCActiveCommandHandler calling" )
    iSession = NULL;
    LOG( SIMPLE, 
    "SATINTERNALCLIENT: CSatCActiveCommandHandler::~CSatCActiveCommandHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCActiveCommandHandler::MenuSelection
// Sends the Menu Selection Active command
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCActiveCommandHandler::MenuSelection(
    TInt aMenuItem,
    TBool aHelpRequested )
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCActiveCommandHandler::MenuSelection calling" )

    // Empty the IPC data
    RSat::TMenuSelectionV1 menuSelection;
    iMenuSelectionData = menuSelection;

    // Map the index into correct item ID.
    RSat::TItem item;
    iSession->CommandProcessor()->SetUpMenuHandler()->
        OldSetUpMenuData().GetItem( static_cast<TUint>( aMenuItem + 1 ), item );

    // Set the item ID of the selected item.
    iMenuSelectionData.iItemId = item.iItemId;

    // Set the help request identifier.
    iMenuSelectionData.iHelp = RSat::EHelpNotRequested;
    if ( aHelpRequested )
        {
        LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCActiveCommandHandler::MenuSelection aHelpRequested" )
        iMenuSelectionData.iHelp = RSat::EHelpRequested;
        }

    // Perform a synchronous service request.
    TIpcArgs arguments( &iMenuSelectionPckg );
    iSession->CreateRequest( ESatSActiveMenuSelection, arguments );

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCActiveCommandHandler::MenuSelection exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCActiveCommandHandler::SessionTerminated
// Sends the Session Terminated Active command
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCActiveCommandHandler::SessionTerminated( TInt8 aType )
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCActiveCommandHandler::SessionTerminated calling" )
    TSatTermination termination;
    //lint -e{603} termination used through terminationPckg.
    TSatTerminationPckg terminationPckg ( termination );

    switch ( aType )
        {
        case ESessionCancel:
            {
            termination.type = ETerminatedByCancel;
            TIpcArgs arguments( &terminationPckg );
            iSession->CreateRequest( ESatSSessionTerminated, arguments );
            LOG( SIMPLE, "SATINTERNALCLIENT:   Terminated by cancel" )
            break;
            }

        case EEndKeyUsed:
            {
            termination.type = ETerminatedByEndKey;
            TIpcArgs arguments( &terminationPckg );
            iSession->CreateRequest( ESatSSessionTerminated, arguments );
            LOG( SIMPLE, "SATINTERNALCLIENT:   Terminated by end key" )
            break;
            }

        default:
            {
            LOG( SIMPLE, "SATINTERNALCLIENT:   Not terminated" )
            break;
            }
        }

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCActiveCommandHandler::SessionTerminated exiting" )
    }

//  End of File
