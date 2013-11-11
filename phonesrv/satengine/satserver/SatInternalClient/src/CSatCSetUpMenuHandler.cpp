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
*                Set Up Menu proactive command.
*
*/



// INCLUDE FILES
#include    <e32base.h>
#include    <e32svr.h>
#include    "RSatUiSession.h"
#include    "MSatUiObserver.h"
#include    "SatSOpcodes.h"
#include    "CSatCSetUpMenuHandler.h"
#include    "SatLog.h"

// CONSTANTS
const TInt8 KMenuItemPop( 3 );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatCSetUpMenuHandler::CSatCSetUpMenuHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatCSetUpMenuHandler::CSatCSetUpMenuHandler(
    TInt aPriority,
    RSatUiSession* aSession ) :
    CActive( aPriority ),
    iSession( aSession ),
    iSetUpMenuData(),
    iSetUpMenuPckg( iSetUpMenuData ),
    iSetUpMenuRsp(),
    iSetUpMenuRspPckg( iSetUpMenuRsp )
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCSetUpMenuHandler::CSatCSetUpMenuHandler calling" )

    // Add to active scheduler.
    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCSetUpMenuHandler::CSatCSetUpMenuHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCSetUpMenuHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatCSetUpMenuHandler* CSatCSetUpMenuHandler::NewL(
    RSatUiSession* aSat )
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCSetUpMenuHandler::NewL calling" )

    // Perform the construction.
    CSatCSetUpMenuHandler* self =
        new ( ELeave ) CSatCSetUpMenuHandler( EPriorityLow, aSat );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCSetUpMenuHandler::NewL exiting" )
    return self;
    }

// Destructor
CSatCSetUpMenuHandler::~CSatCSetUpMenuHandler()
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCSetUpMenuHandler::~CSatCSetUpMenuHandler calling" )

    // Cancel any outstanding requests.
    Cancel();
    iSession = NULL;

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCSetUpMenuHandler::~CSatCSetUpMenuHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCSetUpMenuHandler::Start
// Starts the handler.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

