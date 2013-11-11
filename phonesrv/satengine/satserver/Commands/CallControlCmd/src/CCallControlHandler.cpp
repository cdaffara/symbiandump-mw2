/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles CallControl command
*
*/


//Include Files
#include    <utf.h>
#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatUiSession.h"
#include    "MSatSUiClientHandler.h"
#include    "CCallControlHandler.h"
#include    "ccallcontrolrequesthandler.h"
#include    "msatmultimodeapi.h"
#include    "SatLog.h"

// USSD DCS coding.
const TUint8 KSatDcs7Bit( 0x40 );

 /** 
  * USSD messages coded as a packed string within 160 octets, as defined for a 
  * ussd-String within GSM 04.80 and GSM 03.38. if the Dcs is 7 bit, a ussd 
  * string can have 182 charactor
  */
const TInt   KSatMaxUSSDString( 182 );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCallControlHandler::CCallControlHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized, harmless.
CCallControlHandler::CCallControlHandler() :
    CSatCommandHandler(),
    iCallControlData(),
    iCallControlPckg( iCallControlData ),
    iCallControlSendData(),
    iCallControlSendDataPckg( iCallControlSendData ),
    iCallControlUiRespData(),
    iCallControlUiRespDataPckg( iCallControlUiRespData ),
    iCcStatus( ECcIdle )
    {
    LOG( SIMPLE, "CALLCONTROL: \
        CCallControlHandler::CCallControlHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CCallControlHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCallControlHandler::ConstructL()
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::ConstructL calling" )

    iUtils->RegisterServiceRequestL(
        ESatSProactiveNotification,
        ESatSProactiveNotificationResponse,
        this );

    iRequestHandler = 
        CCallControlRequestHandler::NewL( iUtils->MultiModeApi(), this ); 

    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CCallControlHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCallControlHandler* CCallControlHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::NewL calling" )

    CCallControlHandler* self = new( ELeave ) CCallControlHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::NewL exiting" )
    return self;
    }


// -----------------------------------------------------------------------------
// CCallControlHandler::~CCallControlHandler
// Destructor
// -----------------------------------------------------------------------------
CCallControlHandler::~CCallControlHandler()
    {
    LOG( SIMPLE,
        "CALLCONTROL: CCallControlHandler::~CCallControlHandler calling" )

    Cancel();

    LOG( SIMPLE,
        "CALLCONTROL: CCallControlHandler::~CCallControlHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CCallControlHandler::ClientResponse
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCallControlHandler::ClientResponse()
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::ClientResponse calling" )

    CallControlClientOperation();    

    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::ClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// CCallControlHandler::DoCancel
// Cancels the sat request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCallControlHandler::DoCancel()
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::DoCancel calling" )

    iUtils->USatAPI().NotifyCallControlCancel();
    iRequestHandler->CancelOperation( iCcStatus ); 

    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CCallControlHandler::IssueUSATRequest
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCallControlHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iCallControlData) RSat::TCallControlV6();
    iNeedUiSession = EFalse;
    iUtils->USatAPI().NotifyCallControl( aStatus, iCallControlPckg );

    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// CCallControlHandler::CommandAllowed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCallControlHandler::CommandAllowed()
    {
    LOG( SIMPLE,
        "CALLCONTROL: CCallControlHandler::CommandAllowed calling - exiting" )
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CCallControlHandler::NeedUiSession
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCallControlHandler::NeedUiSession()
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::NeedUiSession calling" )
    // UI is not needed, if control is AllowedWithMod and AlphaId is NULL or
    // control is Allowed and AlphaId is not provided

    //  Get values
    RSat::TAlphaId alphaId;
    RSat::TAlphaIdValidity validity;
    RSat::TControlResult controlResult;

    // Get Alpha Id and Control result by SIM.
    iCallControlPckg().GetAlphaId( validity, alphaId );
    iCallControlPckg().GetCcGeneralResult( controlResult );

    const TBool alphaIdNull( RSat::EAlphaIdNull == alphaId.iStatus ||
                             RSat::EAlphaIdNotSet == alphaId.iStatus );

    const TBool controlAllowed( 
                RSat::EAllowedNoModification == controlResult ||
                RSat::EControlResultNotSet == controlResult );
    LOG2( NORMAL, "CALLCONTROL: CCallControlHandler::NeedUiSession \
            controlResult: %d", controlResult )
    LOG2( NORMAL, "CALLCONTROL: CCallControlHandler::NeedUiSession \
            alphaId.iStatus: %d", alphaId.iStatus )    

    if ( RSat::EAllowedWithModifications == controlResult && 
         alphaIdNull )
        {
        LOG( NORMAL, "CALLCONTROL: CCallControlHandler::NeedUiSession \
             EAllowedWithModifications" )
        iNeedUiSession = EFalse;
        }
    else if ( controlAllowed && ( RSat::EAlphaIdProvided != alphaId.iStatus ) )
        {
        LOG( NORMAL, "CALLCONTROL: CCallControlHandler::NeedUiSession \
             RSat::EAlphaIdProvided != alphaId.iStatus" )
        iNeedUiSession = EFalse;
        }
    else
        {
        iNeedUiSession = ETrue;
        }

    // Notify other commands that we are executing.
    iUtils->NotifyEvent( MSatUtils::ECallControlExecuting );

    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::NeedUiSession exiting" )
    return iNeedUiSession;
    }

// -----------------------------------------------------------------------------
// CCallControlHandler::HandleCommand
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCallControlHandler::HandleCommand()
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::HandleCommand calling" )

    // Check do we need UI
    if ( iNeedUiSession )
        {
        // Clear send data.
        TSatNotificationV1 clear;
        iCallControlSendData = clear;

        //  Build the IPC package.
        RSat::TAlphaId alphaId;
        RSat::TAlphaIdValidity validity;
        RSat::TControlResult controlResult;

        // Get Alpha Id and Control result by SIM.
        iCallControlPckg().GetAlphaId( validity, alphaId );
        iCallControlPckg().GetCcGeneralResult( controlResult );
        
        // Enums to UI related parameters
        TSatAlphaIdStatus satAlphaIdStatus;
        TSatControlResult satControlResult;

        LOG2( NORMAL, "CALLCONTROL: CCallControlHandler::HandleCommand\
                alphaId.iStatus: %d", alphaId.iStatus ) 
        LOG2( NORMAL, "CALLCONTROL: CCallControlHandler::HandleCommand\
                controlResult: %d", controlResult )
        // Map RSat value
        switch ( alphaId.iStatus )
            {
            case RSat::EAlphaIdProvided :
                {
                satAlphaIdStatus = ESatAlphaIdNotNull;
                break;
                }

            case RSat::EAlphaIdNotPresent :
                {
                satAlphaIdStatus = ESatAlphaIdNotProvided;
                break;
                }

            // Use this also as a default value
            case RSat::EAlphaIdNull :
            case RSat::EAlphaIdNotSet :
            default :
                {
                satAlphaIdStatus = ESatAlphaIdNull;
                break;
                }
            }

        // Map RSat value
        switch ( controlResult )
            {
            case RSat::ENotAllowed :
                {
                satControlResult = ESatNotAllowed;
                break;
                }

            case RSat::EAllowedWithModifications :
                {
                satControlResult = ESatAllowedWithModifications;
                break;
                }

            // Use this also as a default value
            case RSat::EAllowedNoModification :
            case RSat::EControlResultNotSet :
            default :
                {
                satControlResult = ESatAllowedNoModification;
                break;
                }
            }

        // Get information needed.
        iCallControlSendData.iCommand = ESatSCallControlNotify;
        iCallControlSendData.iText = alphaId.iAlphaId;
        iCallControlSendData.iAlphaIdStatus = satAlphaIdStatus;
        iCallControlSendData.iControlResult = satControlResult;

        // Register notification observer
        TRAPD( regErr, iUtils->RegisterServiceRequestL(
            ESatSProactiveNotification,
            ESatSProactiveNotificationResponse,
            this )
            ); // TRAPD
        LOG2( NORMAL, "CALLCONTROL: CCallControlHandler::HandleCommand \
                regErr: %d",regErr )
        // No need to check error value, since we cannot send terminal
        // response for this command
        if ( KErrNone == regErr )
            {
            // Get UISession.
            MSatUiSession* uiSession = iUtils->SatUiHandler().UiSession();

            // Send command to SatClient.
            uiSession->SendCommand( &iCallControlSendDataPckg,
                &iCallControlUiRespDataPckg, ESatSProactiveNotification );
            }
        }
    else
        {
        LOG( NORMAL, "CCallControlHandler::HandleCommand UI isn't needed" )
        CallControlClientOperation();
        }
    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// CCallControlHandler::UiLaunchFailed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCallControlHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::UiLaunchFailed calling" )

    // We cannot send terminal response to SIM since it doesn't
    // expect terminal response from MoSmControl command...

    // Inform the session that we are done here
    iUtils->NotifyEvent( MSatUtils::EDelaySimSessionEnd );

    // Start to receive more commands.
    Start();

    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::UiLaunchFailed exiting" )
    }

// -----------------------------------------------------------------------------
// CCallControlHandler::CallControlClientOperation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCallControlHandler::CallControlClientOperation()
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::CallControlClientOperation\
     calling" )
    RSat::TCallParamOrigin controlOrigin;
    iCallControlData.GetCallParamOrigin( controlOrigin );
    LOG2( NORMAL, "CALLCONTROL: CCallControlHandler::\
        CallControlClientOperation GetCallParamOrigin = %d", controlOrigin )
    iCcStatus = ECcIdle;
   
    RSat::TActionOriginator actionOrigin;
    iCallControlData.GetActionOriginator( actionOrigin );
    LOG2( NORMAL, "CALLCONTROL: CCallControlHandler::\
          CallControlClientOperation GetActionOriginator: %d", actionOrigin )

    if ( RSat::EClientOriginator == actionOrigin )
        {
        RSat::TCallControlType controlType;
        iCallControlData.GetCallControlType( controlType );
        
        switch ( controlType )
            {
            case RSat::ECcAddress:
                {
                LOG( NORMAL, "CALLCONTROL: CCallControlHandler::\
                 CallControlClientOperation ControlType ECcArddress" )
                if ( KErrNone == DialNumber() )
                    {
                    iCcStatus = ECcDialWaiting;
                    }
                break;
                }
            case RSat::ECcSsString:
                {
                LOG( NORMAL, "CALLCONTROL: CCallControlHandler::\
                 CallControlClientOperation ControlType ECcSsString" )
                if ( KErrNone == SendSs() )
                    {
                    iCcStatus = ECcSendSsWaiting;
                    }
                break;
                }
            case RSat::ECcUssdString:
                {
                LOG( NORMAL, "CALLCONTROL: CCallControlHandler::\
                 CallControlClientOperation ControlType ECcUssdString" )
                if ( KErrNone == SendUssd() )
                    {
                    iCcStatus = ECcSendUssdWaiting;
                    }
                break;
                }
            default:
                {
                LOG2( NORMAL, "CALLCONTROL: CCallControlHandler::\
                 CallControlClientOperation ControlType %d", controlType )
                break;
                }
            }
        }
            
    if ( ECcIdle == iCcStatus )
        {
        CompleteCallControlCmd( KErrNone );
        }
    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::CallControlClientOperation\
         exiting" )
    }

