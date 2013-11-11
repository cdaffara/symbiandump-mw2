// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Utility functions for reading the config/setting file to update
// the CImImap4Settings, CImPop3Settings, CImSmtpSettings and CImIAPPreferences objects.
// 
//


// User include
#include <t_utilsenumconverter.h>
#include <t_utilsconfigfileparserutility.h>
#include <t_utilsconfigfilemachinename.h>
#include <t_utilsreademailsettingsfromconfigfile.h>

// epoc inlcude
#include <f32file.h>
#include <cdbcols.h>
#include <imapset.h>
#include <smtpset.h>
#include <iapprefs.h>

// Literals used

// Imap Settings related literals
_LIT(KCfgImapServerAddress, "ImapServerAddress"); 
_LIT(KCfgImapLoginName, "ImapLoginName"); 
_LIT(KCfgImapPassword, "ImapPassword");
_LIT(KCfgImapPort, "ImapPort"); 
_LIT(KCfgImapSecureSockets, "ImapSecureSockets");
_LIT(KCfgImapSSLWrapper, "ImapSSLWrapper");
_LIT(KCfgImapFolderPath, "ImapFolderPath");
_LIT(KCfgImapPathSeperator, "ImapPathSeperator");
_LIT(KCfgImapMaxEmailSize, "ImapMaxEmailSize");
_LIT(KCfgImapGetMailOptions, "ImapGetMailOptions");
_LIT(KCfgImapInboxSynchronisationLimit, "ImapInboxSynchronisationLimit");
_LIT(KCfgImapMailboxSynchronisationLimit, "ImapMailSynchronisationLimit");
_LIT(KUseExpunge , "ImapUseExpunge" );
_LIT(KCfgImapAutoSendOnConnect, "ImapAutoSendOnConnect" );
_LIT(KCfgImapDeleteEmailsWhenDisconnecting, "ImapDeleteEmailsWhenDisconnecting" );
_LIT(KCfgImapAcknowledgeReceipts, "ImapAcknowledgeReceipts" );
_LIT(KCfgImapDisconnectedUserMode, "ImapDisconnectedUserMode" );
_LIT(KCfgImapSynchronise, "ImapSynchronise" );
_LIT(KCfgImapSubscribe, "ImapSubscribe" );
_LIT(KCfgImapUpdatingSeenFlags, "ImapUpdatingSeenFlags" );
_LIT(KCfgImapSyncRate, "ImapSyncRate" );
_LIT(KCfgImapFetchSize, "ImapFetchSize" );
_LIT(KCfgImapIdle, "ImapIdle" );
_LIT(KCfgImapIdleTimeout, "ImapIdleTimeout" );
_LIT(KCfgImapPartialMailOptions, "ImapPartialMailOptions" );
_LIT(KCfgImapBodyTextSizeLimit, "ImapBodyTextSizeLimit" ); 
_LIT(KCfgImapAttachmentSizeLimit, "ImapAttachmentSizeLimit" );

_LIT(KCfgImapIapList, "ImapIapList");
_LIT(KCfgImapSNAPId, "ImapSNAPId");
_LIT(KCfgImapBearerMobility, "ImapBearerMobility");
_LIT(KCfgImapDownloadRules, "ImapDownloadRules");
//PREQ1119 IMAP changes
_LIT(KCfgImapAuth, "ImapAuth");

_LIT(KCfgImapFallBack, "ImapFallBack");

// REQ 6985

_LIT(KCfgTlsSslDomainName, "TlsSslDomainName");

//
// POP Settings related literals
//
_LIT(KCfgPopServerAddress, "PopServerAddress");
_LIT(KCfgPopLoginName, "PopLoginName");
_LIT(KCfgPopPassword, "PopPassword");
_LIT(KCfgPopMaxEmailSize, "PopMaxEmailSize");
_LIT(KCfgPopPort, "PopPort");
_LIT(KCfgPopInboxSynchronisationLimit, "PopInboxSynchronisationLimit");
_LIT(KCfgPopSecureSockets, "PopSecureSockets");
_LIT(KCfgPopSSLWrapper, "PopSSLWrapper");
_LIT(KCfgPopGetMailOptions, "PopGetMailOptions");
_LIT(KCfgPopPopulationLimit, "PopPopulationLimit");
_LIT(KCfgPopAutoSendOnConnect, "PopAutoSendOnConnect" );
_LIT(KCfgPopDeleteEmailsWhenDisconnecting, "PopDeleteEmailsWhenDisconnecting" );
_LIT(KCfgPopAcknowledgeReceipts, "PopAcknowledgeReceipts" );
_LIT(KCfgPopDisconnectedUserMode, "PopDisconnectedUserMode" );
_LIT(KCfgPopAPOPEncryption, "PopAPOPEncryption" );

