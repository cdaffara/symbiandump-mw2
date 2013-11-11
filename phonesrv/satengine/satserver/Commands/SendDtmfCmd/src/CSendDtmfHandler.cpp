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
* Description:  Handles SendDtmf command
*
*/


#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatSystemState.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "CSendDtmfHandler.h"
#include    "csatdtmfsender.h"
#include    "msatmultimodeapi.h"
#include    "SatLog.h"

const TUint KBcdZero      = 0x00;
const TUint KBcdOne       = 0x01;
const TUint KBcdTwo       = 0x02;
const TUint KBcdThree     = 0x03;
const TUint KBcdFour      = 0x04;
const TUint KBcdFive      = 0x05;
const TUint KBcdSix       = 0x06;
const TUint KBcdSeven     = 0x07;
const TUint KBcdEight     = 0x08;
const TUint KBcdNine      = 0x09;
const TUint KBcdStar      = 0x0A;
const TUint KBcdHash      = 0x0B;
const TUint KBcdPause     = 0x0C;
const TUint KBcdWild      = 0x0D;
const TUint KBcdExpansion = 0x0E;
const TUint KBcdEnd       = 0x0F;

const TUint KUcs2Zero     = 0x30; // '0'
const TUint KUcs2Star     = 0x2A; // '*'
const TUint KUcs2Hash     = 0x23; // '#'
const TUint KUcs2Pause    = 0x70; // 'p'

const TUint KHighNibble   = 0xF0;
const TUint KLowNibble    = 0x0F;
const TUint KBitsInNibble = 4;

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSendDtmfHandler* CSendDtmfHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::NewL calling" )

    CSendDtmfHandler* self = new( ELeave ) CSendDtmfHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::NewL exiting" )
    return self;
    }


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CSendDtmfHandler::~CSendDtmfHandler()
    {
    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::~CSendDtmfHandler calling" )

    Cancel();

    if ( iDtmfSender )
        {
        delete iDtmfSender;
        }

    if ( iDtmfStringUcs2 )
        {
        delete iDtmfStringUcs2;
        iDtmfStringUcs2 = NULL;
        }

    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::~CSendDtmfHandler exiting" )
    }

