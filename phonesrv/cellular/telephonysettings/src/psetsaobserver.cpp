/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPsetSAObserver handles all the queries to observer.
*
*
*/


// INCLUDE FILES
#include <centralrepository.h>
#include <ctsydomainpskeys.h>
#include <telservicesinternalcrkeys.h> // telephony service local variation keys. 
#include <telservicesvariant.hrh>

#include "psetsaobserver.h" 
#include "mpsetnetworkinfoobs.h" 
#include "mpsetdivertobs.h" 
#include "phonesettingslogger.h" 
#include "psetutility.h" 
#include "psetvariationproxy.h" 

//CONSTANTS
const TInt KPsetRequesterUncertain = 0;

// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// 
// 1st Phase constructor.
// 
// ---------------------------------------------------------
//
EXPORT_C CPsetSAObserver* CPsetSAObserver::NewL()
    {
    CPsetSAObserver* self = new ( ELeave ) CPsetSAObserver;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ---------------------------------------------------------------------------
// 
// C++ constructor.
// 
// ---------------------------------------------------------------------------
//
CPsetSAObserver::CPsetSAObserver() 
                : CActive( EPriorityStandard )
    {
    }

// ---------------------------------------------------------------------------
// DualActvationFeature
// ---------------------------------------------------------------------------
//
TBool CPsetSAObserver::IsDivertDualAffectFeatureEnabledL()
    {
    TBool retValue(EFalse);
    CPSetVariationProxy* variationProxy = 
        CPSetVariationProxy::NewL( KCRUidTelSrvVariation, KTelSrvVariationFlags );
    CleanupStack::PushL( variationProxy );
    retValue = variationProxy->IsFeatureEnabled( KTelephonyLVFlagDivertDualAffect );
    CleanupStack::PopAndDestroy( variationProxy );
    return retValue; 
    }

// ---------------------------------------------------------------------------
// 
// Run active object -- should only be used when trying to catch if call
// has been started. 
//
// ---------------------------------------------------------------------------
void CPsetSAObserver::RunL()
    {
    if ( iNetObserver )
        {
        if ( iStatus.Int() != KErrCancel )
            {
            TInt value = KErrGeneral;
            // This to ensure that events are not lost.
            iNotifyProperty.Subscribe( iStatus );
            iNotifyProperty.Get( value );
            SetActive();
            if ( value != EPSCTsyCallStateNone )
                {
                // This is what was wanted, so cancel subscribe and inform
                // observer.
                iNotifyProperty.Cancel();

                iNetObserver->HandleCallActivatedL();
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// 
// Returns current gprs connection state.
// NOTE that this is not actual GPRS connection, even attached is considered
// connected in this method. 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPsetSAObserver::IsGPRSConnected()
    {
    __PHSLOGSTRING("[PHS]--> CPsetSAObserver::IsGPRSConnected" );
    TInt connectionStatus = KErrGeneral;
    
    TInt error = RProperty::Get( KUidSystemCategory,
                                 KPSUidGprsStatusValue,
                                 connectionStatus );
                                 
    if ( connectionStatus == EPSGprsUnattached )
        {
        connectionStatus = CPsetSAObserver::EPSetGPRSNotConnected;
        }
    else if ( connectionStatus == EPSGprsAttach ||
              connectionStatus == EPSGprsContextActive ||
              connectionStatus == EPSGprsSuspend )
        {
        connectionStatus = CPsetSAObserver::EPSetGPRSConnectionActive;
        }
    __PHSLOGSTRING2("[PHS]<-- CPsetSAObserver::IsGPRSConnected P&S error: %d, connectionStatus: %d", error, connectionStatus );
    //Else: error situation, return error code (already in connectionStatus).
    return connectionStatus;
    }

// ---------------------------------------------------------------------------
// 
// Returns current call state
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPsetSAObserver::IsCallActive()
    {
    TInt callStatus = KErrGeneral;

    TInt error = RProperty::Get( KPSUidCtsyCallInformation,
                                 KCTsyCallState,
                                 callStatus );
    if ( error == KErrNone )
        {
        switch ( callStatus )
            {
            case EPSCTsyCallStateUninitialized:
            case EPSCTsyCallStateNone:
                callStatus = CPsetSAObserver::EPSetNoCallsActive;
                break;
            default:
                callStatus = CPsetSAObserver::EPSetCallActive;
                break;
            }
        }
    // Else - error situation, return error code (already in callStatus).
    return callStatus;
    }

// ---------------------------------------------------------------------------
// 
// Returns current Divert status
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPsetSAObserver::GetCurrentDivertStatus( 
    TUnconditionalCFStatus& aStatus )
    {
    __PHSLOGSTRING("[PHS]--> CPsetSAObserver::GetCurrentDivertStatus" );
    TInt error(KErrNone);
    TInt indicator(KCFIndicatorUnknown);
    TInt forwadingIndicator( indicator );
    TPckg<TInt> indicatorPckg( forwadingIndicator );
       
    error =  iRepository->Get( KCtsyUnconditionalCFStatus, indicatorPckg );
    if ( error == KErrNone )
        {
        aStatus = static_cast <TUnconditionalCFStatus> ( forwadingIndicator );
        }
        
    __PHSLOGSTRING2("[PHS]--> Error: %d, aStatus: %d ", error, aStatus );
    __PHSLOGSTRING("[PHS]<-- CPsetSAObserver::GetCurrentDivertStatus" );
    return error;
    }

// ---------------------------------------------------------------------------
// 
// Sets Network observer to member variable
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetSAObserver::SetNetObserver( 
    MPsetNetworkInfoObserver& aObserver )
    {
    if ( !iNetObserver )
        {
        iNetObserver = &aObserver;
        }
    TRAPD( ignore, NotifyCallStartedL() );
    // Line below fixes armv5 warning.
    ignore = ignore;
    }

// ---------------------------------------------------------------------------
// 
// Cancels active object.
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::DoCancel()
    {
    iNotifyProperty.Cancel();
    }

// ---------------------------------------------------------------------------
// 
// Destructor
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetSAObserver::~CPsetSAObserver()
    {
    Cancel();
    
    if ( iNotifyProperty.Handle() )
        {
        iNotifyProperty.Close();
        }

    iNetObserver = NULL;
    
    delete iRepository;
    iRepository = NULL;
    }

// ---------------------------------------------------------------------------
// 
// Symbian OS 2-phase constructor.
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::ConstructL()
    {
    iRepository = CRepository::NewL( KCRUidCtsyCallForwardingIndicator );
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// 
// Starts to listen to call alerting or call ringing events.
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::NotifyCallStartedL()
    {
    if ( !IsActive() )
        {
        //Ignore any errors here. It is handled in RunL
        iNotifyProperty.Attach( KPSUidCtsyCallInformation, KCTsyCallState );
        iNotifyProperty.Subscribe( iStatus );        
        SetActive();
        }
    }

// ---------------------------------------------------------------------------
// 
// Notifies cenrep that diverts have been changed => sets indicator. 
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetSAObserver::NotifyDivertChange( 
    TSelectedLine aAlsStatus,
    TCallDivertNotifySetting& aDivertNotify,
    const TInt& aActivatorUncertain )
    {
    __PHSLOGSTRING("[PHS]--> CPsetSAObserver::NotifyDivertChange" );
    // we have only one ALS line available
    if( aAlsStatus == ENotSupportedLine || 
        aAlsStatus == KErrUnknown )  
        {
        __PHSLOGSTRING("[PHS]--> we have only one ALS line available" );
        HandleNoAlsIndicators( aDivertNotify, 
                               aActivatorUncertain );
        }
    else if( aAlsStatus == EPrimaryLine )
        {
        __PHSLOGSTRING("[PHS]--> HandlePrimaryLineIndicators" );
        HandlePrimaryLineIndicators( aDivertNotify );        
        }
    else if( aAlsStatus == EAuxiliaryLine )
        {
        __PHSLOGSTRING("[PHS]--> HandleSecondaryLineIndicators" );
        HandleSecondaryLineIndicators( aDivertNotify );
        }
    
     __PHSLOGSTRING1("[PHS]    NEW DIVERT STATUS:: %d", aDivertNotify.iPreviousCfStatus );
    TInt package = aDivertNotify.iPreviousCfStatus;
    TPckg<TInt> indicatorPckg( package );
    TInt error = iRepository->Set( KCtsyUnconditionalCFStatus, indicatorPckg );
    __PHSLOGSTRING("[PHS]<-- CPsetSAObserver::NotifyDivertChange" ); 
    }
    
// ---------------------------------------------------------------------------
// 
// Handles indicators when no ALS.
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::HandleNoAlsIndicators( 
    TCallDivertNotifySetting& aDivertNotify,
    const TInt& aActivatorUncertain )
    {
    __PHSLOGSTRING("[PHS]--> CPsetSAObserver::HandleNoAlsIndicators" );
    TUnconditionalCFStatus previousStatus = aDivertNotify.iPreviousCfStatus;
    TUnconditionalCFStatus currentStatus = aDivertNotify.iPreviousCfStatus;
    TBool voiceMailBox = aDivertNotify.iVmbxDivert;
    TInt bsc = aDivertNotify.iBasicServiceCode;

    __PHSLOGSTRING1("[PHS]--> Previous CF status: %d ", previousStatus );
    __PHSLOGSTRING1("[PHS]-->      Voice MailBox: %d ", voiceMailBox );
    __PHSLOGSTRING1("[PHS]--> Basic Service Code: %d ", bsc );
    __PHSLOGSTRING1("[PHS]--> ActivatorUncertain: %d ", aActivatorUncertain );
    if ( aDivertNotify.iCfActivated )  
        {
        if( voiceMailBox )
            {
            HandleVoiceMailBoxDivertActivation( currentStatus, bsc );
            }
        else
            {
            HandleDivertActivation( currentStatus, bsc );
            }
        }
    else
        {
        __PHSLOGSTRING("[PHS]-->  Deactivition" );
        HandleDivertDeactivation( currentStatus, bsc );
        }
        
    __PHSLOGSTRING1("[PHS]--> Current CF status: %d ", currentStatus );
    // If aActivatorUncertain is zero and previously divert was acitve to 
    // Voice mailbox then status is leaved as it was before.
    if ( aActivatorUncertain == KPsetRequesterUncertain )
        {
        if ( previousStatus != currentStatus )
            {
            if ( ( previousStatus == KCFVoiceForwardedToVoiceMailbox ) || 
                 ( previousStatus == KCFVideoForwardedToVoiceMailbox ) ||
                 ( previousStatus == ( KCFVoiceForwarded | KCFVideoForwarded 
                    | KCFForwardedToVoiceMailbox ) ) )
                {
                __PHSLOGSTRING("[PHS] KCtsyUnconditionalCFStatus is NOT changed" );
                aDivertNotify.iPreviousCfStatus = previousStatus;
                }
            else
                {
                __PHSLOGSTRING("[PHS]RequesterUn-KCtsyUnconditionalCFStatus is going to be changed" );
                aDivertNotify.iPreviousCfStatus = currentStatus;
                }
            }
        }
    else
        {
        __PHSLOGSTRING("[PHS] KCtsyUnconditionalCFStatus is going to be changed" );
        aDivertNotify.iPreviousCfStatus = currentStatus; 
        }
        
    __PHSLOGSTRING("[PHS]<-- CPsetSAObserver::HandleNoAlsIndicators" );
    }

// ---------------------------------------------------------------------------
// 
// Handles line1 indicators.
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::HandlePrimaryLineIndicators( 
    TCallDivertNotifySetting& aDivertNotify )
    {
    __PHSLOGSTRING2("[PHS]--> HandlePrimaryLineIndicators - aPreviousCfStatus: %d, aCfActivated: %d", aDivertNotify.iPreviousCfStatus, aDivertNotify.iCfActivated );

    TUnconditionalCFStatus status = aDivertNotify.iPreviousCfStatus;
    TInt bsc = aDivertNotify.iBasicServiceCode;

    __PHSLOGSTRING1("[PHS]--> status: %d ", status );
    __PHSLOGSTRING1("[PHS]--> bsc:    %d ", bsc );
    
    if ( !IsDivertDualAffectFeatureEnabledL() )
        {
        if( aDivertNotify.iCfActivated )
            {
            __PHSLOGSTRING("[PHS]--> NOT DUAL Activation - Primary" );
            HandlePrimaryLineDivertActivation( status, bsc );
            }
        else
            {
            __PHSLOGSTRING("[PHS]--> NOT DUAL - Primary" );
            HandlePrimaryLineDivertDeactivation( status, bsc );
            }
        }
    else // if dualactivation is active.
        {
        if( aDivertNotify.iCfActivated )
            {
            __PHSLOGSTRING("[PHS]--> Activation - Primary" );
            HandlePrimaryLineDualDivertActivation( status, bsc );
            }
        else
            {
            __PHSLOGSTRING("[PHS]--> Deactivation - Primary" );
            HandlePrimaryLineDualDivertDeactivation( status, bsc );
            }
        }
    
    aDivertNotify.iPreviousCfStatus = status;
    }

// ---------------------------------------------------------------------------
// 
// Handles line2 indicators.
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::HandleSecondaryLineIndicators( 
    TCallDivertNotifySetting& aDivertNotify )
    {
    __PHSLOGSTRING2("[PHS]--> HandleSecondaryLineIndicators - HandleSecondaryLineIndicators: %d, aCfActivated: %d", aDivertNotify.iPreviousCfStatus, aDivertNotify.iCfActivated );

    TUnconditionalCFStatus status = aDivertNotify.iPreviousCfStatus;

    TInt bsc = aDivertNotify.iBasicServiceCode;
    
    __PHSLOGSTRING1("[PHS]--> status: %d ", status );
    __PHSLOGSTRING1("[PHS]--> bsc:    %d ", bsc );
    
    if ( !IsDivertDualAffectFeatureEnabledL() )
        {
        if( aDivertNotify.iCfActivated )
            {
            __PHSLOGSTRING("[PHS]--> NOT DUAL Activation - Secondary" );
            HandleSecondaryLineDivertActivation( status, bsc );
            }
        else
            {
            __PHSLOGSTRING("[PHS]--> NOT DUAL  Deactivation - Secondary" );
            HandleSecondaryLineDivertDeactivation( status, bsc );
            }
        }
    else // if dualactivation is active.
        {
        if( aDivertNotify.iCfActivated )
            {
            __PHSLOGSTRING("[PHS]--> Activation - Secondary" );
            HandleSecondaryLineDualDivertActivation( status, bsc );
            }
        else
            {
            __PHSLOGSTRING("[PHS]--> Deactivation - Secondary" );
            HandleSecondaryLineDualDivertDeactivation( status, bsc );
            }
        }
    
    aDivertNotify.iPreviousCfStatus = status;
    }

// ---------------------------------------------------------------------------
// 
// Set correct indicator value when divert done to voicemail (no ALS).
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::HandleVoiceMailBoxDivertActivation( 
    TUnconditionalCFStatus& aStatus,
    const TInt aBsc  )
    {
    switch ( aStatus )
        {
        case KCFIndicatorUnknown:
        case KCFNoCallsForwarded:
            {
            if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Mailbox - 1" );
                aStatus =  KCFVoiceForwardedToVoiceMailbox;
                }
            else if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Mailbox - 2" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedToVoiceMailbox );
                }
            else if (  aBsc == EAllBearer || aBsc == EAllSync ||
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Mailbox - 3" );
                aStatus =  KCFVideoForwardedToVoiceMailbox;
                }
            break;  
            }
        case KCFVoiceForwarded:
            {       
            if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Mailbox - 4" );
                aStatus =  KCFVoiceForwardedToVoiceMailbox;
                }
            else if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Mailbox - 5" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedToVoiceMailbox );
                }
            else if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Mailbox - 6" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwardedToVoiceMailbox );
                }
            break;  
            }
        case KCFVideoForwarded:
            {
            if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Mailbox - 7" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVideoForwarded | KCFVoiceForwardedToVoiceMailbox );
                }
            else if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Mailbox - 8" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedToVoiceMailbox );
                }
            else if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Mailbox - 9" );
                aStatus = KCFVideoForwardedToVoiceMailbox;
                }
            break;  
            }
        case KCFVoiceForwarded | KCFVideoForwarded:
            {       
            if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Mailbox - 10" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVideoForwarded | KCFVoiceForwardedToVoiceMailbox );
                }
            else if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Mailbox - 11" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedToVoiceMailbox );
                }
            else if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Mailbox - 12" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwardedToVoiceMailbox );
                }
            break;  
            }
        case KCFVoiceForwardedToVoiceMailbox:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllBearer ||
                aBsc == EAllSync || aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Mailbox - 13" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedToVoiceMailbox );
                }
            break;  
            }
        case KCFVideoForwarded | KCFVoiceForwardedToVoiceMailbox:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllBearer ||
                aBsc == EAllSync || aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Mailbox - 14" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedToVoiceMailbox );
                }  
            break;  
            }
        case KCFVideoForwardedToVoiceMailbox:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllTele ||
                aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Mailbox - 15" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedToVoiceMailbox );
                }
            break;  
            }
        case KCFVoiceForwarded | KCFVideoForwardedToVoiceMailbox:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllTele ||
                aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Mailbox - 16" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedToVoiceMailbox );
                }
            break;  
            }
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// 
// Set correct indicator value when divert done to some other number than 
// voicemail (no ALS).
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::HandleDivertActivation( 
    TUnconditionalCFStatus& aStatus,
    const TInt aBsc  )
    {
    switch ( aStatus )
        {
        case KCFIndicatorUnknown:
        case KCFNoCallsForwarded:
            {
            if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Activation - 1" );
                aStatus = KCFVoiceForwarded;
                }
            else if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Activation - 2" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded );
                }
            else if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - 3" );
                aStatus = KCFVideoForwarded;
                }
            break;
            }
        case KCFVoiceForwarded:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllBearer ||
                aBsc == EAllSync || aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - 4" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded );
                }
            break;
            }
        case KCFVideoForwarded:
            {
            if ( aBsc == EAllTele || aBsc == ETelephony ||
                aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Activation - 5" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded );
                }
            break;
            }
        case KCFVoiceForwardedToVoiceMailbox:
            {
            if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Activation - 6" );
                aStatus = KCFVoiceForwarded;
                }
            else if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Activation - 7" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded );
                }
            else if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - 8" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVideoForwarded | KCFVoiceForwardedToVoiceMailbox );
                }
            break;
            }
        case KCFVideoForwardedToVoiceMailbox:
            {
            if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Activation - 9" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwardedToVoiceMailbox );
                }
            else if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Activation - 10" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded );
                }
            else if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - 11" );
                aStatus = KCFVideoForwarded;
                }
            break;
            }
        case KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedToVoiceMailbox:
            {
            if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Activation - 12" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwardedToVoiceMailbox );
                }
            else if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Activation - 13" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded );
                }
            else if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - 14" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVideoForwarded | KCFVoiceForwardedToVoiceMailbox );
                }
            break;
            }
        case KCFVoiceForwarded | KCFVideoForwardedToVoiceMailbox:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllBearer ||
                aBsc == EAllSync || aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - 15" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded );
                //aStatus = EPSAllForwarded;  
                }
            break;
            }
        case KCFVideoForwarded | KCFVoiceForwardedToVoiceMailbox:
            {
            if ( aBsc == EAllTele || aBsc == ETelephony ||
                aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Activation - 16" );
                aStatus =  static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded );
                }
            break;
            }
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// 
// Set correct indicator value when deactivation done(no ALS).
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::HandleDivertDeactivation( 
    TUnconditionalCFStatus& aStatus,
    const TInt aBsc  )
    {
    switch ( aStatus )
        {
        case KCFVoiceForwarded | KCFVideoForwarded:
            {
            if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 1" );
                aStatus = KCFVideoForwarded;
                }
            else if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 2" );
                aStatus = KCFNoCallsForwarded;
                }
            else if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 3" );
                aStatus = KCFVoiceForwarded;
                }
            break;  
            }
        case KCFVoiceForwarded:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllTele || 
                aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 4" );
                aStatus = KCFNoCallsForwarded;
                }
            break;
            }
        case KCFVideoForwarded:
            {
            if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 5" );
                aStatus = KCFVideoForwarded;
                }
            else if ( aBsc == EAllTeleAndBearer || aBsc == EAllBearer || 
                aBsc == EAllSync || aBsc == ESyncData   )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 6" );
                aStatus = KCFNoCallsForwarded;
                }
            break;
            }
        case KCFVoiceForwardedToVoiceMailbox:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllTele || 
                aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 7" );
                aStatus = KCFNoCallsForwarded;
                }
            break;
            }
        case KCFVideoForwarded | KCFVoiceForwardedToVoiceMailbox:
            {
            if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 9" );
                aStatus = KCFNoCallsForwarded;
                }
            else if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 10" );
                aStatus = KCFVideoForwarded;
                }
            else if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 11" );
                aStatus = KCFVoiceForwardedToVoiceMailbox;
                }    
            break;
            }
        case KCFVideoForwardedToVoiceMailbox:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllBearer || 
                aBsc == EAllSync || aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 12" );
                aStatus = KCFNoCallsForwarded;
                }
            break;
            }
        case KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedToVoiceMailbox:
            {
            if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 13" );
                aStatus = KCFVoiceForwardedToVoiceMailbox;
                }
            else if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 14" );
                aStatus = KCFVideoForwardedToVoiceMailbox;
                }
            else if ( aBsc == EAllTeleAndBearer  )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 15" );
                aStatus = KCFNoCallsForwarded;
                }
            break;
            }
        case KCFVoiceForwarded | KCFVideoForwardedToVoiceMailbox:
            {
            if ( aBsc == EAllTeleAndBearer  )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 16" );
                aStatus = KCFNoCallsForwarded;
                }
            else if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 17" );
                aStatus = KCFVideoForwardedToVoiceMailbox;
                }
            else if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc== ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Deactivition - 18" );
                aStatus = KCFVoiceForwarded;
                }
            break;
            }
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// 
// Set correct indicator value when Primary line used.
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::HandlePrimaryLineDivertActivation( 
    TUnconditionalCFStatus& aStatus,
    const TInt aBsc  )
    {
    switch ( aStatus )
        {
        case KCFIndicatorUnknown:
        case KCFNoCallsForwarded:
        case KErrUnknown:
            {
            if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Primary - 1" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedOnLine1 );
                }
            else if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Primary - 2" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFForwardedOnLine1 );
                }
            else if ( aBsc == EAllBearer || aBsc == EAllSync ||
                aBsc == ESyncData  )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Primary - 3" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVideoForwarded | KCFForwardedOnLine1 );
                }
            break;
            }
        case KCFVoiceForwarded | KCFForwardedOnLine1:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllBearer ||
                aBsc == EAllSync || aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Primary - 4" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedOnLine1 );
                }
            break;
            }
        case KCFVideoForwarded | KCFForwardedOnLine1:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllTele || 
                aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Primary - 5" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedOnLine1 );
                }
            break;
            }
        case KCFVideoForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllTele || 
                aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Primary - 6" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedOnLine1 | 
                KCFForwardedOnLine2 );
                }
            break;
            }
        case KCFForwardedOnLine2:
            {
            if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Primary - 7" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVideoForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                }
            else if( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Primary - 8" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedOnLine1 | 
                KCFForwardedOnLine2 );
                }
            else if ( aBsc == EAllTele || 
                aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Primary - 9" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                }
            break;
            }
          
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// 
// Set correct indicator value when Primary line used.
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::HandlePrimaryLineDualDivertActivation(
        TUnconditionalCFStatus& aStatus,
        const TInt aBsc  )
    {
    switch ( aStatus )
        {
        case KCFIndicatorUnknown:
        case KCFNoCallsForwarded:
        case KErrUnknown:
            {
            if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Primary - 1" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFForwardedOnLine1 );
                }
            else if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Primary - 2" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFForwardedOnLine1 );
                }
            break;
            }
        case KCFVoiceForwarded | KCFForwardedOnLine1:
            {
            if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Primary - 3" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                }
            break;
            }
        case KCFForwardedOnLine2:
            {
            if( aBsc == EAllTeleAndBearer || aBsc == EAllTele || 
                aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Primary - 4" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                }
            break;
            }
            
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// 
// Set correct indicator value when Primary line used.
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::HandlePrimaryLineDualDivertDeactivation(
        TUnconditionalCFStatus& aStatus,
        const TInt aBsc  )
    {
    switch ( aStatus )
        {
        case KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllTele 
                || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Primary - 1" );
                aStatus = KCFForwardedOnLine2;
                }
            break;
            }
        case KCFVoiceForwarded | KCFForwardedOnLine1:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllTele 
                || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Primary - 2" );
                aStatus = KCFNoCallsForwarded;
                }
            break;
            }
        case KCFForwardedOnLine2:
            { 
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllTele 
                || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Primary - 3" );
                aStatus = KCFForwardedOnLine2;
                }
            break;
            }
            
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// 
// Set correct indicator value when Primary line used.
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::HandlePrimaryLineDivertDeactivation( 
    TUnconditionalCFStatus& aStatus,
    const TInt aBsc )
    {
    switch ( aStatus )
        {
        case KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2:
            {
            __PHSLOGSTRING("[PHS]--> Deactivation - Primary - 1" );
            aStatus = KCFForwardedOnLine2;
            break;
            }
        case KCFVoiceForwarded | KCFForwardedOnLine1:
            {
            if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Primary - 2" );
                aStatus = KCFNoCallsForwarded;
                }
            else if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Primary - 3");
                aStatus = KCFNoCallsForwarded;
                }
            break;
            }
        case KCFVideoForwarded | KCFForwardedOnLine1:
            {
            if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Primary - 4" );
                aStatus = KCFNoCallsForwarded;
                } 
            else if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Primary - 5" );
                aStatus = KCFNoCallsForwarded;
                }
            break;
            }
        case KCFVideoForwarded | KCFVoiceForwarded | KCFForwardedOnLine1:
            {
            if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Primary - 6" );
                aStatus = KCFNoCallsForwarded;
                }
            else if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Primary - 7" );
                aStatus = aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVideoForwarded | KCFForwardedOnLine1 );
                }
            else if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Primary - 8" );
                aStatus = aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFForwardedOnLine1 );
                }
            break;
            }
        case KCFVideoForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2:
            { 
            if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData || aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Primary - 9" );
                aStatus = KCFForwardedOnLine2;
                }
            else if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Primary - 10" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVideoForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                }
            break;
            }
        case KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2:
            {
            if ( aBsc == EAllTeleAndBearer )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Primary - 11" );
                aStatus =  KCFForwardedOnLine2;
                }
            else if ( aBsc == EAllTele || aBsc == ETelephony )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Primary - 12" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVideoForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                } 
            else if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData  )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Primary - 13" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                }
            break;
            }
        
        default:
            break;
        }
    }
    
