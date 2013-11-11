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
*                Select Item proactive command.
*
*/


#include    <e32svr.h>
#include    <basched.h>
#include    "RSatUiSession.h"
#include    "MSatUiObserver.h"
#include    "SatSOpcodes.h"
#include    "CSatCSelectItemHandler.h"
#include    "SatLog.h"

const TInt8 KMenuItemPop( 3 );

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatCSelectItemHandler::CSatCSelectItemHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatCSelectItemHandler::CSatCSelectItemHandler(
    TInt aPriority,
    RSatUiSession* aSession ) :
    CActive( aPriority ),
    iSession( aSession ),
    iSelectItemData(),
    iSelectItemPckg( iSelectItemData ),
    iSelectItemRsp(),
    iSelectItemRspPckg( iSelectItemRsp )
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCSelectItemHandler::CSatCSelectItemHandler calling" )

    // Add to active scheduler.
    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCSelectItemHandler::CSatCSelectItemHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCSelectItemHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatCSelectItemHandler* CSatCSelectItemHandler::NewL(
    RSatUiSession* aSat )
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCSelectItemHandler::NewL calling" )

    // Perform the construction.
    CSatCSelectItemHandler* self =
        new ( ELeave ) CSatCSelectItemHandler( EPriorityLow, aSat );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCSelectItemHandler::NewL exiting" )
    return self;
    }

