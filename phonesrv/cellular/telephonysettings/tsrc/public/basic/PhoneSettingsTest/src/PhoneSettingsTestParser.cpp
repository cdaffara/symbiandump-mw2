/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneSettingsTestParser class.
*
*/



// INCLUDE FILES
#include <StifLogger.h>
#include <StifTestModule.h>
#include <rsssettings.h> 
#include "MPhoneSettingsTestMessageHandler.h"
#include "PhoneSettingsTestParser.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::CPhoneSettingsTestParser
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestParser::CPhoneSettingsTestParser(
	MPhoneSettingsTestMessageHandler& aMessageHandler )
	:iMessageHandler( aMessageHandler )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestParser* CPhoneSettingsTestParser::NewL(
	MPhoneSettingsTestMessageHandler& aMessageHandler )
    {
    CPhoneSettingsTestParser* self = new(ELeave) 
    	CPhoneSettingsTestParser( aMessageHandler );     
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestParser::~CPhoneSettingsTestParser()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTServiceGroup
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTServiceGroup(TServiceGroup& aServiceGroup) 	
    {    
    iMessageHandler.PrintMessage(KServiceGroup());    
        
    switch( aServiceGroup )    
        {
        case EServiceGroupVoice: 
            iMessageHandler.PrintMessage(KServiceGroupVoice());    
            break;
        case EServiceGroupData: 
            iMessageHandler.PrintMessage(KServiceGroupData());    
            break;
        case EServiceGroupFax: 
            iMessageHandler.PrintMessage(KServiceGroupFax());    
            break;
        case EServiceGroupAllTeleservices: 
            iMessageHandler.PrintMessage(KServiceGroupAllTeleservices());    
            break;
        
        default: 
            iMessageHandler.PrintMessage(KErrorUnknownEnumeration());    
            break;         
        }
    }
    
// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTBarringProgram
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTBarringProgram(TBarringProgram& aBarringProgram)
    {   
    iMessageHandler.PrintMessage(KBarringProgram());
    switch( aBarringProgram )    
        {
        case EBarringTypeAllBarrings: 
            iMessageHandler.PrintMessage(KBarringProgramAllBarrings());    
            break;
        case EBarringTypeAllOutgoing: 
            iMessageHandler.PrintMessage(KBarringProgramAllOutgoing());    
            break;
        case EBarringTypeOutgoingInternational: 
            iMessageHandler.PrintMessage(KBarringProgramOutgoingInternational());    
            break;
        case EBarringTypeOutgoingInternationalExceptToHomeCountry: 
            iMessageHandler.PrintMessage(
                KBarringProgramInternationalExceptToHomeCountry() );    
            break;
        case EBarringTypeAllIncoming: 
            iMessageHandler.PrintMessage(KBarringProgramAllIncoming());    
            break;
        case EBarringTypeIncomingWhenRoaming: 
            iMessageHandler.PrintMessage(KBarringProgramIncomingWhenRoaming());    
            break;
        case EBarringTypeAllServices: 
            iMessageHandler.PrintMessage(KBarringProgramAllServices());    
            break;
        case EBarringTypeAllOutgoingServices: 
            iMessageHandler.PrintMessage(KBarringProgramAllOutgoingServices());    
            break;
        case EBarringTypeAllIncomingServices: 
            iMessageHandler.PrintMessage(KBarringProgramAllIncomingServices());    
            break;
        
        default: 
            iMessageHandler.PrintMessage(KErrorUnknownEnumeration());    
            break;         
        }
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTBasicServiceGroups
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTBasicServiceGroups(
    TBasicServiceGroups& aBasicServiceGroups)
    {   
    iMessageHandler.PrintMessage(KTBasicServiceGroups());
  
    switch( aBasicServiceGroups )
        {
        case EAllTeleAndBearer: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsAllTeleAndBearer());    
            break;
        case EAllTele : 
            iMessageHandler.PrintMessage(KBasicServiceGroupsAllTele());    
            break;
        case ETelephony: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsTelephony());    
            break;
        case EAllDataTele: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsAllDataTele());    
            break;
        case EFax: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsFax());    
            break;
        case ESms: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsSms());    
            break;
        case EAllDataExSms: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsAllDataExSms());    
            break;
        case EAllTeleExcSms: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsAllTeleExcSms());    
            break;
        case EAllPlmnTele: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsAllPlmnTele());    
            break;
        case EPlmnTele1: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnTele());    
            break;
        case EPlmnTele2: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnTele());    
            break;
        case EPlmnTele3: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnTele());    
            break;
        case EPlmnTele4: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnTele());    
            break;
        case EPlmnTele5: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnTele());    
            break;
        case EPlmnTele6: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnTele());    
            break;
        case EPlmnTele7: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnTele());    
            break;
        case EPlmnTele8: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnTele());    
            break;
        case EPlmnTele9: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnTele());    
            break;
        case EPlmnTeleA: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnTele());    
            break;
        case EPlmnTeleB: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnTele());    
            break;
        case EPlmnTeleC: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnTele());    
            break;
        case EPlmnTeleD: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnTele());    
            break;
        case EPlmnTeleE: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnTele());    
            break;
        case EPlmnTeleF: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnTele());    
            break;
        case EAllBearer: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsAllBearer());    
            break;
        case EAllAsync: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsAllAsync());    
            break;
        case EAllSync: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsAllSync());    
            break;
        case ESyncData: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsSyncData());    
            break;
        case EAsyncData: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsAsyncData());    
            break;
        case EPacketData: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPacketData());    
            break;
        case EPadAccess: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPadAccess());    
            break;
        case EAllPlmnBearer: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsAllPlmnBearer());    
            break;
        case EPlmnBearerServ1: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnBearerServ());    
            break;
        case EPlmnBearerServ2: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnBearerServ());    
            break;
        case EPlmnBearerServ3: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnBearerServ());    
            break;
        case EPlmnBearerServ4: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnBearerServ());    
            break;
        case EPlmnBearerServ5: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnBearerServ());    
            break;
        case EPlmnBearerServ6: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnBearerServ());    
            break;
        case EPlmnBearerServ7: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnBearerServ());    
            break;
        case EPlmnBearerServ8: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnBearerServ());    
            break;
        case EPlmnBearerServ9: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnBearerServ());    
            break;
        case EPlmnBearerServA: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnBearerServ());    
            break;
        case EPlmnBearerServB: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnBearerServ());    
            break;
        case EPlmnBearerServC: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnBearerServ());    
            break;
        case EPlmnBearerServD: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnBearerServ());    
            break;
        case EPlmnBearerServE: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnBearerServ());    
            break;
        case EPlmnBearerServF: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsPlmnBearerServ());    
            break;
        case EAltTele: 
            iMessageHandler.PrintMessage(KBasicServiceGroupsAltTele());    
            break;
        
        case EUnknown: // Not part of GSM standard 
        default: 
            iMessageHandler.PrintMessage(KErrorUnknownEnumeration());    
            break;         
        }
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTMobilePhonePasswordChangeV2
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTMobilePhonePasswordChangeV2( 
    RMobilePhone::TMobilePhonePasswordChangeV2& aTMobilePhonePasswordChangeV2)
    {
    iMessageHandler.PrintMessage( KTMobilePhonePasswordChangeV2() );
    iMessageHandler.PrintMessage( aTMobilePhonePasswordChangeV2.iVerifiedPassword );    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTMobilePhonePasswordChangeV2
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTSSSettingsSetting( 
    TSSSettingsSetting& aSsSetting)
    {
    iMessageHandler.PrintMessage( KTSSSettingsSetting() );

    switch( aSsSetting )
        {
        case ESSSettingsCug:
            iMessageHandler.PrintMessage( KTSSSettingsSettingCug() );
            break;  
        case ESSSettingsClir:
            iMessageHandler.PrintMessage( KTSSSettingsSettingClir() );
            break;  
        case ESSSettingsAls:
            iMessageHandler.PrintMessage( KTSSSettingsSettingAls() );
            break;  
        case ESSSettingsAlsBlocking:
            iMessageHandler.PrintMessage( KTSSSettingsSettingAlsBlocking() );
            break;  
        case ESSSettingsDefaultCug:
            iMessageHandler.PrintMessage( KTSSSettingsSettingDefaultCug() );
            break; 
             
        default: 
            iMessageHandler.PrintMessage( KErrorUnknownEnumeration() );    
            break;             
        }
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTCallDivertSetting
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTCallDivertSetting(TCallDivertSetting& 
    aDivertSetting)
    {
    iMessageHandler.PrintMessage( KTCallDivertSetting() );

    ParseTCallDivertingCondition( aDivertSetting.iCondition );
    ParseTServiceGroup(aDivertSetting.iServiceGroup);         
    ParseTDivertingSetting( aDivertSetting.iSetting ); 
    ParseTDivertingStatus( aDivertSetting.iStatus );      
    
    iMessageHandler.PrintMessage( KTCallDivertSettingiNumber() );
    iMessageHandler.PrintMessage( aDivertSetting.iNumber );

    iMessageHandler.PrintMessage( KTCallDivertingSettingiNoReplyTimer() );
    TBuf<KMessageHandlerMaxMessageSize> noReplyTimerBuf;     
    noReplyTimerBuf.Num(aDivertSetting.iNoReplyTimer);     
    iMessageHandler.PrintMessage( noReplyTimerBuf );
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTDivertingSetting
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTDivertingSetting(TDivertingSetting& 
    aDivertingSetting)
    {
    iMessageHandler.PrintMessage( KTDivertingSetting() );
    
    switch( aDivertingSetting )
        {
        case EActivateDiverting:
            iMessageHandler.PrintMessage( KActivateDiverting() );    
            break; 
        case ECancelDiverting:
            iMessageHandler.PrintMessage( KCancelDiverting() );    
            break; 
        case ERegisterDiverting: 
            iMessageHandler.PrintMessage( KRegisterDiverting() );    
            break; 
        case EEraseDiverting:
            iMessageHandler.PrintMessage( KEraseDiverting() );    
            break; 
        case ECheckStatus:        
            iMessageHandler.PrintMessage( KCheckStatus() );    
            break; 
        
        default: 
            iMessageHandler.PrintMessage( KErrorUnknownEnumeration() );    
            break;             
        }
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTCallDivertingCondition
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTCallDivertingCondition(TCallDivertingCondition& 
    aDivertingCondition)
    {
    iMessageHandler.PrintMessage( KTCallDivertingCondition() );
    
    switch( aDivertingCondition )
        {
        case EDivertConditionUnconditional: 
            iMessageHandler.PrintMessage( KDivertConditionUnconditional() );    
            break; 
        case EDivertConditionBusy: 
            iMessageHandler.PrintMessage( KDivertConditionBusy() );    
            break; 
        case EDivertConditionNoReply: 
            iMessageHandler.PrintMessage( KDivertConditionNoReply() );    
            break; 
        case EDivertConditionNotReachable: 
            iMessageHandler.PrintMessage( KDivertConditionNotReachable() );    
            break; 
        case EDivertConditionAllCalls: 
            iMessageHandler.PrintMessage( KDivertConditionAllCalls() );    
            break; 
        case EDivertConditionAllConditionalCases:
            iMessageHandler.PrintMessage( KDivertConditionAllConditionalCases() );    
            break; 

        default: 
            iMessageHandler.PrintMessage( KErrorUnknownEnumeration() );    
            break;             
        }
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTDivertingStatus
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTDivertingStatus(TDivertingStatus& aDivertingStatus)
    {
    iMessageHandler.PrintMessage( KTDivertingStatus() );
    
    switch( aDivertingStatus )
        {
        case EDivertingStatusActive:
            iMessageHandler.PrintMessage( KDivertingStatusActive() );    
            break; 
        case EDivertingStatusInactive:
            iMessageHandler.PrintMessage( KDivertingStatusInactive() );    
            break; 
        case EDivertingStatusNotRegistered:
            iMessageHandler.PrintMessage( KDivertingStatusNotRegistered() );    
            break; 
        case EDivertingStatusNotProvisioned:
            iMessageHandler.PrintMessage( KDivertingStatusNotProvisioned() );    
            break; 
        case EDivertingStatusUnknown:
            iMessageHandler.PrintMessage( KDivertingStatusUnknown() );    
            break; 
    
        default: 
            iMessageHandler.PrintMessage( KErrorUnknownEnumeration() );    
            break;             
        }
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTSetCallWaiting
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTSetCallWaiting(
    MPsetCallWaiting::TSetCallWaiting& aSetCallWaiting)
    {
    iMessageHandler.PrintMessage( KTSetCallWaiting() );
    
    switch( aSetCallWaiting )
        {
        case MPsetCallWaiting::EActivateCallWaiting:
            iMessageHandler.PrintMessage( KActivateCallWaiting() );    
            break; 
        case MPsetCallWaiting::EDeactivateCallWaiting:
            iMessageHandler.PrintMessage( KDeactivateCallWaiting() );    
            break; 
    
        default: 
            iMessageHandler.PrintMessage( KErrorUnknownEnumeration() );    
            break;             
        }
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTCurrentNetworkInfo
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTCurrentNetworkInfo(
    MPsetNetworkSelect::TCurrentNetworkInfo& aCurrentNetworkInfo)     
    {
    iMessageHandler.PrintMessage( KTCurrentNetworkInfo() );

    switch( aCurrentNetworkInfo.iStatus )
        {
        case MPsetNetworkSelect::ENotRegisteredNotSearching:
            iMessageHandler.PrintMessage( KENotRegisteredNotSearching() );    
            break;             
        case MPsetNetworkSelect::ERegisteredOnHomeNetwork:
            iMessageHandler.PrintMessage( KERegisteredOnHomeNetwork() );    
            break;             
        case MPsetNetworkSelect::ENotRegisteredSearching: 
            iMessageHandler.PrintMessage( KENotRegisteredSearching() );    
            break;             
        case MPsetNetworkSelect::ERegistrationDenied: 
            iMessageHandler.PrintMessage( KERegistrationDenied() );    
            break;             
        case MPsetNetworkSelect::ERegisteredRoaming:        
            iMessageHandler.PrintMessage( KERegisteredRoaming() );    
            break;             

        case MPsetNetworkSelect::EUnknown:
        default: 
            iMessageHandler.PrintMessage( KErrorUnknownEnumeration() );    
            break;             
        }

    TBuf<KMessageHandlerMaxMessageSize> messageBuffer; 
    
    iMessageHandler.PrintMessage( KLocationAreaCode() ); 
    messageBuffer.Num( aCurrentNetworkInfo.iLocationAreaCode );     
    iMessageHandler.PrintMessage( messageBuffer );     
     
    iMessageHandler.PrintMessage( KCellId() ); 
    messageBuffer.Num( aCurrentNetworkInfo.iCellId );     
    iMessageHandler.PrintMessage( messageBuffer );     
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTSelectMode
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTSelectMode(
    MPsetNetworkSelect::TSelectMode& aSelectMode) 
    {
    iMessageHandler.PrintMessage( KTSelectMode() );

    switch( aSelectMode )
        {
        case MPsetNetworkSelect::ENetSelectModeAutomatic:
            iMessageHandler.PrintMessage( KENetSelectModeAutomatic() );    
            break;             
        case MPsetNetworkSelect::ENetSelectModeManual:
            iMessageHandler.PrintMessage( KENetSelectModeManual() );    
            break;             

        default: 
            iMessageHandler.PrintMessage( KErrorUnknownEnumeration() );    
            break;             
        }
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTNetworkId
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTNetworkId(
    MPsetNetworkSelect::TNetworkId& aNetworkId)     
    {
    iMessageHandler.PrintMessage( KTNetworkId() );
    
    iMessageHandler.PrintMessage( KCountryCode() );    
    iMessageHandler.PrintMessage( aNetworkId.iCountryCode );            
    
    iMessageHandler.PrintMessage( aNetworkId.iNetworkCode );            
    iMessageHandler.PrintMessage( KNetworkCode() );    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTNetworkStatus
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTNetworkStatus(
    MPsetNetworkSelect::TNetworkStatus& aNetworkStatus)     
    {
    iMessageHandler.PrintMessage( KTNetworkStatus() );

    switch( aNetworkStatus )
        {
        case MPsetNetworkSelect::ENetStatUnknown:
            iMessageHandler.PrintMessage( KENetStatUnknown() );    
            break;             
        case MPsetNetworkSelect::ENetStatAvailable:
            iMessageHandler.PrintMessage( KENetStatAvailable() );    
            break;             
        case MPsetNetworkSelect::ENetStatCurrent:
            iMessageHandler.PrintMessage( KENetStatCurrent() );    
            break;             
        case MPsetNetworkSelect::ENetStatForbidden:
            iMessageHandler.PrintMessage( KENetStatForbidden() );    
            break;             

        default: 
            iMessageHandler.PrintMessage( KErrorUnknownEnumeration() );    
            break;             
        }
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTNetworkAccess
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTNetworkAccess(
    MPsetNetworkSelect::TNetworkAccess& aNetworkAccess )     
    {
    iMessageHandler.PrintMessage( KTNetworkAccess() );

    switch( aNetworkAccess )
        {
        case MPsetNetworkSelect::ENetNetworkGSM:
            iMessageHandler.PrintMessage( KENetNetworkGSM() );    
            break;             
        case MPsetNetworkSelect::ENetNetworkWCDMA:
            iMessageHandler.PrintMessage( KENetNetworkWCDMA() );    
            break;                         
            
        default: 
            iMessageHandler.PrintMessage( KErrorUnknownEnumeration() );    
            break;             
        }
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTNetworkInfo
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTNetworkInfo(
    MPsetNetworkSelect::TNetworkInfo& aNetworkInfo)     
    {
    iMessageHandler.PrintMessage( KTNetworkInfo() );

    ParseTNetworkId( aNetworkInfo.iId ); 
    ParseTNetworkStatus( aNetworkInfo.iStatus ); 
    ParseTSelectMode( aNetworkInfo.iMode ); 
    ParseTNetworkAccess( aNetworkInfo.iAccess ); 
    
    TBuf<KMessageHandlerMaxMessageSize> messageBuffer;  
    iMessageHandler.PrintMessage( KShortName() );            
    iMessageHandler.PrintMessage( KLongName() );            
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTNetworkModeCaps
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTNetworkModeCaps(
    RMmCustomAPI::TNetworkModeCaps& aNetworkModeCaps)
    {
    iMessageHandler.PrintMessage( KTNetworkModeCaps() );

    switch( aNetworkModeCaps )
        {
        case RMmCustomAPI::KCapsNetworkModeGsm:
            iMessageHandler.PrintMessage( KKCapsNetworkModeGsm() );
            break;             
        case RMmCustomAPI::KCapsNetworkModeUmts:
            iMessageHandler.PrintMessage( KKCapsNetworkModeUmts() );
            break;                         
        case RMmCustomAPI::KCapsNetworkModeDual:
            iMessageHandler.PrintMessage( KKCapsNetworkModeDual() );
            break;                   
            
        default: 
            iMessageHandler.PrintMessage( KErrorUnknownEnumeration() );    
            break;             
        }
    }
    
// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTUnconditionalCFStatus
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTUnconditionalCFStatus(
    TUnconditionalCFStatus& aUnconditionalCFStatus)
    {
    iMessageHandler.PrintMessage( KTUnconditionalCFStatus() );

    switch( aUnconditionalCFStatus )
        {
        case KCFIndicatorUnknown:
            iMessageHandler.PrintMessage( KKCFIndicatorUnknown() );    
            break;             
        case KCFNoCallsForwarded :
            iMessageHandler.PrintMessage( KKCFNoCallsForwarded() );    
            break;             
        case KCFVoiceForwarded:
            iMessageHandler.PrintMessage( KKCFVoiceForwarded() );    
            break;             
        case KCFVideoForwarded:
            iMessageHandler.PrintMessage( KKCFVideoForwarded() );    
            break;             
        case KCFForwardedToVoiceMailbox:
            iMessageHandler.PrintMessage( KKCFForwardedToVoiceMailbox() );    
            break;             
        case KCFVoiceForwardedToVoiceMailbox:
            iMessageHandler.PrintMessage( KKCFVoiceForwardedToVoiceMailbox() );    
            break;             
        case KCFVideoForwardedToVoiceMailbox:
            iMessageHandler.PrintMessage( KKCFVideoForwardedToVoiceMailbox() );    
            break;             
        case KCFForwardedOnLine1:
            iMessageHandler.PrintMessage( KKCFForwardedOnLine1() );    
            break;             
        case KCFForwardedOnLine2:
            iMessageHandler.PrintMessage( KKCFForwardedOnLine2() );    
            break;             

        default: 
            iMessageHandler.PrintMessage( KErrorUnknownEnumeration() );    
            break;             
        }
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTSelectedLine
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTSelectedLine(TSelectedLine& aSelectedLine)
    {
    iMessageHandler.PrintMessage( KTSelectedLine() );

    switch( aSelectedLine )
        {
        case EPrimaryLine:
            iMessageHandler.PrintMessage( KEPrimaryLine() );
            break;             
        case EAuxiliaryLine:
            iMessageHandler.PrintMessage( KEAuxiliaryLine() );
            break;             
        case EUnknownLine:
            iMessageHandler.PrintMessage( KEUnknownLine() );
            break;             
        case ENotSupportedLine:
            iMessageHandler.PrintMessage( KENotSupportedLine() );
            break;             

        default: 
            iMessageHandler.PrintMessage( KErrorUnknownEnumeration() );    
            break;             
        }    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTCallDivertNotifySetting
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTCallDivertNotifySetting(
    TCallDivertNotifySetting& aCallDivertNotifySetting)
    {
    iMessageHandler.PrintMessage( KTCallDivertNotifySetting() );

    ParseTUnconditionalCFStatus( aCallDivertNotifySetting.iPreviousCfStatus );
    
    iMessageHandler.PrintMessage( KCfActivated() );
    ParseTBool( aCallDivertNotifySetting.iCfActivated ); 

    iMessageHandler.PrintMessage( KVmbxDivert() );
    ParseTBool( aCallDivertNotifySetting.iVmbxDivert ); 

    iMessageHandler.PrintMessage( KBasicServiceCode() );
    TBuf<KMessageHandlerMaxMessageSize> messageBuffer;         
    messageBuffer.Num( aCallDivertNotifySetting.iBasicServiceCode );         
    iMessageHandler.PrintMessage( messageBuffer );
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestParser::ParseTBool
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestParser::ParseTBool(TBool aBool)
    {
    iMessageHandler.PrintMessage( KTBoolValue() );

    if( aBool )
        {
        iMessageHandler.PrintMessage( KTrue() );    
        }
    else
        {
        iMessageHandler.PrintMessage( KFalse() );    
        }
    }
        

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
