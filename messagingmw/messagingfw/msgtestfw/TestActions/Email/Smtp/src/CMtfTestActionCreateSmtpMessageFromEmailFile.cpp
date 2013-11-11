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
// CreateSmtpMessageFromEmailFile
// [Action Parameters]
// CMsvSession			Session		<input>: Reference to the session.
// TMsvId				ServiceId   <input>: Value of the SMTP service id.
// TMsvId				FolderId    <input>: Value of the local folder id where the message will be created.
// TInt		 (ConfigIndexEmailFile) <input>: Value of config index for email file. Default is 0. 
// TMsvID	MessageId   <output-initiation>: Value of the created SMTP message id.
// TInt					OverrideAddress <input>: Override the To: address in the email to the current SMTP settings
// [Action Description]
// Creates a SMTP message on one of the local folders.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::Entry
// TMsvId::Id
// __ACTION_INFO_END__
// 
//

/**
 @file
 @internalTechnology 
*/

// EPOC include
#include <msvapi.h>
#include <miutset.h>
#include <smtpset.h>
#include <miuthdr.h>
#ifdef __MESSAGING_API_V2__
#include  <cemailaccounts.h>
#endif // __MESSAGING_API_V2__
	

// User include
#include "CMtfTestActionCreateSmtpMessageFromEmailFile.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "ImCltCvRecv.h"


// Maximum lenght of a line read from Email file
const TInt	 KMaxLenghtOfLine = 1024;


/**
  NewL()
  Constructs a CMtfTestActionCreateSmtpMessageFromEmailFile object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionCreateSmtpMessageFromEmailFile
  @pre    None
  @post   CMtfTestActionCreateSmtpMessageFromEmailFile object is created
*/
CMtfTestAction* CMtfTestActionCreateSmtpMessageFromEmailFile::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateSmtpMessageFromEmailFile* self = new (ELeave) CMtfTestActionCreateSmtpMessageFromEmailFile(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionCreateSmtpMessageFromEmailFile constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 	
CMtfTestActionCreateSmtpMessageFromEmailFile::CMtfTestActionCreateSmtpMessageFromEmailFile(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/** Destructor */
CMtfTestActionCreateSmtpMessageFromEmailFile::~CMtfTestActionCreateSmtpMessageFromEmailFile()
	{
	}

/**
  ExecuteActionL
  Parse the Email file and creates an email in the specified local folder.  Creates
  simple emails, emails with HTML, attachments, multipart and embedded emails.
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionCreateSmtpMessageFromEmailFile::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateSmtpMessageFromEmailFile);
	CMsvSession* paramSession	= ObtainParameterReferenceL<CMsvSession>(TestCase(),
									ActionParameters().Parameter(0));
								 
	TMsvId paramServiceId 		= ObtainValueParameterL<TMsvId>(TestCase(),
									ActionParameters().Parameter(1));
								 
	TMsvId paramFolderId 		= ObtainValueParameterL<TMsvId>(TestCase(),
								    ActionParameters().Parameter(2));
								 
	TInt paramConfigIndexMailFile = ObtainValueParameterL<TInt>(TestCase(),
								 ActionParameters().Parameter(3),0);
	
	TInt overrideAddress=0;
	if(ActionParameters().Count()>5)
		{
		overrideAddress=ObtainValueParameterL<TInt>(TestCase(),
								 ActionParameters().Parameter(5));		
		}

	TPtrC fileName = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfEmailFile,
														paramConfigIndexMailFile);

	/************************************************************************************
	Get the name of the Email file save at the Configuration file index, open the  file, 
	read one line of text from the file, parse the line and set the fields of the email 
	message based on the contents of the parsed line.
	************************************************************************************/
	
	// Set the context ot the folder in which emails have to be created
	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramFolderId,TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	entry->SetEntryL(paramFolderId);

	RFs fs;
	fs.Connect();

	CImCltRecvConvert* recvConvert = CImCltRecvConvert::NewLC(fs,entry,KUidMsgTypeSMTP,
															   paramServiceId);
	recvConvert->SetMsvId(paramFolderId);

	// Open the Email file
	RFile file;
	User::LeaveIfError(file.Open(fs,fileName,EFileRead));

	TestCase().INFO_PRINTF2(_L("Email Data file: %S"), &fileName);
	
	TBuf8<KMaxLenghtOfLine> line;
	TBuf8<1> aChar;				// To read one character from the file
	TBool finished = FALSE;

	recvConvert->ResetL();

	// Parse each line from the 

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


	/****************************************************************************
	 Complete the message creation by setting all the required fields and 
	 storing the email on to the message store.
	 The created email's ID is returned by MessageCompelteL function
	****************************************************************************/
	TMsvId paramMessageId;
	paramMessageId = recvConvert->MessageCompleteL();

	if(paramMessageId == 0)
		{
		User::Leave(KErrUnknown);
		}

	file.Close();
	CleanupStack::PopAndDestroy(recvConvert);
	
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
		
		store=entry->EditStoreL();
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
	
	CleanupStack::PopAndDestroy(entry);

	TestCase().INFO_PRINTF2(_L("Created a email from file %S"),&fileName);

	StoreParameterL<TMsvId>(TestCase(),paramMessageId,ActionParameters().Parameter(4));
	fs.Close();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateSmtpMessageFromEmailFile);
	TestCase().ActionCompletedL(*this);
	}