_LIT(KCfgPopIapList, "PopIapList");
_LIT(KCfgPopSNAPId, "PopSNAPId");
_LIT(KCfgPopBearerMobility, "PopBearerMobility");

//PREQ1119 POP changes
_LIT(KCfgPop3Auth, "Pop3Auth");
_LIT(KCfgPop3FallBack, "Pop3FallBack");


//
// SMTP settings related literals
//
_LIT(KCfgSmtpServerAddress, "SmtpServerAddress");
_LIT(KCfgSmtpEmailAddress, "SmtpEmailAddress");
_LIT(KCfgSmtpBodyEncoding, "SmtpBodyEncoding");
_LIT(KCfgSmtpReceiptAddress, "SmtpReceiptAddress");
_LIT(KCfgSmtpRequestReceipts, "SmtpRequestReceipts");
_LIT(KCfgSmtpSendMessageOption, "SmtpSendMessageOption");
_LIT(KCfgSmtpEmailAlias, "SmtpEmailAlias");
_LIT(KCfgSmtpDefaultMsgCharSet, "SmtpMsgCharSet");
_LIT(KCfgSmtpSetPort, "SmtpSetPort");
_LIT(KCfgSmtpSetReplyToAddress, "SmtpSetReplyToAddress");
_LIT(KCfgSmtpSetAddVCardToEmail, "SmtpSetAddVCardToEmail");
_LIT(KCfgSmtpSetAddSignatureToEmail, "SmtpSetAddSignatureToEmail");
_LIT(KCfgSmtpSetSendCopyToSelf, "SmtpSetSendCopyToSelf");
_LIT(KCfgSmtpSetLoginName, "SmtpSetLoginName");
_LIT(KCfgSmtpSetPassword, "SmtpSetPassword");
_LIT(KCfgSmtpSetSMTPAuth, "SmtpSetSMTPAuth");
_LIT(KCfgSmtpSetInboxLoginDetails, "SmtpSetInboxLoginDetails");
_LIT(KCfgSmtpSecureSockets, "SmtpSetSecureSockets");
_LIT(KCfgSmtpSSLWrapper, "SmtpSetSSLWrapper");

_LIT(KCfgSmtpIapList, "SmtpIapList");
_LIT(KCfgSmtpSNAPId, "SmtpSNAPId");
_LIT(KCfgSmtpBearerMobility, "SmtpBearerMobility");

// Other Literals
_LIT(KImapIAPSuffix, "_ImapIAP");
_LIT(KImapCommDbDialogPref, "_ImapCommDbDialogPref");
_LIT(KImapIAPIndex, "_ImapIAPIndex");
_LIT(KPopIAPSuffix, "_PopIAP");
_LIT(KPopCommDbDialogPref, "_PopCommDbDialogPref");
_LIT(KPopIAPIndex, "_PopIAPIndex");
_LIT(KSmtpIAPSuffix, "_SmtpIAP");
_LIT(KSmtpCommDbDialogPref, "_SmtpCommDbDialogPref");
_LIT(KSmtpIAPIndex, "_SmtpIAPIndex");