// -----------------------------------------------------------------------------
// CCallControlHandler::DialNumber
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCallControlHandler::DialNumber()
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::DialNumber calling" )
    TInt err( KErrNone );
    iCcStatus = ECcDialWaiting;
    RSat::TCallSetUpParams callParams;
    err = iCallControlData.GetCallSetUpDetails ( callParams );
        
    if ( !err )
        {
        RMobileCall::TMobileCallParamsV7 dialParams;
        dialParams.iAutoRedial = ETrue;
        dialParams.iBearerMode = RMobileCall::EMulticallNewBearer;
        dialParams.iCallParamOrigin = RMobileCall::EOriginatorEtelClient;
        dialParams.iSubAddress = callParams.iSubAddress;
        dialParams.iBearerCap1 = callParams.iCcp1;
        dialParams.iBearerCap2 = callParams.iCcp2;        
        iCallControlData.GetBCRepeatIndicator( 
            reinterpret_cast< RSat::TBCRepeatIndicator& > 
            ( dialParams.iBCRepeatIndicator ) );
        dialParams.iIconId.iQualifier = RMobileCall::EIconQualifierNotSet;
        dialParams.iIconId.iIdentifier = 0;
        
        RSat::TAlphaId alphaid;
        RSat::TAlphaIdValidity validity;
        iCallControlData.GetAlphaId( validity, alphaid );
        
        if ( (RSat::EValidAlpaId == validity)
            && ( RSat::EAlphaIdProvided == alphaid.iStatus ) )
            {                   
            LOG2( NORMAL, 
                "CALLCONTROL: CCallControlHandler::DialNumber id:%S",
                &alphaid.iAlphaId )
            dialParams.iAlphaId = alphaid.iAlphaId;
            }        

        LOG2( NORMAL, 
            "CALLCONTROL: CCallControlHandler::DialNumber string %S",
            &callParams.iAddress.iTelNumber )

        
        RMobileCall::TMobileCallParamsV7Pckg package( dialParams );
        iRequestHandler->DialNumber( package, callParams.iAddress.iTelNumber );
        }
    LOG2( NORMAL, "CALLCONTROL: CCallControlHandler::DialNumber exiting %d",
          err)
    return err;
    }

