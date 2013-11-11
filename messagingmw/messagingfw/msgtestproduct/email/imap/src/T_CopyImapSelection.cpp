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
// CopyImapSelection
// [Paramaters]
// ImapAccountName				:	Name of the Imap account
// FolderName					:   Name of the remote folder whose entries needs to be copied
// TargetFolderName				:   Name of the target folder to which the entries need to be copied 				
// [Action Description]
// Copies the entries to the target.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::CopyL
// 
//



/* User includes*/
#include "T_CopyImapSelection.h"
#include <t_utilscentralrepository.h>
#include <t_utils.h>
#include <t_utilsenumconverter.h>


/* Literals Used */
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KFolderName,"FolderName");
_LIT(KTargetFolderName,"TargetFolderName");


/**
  Function : CT_MsgCopyImapSelection
  Description : Constructor
  @param :	aSharedDataIMAP		Reference to CT_MsgSharedDataImap 
  @return : N/A
*/
CT_MsgCopyImapSelection::CT_MsgCopyImapSelection(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iOperation(NULL)
	{
	SetTestStepName(KCopyImapSelection);
	}



/**
  Function : NewL
  Description : Creates an object of CT_MsgCopyImapSelection 				 
  @param :	aSharedDataIMAP		Reference to CT_MsgSharedDataImap
  @return : A pointer of type CT_MsgCopyImapSelection
*/
CT_MsgCopyImapSelection* CT_MsgCopyImapSelection::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgCopyImapSelection* self = new(ELeave) CT_MsgCopyImapSelection(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					
	CleanupStack::Pop(self);
	return self;
	}	


/**
  Function : ~CT_MsgCopyImapSelection
  Description : Destructor
  @return : N/A
*/
CT_MsgCopyImapSelection::~CT_MsgCopyImapSelection()
	{
	delete iOperation;
	iOperation=NULL;
	}

/**
  Function : ProgressL
  Description : Displays the status of the asynchronous operation
  @param :	bFinal		Decides the status of the operation
  @return : none
*/
void CT_MsgCopyImapSelection::ProgressL(TBool bFinal)
	{
	//	TODO
	if ( bFinal )
		{
		/*	Display final progress iOperation*/
		}
	else
		{
		/*	Display current progress iOperation*/
		}
	}



/**
  Function : CancelStep
  Description : Cancels the asynchronous operation
  @return : none
*/
void CT_MsgCopyImapSelection::CancelStep()
	{
	iOperation->Cancel();
	}



/**
  Function : doTestStepL
  Description : Connection with the IMAP server is already established in the base class.
				Reads the Imap account name and the remote folder name from the ini file
				It obtains a selection of the entires under the remote folder given.
				It copies these entries into the specified target folder.
  @return : TVerdict - Test step result
  @leave :	KMsvNullIndexEntryId	Invalid remote folder name specified
			KErrNotFound			Invalid target folder name specified
			KErrArgument			If there are no entries in the remote folder to be copied
*/
TVerdict CT_MsgCopyImapSelection::doTestStepL()
	{
	INFO_PRINTF1(_L("TestStep : CopyImapSelection"));

	/* Reads the IMAP account name from the ini file */
	TPtrC	imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("ImapAccount Name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		/* Retrieves the Imap service Id for the given Imap account*/
		TMsvId	imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
		INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId );

		if(imapServiceId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid IMAP account name specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			/* Reads the name of the remote folder from the ini file */
			TPtrC	folderName;
			if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
				{
				ERR_PRINTF1(_L("Folder name is not specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				/* Gets the Id of the remote folder whose contents need to be copied */
				TMsvId	folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,
													imapAccountName,folderName);
				INFO_PRINTF2(_L(" The folder Id is %d"),folderId);

				if( folderId == KMsvNullIndexEntryId)
					{
					ERR_PRINTF1(_L("Invalid remote folder name specified"));
					SetTestStepResult(EFail);
					}
				else
					{								
					/* Gets a selection of the children entries for the remote folder */
					CMsvEntrySelection*	selection =  CT_MsgUtils::CreateChildrenSelectionRemoteFolderL(
														iSharedDataIMAP.iSession,
														imapAccountName, folderName);
					CleanupStack::PushL(selection);

					/* Reads the name of the target folder to where the entries needs to be copied */
					TPtrC targetFolderName;
					if(!GetStringFromConfig( ConfigSection(), KTargetFolderName, targetFolderName))
						{
						ERR_PRINTF1(_L("Target Folder is not specified"));
						SetTestStepResult(EFail);
						}
					else
						{
						/* Retrieves the folder Id based on the local folder name read from the ini file */
						TMsvId	targetFolderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(targetFolderName);

						if ( targetFolderId == KErrNotFound )
							{
							ERR_PRINTF1(_L("Invalid target folder name specified"));
							SetTestStepResult(EFail);
							}
						else
							{
							/* 
							 * Checks if there are entries under the remote folder
							 * If there are no entries then leaves with KErrArgument
							 */
							TInt count = selection->Count();
							if (count == 0)
								{
								User::Leave(KErrArgument);
								}
							/* Fetches the Id of the first entry */
							TMsvId	entryId = (*selection)[0];

							/* Creates a new CMsvEntry for the specified entry ID.*/
							CMsvEntry*	entry = CMsvEntry::NewL(*iSharedDataIMAP.iSession,entryId,TMsvSelectionOrdering());
							CleanupStack::PushL(entry);

							/* Sets the context to the parent of the specified entry */
							entry->SetEntryL(entryId);
							entry->SetEntryL(entry->Entry().Parent());

							CT_MsgActive&	active=Active();
							
							/* Performs a copy of the entries to the target folder*/
							iOperation = entry->CopyL(*selection,targetFolderId, active.iStatus);

							active.Activate();
							CActiveScheduler::Start();
							CleanupStack::PopAndDestroy(entry);	/* operation,entry */
							delete iOperation;
							iOperation=NULL;
							
							/* Retrieved the result of the asynchronous operation */
							User::LeaveIfError(active.Result());
							}
						}
					CleanupStack::PopAndDestroy(selection);
					}
				}
			}
		}
	return TestStepResult();
	}
