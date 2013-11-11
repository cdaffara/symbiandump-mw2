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
//

/**
 @file
*/
 
 
#include "CMtfTestActionUtilsPopScripts.h"

#include <pop3set.h>
#include <smtpset.h>
#include <iapprefs.h>

#include "CMtfTestActionUtilsConfigFileParser.h"
#include "CMtfTestActionUtilsConfigFileMachineName.h"

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

// POP Settings 

_LIT(KCfgPopServerAddress, "PopServerAddress"); //*
_LIT(KEmailAddressExtension, "EmailAddressExtension");
_LIT(KCfgPopLoginName, "PopLoginName"); //*
_LIT(KCfgPopPassword, "PopPassword"); //*
_LIT(KCfgPopMaxEmailSize, "PopMaxEmailSize"); //*
_LIT(KCfgPopPort, "PopPort"); //*
_LIT(KCfgPopInboxSynchronisationLimit, "PopInboxSynchronisationLimit"); //*
_LIT(KCfgPopSecureSockets, "PopSecureSockets"); //*
_LIT(KCfgPopSSLWrapper, "PopSSLWrapper"); //*
_LIT(KCfgPopGetMailOptions, "PopGetMailOptions"); //*
_LIT(KCfgPopPopulationLimit, "PopPopulationLimit"); //*
_LIT(KCfgPopAutoSendOnConnect, "PopAutoSendOnConnect" ); //* 
_LIT(KCfgPopDeleteEmailsWhenDisconnecting, "PopDeleteEmailsWhenDisconnecting" ); //*
_LIT(KCfgPopAcknowledgeReceipts, "PopAcknowledgeReceipts" ); //*
_LIT(KCfgPopDisconnectedUserMode, "PopDisconnectedUserMode" ); //*
_LIT(KCfgPopAPOPEncryption, "PopAPOPEncryption" ); //*


_LIT(KCfgPopIapList , "PopIapList" );

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
	//???? Currently All in same file is this what is intended or are they intended to be in 
	// seperate files, which would require more index's ?

_LIT(KCfgSmtpIapList , "SmtpIapList" );


void CMtfTestActionUtilsPopScripts::ReadPopSettingsFromConfigurationFileL(CMtfTestCase& aTestCase, const TDesC& aSettingsFile, 
											CImPop3Settings& aPopSet, CImIAPPreferences& aPopIapPref, 
											CImSmtpSettings& aSmtpSet, CImIAPPreferences& aSmtpIapPref)
{
	CMtfTestActionUtilsConfigFileParser* scriptFileParser = CMtfTestActionUtilsConfigFileParser::NewL(aSettingsFile);
	CleanupStack::PushL(scriptFileParser);
	
	TPtrC8 string8Ptr; 
	TPtrC stringPtr; 
	TInt intValue;


	// POP3 settings
	if(scriptFileParser->GetFieldAsString(KCfgPopServerAddress, stringPtr) == KErrNone)
	{	
		aPopSet.SetServerAddressL(stringPtr);
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
		aPopSet.SetInboxSynchronisationLimit(intValue); // ??? should this be SetInboxSynchronisation?
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
		TPop3GetMailOptions options = ObtainValueParameterL<TPop3GetMailOptions>(aTestCase, stringPtr);
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
		
	scriptFileParser->GetFieldAsString(KCfgPopIapList, stringPtr);
	
	ReadIAPAndAddL( aTestCase, scriptFileParser, KCfgPopIapList, aPopIapPref );
	
	
	// SMTP settings
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
	

	
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpRequestReceipts, intValue) == KErrNone)
	{		
		aSmtpSet.SetRequestReceipts( (TBool) intValue );
	}
	
	if(scriptFileParser->GetFieldAsString(KCfgSmtpSendMessageOption, stringPtr) == KErrNone)
	{
		TImSMTPSendMessageOption setSendMessageOption = ObtainValueParameterL<TImSMTPSendMessageOption>(aTestCase, stringPtr);
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
		aSmtpSet.SetDefaultMsgCharSet( tmp );
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
		aSmtpSet.SetAddVCardToEmail( (TBool) intValue );
	}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSetAddSignatureToEmail, intValue) == KErrNone)
	{		
		aSmtpSet.SetAddSignatureToEmail( (TBool) intValue );
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
		
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSetSMTPAuth, intValue) == KErrNone)
	{		
		aSmtpSet.SetSMTPAuth( (TBool) intValue );
	}

	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSetInboxLoginDetails, intValue) == KErrNone)
	{		
		aSmtpSet.SetInboxLoginDetails( (TBool) intValue );
	}

	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSecureSockets, intValue) == KErrNone)
	{		
		aSmtpSet.SetSecureSockets( (TBool) intValue );
	}
	
	if(scriptFileParser->GetFieldAsInteger(KCfgSmtpSSLWrapper, intValue) == KErrNone)
	{		
		aSmtpSet.SetSSLWrapper( (TBool) intValue );
	}
	
	ReadIAPAndAddL( aTestCase, scriptFileParser, KCfgSmtpIapList, aSmtpIapPref );
	
	//Pop and destroy scriptfileParser and machineNameFile	
	CleanupStack::PopAndDestroy(2,scriptFileParser);	
}

void CMtfTestActionUtilsPopScripts::ReadIAPAndAddL( CMtfTestCase& aTestCase, 
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

TBool CMtfTestActionUtilsPopScripts::GetString( const TDesC& aStringPtr , TInt& aStart, TInt aEnd , TPtrC& aBaseString )
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


void CMtfTestActionUtilsPopScripts::_ReadIAPAndAddL( CMtfTestCase& aTestCase, 
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
	buf->Des().Append( _L("_PopIAP") );
	
			
	if(aScriptFileParser->GetFieldAsInteger(buf->Des(), intTmp) == KErrNone)
	{
		iapChoice.iIAP = intTmp;
		modified=ETrue;
	}
	

	buf->Des() = aFieldName;
	buf->Des().Append( _L("_PopCommDbDialogPref") );
	if(aScriptFileParser->GetFieldAsString(buf->Des(), stringPtr) == KErrNone)
	{
		TCommDbDialogPref setCommDbDialogPref = ObtainValueParameterL<TCommDbDialogPref>(aTestCase, stringPtr);
		iapChoice.iDialogPref= setCommDbDialogPref;
		modified=ETrue;
	}

	
	buf->Des() = aFieldName;
	buf->Des().Append( _L("_PopIAPIndex") );
	if(aScriptFileParser->GetFieldAsInteger(buf->Des(), intTmp) == KErrNone)
	{
		index = intTmp;
		modified=ETrue;
	}
	
	if (modified)
	{	// Note only want to set the values if they are specified. Otherwise leave well alone.
		// Really should specify iapChoice.iIAP and iapChoice.iDialogPref at the same time.
		aIapPref.AddIAPL(iapChoice ,index );
	}

	CleanupStack::PopAndDestroy(buf);
}