// -----------------------------------------------------------------------------
// Gets the result of DTMF sending.
// -----------------------------------------------------------------------------
//
void CSendDtmfHandler::SendDtmfComplete( const TInt aErrCode )
    {
    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::SendDtmfComplete calling" )

    iDtmfResult = aErrCode;
    // Handles result and sends terminal response.
    HandleResult();

    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::SendDtmfComplete exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatCommand.
// Response from the client
// -----------------------------------------------------------------------------
//
void CSendDtmfHandler::ClientResponse()
    {
    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::ClientResponse calling" )

    iClientResponseReceived = ETrue;
    if( iWait.IsStarted() )
        {
        iWait.AsyncStop();
        }

    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::ClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Waits for indication of user rejection
// -----------------------------------------------------------------------------
//
void CSendDtmfHandler::Event( TInt aEvent )
    {
    LOG2( SIMPLE,
        "SENDTMF: CSendDtmfHandler::Event calling, aEvent:%d", aEvent )

    switch ( aEvent )
        {
        case MSatUtils::ECommandCancelled:
            {
            // Cancel DTMF sending
            if ( iDtmfSender )
                {
                LOG( SIMPLE, 
                "SENDTMF: CSendDtmfHandler::Event Cancel DTMF sending" )
                iDtmfSender->Cancel();
                }

            // Cancel key pressed. Send terminal response.
            iSendDtmfRsp.iGeneralResult = RSat::KPSessionTerminatedByUser;
            iSendDtmfRsp.iInfoType = RSat::KNoAdditionalInfo;
            iSendDtmfRsp.iAdditionalInfo.Zero();

            SendTerminalResponse();
            break;
            }

        case MSatUtils::ECancelledUsingEndKey:
            {
            // Cancel DTMF sending
            if ( iDtmfSender )
                {
                LOG( SIMPLE, 
                "CSendDtmfHandler::Event Cancel DTMF sending by endkey" )
                iDtmfSender->Cancel();
                }

            // End key pressed. Send terminal response.
            iSendDtmfRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iSendDtmfRsp.iInfoType = RSat::KMeProblem;
            iSendDtmfRsp.iAdditionalInfo.Zero();
            iSendDtmfRsp.iAdditionalInfo.Append( RSat::KNotInSpeechCall );

            // Next SimSession end will close the ui session
            iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
            SendTerminalResponse();
            break;
            }

        default:
            {
            // Move event to base class
            CSatCommandHandler::Event( aEvent );
            }
        }

    LOG( SIMPLE, "SENDTMF: CSendDtmfHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CSendDtmfHandler::DoCancel()
    {
    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::DoCancel calling" )

    iUtils->USatAPI().NotifySendDtmfCancel();

    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Requests the command notification.
// -----------------------------------------------------------------------------
//
void CSendDtmfHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iSendDtmfData) RSat::TSendDtmfV1();

    if ( iDtmfStringUcs2 )
        {
        LOG( SIMPLE, 
        "CSendDtmfHandler::IssueUSATRequest iDtmfStringUcs2 true" )
        delete iDtmfStringUcs2;
        iDtmfStringUcs2 = NULL;
        }

    iUtils->USatAPI().NotifySendDtmf( aStatus, iSendDtmfPckg );

    // Unregister from events
    iUtils->UnregisterEvent( this, MSatUtils::ECommandCancelled );
    iUtils->UnregisterEvent( this, MSatUtils::ECancelledUsingEndKey );

    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// SendDtmf command is not allowed if there is not active speech call.
// -----------------------------------------------------------------------------
//
TBool CSendDtmfHandler::CommandAllowed()
    {
    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::CommandAllowed calling" )

    // By default, this is true
    TBool commandAllowed( ETrue );

    // Allow next terminal response to be sent
    iTerminalRespSent = EFalse;

    // This is needed every time
    iSendDtmfRsp.SetPCmdNumber( iSendDtmfData.PCmdNumber() );

    // Call has to be established, otherwise DTMF cannot be sent.
#ifdef SAT_USE_DUMMY_TSY
    TInt firstChar( ' ' );
    if ( iSendDtmfData.iAlphaId.iStatus == RSat::EAlphaIdProvided )
        {
        firstChar = iSendDtmfData.iAlphaId.iAlphaId[0];
        }
    const TBool callIsOngoing( firstChar != 'T' );
#else
    const TBool callIsOngoing( iUtils->SystemState().IsCallActive() );
#endif

    const TInt dtmfLength( iSendDtmfData.iDtmfString.Length() );

    // DTMF string shall have at least one BCD character.
    // Otherwise, minimum set of data objects is not complete,
    // and command has to be rejected with 'Error, required
    // values are missing'.
    const TBool dtmfStringHasContent( dtmfLength > 0 );

    // DTMF string shall not be longer than is possible
    // according to TS 11.14, clause 6.6.24. If the string is
    // longer than this, the DTMF String data object is not valid,
    // which means that the minimum set of data objects
    // is not complete, and the command has to be rejected
    // with 'Error, required values are missing'.
    const TBool dtmfStringNotTooLong( dtmfLength <= RSat::KDtmfStringMaxSize );

    // If icon data without alpha id.
    if ( ( RSat::EAlphaIdProvided != iSendDtmfData.iAlphaId.iStatus ) &&
        ( ( RSat::ESelfExplanatory == iSendDtmfData.iIconId.iQualifier ) ||
        ( RSat::ENotSelfExplanatory == iSendDtmfData.iIconId.iQualifier ) ) )
        {
        commandAllowed = EFalse;
        iSendDtmfRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
        iSendDtmfRsp.iInfoType = RSat::KNoAdditionalInfo;
        iSendDtmfRsp.iAdditionalInfo.Zero();
        LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::CommandAllowed data not \
            understood" )
        }
    else if ( !( dtmfStringHasContent && callIsOngoing && dtmfStringNotTooLong ) )
        { // Handle the error cases.

        if ( !dtmfStringHasContent || !dtmfStringNotTooLong )
            {
            LOG( SIMPLE,
                "SENDDTMF: !dtmfStringHasContent || !dtmfStringNotTooLong" )
            iSendDtmfRsp.iInfoType = RSat::KNoAdditionalInfo;
            iSendDtmfRsp.iAdditionalInfo.Zero();
            iSendDtmfRsp.iGeneralResult = RSat::KErrorRequiredValuesMissing;
            }
        else // No callIsOngoing
            {
            LOG( SIMPLE, "SENDDTMF: !callIsOngoing" )
            // ME is not in speech call and dtmf string cannot be sent.
            iSendDtmfRsp.iInfoType = RSat::KMeProblem;
            iSendDtmfRsp.iAdditionalInfo.Zero();
            iSendDtmfRsp.iAdditionalInfo.Append( RSat::KNotInSpeechCall );
            iSendDtmfRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            }
        // Command is not allowed
        commandAllowed = EFalse;
        }
    else
        {
        // Convert DTMF string. This is done here, so UI is not launched.
        // for no reason.

        // At the ETel interface level, the DTMF string uses a TBuf where each
        // buffer character represents two DTMF digits coded as BCD characters.
        const TUint KDtmfStringETelNumberOfBcdPerBufferCharacter = 2;

        // The DTMF string uses a TBuf where each buffer character represents
        // one DTMF digit.
        const TUint KDtmfStringMaxLength =
            KDtmfStringETelNumberOfBcdPerBufferCharacter *
                RSat::KDtmfStringMaxSize;

        TRAPD( noMemory, iDtmfStringUcs2 =
            HBufC16::NewL( KDtmfStringMaxLength ) )

        if ( KErrNone != noMemory )
            {
            LOG( SIMPLE, 
            "SENDDTMF: CSendDtmfHandler::CommandAllowed KErrNone != noMemory" )
            iSendDtmfRsp.iInfoType = RSat::KNoAdditionalInfo;
            iSendDtmfRsp.iAdditionalInfo.Zero();
            iSendDtmfRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            // Command is not allowed
            commandAllowed = EFalse;
            }
        else
            {

            TPtr ptr = iDtmfStringUcs2->Des();

            if ( ! ConvertDtmfStringToUcs2( ptr ) )
                {
                LOG( SIMPLE, 
                "CSendDtmfHandler::CommandAllowed not allowed" )
                // DTMF string contains illegal characters
                iSendDtmfRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
                iSendDtmfRsp.iInfoType = RSat::KNoAdditionalInfo;
                iSendDtmfRsp.iAdditionalInfo.Zero();
                // Command is not allowed
                commandAllowed = EFalse;
                }
            }
        }

    if ( !commandAllowed )
        {
        LOG( SIMPLE, 
        "CSendDtmfHandler::CommandAllowed commandAllowed false" )
        SendTerminalResponse();
        }
    // Set icon command flag whether icon data was received and set qualifier
    // to no icon id
    // To be removed when icons are allowed in this command
    else if ( ( RSat::ESelfExplanatory ==
        iSendDtmfData.iIconId.iQualifier ) ||
        ( RSat::ENotSelfExplanatory ==
        iSendDtmfData.iIconId.iQualifier ) )
        {
        LOG( SIMPLE, 
        "CSendDtmfHandler::CommandAllowed ENoIconId" )
        iIconCommand = ETrue;
        iSendDtmfData.iIconId.iQualifier = RSat::ENoIconId;
        }
    else
        {
        iIconCommand = EFalse;
        }

    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::CommandAllowed exiting" )
    return commandAllowed;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Need for ui session.
// -----------------------------------------------------------------------------
//
TBool CSendDtmfHandler::NeedUiSession()
    {
    LOG( NORMAL, "SENDDTMF: CSendDtmfHandler::NeedUiSession calling" )
    iNeedUiSession = ETrue;

    if ( RSat::EAlphaIdNull == iSendDtmfData.iAlphaId.iStatus )
        {
        LOG( NORMAL, "SENDDTMF: CSendDtmfHandler::NeedUiSession EAlphaIdNull" )
        iNeedUiSession = EFalse;
        }

    // Notify Cover UI if it's supported
    if ( iNeedUiSession && iUtils->CoverUiSupported() )
        {
        LOG( NORMAL, 
        "SENDDTMF: CSendDtmfHandler::NeedUiSession CoverUiSupported" )
        TSatCommandData medEventData;
        medEventData.iPCmdNumber = RSat::ESendDtmf;
        medEventData.iAlphaId =  iSendDtmfData.iAlphaId;
        medEventData.iDuration.iNumOfUnits = KSatDefaultDuration;
        medEventData.iDuration.iTimeUnit = RSat::ESeconds;
        medEventData.iIconID = iSendDtmfData.iIconId;
        TSatCommandPckg tPckg( medEventData );
        iUtils->RaiseSatEvent( tPckg );
        }

    LOG2( NORMAL, 
    "SENDDTMF: CSendDtmfHandler::NeedUiSession exiting,iNeedUiSession: %d",
    iNeedUiSession )
    return iNeedUiSession;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Called when USAT API notifies that command.
// -----------------------------------------------------------------------------
//
void CSendDtmfHandler::HandleCommand()
    {
    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::HandleCommand calling" )

    iUtils->NotifyEvent( MSatUtils::ESendDtmfExecuting );

    // Lower error granularity. This helps for mapping error cases
    iUtils->MultiModeApi().LowerErrorGranularity();

    // Pointer to converted DTMF string
    TPtr ptr = iDtmfStringUcs2->Des();

    iClientResponseReceived = EFalse;
    
    // First, send DTMF string
    iDtmfSender->SendToneString( ptr );

    if ( iNeedUiSession )
        {
        TRAPD( regErr,
        // Register to listen user cancel events:
        // Cancel key event from dialog
        iUtils->RegisterL( this, MSatUtils::ECommandCancelled );
        // End key from dialog
        iUtils->RegisterL( this, MSatUtils::ECancelledUsingEndKey );

        // Then send notification to UI
        // Register notification observer
        iUtils->RegisterServiceRequestL(
            ESatSProactiveNotification,
            ESatSProactiveNotificationResponse,
            this )
            ); // TRAPD
        LOG2( NORMAL, 
        "CSendDtmfHandler::HandleCommand regErr: %d", regErr )
        if ( KErrNone != regErr )
            {
            iDtmfSender->Cancel();
            // Raise error granularity back to extended errors
            iUtils->MultiModeApi().RaiseErrorGranularity();

            iSendDtmfRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iSendDtmfRsp.iInfoType = RSat::KMeProblem;
            iSendDtmfRsp.iAdditionalInfo.Zero();
            iSendDtmfRsp.iAdditionalInfo.Append( RSat::KNoSpecificMeProblem );
            SendTerminalResponse();
            }
        else
            {
            TSatAlphaIdStatus alphaIdStatus;
            if ( iSendDtmfData.iAlphaId.iStatus == RSat::EAlphaIdNotPresent )
                {
                LOG( SIMPLE, 
                "SENDDTMF: CSendDtmfHandler::HandleCommand EAlphaIdNotPresent" )
                // If AlphaID is not provided, show DTMF string in note.
                // Deliver empty text if DTMF string is bigger than it is
                // capable to deliver to SAT UI.
                alphaIdStatus = ESatAlphaIdNotProvided;
                LOG2( NORMAL, 
                "SENDDTMF: CSendDtmfHandler::HandleCommand ptr.Length: %d", 
                ptr.Length() )
                if ( RSat::KAlphaIdMaxSize >= ptr.Length() )
                    {
                    iNotificationData.iText = ptr;
                    }
                else
                    {
                    iNotificationData.iText.Zero();
                    }
                }
            else if ( iSendDtmfData.iAlphaId.iStatus == RSat::EAlphaIdProvided )
                {
                LOG( SIMPLE, 
                "SENDDTMF: CSendDtmfHandler::HandleCommand EAlphaIdProvided" )
                alphaIdStatus = ESatAlphaIdNotNull;
                iNotificationData.iText = iSendDtmfData.iAlphaId.iAlphaId;
                }
            else
                {
                LOG( SIMPLE, 
                "SENDDTMF: CSendDtmfHandler::HandleCommand others" )
                alphaIdStatus = ESatAlphaIdNull;
                iNotificationData.iText.Zero();
                }

            // Build notification data
            iNotificationData.iAlphaIdStatus = alphaIdStatus;
            iNotificationData.iCommand = ESatSSendDtmfNotify;

            // Has to be casted to TInt before casting to TSatIconQualifier,
            // because GCC warns about the direct cast.
            const struct TSatIconId iconId =
                { iSendDtmfData.iIconId.iIdentifier,
                  static_cast<TSatIconQualifier>(
                    static_cast<TInt>( iSendDtmfData.iIconId.iQualifier ) ) };

            iNotificationData.iIconId = iconId;

            // Send notification
            iUtils->SatUiHandler().UiSession()->SendCommand(
                &iNotificationDataPckg,
                &iNotificationRspPckg,
                ESatSProactiveNotification );

            // DTMF String sent, wait for completion
            }
        }

    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Called when UI launch fails.
// -----------------------------------------------------------------------------
//
void CSendDtmfHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::UiLaunchFailed calling" )

    iSendDtmfRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
    iSendDtmfRsp.iInfoType = RSat::KMeProblem;
    iSendDtmfRsp.iAdditionalInfo.Append( RSat::KNoSpecificMeProblem );
    iSendDtmfRsp.SetPCmdNumber( iSendDtmfData.PCmdNumber() );
    SendTerminalResponse();

    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::UiLaunchFailed exiting" )
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized, harmless.
CSendDtmfHandler::CSendDtmfHandler() :
    CSatCommandHandler(),
    iSendDtmfData(),
    iSendDtmfPckg( iSendDtmfData ),
    iSendDtmfRsp(),
    iSendDtmfRspPckg( iSendDtmfRsp ),
    iNotificationData(),
    iNotificationDataPckg( iNotificationData ),
    iNotificationRsp(),
    iNotificationRspPckg( iNotificationRsp ),
    // To be removed when icons are allowed in this command
    iIconCommand( EFalse )
    {
    LOG( SIMPLE,
        "SENDDTMF: CSendDtmfHandler::CSendDtmfHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSendDtmfHandler::ConstructL()
    {
    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::ConstructL calling" )

    // Register notification observer
    iUtils->RegisterServiceRequestL(
        ESatSProactiveNotification,
        ESatSProactiveNotificationResponse,
        this );

    // DTMF Sender
    iDtmfSender = 
        new ( ELeave ) CSatDtmfSender( *this, iUtils->MultiModeApi() );

    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::ConstructL exiting" )
    }


// -----------------------------------------------------------------------------
// Converts DTMF string from extended BCD to UCS2.
// -----------------------------------------------------------------------------
//
TBool CSendDtmfHandler::ConvertDtmfStringToUcs2( TDes& aDtmfString )
    {
    LOG( SIMPLE,
        "SENDDTMF: CSendDtmfHandler::ConvertDtmfStringToUcs2 calling" )
    aDtmfString.Zero();

    const TInt bcdLength = iSendDtmfData.iDtmfString.Length();
    TInt i( 0 );
    TBool retVal( ETrue );
    TBool continueLoop( ETrue );

    for ( i = 0; i < bcdLength && continueLoop; i++ )
        {
        // each 16 bit DTMF character of SAT ETel API holds two nibbles,
        // high byte always being 0
        TChar twoBcd = iSendDtmfData.iDtmfString[i];

        // low nibble yields first BCD in semi-octet representation
        TChar firstBcd = twoBcd & KLowNibble;

        // high nibble yields second BCD in semi-octet representation
        TChar secondBcd = ( twoBcd & KHighNibble ) >> KBitsInNibble;

        TChar destChar;
        TBool endOfString;
        TBool dtmfCharacter;

        // convert first extended BCD character
        ConvertDtmfCharacterToUcs2(
            firstBcd, endOfString, dtmfCharacter, destChar );

        if ( endOfString )
            {
            LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::ConvertDtmfStringToUcs2 \
                (End of string in first BCD)" )
            retVal = ETrue; // completed string (F digit)
            continueLoop = EFalse;
            }
        else if ( !dtmfCharacter )
            {
            LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::ConvertDtmfStringToUcs2 \
                (not dtmfCharacter in first BCD)" )
            retVal = EFalse;
            continueLoop = EFalse;
            }
        else
            {
            aDtmfString.Append( destChar );

            // convert second extended BCD character
            ConvertDtmfCharacterToUcs2(
                secondBcd, endOfString, dtmfCharacter, destChar );

            if ( endOfString )
                {
                LOG( SIMPLE,
                    "SENDDTMF: CSendDtmfHandler::ConvertDtmfStringToUcs2 \
                    (End of string in second BCD)" )
                retVal = ETrue; // completed string (F digit)
                continueLoop = EFalse;
                }
            else if ( !dtmfCharacter )
                {
                LOG( SIMPLE,
                    "SENDDTMF: CSendDtmfHandler::ConvertDtmfStringToUcs2 \
                    (not dtmfCharacter in second BCD)" )
                retVal = EFalse;
                continueLoop = EFalse;
                }
            else
                {
                LOG( SIMPLE,
                    "SENDDTMF: CSendDtmfHandler::ConvertDtmfStringToUcs2 \
                    others" )
                aDtmfString.Append( destChar );
                }
            }
        }
    LOG2( NORMAL, 
        "CSendDtmfHandler::ConvertDtmfStringToUcs2 i: %d", i )
    LOG2( SIMPLE,
        "SENDDTMF: CSendDtmfHandler::ConvertDtmfStringToUcs2 exiting,\
        retVal: %d", retVal )
    return retVal; // completed string (all bytes converted)
    }

// -----------------------------------------------------------------------------
// Converts character from extended BCD to UCS2.
// -----------------------------------------------------------------------------
//
void CSendDtmfHandler::ConvertDtmfCharacterToUcs2(
    const TChar& aExtBcd,
    TBool& aEndOfString,
    TBool& aDtmfCharacter,
    TChar& aUcs2Char ) const
    {
    LOG( SIMPLE,
        "SENDDTMF: CSendDtmfHandler::ConvertDtmfCharacterToUcs2 calling" )

    aEndOfString = EFalse;
    aDtmfCharacter = ETrue;

    switch ( aExtBcd )
        {
        case KBcdZero:
        case KBcdOne:
        case KBcdTwo:
        case KBcdThree:
        case KBcdFour:
        case KBcdFive:
        case KBcdSix:
        case KBcdSeven:
        case KBcdEight:
        case KBcdNine:
            {
            LOG( SIMPLE,
            "SENDDTMF: CSendDtmfHandler::ConvertDtmfCharacterToUcs2 Digit" )
            aUcs2Char = KUcs2Zero + aExtBcd;
            break;
            }

        case KBcdStar:
            {
            LOG( SIMPLE,
            "SENDDTMF: CSendDtmfHandler::ConvertDtmfCharacterToUcs2 KBcdStar" )
            aUcs2Char = KUcs2Star;
            break;
            }

        case KBcdHash:
            {
            LOG( SIMPLE,
            "SENDDTMF: CSendDtmfHandler::ConvertDtmfCharacterToUcs2 KBcdHash" )
            aUcs2Char = KUcs2Hash;
            break;
            }

        case KBcdPause:
            {
            LOG( SIMPLE,
            "SENDDTMF: CSendDtmfHandler::ConvertDtmfCharacterToUcs2 KBcdPause" )
            aUcs2Char = KUcs2Pause;
            break;
            }

        case KBcdEnd:
            {
            LOG( SIMPLE,
            "SENDDTMF: CSendDtmfHandler::ConvertDtmfCharacterToUcs2 KBcdEnd" )
            aEndOfString = ETrue;
            break;
            }

        case KBcdWild:
        case KBcdExpansion:
        default:
            {
            LOG( SIMPLE,
            "SENDDTMF: CSendDtmfHandler::ConvertDtmfCharacterToUcs2 unknown" )
            aDtmfCharacter = EFalse;
            }
        }

    LOG( SIMPLE,
        "SENDDTMF: CSendDtmfHandler::ConvertDtmfCharacterToUcs2 exiting" )
    }

// -----------------------------------------------------------------------------
// Handles the result of DTMF sending and sends terminal response.
// -----------------------------------------------------------------------------
//
void CSendDtmfHandler::HandleResult()
    {
    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::HandleResult calling" )

    // If UI is needed, send DTMF Done event to close any progress bar
    if ( iNeedUiSession )
        {
        LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::HandleResult NeedUiSession" )
        // To prevent client traffic jamming.
        // Send UI event only when client response is received.
        if ( !iClientResponseReceived && !iWait.IsStarted() )
            {
            LOG( SIMPLE, 
            "SENDDTMF: CSendDtmfHandler::HandleResult start iwait" )
            iWait.Start();
            }
        // No need to send command result since this only removes any
        // existing wait notes...
        iUtils->NotifyUiEvent( ESatSDtmfEndEvent, ESatEventNone, KErrNone );
        }

    // Raise error granularity back to extended errors
    iUtils->MultiModeApi().RaiseErrorGranularity();

#ifdef __WINS__
    if ( KErrTimedOut == iDtmfResult )
        {
        iDtmfResult = KErrNone;
        }
#endif

    LOG2( NORMAL, 
    "SENDDTMF: CSendDtmfHandler::HandleResult Send DTMF result: %i", 
    iDtmfResult )

    switch ( iDtmfResult )
        {
        case KErrNone:
            {
            // No errors in DTMF sending. But cases like user termination and
            // icons are handled here.
            if ( iNotificationRsp.iSessionTerminatedByUser )
                {
                LOG( DETAILED,
                "SENDDTMF: CSendDtmfHandler::HandleResult session terminated \
                by user" )
                iSendDtmfRsp.iGeneralResult = RSat::KPSessionTerminatedByUser;
                iSendDtmfRsp.iInfoType = RSat::KNoAdditionalInfo;
                iSendDtmfRsp.iAdditionalInfo.Zero();
                }
            else
                {
                iSendDtmfRsp.iGeneralResult = RSat::KSuccess;
                iSendDtmfRsp.iInfoType = RSat::KNoAdditionalInfo;
                iSendDtmfRsp.iAdditionalInfo.Zero();

                // If command had icon data and was done succesfully,
                // report that icon was not shown.
                // To be removed and correct handling (i.e. ClientResponse to
                // notification is received) for general result
                // KSuccessRequestedIconNotDisplayed must be added when icons
                // are allowed in this command.
                if ( iIconCommand )
                    {
                    LOG( DETAILED,
                    "SENDDTMF: CSendDtmfHandler::HandleResult iIconCommand \
                    true" )
                    iSendDtmfRsp.iGeneralResult =
                        RSat::KSuccessRequestedIconNotDisplayed;
                    }
                }

            break;
            }
        case KErrCancel:
        case KErrEtelNoCarrier:
            {
            // Problems in communicating with receiving device are handled here
            // e.g. call was unexpectedly dropped.
            iSendDtmfRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iSendDtmfRsp.iInfoType = RSat::KMeProblem;
            iSendDtmfRsp.iAdditionalInfo.Zero();
            iSendDtmfRsp.iAdditionalInfo.Append( RSat::KNoSpecificMeProblem );
            break;
            }
        case KErrArgument:
            {
            // Maximum length of DTMF string reached.
            LOG( DETAILED,
                 "SENDDTMF: CSendDtmfHandler  Over maximum DTMF length" )
            iSendDtmfRsp.iGeneralResult = RSat::KErrorRequiredValuesMissing;
            iSendDtmfRsp.iInfoType = RSat::KNoAdditionalInfo;
            iSendDtmfRsp.iAdditionalInfo.Zero();
            break;
            }
        default:
            {
            // By default for other errors.
            iSendDtmfRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iSendDtmfRsp.iInfoType = RSat::KMeProblem;
            iSendDtmfRsp.iAdditionalInfo.Zero();
            iSendDtmfRsp.iAdditionalInfo.Append( RSat::KNotInSpeechCall );
            break;
            }
        }

    // Cancel DTMF sending if it is running.
    iDtmfSender->Cancel();
    SendTerminalResponse();

    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::HandleResult exiting" )
    }

// -----------------------------------------------------------------------------
// Sends terminal response. Makes sure that terminal response
// is not send more that once / command.
// -----------------------------------------------------------------------------
//
void CSendDtmfHandler::SendTerminalResponse()
    {
    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::SendTerminalResponse calling" )

    if ( !iTerminalRespSent )
        {
        LOG( SIMPLE, 
        "CSendDtmfHandler::SendTerminalResponse iTerminalRespSent false" )
        iTerminalRespSent = ETrue;
        TerminalRsp( RSat::ESendDtmf, iSendDtmfRspPckg );
        }

    LOG( SIMPLE, "SENDDTMF: CSendDtmfHandler::SendTerminalResponse exiting" )
    }