/**
ReadImapSettingsFromConfigurationFileL()
Modifies the IMAP related default setting and IAP objects.

@param  aSettingsFile
Configuration/setting file path. 

@param  aImapSet
CImImap4Settings object.

@param  aImapIapPref
CImIAPPreferences object.
*/
EXPORT_C void CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadImapSettingsFromConfigurationFileL(
															TDesC& aSettingsFile, CImImap4Settings& aImapSet, CImIAPPreferences& aImapIapPref,const TDesC& /* aPtrEmailAdd */)

	{
	// Parses the setting file
	CT_MsgUtilsConfigFileParserUtility* scriptFileParser = CT_MsgUtilsConfigFileParserUtility::NewL(aSettingsFile);
	CleanupStack::PushL(scriptFileParser);	
	
	TPtrC8 string8Ptr; 
	TPtrC stringPtr = KNone(); 
	TInt intTmp;
		
	if(scriptFileParser->GetFieldAsString(KCfgImapServerAddress, stringPtr) == KErrNone)
		{
		aImapSet.SetServerAddressL(stringPtr);
		}

	CT_MsgUtilsConfigFileMachineName* machineNameFile = CT_MsgUtilsConfigFileMachineName::NewLC(stringPtr);
	TPtrC8 machineName(machineNameFile->MachineName());
	
	if(scriptFileParser->GetFieldAsString8(KCfgImapLoginName, string8Ptr) == KErrNone)
		{
		aImapSet.SetLoginNameL(string8Ptr);
		}
	else
		{
		aImapSet.SetLoginNameL(machineName);
		}

	if(scriptFileParser->GetFieldAsString8(KCfgImapPassword, string8Ptr) == KErrNone)
		{
		aImapSet.SetPasswordL(string8Ptr);
		}
	else
		{
		aImapSet.SetPasswordL(machineName);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgImapPort, intTmp) == KErrNone)
		{
		aImapSet.SetPort(intTmp);
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgImapSecureSockets, intTmp) == KErrNone)
		{		
		aImapSet.SetSecureSockets( (TBool) intTmp );
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgImapSSLWrapper, intTmp ) == KErrNone)
		{		
		aImapSet.SetSSLWrapper( (TBool) intTmp );
		}

	if(scriptFileParser->GetFieldAsString8(KCfgImapFolderPath, string8Ptr) == KErrNone)
		{
		aImapSet.SetFolderPathL(string8Ptr);
		}

	if(scriptFileParser->GetFieldAsString8(KCfgImapPathSeperator, string8Ptr) == KErrNone)
		{
		if ( string8Ptr.Length() == 1  )
			{
			aImapSet.SetPathSeparator( string8Ptr[0] );			
			}
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgImapMaxEmailSize, intTmp) == KErrNone)
		{
		aImapSet.SetMaxEmailSize(intTmp);
		}
	
	if(scriptFileParser->GetFieldAsString(KCfgImapGetMailOptions, stringPtr) == KErrNone)
		{
		TImap4GetMailOptions setGetMailOptions = CT_MsgUtilsEnumConverter::ConvertDesToTImap4GetMailOptions(stringPtr);
		aImapSet.SetGetMailOptions(setGetMailOptions);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgImapInboxSynchronisationLimit, intTmp) == KErrNone)
		{
		aImapSet.SetInboxSynchronisationLimit(intTmp);
		}
		
	if(scriptFileParser->GetFieldAsInteger(KCfgImapMailboxSynchronisationLimit, intTmp) == KErrNone)
		{
		aImapSet.SetMailboxSynchronisationLimit(intTmp);
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgImapAutoSendOnConnect, intTmp) == KErrNone)
		{		
		aImapSet.SetAutoSendOnConnect((TBool) intTmp);
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgImapDeleteEmailsWhenDisconnecting, intTmp) == KErrNone)
		{		
		aImapSet.SetDeleteEmailsWhenDisconnecting((TBool) intTmp);
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgImapAcknowledgeReceipts, intTmp) == KErrNone)
		{		
		aImapSet.SetAcknowledgeReceipts((TBool) intTmp);
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgImapDisconnectedUserMode, intTmp) == KErrNone)
		{		
		aImapSet.SetDisconnectedUserMode((TBool) intTmp);
		}

	if(scriptFileParser->GetFieldAsString(KCfgImapSynchronise, stringPtr) == KErrNone)
		{
		TFolderSyncType setFolderSyncType = CT_MsgUtilsEnumConverter::ConvertDesToTFolderSyncType(stringPtr);
		aImapSet.SetSynchronise(setFolderSyncType);
		}

	if(scriptFileParser->GetFieldAsString(KCfgImapSubscribe, stringPtr) == KErrNone)
		{
		TFolderSubscribeType setFolderSubscribeType = CT_MsgUtilsEnumConverter::ConvertDesToTFolderSubscribeType(stringPtr);
		aImapSet.SetSubscribe(setFolderSubscribeType);
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgImapUpdatingSeenFlags, intTmp) == KErrNone)
		{		
		aImapSet.SetUpdatingSeenFlags((TBool) intTmp);
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgImapSyncRate, intTmp) == KErrNone)
		{
		aImapSet.SetSyncRateL(intTmp);
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgImapFetchSize, intTmp) == KErrNone)
		{
		aImapSet.SetFetchSizeL(intTmp);
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgImapIdle, intTmp) == KErrNone)
		{		
		aImapSet.SetImapIdle( (TBool) intTmp );
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgImapIdleTimeout, intTmp) == KErrNone)
		{
		aImapSet.SetImapIdleTimeoutL(intTmp);
		}

	if(scriptFileParser->GetFieldAsString(KCfgImapPartialMailOptions, stringPtr) == KErrNone)
		{
		TImImap4PartialMailOptions setImap4PartialMailOptions = CT_MsgUtilsEnumConverter::ConvertDesToTImImap4PartialMailOptions(stringPtr);
		aImapSet.SetPartialMailOptionsL(setImap4PartialMailOptions);
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgImapBodyTextSizeLimit, intTmp) == KErrNone)
		{
		aImapSet.SetBodyTextSizeLimitL(intTmp);
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgImapAttachmentSizeLimit, intTmp) == KErrNone)
		{
		aImapSet.SetAttachmentSizeLimitL(intTmp);
		}

	if(scriptFileParser->GetFieldAsString(KCfgImapIapList, stringPtr) == KErrNone)
		{
		ReadImapIAPAndAddL(scriptFileParser, (TDesC&)KCfgImapIapList, aImapIapPref);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KUseExpunge, intTmp) == KErrNone)
		{		
		aImapSet.SetUseExpunge( (TBool) intTmp);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgImapSNAPId, intTmp) == KErrNone)
		{		
		aImapIapPref.SetSNAPL(intTmp);
		}
		
	if(scriptFileParser->GetFieldAsInteger(KCfgImapBearerMobility, intTmp) == KErrNone)
		{		
		aImapSet.SetBearerMobility((TBool) intTmp);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgImapDownloadRules, intTmp) == KErrNone)
		{		
		aImapSet.SetUseSyncDownloadRules((TBool) intTmp);
		}	
			
	if(scriptFileParser->GetFieldAsString8(KCfgTlsSslDomainName, string8Ptr) == KErrNone)
		{
		aImapSet.SetTlsSslDomainL(string8Ptr);
		}
