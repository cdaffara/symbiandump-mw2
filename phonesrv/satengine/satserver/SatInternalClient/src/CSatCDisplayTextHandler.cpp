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
*                Display Text proactive command.
*
*/


#include    <e32svr.h>
#include    "RSatUiSession.h"
#include    "MSatUiObserver.h"
#include    "CSatCDisplayTextHandler.h"
#include    "SatSOpcodes.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatCDisplayTextHandler::CSatCDisplayTextHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatCDisplayTextHandler::CSatCDisplayTextHandler(
    TInt aPriority,
    RSatUiSession* aSession ) :
    CActive( aPriority ), iSession( aSession ),
    iDisplayTextData(),
    iDisplayTextPckg( iDisplayTextData ),
    iDisplayTextRsp(),
    iDisplayTextRspPckg( iDisplayTextRsp )
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCDisplayTextHandler::CSatCDisplayTextHandler calling" )

    CActiveScheduler::Add( this ); // add to active scheduler

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCDisplayTextHandler::CSatCDisplayTextHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCDisplayTextHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatCDisplayTextHandler* CSatCDisplayTextHandler::NewL(
    RSatUiSession* aSat )
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCDisplayTextHandler::NewL calling" )

    // Perform construction.
    CSatCDisplayTextHandler* self =
        new ( ELeave ) CSatCDisplayTextHandler( EPriorityLow, aSat );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCDisplayTextHandler::NewL exiting" )
    return self;
    }

