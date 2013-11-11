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
*                Get Inkey proactive command.
*
*/


#include    <e32svr.h>
#include    "RSatUiSession.h"
#include    "MSatUiObserver.h"
#include    "CSatCGetInkeyHandler.h"
#include    "SatSOpcodes.h"
#include    "SatLog.h"

const TInt KTenthOfSecondsInMinute( 600 );
const TInt8 KSecond( 10 );
const TUint8 KHalfSecond( 5 );
const TInt KHalfMinute( 300 );
const TUint8 KByteMax( 255 );

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatCGetInkeyHandler::CSatCGetInkeyHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatCGetInkeyHandler::CSatCGetInkeyHandler(
    TInt aPriority,
    RSatUiSession* aSession ) :
    CActive( aPriority ),
    iSession( aSession ),
    iGetInkeyData(),
    iGetInkeyPckg( iGetInkeyData ),
    iGetInkeyRsp(),
    iGetInkeyRspPckg( iGetInkeyRsp )
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCGetInkeyHandler::CSatCGetInkeyHandler calling" )

    // Add to active scheduler.
    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCGetInkeyHandler::CSatCGetInkeyHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCGetInkeyHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatCGetInkeyHandler* CSatCGetInkeyHandler::NewL(
    RSatUiSession* aSat )
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInkeyHandler::NewL calling" )

    // Perform construction.
    CSatCGetInkeyHandler* self =
        new ( ELeave ) CSatCGetInkeyHandler( EPriorityLow, aSat );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInkeyHandler::NewL exiting" )
    return self;
    }