#if (defined SYMBIAN_EMAIL_CAPABILITY_SUPPORT)	
		if(scriptFileParser->GetFieldAsInteger(KCfgImapAuth, intTmp) == KErrNone)
		{		
		aImapSet.SetIMAP4Auth( (TBool) intTmp);
		}
		if(scriptFileParser->GetFieldAsInteger(KCfgImapFallBack, intTmp) == KErrNone)
		{		
		aImapSet.SetFallBack( (TBool) intTmp);
		}
#endif	
	
	CleanupStack::PopAndDestroy(2,scriptFileParser);
	}

/**
ReadImapIAPAndAdd()_
Gets the base string for the IAP list.

@param  aScriptFileParser
CT_MsgUtilsConfigFileParserUtility object.

@param  aParamListName
@param  aImapIapPref
CImIAPPreferences object.
*/	
EXPORT_C void CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadImapIAPAndAddL(CT_MsgUtilsConfigFileParserUtility* aScriptFileParser, const TDesC& aParamListName, CImIAPPreferences& aImapIapPref)
	{
	TPtrC stringPtr; 	
	TPtrC baseString; 
	
	if( aScriptFileParser->GetFieldAsString(aParamListName, stringPtr ) == KErrNone )
		{
		TInt start = 0;
		TInt end = stringPtr.Length();
		
		while (GetString(stringPtr, start, end, baseString))
			{
			ReadImapIAPAndAddFieldL(aScriptFileParser, baseString, aImapIapPref);
			//ReadImapIAPAndAddL(aScriptFileParser, baseString, aImapIapPref);
			}
		}		
	}

/**
ReadImapIAPAndAddField()
Modifies the CImIAPPreferences object.

@param  aScriptFileParser
CT_MsgUtilsConfigFileParserUtility object.

@param  aFieldName 
@param  aImapIapPref
CImIAPPreferences object.
*/
EXPORT_C void CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadImapIAPAndAddFieldL( 
										CT_MsgUtilsConfigFileParserUtility* aScriptFileParser, 
										const TDesC& aFieldName, CImIAPPreferences& aImapIapPref)
	{
	
	TPtrC stringPtr; 
	TInt intTmp;
	
	TImIAPChoice iapChoice;
	iapChoice.iIAP = 0;
	iapChoice.iDialogPref = ECommDbDialogPrefUnknown;
	
	TInt index = 0;
	TBool modified = EFalse;

	HBufC* buf = HBufC::NewLC(100);
			
	buf->Des() = aFieldName;
	buf->Des().Append(KImapIAPSuffix);
			
	if(aScriptFileParser->GetFieldAsInteger(buf->Des(), intTmp) == KErrNone)
		{
		iapChoice.iIAP = intTmp;
		modified = ETrue;
		}

	buf->Des() = aFieldName;
	buf->Des().Append(KImapCommDbDialogPref);
	
	if(aScriptFileParser->GetFieldAsString(buf->Des(), stringPtr) == KErrNone)
		{
		TCommDbDialogPref setCommDbDialogPref = CT_MsgUtilsEnumConverter::ConvertDesToTCommDbDialogPref(stringPtr);
		iapChoice.iDialogPref = setCommDbDialogPref;
		modified = ETrue;
		}
	
	buf->Des() = aFieldName;
	buf->Des().Append(KImapIAPIndex);
	if(aScriptFileParser->GetFieldAsInteger(buf->Des(), intTmp) == KErrNone)
		{
		index = intTmp;
		modified = ETrue;
		}
	
	if (modified)
		{ 
		aImapIapPref.AddIAPL(iapChoice ,index );
		}

	CleanupStack::PopAndDestroy(buf);
	}