// ---------------------------------------------------------------------------
// 
// Set correct indicator value when secondary line used.
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::HandleSecondaryLineDivertActivation( 
    TUnconditionalCFStatus& aStatus,
    const TInt aBsc  )
    {
    switch ( aStatus )
        {
        case KCFIndicatorUnknown:
        case KCFNoCallsForwarded:
        case KErrUnknown:
            {
            if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Secondary - 1" );   
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVideoForwarded | KCFForwardedOnLine1 );
                }
            else if ( aBsc == EAllTeleAndBearer || aBsc == EAltTele ||
                aBsc == EAllTele )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Secondary - 2" );   
                aStatus = KCFForwardedOnLine2;
                }    
            break;
            }
        case KCFVoiceForwarded | KCFForwardedOnLine1:
            {
            if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Secondary - 3" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedOnLine1 );
                }
            else if ( aBsc == EAllTeleAndBearer || aBsc == EAltTele ||
                aBsc == EAllTele )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Secondary - 4");
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                }
            break;
            }
        case KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedOnLine1:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAltTele ||
                aBsc == EAllTele )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Secondary - 5");
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedOnLine1 |
                KCFForwardedOnLine2 );
                }
            break;
            }
        case KCFForwardedOnLine2:
            {
            if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Secondary - 6");
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVideoForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                }
            break;
            }
        case KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2:
            {
            if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Secondary - 7");
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                }
            break;
            }
        case KCFVideoForwarded | KCFForwardedOnLine1:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAltTele ||
                aBsc == EAllTele )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Secondary - 8");
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVideoForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                }
            break;
            }
        
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// 
// Set correct indicator value when secondary line used.
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::HandleSecondaryLineDualDivertActivation( 
    TUnconditionalCFStatus& aStatus,
    const TInt aBsc  )
    {
    switch ( aStatus )
        {
        case KCFIndicatorUnknown:
        case KCFNoCallsForwarded:
        case KErrUnknown:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAltTele ||
                aBsc == EAllTele )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Secondary - 1" );   
                aStatus = KCFForwardedOnLine2;
                }    
            break;
            }
        case KCFVoiceForwarded | KCFForwardedOnLine1:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAltTele ||
                aBsc == EAllTele )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Secondary - 2");
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                }
            break;
            }
        case KCFForwardedOnLine2:
            {
            if ( aBsc == EAllTeleAndBearer ||  aBsc == EAllTele )
                {
                __PHSLOGSTRING("[PHS]--> Activation - Secondary - 3");
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                }
            break;
            }
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// 
// Set correct indicator value when secondary line used.
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::HandleSecondaryLineDivertDeactivation( 
    TUnconditionalCFStatus& aStatus,
    const TInt aBsc  )
    {
    switch ( aStatus )
        {
        case KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllTele ||
                aBsc == EAltTele )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Secondary - 1" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFForwardedOnLine1 );
                }
            break;
            }
        case KCFVideoForwarded |KCFVoiceForwarded | KCFForwardedOnLine1:
            {
            if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Secondary - 2" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFForwardedOnLine1 );
                }
            break;
            }
        case KCFVoiceForwarded | KCFForwardedOnLine1:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllTele )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Secondary - 3" );
                aStatus = KCFNoCallsForwarded;
                }
            break;
            }
        case KCFVideoForwarded | KCFForwardedOnLine1:
            {
            if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Secondary - 4" );
                aStatus = KCFNoCallsForwarded;
                }
            break;  
            } 
        case KCFForwardedOnLine2:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAltTele ||
                aBsc == EAllTele )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Secondary - 5" );
                aStatus = KCFNoCallsForwarded;
                }
            break;
            }
        case KCFVideoForwarded | KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2:
            {
            if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Secondary - 6" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                }
            else if ( aBsc == EAllTeleAndBearer || aBsc == EAltTele ||
                aBsc == EAllTele )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Secondary - 7" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVideoForwarded | KCFVoiceForwarded | KCFForwardedOnLine1 );
                }
            break;
            }
        case KCFVideoForwarded |KCFVoiceForwarded:
            {
            if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Secondary - 8" );
                aStatus = KCFNoCallsForwarded;
                }
            break;
            }
        case KCFVideoForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2:
            {
            if ( aBsc == EAllBearer || aBsc == EAllSync || 
                aBsc == ESyncData )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Secondary - 9" );
                aStatus = KCFForwardedOnLine2;
                }
            else if ( aBsc == EAllTeleAndBearer || aBsc == EAltTele ||
                aBsc == EAllTele )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Secondary - 10" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVideoForwarded | KCFForwardedOnLine1 );
                }
            break;
            }
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// 
// Set correct indicator value when secondary line used.
// 
// ---------------------------------------------------------------------------
//
void CPsetSAObserver::HandleSecondaryLineDualDivertDeactivation( 
    TUnconditionalCFStatus& aStatus,
    const TInt aBsc  )
    {
    switch ( aStatus )
        {
        case KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllTele ||
                aBsc == EAltTele )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Secondary - 1" );
                aStatus = static_cast<TUnconditionalCFStatus>( 
                KCFVoiceForwarded | KCFForwardedOnLine1 );
                }
            break;
            }
        case KCFVoiceForwarded | KCFForwardedOnLine1:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAllTele )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Secondary - 2" );
                aStatus = KCFNoCallsForwarded;
                }
            break;
            }
        case KCFForwardedOnLine2:
            {
            if ( aBsc == EAllTeleAndBearer || aBsc == EAltTele ||
                aBsc == EAllTele )
                {
                __PHSLOGSTRING("[PHS]--> Deactivation - Secondary - 3" );
                aStatus = KCFNoCallsForwarded;
                }
            break;
            }
        default:
            break;
        }
    }
    
//  End of File
