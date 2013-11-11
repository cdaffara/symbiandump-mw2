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
// ImapCopyAsyncFunction
// [Paramaters]
// ImapAccountName			: Name of the Imap account
// CommandName				: Name of the Imap command to be executed
// SourceLookup				: Name of the source look up 
// SourceFolderName			: Name of the source folder
// SourceParentName			: Name of the parent of the source folder
// TargetLookup 			: Name of the target look up
// TargetFolderName			: Name of the target folder
// TargetParentName			: Name of the parent of the target folder
// DownloadLimits			: The download limits,1 - Full, 0 - Partial
// This test step invokes the copy commands of IMAP client mtm for 
// copying the message from source folder to target folder
// [APIs Used]
// CMsvEntrySelection::AppendL()
// CMsvEntrySelection::ResizeL()
// CImap4ClientMtm::InvokeAsyncFunctionL()
// 
//

// User includes 
#include "T_IMAP4CopyAsyncFunction.h"
#include <t_utilscentralrepository.h>
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include "T_UtilsCompareEmailMessages.h"


// Epoc includes 
#include <e32cmn.h>
#include <imapset.h>
#include <cemailaccounts.h>


// Literals Used 
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KCommandName,"CommandName");

_LIT(KSourceLookup, "SourceLookup");
_LIT(KTargetLookup, "TargetLookup");

_LIT(KSourceFolderName,"SourceFolderName");
_LIT(KSourceParentName,"SourceParentName");
_LIT(KTargetFolderName, "TargetFolderName");
_LIT(KTargetParentName, "TargetParentName");

_LIT(KDownloadLimits,"DownloadLimits");


