// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// SetImap4ServiceParameters
// [Action Parameters]
// Session                   <input>: Reference to the session.
// ServiceId                 <input>: Value of the Imap4 service id.
// (ConfigIndexSettingsFile) <input>: Value of config index for settings file. Default is 0. 
// [Action Description]
// Sets parameters of Imap4 service.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::EditStoreL
// CImImap4Settings::Reset
// CImImap4Settings::SetServerAddressL
// CImImap4Settings::SetLoginNameL
// CImImap4Settings::SetPasswordL
// CImImap4Settings::SetAutoSendOnConnect
// CImImap4Settings::StoreL
// CMsvStore::CommitL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include  <cemailaccounts.h>

#include <msvapi.h>
#include <imapset.h>
#include <iapprefs.h>

#include "CMtfTestActionSetImap4ServiceParameters.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfConfigurationType.h"
#include "CMtfTestActionUtilsImapScripts.h"
#include "CMtfTestActionUtilsConfigFileMachineName.h"

_LIT(KServerName,	"ServerName");
_LIT(KEmailAddressExtension, "EmailAddressExtension");
_LIT(KLoginName,	"LoginName"); 
_LIT(KPassword,		"Password");  
_LIT(KFolderPath,	"FolderPath"); 
_LIT(KEmailDeleteFlag,	"DeleteEmailWhenDisconnected");
_LIT(KAutoSendFlag, "AutoSendFlag");
_LIT(KImapSearchString,"ImapSearchString");

CMtfTestAction* CMtfTestActionSetImap4ServiceParameters::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetImap4ServiceParameters* self = new (ELeave) CMtfTestActionSetImap4ServiceParameters(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSetImap4ServiceParameters::CMtfTestActionSetImap4ServiceParameters(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSetImap4ServiceParameters::~CMtfTestActionSetImap4ServiceParameters()
	{
	delete iParser;
	}



void CMtfTestActionSetImap4ServiceParameters::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetImap4ServiceParameters);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TInt paramConfigIndex = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2),0);

	CImImap4Settings* settings = new(ELeave) CImImap4Settings();
	CleanupStack::PushL(settings);
	TPtrC fileName = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfImap4Settings,paramConfigIndex);
	iParser = CMtfTestActionUtilsConfigFileParser::NewL(fileName);
	
	settings->Reset();

	TPtrC serverName;
	User::LeaveIfError(iParser->GetFieldAsString(KServerName,serverName));
	settings->SetServerAddressL(serverName);

	CMtfTestActionUtilsConfigFileMachineName* machineNameFile = NULL;

	TPtrC emailAddressExtension;
	if (iParser->GetFieldAsString(KEmailAddressExtension, emailAddressExtension) == KErrNone)
		{
		machineNameFile = CMtfTestActionUtilsConfigFileMachineName::NewLC(emailAddressExtension);
		}
	else
		{
		machineNameFile = CMtfTestActionUtilsConfigFileMachineName::NewLC(serverName);
		}

	TPtrC8 machineName(machineNameFile->MachineName());

	TPtrC8 loginName;
	if(iParser->GetFieldAsString8(KLoginName,loginName)==KErrNone)
		{
		settings->SetLoginNameL(loginName);	
		}
	else
		{
		settings->SetLoginNameL(machineName);	
		}

	TPtrC8 password;
	if(iParser->GetFieldAsString8(KPassword,password)==KErrNone)
		{
		settings->SetPasswordL(password);	
		}
	else
		{
		settings->SetPasswordL(machineName);	
		}

	TPtrC8 folderPath;
	TInt err = iParser->GetFieldAsString8(KFolderPath,folderPath);
	if (err == KErrNone)
		settings->SetFolderPathL(folderPath);
	
	TPtrC8 imapSearchString;
	err = iParser->GetFieldAsString8(KImapSearchString,imapSearchString);
	if (err == KErrNone)
		settings->SetSearchStringL(imapSearchString);

	TInt emailDeleteFlag;
	err = iParser->GetFieldAsInteger(KEmailDeleteFlag,emailDeleteFlag);
	if (err == KErrNone)
		settings->SetDeleteEmailsWhenDisconnecting(emailDeleteFlag);

	TInt autoSendFlag;
	err = iParser->GetFieldAsInteger(KAutoSendFlag,autoSendFlag);
	if (err == KErrNone)
		settings->SetAutoSendOnConnect(autoSendFlag);	
	
 	CEmailAccounts* accounts = CEmailAccounts::NewLC();
 	TImapAccount imapAccount;
 	accounts->GetImapAccountL(paramServiceId, imapAccount);
 	accounts->SaveImapSettingsL(imapAccount, *settings);
 	//Pop and destroy settings, machineNameFile and accounts
	CleanupStack::PopAndDestroy(3, settings);    
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSetImap4ServiceParameters);	
	
	TestCase().ActionCompletedL(*this);
	}