// -----------------------------------------------------------------------------
// CCallControlHandler::SendSS
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCallControlHandler::SendSs()
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::SendSs calling" )
    TInt err( KErrNone );
    
    RSat::TSsString ssString;
    err = iCallControlData.GetSendSsDetails( ssString );
    if ( !err )
        {
        LOG2( NORMAL, "CALLCONTROL: CCallControlHandler::SendSs type %d",
          ssString.iTypeOfNumber )
        LOG2( NORMAL, "CALLCONTROL: CCallControlHandler::SendSs plan %d",
          ssString.iNumberPlan )

        HBufC16* ss16 =  HBufC16::New( ssString.iSsString.Length() );
        if ( ss16 )
            {
            TPtr16 ssPtr( ss16->Des() );
            CnvUtfConverter::ConvertToUnicodeFromUtf8( ssPtr,
                ssString.iSsString );  
    
            LOG2( NORMAL, "CALLCONTROL: CCallControlHandler::SendSs string %S",
              ss16 )
            iRequestHandler->SendSs( *ss16 );        
            delete ss16;
            }
        else
            {
            err = KErrNoMemory;
            }    
        }
    LOG2( NORMAL, "CALLCONTROL: CCallControlHandler::SendSs exiting %d", err)
    return err;
    }

// -----------------------------------------------------------------------------
// CCallControlHandler::SendUssd
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCallControlHandler::SendUssd()
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::SendUssd calling" )
    TInt err( KErrNone );
    RSat::TUssdString ussdString;
    err = iCallControlData.GetSendUssdDetails ( ussdString );
    //If the Dcs or string length is not valid we should not send the ussd.
    if( !err )
        {
        if( !( iUtils->MultiModeApi().IsValidUssdDcs( ussdString.iDcs ) )
            || ( KSatMaxUSSDString < ussdString.iUssdString.Length() ) )
            {   
            err = KErrArgument;
            }
        }
    
    if ( !err )
        {
        LOG2( NORMAL, "CALLCONTROL: CCallControlHandler::SendUssd string %S",
            &ussdString.iUssdString )
         
        RMobileUssdMessaging::TMobileUssdAttributesV1 ussdAttr;
        if ( KSatDcs7Bit == ussdString.iDcs )
            {
            ussdAttr.iFormat= RMobileUssdMessaging::EFormatPackedString;
            }
        else
            {
            ussdAttr.iFormat= RMobileUssdMessaging::EFormatUnspecified;                        
            }
        
        HBufC8* ussd8 =  HBufC8::New( ussdString.iUssdString.Length() );
        if ( ussd8 )
            {
            ussd8->Des().Copy( ussdString.iUssdString );
    
            ussdAttr.iFlags = RMobileUssdMessaging::KUssdDataFormat
               | RMobileUssdMessaging::KUssdMessageType
               | RMobileUssdMessaging::KUssdMessageDcs;
                
            ussdAttr.iDcs = ussdString.iDcs;
            ussdAttr.iType = RMobileUssdMessaging::EUssdMORequest;
            RMobileUssdMessaging::TMobileUssdAttributesV1Pckg 
                ussdPckg( ussdAttr );
    
            iRequestHandler->SendUssd ( *ussd8, ussdPckg );
            delete ussd8;
            }
        else
            {
            err = KErrNoMemory;
            }    
        }
    
    LOG2( NORMAL, "CALLCONTROL: CCallControlHandler::SendUssd exiting %d",
          err)
    return err;
    }