/**
  Function 		: CT_MsgImapCopyAsyncFunction
  Description 	: Constructor
  @param 		: aSharedDataIMAP		Reference to CT_MsgSharedDataImap
  @return 		: N/A
*/
CT_MsgImapCopyAsyncFunction::CT_MsgImapCopyAsyncFunction(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iOperation(NULL)
,	iMailSelection(NULL)
,	iRecvMsgSelectionList(NULL)
	{
	SetTestStepName(KImap4CopyAsyncFunction);
	}


/**
  Function 		: NewL
  Description 	: Creates an object of CT_MsgImapCopyAsyncFunction 				 
  @return 		: N/A
*/
CT_MsgImapCopyAsyncFunction* CT_MsgImapCopyAsyncFunction::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgImapCopyAsyncFunction* self = new(ELeave) CT_MsgImapCopyAsyncFunction(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 
	CleanupStack::Pop(self);
	return self;
	}	

	
/**
  Function 		: ~CT_MsgImapCopyAsyncFunction
  Description 	: Destructor
  @return 		: N/A
*/
CT_MsgImapCopyAsyncFunction::~CT_MsgImapCopyAsyncFunction()
	{
	delete iOperation;
	iOperation = NULL;

	delete iMailSelection;
	iMailSelection = NULL;

	delete iRecvMsgSelectionList;
	iRecvMsgSelectionList = NULL;
	}

/**
  Function 		: ProgressL
  Description 	: Displays the status of the asynchronous operation
  @return 		: N/A
*/
void CT_MsgImapCopyAsyncFunction::ProgressL(TBool /*bFinal*/)
	{}


/**
  Function 		: CancelStep
  Description 	: Cancels the asynchronous operation
  @return 		: N/A
*/
void CT_MsgImapCopyAsyncFunction::CancelStep()
	{
	iOperation->Cancel();
	}


/**
  Function 		: doTestStepL
  Description 	: Reads the Imap account name, folder names, download limts and the command name from 
				  the ini file. A selection of the entries uner the remote folder is obtained and passed to 
				  InvokeAsyncFunctionL based on the command.
  @return 		: TVerdict - Test step result
  @leave  		: KMsvNullIndexEntryId		Invalid IMAP account name specified or
											Invalid folder names specified
*/
TVerdict CT_MsgImapCopyAsyncFunction::doTestStepL()
	{
	INFO_PRINTF1(_L("TestStep: CopyAsyncFunction"));

	// Read the IMAP account name from the ini file
	TPtrC imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("ImapAccount Name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		// Retrieves the Imap service Id for the given Imap account
		TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
		INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId );

		// Fails the test step if an invalid IMAP account is specified 
		if(imapServiceId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid IMAP account name specified"));
			SetTestStepResult(EFail);
			}
		else
			{
						
			// Read the name of the source parent folder from the ini file
			TPtrC srcParentFolderName;
			if(!GetStringFromConfig( ConfigSection(), KSourceParentName, srcParentFolderName))
				{
				ERR_PRINTF1(_L("Src Parent Folder name is not specified"));
				SetTestStepResult(EFail);
				}		
			// Read the name of the source folder from the ini file
			TPtrC srcFolderName;
			if(!GetStringFromConfig( ConfigSection(), KSourceFolderName, srcFolderName))
				{
				ERR_PRINTF1(_L("Folder name is not specified"));
				SetTestStepResult(EFail);
				}		
			// Read the name of the target parent folder from the ini file
			TPtrC trgtParentFolderName;
			if(!GetStringFromConfig( ConfigSection(), KTargetParentName, trgtParentFolderName))
				{
				ERR_PRINTF1(_L("Trgt Parent Folder name is not specified"));
				SetTestStepResult(EFail);
				}
			// Read the name of the target folder from the ini file
			TPtrC trgtFolderName;
			if(!GetStringFromConfig( ConfigSection(), KTargetFolderName, trgtFolderName))
				{
				ERR_PRINTF1(_L("Folder name is not specified"));
				SetTestStepResult(EFail);
				}			
			else
				{
				TInt err = KErrNone;
				// Read the name of source Lookup Folder from the ini file
				TPtrC srcLookupIn;
				if(!GetStringFromConfig( ConfigSection(), KSourceLookup, srcLookupIn))
					{
					ERR_PRINTF1(_L("LookUpFolder name is not specified"));
					SetTestStepResult(EFail);
					}
				// Lookup the source folder
				TMsvId srcFolderId;
				if(srcLookupIn.Compare(_L("REMOTE")) == 0)
					{
					// Get the Id of the remote folder
					TRAP(err,srcFolderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,
														imapAccountName,srcFolderName,srcParentFolderName));
					}
				else
					{
					if(srcLookupIn.Compare(_L("LOCAL"))==0)
						{
						// Get the Id of the local folder
						TRAP(err,srcFolderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(srcFolderName));
						}
					else
						{
						ERR_PRINTF1(_L("Wrong Input"));
						SetTestStepResult(EFail);
						}
					}
					
				INFO_PRINTF2(_L(" The srcfolder Id is %d"),srcFolderId);

				// Read the name of target Lookup Folder from the ini file
				TPtrC trgtLookup;
				if(!GetStringFromConfig( ConfigSection(), KTargetLookup, trgtLookup))
					{
					ERR_PRINTF1(_L("LookUpFolder name is not specified"));
					SetTestStepResult(EFail);
					}
				
				// Lookup the target folder
				TMsvId trgtFolderId;
				if(trgtLookup.Compare(_L("REMOTE")) == 0)
					{
					// Get the Id of the remote folder
					TRAP(err,trgtFolderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,
										imapAccountName,trgtFolderName,trgtParentFolderName));
					}
				else
					{
					if(trgtLookup.Compare(_L("LOCAL"))==0)
						{
						// Get the Id of the local folder
						TRAP(err,trgtFolderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(trgtFolderName));
						}
					else
						{
						ERR_PRINTF1(_L("Wrong Input"));
						SetTestStepResult(EFail);
						}
					}

				INFO_PRINTF2(_L(" The trgtfolder Id is %d"),trgtFolderId);
			

				if(trgtFolderId == KMsvNullIndexEntryId)
					{
					ERR_PRINTF1(_L("Invalid remote folder name specified"));
					SetTestStepResult(EFail);
					}
				else
					{
					INFO_PRINTF2(_L(" The folder Id is %d"),trgtFolderId);

					// Reads the download limits set 
					TBool	downloadLimits;
					if(!GetBoolFromConfig( ConfigSection(),KDownloadLimits,downloadLimits))
						{
						ERR_PRINTF1(_L("DownloadLimits is not specified"));
						SetTestStepResult(EFail);
						}
					else
						{	
						// Reads the IMAP command to be executed from the ini file
						TPtrC commandName;
						if(!GetStringFromConfig( ConfigSection(), KCommandName, commandName))
							{
							ERR_PRINTF1(_L("Command name is not specified"));
							SetTestStepResult(EFail);
							}
						else
							{		
							// Retrieves the IMAP command to be executed based on the comman name 
							TImap4Cmds command = CT_MsgUtilsEnumConverter::ConvertDesToTImap4Cmds((TDesC&) commandName);									

							
							// Based on the command decide whether a selection object needs to be passed 
							TBool isWithSelection = EFalse;
							isWithSelection = IsWithSelection(command);
							if(isWithSelection)
								{
								if(srcLookupIn.Compare(_L("REMOTE")) == 0)
									{
									// Gets a selection of the children entries for the remote folder
									iRecvMsgSelectionList =  CT_MsgUtils::CreateChildrenSelectionRemoteFolderL(iSharedDataIMAP.iSession,
												imapAccountName,srcFolderName);	
									}
								if(srcLookupIn.Compare(_L("LOCAL"))==0)
									{
									iRecvMsgSelectionList =  CT_MsgUtils::CreateChildrenSelectionL(iSharedDataIMAP.iSession,srcFolderId);
									}
												
							
								ImapCopyAsyncFunctionWithSelectionL(command,imapServiceId,*iRecvMsgSelectionList,downloadLimits,trgtFolderId);
								}
							else
								{
								ImapCopyAsyncFunctionWithoutSelectionL(command,imapServiceId,srcFolderId, trgtFolderId,downloadLimits);
								// Gets a selection of the children entries for the remote folder 
								}
							}
						}
					}
				}
			}
		}
	return TestStepResult();
	}
												
			

