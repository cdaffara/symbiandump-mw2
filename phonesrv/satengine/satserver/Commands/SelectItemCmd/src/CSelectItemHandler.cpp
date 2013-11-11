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
* Description:  Handles SelectItem command
*
*/

#include    <etelsat.h>
#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "CSelectItemHandler.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSelectItemHandler::CSelectItemHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSelectItemHandler::CSelectItemHandler() :
    CSatCommandHandler(),
    iSelectItemData(),
    iSelectItemPckg( iSelectItemData ),
    iSelectItemRsp(),
    iSelectItemRspPckg( iSelectItemRsp ),
    // To be removed when icons are allowed in this command
    iIconCommand( EFalse )
    {
    LOG( SIMPLE,
        "SELECTITEM: CSelectItemHandler::CSelectItemHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CSelectItemHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSelectItemHandler::ConstructL()
    {
    LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::ConstructL calling" )

    // Register service request handler for SelectItem command.
    iUtils->RegisterServiceRequestL(
        ESatSProactiveSelectItem,
        ESatSProactiveSelectItemResponse,
        this );

    LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSelectItemHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSelectItemHandler* CSelectItemHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::NewL calling" )
    CSelectItemHandler* self = new( ELeave ) CSelectItemHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::NewL exiting" )
    return self;
    }


// Destructor
CSelectItemHandler::~CSelectItemHandler()
    {
    LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::~CSelectItemHandler calling" )

    Cancel();

    LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::~CSelectItemHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSelectItemHandler::ClientResponse
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSelectItemHandler::ClientResponse()
    {
    LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::ClientResponse calling" )

    //lint -e{961} Only these two cases need to be handeled.
    if ( RSat::KPSessionTerminatedByUser == iSelectItemRsp.iGeneralResult )
        {
        LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::ClientResponse close \
             ui session" )
        // Next SimSession end will close the ui session.
        iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
        }
    // If command had icon data and was done succesfully, report that icon
    // was not shown
    // To be removed when icons are allowed in this command
    else if ( ( RSat::KSuccess == iSelectItemRsp.iGeneralResult ) &&
        iIconCommand )
        {
        LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::ClientResponse \
             KSuccessRequestedIconNotDisplayed" )
        iSelectItemRsp.iGeneralResult =
                    RSat::KSuccessRequestedIconNotDisplayed;
        }

    CSatCommandHandler::TerminalRsp( RSat::ESelectItem, iSelectItemRspPckg );

    LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::ClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// CSelectItemHandler::DoCancel
// Cancels the sat request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSelectItemHandler::DoCancel()
    {
    LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::DoCancel calling" )

    iUtils->USatAPI().NotifySelectItemCancel();

    LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSelectItemHandler::IssueUSATRequest
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSelectItemHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    iSelectItemData.Reset();
    iSelectItemData.iDefaultItemId = 0;
    iSelectItemData.iIconId.iQualifier = RSat::EIconQualifierNotSet;
    iSelectItemData.iIconListQualifier = RSat::EIconQualifierNotSet;
    iSelectItemData.iIconId.iIdentifier = 0;

    iUtils->USatAPI().NotifySelectItem( aStatus, iSelectItemPckg );

    LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// CSelectItemHandler::CommandAllowed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CSelectItemHandler::CommandAllowed()
    {
    LOG( SIMPLE,
        "SELECTITEM: CSelectItemHandler::CommandAllowed calling" )

    TBool commandAllowed( ETrue );
    TBool nullText( EFalse );
    TInt err ( KErrNone );
    RSat::TItem item;
    TUint numberOfItems( iSelectItemData.NumberOfItems() );
    TBool titleIcon(
        ( RSat::ENotSelfExplanatory == iSelectItemData.iIconId.iQualifier ) ||
        ( RSat::ESelfExplanatory == iSelectItemData.iIconId.iQualifier ) );
    TBool itemIcons(
        ( RSat::ENotSelfExplanatory == iSelectItemData.iIconListQualifier ) ||
        ( RSat::ESelfExplanatory == iSelectItemData.iIconListQualifier ) );
    LOG2( NORMAL, "SELECTITEM: CSelectItemHandler::CommandAllowed \
          numberOfItems: %d", numberOfItems )
    // Check if some item does not have text
    for ( TUint count = 1; ( count <= numberOfItems ) && !nullText &&
        ( KErrNone == err ); count++ )
        {
        err = iSelectItemData.GetItem( count, item );

        if ( ( KErrNone == err ) && ( 0 == item.iItemString.Length() ) )
            {
            LOG( SIMPLE,
            "SELECTITEM: CSelectItemHandler::CommandAllowed null string" )
            nullText = ETrue;
            }
        }

    // If icon is provided without any text
    if ( ( ( RSat::EAlphaIdProvided != iSelectItemData.iAlphaId.iStatus ) &&
             titleIcon ) || ( nullText && itemIcons ) )
        {
        iSelectItemRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
        iSelectItemRsp.iInfoType = RSat::KNoAdditionalInfo;
        iSelectItemRsp.iAdditionalInfo.Zero();
        iSelectItemRsp.SetPCmdNumber( iSelectItemData.PCmdNumber() );
        TerminalRsp( RSat::ESelectItem, iSelectItemRspPckg );
        commandAllowed = EFalse;
        LOG( NORMAL, "SELECTITEM: CSelectItemHandler::\
            CommandAllowed icon received without alpha id" )
        }
    // Set icon command flag whether icon data was received and set qualifier
    // to no icon id
    // To be removed when icons are allowed in this command
    else if ( !titleIcon && !itemIcons )
        {
        LOG( NORMAL, "SELECTITEM: CSelectItemHandler::\
            CommandAllowed no icon " )
        iIconCommand = EFalse;
        }
    else
        {
        iIconCommand = ETrue;

        if ( titleIcon )
            {
            LOG( NORMAL, "SELECTITEM: CSelectItemHandler::\
            CommandAllowed titleIcon true " )
            iSelectItemData.iIconId.iQualifier = RSat::ENoIconId;
            }

        if ( itemIcons )
            {
            LOG( NORMAL, "SELECTITEM: CSelectItemHandler::\
            CommandAllowed itemIcons true " )
            iSelectItemData.iIconListQualifier = RSat::ENoIconId;
            }
        }

    LOG2( SIMPLE,
        "SELECTITEM: CSelectItemHandler::CommandAllowed exiting,\
        commandAllowed: %d", commandAllowed )
    return commandAllowed;
    }

// -----------------------------------------------------------------------------
// CSelectItemHandler::NeedUiSession
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CSelectItemHandler::NeedUiSession()
    {
    LOG( NORMAL, "SELECTITEM: CSelectItemHandler::NeedUiSession calling" )
    // Notify Cover UI if it's supported
    if ( iUtils->CoverUiSupported() )
        {
        LOG( NORMAL, "SELECTITEM: CSelectItemHandler::NeedUiSession \
             CoverUiSupported" )
        TSatCommandData medEventData;
        medEventData.iPCmdNumber = RSat::ESelectItem;
        medEventData.iAlphaId =  iSelectItemData.iAlphaId;
        medEventData.iDuration.iNumOfUnits = KSatDefaultDuration;
        medEventData.iDuration.iTimeUnit = RSat::ESeconds;
        medEventData.iIconID = iSelectItemData.iIconId;
        TSatCommandPckg tPckg( medEventData );
        iUtils->RaiseSatEvent( tPckg );
        #ifdef ENABLE_SAT_LOGGING    
        TBuf<RSat::KAlphaIdMaxSize> logAlphaId;        
        logAlphaId.Copy( medEventData.iAlphaId.iAlphaId );                
        LOG2( NORMAL, "SELECTITEM: CSelectItemHandler::NeedUiSession \
        iAlphaId: %S", &logAlphaId )
        #endif
        }
    LOG( NORMAL, "SELECTITEM: CSelectItemHandler::NeedUiSession exiting" )
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CSelectItemHandler::HandleCommand
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSelectItemHandler::HandleCommand()
    {
    LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::HandleCommand calling" )

    iUtils->NotifyEvent( MSatUtils::ESelectItemExecuting );

    MSatUiSession* uiSession = iUtils->SatUiHandler().UiSession();
    uiSession->SendCommand(
        &iSelectItemPckg,
        &iSelectItemRspPckg,
        ESatSProactiveSelectItem );

    LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// CSelectItemHandler::UiLaunchFailed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSelectItemHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::UiLaunchFailed calling" )

    iSelectItemRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
    iSelectItemRsp.iInfoType = RSat::KMeProblem;
    iSelectItemRsp.iAdditionalInfo.SetLength( 1 );
    iSelectItemRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
    iSelectItemRsp.SetPCmdNumber( iSelectItemData.PCmdNumber() );
    CSatCommandHandler::TerminalRsp( RSat::ESelectItem, iSelectItemRspPckg );

    LOG( SIMPLE, "SELECTITEM: CSelectItemHandler::UiLaunchFailed exiting" )
    }
