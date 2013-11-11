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
*                Play Tone proactive command.
*
*/


#include    <e32svr.h>
#include    "RSatUiSession.h"
#include    "MSatUiObserver.h"
#include    "SatSOpcodes.h"
#include    "CSatCPlayToneHandler.h"
#include    "SatLog.h"

// Used when Play tone doesn't contain duration.
const TInt KSatPtDefaultDuration = 5;

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatCPlayToneHandler::CSatCPlayToneHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatCPlayToneHandler::CSatCPlayToneHandler(
    TInt aPriority,
    RSatUiSession* aSession ) :
    CActive( aPriority ),
    iSession( aSession ),
    iPlayToneData(),
    iPlayTonePckg( iPlayToneData ),
    iPlayToneRsp(),
    iPlayToneRspPckg( iPlayToneRsp )
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCPlayToneHandler::CSatCPlayToneHandler calling" )

    // Add to active scheduler.
    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCPlayToneHandler::CSatCPlayToneHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCPlayToneHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatCPlayToneHandler* CSatCPlayToneHandler::NewL(
    RSatUiSession* aSat )
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCPlayToneHandler::NewL calling" )

    // Perform the construction.
    CSatCPlayToneHandler* self =
        new ( ELeave ) CSatCPlayToneHandler( EPriorityLow, aSat );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCPlayToneHandler::NewL exiting" )
    return self;
    }

