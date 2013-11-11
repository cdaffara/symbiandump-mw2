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
// @file 
// [TestStep Name]
// ImapInvokeAsyncFunction
// [Paramaters]
// ImapAccountName			: Name of the Imap account
// FolderName				: Name of the remote folder
// Command Name				: Name of the Imap command to be executed
// DownloadLimits			: The download limits,1 - Full, 0 - Partial
// The test obtains a selection of the entries under the remote folder for the given Imap account.
// Based on the command, it establishes connection, calls the InvokeAsyncFunction with the command
// and disconnects based on the command.It then calls CompareEmailL to compare the sent and received messages.
// The test passes only when these messages match.Else it fails.
// [APIs Used]
// CMsvEntry::SetEntryL()
// CMsvEntrySelection::AppendL()
// CMsvEntrySelection::ResizeL()
// CImap4ClientMtm::InvokeAsyncFunctionL()
// 
//

/* User includes */
#include "T_ImapInvokeAsyncFunction.h"
#include <t_utilscentralrepository.h>
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include "T_UtilsCompareEmailMessages.h"


/* Epoc includes */
#include <e32cmn.h>
#include <imapset.h>
#include <cemailaccounts.h>


/* Literals Used */
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KCommandName,"CommandName");
_LIT(KFolderName,"FolderName");
_LIT(KDownloadLimits,"DownloadLimits");
_LIT(KIsCompareReqd,"IsCompareReqd");



/**
  Function : CT_MsgImapInvokeAsyncFunction
  Description : Constructor
  @param :	aSharedDataIMAP		Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgImapInvokeAsyncFunction::CT_MsgImapInvokeAsyncFunction(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iOperation(NULL)
,	iMailSelection(NULL)
,	iRecvMsgSelectionList(NULL)
	{
	SetTestStepName(KImapInvokeAsyncFunction);
	}


/**
  Function : NewL
  Description : Creates an object of CT_MsgImapInvokeAsyncFunction 				 
  @return : N/A
*/
CT_MsgImapInvokeAsyncFunction* CT_MsgImapInvokeAsyncFunction::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgImapInvokeAsyncFunction* self = new(ELeave) CT_MsgImapInvokeAsyncFunction(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 
	CleanupStack::Pop(self);
	return self;
	}	

	
/**
  Function : ~CT_MsgImapInvokeAsyncFunction
  Description : Destructor
  @return : N/A
*/
CT_MsgImapInvokeAsyncFunction::~CT_MsgImapInvokeAsyncFunction()
	{
	delete iOperation;
	iOperation = NULL;

	delete iMailSelection;
	iMailSelection = NULL;

	delete iRecvMsgSelectionList;
	iRecvMsgSelectionList = NULL;
	}

/**
  Function : ProgressL
  Description : Displays the status of the asynchronous operation
  @return : N/A
*/
void CT_MsgImapInvokeAsyncFunction::ProgressL(TBool bFinal)
	{
	//	TODO
	if ( bFinal )
		{
		/*	Display final progress iOperation */
		}
	else
		{
		/*	Display current progress iOperation*/
		}
	}


/**
  Function : CancelStep
  Description : Cancels the asynchronous operation
  @return : N/A
*/
void CT_MsgImapInvokeAsyncFunction::CancelStep()
	{
	iOperation->Cancel();
	}


