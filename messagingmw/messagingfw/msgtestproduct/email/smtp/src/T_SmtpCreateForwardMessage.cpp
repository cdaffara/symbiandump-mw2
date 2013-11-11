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
// SmtpCreateForwadMessage
// [Parameters]
// ServiceType			:	Specifies the service type(IMAP or POP) under which message exists.
// FolderName			:	Destination local folder where message is to be created.
// ImapAccountName		:	If service type is IMAP4, specify the IMAP account name that holds the
// message.
// ParentFolderName	:	IMAP4 folder name that holds the message.
// PopAccountName		:	If the the sevice type is POP3, specify POP account name.
// CommandName			:	SMTP client MTM command used to create the forward message.
// FolderName			:	Destination local folder in which message would be storted.
// SubjectOfExistingMsg:	Sbject line of the existing message to which forward message
// would be creted.
// SubjectOfForwardMsg	:	Specifies the subject line to be set for the froward message.
// This is an optional parameter and if not specified the subject line
// would take the default format(Fwd: <SubjectOfExistingMsg>).
// Creates a forward message to an existing message using the SMTP client MTM command
// and modifies the message header information.
// [APIs Used]
// CImEmailMessage::OpenPlainBodyTextForReadL
// CImPlainBodyText::NextChunkL
// 
//


// User includes 
#include "T_SmtpCreateForwardMessage.h"
#include <t_utilscentralrepository.h>
#include <t_utilsenumconverter.h>
#include <t_utils.h>


// Epoc includes 
#include <miutmsg.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "cimmessagepart.h"
#endif
// Literals Used 
_LIT(KServiceType, "ServiceType");

_LIT(KImapAccountName,"ImapAccountName");
_LIT(KParentFolderName,"ParentFolderName");

_LIT(KPopAccountName,"PopAccountName");

_LIT(KCommandName,"CommandName");
_LIT(KFolderName, "FolderName");
_LIT(KSubjectOfExistingMsg,"SubjectOfExistingMsg");

_LIT(KSubjectOfForwardMsg, "SubjectOfForwardMsg");
_LIT(KRecipientsAddress, "RecipientsAddress");

const TInt KZero={0};
/**
  Function 		: CT_SmtpCreateForwadMessage
  Description 	: Constructor
  @param 		: aSharedDataSMTP		Reference to CT_MsgSharedDataSmtp
  @return 		: N/A
*/
CT_SmtpCreateForwadMessage::CT_SmtpCreateForwadMessage(CT_MsgSharedDataSmtp& aSharedDataSMTP)
:	CT_MsgAsyncStepSMTP(aSharedDataSMTP)
,	iOperation(NULL)
	{
	SetTestStepName(KSmtpCreateForwadMessage);
	}


