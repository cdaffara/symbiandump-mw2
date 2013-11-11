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
* Description:  Process forwarding procedures.
*
*/

#include "csatsendsshandler.h"
#include <psetcallwaiting.h>
#include <psetcontainer.h>
#include <psetcallbarring.h>
#include <psetcalldiverting.h>
#include <psetcli.h>

#include "csatsendssbarringnouiobs.h"
#include "csatsendssdivertnouiobs.h"
#include "csatsendsscallwaitingnouiobs.h"
#include "csatsendssclinouiobs.h"

#include    "SatLog.h"

// ================= MEMBER FUNCTIONS ==========================================

// Destructor
CSatSendSsHandler::~CSatSendSsHandler()
    {
    delete iDivert;
    delete iBarring;
    delete iWaiting;
    delete iCli;
    delete iSettings;
    delete iSatCwObserver;
    delete iSatCbObserver;
    delete iSatCliObserver;
    delete iSatCfObserver;
    }

// -----------------------------------------------------------------------------
// CSatSendSsHandler::ProcessForwardingL
// -----------------------------------------------------------------------------
void CSatSendSsHandler::ProcessForwardingL(
        TAction aAction,
        TForwardingProcedure aForwarding,
        const TDesC& aNumber,
        const TDesC& aBasicCode,
        const TDesC& aTimer )
    {
    LOG( SIMPLE, "CSatSendSsHandler::ProcessForwardingL calling" )

    TBasicServiceGroups bsc = ChangeToBsc( aBasicCode );
    ConnectToSsEngineL();

    TCallDivertSetting setDivert;
    setDivert.iNumber.Copy( aNumber );
    setDivert.iServiceGroup = EServiceGroupVoice;
    setDivert.iStatus = EDivertingStatusUnknown;
    setDivert.iNoReplyTimer = 0;// all unconditional

    LOG2( NORMAL, 
    "CSatSendSsHandler::ProcessForwardingL aForwarding: %d", aForwarding )
    switch ( aForwarding )
        {
        case EForwardingNotReachable:
            {
            LOG( NORMAL, 
            "CSatSendSsHandler::ProcessForwardingL EForwardingNotReachable" )
            setDivert.iCondition = EDivertConditionNotReachable;
            break;
            }

        case EForwardingNoReply:
            {
            LOG( NORMAL, 
            "CSatSendSsHandler::ProcessForwardingL EForwardingNoReply" )
            if ( aTimer.Length() != 0 )
                {
                TLex input( aTimer );
                User::LeaveIfError( input.Val( setDivert.iNoReplyTimer ) );
                }
            else
                {
                LOG( NORMAL, 
                "CSatSendSsHandler::ProcessForwardingL iNoReplyTimer" )
                setDivert.iNoReplyTimer = iDivert->GetTimerValueL();
                }
            setDivert.iCondition = EDivertConditionNoReply;
            break;
            }

        case EForwardingBusy:
            {
            LOG( NORMAL, 
            "CSatSendSsHandler::ProcessForwardingL EForwardingBusy" )
            setDivert.iCondition = EDivertConditionBusy;
            break;
            }

        case EForwardingUnconditional:
            {
            LOG( NORMAL, 
            "CSatSendSsHandler::ProcessForwardingL EForwardingUnconditional" )
            setDivert.iCondition = EDivertConditionUnconditional;
            break;
            }

        case EForwardingAll:
            {
            LOG( NORMAL, 
            "CSatSendSsHandler::ProcessForwardingL EForwardingAll" )
            setDivert.iCondition = EDivertConditionAllCalls;
            break;
            }

        case EForwardingAllConditional:
            {
            LOG( NORMAL, 
            "CSatSendSsHandler::ProcessForwardingL \
            EForwardingAllConditional" )
            setDivert.iCondition = EDivertConditionAllConditionalCases;
            break;
            }

        default:
            {
            LOG( NORMAL, 
            "CSatSendSsHandler::ProcessForwardingL Unknown forwarding" )
            break;
            }
        }

    switch ( aAction )
        {
        case EActivate:
            {
            LOG( NORMAL, "CSatSendSsHandler::EActivate" )
            setDivert.iSetting = EActivateDiverting;
            iDivert->SetDivertingL( setDivert, bsc );
            break;
            }

        case EDeactivate:
            {
            LOG( NORMAL, "CSatSendSsHandler::EDeactivate" )
            setDivert.iSetting = ECancelDiverting;
            iDivert->SetDivertingL( setDivert, bsc );
            break;
            }

        case EInterrogate:
            {
            LOG( NORMAL, "CSatSendSsHandler::EInterrogate" )
            iDivert->GetDivertingStatusL( EServiceGroupVoice,
                setDivert.iCondition, bsc );
            break;
            }

        case ERegister:
            {
            LOG( NORMAL, "CSatSendSsHandler::ERegister" )
            setDivert.iSetting = ERegisterDiverting;
            iDivert->SetDivertingL( setDivert, bsc );
            break;
            }

        case EErase:
            {
            LOG( NORMAL, "CSatSendSsHandler::EErase" )
            setDivert.iSetting = EEraseDiverting;
            iDivert->SetDivertingL( setDivert, bsc );
            break;
            }

        default:
            {
            LOG( NORMAL, "  Unknown action" )
            break;
            }
        }

    LOG( SIMPLE, "CSatSendSsHandler::ProcessForwardingL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsHandler::ProcessBarringL
// -----------------------------------------------------------------------------
void CSatSendSsHandler::ProcessBarringL(
        TAction aAction,
        TBarringProcedure aBarring,
        const TDesC& aPassword,
        const TDesC& aBasicCode )
    {
    LOG2( SIMPLE, "CSatSendSsHandler::ProcessBarringL calling, aBarring: %d",
          aBarring )

    TBasicServiceGroups bsc = ChangeToBsc( aBasicCode );
    ConnectToSsEngineL();
    TCallBarringSetting setBarring;
    setBarring.iServiceGroup = EServiceGroupVoice;

    setBarring.iPassword = aPassword;
    switch ( aBarring )
        {
        case EBarringAllOutgoingCalls:
            {
            setBarring.iType = EBarringTypeAllOutgoing;
            break;
            }

        case EBarringAllOutgoingInternationalCalls:
            {
            setBarring.iType = EBarringTypeOutgoingInternational;
            break;
            }

        case EBarringAllOutgoingInternationalCallsExceptHome:
            {
            setBarring.iType = EBarringTypeOutgoingInternationalExceptToHomeCountry;
            break;
            }

        case EBarringAllIncomingCalls:
            {
            setBarring.iType = EBarringTypeAllIncoming;
            break;
            }

        case EBarringAllIncomingCallsWhenRoaming:
            {
            setBarring.iType = EBarringTypeIncomingWhenRoaming;
            break;
            }

        case EBarringAllServices:
            {
            setBarring.iType = EBarringTypeAllServices;
            break;
            }

        case EBarringAllOutgoingServices:
            {
            setBarring.iType = EBarringTypeAllOutgoingServices;
            break;
            }

        case EBarringAllIncomingServices:
            {
            setBarring.iType = EBarringTypeAllIncomingServices;
            break;
            }

        default:
            {
            break;
            }
        }
    LOG2( SIMPLE, "CSatSendSsHandler::ProcessBarringL aAction: %d", aAction )
    switch ( aAction )
        {
        case EActivate:
            {
            setBarring.iSetting = EActivateBarring;
            iBarring->SetBarringL( setBarring, bsc );
            break;
            }

        case EDeactivate:
            {
            setBarring.iSetting = ECancelBarring;
            iBarring->SetBarringL( setBarring, bsc );
            break;
            }

        case EInterrogate:
            {
            iBarring->GetBarringStatusL( EServiceGroupVoice, setBarring.iType );
            break;
            }

        case ERegister:
        case EErase:
        default:
            {
            break;
            }
        }

    LOG( SIMPLE, "CSatSendSsHandler::ProcessBarringL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsHandler::ProcessPasswordChangeL
// -----------------------------------------------------------------------------
void CSatSendSsHandler::ProcessPasswordChangeL(
        TPasswordProcedure aPassword,
        const TDesC& aOldPassword,
        const TDesC& aNewPassword,
        const TDesC& aVerifyNewPassword )
    {
    LOG( SIMPLE, "CSatSendSsHandler::ProcessPasswordChangeL calling" )

    ConnectToSsEngineL();

    //check the validity of aNewPassword and aVerifyNewPassword
    if ( aNewPassword.Compare( aVerifyNewPassword ) != 0 )
        {
        LOG( SIMPLE, 
        "CSatSendSsHandler::ProcessPasswordChangeL password error" )
        // if not equal, show "PASSWORD ERROR"
        iSatCbObserver->HandleBarringErrorL( 1 );
        }
    else
        {
        RMobilePhone::TMobilePhonePasswordChangeV2 passWords;
        passWords.iOldPassword = aOldPassword;
        passWords.iNewPassword = aNewPassword;
        passWords.iVerifiedPassword = aVerifyNewPassword;
        // if equal send to cpsetcallbarring
        if ( aPassword == EPasswordBarring )
            {
            LOG( SIMPLE, 
            "CSatSendSsHandler::ProcessPasswordChangeL EPasswordBarring" )
            iBarring->ChangePasswordL( passWords, ETrue );
            }
        else
            {
            LOG( SIMPLE, 
            "CSatSendSsHandler::ProcessPasswordChangeL others" )
            iBarring->ChangePasswordL( passWords, EFalse );
            }
        }

    LOG( SIMPLE, "CSatSendSsHandler::ProcessPasswordChangeL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsHandler::ChangeToBsc
// -----------------------------------------------------------------------------
TBasicServiceGroups CSatSendSsHandler::ChangeToBsc(
    const TDesC& aBasicCode ) const
    {
    LOG( SIMPLE, "CSatSendSsHandler::ChangeToBsc calling" )

    // Null values must be handled as well, length == 0
    if ( aBasicCode.Length() == 0 )
        {
        LOG( SIMPLE, "CSatSendSsHandler::ChangeToBsc length is zero" )
        return EAllTeleAndBearer;
        }

    TInt value = 0;
    TLex input( aBasicCode );
    input.Val( value );

    // Presumed that the values given in aBasicCode are not acting violently
    TBasicServiceGroups returnValue =
        static_cast<TBasicServiceGroups>( value );

    LOG( SIMPLE, "CSatSendSsHandler::ChangeToBsc exiting" )
    return returnValue;
    }

// -----------------------------------------------------------------------------
// CSatSendSsHandler::ConnectToSsEngine
// -----------------------------------------------------------------------------
void CSatSendSsHandler::ConnectToSsEngineL()
    {
    LOG( SIMPLE, "CSatSendSsHandler::ConnectToSsEngineL calling" )

    if ( !iSettings ) //create connection
        {
        LOG( SIMPLE, 
        "CSatSendSsHandler::ConnectToSsEngineL iSettings false" )
        //creates the containers
        iSettings = CPsetContainer::NewL();

        //call waiting engine
        iSatCwObserver = new ( ELeave ) CSatSendSsCallWaitingNoUiObs();

        iWaiting = iSettings->CreateCWObjectL( *iSatCwObserver );

        //call barring engine
        iSatCbObserver = new ( ELeave ) CSatSendSsBarringNoUiObs();

        iBarring = iSettings->CreateCBObjectL( *iSatCbObserver );

        iSatCliObserver = new ( ELeave ) CSatSendSsCliNoUiObs();

        iCli = iSettings->CreateCliObjectL( *iSatCliObserver );

        //call divert observer and engine
        iSatCfObserver = new ( ELeave ) CSatSendSsDivertNoUiObs();

        iDivert = iSettings->CreateCFObjectL( *iSatCfObserver );
        }

    LOG( SIMPLE, "CSatSendSsHandler::ConnectToSsEngineL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsHandler::Cancel
// -----------------------------------------------------------------------------
//
void CSatSendSsHandler::Cancel()
    {
    LOG( SIMPLE, "CSatSendSsHandler::Cancel calling" )

    if ( iWaiting )
        {
        iWaiting->CancelProcess();
        }

    if ( iBarring )
        {
        iBarring->CancelCurrentRequest();
        }

    if ( iCli )
        {
        iCli->CancelAll();
        }

    if ( iDivert )
        {
        iDivert->CancelCurrentRequest();
        }

    LOG( SIMPLE, "CSatSendSsHandler::Cancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsHandler::ProcessWaitingL
// -----------------------------------------------------------------------------
void CSatSendSsHandler::ProcessWaitingL(
        TAction aAction,
        const TDesC& aBasicCode )
    {
    LOG2( SIMPLE, "CSatSendSsHandler::ProcessWaitingL calling,aAction: %d",
          aAction )

    TBasicServiceGroups bsc = ChangeToBsc( aBasicCode );
    ConnectToSsEngineL();

    switch ( aAction )
        {
        case EActivate:
            {
            iWaiting->SetCallWaitingL( MPsetCallWaiting::EActivateCallWaiting,
                bsc );
            break;
            }

        case EDeactivate:
            {
            iWaiting->SetCallWaitingL( MPsetCallWaiting::EDeactivateCallWaiting,
                bsc );
            break;
            }

        case EInterrogate:
            {
            iWaiting->GetCallWaitingStatusL();
            break;
            }

        case ERegister:
        case EErase:
        default:
            {
            break;
            }
        }

    LOG( SIMPLE, "CSatSendSsHandler::ProcessWaitingL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsHandler::ProcessClipStatusL
// -----------------------------------------------------------------------------
void CSatSendSsHandler::ProcessClipStatusL()
    {
    LOG( SIMPLE, "CSatSendSsHandler::ProcessClipStatusL calling" )

    ConnectToSsEngineL();
    iCli->GetClipModeL();

    LOG( SIMPLE, "CSatSendSsHandler::ProcessClipStatusL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsHandler::ProcessClirStatusL
// -----------------------------------------------------------------------------
void CSatSendSsHandler::ProcessClirStatusL()
    {
    LOG( SIMPLE, "CSatSendSsHandler::ProcessClirStatusL calling" )

    ConnectToSsEngineL();
    iCli->GetClirModeL();

    LOG( SIMPLE, "CSatSendSsHandler::ProcessClirStatusL calling" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsHandler::ProcessColpStatusL
// -----------------------------------------------------------------------------
void CSatSendSsHandler::ProcessColpStatusL()
    {
    LOG( SIMPLE, "CSatSendSsHandler::ProcessColpStatusL calling" )

    ConnectToSsEngineL();
    iCli->GetColpModeL();

    LOG( SIMPLE, "CSatSendSsHandler::ProcessColpStatusL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsHandler::ProcessColrStatusL
// -----------------------------------------------------------------------------
void CSatSendSsHandler::ProcessColrStatusL()
    {
    LOG( SIMPLE, "CSatSendSsHandler::ProcessColrStatusL calling" )

    ConnectToSsEngineL();
    iCli->GetColrModeL();

    LOG( SIMPLE, "CSatSendSsHandler::ProcessColrStatusL exiting " )
    }

// -----------------------------------------------------------------------------
// CSatSendSsHandler::ProcessCnapStatusL
// -----------------------------------------------------------------------------
void CSatSendSsHandler::ProcessCnapStatusL()
    {
    LOG( SIMPLE, "CSatSendSsHandler::ProcessCnapStatusL calling" )

    ConnectToSsEngineL();
    iCli->GetCnapL();

    LOG( SIMPLE, "CSatSendSsHandler::ProcessCnapStatusL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsHandler::RequestComplete
// -----------------------------------------------------------------------------
void CSatSendSsHandler::RequestComplete()
    {
    LOG( SIMPLE, "CSatSendSsHandler::RequestComplete calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsHandler::RequestStatusChanged
// -----------------------------------------------------------------------------
void CSatSendSsHandler::RequestStatusChanged( TInt aNewStatus )
    {
    LOG( SIMPLE, "CSatSendSsHandler::RequestStatusChanged calling" )

    TInt status = aNewStatus;
    status = status;
    LOG2( SIMPLE, "   RequestStatusChanged (%i)", status )

    LOG( SIMPLE, "CSatSendSsHandler::RequestStatusChanged exiting" )
    }

//  End of File