/**
  Function 		: ImapCopyAsyncFunctionWithSelection
  Description 	: Calls CopyAsyncImap4CommandL after creating a selection object
				  with all the children entries appended to the selection object
  @param 		: aCommand			The Imap command to be executed.
				  aImapServiceId	The Imap service Id
				  aMailSelection	The selection object
				  aDownloadLimits	The download limts set.
				  aFolderId			The folder Id
  @return 		: none
*/
void CT_MsgImapCopyAsyncFunction::ImapCopyAsyncFunctionWithSelectionL(TImap4Cmds aCommand,TMsvId aImapServiceId,
									CMsvEntrySelection& aMailSelection, TBool aDownloadLimits,TMsvId aFolderId)
	{
	// Create a new selection object
	iMailSelection =  new (ELeave) CMsvEntrySelection;
	
	// The first entry in this selection MUST be the IMAP service 
	iMailSelection->InsertL(0, aImapServiceId);
		
	// Gets a count of the children entries of the remote folder
	TInt mailCount = aMailSelection.Count();

	// Gets the Ids of all the children entries and appends it to the new selection object
	for (TInt index=0; index< mailCount; index++)	
		{
		TMsvId msvId = aMailSelection.At(index);
		iMailSelection->AppendL(msvId);
		}
					
	// Function to Copy asynchronous IMAP4-specific operation
	CopyAsyncImap4CommandL(aCommand, *iMailSelection, aImapServiceId,
											aDownloadLimits,aFolderId);
	}
		


