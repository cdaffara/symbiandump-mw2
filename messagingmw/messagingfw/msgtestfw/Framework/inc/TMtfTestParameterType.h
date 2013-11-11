/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/



/**
 @file
*/

#ifndef __TMTF_TEST_PARAMETER_TYPE_H__
#define __TMTF_TEST_PARAMETER_TYPE_H__

#include <msvstd.h>
#include <smutset.h>
#include <imapset.h> 
#include <smtpset.h> 
#include <cdbcols.h>
#include <sendas2.h>

// #include <cemailaccounts.h>

#include <cmsvattachment.h>
#include <bif.h>

#include "CMtfConfigurationType.h"

class CBaseMtm;
class CClientMtmRegistry;
class CImap4ClientMtm;
class CMmsClientMtm;
class CMsvEntry;
class CMsvEntryFilter;
class CMsvEntrySelection;
class CMsvSession;
class CMtmRegistryControl;
class CPop3ClientMtm;
class CServerMtmDllRegistry;
class CSmtpClientMtm;
class HBufC8;
class TMsvEntry;
class TUid;
class CWatcher;
class RProcess;
class RThread;
class CPop3MailInfo;
class CExpPop3MailInfo;
class CSmsSettings;
class CMsvScheduleSettings;
class CMsvOffPeakTimes;
class CMsvSendErrorActions;
class CMsvSysAgentActions;
class TPopAccount;
class TImapAccount;
class CImPop3Settings;
class CImImap4Settings;
class CImIAPPreferences;
class CImSmtpSettings;
class CMsvStore;
class RSendAs;
class RSendAsMessage;
class CDesC16ArrayFlat;
class RFs;
class RTelServer;

_LIT(KMtfTypeMismatch,"Type Mismatch");
_LIT(KMtfUnrecognisedType,"Unrecognised Type");
_LIT(KMtfParameterGeneralTypeUnrecognised,"Unrecognised General type Type");
_LIT(KMtfUnrecognisedHBufCType,"Unrecognised HBufC Type");

/** Enumerations that defines all possible general parameter types. */
enum TMtfTestParameterGeneralType
{
	EMtfUnrecognisedType,
	EMtfValueType,
	EMtfEnumeratorType,
	EMtfCBaseType,
	EMtfHBufCType,
	EMtfActionType
};

/** Enumeration that defines all possible specific parameter types. */
enum TMtfTestParameterSpecificType
{
	EMtfUnrecognisedSpecificType,
	EMtfAction,
	EMtfCBaseMtm,
	EMtfCClientMtmRegistry,
	EMtfCImap4ClientMtm,
	EMtfCMmsClientMtm,
	EMtfCMsvEntry,
	EMtfCMsvEntryFilter,
	EMtfCMsvEntrySelection,
	EMtfCMsvSession,
	EMtfCMtmRegistryControl,
	EMtfCPop3ClientMtm,
	EMtfCServerMtmDllRegistry,
	EMtfCSmtpClientMtm,
	EMtfHBufC,
	EMtfHBufC8,
	EMtfTInt,
	EMtfTMsvEntry,
	EMtfTMsvId,
	EMtfTTime,
	EMtfCWatcher,
	EMtfTMsvPriority,
	EMtfTUid,
	EMtfRProcess,
	EMtfRThread,
	EMtfTSmsDelivery,
	EMtfTSmsReportHandling,
	EMtfTSmsSettingsCommDbAction,
	EMtfTSmsAlphabet,
	EMtfTSmsPIDConversion,
	EMtfSmsValidityPeriodFormat,
	EMtfMobileSmsBearer,
	EMtfConfigurationType,
	EImPop3MailInfoType,
	EImExpMailInfo,
	EMtfTFolderSyncType,
	EMtfTFolderSubscribeType,
	EMtfTImap4GetMailOptions,
	EMtfTImImap4PartialMailOptions,
	EMtfTImImap4GetMailInfo,
	EMtfTImImap4GetPartialMailInfo,
	EMtfSmsSettings,
	EMtfScheduleSettings, 
	EMtfOffPeakTimes,
	EMtfSendErrorActions,
	EMtfSysAgtActions,
	EMtfRArrayTPop3Accounts,
	EMtfRArrayTImap4Accounts,
	EMtfTPOP3Account,
	EMtfTIMAP4Account,
	EMtfCImPop3Settings,
	EMtfCImImap4Settings,
	EMtfCImIAPPreferences,
	EMtfMsgOutboxBodyEncoding,
	ETImSMTPSendMessageOption,
	ETImSMTPSendCopyToSelf,
	EKMtfTCommDbDialogPref,
	EMtfCDesC16ArrayFlat,
	EMtfRSendAs,
	EMtfRSendAsMessage,
//	EMtfTSendAsAccount,
	EMtfTSendAsRecipientType,
//	EMtfCMsvEntry,
	EMtfCSendAsMessageTypes,
	EMtfCSendAsAccounts,
	EMtfTBioMsgIdType,
	EMtfTSendOperationState,
	EMtfTFieldList,
	EMtfCDesC16Array,
	EMtfCMsvStore,
	EMtfCImSmtpSettings,
	EMtfTMsvAttachmentId,
	EMtfCMsvAttachment,
	EMtfRFs,
	EMtfRtelServer
};
	