/**
GetString()

@param aStringPtr
@param aStart
@param aEnd
@param aBaseString
@return 
*/
EXPORT_C TBool CT_MsgUtilsReadEmailSettingsFromConfigFile::GetString(TDesC& aStringPtr, TInt& aStart, TInt aEnd, TPtrC& aBaseString)
	{
	TBool ret = EFalse;

	if (aStart == aEnd)
		{
		return EFalse;				
		}
	
	TInt currPos = aStart;
	
	// strip out any starting spaces.
	while (currPos < aEnd )
		{
		if (aStringPtr[currPos] == ' ' || aStringPtr[currPos] == '\t')
			{			
			currPos++;	
			}
		else
			{			
			break;	
			}
		}
	TInt startTmp = currPos;
	//Looking for terminating space or Tab to determine the length of string
	while (currPos < aEnd )
		{
		if (aStringPtr[currPos] == ' ' || aStringPtr[currPos] == '\t')
			{			
			break;
			}
		else
			{
			currPos++;
			ret = ETrue;
			}
		}
	
	if (ret)
		{	// We have a string.
		aBaseString.Set(aStringPtr.Mid( startTmp, currPos - startTmp)); 
		}
		
	aStart = currPos;
	return ret;
	}

/**
ReadPopSettingsFromConfigurationFileL()
Modifies the POP related default setting and IAP objects.

@param  aSettingsFile
Configuration/setting file path. 

@param  aPopSet
CImPop3Settings object.

@param  aPopIapPref
*/
EXPORT_C void CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadPopSettingsFromConfigurationFileL(TDesC& aSettingsFile, 
											CImPop3Settings& aPopSet, CImIAPPreferences& aPopIapPref, const TDesC& /*aPtrEmailAdd*/)
	{
	// Parses the setting file
	CT_MsgUtilsConfigFileParserUtility* scriptFileParser = CT_MsgUtilsConfigFileParserUtility::NewL(aSettingsFile);
	CleanupStack::PushL(scriptFileParser);	
	
	TPtrC8 string8Ptr; 
	TPtrC stringPtr = KNone(); 
	TInt intValue;

	if(scriptFileParser->GetFieldAsString(KCfgPopServerAddress, stringPtr) == KErrNone)
		{	
		aPopSet.SetServerAddressL(stringPtr);
		}

	CT_MsgUtilsConfigFileMachineName* machineNameFile = CT_MsgUtilsConfigFileMachineName::NewLC(stringPtr);
	TPtrC8 machineName(machineNameFile->MachineName());

	
	if(scriptFileParser->GetFieldAsString8(KCfgPopLoginName, string8Ptr) == KErrNone)
		{
		aPopSet.SetLoginNameL(string8Ptr);
		}

	else
		{
		aPopSet.SetLoginNameL(machineName);
		}
	
	if(scriptFileParser->GetFieldAsString8(KCfgPopPassword, string8Ptr) == KErrNone)
		{
		aPopSet.SetPasswordL(string8Ptr);
		}

	else
		{
		aPopSet.SetPasswordL(machineName);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgPopMaxEmailSize, intValue) == KErrNone)
		{
		aPopSet.SetMaxEmailSize(intValue);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgPopPort, intValue) == KErrNone)
		{
		aPopSet.SetPort(intValue);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgPopInboxSynchronisationLimit, intValue) == KErrNone)
		{
		aPopSet.SetInboxSynchronisationLimit(intValue);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgPopSecureSockets, intValue) == KErrNone)
		{
		aPopSet.SetSecureSockets((TBool)intValue);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgPopSSLWrapper, intValue) == KErrNone)
		{
		aPopSet.SetSSLWrapper((TBool)intValue);
		}
	
	if(scriptFileParser->GetFieldAsString(KCfgPopGetMailOptions, stringPtr) == KErrNone)
		{
		TPop3GetMailOptions options = CT_MsgUtilsEnumConverter::ConvertDesToTPop3GetMailOptions(stringPtr);
		aPopSet.SetGetMailOptions(options);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgPopPopulationLimit, intValue) == KErrNone)
		{
		aPopSet.SetPopulationLimitL(intValue);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgPopAutoSendOnConnect, intValue) == KErrNone)
		{	
		aPopSet.SetAutoSendOnConnect((TBool) intValue);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgPopDeleteEmailsWhenDisconnecting, intValue) == KErrNone)
		{	
		aPopSet.SetDeleteEmailsWhenDisconnecting((TBool) intValue);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgPopAcknowledgeReceipts, intValue) == KErrNone)
		{	
		aPopSet.SetAcknowledgeReceipts((TBool) intValue);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgPopDisconnectedUserMode, intValue) == KErrNone)
		{	
		aPopSet.SetDisconnectedUserMode((TBool) intValue);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgPopAPOPEncryption, intValue) == KErrNone)
		{	
		aPopSet.SetApop((TBool) intValue);
		}
		
	if(scriptFileParser->GetFieldAsString(KCfgPopIapList, stringPtr) == KErrNone)
		{
		ReadPopIAPAndAddL(scriptFileParser, (TDesC&) KCfgPopIapList, aPopIapPref);
		}
		
	if(scriptFileParser->GetFieldAsInteger(KCfgPopSNAPId, intValue) == KErrNone)
		{		
		aPopIapPref.SetSNAPL(intValue);
		}
		
	if(scriptFileParser->GetFieldAsInteger(KCfgPopBearerMobility, intValue) == KErrNone)
		{	
		aPopSet.SetBearerMobility((TBool) intValue);
		}
		
	if(scriptFileParser->GetFieldAsString8(KCfgTlsSslDomainName, string8Ptr) == KErrNone)
		{
		aPopSet.SetTlsSslDomainL(string8Ptr);
		}
