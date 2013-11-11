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
* Description:  Handles GetChannelStatus command
*
*/



// INCLUDE FILES
#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatBIPUtils.h"
#include    "MSatBIPDataChannel.h"
#include    "CGetChannelStatusHandler.h"
#include    "SatLog.h"

// CONSTANTS
// length of channel status info
const TUint8 KChannelStatusLength = 2;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CGetChannelStatusHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CGetChannelStatusHandler* CGetChannelStatusHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "GETCHANNELSTATUS: CGetChannelStatusHandler::NewL calling" )
    CGetChannelStatusHandler* self = new( ELeave ) CGetChannelStatusHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    // ConstructL not needed.
    CleanupStack::Pop( self );

    LOG( SIMPLE, "GETCHANNELSTATUS: CGetChannelStatusHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CGetChannelStatusHandler::~CGetChannelStatusHandler
// Destructor
// -----------------------------------------------------------------------------
//
CGetChannelStatusHandler::~CGetChannelStatusHandler()
    {
    LOG( SIMPLE, "GETCHANNELSTATUS: CGetChannelStatusHandler::\
        CGetChannelStatusHandler calling" )

    Cancel();

    LOG( SIMPLE, "GETCHANNELSTATUS: CGetChannelStatusHandler::\
        ~CGetChannelStatusHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CGetChannelStatusHandler::DoCancel
// Cancels the sat request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CGetChannelStatusHandler::DoCancel()
    {
    LOG( SIMPLE, "GETCHANNELSTATUS: CGetChannelStatusHandler::\
        DoCancel calling" )

    iUtils->USatAPI().NotifyGetChannelStatusCancel();

    LOG( SIMPLE, "GETCHANNELSTATUS: CGetChannelStatusHandler::\
        DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CGetChannelStatusHandler::IssueUSATRequest
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CGetChannelStatusHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE,
        "GETCHANNELSTATUS: CGetChannelStatusHandler::IssueUSATRequest calling" )

    // Reset previously received data
    new (&iGetChannelData) RSat::TGetChannelStatusV2();
    iUtils->USatAPI().NotifyGetChannelStatus( aStatus, iGetChannelDataPckg );

    LOG( SIMPLE,
        "GETCHANNELSTATUS: CGetChannelStatusHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// CGetChannelStatusHandler::CommandAllowed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CGetChannelStatusHandler::CommandAllowed()
    {
    LOG( SIMPLE, "GETCHANNELSTATUS: CGetChannelStatusHandler::CommandAllowed \
        calling - exiting" )
    // Get channel status command is always allowed
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CGetChannelStatusHandler::NeedUiSession
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CGetChannelStatusHandler::NeedUiSession()
    {
    LOG( SIMPLE, "GETCHANNELSTATUS: CGetChannelStatusHandler::NeedUiSession\
        calling - exiting" )
    // This command doesn't need UI
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CGetChannelStatusHandler::HandleCommand
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CGetChannelStatusHandler::HandleCommand()
    {
    LOG( SIMPLE,
        "GETCHANNELSTATUS: CGetChannelStatusHandler::HandleCommand calling" )

    iUtils->NotifyEvent( MSatUtils::EGetChannelStatusExecuting );
    // Get data channels from BIP Utils
    CArrayPtrFlat<MSatBIPDataChannel>* dataChannels(
        iUtils->BipUtils().DataChannels() );

    // Get number of data channels
    const TInt channelCount( dataChannels->Count() );

    // Create IPC package
    iGetChannelRsp.SetPCmdNumber( iGetChannelData.PCmdNumber() );

    // Check that count is more than 0
    if ( channelCount > 0 )
        {
        LOG( NORMAL, "GETCHANNELSTATUS: CGetChannelStatusHandler::HandleCommand\
            Gathering channel statuses" )

        TRAPD( err, GetChannelStatusesL( channelCount ) );
        if ( KErrNone == err )
            {
            // Result is success
            iGetChannelRsp.iInfoType = RSat::KChannelStatusInfo;
            iGetChannelRsp.iGeneralResult = RSat::KSuccess;
            }
        else
            {
            LOG2( NORMAL, "GETCHANNELSTATUS: CGetChannelStatusHandler::\
                HandleCommand with error: %i", err )
            // Error
            iGetChannelRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iGetChannelRsp.iInfoType = RSat::KMeProblem;
            iGetChannelRsp.iAdditionalInfo.SetLength( 1 );
            iGetChannelRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
            }
        }
    else
        {
        LOG( NORMAL, "GETCHANNELSTATUS: CGetChannelStatusHandler::HandleCommand\
            No data channels" )
        // No data channels available, return one empty ChannelStatus object
        iGetChannelRsp.iInfoType = RSat::KChannelStatusInfo;
        iGetChannelRsp.iGeneralResult = RSat::KSuccess;
        // No channels, return ChannelStatusInfo 00 00
        iGetChannelRsp.iAdditionalInfo.SetLength( KChannelStatusLength );
        iGetChannelRsp.iAdditionalInfo[0] = 0x00;
        iGetChannelRsp.iAdditionalInfo[1] = 0x00;
        }

    // Send terminal response
    TerminalRsp( RSat::EGetChannelStatus, iGetChannelRspPckg );

    LOG( SIMPLE,
        "GETCHANNELSTATUS: CGetChannelStatusHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// CGetChannelStatusHandler::UiLaunchFailed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CGetChannelStatusHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "GETCHANNELSTATUS: CGetChannelStatusHandler::UiLaunchFailed \
        calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CGetChannelStatusHandler::GetChannelStatusesL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CGetChannelStatusHandler::GetChannelStatusesL( const TInt aChannelCount )
    {
    LOG( SIMPLE, "GETCHANNELSTATUS: CGetChannelStatusHandler::\
        GetChannelStatuses calling" )

    // Get data channels from BIP Utils
    CArrayPtrFlat<MSatBIPDataChannel>* dataChannels(
        iUtils->BipUtils().DataChannels() );

    // Each data channel needs 2 slots in additional info
    iGetChannelRsp.iAdditionalInfo.SetLength(
        aChannelCount * KChannelStatusLength );

    TInt addInfoIndex = 0;
    LOG2( SIMPLE, "GETCHANNELSTATUS: CGetChannelStatusHandler::\
        GetChannelStatusesL aChannelCount: %d", aChannelCount )
    // Loop data channels
    if ( dataChannels->Count() >= aChannelCount )
        {
        LOG( SIMPLE, "GETCHANNELSTATUS: CGetChannelStatusHandler::\
             GetChannelStatusesL dataChannels->Count() >= aChannelCount" )
        for ( TInt i = 0; i < aChannelCount; i++ )
            {
            // Get data channel from array
            MSatBIPDataChannel* channel = dataChannels->At( i );
            // Generate TChannelStatus
            RSat::TChannelStatus status(
                iUtils->BipUtils().GenerateChannelStatusL(
                    channel->ChannelId(), channel->Status() ) );

            // Add first byte (channel ID and context activation state)
            iGetChannelRsp.iAdditionalInfo[addInfoIndex++] = status[0];
            // Add second byte (channel status)
            iGetChannelRsp.iAdditionalInfo[addInfoIndex++] = status[1];
            }
        }

    LOG( SIMPLE, "GETCHANNELSTATUS: CGetChannelStatusHandler::\
        GetChannelStatuses exiting" )
    }

// -----------------------------------------------------------------------------
// CGetChannelStatusHandler::CGetChannelStatusHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CGetChannelStatusHandler::CGetChannelStatusHandler() :
    CSatCommandHandler(),
    iGetChannelData(),
    iGetChannelDataPckg( iGetChannelData ),
    iGetChannelRsp(),
    iGetChannelRspPckg( iGetChannelRsp )
    {
    LOG( SIMPLE,
        "GETCHANNELSTATUS: CGetChannelStatusHandler::\
            CGetChannelStatusHandler calling - exiting" )
    }

//  End of File