/**
  Function : doTestStepL
  Description : Reads the Imap account name, remote folder name, download limts and the command name
				from the ini file.Based on the command the connection is established.
				A selection of the entries uner the remote folder is obtained and passed to InvokeAsyncFunction
				based on the command.Then disconnection is done based on the command again.
				The sent and received messages are then oompared.If they do not match the test fails.Else it passes.
  @return : TVerdict - Test step result
  @leave  : KMsvNullIndexEntryId		Invalid IMAP account name specified or
										Invalid remote folder name specified

*/
TVerdict CT_MsgImapInvokeAsyncFunction::doTestStepL()
	{
	INFO_PRINTF1(_L("TestStep: InvokeAsyncFunction"));

	/* Read the IMAP account name from the ini file*/
	TPtrC imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("ImapAccount Name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		/* Retrieves the Imap service Id for the given Imap account*/
		TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
		INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId );

		/* Fails the test step if an invalid IMAP account is specified */
		if(imapServiceId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid IMAP account name specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			/**
			 * A selection of mails that needs to be downloaded to a local folder
			 * The first entry in this selection MUST be the service. 
			 * For populating a selection of mails, the selection of mails should
			 * be appended after the service ID. The selection of mails MUST have the
			 * same parent i.e. MUST be in the same folder.
			 */
			
			/* Reads the name of the remote folder from the ini file */
			TPtrC folderName;
			if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
				{
				ERR_PRINTF1(_L("Folder name is not specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				/* Get the Id of the remote folder */
				TMsvId folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,
														imapAccountName,folderName);

				if(folderId == KMsvNullIndexEntryId)
					{
					ERR_PRINTF1(_L("Invalid remote folder name specified"));
					SetTestStepResult(EFail);
					}
				else
					{
					INFO_PRINTF2(_L(" The folder Id is %d"),folderId);

					/* Reads the download limits set */
					TBool	downloadLimits;
					if(!GetBoolFromConfig( ConfigSection(),KDownloadLimits,downloadLimits))
						{
						ERR_PRINTF1(_L("DownloadLimits is not specified"));
						SetTestStepResult(EFail);
						}
					else
						{	
						/* Reads the IMAP command to be executed from the ini file*/
						TPtrC commandName;
						if(!GetStringFromConfig( ConfigSection(), KCommandName, commandName))
							{
							ERR_PRINTF1(_L("Command name is not specified"));
							SetTestStepResult(EFail);
							}
						else
							{
							/* Returns a selection of the children entries under the Sent Items folder */
							CMsvEntrySelection* sentMsgSelectionList = CT_MsgUtils::CreateChildrenSelectionL(iSharedDataIMAP.iSession,
																	   KMsvSentEntryId);
							CleanupStack::PushL(sentMsgSelectionList);
								
							/* Retrieves the IMAP command to be executed based on the comman name */
							TImap4Cmds command = CT_MsgUtilsEnumConverter::ConvertDesToTImap4Cmds((TDesC&) commandName);									

							
							/* Based on the command decide whether a selection object needs to be passed */
							TBool isWithSelection = EFalse;
							isWithSelection = IsWithSelection(command);
							if(isWithSelection)
								{
								/* Gets a selection of the children entries for the remote folder*/
								iRecvMsgSelectionList =  CT_MsgUtils::CreateChildrenSelectionRemoteFolderL(iSharedDataIMAP.iSession,
												imapAccountName,folderName);
								
								if (iRecvMsgSelectionList->Count() > 0)
									{
									TInt error = KErrNone;
									TRAP(error, ImapInvokeAsyncFunctionWithSelectionL(command,imapServiceId,*iRecvMsgSelectionList,downloadLimits,folderId));
									if(error != KErrNone)
										{
										ERR_PRINTF2(_L("ImapInvokeAsyncFunctionWithSelectionL() failed with %d error"), error);
										SetTestStepError(error);
										return TestStepResult();
										}
									}
								else
									{
									ERR_PRINTF1(_L("Tried to invoke async function with empty selection"));
									SetTestStepResult(EFail);
									}
								}
							else
								{
								TInt error = KErrNone;
								TRAP(error, ImapInvokeAsyncFunctionWithoutSelectionL(command,imapServiceId,folderId,downloadLimits));
								if(error != KErrNone)
									{
									ERR_PRINTF2(_L("ImapInvokeAsyncFunctionWithoutSelectionL() failed with %d error"), error);
									SetTestStepError(error);
									return TestStepResult();
									}
								else
									{
									/* Gets a selection of the children entries for the remote folder */
									iRecvMsgSelectionList =  CT_MsgUtils::CreateChildrenSelectionRemoteFolderL(iSharedDataIMAP.iSession, imapAccountName,folderName);
									}
								}

							/* If any of the sent items selection or received items selection is empty then flag an error */
							if( sentMsgSelectionList == NULL || iRecvMsgSelectionList == NULL)
								{
								ERR_PRINTF1(_L("Invalid message selection object found"));
								SetTestStepResult(EFail);
								}
							else
								{
								/* Reads if comparision of the messages is required */
								TBool isCompareRequired = ETrue;
								GetBoolFromConfig( ConfigSection(), KIsCompareReqd, isCompareRequired);
									
								if(isCompareRequired)
									{
									/* Comparing the Email messages of the remote folder with those in the sent items */
									CT_MsgUtilsCompareEmailMessages* obj = CT_MsgUtilsCompareEmailMessages::NewL(*this);
									CleanupStack::PushL(obj);
									
									/* Retrieves the result of the comparision */
									TBool finalResult = obj->CompareEmailL(*iSharedDataIMAP.iSession, imapServiceId, folderId,downloadLimits, *iRecvMsgSelectionList,*sentMsgSelectionList);
									if(!finalResult)
										{
										ERR_PRINTF1(_L("Compare Email Messages Failed"));
										SetTestStepResult(EFail);
										}		
									CleanupStack::PopAndDestroy(obj);	/*obj*/
									}
								}
							CleanupStack::PopAndDestroy(sentMsgSelectionList);	/*sentMsgSelectionList*/
							}
						}
					}
				}
			}
		}
	return TestStepResult();
	}
												
			

