// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// ???01 : Start
// Move to own file something like 
// "\messaging\terstframework\testactionutils\src\CmtfTestActionUtilsImapScripts.cpp"
// 
//

/**
 @file
*/

#include "CMtfTestActionUtilsImapScripts.h"

#include <imapset.h>
#include <smtpset.h>
#include <iapprefs.h>

#include "CMtfTestActionUtilsConfigFileParser.h"
#include "CMtfTestActionUtilsConfigFileMachineName.h"

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

// Imap Settings 

_LIT(KCfgImapServerAddress, "ImapServerAddress"); //*
_LIT(KEmailAddressExtension, "EmailAddressExtension");
_LIT(KCfgImapLoginName, "ImapLoginName"); //*
_LIT(KCfgImapPassword, "ImapPassword"); //*
_LIT(KCfgImapPort, "ImapPort"); //*
_LIT(KCfgImapSecureSockets, "ImapSecureSockets"); //*
_LIT(KCfgImapSSLWrapper, "ImapSSLWrapper"); //*
//_LIT(KCfgImapVersion, "ImapVersion"); // Does not actually exists is hardcoded. 
_LIT(KCfgImapFolderPath, "ImapFolderPath");   //* errors question. 
_LIT(KCfgImapPathSeperator, "ImapPathSeperator"); //*
_LIT(KCfgImapMaxEmailSize, "ImapMaxEmailSize"); //*
_LIT(KCfgImapGetMailOptions, "ImapGetMailOptions"); //*
_LIT(KCfgImapInboxSynchronisationLimit, "ImapInboxSynchronisationLimit"); //*
_LIT(KCfgImapMailboxSynchronisationLimit, "ImapMailSynchronisationLimit"); //*

_LIT(KCfgImapAutoSendOnConnect, "ImapAutoSendOnConnect" ); //* 
_LIT(KCfgImapDeleteEmailsWhenDisconnecting, "ImapDeleteEmailsWhenDisconnecting" ); //*
_LIT(KCfgImapAcknowledgeReceipts, "ImapAcknowledgeReceipts" ); //*
_LIT(KCfgImapDisconnectedUserMode, "ImapDisconnectedUserMode" ); //*
_LIT(KCfgImapSynchronise, "ImapSynchronise" ); //* Constants Added
_LIT(KCfgImapSubscribe, "ImapSubscribe" ); //* Constants Added.
_LIT(KCfgImapUpdatingSeenFlags, "ImapUpdatingSeenFlags" ); //*
_LIT(KCfgImapSyncRate, "ImapSyncRate" ); //*
_LIT(KCfgImapFetchSize, "ImapFetchSize" ); //*
_LIT(KCfgImapIdle, "ImapIdle" ); //*
_LIT(KCfgImapIdleTimeout, "ImapIdleTimeout" ); //*
_LIT(KCfgImapPartialMailOptions, "ImapPartialMailOptions" ); //*
_LIT(KCfgImapBodyTextSizeLimit, "ImapBodyTextSizeLimit" ); //*
_LIT(KCfgImapAttachmentSizeLimit, "ImapAttachmentSizeLimit" ); //*


_LIT(KCfgImapIapList , "ImapIapList" );


// SMTP settings


_LIT(KCfgSmtpServerAddress, "SmtpServerAddress");  //*
_LIT(KCfgSmtpEmailAddress, "SmtpEmailAddress");  //*
_LIT(KCfgSmtpBodyEncoding, "SmtpBodyEncoding");  //*
_LIT(KCfgSmtpReceiptAddress, "SmtpReceiptAddress");  //*
_LIT(KCfgSmtpRequestReceipts, "SmtpRequestReceipts");  //*
_LIT(KCfgSmtpSendMessageOption, "SmtpSendMessageOption");  //* Constants Added
_LIT(KCfgSmtpSetPort, "SmtpSetPort");  //*
_LIT(KCfgSmtpEmailAlias, "SmtpEmailAlias");  //*
_LIT(KCfgSmtpDefaultMsgCharSet, "SmtpMsgCharSet");  //*
_LIT(KCfgSmtpSetReplyToAddress, "SmtpSetReplyToAddress");  //*
_LIT(KCfgSmtpSetAddVCardToEmail, "SmtpSetAddVCardToEmail"); //*
_LIT(KCfgSmtpSetAddSignatureToEmail, "SmtpSetAddSignatureToEmail");  //*
_LIT(KCfgSmtpSetSendCopyToSelf, "SmtpSetSendCopyToSelf");  //* 
_LIT(KCfgSmtpSetLoginName, "SmtpSetLoginName");  //*
_LIT(KCfgSmtpSetPassword, "SmtpSetPassword");  //*
_LIT(KCfgSmtpSetSMTPAuth, "SmtpSetSMTPAuth");  //*
_LIT(KCfgSmtpSetInboxLoginDetails, "SmtpSetInboxLoginDetails");  //*


