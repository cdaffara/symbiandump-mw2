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
// [TestStep Name]
// IsImapMessageHeaderOnly
// [Paramaters]
// ImapAccountName			:Name of the Imap account.
// ImapFolderName		  	:Name of the remote folder that the message resides.
// Subject			  		:Subject of the received message to be checked.
// HeaderOnly				:The state to test: 
// :ETrue(1) => Header only message expected.
// :EFalse(0) => Message with header and some or all content (body and/or attachments) expected.
// Checks if the fetched IMAP message for a given account is header only
// or content for it has been downloaded such as body and attachments .
// 
//

//User includes
#include "T_IsImapMessageHeaderOnly.h"
#include <t_utilscentralrepository.h>
#include <t_utils.h>

//Epoc inlcudes
#include <msvids.h>
#include <mmsvattachmentmanager.h>
#include <miutmsg.h>

//Literals used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KImapFolderName,"ImapFolderName");
_LIT(KSubject,"Subject");
_LIT(KHeaderOnly,"HeaderOnly");

/**
  Function : CT_MsgIsImapMessageHeaderOnly
  Description : Constructor 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgIsImapMessageHeaderOnly::CT_MsgIsImapMessageHeaderOnly(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,iOperation(NULL)
,iRecvMessageInfo(NULL)
	{
	SetTestStepName(KIsImapMessageHeaderOnly);
	}

/**
  Function : ~CT_MsgIsImapMessageHeaderOnly
  Description : Destructor
  @return : N/A
*/
CT_MsgIsImapMessageHeaderOnly::~CT_MsgIsImapMessageHeaderOnly()
	{
	delete iOperation;
	delete iRecvMessageInfo;
	}
	
/**
  Function : NewL
  Description : Creates an object of CT_MsgIsImapMessageHeaderOnly 				 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : Pointer to CT_MsgIsImapMessageHeaderOnly
*/
CT_MsgIsImapMessageHeaderOnly* CT_MsgIsImapMessageHeaderOnly::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgIsImapMessageHeaderOnly* self = new(ELeave) CT_MsgIsImapMessageHeaderOnly(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgIsImapMessageHeaderOnly::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KImapFolderName, iImapFolderName))
		{
		ERR_PRINTF1(_L("Remote Imap folder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KSubject, iSubject))
		{
		ERR_PRINTF1(_L("Subject name of the received message to be checked is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetBoolFromConfig(ConfigSection(), KHeaderOnly, iHeaderOnly))
		{
		ERR_PRINTF1(_L("An expected \"ETrue\" or \"EFalse\" boolean value is not specified for checking header only state"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	return result;
	}

/**
  Function : doTestStepL
  Description : Checks if the fetched IMAP message for a given account is header only
   				or content for it has been downloaded such as body and attachments.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgIsImapMessageHeaderOnly::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : IsImapMessageHeaderOnly"));	
	if(ReadIni())
		{
		// Retrieve the Imap service Id for the given Imap account
		TMsvId	imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)iImapAccountName);
		INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId );
		
		// If the IMAP Service Id is KMsvNullIndexEntryId then an invalid IMAP account name is specified
		if(imapServiceId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid IMAP account name is specified"));
			SetTestStepResult(EFail);
			}
		
		// Retrieve the message Id based on the message subject
		TMsvId messageId = CT_MsgUtils::FindEntryByNameL(iSharedDataIMAP.iSession, imapServiceId, (TDesC&)iSubject, (TDesC&)iImapFolderName);

		if(messageId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("The given message is not found"));
			SetTestStepResult(EFail);
			}
			// Message found	
		else
			{
			INFO_PRINTF2(_L("The message Id is %d"),messageId );
			}
			
		CMsvEntry* recvMsvEntry = (iSharedDataIMAP.iSession)->GetEntryL(KMsvRootIndexEntryId);
		CleanupStack::PushL(recvMsvEntry);
		//Set the context to the sent message entry
		recvMsvEntry->SetEntryL(messageId);
	
		iRecvMessageInfo = CT_MsgUtilsEmailMessage::NewL(*recvMsvEntry,*this);
		
		TBool headerOnly = EFalse;
		
		TInt32 bodyTextSize = iRecvMessageInfo->GetBodyTextSize();
		
		// Get attachment list for the received message
		CImEmailMessage* recvEmailMessage = iRecvMessageInfo->GetEmailMessageL();
		
		// Call the asynchronous function to get the attachment list
		TInt err = iRecvMessageInfo->GetMessageAttachmentListL();
		if (err != KErrNone)
			{
			SetTestStepResult(EFail);
			return TestStepResult();
			}
			
		// Get the list of attachments present in received message
		MMsvAttachmentManager& recvManager = recvEmailMessage->AttachmentManager();
		TInt recvMsgAttachmentCount = recvManager.AttachmentCount();
		
		if (bodyTextSize == 0 && recvMsgAttachmentCount == 0)
			{
			headerOnly = ETrue;
			}
		else if (bodyTextSize > 0 || recvMsgAttachmentCount > 0)
			{
			headerOnly = EFalse;
			}

		// Test expected state
		if (iHeaderOnly == headerOnly)
			{
			INFO_PRINTF1(_L("State of received message is as expected"));
			}
		else
			{
			ERR_PRINTF1(_L("State of received message is not as expected"));
			SetTestStepResult(EFail);
			}
		CleanupStack::PopAndDestroy(recvMsvEntry); // recvMsvEntry
		}
	return TestStepResult();
	}
	
/**
  Function : ProgressL
  Description : Displays the progress information of the asynchronous operation
  @return : void
*/
void CT_MsgIsImapMessageHeaderOnly::ProgressL(TBool bFinal)
	{
	//	Stub
	if ( bFinal )
		{
		/*	Display final progress iOperation */
		}
	else
		{
		/*	Display current progress iOperation */
		}
	}

/**
  Function : CancelStep
  Description : Cancels the asynchronous operation
  @return : void
*/
void CT_MsgIsImapMessageHeaderOnly::CancelStep()
	{
	iOperation->Cancel();
	}
