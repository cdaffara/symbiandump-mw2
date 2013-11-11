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
// SendSmtpMessage
// [Parameters]
// ParentFolderName  :   Local folder name in which the message to be sent is 
// present
// Subject			  :	  Subject of the message to be sent 	
// SmtpAccountName   :	  SMTP account name to be used for sending the message. If this is
// not specified, message(s) would send using the default SMTP service.
// Searches for the message with the specified subject and sends the message if the
// message is found.  If the local folder name is invalid or if the message is 
// not found in the given folder,the test step fails.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::CopyL
// 
//

/**
 @file
*/



//User Includes
#include "T_SendSmtpMessage.h"
#include <t_utilsenumconverter.h>
#include <t_utils.h>
#include <t_utilscentralrepository.h>


// Literals Used
_LIT(KSubject,"Subject");
_LIT(KParentFolderName,"ParentFolderName");
_LIT(KSmtpAccountName, "SmtpAccountName");


/**
CT_MsgSendSmtpMessage
Constructor

@param :
@return : N/A


*/
CT_MsgSendSmtpMessage::CT_MsgSendSmtpMessage(CT_MsgSharedDataSmtp& aSharedDataSMTP)
:	CT_MsgAsyncStepSMTP(aSharedDataSMTP)
,	iOperation(NULL)
	{
	SetTestStepName(KSendSmtpMessage);
	}


/**
NewL()
Static factory constructor. Uses two phase 
construction.

@param aSharedDataSMTP
@leave KErrNoMemory
@return
A pointer to the newly created CT_MsgSendSmtpMessage object.
*/
CT_MsgSendSmtpMessage* CT_MsgSendSmtpMessage::NewL(CT_MsgSharedDataSmtp& aSharedDataSMTP)
	{
	CT_MsgSendSmtpMessage* self = new(ELeave) CT_MsgSendSmtpMessage(aSharedDataSMTP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 // Call CT_AsyncStep::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}


/**
~CT_MsgSendSmtpMessage()
Destructor
*/
CT_MsgSendSmtpMessage::~CT_MsgSendSmtpMessage()
	{
	delete iOperation;
	iOperation=NULL;
	}


/**
ProgressL()

@param bFinal
*/
void CT_MsgSendSmtpMessage::ProgressL(TBool bFinal)
	{
	//	TODO
	if ( bFinal )
		{
		//	Display final progress iOperation
		}
	else
		{
		//	Display current progress iOperation
		}
	}


/**
CancelStep()
*/
void CT_MsgSendSmtpMessage::CancelStep()
	{
	iOperation->Cancel();
	}

	
/**
doTestStepL()
Reads the parent folder name, the message subject from the ini file
Searches for the message under the specified folder, and sends the message

@return
Returns the test step result
*/
TVerdict CT_MsgSendSmtpMessage::doTestStepL()
	{
	INFO_PRINTF1( _L("Test Step : Send Smtp Message") );	
	TPtrC	parentFolderName;
	if(!GetStringFromConfig( ConfigSection(), KParentFolderName, parentFolderName))
		{
		ERR_PRINTF1(_L("Parent Folder is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		TPtrC	subject;	
		if( !GetStringFromConfig(ConfigSection(), KSubject, subject ))
			{
			ERR_PRINTF1(_L("The subject of the message is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			// retrieves the folder Id based on the local folder name read from the ini file
			TMsvId	parentFolderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(parentFolderName);
			if( parentFolderId == KErrNotFound)
				{
				ERR_PRINTF1(_L("Invalid local folder name specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				INFO_PRINTF2(_L("The parent folder Id is %d"),parentFolderId );

				// Retrieves the message Id based ont he message subject from the given local folder
				TMsvId	messageId = CT_MsgUtils::SearchMessageBySubjectL(iSharedDataSMTP.iSession, parentFolderId , subject);
				if( messageId == KMsvNullIndexEntryId)
					{
					ERR_PRINTF1(_L("The given message is not found"));
					SetTestStepResult(EFail);
					}
				// Message found	
				else
					{
					INFO_PRINTF2(_L("The Message Id is %d"),messageId );

					// Retrieve the default Smtp service Id
					TMsvId	smtpServiceId(0);
					TPtrC smtpAccountName;
					TInt err;
					if( !GetStringFromConfig(ConfigSection(), KSmtpAccountName, smtpAccountName ))
						{
						TRAP(err, smtpServiceId = CT_MsgUtilsCentralRepository::GetDefaultSmtpServiceIdL());
						}
					else
						{
						TRAP(err, smtpServiceId = CT_MsgUtilsCentralRepository::GetSmtpServiceIdL((TDes&)smtpAccountName));
						}

					if(err != KErrNone)
						{
						ERR_PRINTF2(_L("Failure while getting the SMTP Service Id.  error = %d"),err);
						SetTestStepResult(EFail);
						}
					else
						{
						INFO_PRINTF2(_L("The SMTP Service Id is %d"),smtpServiceId );
						// Setting the current context to the parent of the mesage
						CMsvEntry*	folderEntry = CMsvEntry::NewL(*iSharedDataSMTP.iSession, messageId , TMsvSelectionOrdering());
						CleanupStack::PushL(folderEntry);
						folderEntry->SetEntryL(messageId);
						folderEntry->SetEntryL(folderEntry->Entry().Parent());
						// Sends the message
						CT_MsgActive&	active=Active();
						iOperation = folderEntry->CopyL(messageId, smtpServiceId, active.iStatus);
						active.Activate();
						CActiveScheduler::Start();

						TImSmtpProgress temp;
						TPckgC<TImSmtpProgress> paramPack(temp);
						paramPack.Set(iOperation->ProgressL());
						TImSmtpProgress progress=paramPack();
						SetTestStepError(progress.Error());

						if (NULL!=iOperation)
							{
							delete iOperation;
							iOperation=NULL;
							}
						CleanupStack::PopAndDestroy(folderEntry); //operation,folderEntry
						}
					}
				}
			}
		}

	return TestStepResult();
	}