/**
  Function 		: NewL
  Description 	: Creates an object of CT_SmtpCreateForwadMessage 				 
  @return 		: N/A
*/
CT_SmtpCreateForwadMessage* CT_SmtpCreateForwadMessage::NewL(CT_MsgSharedDataSmtp& aSharedDataSMTP)
	{
	CT_SmtpCreateForwadMessage* self = new(ELeave) CT_SmtpCreateForwadMessage(aSharedDataSMTP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 
	CleanupStack::Pop(self);
	return self;
	}	

	
/**
  Function 		: ~CT_SmtpCreateForwadMessage
  Description 	: Destructor
  @return 		: N/A
*/
CT_SmtpCreateForwadMessage::~CT_SmtpCreateForwadMessage()
	{
	delete iRecipientAddress;
	delete iOperation;
	iOperation = NULL;
	}

/**
  Function 		: ProgressL
  Description 	: Displays the status of the asynchronous operation
  @return 		: N/A
*/
void CT_SmtpCreateForwadMessage::ProgressL(TBool /*bFinal*/)
	{}


/**
  Function 		: CancelStep
  Description 	: Cancels the asynchronous operation
  @return 		: N/A
*/
void CT_SmtpCreateForwadMessage::CancelStep()
	{
	iOperation->Cancel();
	}


/**
  Function 		: doTestStepL
  Description 	: Creates a forward message for an existing message. Existing message
  				  may be under some remot folder under IMAP service or under POP service.
  @return 		: TVerdict - Test step result
*/
TVerdict CT_SmtpCreateForwadMessage::doTestStepL()
	{
	INFO_PRINTF1(_L("TestStep: SmtpCreateFwdMsgForPlainBodyTextMsg"));
	TMsvId	entryToBesearched = KMsvNullIndexEntryId;
	TPtrC serviceType;
	if(!GetStringFromConfig( ConfigSection(), KServiceType, serviceType))
		{
		ERR_PRINTF1(_L("\"ServiceType\" for the message is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		TBuf<4> temp(serviceType);
		temp.UpperCase();						// Makes case insensitive
		// Perform the following if message has been downloaded using IMAP4 service
		if(temp.CompareC(_L("IMAP")) == KZero)
			{
			// Read the IMAP account name from the ini file
			TPtrC imapAccountName;
			if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
				{
				ERR_PRINTF1(_L("ImapAccount Name is not specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				TPtrC	parentFolderName;
				if(!GetStringFromConfig( ConfigSection(), KParentFolderName, parentFolderName))
					{
					ERR_PRINTF1(_L("Parent Folder is not specified"));
					SetTestStepResult(EFail);
					}
				else
					{
					
					// retrieves the folder Id based on the remote service folder name read from the ini file
					entryToBesearched = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataSMTP.iSession, imapAccountName, parentFolderName);
					if( entryToBesearched == KErrNotFound)
						{
						ERR_PRINTF1(_L("Invalid Remote folder name specified"));
						SetTestStepResult(EFail);
						}
					else
						{
						INFO_PRINTF2(_L("The parent folder Id is %d"),entryToBesearched );
						}
					}
				}
			}
		// Perform the following if message has been downloaded using POP3 service 	
		else if(temp.CompareC(_L("POP")) == KZero)
			{
			TPtrC popAccountName;
			if(!GetStringFromConfig( ConfigSection(), KPopAccountName, popAccountName))
				{
				ERR_PRINTF1(_L("PopAccount Name is not specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				// Retrieves the Pop service Id for the given Pop account
				entryToBesearched = CT_MsgUtilsCentralRepository::GetPopServiceIdL((TDes&)popAccountName);
				INFO_PRINTF2(_L("Pop service id is %d"), entryToBesearched);

				// Fails the test step if an invalid POP account is specified 
				if(entryToBesearched == KMsvNullIndexEntryId)
					{
					ERR_PRINTF1(_L("Invalid POP account name specified"));
					SetTestStepResult(EFail);
					}
				}
			}
		else
			{
			ERR_PRINTF1(_L("Sevice Type is not specified: It should be IMAP or POP"));
			SetTestStepResult(EFail);
			}
		}
		
	if(TestStepResult() == EPass)
		{
		TPtrC commandName;
		if(!GetStringFromConfig( ConfigSection(), KCommandName, commandName))
			{
			ERR_PRINTF1(_L("Command name is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			// Read the name of the target folder from the ini file
			TPtrC folderName;
			if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
				{
				ERR_PRINTF1(_L("Folder name is not specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				TPtrC subjectOfExistingMsg;	
				if( !GetStringFromConfig(ConfigSection(), KSubjectOfExistingMsg, subjectOfExistingMsg))
					{
					ERR_PRINTF1(_L("\"SubjectOfExistingMsg\" is not specified"));
					SetTestStepResult(EFail);
					}
				else
					{
					// Retrieves the message Id based on the message subject from the given remote service folder
					TMsvId messageId;
					messageId = CT_MsgUtils::SearchMessageBySubjectL(iSharedDataSMTP.iSession, entryToBesearched, subjectOfExistingMsg, ETrue);
					if( messageId == KMsvNullIndexEntryId)
						{
						ERR_PRINTF1(_L("The given message is not found"));
						SetTestStepResult(EFail);
						}
					else
						{
						TMsvId folderId;
						folderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(folderName);
						
						// Interpret the SMTP client MTM command to be executed 
						TSmtpCmds commandId = CT_MsgUtilsEnumConverter::ConvertDesToTSmtpCmdsL((TDesC&) commandName);									

						CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
						CleanupStack::PushL(selection);
									
						selection->AppendL(folderId);
						selection->AppendL(messageId);

						TImCreateMessageOptions createMessageOptions;
						createMessageOptions.iMsvPartList = KMsvMessagePartBody;
						createMessageOptions.iMessageType = KUidMsgTypeSMTP;
						createMessageOptions.iMsvEmailTypeList = 0;
						TPckgBuf<TImCreateMessageOptions> paramPack(createMessageOptions);
						
						CT_MsgActive&	active=Active();
						delete iOperation;
						iOperation = NULL;
						TRAPD(err2,iOperation = iSharedDataSMTP.iMtm->InvokeAsyncFunctionL(commandId,*selection,paramPack,active.iStatus));
						active.Activate();
						CActiveScheduler::Start();
						SetHeaderPartL();

						User::LeaveIfError(active.Result());
						INFO_PRINTF2(_L("err2 : %d"), err2);
						CleanupStack::PopAndDestroy(selection); // selection
						}
					}
				}
			}
		}
	return TestStepResult();
	}


/**
  Function 		: SetHeaderPartL
  Description 	: Sets the message header fields. If the field values are not provided
  				  as input, it takes the default values.
  @return 		: N/A  				  
*/
void CT_SmtpCreateForwadMessage::SetHeaderPartL()
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
		entry->SetEntryL(newMessageId);			// 
		CMsvStore* store = entry->EditStoreL();	
		CleanupStack::PushL(store);
		
		CImHeader* header = CImHeader::NewLC(); 
		header->RestoreL(*store); // Retrieves the email message header part from the message store
		TPtrC subjectOfForwardMsg;
		if(GetStringFromConfig(ConfigSection(), KSubjectOfForwardMsg, subjectOfForwardMsg))
			{
			header->SetSubjectL(subjectOfForwardMsg);
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
		INFO_PRINTF2(_L("Subject field of Forward message : %S"), &(indexEntry.iDescription));
		indexEntry.SetVisible(ETrue);
		entry->ChangeL(indexEntry);			// Modify the corresponding index entry permanently

		CleanupStack::PopAndDestroy(3,entry);	//	header, store, 	entry
		}
	}


/**
  Function 		: GetDefaultAddressL
  Description 	: Retrieve the default email address from the default SMTP service.
  @return 		: N/A
*/
void CT_SmtpCreateForwadMessage::GetDefaultAddressL()
	{
	CImSmtpSettings* settings = new(ELeave) CImSmtpSettings();
	CleanupStack::PushL(settings);

	CEmailAccounts* accounts = CEmailAccounts::NewLC();
	TSmtpAccount smtpAccount;
	smtpAccount.iSmtpService = KMsvNullIndexEntryId;
	accounts->DefaultSmtpAccountL(smtpAccount);
	accounts->LoadSmtpSettingsL(smtpAccount, *settings);
	CleanupStack::PopAndDestroy(accounts);					// accounts
	iRecipientAddress = settings->EmailAddress().AllocL();
	INFO_PRINTF2(_L("Default Email Address is %s"),iRecipientAddress->Ptr());
	CleanupStack::PopAndDestroy(settings);					// settings
	}

