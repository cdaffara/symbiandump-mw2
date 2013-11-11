/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Utility class to switch between ETEL internal enumerations and
*                codes defined in the GSM standard.
*
*/


// INCLUDE FILES
#include "psetutility.h" 
#include <rsssettings.h> 
#include "phonesettingslogger.h" 

// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// 
// Translates given basic service code from code used in GSM standards to 
// ETEL internal code.
// ---------------------------------------------------------------------------
//
RMobilePhone::TMobileService PSetUtility::ChangeToEtelInternal( const TBasicServiceGroups& aBsc )
    {
    switch ( aBsc )
        {        
        case EAllTeleAndBearer:
            return RMobilePhone::EAllServices;
        case EAllTele:
            return RMobilePhone::EAllTele;
        case ETelephony:
            return RMobilePhone::ETelephony;
        case EAllDataTele:
            return RMobilePhone::EAllDataTele;
        case EFax:
            return RMobilePhone::EFaxService;
        case ESms:
            return RMobilePhone::EShortMessageService;
        case EAllDataExSms:
            return RMobilePhone::EAllDataExSms;
        case EAllTeleExcSms:
            return RMobilePhone::EAllTeleExcSms;
        case EAllPlmnTele:
            return RMobilePhone::EAllPlmnTele;
        case EPlmnTele1:
            return RMobilePhone::EPlmnTele1;
        case EPlmnTele2:
            return RMobilePhone::EPlmnTele2;
        case EPlmnTele3:
            return RMobilePhone::EPlmnTele3;
        case EPlmnTele4:
            return RMobilePhone::EPlmnTele4;
        case EPlmnTele5:
            return RMobilePhone::EPlmnTele5;
        case EPlmnTele6:
            return RMobilePhone::EPlmnTele6;
        case EPlmnTele7:
            return RMobilePhone::EPlmnTele7;
        case EPlmnTele8:
            return RMobilePhone::EPlmnTele8;
        case EPlmnTele9:
            return RMobilePhone::EPlmnTele9;
        case EPlmnTeleA:
            return RMobilePhone::EPlmnTeleA;
        case EPlmnTeleB:
            return RMobilePhone::EPlmnTeleB;
        case EPlmnTeleC:
            return RMobilePhone::EPlmnTeleC;
        case EPlmnTeleD:
            return RMobilePhone::EPlmnTeleD;
        case EPlmnTeleE:
            return RMobilePhone::EPlmnTeleE;
        case EPlmnTeleF:
            return RMobilePhone::EPlmnTeleF;
        case EAllBearer:
            return RMobilePhone::EAllBearer;
        case EAllAsync:
            return RMobilePhone::EAllAsync;
        case EAllSync:
            return RMobilePhone::EAllSync;
        case ESyncData:
            return RMobilePhone::ESyncData;
        case EAsyncData:
            return RMobilePhone::EAsyncData;
        case EPacketData:
            return RMobilePhone::EPacketDataService;
        case EPadAccess:
            return RMobilePhone::EPadAccess;
        case EAllPlmnBearer:
            return RMobilePhone::EAllPlmnBearer;
        case EPlmnBearerServ1:
            return RMobilePhone::EPlmnBearerServ1;
        case EPlmnBearerServ2:
            return RMobilePhone::EPlmnBearerServ2;
        case EPlmnBearerServ3:
            return RMobilePhone::EPlmnBearerServ3;
        case EPlmnBearerServ4:
            return RMobilePhone::EPlmnBearerServ4;
        case EPlmnBearerServ5:
            return RMobilePhone::EPlmnBearerServ5;
        case EPlmnBearerServ6:
            return RMobilePhone::EPlmnBearerServ6;
        case EPlmnBearerServ7:
            return RMobilePhone::EPlmnBearerServ7;
        case EPlmnBearerServ8:
            return RMobilePhone::EPlmnBearerServ8;
        case EPlmnBearerServ9:
            return RMobilePhone::EPlmnBearerServ9;
        case EPlmnBearerServA:
            return RMobilePhone::EPlmnBearerServA;
        case EPlmnBearerServB:
            return RMobilePhone::EPlmnBearerServB;
        case EPlmnBearerServC:
            return RMobilePhone::EPlmnBearerServC;
        case EPlmnBearerServD:
            return RMobilePhone::EPlmnBearerServD;
        case EPlmnBearerServE:
            return RMobilePhone::EPlmnBearerServE;
        case EPlmnBearerServF:
            return RMobilePhone::EPlmnBearerServF;
        case EAltTele:
            return RMobilePhone::EAuxVoiceService;
        case EUnknown:
        default:
            return RMobilePhone::EServiceUnspecified;
        }
    }

