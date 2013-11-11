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
// This test action was created from CMtfTestActionCreateSmtpMessageVariable.  It adds a fifth parameter
// which is a length so that emails with body text of any size can be sent (in multiples of 1024 characters).
// The original has not been changed for this due to the file being independently modified - and those
// modifications include a fifth parameter - which is different to this fifth parameter.
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// CreateSmtpMessageFromEmailFile
// [Action Parameters]
// Session                <input>: Reference to the session.
// ServiceId              <input>: Value of the SMTP service id.
// FolderId               <input>: Value of the local folder id where the message will be created.
// (ConfigIndexEmailFile) <input>: Value of config index for email file. Default is 0. 
// MessageId             <output>: Value of the created SMTP message id.
// KBExtra                <input>: Number of lines of 1024 characters to add to body text
// OverrideAddress <input>: Override the To: address in the email to the current SMTP settings
// [Action Description]
// Creates a SMTP message on one of the local folders.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::Entry
// TMsvId::Id
// CImCltRecvConvert::SetMsvId
// CImCltRecvConvert::ResetL
// CImCltRecvConvert::MessageCompleteL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionCreateSmtpMessageVariable.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>
#include <miutset.h>
#ifdef __MESSAGING_API_V2__
#include  <cemailaccounts.h>
#endif // __MESSAGING_API_V2__
#include "ImCltCvRecv.h"

// Maximum length of a line read from Email file
const TInt	 KMaxLengthOfLine = 1024;

CMtfTestAction* CMtfTestActionCreateSmtpMessageVariable::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateSmtpMessageVariable* self = new (ELeave) CMtfTestActionCreateSmtpMessageVariable(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateSmtpMessageVariable::CMtfTestActionCreateSmtpMessageVariable(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateSmtpMessageVariable::~CMtfTestActionCreateSmtpMessageVariable()
	{
	}


void CMtfTestActionCreateSmtpMessageVariable::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateSmtpMessageVariable);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TMsvId paramFolderId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(2));
	TInt paramConfigIndexMailFile = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(3),0);
	TInt numKilobytesExtra = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(5));
	
	
	TInt overrideAddress=1;
	if(ActionParameters().Count()>6)
		{
		overrideAddress=ObtainValueParameterL<TInt>(TestCase(),
								 ActionParameters().Parameter(6));		
		}
	
	TPtrC fileName = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfEmailFile,paramConfigIndexMailFile);

	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramFolderId,TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	entry->SetEntryL(paramServiceId);

	RFs fs;
	fs.Connect();

	CImCltRecvConvert* recvConvert = CImCltRecvConvert::NewLC(fs,entry,entry->Entry().iMtm,paramServiceId);
	recvConvert->SetMsvId(paramFolderId);

	RFile file;
	User::LeaveIfError(file.Open(fs,fileName,EFileRead));

	TestCase().INFO_PRINTF2(_L("Email Data file: %S"), &fileName);

	TBuf8<KMaxLengthOfLine> line;
	TBuf8<1> aChar;
	TBool finished = FALSE;

	recvConvert->ResetL();
	
	do {
		line.FillZ();
		line.SetLength(0);
		// Read one line from email file
		do {
			file.Read(aChar, 1);
			if(aChar.Length())
				{
				line.Append(aChar);
				}				
			else
				{
				finished = TRUE;
				}				
			} while(aChar.Length() && aChar[0] != 0x0A);
			
			if(!line.Length())
				{
				break;
				}
			// Parse the line for the fields of the email and store them				
			recvConvert->ParseNextFieldL(line);
		} while(!finished);

	// For a large email, pad the email with extra characters
	if(numKilobytesExtra > 0)
		{
		const TChar padChar = 'L';
		line.Fill(padChar, 1024);
		for(TInt i = 0; i < numKilobytesExtra; i++)
			{
				recvConvert->ParseNextFieldL(line);
			}
		}

	TMsvId paramMessageId = recvConvert->MessageCompleteL();
	if(paramMessageId == 0)
		{
		User::Leave(KErrUnknown);
		}
	

	file.Close();
	CleanupStack::PopAndDestroy(recvConvert);
	
	//*****
	if(overrideAddress)
		{
#ifndef __MESSAGING_API_V2__		
		entry->SetEntryL(paramServiceId);
		CMsvStore* store=entry->ReadStoreL();
		CleanupStack::PushL(store);
#endif		
		CImSmtpSettings* settings= new(ELeave) CImSmtpSettings();
		CleanupStack::PushL(settings);
#ifdef __MESSAGING_API_V2__
 		CEmailAccounts* accounts = CEmailAccounts::NewLC();
 		TSmtpAccount smtpAccount;
 		accounts->GetSmtpAccountL(paramServiceId, smtpAccount);
		accounts->LoadSmtpSettingsL(smtpAccount, *settings);
		CleanupStack::PopAndDestroy();
#else		
		settings->RestoreL(*store);
		CleanupStack::PopAndDestroy();
#endif		
		HBufC* emailAddress=settings->EmailAddress().AllocL();
		CleanupStack::PopAndDestroy();
		CleanupStack::PushL(emailAddress);
		entry->SetEntryL(paramMessageId);
#ifdef __MESSAGING_API_V2__
		CMsvStore* store=entry->EditStoreL();
#else	 	
		
#endif
		CleanupStack::PushL(store);
		
		CImHeader* header= CImHeader::NewLC();
		header->RestoreL(*store);
		
		header->ToRecipients().Reset();
		header->ToRecipients().AppendL(*emailAddress);
		
		header->StoreL(*store);
		store->CommitL();
		
		CleanupStack::PopAndDestroy(3,emailAddress);
		}
	//*****
	
	CleanupStack::PopAndDestroy(entry);
	
	TestCase().INFO_PRINTF2(_L("Created a email from file %S"),&fileName);

	StoreParameterL<TMsvId>(TestCase(),paramMessageId,ActionParameters().Parameter(4));
	fs.Close();

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateSmtpMessageVariable);
	TestCase().ActionCompletedL(*this);
	}