#if (defined SYMBIAN_EMAIL_CAPABILITY_SUPPORT)	
	if(scriptFileParser->GetFieldAsInteger(KCfgPop3Auth, intValue) == KErrNone)
		{	
		aPopSet.SetPOP3Auth((TBool) intValue);
		}
	if(scriptFileParser->GetFieldAsInteger(KCfgPop3FallBack, intValue) == KErrNone)
		{	
		aPopSet.SetFallBack((TBool) intValue);
		}
#endif		
	CleanupStack::PopAndDestroy(2,scriptFileParser);
	}

/**
*
ReadPopIAPAndAdd()
Gets the base string for the IAP list.

@param  aScriptFileParser
CT_MsgUtilsConfigFileParserUtility object.

@param  aParamListName
@param  aPopIapPref
*/	
EXPORT_C void CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadPopIAPAndAddL( 
									CT_MsgUtilsConfigFileParserUtility* aScriptFileParser,
									const TDesC& aParamListName, CImIAPPreferences& aPopIapPref)
	{
	TPtrC stringPtr; 	
	TPtrC baseString; 
	
	if( aScriptFileParser->GetFieldAsString(aParamListName, stringPtr) == KErrNone )
		{
		TInt start = 0;
		TInt end = stringPtr.Length();
		
		while (GetString(stringPtr, start, end, baseString))
			{
			ReadPopIAPAndAddFieldL(aScriptFileParser, baseString, aPopIapPref);
			}
		}		
		
	}
/**
ReadPopIAPAndAddField()
Modifies the CImIAPPreferences object.

@param  aScriptFileParser
CT_MsgUtilsConfigFileParserUtility object.

@param  aFieldName
@param  aPopIapPref
*/	
EXPORT_C void CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadPopIAPAndAddFieldL( 
									CT_MsgUtilsConfigFileParserUtility* aScriptFileParser, 
									const TDesC& aFieldName, CImIAPPreferences& aPopIapPref)
	{
	TPtrC stringPtr; 
	TInt intTmp;
	
	TImIAPChoice iapChoice;

	iapChoice.iIAP = 0;
	iapChoice.iDialogPref = ECommDbDialogPrefUnknown;
	
	TInt index = 0;
	TBool modified = EFalse;
		
	HBufC* buf = HBufC::NewLC(100);
			
	buf->Des() = aFieldName;
	buf->Des().Append(KPopIAPSuffix);
	
	if(aScriptFileParser->GetFieldAsInteger(buf->Des(), intTmp) == KErrNone)
		{
		iapChoice.iIAP = intTmp;
		modified = ETrue;
		}

	buf->Des() = aFieldName;
	buf->Des().Append(KPopCommDbDialogPref);
	if(aScriptFileParser->GetFieldAsString(buf->Des(), stringPtr) == KErrNone)
		{
		TCommDbDialogPref setCommDbDialogPref = CT_MsgUtilsEnumConverter::ConvertDesToTCommDbDialogPref(stringPtr);
		iapChoice.iDialogPref = setCommDbDialogPref;
		modified = ETrue;
		}

	buf->Des() = aFieldName;
	buf->Des().Append(KPopIAPIndex);
	if(aScriptFileParser->GetFieldAsInteger(buf->Des(), intTmp) == KErrNone)
		{
		index = intTmp;
		modified = ETrue;
		}
	
	if (modified)
		{
		aPopIapPref.AddIAPL(iapChoice ,index);
		}

	CleanupStack::PopAndDestroy(buf);		
	}

