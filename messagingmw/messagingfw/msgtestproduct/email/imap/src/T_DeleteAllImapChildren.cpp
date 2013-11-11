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
// DeleteAllImapChildren
// [Paramaters]
// FolderName			:			Name of the remote folder whose entries needs to be deleted
// AccountName			:			Name of the Imap account 
// Deletes all children entries for the specified folder under the remote Imap account
// [APIs Used]
// none
// 
//



/* User Includes*/
#include "T_DeleteAllImapChildren.h"
#include <t_utilsdeleteallchildren.h>
#include <t_utils.h>


/* epoc includes */
#include <barsc.h>
#include <bautils.h>
#include <msgs.rsg>


 /*Literals Used */
_LIT(KFolderName,"FolderName");
_LIT(KImapAccountName,"ImapAccountName");


/**
  Function : CT_MsgDeleteAllImapChildren
  Description : Constructor
  @param : aSharedDataIMAP		Reference to the CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgDeleteAllImapChildren::CT_MsgDeleteAllImapChildren(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iUtil(NULL)
	{
	SetTestStepName(KDeleteAllImapChildren);
	}


	
/**
  Function : NewL
  Description : Creates an object of CT_MsgDeleteAllImapChildren 				 
  @param : aSharedDataIMAP		Reference to the CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgDeleteAllImapChildren* CT_MsgDeleteAllImapChildren::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgDeleteAllImapChildren* self = new(ELeave) CT_MsgDeleteAllImapChildren(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 
	CleanupStack::Pop(self);
	return self;
	}

	
/**
  Function : ~CT_MsgDeleteAllImapChildren
  Description : Destructor
  @return : N/A
*/
CT_MsgDeleteAllImapChildren::~CT_MsgDeleteAllImapChildren()
	{
	if(NULL!=iUtil)
		{
		delete iUtil;
		iUtil=NULL;
		}
	}



/**
  Function : ProgressL
  Description : Displays the status of the asynchronous operation
  @param :	bFinal	Decides the status of the asynchronous operation
  @return : N/A
*/
void CT_MsgDeleteAllImapChildren::ProgressL(TBool bFinal)
	{
	//	TODO
	if ( bFinal )
		{
		/*	Display final progress iUtil*/
		}
	else
		{
		/*	Display current progress iUtil */
		}
	}



/**
  Function : CancelStep
  Description : Cancels the asynchronous operation
  @return : N/A
*/
void CT_MsgDeleteAllImapChildren::CancelStep()
	{
	//	TODO iUtil
	}


/**
  Function : doTestStepL
  Description : Reads the  Imap account name and the folder under that from the ini file.
				It obtains a selection of the enries under the specified folder and 
				calls theT_UtilsDeleteAllChildren to delete them
  @return : TVerdict - Test step result
  @leave : KMsvNullIndexEntryId		Invalid folder name specified	
*/
TVerdict CT_MsgDeleteAllImapChildren::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : DeleteAllImapChildren "));

	/* Reads the name of the remote folder from the ini file*/
	TPtrC folderName;
	if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		/* Reads the IMAP account name from the ini file */
		TPtrC imapAccountName;
		if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
			{
			ERR_PRINTF1(_L("Imap account name is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			/* Gets the Id of the remote folder based on the name specified */
			TMsvId folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,
													imapAccountName,folderName);	
			INFO_PRINTF2(_L(" The folder Id is %d"),folderId);

			/* Fails the test if an invalid folder name is specified */
			if(folderId == KMsvNullIndexEntryId)
				{
				ERR_PRINTF1(_L("Invalid folder name specified"));
				SetTestStepResult(EFail);
				}
			/* 
			 * Creates an object of the utility class CT_MsgUtilsDeleteAllChildren which
			 * deletes all the child entries based on the parent Id given 
			 */
			else
				{
				CT_MsgActive&	active=Active();
				if(iUtil==NULL)
				{
				iUtil= new (ELeave) CT_MsgUtilsDeleteAllChildren(iSharedDataIMAP.iSession, folderId);
				}

				/* Deletes all the entries under the remote folder */
				iUtil->StartL(active.iStatus);
				active.Activate();
				CActiveScheduler::Start();

				delete iUtil;
				iUtil=NULL;
				}
			}
		}
	return TestStepResult();
	}
