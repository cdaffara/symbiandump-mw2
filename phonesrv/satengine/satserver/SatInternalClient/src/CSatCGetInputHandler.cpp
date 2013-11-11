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
*                Get Input proactive command.
*
*/


#include    <e32svr.h>
#include    "RSatUiSession.h"
#include    "MSatUiObserver.h"
#include    "CSatCGetInputHandler.h"
#include    "SatSOpcodes.h"
#include    "SatSTypes.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatCGetInputHandler::CSatCGetInputHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatCGetInputHandler::CSatCGetInputHandler(
    TInt aPriority,
    RSatUiSession* aSession ) :
    CActive( aPriority ),
    iSession( aSession ),
    iGetInputData(),
    iGetInputPckg( iGetInputData ),
    iGetInputRsp(),
    iGetInputRspPckg( iGetInputRsp )
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCGetInputHandler::CSatCGetInputHandler calling" )

    // Add to active scheduler.
    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCGetInputHandler::CSatCGetInputHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCGetInputHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatCGetInputHandler* CSatCGetInputHandler::NewL(
    RSatUiSession* aSat )
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInputHandler::NewL calling" )

    // Perform construction.
    CSatCGetInputHandler* self =
        new ( ELeave ) CSatCGetInputHandler( EPriorityLow, aSat );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInputHandler::NewL exiting" )
    return self;
    }

// Class destructor.
CSatCGetInputHandler::~CSatCGetInputHandler()
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCGetInputHandler::~CSatCGetInputHandler calling" )

    // Cancel any outstanding requests.
    Cancel();

    iSession = NULL;

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCGetInputHandler::~CSatCGetInputHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCGetInputHandler::Start
// Starts the handler.
// -----------------------------------------------------------------------------
//
void CSatCGetInputHandler::Start()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInputHandler::Start calling" )

    // Empty the IPC data
    RSat::TGetInputV1 temp;
    iGetInputData = temp;
    RSat::TGetInputRspV1 temp2;
    iGetInputRsp = temp2;

    // Request Get Input notification.
    TIpcArgs arguments( &iGetInputPckg );

    // Pass the Get Input IPC package.
    iSession->CreateRequest( ESatSProactiveGetInput, arguments, iStatus );

    // Set this handler to active so that it can receive requests.
    SetActive();

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInputHandler::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCGetInputHandler::ConvertCharacterSet
// Converts USAT character set to SAT character set type.
// -----------------------------------------------------------------------------
//
TSatCharacterSet CSatCGetInputHandler::ConvertCharacterSet(
    const RSat::TGetInputRspFormat& aCharacterSet ) const
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCGetInputHandler::ConvertCharacterSet calling" )

    TSatCharacterSet charSet( ESatCharSmsDefaultAlphabet );
    
    LOG2( SIMPLE,
    "SATINTERNALCLIENT: CSatCGetInputHandler::ConvertCharacterSet aCharacterSet: %d",
    aCharacterSet )
    
    if ( ( RSat::EDigitOnlyUnpacked == aCharacterSet ) ||
         ( RSat::EDigitOnlyPacked == aCharacterSet ) )
        {        
        charSet = ESatDigitOnly;
        }
    else if ( aCharacterSet == RSat::EUcs2Alphabet )
        {       
        charSet = ESatCharUcs2Alphabet;
        }
    else
        {        
        charSet = ESatCharSmsDefaultAlphabet;
        }

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCGetInputHandler::ConvertCharacterSet exiting" )
    return charSet;
    }