// -----------------------------------------------------------------------------
// CCallControlHandler::DispatchCcRequestComplete
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCallControlHandler::DispatchCcRequestComplete( TInt aErrCode )
    {
    LOG( SIMPLE, 
    "CALLCONTROL: CCallControlHandler::DispatchCcRequestComplete calling" )
    
    LOG2( NORMAL, 
    "CALLCONTROL: CCallControlHandler::DispatchCcRequestComplete aErrCode %d",
    aErrCode )

    iCcStatus = ECcIdle;
    CompleteCallControlCmd( aErrCode );
    
    LOG2( NORMAL, 
        "CALLCONTROL: CCallControlHandler::SendUssd exiting %d", aErrCode )
    }


// -----------------------------------------------------------------------------
// CCallControlHandler::CompleteCallControlCmd
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCallControlHandler::CompleteCallControlCmd( TInt /*aErrCode*/ )
    {
    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::CompleteCallControlCmd\
         calling" )

    // Start to receive more commands. We have to start again here because
    // we don't send terminal response, wich does this automatically.
    Start();

    // Inform the system that we are finished executing. If there is 
    // proactive SendSm command pending, it is notified and it continues
    // from here.
    iUtils->NotifyEvent( MSatUtils::ECallControlDone );

    // If we launched UI, close it.
    if ( !iUtils->SatUiHandler().UiLaunchedByUser() )
        {
        iUtils->NotifyEvent( MSatUtils::ESimSessionEndCallBack );
        }
    else
        {
        // Inform the session that we are done here.
        iUtils->NotifyEvent( MSatUtils::EDelaySimSessionEnd );
        }
        
    // We do not send TerminalResponse to SIM.
    LOG( SIMPLE, "CALLCONTROL: CCallControlHandler::CompleteCallControlCmd\
          exiting" )
    }
    
//  End of File