// Destructor
CSatCDisplayTextHandler::~CSatCDisplayTextHandler()
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCDisplayTextHandler::~CSatCDisplayTextHandler calling" )

    // Cancel any outstanding requests.
    Cancel();
    iSession = NULL;

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCDisplayTextHandler::~CSatCDisplayTextHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCDisplayTextHandler::Start
// Starts the handler.
// -----------------------------------------------------------------------------
//
void CSatCDisplayTextHandler::Start()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCDisplayTextHandler::Start calling" )

    // Empty the IPC data
    TSatDisplayTextV1 temp;
    iDisplayTextData = temp;
    RSat::TDisplayTextRspV1 temp2;
    iDisplayTextRsp = temp2;

    // Request Display Text notifications.
    TIpcArgs arguments( &iDisplayTextPckg );

    // Pass the Display Text IPC package
    iSession->CreateRequest( ESatSProactiveDisplayText, arguments, iStatus );

    // Set this handler to active so that it can receive requests.
    SetActive();

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCDisplayTextHandler::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCDisplayTextHandler::RunL
// Handles the command.
// -----------------------------------------------------------------------------
//
void CSatCDisplayTextHandler::RunL()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCDisplayTextHandler::RunL calling" )

    // Check the status of the asnychronous operation
    if ( KErrNone != iStatus.Int() )
        {
        LOG2(
            SIMPLE,
            "SATINTERNALCLIENT: CSatCDisplayTextHandler::RunL exiting, error: %d",
            iStatus.Int() )

        // Renew the request
        Start();
        }
    else
        {
        // Has to be casted to TInt before casting to TSatIconQualifier, because
        // gcc warns about the direct cast.
        const struct TSatIconId iconId = { iDisplayTextData.iIconId.iIdentifier,
            static_cast<TSatIconQualifier>(
                static_cast<TInt>( iDisplayTextData.iIconId.iQualifier ) ) };

        // This will contain EFalse if requested icon is not displayed.
        TBool requestedIconDisplayed( ETrue );

        // Indicator if user is expected to clear the message
        TBool waitUserToClear( ETrue );

        if ( RSat::EClearAfterDelay == iDisplayTextData.iClearScreen )
            {
            LOG( SIMPLE, 
            "SATINTERNALCLIENT: CSatCDisplayTextHandler::RunL EClearAfterDelay" )
            waitUserToClear = EFalse;
            }

        // Notify the registered client and save the response.
        TSatUiResponse response;
        response = iSession->SatUiObserver()->DisplayTextL(
            iDisplayTextData.iText,
            iDisplayTextData.iSimApplicationName,
            iconId,
            requestedIconDisplayed,
            iDisplayTextData.iSustainedText,
            iDisplayTextData.iDuration,
            waitUserToClear );

        // This command never has any additional information.
        iDisplayTextRsp.iInfoType = RSat::KNoAdditionalInfo;
        iDisplayTextRsp.iAdditionalInfo.Zero();

        iDisplayTextRsp.SetPCmdNumber( iDisplayTextData.iPCmdNumber );

        // Examine the client response.
        ExamineClientResponse(
            response, requestedIconDisplayed );

        // Pass the Display Text response IPC package.
        TIpcArgs arguments( &iDisplayTextRspPckg );

        // Perform the IPC data transfer.
        iSession->CreateRequest( ESatSProactiveDisplayTextResponse, arguments );

        // Renew the service request.
        Start();        
        }
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCDisplayTextHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCDisplayTextHandler::ExamineClientResponse
// Examine the client response.
// -----------------------------------------------------------------------------
//
void CSatCDisplayTextHandler::ExamineClientResponse(
    TSatUiResponse aResponse,
    TBool aRequestedIconDisplayed )
    {
    LOG2( SIMPLE,
    "SATINTERNALCLIENT: CSatCDisplayTextHandler::ExamineClientResponse calling,\
    aResponse: %x", aResponse )

    switch ( aResponse )
        {
        case ESatSuccess:
            {
            // Convert terminal rsp if icon used
            RSat::TPCmdResult result( RSat::KSuccess );
            RSat::TIconQualifier iconQualifier(
                    iDisplayTextData.iIconId.iQualifier );

            if ( !aRequestedIconDisplayed )
                {
                LOG( SIMPLE,
                "SATINTERNALCLIENT: CSatCDisplayTextHandler::ExamineClientResponse \
                aRequestedIconDisplayed false" )
                if ( iconQualifier == RSat::ESelfExplanatory ||
                     iconQualifier == RSat::ENotSelfExplanatory )
                    {
                    LOG( SIMPLE,
                    "SATINTERNALCLIENT: CSatCDisplayTextHandler::ExamineClientResponse \
                    IconNotDisplayed" )
                    result = RSat::KSuccessRequestedIconNotDisplayed;
                    }
                }

            iDisplayTextRsp.iGeneralResult = result;
            break;
            }

        case ESatFailure:
            {
            iDisplayTextRsp.iInfoType = RSat::KMeProblem;
            iDisplayTextRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iDisplayTextRsp.iAdditionalInfo.SetLength( 1 );
            iDisplayTextRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
            break;
            }

        case ESatSessionTerminatedByUser:
            {
            iDisplayTextRsp.iGeneralResult = RSat::KPSessionTerminatedByUser;
            break;
            }

        case ESatBackwardModeRequestedByUser:
            {
            iDisplayTextRsp.iGeneralResult =
                RSat::KBackwardModeRequestedByUser;
            break;
            }

        case ESatNoResponseFromUser:
            {
            iDisplayTextRsp.iGeneralResult = RSat::KNoResponseFromUser;
            break;
            }

        // No help is ever supplied with this command
        case EHelpRequestedByUser:
        case EPCmdNotAcceptedByUser:
        case ESatCmdDataNotUnderstood:
        default:
            {
            iSession->Panic( ESatInvalidResponse );
            break;
            }
        }

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCDisplayTextHandler::ExamineClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCDisplayTextHandler::DoCancel
// Cancels the pending request.
// -----------------------------------------------------------------------------
//
void CSatCDisplayTextHandler::DoCancel()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCDisplayTextHandler::DoCancel calling" )

    // Complete the request with cancel code.
    TRequestStatus* requestStatus = &iStatus;
    User::RequestComplete( requestStatus, KErrCancel );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCDisplayTextHandler::DoCancel exiting" )
    }