// Destructor
CSatCSelectItemHandler::~CSatCSelectItemHandler()
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCSelectItemHandler::~CSatCSelectItemHandler calling" )

    // Cancel any outstanding requests.
    Cancel();
    iSession = NULL;

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCSelectItemHandler::~CSatCSelectItemHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCSelectItemHandler::Start
// Starts the handler.
// -----------------------------------------------------------------------------
//
void CSatCSelectItemHandler::Start()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCSelectItemHandler::Start calling" )

    // Empty the IPC data
    RSat::TSelectItemV2 temp;
    iSelectItemData = temp;
    RSat::TSelectItemRspV1 temp2;
    iSelectItemRsp = temp2;

    // Request Select Item notification.
    TIpcArgs arguments( &iSelectItemPckg );

    // Pass the Select Item IPC package.
    iSession->CreateRequest( ESatSProactiveSelectItem, arguments, iStatus );

    // Set this handler to active so that it can receive requests.
    SetActive();

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCSelectItemHandler::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCSelectItemHandler::RunL
// Handles the command.
// -----------------------------------------------------------------------------
//
void CSatCSelectItemHandler::RunL()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCSelectItemHandler::RunL calling" )

    // Check the status of the asnychronous operation
    if ( KErrNone != iStatus.Int() )
        {
        LOG2(
            SIMPLE,
            "SATINTERNALCLIENT: CSatCSelectItemHandler::RunL error: %d",
            iStatus.Int() )

        // Renew the request
        Start();
        }
    else
        {
        // Create the menu item array.
        CDesC16ArrayFlat* menuItems = new( ELeave ) CDesC16ArrayFlat( 1 );
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
        // This is the default menu item.
        TInt defaultItem( 0 );
        TBool defaultItemIdFound( EFalse );

        const TInt numberOfItems( static_cast<TInt>(
            iSelectItemData.NumberOfItems() ) );
            
        LOG2( SIMPLE,
        "SATINTERNALCLIENT: CSatCSelectItemHandler::RunL numberOfItems: %d",
        numberOfItems )
        for ( TInt currentItem = 1; currentItem <= numberOfItems; currentItem++ )
            {
            RSat::TItem item;
            TInt action;
            TInt icon( KErrNotFound );

            // Fetch all data for this index.
            iSelectItemData.GetItem( static_cast<TUint>( currentItem ),
                                     item, action, icon );

            if ( item.iItemId == iSelectItemData.iDefaultItemId )
                {
                LOG( SIMPLE, 
                "SATINTERNALCLIENT: CSatCSelectItemHandler::RunL has defaultItemId" )
                defaultItem = currentItem - 1;
                defaultItemIdFound = ETrue;
                }

            // Save the item name
            menuItems->AppendL( item.iItemString );

            // Save icon identifier.
            // icon will be KErrNotFound (-1) if it is not present in the
            // SAT menu.
            menuIcons->AppendL( icon );
            LOG2( SIMPLE,
            "SATINTERNALCLIENT: CSatCSelectItemHandler::RunL action: %d", action )
            if ( KErrNotFound != action )
                {
                // Save the next action indicator, if found.
                menuActions->AppendL( STATIC_CAST( TSatAction, action ) );
                }
            }

        // Do not return partial comprehension response when
        // default item is not set.
        if ( !iSelectItemData.iDefaultItemId )
            {
            LOG( SIMPLE, 
            "SATINTERNALCLIENT: CSatCSelectItemHandler::RunL \
            iSelectItemData.iDefaultItemId false" )
            defaultItemIdFound = ETrue;
            }

        // This will contain the user selection.
        TUint8 selection;

        // Indicates whether help is available
        TBool helpIsAvailable( EFalse );
        if ( iSelectItemData.iHelp == RSat::EHelpAvailable )
            {
            LOG( SIMPLE, 
            "SATINTERNALCLIENT: CSatCSelectItemHandler::RunL EHelpAvailable" )
            helpIsAvailable = ETrue;
            }

        // Has to be casted to TInt before casting to TSatIconQualifier,
        // because gcc warns about the direct cast.
        const struct TSatIconId iconId = { iSelectItemData.iIconId.iIdentifier,
            static_cast<TSatIconQualifier>(
                static_cast<TInt>( iSelectItemData.iIconId.iQualifier ) ) };

        const enum TSatIconQualifier iconListQualifier(
            static_cast<TSatIconQualifier>(
                static_cast<TInt>( iSelectItemData.iIconListQualifier ) ) );

        const enum TSatSelectionPreference selectionPreference(
            static_cast<TSatSelectionPreference>(
                static_cast<TInt>( iSelectItemData.iPreference ) ) );

        // This will contain EFalse if requested icon is not displayed.
        TBool requestedIconDisplayed( ETrue );

        // About lint e64:
        // Error in lint was MDesC8Array = CDesC16ArrayFlat, which
        // is not the case.
        // Notify the registered client and save the response.
        //lint -e{64}
        TSatUiResponse response = iSession->SatUiObserver()->SelectItemL(
            iSelectItemData.iAlphaId.iAlphaId,
            *menuItems,
            menuActions,
            defaultItem,
            selection,
            helpIsAvailable,
            iconId,
            menuIcons,
            iconListQualifier,
            requestedIconDisplayed,
            selectionPreference );

        // By default, this command does not have additional information.
        iSelectItemRsp.iInfoType = RSat::KNoAdditionalInfo;
        iSelectItemRsp.iAdditionalInfo.Zero();

        // must use the same pcmd
        iSelectItemRsp.SetPCmdNumber( iSelectItemData.PCmdNumber() );

        // Examine the client response.
        ExamineClientResponse(
            response, selection, requestedIconDisplayed, defaultItemIdFound );

        // Pass the Select Item response package.
        TIpcArgs arguments( &iSelectItemRspPckg );

        // Perform the IPC data transfer.
        iSession->CreateRequest( ESatSProactiveSelectItemResponse, arguments );

        // Destroy the array objects: menuIcons, menuActions, menuItems.
        CleanupStack::PopAndDestroy( KMenuItemPop, menuItems );

        // Renew the service request.
        Start();
        }

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCSelectItemHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCSelectItemHandler::ExamineClientResponse
// Examine the client response.
// -----------------------------------------------------------------------------
//
void CSatCSelectItemHandler::ExamineClientResponse(
    TSatUiResponse aResponse,
    TUint8 aSelection,
    TBool aRequestedIconDisplayed,
    TBool aDefaultItemIdFound )
    {
    LOG2( SIMPLE,
    "SATINTERNALCLIENT: CSatCSelectItemHandler::ExamineClientResponse calling,\
    aResponse: %x", aResponse )

    // Examine the client response.
    switch ( aResponse )
        {
        case ESatSuccess:
            {
            // Convert terminal rsp if icon used
            RSat::TPCmdResult result( RSat::KSuccess );
            RSat::TIconQualifier iconQualifier(
                    iSelectItemData.iIconId.iQualifier );

            //lint -e{961} Else block meaningless
            if ( iSelectItemData.iDefaultItemId && !aDefaultItemIdFound )
                {
                LOG( SIMPLE,
                "SATINTERNALCLIENT: CSatCSelectItemHandler::ExamineClientResponse \
                KPartialComprehension" )
                result = RSat::KPartialComprehension;
                }
            else if ( !aRequestedIconDisplayed )
                {
                LOG( SIMPLE,
                "SATINTERNALCLIENT: CSatCSelectItemHandler::ExamineClientResponse \
                aRequestedIconDisplayed false" )
                if ( iconQualifier == RSat::ESelfExplanatory ||
                     iconQualifier == RSat::ENotSelfExplanatory )
                    {
                    LOG( SIMPLE,
                    "SATINTERNALCLIENT: CSatCSelectItemHandler::ExamineClientResponse \
                    IconNotDisplayed" )
                    result = RSat::KSuccessRequestedIconNotDisplayed;
                    }
                }

            iSelectItemRsp.iGeneralResult = result;

            // Change the additional information type.
            iSelectItemRsp.iInfoType = RSat::KItemIdentifier;

            // Search for the correct item ID.
            // Underflow not possible due to selection
            // being of type TUint8
            if ( aSelection > iSelectItemData.NumberOfItems() - 1 )
                {
                LOG( SIMPLE,
                "SATINTERNALCLIENT: CSatCSelectItemHandler::ExamineClientResponse \
                ESatSuccess ESelectedItemOutOfRange" )
                iSession->Panic( ESelectedItemOutOfRange );
                }

            // Get the item with the desired index.
            RSat::TItem item;
            iSelectItemData.GetItem( aSelection + 1, item );

            // Save the index of the item selected by the user.
            iSelectItemRsp.iAdditionalInfo.SetLength( 1 );
            iSelectItemRsp.iAdditionalInfo[0] = item.iItemId;
            break;
            }
        // Otherwise, just return the response.
        case ESatFailure:
            {
            iSelectItemRsp.iInfoType = RSat::KMeProblem;
            iSelectItemRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iSelectItemRsp.iAdditionalInfo.SetLength( 1 );
            iSelectItemRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
            break;
            }
        case ESatSessionTerminatedByUser:
            {
            iSelectItemRsp.iGeneralResult = RSat::KPSessionTerminatedByUser;
            break;
            }
        case ESatBackwardModeRequestedByUser:
            {
            iSelectItemRsp.iGeneralResult =
                RSat::KBackwardModeRequestedByUser;
            break;
            }
        case ESatNoResponseFromUser:
            {
            iSelectItemRsp.iGeneralResult = RSat::KNoResponseFromUser;
            break;
            }
        case EHelpRequestedByUser:
            {
            iSelectItemRsp.iGeneralResult = RSat::KHelpRequestedByUser;

            // Change the additional information type.
            iSelectItemRsp.iInfoType = RSat::KItemIdentifier;

            // Search for the correct item ID.
            // Underflow not possible due to selection
            // being of type TUint8
            if ( aSelection > iSelectItemData.NumberOfItems() - 1 )
                {
                LOG( SIMPLE,
                "SATINTERNALCLIENT: CSatCSelectItemHandler::ExamineClientResponse \
                EHelpRequestedByUser ESelectedItemOutOfRange" )
                iSession->Panic( ESelectedItemOutOfRange );
                }

            // Get the item with the desired index.
            RSat::TItem item;
            iSelectItemData.GetItem( aSelection + 1, item );

            // Save the index of the item selected by the user.
            iSelectItemRsp.iAdditionalInfo.SetLength( 1 );
            iSelectItemRsp.iAdditionalInfo[0] = item.iItemId;
            break;
            }
        case EPCmdNotAcceptedByUser:
        case ESatCmdDataNotUnderstood:
        default:
            {
            iSession->Panic( ESatInvalidResponse );
            break;
            }
        }

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCSelectItemHandler::ExamineClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCSelectItemHandler::DoCancel
// Cancels the pending request.
// -----------------------------------------------------------------------------
//
void CSatCSelectItemHandler::DoCancel()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCSelectItemHandler::DoCancel calling" )

    // Complete the request with cancel code.
    TRequestStatus* requestStatus = &iStatus;
    User::RequestComplete( requestStatus, KErrCancel );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCSelectItemHandler::DoCancel exiting" )
    }