// Destructor
CSatCGetInkeyHandler::~CSatCGetInkeyHandler()
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCGetInkeyHandler::~CSatCGetInkeyHandler calling" )

    // Cancel any outstanding requests.
    Cancel();
    iSession = NULL;

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCGetInkeyHandler::~CSatCGetInkeyHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCGetInkeyHandler::Start
// Starts the handler.
// -----------------------------------------------------------------------------
//
void CSatCGetInkeyHandler::Start()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInkeyHandler::Start calling" )

    // Empty the IPC data
    RSat::TGetInkeyV2 temp;
    iGetInkeyData = temp;
    RSat::TGetInkeyRspV2 temp2;
    iGetInkeyRsp = temp2;

    // Request Get Inkey notifications.
    TIpcArgs arguments( &iGetInkeyPckg );

    // Pass the Get Inkey IPC package.
    iSession->CreateRequest( ESatSProactiveGetInkey, arguments, iStatus );

    // Set this handler to active so that it can receive requests.
    SetActive();

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInkeyHandler::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCGetInkeyHandler::RunL
// Handles the command.
// -----------------------------------------------------------------------------
//
void CSatCGetInkeyHandler::RunL()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInkeyHandler::RunL calling" )

    // Check the status of the asnychronous operation
    if ( KErrNone != iStatus.Int() )
        {
        LOG2(
            SIMPLE,
            "SATINTERNALCLIENT: CSatCGetInkeyHandler::RunL exiting, error: %d",
            iStatus.Int() )

        // Renew the request
        Start();

        return;
        }
    
    LOG2( SIMPLE,
    "SATINTERNALCLIENT: CSatCGetInkeyHandler::RunL iGetInkeyData.iRspFormat: %d",
    iGetInkeyData.iRspFormat )
    // Set the character set parameter.
    TSatCharacterSet characterSet = ESatCharSmsDefaultAlphabet;
    if ( RSat::EDigitOnly == iGetInkeyData.iRspFormat )
        {        
        characterSet = ESatDigitOnly;
        }
    else if ( RSat::ECharSmsDefaultAlphabet == iGetInkeyData.iRspFormat )
        {        
        characterSet = ESatCharSmsDefaultAlphabet;
        }
    else if ( RSat::ECharUcs2Alphabet == iGetInkeyData.iRspFormat )
        {        
        characterSet = ESatCharUcs2Alphabet;
        }
    else if ( RSat::EYesNo == iGetInkeyData.iRspFormat )
        {        
        characterSet = ESatYesNo;
        }
    else
        {        
        characterSet = ESatCharSmsDefaultAlphabet;
        }

    // This will contain user input.
    TChar character;

    // Indicates whether help is available
    TBool helpIsAvailable( EFalse );
    if ( RSat::EHelpAvailable == iGetInkeyData.iHelp )
        {
        LOG( SIMPLE, 
        "SATINTERNALCLIENT: CSatCGetInkeyHandler::RunL HelpAvailable" )
        helpIsAvailable = ETrue;
        }

    // Has to be casted to TInt before casting to TSatIconQualifier, because
    // GCC warns about the direct cast.
    const struct TSatIconId iconId = { iGetInkeyData.iIconId.iIdentifier,
        static_cast<TSatIconQualifier>(
            static_cast<TInt>( iGetInkeyData.iIconId.iQualifier ) ) };

    // This will contain EFalse if requested icon is not displayed.
    // And if icon is displayed, it contains ETrue.
    TBool requestedIconDisplayed( EFalse );

    TUint duration( 0 );
    TUint8 timeUnit( iGetInkeyData.iDuration.iTimeUnit );

    // check if duration data is available.
    if ( ( RSat::ENoDurationAvailable != timeUnit ) &&
        ( RSat::ETimeUnitNotSet != timeUnit ) &&
        iGetInkeyData.iDuration.iNumOfUnits )
        {
        // The resolution of a timer is tenth of second.
        duration = DurationInTenthOfSeconds();
        LOG2( SIMPLE, "SATINTERNALCLIENT: CSatCGetInkeyHandler duration %i",
            duration )
        }

    TBool immediateDigitResponse( EFalse );
    if ( RSat::EImmediateDigitRsp == iGetInkeyData.iMode )
        {
        LOG( SIMPLE, 
        "SATINTERNALCLIENT: CSatCGetInkeyHandler::RunL EImmediateDigitRsp" )
        immediateDigitResponse = ETrue;
        }

    // Notify the registered client and save the response.
    TSatUiResponse response = iSession->SatUiObserver()->GetInkeyL(
        iGetInkeyData.iText,
        characterSet, character, helpIsAvailable,
        iconId, requestedIconDisplayed,
        duration, immediateDigitResponse
        );

    // If duration exists set duration value in response
    if ( duration &&
        ( ESatSuccess == response || ESatNoResponseFromUser == response ) )
        {
        TenthOfSecondsToDuration( duration );
        LOG2( SIMPLE,
            "SATINTERNALCLIENT: CSatCGetInkeyHandler duration in response %i",
            iGetInkeyRsp.iDuration.iNumOfUnits )
        }

    // Use the same format in the response as it is in the input.
    iGetInkeyRsp.iRspFormat = iGetInkeyData.iRspFormat;

    // By default, this command does not have additional information.
    iGetInkeyRsp.iInfoType = RSat::KNoAdditionalInfo;
    iGetInkeyRsp.iAdditionalInfo.Zero();

    iGetInkeyRsp.SetPCmdNumber( iGetInkeyData.PCmdNumber() );

    // Examine the client response.
    ExamineClientResponse(
        response, character, requestedIconDisplayed );

    // Pass the Get Inkey response package.
    TIpcArgs arguments( &iGetInkeyRspPckg );

    // Perform the IPC data transfer.
    iSession->CreateRequest( ESatSProactiveGetInkeyResponse, arguments );

    // Renew the service request.
    Start();

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInkeyHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCGetInkeyHandler::ExamineClientResponse
// Examine the client response.
// -----------------------------------------------------------------------------
//
void CSatCGetInkeyHandler::ExamineClientResponse(
    TSatUiResponse aResponse,
    const TChar& aCharacter,
    TBool aRequestedIconDisplayed )
    {
    LOG2( SIMPLE,
    "SATINTERNALCLIENT: CSatCGetInkeyHandler::ExamineClientResponse calling,\
    aResponse: %x", aResponse )

    // Examine the client response.
    switch ( aResponse )
        {
        case ESatSuccess:
            {
             // Convert terminal rsp if icon used
            RSat::TPCmdResult result( RSat::KSuccess );
            RSat::TIconQualifier iconQualifier(
                iGetInkeyData.iIconId.iQualifier );

            if ( !aRequestedIconDisplayed )
                {
                LOG( SIMPLE,
                "SATINTERNALCLIENT: CSatCGetInkeyHandler::ExamineClientResponse \
                aRequestedIconDisplayed false" )
                if ( iconQualifier == RSat::ESelfExplanatory ||
                    iconQualifier == RSat::ENotSelfExplanatory )
                    {
                    LOG( SIMPLE,
                    "SATINTERNALCLIENT: CSatCGetInkeyHandler::ExamineClientResponse \
                    IconNotDisplayed" )
                    result = RSat::KSuccessRequestedIconNotDisplayed;
                    }
                }

            iGetInkeyRsp.iGeneralResult = result;

            // If GetInkey is type of YesNo, aCharacter is 0 if user selects NO
            if ( aCharacter || ( RSat::EYesNo == iGetInkeyData.iRspFormat ) )
                {
                LOG( SIMPLE,
                "SATINTERNALCLIENT: CSatCGetInkeyHandler::ExamineClientResponse \
                set AdditionalInfo" )
                // Change the additional information type
                iGetInkeyRsp.iInfoType = RSat::KTextString;

                // Save the character input by the user
                iGetInkeyRsp.iAdditionalInfo.Append( aCharacter );
                }
            else
                {
                LOG( SIMPLE,
                "SATINTERNALCLIENT: CSatCGetInkeyHandler::ExamineClientResponse \
                no AdditionalInfo" )
                // Otherwise, just return the response.
                iGetInkeyRsp.iInfoType = RSat::KNoAdditionalInfo;
                }
            break;
            }
        case ESatFailure:
            {
            iGetInkeyRsp.iInfoType = RSat::KMeProblem;
            iGetInkeyRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iGetInkeyRsp.iAdditionalInfo.SetLength( 1 );
            iGetInkeyRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
            break;
            }
        case ESatSessionTerminatedByUser:
            {
            iGetInkeyRsp.iGeneralResult = RSat::KPSessionTerminatedByUser;
            break;
            }
        case ESatBackwardModeRequestedByUser:
            {
            iGetInkeyRsp.iGeneralResult =
                RSat::KBackwardModeRequestedByUser;
            break;
            }
        case ESatNoResponseFromUser:
            {
            iGetInkeyRsp.iGeneralResult = RSat::KNoResponseFromUser;
            break;
            }
        case EHelpRequestedByUser:
            {
            iGetInkeyRsp.iGeneralResult = RSat::KHelpRequestedByUser;
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
        "SATINTERNALCLIENT: CSatCGetInkeyHandler::ExamineClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCGetInkeyHandler::DoCancel
// Cancels the pending request.
// -----------------------------------------------------------------------------
//
void CSatCGetInkeyHandler::DoCancel()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInkeyHandler::DoCancel calling" )

    // Complete the request with cancel code.
    TRequestStatus* requestStatus = &iStatus;
    User::RequestComplete( requestStatus, KErrCancel );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCGetInkeyHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCGetInkeyHandler::DurationInTenthOfSeconds
// Return duration in seconds.
// -----------------------------------------------------------------------------
//
TUint CSatCGetInkeyHandler::DurationInTenthOfSeconds() const
    {
    LOG2( SIMPLE, 
    "SATINTERNALCLIENT: CSatCGetInkeyHandler::DurationInTenthOfSeconds calling,\
    iGetInkeyData.iDuration.iTimeUnit: %d",iGetInkeyData.iDuration.iTimeUnit )
    TUint duration( 0 );

    switch ( iGetInkeyData.iDuration.iTimeUnit )
        {
        case RSat::EMinutes:
            {
            duration =
                iGetInkeyData.iDuration.iNumOfUnits * KTenthOfSecondsInMinute;
            break;
            }
        case RSat::ESeconds:
            {
            duration = iGetInkeyData.iDuration.iNumOfUnits * KSecond;
            break;
            }
        case RSat::ETenthsOfSeconds:
            {
            duration = iGetInkeyData.iDuration.iNumOfUnits;
            break;
            }
        default:
            {
            //duration is 0
            break;
            }
        }
    LOG2( SIMPLE, 
    "SATINTERNALCLIENT: CSatCGetInkeyHandler::DurationInTenthOfSeconds exiting,\
    duration: %d", duration )
    return duration;
    }

// -----------------------------------------------------------------------------
// CSatCGetInkeyHandler::TenthOfSecondsToDuration
// Convert second to duration.
// -----------------------------------------------------------------------------
//
void CSatCGetInkeyHandler::TenthOfSecondsToDuration(
    TUint aDuration )
    {
    LOG2( SIMPLE, 
    "SATINTERNALCLIENT: CSatCGetInkeyHandler::TenthOfSecondsToDuration calling,\
    iGetInkeyData.iDuration.iTimeUnit: %d",iGetInkeyData.iDuration.iTimeUnit )
    switch ( iGetInkeyData.iDuration.iTimeUnit )
        {
        case RSat::EMinutes:
            {
            iGetInkeyRsp.iDuration.iTimeUnit = RSat::EMinutes;

            // Make Roundup
            TUint numOfUnits(
                ( aDuration + KHalfMinute ) / KTenthOfSecondsInMinute );

            if ( KByteMax >= numOfUnits )
                {
                LOG( SIMPLE, 
                "SATINTERNALCLIENT: CSatCGetInkeyHandler::TenthOfSecondsToDuration \
                EMinutes KByteMax >= numOfUnits" )
                iGetInkeyRsp.iDuration.iNumOfUnits =
                    static_cast<TUint8>( numOfUnits );
                }
            else
                {
                iGetInkeyRsp.iDuration.iNumOfUnits = KByteMax;
                }

            break;
            }
        case RSat::ESeconds:
            {
            iGetInkeyRsp.iDuration.iTimeUnit = RSat::ESeconds;

            // Make Roundup
            TUint numOfUnits(
                ( aDuration + KHalfSecond ) / KSecond );

            if ( KByteMax >= numOfUnits )
                {
                LOG( SIMPLE, 
                "SATINTERNALCLIENT: CSatCGetInkeyHandler::TenthOfSecondsToDuration \
                ESeconds KByteMax >= numOfUnits" )
                iGetInkeyRsp.iDuration.iNumOfUnits =
                    static_cast<TUint8>( numOfUnits );
                }
            else
                {
                iGetInkeyRsp.iDuration.iNumOfUnits = KByteMax;
                }

            break;
            }
        case RSat::ETenthsOfSeconds:
            {
            iGetInkeyRsp.iDuration.iTimeUnit = RSat::ETenthsOfSeconds;
            if ( KByteMax >= aDuration )
                {
                LOG( SIMPLE, 
                "SATINTERNALCLIENT: CSatCGetInkeyHandler::TenthOfSecondsToDuration \
                ETenthsOfSeconds KByteMax >= aDuration" )
                iGetInkeyRsp.iDuration.iNumOfUnits =
                    static_cast<TUint8>( aDuration );
                }
            else
                {
                iGetInkeyRsp.iDuration.iNumOfUnits = KByteMax;
                }

            break;
            }
        default:
            {
            break;
            }
        }
    LOG( SIMPLE, 
    "SATINTERNALCLIENT: CSatCGetInkeyHandler::TenthOfSecondsToDuration exiting" )
    }