_LIT(KCfgSmtpSecureSockets, "SmtpSetSecureSockets");  //*
_LIT(KCfgSmtpSSLWrapper, "SmtpSetSSLWrapper");  //*

_LIT(KCfgSmtpIapList , "SmtpIapList" );


     
void CMtfTestActionUtilsImapScripts::ReadIAPAndAddL( CMtfTestCase& aTestCase, 
	CMtfTestActionUtilsConfigFileParser* aScriptFileParser , 
	const TDesC& aParamListName , CImIAPPreferences& aIapPref )
	{
	TPtrC stringPtr; 	
	TPtrC baseString; 
	
	if( aScriptFileParser->GetFieldAsString(aParamListName, stringPtr ) == KErrNone )
		{
		
		TInt start=0;
		TInt end=stringPtr.Length();
		
		while ( GetString( stringPtr , start, end , baseString ) )
			{
			_ReadIAPAndAddL( aTestCase, aScriptFileParser, baseString, aIapPref );
			}
		
		}		

	}




TBool CMtfTestActionUtilsImapScripts::GetString( const TDesC& aStringPtr , TInt& aStart, TInt aEnd , TPtrC& aBaseString )
	{
	TBool ret=EFalse;
	
	if (aStart==aEnd)
		{
		return EFalse;				
		}
	
	TInt currPos=aStart;
	

	// strip out any starting spaces.
	while (currPos<aEnd )
		{
		if ( aStringPtr[currPos] == ' ' || aStringPtr[currPos] == '\t' )
			{			
			currPos++;	
			}
		else
			{			
			break;	
			}
		}
	TInt startTmp=currPos;
	
	while (currPos<aEnd )
		{
		if ( aStringPtr[currPos] == ' ' || aStringPtr[currPos] == '\t' )
			{			
			break;
			}
		else
			{
			currPos++;
			ret=ETrue;
			}
		}
	
	if (ret)
		{	// We have a string.
		aBaseString.Set( aStringPtr.Mid( startTmp, currPos-startTmp) ); 
		}
		
	aStart=currPos;
	
	return ret;
	}


void CMtfTestActionUtilsImapScripts::_ReadIAPAndAddL( CMtfTestCase& aTestCase, 
	CMtfTestActionUtilsConfigFileParser* aScriptFileParser , 
	const TDesC& aFieldName , CImIAPPreferences& aIapPref )
	{
	
	TPtrC stringPtr; 
	TInt intTmp;
	
	TImIAPChoice iapChoice;
	iapChoice.iIAP = 0;
	iapChoice.iDialogPref = ECommDbDialogPrefUnknown;
	
	TInt index=0;
	TBool modified=EFalse;
		
	HBufC* buf = HBufC::NewLC(100);
			
	buf->Des() = aFieldName;
	buf->Des().Append( _L("_ImapIAP") );
	
			
	if(aScriptFileParser->GetFieldAsInteger(buf->Des(), intTmp) == KErrNone)
		{
		iapChoice.iIAP = intTmp;
		modified=ETrue;
		}
	

	buf->Des() = aFieldName;
	buf->Des().Append( _L("_ImapCommDbDialogPref") );
	if(aScriptFileParser->GetFieldAsString(buf->Des(), stringPtr) == KErrNone)
		{
		TCommDbDialogPref setCommDbDialogPref = ObtainValueParameterL<TCommDbDialogPref>(aTestCase, stringPtr);
		iapChoice.iDialogPref= setCommDbDialogPref;
		modified=ETrue;
		}

	
	buf->Des() = aFieldName;
	buf->Des().Append( _L("_ImapIAPIndex") );
	if(aScriptFileParser->GetFieldAsInteger(buf->Des(), intTmp) == KErrNone)
		{
		index = intTmp;
		modified=ETrue;
		}
	
	if (modified)
		{ // Note only want to set the values if they are specified. Otherwise leave well alone.
		  // Really should specify iapChoice.iIAP and iapChoice.iDialogPref at the same time.
		aIapPref.AddIAPL(iapChoice ,index );
		}

	CleanupStack::PopAndDestroy(buf);

	}