/**
ReadSmtpSettingsFromConfigurationFileL()
Modifies the POP related default setting and IAP objects.

@param  aSettingsFile
Configuration/setting file path. 

@param  aSmtpSet
@param  aSmtpIapPref
*/	
EXPORT_C void CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadSmtpSettingsFromConfigurationFileL(TDesC& aSettingsFile, 
										CImSmtpSettings& aSmtpSet, CImIAPPreferences& aSmtpIapPref,const TDesC&/* aPtrEmailAdd*/)
	{
	// Parses the setting file
	CT_MsgUtilsConfigFileParserUtility* scriptFileParser = CT_MsgUtilsConfigFileParserUtility::NewL(aSettingsFile);
	CleanupStack::PushL(scriptFileParser);	
	
	TPtrC8 string8Ptr; 
	TPtrC stringPtr = KNone(); 
	TInt intValue;

	if(scriptFileParser->GetFieldAsString(KCfgSmtpServerAddress, stringPtr) == KErrNone)
		{
		aSmtpSet.SetServerAddressL(stringPtr);
		}
	
	CT_MsgUtilsConfigFileMachineName* machineNameFile = CT_MsgUtilsConfigFileMachineName::NewLC(stringPtr);
	TPtrC emailAddr(machineNameFile->MachineNameEmail());
	TPtrC8 machineName(machineNameFile->MachineName());
	
	if(scriptFileParser->GetFieldAsString(KCfgSmtpEmailAddress, stringPtr) == KErrNone)
		{
		aSmtpSet.SetEmailAddressL(stringPtr);
		}
		
	else
		{
		aSmtpSet.SetEmailAddressL(emailAddr);
		}

	if(scriptFileParser->GetFieldAsString(KCfgSmtpBodyEncoding, stringPtr) == KErrNone)
		{
		TMsgOutboxBodyEncoding setBodyEncoding = CT_MsgUtilsEnumConverter::ConvertDesToTMsgOutboxBodyEncoding(stringPtr);
		aSmtpSet.SetBodyEncoding(setBodyEncoding);
		}
	
	if(scriptFileParser->GetFieldAsString(KCfgSmtpReceiptAddress, stringPtr) == KErrNone)
		{
		aSmtpSet.SetReceiptAddressL(stringPtr);
		}
		
	else
		{
		aSmtpSet.SetReceiptAddressL(emailAddr);
		}
		
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpRequestReceipts, intValue) == KErrNone)
		{		
		aSmtpSet.SetRequestReceipts((TBool) intValue);
		}
	
	if(scriptFileParser->GetFieldAsString(KCfgSmtpSendMessageOption, stringPtr) == KErrNone)
		{
		TImSMTPSendMessageOption setSendMessageOption = CT_MsgUtilsEnumConverter::ConvertDesToTImSMTPSendMessageOption(stringPtr);
		aSmtpSet.SetSendMessageOption(setSendMessageOption);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSetPort, intValue) == KErrNone)
		{
		aSmtpSet.SetPort(intValue);
		}
	
	if(scriptFileParser->GetFieldAsString(KCfgSmtpEmailAlias, stringPtr) == KErrNone)
		{
		aSmtpSet.SetEmailAliasL(stringPtr);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpDefaultMsgCharSet, intValue) == KErrNone)
		{
		TUid tmp = { intValue };
		aSmtpSet.SetDefaultMsgCharSet(tmp);
		}
	
	if(scriptFileParser->GetFieldAsString(KCfgSmtpSetReplyToAddress, stringPtr) == KErrNone)
		{
		aSmtpSet.SetReplyToAddressL(stringPtr);
		}
		
	else
		{
		aSmtpSet.SetReplyToAddressL(emailAddr);
		}
			
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSetAddVCardToEmail, intValue) == KErrNone)
		{		
		aSmtpSet.SetAddVCardToEmail((TBool) intValue);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSetAddSignatureToEmail, intValue) == KErrNone)
		{		
		aSmtpSet.SetAddSignatureToEmail((TBool) intValue);
		}
	
	if(scriptFileParser->GetFieldAsString(KCfgSmtpSetSendCopyToSelf, stringPtr) == KErrNone)
		{
		TImSMTPSendCopyToSelf setSMTPSendCopyToSelf = CT_MsgUtilsEnumConverter::ConvertDesToTImSMTPSendCopyToSelf(stringPtr);
		aSmtpSet.SetSendCopyToSelf(setSMTPSendCopyToSelf);
		}
	
	if(scriptFileParser->GetFieldAsString8(KCfgSmtpSetLoginName, string8Ptr) == KErrNone)
		{
		aSmtpSet.SetLoginNameL(string8Ptr);
		}
		
	else
		{
		aSmtpSet.SetLoginNameL(machineName);	
		}
			
	if(scriptFileParser->GetFieldAsString8(KCfgSmtpSetPassword, string8Ptr) == KErrNone)
		{
		aSmtpSet.SetPasswordL(string8Ptr);
		}

	else
		{
		aSmtpSet.SetPasswordL(machineName);	
		}
			
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSetSMTPAuth, intValue) == KErrNone)
		{		
		aSmtpSet.SetSMTPAuth((TBool) intValue);
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSetInboxLoginDetails, intValue) == KErrNone)
		{		
		aSmtpSet.SetInboxLoginDetails((TBool) intValue);
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSecureSockets, intValue) == KErrNone)
		{		
		aSmtpSet.SetSecureSockets((TBool) intValue);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSSLWrapper, intValue) == KErrNone)
		{		
		aSmtpSet.SetSSLWrapper((TBool) intValue);
		}
	
	if(scriptFileParser->GetFieldAsString(KCfgSmtpIapList, stringPtr) == KErrNone)
		{
		ReadSmtpIAPAndAddL(scriptFileParser, KCfgSmtpIapList, aSmtpIapPref);
		}
		
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSNAPId, intValue) == KErrNone)
		{		
		aSmtpIapPref.SetSNAPL(intValue);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpBearerMobility, intValue) == KErrNone)
		{		
		aSmtpSet.SetBearerMobility((TBool) intValue);
		}
		
	if(scriptFileParser->GetFieldAsString8(KCfgTlsSslDomainName, string8Ptr) == KErrNone)
		{
		aSmtpSet.SetTlsSslDomainL(string8Ptr);
		}
	
	CleanupStack::PopAndDestroy(2,scriptFileParser);
	
	}
	