// ---------------------------------------------------------------------------
// 
// Translates given basic service code from ETEL internal code to 
// the ones used in GSM standards.
// ---------------------------------------------------------------------------
//
TBasicServiceGroups PSetUtility::ChangeToGSM( const RMobilePhone::TMobileService& aBsc )
    {
    switch ( aBsc )
        {               
        case RMobilePhone::EAllServices:
            return EAllTeleAndBearer;
        case RMobilePhone::EAllTele:
            return EAllTele;
        case RMobilePhone::EVoiceService:
        case RMobilePhone::ETelephony:
            return ETelephony;
        case RMobilePhone::EAllDataTele:
            return EAllDataTele;
        case RMobilePhone::EFaxService:
            return EFax;
        case RMobilePhone::EShortMessageService:
            return ESms;
        case RMobilePhone::EAllDataExSms:
            return EAllDataExSms;
        case RMobilePhone::EAllTeleExcSms:
            return EAllTeleExcSms;
        case RMobilePhone::EAllPlmnTele:
            return EAllPlmnTele;
        case RMobilePhone::EPlmnTele1:
            return EPlmnTele1;
        case RMobilePhone::EPlmnTele2:
            return EPlmnTele2;
        case RMobilePhone::EPlmnTele3:
            return EPlmnTele3;
        case RMobilePhone::EPlmnTele4:
            return EPlmnTele4;
        case RMobilePhone::EPlmnTele5:
            return EPlmnTele5;
        case RMobilePhone::EPlmnTele6:
            return EPlmnTele6;
        case RMobilePhone::EPlmnTele7:
            return EPlmnTele7;
        case RMobilePhone::EPlmnTele8:
            return EPlmnTele8;
        case RMobilePhone::EPlmnTele9:
            return EPlmnTele9;
        case RMobilePhone::EPlmnTeleA:
            return EPlmnTeleA;
        case RMobilePhone::EPlmnTeleB:
            return EPlmnTeleB;
        case RMobilePhone::EPlmnTeleC:
            return EPlmnTeleC;
        case RMobilePhone::EPlmnTeleD:
            return EPlmnTeleD;
        case RMobilePhone::EPlmnTeleE:
            return EPlmnTeleE;
        case RMobilePhone::EPlmnTeleF:
            return EPlmnTeleF;
        case RMobilePhone::EAllBearer:
            return EAllBearer;
        case RMobilePhone::ECircuitDataService:
        case RMobilePhone::EAllAsync:
            return EAllAsync;
        case RMobilePhone::EAllSync:
            return EAllSync;
        case RMobilePhone::ESyncData:
            return ESyncData;        
        case RMobilePhone::EAsyncData:
            return EAsyncData;
        case RMobilePhone::EPacketDataService:
        case RMobilePhone::EPacketData:
            return EPacketData;
        case RMobilePhone::EPadAccess:
            return EPadAccess;
        case RMobilePhone::EAllPlmnBearer:
            return EAllPlmnBearer;
        case RMobilePhone::EPlmnBearerServ1:
            return EPlmnBearerServ1;
        case RMobilePhone::EPlmnBearerServ2:
            return EPlmnBearerServ2;
        case RMobilePhone::EPlmnBearerServ3:
            return EPlmnBearerServ3;
        case RMobilePhone::EPlmnBearerServ4:
            return EPlmnBearerServ4;
        case RMobilePhone::EPlmnBearerServ5:
            return EPlmnBearerServ5;
        case RMobilePhone::EPlmnBearerServ6:
            return EPlmnBearerServ6;
        case RMobilePhone::EPlmnBearerServ7:
            return EPlmnBearerServ7;
        case RMobilePhone::EPlmnBearerServ8:
            return EPlmnBearerServ8;
        case RMobilePhone::EPlmnBearerServ9:
            return EPlmnBearerServ9;
        case RMobilePhone::EPlmnBearerServA:
            return EPlmnBearerServA;
        case RMobilePhone::EPlmnBearerServB:
            return EPlmnBearerServB;
        case RMobilePhone::EPlmnBearerServC:
            return EPlmnBearerServC;
        case RMobilePhone::EPlmnBearerServD:
            return EPlmnBearerServD;
        case RMobilePhone::EPlmnBearerServE:
            return EPlmnBearerServE;
        case RMobilePhone::EPlmnBearerServF:
            return EPlmnBearerServF;
        case RMobilePhone::EAuxVoiceService:
        case RMobilePhone::EAltTele:
            return EAltTele;
        case RMobilePhone::EServiceUnspecified:
        default:
            return EUnknown;
        }
    }