void CMtfTestActionUtilsImapScripts::ReadImapSettingsFromConfigurationFileL(CMtfTestCase& aTestCase, const TDesC& aSettingsFile, 
											CImImap4Settings& aImapSet, CImIAPPreferences& aImapIapPref, 
											CImSmtpSettings& aSmtpSet, CImIAPPreferences& aSmtpIapPref)
	{
	CMtfTestActionUtilsConfigFileParser* scriptFileParser = CMtfTestActionUtilsConfigFileParser::NewL(aSettingsFile);
	CleanupStack::PushL(scriptFileParser);
	
	TPtrC8 string8Ptr; 
	TPtrC stringPtr; 
	TInt intTmp;
	
	
	// Read in CImImap4Settings.

	if(scriptFileParser->GetFieldAsString(KCfgImapServerAddress, stringPtr) == KErrNone)
		{
		aImapSet.SetServerAddressL(stringPtr);
		}

	CMtfTestActionUtilsConfigFileMachineName* machineNameFile = NULL;

	TPtrC emailAddressExtension;
	if (scriptFileParser->GetFieldAsString(KEmailAddressExtension, emailAddressExtension) == KErrNone)
		{
		machineNameFile = CMtfTestActionUtilsConfigFileMachineName::NewLC(emailAddressExtension);
		}
	else
		{
		machineNameFile = CMtfTestActionUtilsConfigFileMachineName::NewLC(stringPtr);
		}

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


	// Note  CImImap4Settings::SetVersion() although in the documentation does not actually exist.
	

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
			else
				{	// Path seperator must be a single char.
				aTestCase.ERR_PRINTF1(_L("Path seperator must be a single char"));
				aTestCase.SetTestStepResult(EFail);
				}
		}


	if(scriptFileParser->GetFieldAsInteger(KCfgImapMaxEmailSize, intTmp) == KErrNone)
		{
		aImapSet.SetMaxEmailSize(intTmp);
		}


	//???? Check input of constants works.	
	if(scriptFileParser->GetFieldAsString(KCfgImapGetMailOptions, stringPtr) == KErrNone)
		{
		TImap4GetMailOptions setGetMailOptions = ObtainValueParameterL<TImap4GetMailOptions>(aTestCase, stringPtr);
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
		aImapSet.SetAutoSendOnConnect( (TBool) intTmp );
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgImapDeleteEmailsWhenDisconnecting, intTmp) == KErrNone)
		{		
		aImapSet.SetDeleteEmailsWhenDisconnecting( (TBool) intTmp );
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgImapAcknowledgeReceipts, intTmp) == KErrNone)
		{		
		aImapSet.SetAcknowledgeReceipts( (TBool) intTmp );
		}



	if(scriptFileParser->GetFieldAsInteger(KCfgImapDisconnectedUserMode, intTmp) == KErrNone)
		{		
		aImapSet.SetDisconnectedUserMode( (TBool) intTmp );
		}




	if(scriptFileParser->GetFieldAsString(KCfgImapSynchronise, stringPtr) == KErrNone)
		{
		TFolderSyncType setFolderSyncType = ObtainValueParameterL<TFolderSyncType>(aTestCase, stringPtr);
		aImapSet.SetSynchronise(setFolderSyncType);
		}


	if(scriptFileParser->GetFieldAsString(KCfgImapSubscribe, stringPtr) == KErrNone)
		{
		TFolderSubscribeType setFolderSubscribeType = ObtainValueParameterL<TFolderSubscribeType>(aTestCase, stringPtr);
		aImapSet.SetSubscribe(setFolderSubscribeType);
		}


	if(scriptFileParser->GetFieldAsInteger(KCfgImapUpdatingSeenFlags, intTmp) == KErrNone)
		{		
		aImapSet.SetUpdatingSeenFlags( (TBool) intTmp );
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
		TImImap4PartialMailOptions setImap4PartialMailOptions = ObtainValueParameterL<TImImap4PartialMailOptions>(aTestCase, stringPtr);
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


	scriptFileParser->GetFieldAsString(KCfgImapIapList, stringPtr);
	
	ReadIAPAndAddL( aTestCase, scriptFileParser, KCfgImapIapList, aImapIapPref );
	
	// Read in CImSmtpSettings.

	if(scriptFileParser->GetFieldAsString(KCfgSmtpServerAddress, stringPtr) == KErrNone)
		{
		aSmtpSet.SetServerAddressL(stringPtr);
		}

	TPtrC emailAddr(machineNameFile->MachineNameEmail());
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
		TMsgOutboxBodyEncoding setGetMailOptions = ObtainValueParameterL<TMsgOutboxBodyEncoding>(aTestCase, stringPtr);
		aSmtpSet.SetBodyEncoding(setGetMailOptions);
		}
	
	if(scriptFileParser->GetFieldAsString(KCfgSmtpReceiptAddress, stringPtr) == KErrNone)
		{
		aSmtpSet.SetReceiptAddressL(stringPtr);
		}
	else
		{
		aSmtpSet.SetReceiptAddressL(emailAddr);
		}
		
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpRequestReceipts, intTmp) == KErrNone)
		{		
		aSmtpSet.SetRequestReceipts( (TBool) intTmp );
		}

	
	if(scriptFileParser->GetFieldAsString(KCfgSmtpSendMessageOption, stringPtr) == KErrNone)
		{
		TImSMTPSendMessageOption setSendMessageOption = ObtainValueParameterL<TImSMTPSendMessageOption>(aTestCase, stringPtr);
		aSmtpSet.SetSendMessageOption(setSendMessageOption);
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSetPort, intTmp) == KErrNone)
		{
		aSmtpSet.SetPort(intTmp);
		}
	
	
	if(scriptFileParser->GetFieldAsString(KCfgSmtpEmailAlias, stringPtr) == KErrNone)
		{
		aSmtpSet.SetEmailAliasL(stringPtr);
		}
	
	
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpDefaultMsgCharSet, intTmp) == KErrNone)
		{
		TUid tmp = { intTmp };
		aSmtpSet.SetDefaultMsgCharSet( tmp );
		}
	
	// CImSmtpSettings::SetVersion() does not exist.
	
	
	if(scriptFileParser->GetFieldAsString(KCfgSmtpSetReplyToAddress, stringPtr) == KErrNone)
		{
		aSmtpSet.SetReplyToAddressL(stringPtr);
		}
	else
		{
		aSmtpSet.SetReplyToAddressL(emailAddr);
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSetAddVCardToEmail, intTmp) == KErrNone)
		{		
		aSmtpSet.SetAddVCardToEmail( (TBool) intTmp );
		}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSetAddSignatureToEmail, intTmp) == KErrNone)
		{		
		aSmtpSet.SetAddSignatureToEmail( (TBool) intTmp );
		}
	

	if(scriptFileParser->GetFieldAsString(KCfgSmtpSetSendCopyToSelf, stringPtr) == KErrNone)
		{
		TImSMTPSendCopyToSelf setSMTPSendCopyToSelf = ObtainValueParameterL<TImSMTPSendCopyToSelf>(aTestCase, stringPtr);
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
	
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSetSMTPAuth, intTmp) == KErrNone)
		{		
		aSmtpSet.SetSMTPAuth( (TBool) intTmp );
		}


	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSetInboxLoginDetails, intTmp) == KErrNone)
		{		
		aSmtpSet.SetInboxLoginDetails( (TBool) intTmp );
		}

	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSecureSockets, intTmp) == KErrNone)
		{		
		aSmtpSet.SetSecureSockets( (TBool) intTmp );
		}
	
		
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSSLWrapper, intTmp) == KErrNone)
		{		
		aSmtpSet.SetSSLWrapper( (TBool) intTmp );
		}
	
	ReadIAPAndAddL( aTestCase, scriptFileParser, KCfgSmtpIapList, aSmtpIapPref );
	
	//Pop and destroy scriptfileParser and machineNameFile
	CleanupStack::PopAndDestroy(2,scriptFileParser);
	}



TBool CMtfTestActionUtilsImapScripts::CompareIapPrefs(CImIAPPreferences& prefs1, CImIAPPreferences& prefs2)
	{
	// Compare version
	if (prefs1.Version() != prefs2.Version())
		{
		return EFalse;
		}
	
	// Compare number of IAPs stored
	TInt numIAPs1 = prefs1.NumberOfIAPs();
	
	if (numIAPs1 != prefs2.NumberOfIAPs())
		{
		return EFalse;
		}
	
	// Compare individual IAPs
	for (TInt i = 0; i < numIAPs1; i++)
		{
		if (prefs1.IAPPreference(i).iIAP != prefs2.IAPPreference(i).iIAP ||
			prefs1.IAPPreference(i).iDialogPref != prefs2.IAPPreference(i).iDialogPref)
			{
			return EFalse;
			}
		}
	
	return ETrue;

	}