/** Template class that returns type information. Instantiating this template
always implies an error because it means that there is no type information for the
current parameter. */
template<class T>
class TMtfTestParameterType
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 
	{User::Panic(KMtfUnrecognisedType,0);return EMtfUnrecognisedType;}
	
	TMtfTestParameterSpecificType SpecificTypeId()
	{User::Panic(KMtfUnrecognisedType,0);return EMtfUnrecognisedSpecificType;}
};

template<>
class TMtfTestParameterType<CBaseMtm>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfCBaseMtm;}
};

template<>
class TMtfTestParameterType<CClientMtmRegistry>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfCClientMtmRegistry;}
};

template<>
class TMtfTestParameterType<CImap4ClientMtm>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfCImap4ClientMtm;}
};

template<>
class TMtfTestParameterType<CMmsClientMtm>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfCMmsClientMtm;}
};

template<>
class TMtfTestParameterType<CMsvEntry>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfCMsvEntry;}
};

template<>
class TMtfTestParameterType<CMsvEntryFilter>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfCMsvEntryFilter;}
};

template<>
class TMtfTestParameterType<CMsvEntrySelection>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfCMsvEntrySelection;}
};

template<>
class TMtfTestParameterType<CMsvSession>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfCMsvSession;}
};

template<>
class TMtfTestParameterType<CMtmRegistryControl>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfCMtmRegistryControl;}
};

template<>
class TMtfTestParameterType<CPop3ClientMtm>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfCPop3ClientMtm;}
};

template<>
class TMtfTestParameterType<CServerMtmDllRegistry>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfCServerMtmDllRegistry;}
};

template<>
class TMtfTestParameterType<CSmtpClientMtm>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfCSmtpClientMtm;}
};

template<>
class TMtfTestParameterType<HBufC>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfHBufCType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfHBufC;}
};

template<>
class TMtfTestParameterType<HBufC8>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfHBufCType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfHBufC8;}
};

template<>
class TMtfTestParameterType<TInt>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfValueType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTInt;}
};

template<>
class TMtfTestParameterType<TMsvEntry>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfValueType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTMsvEntry;}
};

template<>
class TMtfTestParameterType<TMsvId>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfValueType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTMsvId;}
};
template<>
class TMtfTestParameterType<TInt64>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfValueType;};
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTTime;};
};

template<>
class TMtfTestParameterType<TTime>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;};
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTTime;};
};

template<>
class TMtfTestParameterType<TMsvAttachmentId>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 
	{return EMtfValueType;};
	
	TMtfTestParameterSpecificType SpecificTypeId()
	{return EMtfTMsvAttachmentId;};
};

template<>
class TMtfTestParameterType<CMsvAttachment>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 
	{return EMtfCBaseType;};
	
	TMtfTestParameterSpecificType SpecificTypeId()
	{return EMtfCMsvAttachment;};
};