/**
  Function : ImapInvokeAsyncFunctionWithSelection
  Description : Calls InvokeAsyncImap4PopulateCommandL after creating a selection object
				with all the children entries appended to the selection object
  @param :	aCommand		The Imap command to be executed.
			aImapServiceId	The Imap service Id
			aMailSelection	The selection object
			aDownloadLimits	The download limts set.
			aFolderId		The folder Id
  @return : none
*/
void CT_MsgImapInvokeAsyncFunction::ImapInvokeAsyncFunctionWithSelectionL(TImap4Cmds aCommand,TMsvId aImapServiceId,
									CMsvEntrySelection& aMailSelection, TBool aDownloadLimits,TMsvId aFolderId)
	{
	/* Create a new selection object*/
	iMailSelection =  new (ELeave) CMsvEntrySelection;
	
	/* The first entry in this selection MUST be the IMAP service */
	if(aCommand != KIMAP4MTMPopulate)
		{
		iMailSelection->InsertL(0, aImapServiceId);
		}
		
	/* Gets a count of the children entries of the remote folder*/
	TInt mailCount = aMailSelection.Count();

	/* Gets the Ids of all the children entries and appends it to the new selection object*/
	for (TInt index=0; index< mailCount; index++)	
		{
		TMsvId msvId = aMailSelection.At(index);
		iMailSelection->AppendL(msvId);
		}
					
	/*Function to invoke asynchronous IMAP4-specific operation*/
	InvokeAsyncImap4PopulateCommandL(aCommand, *iMailSelection, aImapServiceId,
											aDownloadLimits,aFolderId);
	}
		


/**
  Function : ImapInvokeAsyncFunctionWithoutSelection
  Description : Calls InvokeAsyncImap4PopulateCommandL after creating a selection object
				with all the children entries appended to the selection object
  @param :	aCommand		The Imap command to be executed.
			aImapServiceId	The Imap service Id
			aDownloadLimits	The download limts set.
			aFolderId		The folder Id
  @return : none
*/
void CT_MsgImapInvokeAsyncFunction::ImapInvokeAsyncFunctionWithoutSelectionL(TImap4Cmds aCommand,TMsvId aImapServiceId,
										TMsvId aFolderId, TBool aDownloadLimits)
	{

	/*
	 * Create a mail selection. The first entry in this selection MUST be 
	 * the service. For the 'populate new', 'populate all' situations, the 
	 * folder from which the mails are to be populated should be appended
	 * after the service ID.
	 */
	iMailSelection = new (ELeave) CMsvEntrySelection;
	
	iMailSelection->ResizeL(0);
	iMailSelection->InsertL(0,aImapServiceId);
	iMailSelection->InsertL(1,aFolderId); 

	/* Function to invoke asynchronous IMAP4-specific operation */
	InvokeAsyncImap4PopulateCommandL(aCommand, *iMailSelection, 
			aImapServiceId,aDownloadLimits,aFolderId);
	
	}



