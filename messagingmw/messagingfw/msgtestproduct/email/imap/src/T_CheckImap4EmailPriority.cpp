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
// [TestStep Name]
// CheckImap4EmailPriority
// [Parameters]
// ParentFolderName		:Parent folder name in which the message to be checked is present
// EmailSubject			:Subject of the message to be searched
// Target					:Specify either REMOTE or LOCAL depending on where you want to search for the email message
// ImapAccountName			:Name of the Imap account
// ExpectedPriority		:Value 0 means priority flag is not set (EFalse) and 1 means it is set (ETrue) 
// Checks the priority of an email message via the IMAP \Flagged flag
// 
//

/**
 @file
*/

//User includes
#include "T_CheckImap4EmailPriority.h"
#include <t_utilsenumconverter.h>
#include <t_utils.h>
#include <t_utilscentralrepository.h>

//Literals used
_LIT(KParentFolderName,"ParentFolderName");
_LIT(KEmailSubject,"EmailSubject");
_LIT(KTarget,"Target");
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KExpectedPriority,"ExpectedPriority");

/**
  Function : CT_MsgCheckImap4EmailPriority
  Description : Constructor 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgCheckImap4EmailPriority::CT_MsgCheckImap4EmailPriority(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iOperation(NULL)
	{
	SetTestStepName(KCheckImap4EmailPriority);
	}

/**
  Function : NewL
  Description : Creates an object of CT_MsgCheckImap4EmailPriority 				 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : Pointer to CT_MsgCheckImap4EmailPriority
*/
CT_MsgCheckImap4EmailPriority* CT_MsgCheckImap4EmailPriority::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgCheckImap4EmailPriority* self = new(ELeave) CT_MsgCheckImap4EmailPriority(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL();// Call CT_AsyncStep::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : ~CT_MsgCreateImap4Folder
  Description : Destructor
  @return : N/A
*/
CT_MsgCheckImap4EmailPriority::~CT_MsgCheckImap4EmailPriority()
	{
	delete iOperation;
	iOperation=NULL;
	}
	
/**
  Function : doTestStepL
  Description : Checks the priority of an email message via the IMAP \Flagged flag
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgCheckImap4EmailPriority::doTestStepL()
	{
	INFO_PRINTF1( _L("Test Step : CheckSmtpEmailPriority") );
	
	if (ReadIni())
		{
		TMsvId parentFolderId;
		// Local parent folder target...
		if(iTarget.Compare(_L("LOCAL"))==0)
			{
			// Retrieves the folder Id based on the local folder name read from the ini file
			parentFolderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(iParentFolderName);
			if(parentFolderId == KErrNotFound)
				{
				ERR_PRINTF1(_L("Invalid local parent folder name specified"));
				SetTestStepResult(EFail);
				return TestStepResult();
				}
			INFO_PRINTF2(_L("The local parent folder Id is %d"),parentFolderId);
			}
		// Remote parent folder target...
		else if(iTarget.Compare(_L("REMOTE"))==0)
			{
			// Note: Using new overloaded recursive CT_MsgUtils::GetRemoteFolderIdByNameL
			TRAPD(err,parentFolderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession, iImapAccountName, iParentFolderName));
			if(err == KErrNotFound)
				{
				ERR_PRINTF1(_L("Invalid remote parent folder name specified"));
				SetTestStepResult(EFail);
				return TestStepResult();
				}	
			INFO_PRINTF2(_L(" The remote parent folder Id is %d"),parentFolderId);	
			}
		// Target Local or Remote not specified...
		else
			{
			ERR_PRINTF1(_L("Invalid parent folder target"));
			SetTestStepResult(EFail);
			return TestStepResult();
			}
			
		// Retrieves the message Id based on the message subject of the email under the given parent folder
		TMsvId	messageId;
		TRAPD(err,messageId = CT_MsgUtils::SearchMessageBySubjectL(iSharedDataIMAP.iSession, parentFolderId, iEmailSubject));
		if(err == KErrNotFound)
			{
			ERR_PRINTF1(_L("The given email message is not found"));
			SetTestStepResult(EFail);
			}
		// Message found	
		else
			{
			INFO_PRINTF2(_L("The Message Id is %d"),messageId);

			// Retrieve the default Smtp service Id
			TMsvId smtpServiceId(0);
			TRAPD(err,smtpServiceId = CT_MsgUtilsCentralRepository::GetDefaultSmtpServiceIdL());
			if(err != KErrNone)
				{
				ERR_PRINTF2(_L("Failure while getting the default SMTP Service Id. Error = %d"),err);
				SetTestStepResult(EFail);
				}
			else
				{
				INFO_PRINTF2(_L("The Default Smtp Service Id is %d"),smtpServiceId);

				// Setting the current context to the parent of the mesage
				CMsvEntry*	entry = CMsvEntry::NewL(*iSharedDataIMAP.iSession, messageId, TMsvSelectionOrdering());
				CleanupStack::PushL(entry);
			
				TMsvEmailEntry emailEntry(entry->Entry());
				TBool flag = emailEntry.FlaggedIMAP4Flag();
				
				if(flag)
					{
					INFO_PRINTF1(_L("The Flagged IMAP4 flag for the email is set for urgent/special attention"));
					if(iExpectedPriority==1)
						{
						INFO_PRINTF1(_L("The priority of email is as expected"));
						}
					else
						{
						ERR_PRINTF1(_L("The priority of email is not as expected!"));
						SetTestStepResult(EFail);
						}
					}
				else
					{
					INFO_PRINTF1(_L("The Flagged IMAP4 flag for the email is NOT set for urgent/special attention"));
					if(iExpectedPriority==0)
						{
						INFO_PRINTF1(_L("The priority of email is as expected"));
						}
					else
						{
						ERR_PRINTF1(_L("The priority of email is not as expected!"));
						SetTestStepResult(EFail);
						}
					}
				CleanupStack::PopAndDestroy(entry);
				}
			}
		}
	return TestStepResult();
	}
	
/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/	
TBool CT_MsgCheckImap4EmailPriority::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KParentFolderName, iParentFolderName))
		{
		ERR_PRINTF1(_L("Parent folder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KEmailSubject, iEmailSubject))
		{
		ERR_PRINTF1(_L("Email subject is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KTarget, iTarget))
		{
		ERR_PRINTF1(_L("Target of either REMOTE or LOCAL is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetIntFromConfig(ConfigSection(), KExpectedPriority, iExpectedPriority))
		{
		ERR_PRINTF1(_L("Expected priority is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	return result;
	}

/**
  Function : ProgressL
  Description : Displays the progress information of the asynchronous operation
  @return : void
*/
void CT_MsgCheckImap4EmailPriority::ProgressL(TBool bFinal)
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
void CT_MsgCheckImap4EmailPriority::CancelStep()
	{
	iOperation->Cancel();
	}