template<>
class TMtfTestParameterType<TMsvPriority>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfEnumeratorType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTMsvPriority;}
};

template<>
class TMtfTestParameterType<TSmsDelivery>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfEnumeratorType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTSmsDelivery;}
};

template<>
class TMtfTestParameterType<CSmsSettings::TSmsReportHandling>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfEnumeratorType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTSmsReportHandling;}
};

template<>
class TMtfTestParameterType<CSmsSettings::TSmsSettingsCommDbAction>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfEnumeratorType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTSmsSettingsCommDbAction;}
};

template<>
class TMtfTestParameterType<TSmsDataCodingScheme::TSmsAlphabet>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfEnumeratorType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTSmsAlphabet;}
};

template<>
class TMtfTestParameterType<TSmsPIDConversion>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfEnumeratorType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTSmsPIDConversion;}
};

template<>
class TMtfTestParameterType<TSmsFirstOctet::TSmsValidityPeriodFormat>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfEnumeratorType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfSmsValidityPeriodFormat;}
};

template<>
class TMtfTestParameterType<CSmsSettings::TMobileSmsBearer>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfEnumeratorType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfMobileSmsBearer;}
};

template<>
class TMtfTestParameterType<TUid>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfValueType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTUid;}
};

template<>
class TMtfTestParameterType<CWatcher>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfCWatcher;}
};

template<>
class TMtfTestParameterType<RProcess>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfValueType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfRProcess;}
};


template<>
class TMtfTestParameterType<RThread>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfValueType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfRThread;}
};

template<>
class TMtfTestParameterType<CMtfConfigurationType::TMtfConfigurationType>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfEnumeratorType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfConfigurationType;}
};


template<>
class TMtfTestParameterType<CPop3MailInfo>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}						
	TMtfTestParameterSpecificType SpecificTypeId()		{return EImPop3MailInfoType;}
};


template<>
class TMtfTestParameterType<CExpPop3MailInfo>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}						
	TMtfTestParameterSpecificType SpecificTypeId()		{return EImExpMailInfo;}

};


template<>
class TMtfTestParameterType<TFolderSyncType>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 
	{return EMtfEnumeratorType;};
	
	TMtfTestParameterSpecificType SpecificTypeId()
	{return EMtfTFolderSyncType;};
};


template<>
class TMtfTestParameterType<TFolderSubscribeType>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 
	{return EMtfEnumeratorType;};
	
	TMtfTestParameterSpecificType SpecificTypeId()
	{return EMtfTFolderSubscribeType;};
};


template<>
class TMtfTestParameterType<TImap4GetMailOptions>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfEnumeratorType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTImap4GetMailOptions;}
};


template<>
class TMtfTestParameterType<TImImap4PartialMailOptions>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfEnumeratorType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTImImap4PartialMailOptions;}
};



template<> class TMtfTestParameterType<TImImap4GetMailInfo>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfValueType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTImImap4GetMailInfo;}
};



template<> class TMtfTestParameterType<TImImap4GetPartialMailInfo>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfValueType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTImImap4GetPartialMailInfo;};
};


template<> class TMtfTestParameterType<CSmsSettings>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfSmsSettings;}
};


template<> class TMtfTestParameterType<CMsvScheduleSettings>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfScheduleSettings;}
};

template<> class TMtfTestParameterType<CMsvOffPeakTimes>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 	 	{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfOffPeakTimes;}
};

template<> class TMtfTestParameterType<CMsvSendErrorActions>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId()  		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfSendErrorActions;}
};
	
template<> class TMtfTestParameterType<CMsvSysAgentActions>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId() 		{return EMtfSysAgtActions;}
};


typedef RArray<TPopAccount> TPopAccounts;

template<>
class TMtfTestParameterType<TPopAccounts>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId()   		{return EMtfValueType;}
	TMtfTestParameterSpecificType SpecificTypeId() 		{return EMtfRArrayTPop3Accounts;}
};


typedef RArray<TImapAccount> TImapAccounts;

