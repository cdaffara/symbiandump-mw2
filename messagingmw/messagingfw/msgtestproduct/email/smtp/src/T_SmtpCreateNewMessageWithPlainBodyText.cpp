// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file 
// [TestStep Name]
// SmtpCreateNewMessageWithPlainBodyText
// [Paramaters]
// Subject			:	Subject line of the message.
// FolderName		:	Destination local folder where message is to be created.
// RecipientsAddress:	Recepient's address.
// EmailFileName	:	Plain text file name from which body text part would be populated. 
// Creates a new message entry with plain body text. For creating the message entry it uses
// CImEmailOperation::CreateNewL() static metod. After creating the message  entry
// it populates the body part with the contents of a plain text file.
// [APIs Used]
// CImPlainBodyText::CommitL
// CImPlainBodyText::StoreChunkL
// CImEmailOperation::CreateNewL
// CImEmailMessage::OpenPlainBodyTextForWriteL
// 
//

// User includes 
#include "t_smtpcreatenewmessagewithplainbodytext.h"
#include <t_utilscentralrepository.h>
#include <t_utilsenumconverter.h>
#include <t_utils.h>

// Epoc includes 
#include <miutmsg.h>
#include <cimplainbodytext.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "cimmessagepart.h"
#endif

// Literals Used 
_LIT(KSubject, "Subject");
_LIT(KFolderName, "FolderName");
_LIT(KEmailFileName, "EmailFileName");
_LIT(KRecipientsAddress, "RecipientsAddress");

const TInt KChunkSize = 1024;

/**
  Function 		: CT_SmtpCreateNewMessageWithPlainBodyText
  Description 	: Constructor
  @param 		: aSharedDataSMTP		Reference to CT_MsgSharedDataSmtp
  @return 		: N/A
*/
CT_SmtpCreateNewMessageWithPlainBodyText::CT_SmtpCreateNewMessageWithPlainBodyText(CT_MsgSharedDataSmtp& aSharedDataSMTP)
:	CT_MsgAsyncStepSMTP(aSharedDataSMTP)
,	iOperation(NULL)
	{
	SetTestStepName(KSmtpCreateNewMessageWithPlainBodyText);
	}


/**
  Function 		: NewL
  Description 	: Creates an object of CT_SmtpCreateNewMessageWithPlainBodyText 				 
  @return 		: N/A
*/
CT_SmtpCreateNewMessageWithPlainBodyText* CT_SmtpCreateNewMessageWithPlainBodyText::NewL(CT_MsgSharedDataSmtp& aSharedDataSMTP)
	{
	CT_SmtpCreateNewMessageWithPlainBodyText* self = new(ELeave) CT_SmtpCreateNewMessageWithPlainBodyText(aSharedDataSMTP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 
	CleanupStack::Pop(self);
	return self;
	}	

	
/**
  Function 		: ~CT_SmtpCreateNewMessageWithPlainBodyText
  Description 	: Destructor
  @return 		: N/A
*/
CT_SmtpCreateNewMessageWithPlainBodyText::~CT_SmtpCreateNewMessageWithPlainBodyText()
	{
	delete iRecipientAddress;
	delete iOperation;
	}

/**
  Function 		: ProgressL
  Description 	: Displays the status of the asynchronous operation
  @return 		: N/A
*/
void CT_SmtpCreateNewMessageWithPlainBodyText::ProgressL(TBool /*bFinal*/)
	{}


/**
  Function 		: CancelStep
  Description 	: Cancels the asynchronous operation
  @return 		: N/A
*/
void CT_SmtpCreateNewMessageWithPlainBodyText::CancelStep()
	{
	iOperation->Cancel();
	}


/**
  Function 		: doTestStepL
  Description 	: Reads the SMTP account name, folder names, download limts and the command name from 
				  the ini file. A selection of the entries uner the remote folder is obtained and passed to 
				  InvokeAsyncFunctionL based on the command.
  @return 		: TVerdict - Test step result
*/
TVerdict CT_SmtpCreateNewMessageWithPlainBodyText::doTestStepL()
	{
	INFO_PRINTF1(_L("TestStep: SmtpCreateNewMessageWithPlainBodyText"));

	// Read the name of the target folder from the ini file
	TPtrC folderName;
	if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);
		}

	TMsvId folderId;
	// Get the Id of the local folder
	folderId= CT_MsgUtilsEnumConverter::FindFolderIdByName(folderName);
	iSharedDataSMTP.iMtm->SwitchCurrentEntryL(folderId);
	TImCreateMessageOptions createMessageOptions;
	createMessageOptions.iMsvPartList = KMsvMessagePartBody;
	createMessageOptions.iMessageType = KUidMsgTypeSMTP;
	
	TPckgBuf<TImCreateMessageOptions> paramPack(createMessageOptions);
	
	CT_MsgActive&	active=Active();
	delete iOperation;
	iOperation = NULL;
	TRAPD(err2, iOperation = CImEmailOperation::CreateNewL(active.iStatus, *iSharedDataSMTP.iSession, folderId,KMsvNullIndexEntryId, paramPack().iMsvPartList, paramPack().iMsvEmailTypeList, paramPack().iMessageType, CActive::EPriorityStandard, ETrue));
	active.Activate();
	CActiveScheduler::Start();

	SetHeaderPartL();
	PopulateBodyPartL();
	User::LeaveIfError(active.Result());
	INFO_PRINTF2(_L("err2 : %d"), err2);
	return TestStepResult();
	}
		