/**
  Function : IsWithSelection
  Description : This function checks for the commands which require seperate mail selection 
  @param :	aImapCmd		The Imap command to be executed.			
  @return : ETrue if the command requires a selection.Else EFalse
*/
TBool CT_MsgImapInvokeAsyncFunction::IsWithSelection(TImap4Cmds aImapCmd)
	{
	if( aImapCmd == KIMAP4MTMConnectAndPopulateMailSelectionAndDisconnect ||
		aImapCmd == KIMAP4MTMConnectAndPopulateMailSelectionAndStayOnline ||
		aImapCmd == KIMAP4MTMPopulate ||
		aImapCmd == KIMAP4MTMPopulateMailSelectionWhenAlreadyConnected
	  )
		{
		return ETrue;
		}
	else
		{
		return EFalse;
		}
	}



/**
  Function : InvokeAsyncImap4PopulateCommandL
  Description : This function retrieves the download limits set
  @param :	aCommand		The Imap command to be executed.
			aImapServiceId	The Imap service Id
			aMailSelection	The selection object
			aDownloadLimits	The download limts set.		
  @return : none
*/
void CT_MsgImapInvokeAsyncFunction::InvokeAsyncImap4PopulateCommandL( TImap4Cmds aCommand,
										    CMsvEntrySelection& aMailSelection, TMsvId aImapServiceId , TBool aDownloadLimits, TMsvId afolderId)
	{
	
	/* Loads the Imap account settings and saves the download limits */
	CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();	
	CImImap4Settings* imapSettings =  new(ELeave) CImImap4Settings();
	CleanupStack::PushL(imapSettings);

	TImapAccount imapAccount;
	emailAccounts->GetImapAccountL(aImapServiceId, imapAccount);
	emailAccounts->LoadImapSettingsL(imapAccount, *imapSettings);


	/* If full download limts are set load it onto the the account */
	CT_MsgActive&	active=Active();
	if(aDownloadLimits)
		{
		TImImap4GetMailInfo imapFullinfo;
		imapFullinfo.iDestinationFolder = afolderId;
		imapFullinfo.iGetMailBodyParts = imapSettings->GetMailOptions();
		imapFullinfo.iMaxEmailSize = imapSettings->MaxEmailSize();
	
		// Package of the download limits
		TPckgBuf<TImImap4GetMailInfo> fullPack(imapFullinfo);

		iSharedDataIMAP.iMtm->SwitchCurrentEntryL(aImapServiceId);
		// Invokes asynchronous IMAP4-specific operation 
		iOperation = iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(aCommand,aMailSelection, fullPack, active.iStatus);	
		}
	/* If partial download limts are set load it onto the the account */
	else 
		{
		TImImap4GetPartialMailInfo imapPartialinfo;
		imapPartialinfo.iAttachmentSizeLimit = imapSettings->AttachmentSizeLimit();
		imapPartialinfo.iBodyTextSizeLimit = imapSettings->BodyTextSizeLimit();
		imapPartialinfo.iDestinationFolder = afolderId;
		imapPartialinfo.iPartialMailOptions = imapSettings->PartialMailOptions();
		imapPartialinfo.iTotalSizeLimit = imapSettings->MaxEmailSize();
	
		/* Package of the download limits */
		TPckgBuf<TImImap4GetPartialMailInfo> partialPack(imapPartialinfo);
		
		/* Switches the cuurent context on to the IMAP service ID */
		iSharedDataIMAP.iMtm->SwitchCurrentEntryL(aImapServiceId);
		
		/* Invokes asynchronous IMAP4-specific operation */
		iOperation = iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(aCommand,aMailSelection, partialPack, active.iStatus);
		}

	active.Activate();
	CActiveScheduler::Start();
	User::LeaveIfError(active.Result());

	CleanupStack::PopAndDestroy(2,emailAccounts); /*imapSettings,emailAccounts*/
	}