template<>
class TMtfTestParameterType<TImapAccounts>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId()  		{return EMtfValueType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfRArrayTImap4Accounts;}
};


template<>
class TMtfTestParameterType<TPopAccount>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId()  		{return EMtfValueType;}
	TMtfTestParameterSpecificType SpecificTypeId() 		{return EMtfTPOP3Account;}
};


template<>
class TMtfTestParameterType<TImapAccount>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId()  		{return EMtfValueType;}
	TMtfTestParameterSpecificType SpecificTypeId() 		{return EMtfTIMAP4Account;}
};

template<>
class TMtfTestParameterType<CImPop3Settings>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId()  		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId() 		{return EMtfCImPop3Settings;}
};


template<>
class TMtfTestParameterType<CImImap4Settings>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId()  		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId() 		{return EMtfCImImap4Settings;}
};

template<>
class TMtfTestParameterType<CImIAPPreferences>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId()  		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId() 		{return EMtfCImIAPPreferences;}
};



template<> class TMtfTestParameterType<TMsgOutboxBodyEncoding>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 
	{return EMtfEnumeratorType;};
	
	TMtfTestParameterSpecificType SpecificTypeId()
	{return EMtfMsgOutboxBodyEncoding;};
};


template<>
class TMtfTestParameterType<TImSMTPSendMessageOption>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 
	{return EMtfEnumeratorType;};
	
	TMtfTestParameterSpecificType SpecificTypeId()
	{return ETImSMTPSendMessageOption;};
};



template<>
class TMtfTestParameterType<TImSMTPSendCopyToSelf>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 
	{return EMtfEnumeratorType;};
	
	TMtfTestParameterSpecificType SpecificTypeId()
	{return ETImSMTPSendCopyToSelf;};
};


template<>
class TMtfTestParameterType<TCommDbDialogPref>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 
	{return EMtfEnumeratorType;};
	
	TMtfTestParameterSpecificType SpecificTypeId()
	{return EKMtfTCommDbDialogPref;};
};


template<>
class TMtfTestParameterType<CImSmtpSettings>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId()  		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId() 		{return EMtfCImSmtpSettings;}
};

template<>
class TMtfTestParameterType<CMsvStore>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 
	{return EMtfCBaseType;};
	
	TMtfTestParameterSpecificType SpecificTypeId()
	{return EMtfCMsvStore;};
};

//SendAs Common
template<>
class TMtfTestParameterType<CSendAsMessageTypes>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfCSendAsMessageTypes;}
};
template<>
class TMtfTestParameterType<CSendAsAccounts>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfCSendAsAccounts;}
};

template<>
class TMtfTestParameterType<RSendAs>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfValueType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfRSendAs;}
};

template<>
class TMtfTestParameterType<RSendAsMessage>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfValueType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfRSendAsMessage;}
};

template<>
class TMtfTestParameterType<RSendAsMessage::TSendAsRecipientType>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfEnumeratorType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTSendAsRecipientType;}
};

template<>
class TMtfTestParameterType<TBioMsgIdType>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfEnumeratorType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTBioMsgIdType;}
};

template<>
class TMtfTestParameterType<CMsvSendOperation::TSendOperationState>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfEnumeratorType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTSendOperationState;}
};

template<>
class TMtfTestParameterType<TImHeaderEncodingInfo::TFieldList>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfEnumeratorType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfTFieldList;}
};

template<>
class TMtfTestParameterType<CDesC16ArrayFlat>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId()  		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId() 		{return EMtfCDesC16ArrayFlat;}
};


template<>
class TMtfTestParameterType<CDesC16Array>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId()  		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId() 		{return EMtfCDesC16Array;}
};

template<>
class TMtfTestParameterType<RFs>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfCBaseType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfRFs;}
};

template<>
class TMtfTestParameterType<RTelServer>
{
public:
	TMtfTestParameterGeneralType GeneralTypeId() 		{return EMtfValueType;}
	TMtfTestParameterSpecificType SpecificTypeId()		{return EMtfRtelServer;}
};

#endif  // __TMTF_TEST_PARAMETER_TYPE_H__