// ---------------------------------------------------------------------------
// Returns change information status, status is provided as TDivertingStatus,
// not as TSY's MAdvGsmLineForwarding enumeration.
// 
// ---------------------------------------------------------------------------
//
TDivertingStatus PSetUtility::GetChangeInfoStatus( 
    RMobilePhone::TMobilePhoneServiceAction& aAction )
    {
    switch ( aAction )
        {
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        case RMobilePhone::EServiceActionActivate:
        case RMobilePhone::EServiceActionRegister:
            return EDivertingStatusActive;
        case RMobilePhone::EServiceActionDeactivate:
            return EDivertingStatusInactive;
        case RMobilePhone::EServiceActionErase:
            return EDivertingStatusNotRegistered;
        default:
            return EDivertingStatusUnknown;
        }
    }

// ---------------------------------------------------------------------------
// 
// Returns divert reason, reason is provided as TSY's TCallForwardingReasonGet.
// 
// ---------------------------------------------------------------------------
//
RMobilePhone::TMobilePhoneCFCondition PSetUtility::GetDivertReason(
  TCallDivertingCondition aCondition )
    {
    switch ( aCondition )
        {
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        case EDivertConditionAllConditionalCases:
            return RMobilePhone::ECallForwardingAllConditionalCases;
        case EDivertConditionAllCalls:
            return RMobilePhone::ECallForwardingAllCases;
        case EDivertConditionUnconditional:
            return RMobilePhone::ECallForwardingUnconditional;
        case EDivertConditionBusy:
           return RMobilePhone::ECallForwardingBusy;
        case EDivertConditionNoReply:
            return RMobilePhone::ECallForwardingNoReply;
        case EDivertConditionNotReachable:
           return RMobilePhone::ECallForwardingNotReachable;
        default:
            return RMobilePhone::ECallForwardingUnspecified;
        }
    }

// ---------------------------------------------------------------------------
// 
// Returns divert reason
// 
// ---------------------------------------------------------------------------
//
TCallDivertingCondition PSetUtility::GetDivertReason(
    RMobilePhone::TMobilePhoneCFCondition aCondition )
    {
    switch ( aCondition )
        {
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        case RMobilePhone::ECallForwardingAllConditionalCases:
            return EDivertConditionAllConditionalCases;
        case RMobilePhone::ECallForwardingAllCases:
            return EDivertConditionAllCalls;
        case RMobilePhone::ECallForwardingUnconditional:
            return EDivertConditionUnconditional;
        case RMobilePhone::ECallForwardingBusy:
           return EDivertConditionBusy;
        case RMobilePhone::ECallForwardingNoReply:
            return EDivertConditionNoReply;
        case RMobilePhone::ECallForwardingNotReachable:
        default:
           return EDivertConditionNotReachable;
        }
    }

// ---------------------------------------------------------------------------
// Verify that ALS is really used.
// ---------------------------------------------------------------------------
//
RMobilePhone::TMobileService PSetUtility::VerifyAltLineUseL()
    {
    __PHSLOGSTRING("[PHS]--> PSetUtility::VerifyAltLineUseL");
    RSSSettings* ssSettings = new (ELeave) RSSSettings;
    CleanupStack::PushL( ssSettings );
    TInt usedLine;
    User::LeaveIfError( ssSettings->Open() );
    User::LeaveIfError( ssSettings->Get( ESSSettingsAls, usedLine ) );
    __PHSLOGSTRING1("[PHS]    PSetUtility::VerifyAltLineUseL: usedLine: %d", usedLine );

    if ( usedLine == ESSSettingsAlsNotSupported )
        {
        User::Leave( KErrNotSupported );
        }
    ssSettings->Close();
    CleanupStack::PopAndDestroy();
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    __PHSLOGSTRING("[PHS] <--PSetUtility::VerifyAltLineUseL");

    return RMobilePhone::EAuxVoiceService;
    }
    
//  End of File