/**
  Function 		: PopulateBodyPartL
  Description 	: Populates body part with the contents of a plain text file.
  @return 		: N/A
*/
void CT_SmtpCreateNewMessageWithPlainBodyText::PopulateBodyPartL()
	{
	//Get the message ID from the CMsvOperation object
	TMsvId temp;	
	TPckgC<TMsvId> paramPack(temp);
	const TDesC8& progBuf = iOperation->ProgressL();	
	paramPack.Set(progBuf);
	TMsvId newMessageId = paramPack();	
	if (newMessageId!=NULL)
		{
		CMsvEntry *entry = CMsvEntry::NewL(*(iSharedDataSMTP.iSession), newMessageId, TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
		CleanupStack::PushL(entry);
		entry->SetEntryL(newMessageId);
		CImEmailMessage *emailMessage = CImEmailMessage::NewLC(*entry);
		CImPlainBodyText* bodytext = emailMessage->OpenPlainBodyTextForWriteL();
		CleanupStack::PushL(bodytext);
		

		TPtrC emailFileName;	
		// Read the name of the email file from the ini
		if(!GetStringFromConfig(ConfigSection(),KEmailFileName,emailFileName))
			{
			ERR_PRINTF1(_L("Email file name not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			RFs	fs;
			User::LeaveIfError(fs.Connect());
			CleanupClosePushL(fs);

			RFile	file;
			User::LeaveIfError(file.Open(fs,emailFileName,EFileRead));
			CleanupClosePushL(file);


			TBuf8<KChunkSize> line;
			TBuf8<1> aChar;				// To read one character from the file
			TBool finished = FALSE;

			// Read text from the file
			do
				{
				line.FillZ();
				line.SetLength(0);
				// Read one line from email file
				do 
					{
					file.Read(aChar, 1);
					if(aChar.Length())
						{
						line.Append(aChar);
						}
					else
						{
						finished = TRUE;
						}
					}while(aChar.Length() && aChar[0] != 0x0A);
				if(!line.Length())
					{
					break;
					}
				CT_MsgActive& active=Active();				
				bodytext->StoreChunkL(line,active.iStatus);
				active.Activate();
				CActiveScheduler::Start();			
				}while(!finished);
				
			CT_MsgActive& active=Active();	
			bodytext->CommitL(active.iStatus);
			active.Activate();
			CActiveScheduler::Start();			
			
			CleanupStack::PopAndDestroy(2); // delete file,fs
			}
		CleanupStack::PopAndDestroy(3, entry); // delete bodytext,emailMessage,entry
		}
	}


/**
  Function 		: GetDefaultAddressL
  Description 	: Retrieve the default email address from the default SMTP service.
  @return 		: N/A
*/
void CT_SmtpCreateNewMessageWithPlainBodyText::GetDefaultAddressL()
	{
	CImSmtpSettings* settings = new(ELeave) CImSmtpSettings();
	CleanupStack::PushL(settings);

	CEmailAccounts* accounts = CEmailAccounts::NewLC();
	TSmtpAccount smtpAccount;
	smtpAccount.iSmtpService = KMsvNullIndexEntryId;
	accounts->DefaultSmtpAccountL(smtpAccount);
	accounts->LoadSmtpSettingsL(smtpAccount, *settings);
	CleanupStack::PopAndDestroy(accounts);								// accounts
	iRecipientAddress = settings->EmailAddress().AllocL();
	INFO_PRINTF2(_L("Default Email Address is %s"),iRecipientAddress->Ptr());
	CleanupStack::PopAndDestroy(settings);								// settings
	}
	
	
/**
  Function 		: SetHeaderPartL
  Description 	: Sets the message header fields. If the field values are not provided
  				  as input, it takes the default values.
  @return 		: N/A  				  
*/
void CT_SmtpCreateNewMessageWithPlainBodyText::SetHeaderPartL()
	{
	//check progress
	TMsvId temp;	
	TPckgC<TMsvId> paramPack(temp);
	const TDesC8& progBuf = iOperation->ProgressL();	
	paramPack.Set(progBuf);
	TMsvId newMessageId = paramPack();	
	if (newMessageId!=NULL)
		{
		CMsvEntry *entry = CMsvEntry::NewL(*(iSharedDataSMTP.iSession), newMessageId, TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
		CleanupStack::PushL(entry);
		entry->SetEntryL(newMessageId);			// 
		CMsvStore* store = entry->EditStoreL();	
		CleanupStack::PushL(store);
		
		CImHeader* header = CImHeader::NewLC(); 
		header->RestoreL(*store); // Retrieves the email message header part from the message store
		TPtrC subject;
		if(GetStringFromConfig(ConfigSection(), KSubject, subject))
			{
			header->SetSubjectL(subject);
			}
		TPtrC recipientsAddress;	
		if(GetStringFromConfig(ConfigSection(), KRecipientsAddress, recipientsAddress))
			{
			header->ToRecipients().AppendL(recipientsAddress);
			}
		else
			{
			GetDefaultAddressL();
			header->ToRecipients().AppendL(*iRecipientAddress);
			}
		header->StoreL(*store);
		store->CommitL();				// Saves the modifications to message store
		TMsvEntry indexEntry = entry->Entry();
		indexEntry.iDescription.Set(header->Subject());
		INFO_PRINTF2(_L("Subject field created message : %S"), &(indexEntry.iDescription));
		indexEntry.SetVisible(ETrue);
		entry->ChangeL(indexEntry);			// Modify the corresponding index entry permanently

		CleanupStack::PopAndDestroy(3,entry);				// 	header, store, 	entry
		}
	}