// Destructor
CSatCPlayToneHandler::~CSatCPlayToneHandler()
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCPlayToneHandler::~CSatCPlayToneHandler calling" )
    // Cancel any outstanding requests.

    Cancel();
    iSession = NULL;

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCPlayToneHandler::~CSatCPlayToneHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCPlayToneHandler::Start
// Starts the handler.
// -----------------------------------------------------------------------------
//
void CSatCPlayToneHandler::Start()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCPlayToneHandler::Start calling" )

    // Empty the IPC data
    RSat::TPlayToneV2 temp;
    iPlayToneData = temp;
    RSat::TPlayToneRspV1 temp2;
    iPlayToneRsp = temp2;

    // Request Play Tone notification.
    TIpcArgs arguments( &iPlayTonePckg );

    // Pass the Play Tone IPC package.
    iSession->CreateRequest( ESatSProactivePlayTone, arguments, iStatus );

    // Set this handler to active so that it can receive requests.
    SetActive();

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCPlayToneHandler::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCPlayToneHandler::RunL
// Handles the command.
// -----------------------------------------------------------------------------
//
void CSatCPlayToneHandler::RunL()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCPlayToneHandler::RunL calling" )

    // Check the status of the asnychronous operation
    if ( KErrNone != iStatus.Int() )
        {
        LOG2(
            SIMPLE,
            "SATINTERNALCLIENT: CSatCPlayToneHandler::RunL exiting, error: %d",
            iStatus.Int() )

        // Renew the request
        Start();

        return;
        }

    // Determine the play duration.
    TTimeIntervalMicroSeconds interval( static_cast<TInt64>( 0 ) );
    LOG2( SIMPLE,
    "SATINTERNALCLIENT: CSatCPlayToneHandler::RunL \
    iPlayToneData.iDuration.iTimeUnit: %d",iPlayToneData.iDuration.iTimeUnit )
    switch ( iPlayToneData.iDuration.iTimeUnit )
        {
        case RSat::EMinutes:
            {
            TInt64 temp1(
            static_cast<TInt>( iPlayToneData.iDuration.iNumOfUnits ) );
            TInt64 temp2( KSatCMicroSecondsInMinute );
            interval = temp1 * temp2;
            break;
            }
        case RSat::ESeconds:
            {
            interval = iPlayToneData.iDuration.iNumOfUnits *
                KSatCMicroSecondsInSecond;
            break;
            }
        case RSat::ETenthsOfSeconds:
            {
            interval = iPlayToneData.iDuration.iNumOfUnits *
                KSatCMicroSecondsInOneTenthSecond;
            break;
            }
        case RSat::ETimeUnitNotSet:
        case RSat::ENoDurationAvailable:
            {
            // We are defaulting to 5 seconds in S60
            interval = KSatPtDefaultDuration *
                KSatCMicroSecondsInSecond;
            break;
            }
        default:
            {
            LOG( SIMPLE, "SATINTERNALCLIENT: Unexpected time unit" )
            }
        }
    // Set the type of tone to be played.
    // Cannot use static_cast directly because of GCC
    TSatTone dialTone( static_cast<TSatTone>(
        static_cast<TInt>( iPlayToneData.iTone ) ) );

    // Has to be casted to TInt before casting to TSatIconQualifier, because
    // GCC warns about the direct cast.
    const struct TSatIconId iconId = { iPlayToneData.iIconId.iIdentifier,
        static_cast<TSatIconQualifier>(
            static_cast<TInt>( iPlayToneData.iIconId.iQualifier ) ) };

    // This will contain EFalse if requested icon is not displayed.
    // And if icon is displayed, it contains ETrue.
    TBool requestedIconDisplayed( EFalse );

    // Notify the registered client and save the response.
    TSatUiResponse response = iSession->SatUiObserver()->PlayTone(
        iPlayToneData.iAlphaId.iAlphaId, dialTone, interval,
        iconId, requestedIconDisplayed );

    // This command never has any additional information.
    iPlayToneRsp.iInfoType = RSat::KNoAdditionalInfo;
    iPlayToneRsp.iAdditionalInfo.Zero();

    iPlayToneRsp.SetPCmdNumber( iPlayToneData.PCmdNumber() );

    // Examine the client response.
    ExamineClientResponse(
        response, requestedIconDisplayed );

    // Pass the Play Tone response IPC package.
    TIpcArgs arguments( &iPlayToneRspPckg );

    // Perform the IPC data transfer.
    iSession->CreateRequest( ESatSProactivePlayToneResponse, arguments );

    // Renew the request
    Start();

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCPlayToneHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCPlayToneHandler::ExamineClientResponse
// Examine the client response.
// -----------------------------------------------------------------------------
//
void CSatCPlayToneHandler::ExamineClientResponse(
    TSatUiResponse aResponse,
    TBool aRequestedIconDisplayed )
    {
    LOG2( SIMPLE,
    "SATINTERNALCLIENT: CSatCPlayToneHandler::ExamineClientResponse calling,\
    aResponse: %x", aResponse )

    // Examine the client response.
    switch ( aResponse )
        {
        case ESatSuccess:
        case ESatSuccessToneNotPlayed: // for future use,
        // specified in ETSI spec but not currently implemented in etelsat.
            {
            // Convert terminal rsp if icon used
            RSat::TPCmdResult result( RSat::KSuccess );
            RSat::TIconQualifier iconQualifier(
                iPlayToneData.iIconId.iQualifier );

            if ( !aRequestedIconDisplayed )
                {
                LOG( SIMPLE,
                "SATINTERNALCLIENT: CSatCPlayToneHandler::ExamineClientResponse \
                aRequestedIconDisplayed false" )
                if ( iconQualifier == RSat::ESelfExplanatory ||
                     iconQualifier == RSat::ENotSelfExplanatory )
                    {
                    LOG( SIMPLE,
                    "SATINTERNALCLIENT: CSatCPlayToneHandler::ExamineClientResponse \
                    IconNotDisplayed" )
                    result = RSat::KSuccessRequestedIconNotDisplayed;
                    }
                }

            iPlayToneRsp.iGeneralResult = result;
            break;
            }
        case ESatFailure:
            {
            iPlayToneRsp.iInfoType = RSat::KMeProblem;
            iPlayToneRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iPlayToneRsp.iAdditionalInfo.SetLength( 1 );
            iPlayToneRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
            break;
            }
        case ESatSessionTerminatedByUser:
            {
            iPlayToneRsp.iGeneralResult = RSat::KPSessionTerminatedByUser;
            break;
            }
        case ESatBackwardModeRequestedByUser:
            {
            iPlayToneRsp.iGeneralResult =
                RSat::KBackwardModeRequestedByUser;
            break;
            }
        case ESatNoResponseFromUser:
            {
            iPlayToneRsp.iGeneralResult = RSat::KNoResponseFromUser;
            break;
            }
        case ESatCmdDataNotUnderstood:
            {
            iPlayToneRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
            iPlayToneRsp.iInfoType = RSat::KNoAdditionalInfo;
            iPlayToneRsp.iAdditionalInfo.Zero();
            break;
            }
        // No help is ever available with this command.
        case EHelpRequestedByUser:
        case EPCmdNotAcceptedByUser:
        default:
            {
            iSession->Panic( ESatInvalidResponse );
            break;
            }
        }

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCPlayToneHandler::ExamineClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCPlayToneHandler::DoCancel
// Cancels the pending request.
// -----------------------------------------------------------------------------
//
void CSatCPlayToneHandler::DoCancel()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCPlayToneHandler::DoCancel calling" )

    // Complete the request with cancel code.
    TRequestStatus* requestStatus = &iStatus;
    User::RequestComplete( requestStatus, KErrCancel );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCPlayToneHandler::DoCancel exiting" )
    }