/**
ReadSmtpIAPAndAdd()
Gets the base string for the IAP list.

@param  aScriptFileParser
CT_MsgUtilsConfigFileParserUtility object.

@param  aParamListName
@param  aSmtpIapPref
*/	
EXPORT_C void CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadSmtpIAPAndAddL(
						CT_MsgUtilsConfigFileParserUtility* aScriptFileParser, 
						const TDesC& aParamListName, CImIAPPreferences& aSmtpIapPref)
	{
	
	TPtrC stringPtr; 	
	TPtrC baseString; 
	
	if( aScriptFileParser->GetFieldAsString(aParamListName, stringPtr) == KErrNone)
		{
		TInt start = 0;
		TInt end = stringPtr.Length();
		
		while (GetString(stringPtr ,start, end, baseString))
			{
			ReadSmtpIAPAndAddFieldL(aScriptFileParser, baseString, aSmtpIapPref);
			}
		}
	}

/**
ReadSmtpIAPAndAddField()
Modifies the CImIAPPreferences object.

@param  aScriptFileParser
CT_MsgUtilsConfigFileParserUtility object.

@param  aFieldName
@param  aSmtpIapPref
*/	
EXPORT_C void CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadSmtpIAPAndAddFieldL(
							CT_MsgUtilsConfigFileParserUtility* aScriptFileParser , 
							const TDesC& aFieldName, CImIAPPreferences& aSmtpIapPref)
	{
	TPtrC stringPtr; 
	TInt intTmp;
	
	TImIAPChoice iapChoice;

	iapChoice.iIAP = 0;
	iapChoice.iDialogPref = ECommDbDialogPrefUnknown;
	
	TInt index = 0;
	TBool modified = EFalse;
		
	HBufC* buf = HBufC::NewLC(100);
			
	buf->Des() = aFieldName;
	buf->Des().Append(KSmtpIAPSuffix);
	
	if(aScriptFileParser->GetFieldAsInteger(buf->Des(), intTmp) == KErrNone)
		{
		iapChoice.iIAP = intTmp;
		modified = ETrue;
		}

	buf->Des() = aFieldName;
	buf->Des().Append(KSmtpCommDbDialogPref);
	if(aScriptFileParser->GetFieldAsString(buf->Des(), stringPtr) == KErrNone)
		{
		TCommDbDialogPref setCommDbDialogPref = CT_MsgUtilsEnumConverter::ConvertDesToTCommDbDialogPref(stringPtr);
		iapChoice.iDialogPref = setCommDbDialogPref;
		modified = ETrue;
		}

	buf->Des() = aFieldName;
	buf->Des().Append(KSmtpIAPIndex);
	if(aScriptFileParser->GetFieldAsInteger(buf->Des(), intTmp) == KErrNone)
		{
		index = intTmp;
		modified = ETrue;
		}
	
	if (modified)
		{
		aSmtpIapPref.AddIAPL(iapChoice, index);
		}
	CleanupStack::PopAndDestroy(buf);		
	}
	

/**
CompareIapPrefs()
Compares the IAP preference objects.

@param  aPrefs1
CImIAPPreferences object.

@param  aPrefs2
CImIAPPreferences object.

@return
If the objects are same returns ETrue
*/
EXPORT_C TBool CT_MsgUtilsReadEmailSettingsFromConfigFile::CompareIapPrefs(CImIAPPreferences& aPrefs1, CImIAPPreferences& aPrefs2)
	{
	// Compare version
	if (aPrefs1.Version() != aPrefs2.Version())
		{
		return EFalse;
		}	
	// Compare number of IAPs stored
	TInt numIAPs1 = aPrefs1.NumberOfIAPs();
	
	if (numIAPs1 != aPrefs2.NumberOfIAPs())
		{
		return EFalse;
		}
	// Compare individual IAPs
	for (TInt i = 0; i < numIAPs1; i++)
		{
		if (aPrefs1.IAPPreference(i).iIAP != aPrefs2.IAPPreference(i).iIAP ||
			aPrefs1.IAPPreference(i).iDialogPref != aPrefs2.IAPPreference(i).iDialogPref)
			{
			return EFalse;
			}
		}
	return ETrue;
	}

