/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Various definitions
*
*
*/


#ifndef     NWDEFS_H
#define     NWDEFS_H

//  INCLUDES
#include <e32std.h>
#include "psetconstants.h" 

//  DATA TYPES  
//basic service groups
enum TBasicServiceGroups
    {
    EUnknown = -1, //this is not part of GSM standard
    // Phone settings updates this automatically to ETelephony or to EAltTele,
    // depending on if ALS is supported, and which line is active.
    EAllTeleAndBearer = 0,
    EAllTele = 10,
    ETelephony = 11,
    EAllDataTele = 12,
    EFax = 13,
    ESms = 16,
    EAllDataExSms = 18, //voice broadcast in gsm 02.30 v. 7.1.0
    EAllTeleExcSms = 19,

    EAllPlmnTele = 50,
    EPlmnTele1 = 51,
    EPlmnTele2 = 52,
    EPlmnTele3 = 53,
    EPlmnTele4 = 54,
    EPlmnTele5 = 55,
    EPlmnTele6 = 56,
    EPlmnTele7 = 57,
    EPlmnTele8 = 58,
    EPlmnTele9 = 59,
    EPlmnTeleA = 60,
    EPlmnTeleB = 61,
    EPlmnTeleC = 62,
    EPlmnTeleD = 63,
    EPlmnTeleE = 64,
    EPlmnTeleF = 65,

    EAllBearer = 20,
    EAllAsync = 21,
    EAllSync = 22,
    ESyncData = 24,
    EAsyncData = 25,
    EPacketData = 26,
    EPadAccess = 27, 

    EAllPlmnBearer = 70,
    EPlmnBearerServ1 = 71,
    EPlmnBearerServ2 = 72,
    EPlmnBearerServ3 = 73,
    EPlmnBearerServ4 = 74,
    EPlmnBearerServ5 = 75,
    EPlmnBearerServ6 = 76,
    EPlmnBearerServ7 = 77,
    EPlmnBearerServ8 = 78,
    EPlmnBearerServ9 = 79,
    EPlmnBearerServA = 80,
    EPlmnBearerServB = 81,
    EPlmnBearerServC = 82,
    EPlmnBearerServD = 83,
    EPlmnBearerServE = 84,
    EPlmnBearerServF = 85,

    EAltTele = 89  // no ui note support
    };

// basic teleservice groups (ETSI Gsm 02.03 and 02.04)
enum TServiceGroup {
        EServiceGroupVoice  = 0x00000001,
        EServiceGroupData   = 0x00000002,
        EServiceGroupFax    = 0x00000004,
        EServiceGroupAllTeleservices = EServiceGroupVoice | 
                                       EServiceGroupData | 
                                       EServiceGroupFax
        };

/////////////////////////////////////////////////////////////////////////////
// barring data types
//barring supplementary services
enum TBarringProgram
    {
    EBarringTypeAllBarrings                              = 0x0001,
    EBarringTypeAllOutgoing                              = 0x0002,
    EBarringTypeOutgoingInternational                    = 0x0004,
    EBarringTypeOutgoingInternationalExceptToHomeCountry = 0x0008,
    EBarringTypeAllIncoming                              = 0x0010,
    EBarringTypeIncomingWhenRoaming                      = 0x0020,
    EBarringTypeAllServices,
    EBarringTypeAllOutgoingServices,
    EBarringTypeAllIncomingServices
    };

//barring actions
enum TBarringSetting
    {
    EActivateBarring,
    ECancelBarring
    };

//barring status
enum TBarringStatus
    {
    EBarringStatusActive,
    EBarringStatusInactive,
    EBarringStatusNotProvisioned,
    EBarringStatusUnavailable,
    EBarringStatusUnknown
    };

/////////////////////////////////////////////////////////////////////////////
// diverting data types
//call forwarding conditions
enum TCallDivertingCondition {
    EDivertConditionUnconditional,
    EDivertConditionBusy,
    EDivertConditionNoReply,
    EDivertConditionNotReachable,
    EDivertConditionAllCalls,
    EDivertConditionAllConditionalCases
    };

//call forwarding actions
enum TDivertingSetting
    {
    EActivateDiverting,
    ECancelDiverting, 
    ERegisterDiverting, 
    EEraseDiverting,
    ECheckStatus
    };

//call forwarding statuses
enum TDivertingStatus
    {
    EDivertingStatusActive,
    EDivertingStatusInactive,
    EDivertingStatusNotRegistered,
    EDivertingStatusNotProvisioned,
    EDivertingStatusUnknown
    };

//call forwarding cenrep key statuses.
enum TCallForwardingStatus
    {
    EForwardingUnknown      = 0x0,
    EForwardingActiveVoice  = 0x01,
    EForwardingActiveFax    = 0x02,
    EForwardingActiveData   = 0x04
    };
        
//  CLASS DEFINITIONS 
// Supplemental class to contain all call barring settings.
class   TCallBarringSetting
    {
    public: //member data
        //Barring program
        TBarringProgram     iType;
        //Barring settings
        TBarringSetting     iSetting;
        //Barring group
        TServiceGroup       iServiceGroup;
        //Barring password
        TBarringPassword    iPassword;
    };

// Supplemental class to contain all call forwarding settings.
class   TCallDivertSetting
    {
    public: //new
        void Initialize()
            {
            iCondition = EDivertConditionUnconditional;
            iStatus = EDivertingStatusUnknown;
            iServiceGroup = EServiceGroupAllTeleservices;
            iNumber = KNullDesC;
            iNoReplyTimer = 0;
            }

    public: //member data
        //Divert condition
        TCallDivertingCondition iCondition;
        //Divert setting
        TDivertingSetting       iSetting;
        //Divert status
        TDivertingStatus        iStatus;
        //Divert service group
        TServiceGroup           iServiceGroup;
        //Diverted-to number
        TTelNumber              iNumber;
        //Delay time before starting diverting
        TInt                    iNoReplyTimer;
    };
#endif // NWDEFS_H
// End of file
