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
// DeleteAllMessage
// [Paramaters]
// AccountName			:			Name of the Imap account 
// FolderName			:			Source Folder
// LookUpIn				:			Folder(LOCAL/REMOTE) under which the source folder exists
// ExpectedErr			:			Expected Error (for Negative Testing)
// Deletes all Messages in a specified folder 
// (Messages found in "FolderName" under "LookUp")
// [APIs Used]
// none
// 
//



//User Includes
#include "T_DeleteAllMessage.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>

// Literals Used
_LIT(KFolderName,"FolderName");
_LIT(KParentFolderName,"ParentFolderName");
_LIT(KLookUpIn, "LookUpIn");
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KExpectedErr, "ExpectedErr");

/**
  Function : CT_MsgDeleteAllMsg
  Description : Constructor
  @param :CT_MsgSharedDataImap&
  @return : N/A
*/
CT_MsgDeleteAllMsg::CT_MsgDeleteAllMsg(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iOperation(NULL)
, iEntrySelection(NULL)
, iMsgEntry(NULL)
	{
	SetTestStepName(KDeleteAllMessage);
	}

	
	
/**
  Function : ~CT_MsgDeleteAllMsg
  Description : Destructor
  @param :
  @return : N/A
*/
CT_MsgDeleteAllMsg::~CT_MsgDeleteAllMsg()
	{
	delete iOperation;
	iOperation = NULL;
	
	delete iMsgEntry;
	iMsgEntry = NULL;
   
    delete iEntrySelection;
	iEntrySelection = NULL;	
	}


/**
  Function : NewL
  Description : Creates an object of CT_MsgDeleteAllMsg
  @param :CT_MsgSharedDataImap&
  @return : CT_MsgDeleteAllMsg*
*/
CT_MsgDeleteAllMsg* CT_MsgDeleteAllMsg::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgDeleteAllMsg* self = new(ELeave) CT_MsgDeleteAllMsg(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 // Call CT_AsyncStep::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : ProgressL
  Description : stub
  @return : void
*/
void CT_MsgDeleteAllMsg::ProgressL(TBool bFinal)
	{

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
  @return : void
*/
void CT_MsgDeleteAllMsg::CancelStep()
	{
	iOperation->Cancel();
	}

/**
  Function : doTestStepL
  Description : Deletes Specified Message
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgDeleteAllMsg::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : DeleteAllMessage"));
	if(TestStepResult() == EPass)
		{
		if(ReadIni())
			{
			TMsvId folderId;
			TInt err =0;
			if(iLookUpIn.Compare(_L("REMOTE")) == 0)
				{
				// Get the Id of the remote folder
				TRAP(err, folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,
														iImapAccountName,iFolderName,iParentFolderName));
				}
			else
				{
				if(iLookUpIn.Compare(_L("LOCAL"))==0)
					{
					TRAP(err, folderId = CT_MsgUtils::FindEntryByNameL(iSharedDataIMAP.iSession, KMsvLocalServiceIndexEntryId,iFolderName,iParentFolderName));
					}
				else
					{
					ERR_PRINTF1(_L("Wrong Input"));
					SetTestStepResult(EFail);
					}
				}
		
			INFO_PRINTF2(_L(" The folder Id is %d"),folderId);

			if(folderId == KMsvNullIndexEntryId)
				{
				ERR_PRINTF1(_L("Invalid folder name specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataIMAP.iSession,folderId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
				CleanupStack::PushL(entry);

				entry->SetEntryL(folderId);	
				TMsvId messageId = KMsvNullIndexEntryId;
			
				//retrieve all children from the  parent entry
			
 				iEntrySelection=NULL;
				iEntrySelection=entry->ChildrenL();
				TInt count = iEntrySelection->Count();
  				for(TInt i=0;i<count;++i)
  					{
					delete iMsgEntry;
					iMsgEntry = NULL;
					messageId = (*iEntrySelection)[i];
					iMsgEntry = CMsvEntry::NewL(*iSharedDataIMAP.iSession,messageId ,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
					iMsgEntry->SetEntryL((*iEntrySelection)[i]);
					TMsvEntry entryMsg = iMsgEntry->Entry();
					//only delete a message
					if (entryMsg.iType==KUidMsvMessageEntry )
						{
						INFO_PRINTF2(_L("The Message Id is %d"),messageId );
						CT_MsgActive&	active=Active();
						delete iOperation;
						iOperation = NULL;
						iOperation = entry->DeleteL(messageId,active.iStatus);
				
						active.Activate();
						CActiveScheduler::Start();
						User::LeaveIfError(active.Result());
					
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
  Description : Reads in data from ini file
  @return : TBool
  */
TBool CT_MsgDeleteAllMsg::ReadIni()
	{
	TBool result = ETrue;
	
	// Read the name of the remote folder from the ini file
	TPtrC folderName;
	if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iFolderName.Set(folderName);
	
	// Read the Parent name of the remote folder from the ini file
	TPtrC parFolderName;
	if(!GetStringFromConfig( ConfigSection(), KParentFolderName, parFolderName))
		{
		ERR_PRINTF1(_L("Parent Folder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iParentFolderName.Set(parFolderName);
	
	// Read the IMAP account name from the ini file
	TPtrC imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iImapAccountName.Set(imapAccountName);

	// Read the name of Lookup Folder from the ini file
	TPtrC lookupIn;
	if(!GetStringFromConfig( ConfigSection(), KLookUpIn, lookupIn))
		{
		ERR_PRINTF1(_L("LookUpFolder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iLookUpIn.Set(lookupIn);
	

	TInt expectedErr = 0;
	if(!GetIntFromConfig( ConfigSection(), KExpectedErr, expectedErr))
		{
		ERR_PRINTF1(_L("Expected Err is not specified"));
		SetTestStepResult(EFail);
		}
	iExpectedErr = expectedErr;
	
	return result;
	}