// -----------------------------------------------------------------------------
// CSatCGetInputHandler::RunL
// Handles the command.
// -----------------------------------------------------------------------------
//
void CSatCGetInputHandler::RunL()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInputHandler::RunL calling" )

    // Check the status of the asnychronous operation
    if ( KErrNone != iStatus.Int() )
        {
        LOG2(
            SIMPLE,
            "SATINTERNALCLIENT: CSatCGetInputHandler::RunL exiting, error: %d",
            iStatus.Int() )

        // Renew the request
        Start();

        return;
        }

    // Determine the character set.
    const TSatCharacterSet characterSet(
        ConvertCharacterSet( iGetInputData.iRspFormat ) );

    // Determine whether user input should be hidden or not.
    TBool hideInput( EFalse );
    if ( RSat::EHideUserInput == iGetInputData.iInputDisplayOption )
        {
        LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInputHandler::RunL EHideUserInput" )
        hideInput = ETrue;
        }

    // This will contain user input.
    TBuf<KSatGetInputEntrySize> inputText( iGetInputData.iDefaultText );

    // Indicates whether help is available
    TBool helpIsAvailable( EFalse );
    if ( RSat::EHelpAvailable == iGetInputData.iHelp )
        {
        LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInputHandler::RunL EHelpAvailable" )
        helpIsAvailable = ETrue;
        }

    // Has to be casted to TInt before casting to TSatIconQualifier, because
    // GCC warns about the direct cast.
    const struct TSatIconId iconId = { iGetInputData.iIconId.iIdentifier,
        static_cast<TSatIconQualifier>(
            static_cast<TInt>( iGetInputData.iIconId.iQualifier ) ) };

    // This will contain EFalse if requested icon is not displayed.
    // And if icon is displayed, it contains ETrue.
    TBool requestedIconDisplayed( EFalse );

    // Notify the registered client and save the response.
    TSatUiResponse response = iSession->SatUiObserver()->GetInputL(
        iGetInputData.iText,
        characterSet, inputText, iGetInputData.iRspLength.iMinRspLength,
        iGetInputData.iRspLength.iMaxRspLength, hideInput,
        helpIsAvailable, iconId, requestedIconDisplayed );

    // Use the same format in the response as it is in the input.
    iGetInputRsp.iRspFormat = iGetInputData.iRspFormat;

    // By default, this command does not have additional information.
    iGetInputRsp.iInfoType = RSat::KNoAdditionalInfo;
    iGetInputRsp.iAdditionalInfo.Zero();

    iGetInputRsp.SetPCmdNumber( iGetInputData.PCmdNumber() );

    // Examine the client response.
    ExamineClientResponse(
        response, inputText, requestedIconDisplayed );

    // Pass the Get Input response package.
    TIpcArgs arguments( &iGetInputRspPckg );

    // Perform the IPC data transfer.
    iSession->CreateRequest( ESatSProactiveGetInputResponse, arguments );

    // Renew the service request.
    Start();

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInputHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCGetInputHandler::ExamineClientResponse
// Examine the client response.
// -----------------------------------------------------------------------------
//
void CSatCGetInputHandler::ExamineClientResponse(
    TSatUiResponse aResponse,
    const TDes& aInputText,
    TBool aRequestedIconDisplayed )
    {
    LOG2( SIMPLE,
    "SATINTERNALCLIENT: CSatCGetInputHandler::ExamineClientResponse calling,\
    aResponse: %d", aResponse )

    // Examine the client response.
    switch ( aResponse )
        {
        case ESatSuccess:
            {

            // Convert terminal rsp if icon used
            RSat::TPCmdResult result( RSat::KSuccess );
            RSat::TIconQualifier iconQualifier(
                iGetInputData.iIconId.iQualifier );

                if ( !aRequestedIconDisplayed )
                {
                LOG( SIMPLE,
                "SATINTERNALCLIENT: CSatCGetInputHandler::ExamineClientResponse \
                aRequestedIconDisplayed false" )
                if ( iconQualifier == RSat::ESelfExplanatory ||
                     iconQualifier == RSat::ENotSelfExplanatory )
                    {
                    LOG( SIMPLE,
                    "SATINTERNALCLIENT: CSatCGetInputHandler::ExamineClientResponse \
                    IconNotDisplayed" )
                    result = RSat::KSuccessRequestedIconNotDisplayed;
                    }
                }

            iGetInputRsp.iGeneralResult = result;
            LOG2( SIMPLE,
            "SATINTERNALCLIENT: CSatCGetInputHandler::ExamineClientResponse length \
            of aInputText: %d", aInputText.Length() )
            // Change the additional information type
            if ( aInputText.Length() > 0 )
                {
                iGetInputRsp.iInfoType = RSat::KTextString;

                // Save the character input by the user
                iGetInputRsp.iAdditionalInfo.Append( aInputText );
                }
            break;
            }

        // Otherwise, just return the response.
        case ESatFailure:
            {
            iGetInputRsp.iInfoType = RSat::KMeProblem;
            iGetInputRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iGetInputRsp.iAdditionalInfo.SetLength( 1 );
            iGetInputRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
            break;
            }

        case ESatSessionTerminatedByUser:
            {
            iGetInputRsp.iGeneralResult = RSat::KPSessionTerminatedByUser;
            break;
            }

        case ESatBackwardModeRequestedByUser:
            {
            iGetInputRsp.iGeneralResult =
                RSat::KBackwardModeRequestedByUser;
            break;
            }

        case ESatNoResponseFromUser:
            {
            iGetInputRsp.iGeneralResult = RSat::KNoResponseFromUser;
            break;
            }

        case EHelpRequestedByUser:
            {
            iGetInputRsp.iGeneralResult = RSat::KHelpRequestedByUser;
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
        "SATINTERNALCLIENT: CSatCGetInputHandler::ExamineClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCGetInputHandler::DoCancel
// Cancels the pending request.
// -----------------------------------------------------------------------------
//
void CSatCGetInputHandler::DoCancel()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInputHandler::DoCancel calling" )

    // Complete the request with cancel code.
    TRequestStatus* requestStatus = &iStatus;
    User::RequestComplete( requestStatus, KErrCancel );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInputHandler::DoCancel exiting" )
    }