/**
  Function 		: ImapCopyAsyncFunctionWithoutSelection
  Description 	: Calls CopyAsyncImap4CommandL after creating a selection object
				  with all the children entries appended to the selection object
  @param 		: aCommand			The Imap command to be executed.
				  aImapServiceId	The Imap service Id
			   	  aDownloadLimits	The download limts set.
				  aFolderId			The folder Id
  @return 		: none
*/
void CT_MsgImapCopyAsyncFunction::ImapCopyAsyncFunctionWithoutSelectionL(TImap4Cmds aCommand,TMsvId aImapServiceId,
										TMsvId aSrcFolderId, TMsvId aTrgtFolderId, TBool aDownloadLimits)
	{

	/*
	 * Create a mail selection. The first entry in this selection MUST be 
	 * the service. 
	 */
	iMailSelection = new (ELeave) CMsvEntrySelection;
	
	iMailSelection->ResizeL(0);
	iMailSelection->InsertL(0,aImapServiceId);
	iMailSelection->InsertL(1,aSrcFolderId); 

	/* Function to Copy asynchronous IMAP4-specific operation */
	CopyAsyncImap4CommandL(aCommand, *iMailSelection, 
			aImapServiceId,aDownloadLimits,aTrgtFolderId);
	
	}



/**
  Function 		: IsWithSelection
  Description 	: This function checks for the commands which require seperate mail selection 
  @param 		: aImapCmd		The Imap command to be executed.			
  @return 		: ETrue if the command requires a selection.Else EFalse
*/
TBool CT_MsgImapCopyAsyncFunction::IsWithSelection(TImap4Cmds aImapCmd)
	{
	if( aImapCmd == KIMAP4MTMConnectAndCopyMailSelectionAndDisconnect ||
		aImapCmd == KIMAP4MTMConnectAndCopyMailSelectionAndStayOnline ||
		aImapCmd == KIMAP4MTMCopyMailSelectionWhenAlreadyConnected
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
  Function 		: CopyAsyncImap4CommandL
  Description 	: This function retrieves the download limits set
  @param 		: aCommand			The Imap command to be executed.
			 	  aImapServiceId	The Imap service Id
				  aMailSelection	The selection object
				  aDownloadLimits	The download limts set.		
  @return 		: none
*/
void CT_MsgImapCopyAsyncFunction::CopyAsyncImap4CommandL( TImap4Cmds aCommand,
										    CMsvEntrySelection& aMailSelection, TMsvId aImapServiceId , TBool aDownloadLimits, TMsvId afolderId)
	{
	
	// Loads the Imap account settings and saves the download limits
	CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();	
	CImImap4Settings* imapSettings =  new(ELeave) CImImap4Settings();
	CleanupStack::PushL(imapSettings);

	TImapAccount imapAccount;
	emailAccounts->GetImapAccountL(aImapServiceId, imapAccount);
	emailAccounts->LoadImapSettingsL(imapAccount, *imapSettings);


	// If full download limts are set load it onto the the account
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
		// Copy asynchronous IMAP4-specific operation 
		iOperation = iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(aCommand,aMailSelection, fullPack, active.iStatus);	
		}
	// If partial download limts are set load it onto the the account 
	else 
		{
		TImImap4GetPartialMailInfo imapPartialinfo;
		imapPartialinfo.iAttachmentSizeLimit = imapSettings->AttachmentSizeLimit();
		imapPartialinfo.iBodyTextSizeLimit = imapSettings->BodyTextSizeLimit();
		imapPartialinfo.iDestinationFolder = afolderId;
		imapPartialinfo.iPartialMailOptions = imapSettings->PartialMailOptions();
		imapPartialinfo.iTotalSizeLimit = imapSettings->MaxEmailSize();
	
		// Package of the download limits 
		TPckgBuf<TImImap4GetPartialMailInfo> partialPack(imapPartialinfo);
		
		// Switches the cuurent context on to the IMAP service ID 
		iSharedDataIMAP.iMtm->SwitchCurrentEntryL(aImapServiceId);
		
		// Copy asynchronous IMAP4-specific operation
		iOperation = iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(aCommand,aMailSelection, partialPack, active.iStatus);
		}

	active.Activate();
	CActiveScheduler::Start();
	User::LeaveIfError(active.Result());

	CleanupStack::PopAndDestroy(2,emailAccounts); // imapSettings,emailAccounts
	}