void CSatCSetUpMenuHandler::Start()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCSetUpMenuHandler::Start calling" )

    // Empty the IPC data
    RSat::TSetUpMenuV2 temp;
    iSetUpMenuData = temp;
    RSat::TSetUpMenuRspV1 temp2;
    iSetUpMenuRsp = temp2;

    // Request Set Up Menu notification.
    TIpcArgs arguments( &iSetUpMenuPckg );

    // Pass the Set Up Menu IPC package.
    iSession->CreateRequest( ESatSProactiveSetUpMenu, arguments, iStatus );

    // Set this handler to active so that it can receive requests.
    SetActive();

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCSetUpMenuHandler::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCSetUpMenuHandler::RunL
// Handles the command.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCSetUpMenuHandler::RunL()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCSetUpMenuHandler::RunL calling" )

    // Check the status of the asnychronous operation
    if ( KErrNone != iStatus.Int() )
        {
        LOG2(
            SIMPLE,
            "SATINTERNALCLIENT: CSatCSetUpMenuHandler::RunL exiting, error: %d",
            iStatus.Int() )
        }
    else 
        {
        // Save the contents of the data for envelope Menu Selection.
        iOldSetUpMenuData = iSetUpMenuData;

        // Create the menu item array.
        CDesCArrayFlat* menuItems = new( ELeave ) CDesCArrayFlat( 1 );
        CleanupStack::PushL( menuItems );

        // Create the next action indicator array.
        CArrayFixFlat<TSatAction>* menuActions =
            new( ELeave ) CArrayFixFlat<TSatAction>( 1 );
        CleanupStack::PushL( menuActions );

        // Create the menu item icon array.
        CArrayFixFlat<TInt>* menuIcons =
            new( ELeave ) CArrayFixFlat<TInt>( 1 );
        CleanupStack::PushL( menuIcons );

        // Fetch all the menu items.
        // Note that the indexing starts from 1 in SIM item lists.
        const TInt menuItemCount(
            static_cast<TInt>( iSetUpMenuData.NumberOfItems() ) );
        LOG2( SIMPLE,
        "SATINTERNALCLIENT: CSatCSetUpMenuHandler::RunL menuItemCount: %d",
        menuItemCount )
        for ( TInt currentItem = 1;
              currentItem <= menuItemCount;
              currentItem++ )
            {
            RSat::TItem item;
            TInt action;
            TInt icon( KErrNotFound );

            // Fetch all data for this index.
            iSetUpMenuData.GetItem( static_cast<TUint>( currentItem ),
                                    item,
                                    action,
                                    icon );

            // Save the item name
            menuItems->AppendL( item.iItemString );

            // Save icon identifier.
            // icon will be KErrNotFound (-1) if it is not present in the
            // SAT menu.
            menuIcons->AppendL( icon );
            
            if ( KErrNotFound != action )
                {
                LOG2( SIMPLE,
                "SATINTERNALCLIENT: CSatCSetUpMenuHandler::RunL action: %d", action )
                // Save the next action indicator, if found.
                menuActions->AppendL( STATIC_CAST( TSatAction, action ) );
                }
            }

        // Indicates whether help is available
        TBool helpIsAvailable( EFalse );
        if ( RSat::EHelpAvailable == iSetUpMenuData.iHelp )
            {
            LOG( SIMPLE, 
            "SATINTERNALCLIENT: CSatCSetUpMenuHandler::RunL EHelpAvailable" )
            helpIsAvailable = ETrue;
            }

        // Has to be casted to TInt before casting to TSatIconQualifier,
        // because gcc warns about the direct cast.
        const struct TSatIconId iconId = { iSetUpMenuData.iIconId.iIdentifier,
            static_cast<TSatIconQualifier>(
                static_cast<TInt>( iSetUpMenuData.iIconId.iQualifier ) ) };

        const enum TSatIconQualifier iconListQualifier(
            static_cast<TSatIconQualifier>(
                static_cast<TInt>( iSetUpMenuData.iIconListQualifier ) ) );

        const enum TSatSelectionPreference selectionPreference(
            static_cast<TSatSelectionPreference>(
                static_cast<TInt>( iSetUpMenuData.iPreference ) ) );

        // Notify the registered client and save the response.
        TSatUiResponse response(
            iSession->SatUiObserver()->SetUpMenuL(
            iSetUpMenuData.iAlphaId.iAlphaId,
            *menuItems,
            menuActions,
            helpIsAvailable,
            iconId,
            menuIcons,
            iconListQualifier,
            selectionPreference ) );
            
        LOG2( SIMPLE,
        "SATINTERNALCLIENT: CSatCSetUpMenuHandler::RunL response: %x", response )
        // Examine the client response.
        switch ( response )
            {
            case ESatSuccess:
                {
                break;
                }
            case ESatFailure:
                {
                iSession->Panic( ESatSetUpMenuFailure );
                break;
                }
            case ESatSessionTerminatedByUser:
            case ESatBackwardModeRequestedByUser:
            case ESatNoResponseFromUser:
            case EHelpRequestedByUser:
            case EPCmdNotAcceptedByUser:
            case ESatCmdDataNotUnderstood:
            default:
                {
                iSession->Panic( ESatInvalidResponse );
                break;
                }
            }

        // Destroy the array objects: menuIcons, menuActions, menuItems.
        CleanupStack::PopAndDestroy( KMenuItemPop, menuItems );
        }

    // Renew the service request.
    Start();

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCSetUpMenuHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCSetUpMenuHandler::DoCancel
// Cancels the pending request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCSetUpMenuHandler::DoCancel()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCSetUpMenuHandler::DoCancel calling" )

    // Complete the request with cancel code.
    TRequestStatus* requestStatus = &iStatus;
    User::RequestComplete( requestStatus, KErrCancel );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCSetUpMenuHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCSetUpMenuHandler::OldSetUpMenuData
// Returns a pointer to a old the command data object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
RSat::TSetUpMenuV2 CSatCSetUpMenuHandler::OldSetUpMenuData() const
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCSetUpMenuHandler::OldSetUpMenuData calling-exiting" )
    return iOldSetUpMenuData;
    }

//  End of File
