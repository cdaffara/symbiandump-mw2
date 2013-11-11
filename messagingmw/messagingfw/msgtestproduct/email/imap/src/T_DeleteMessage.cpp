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
// DeleteMessage
// [Paramaters]
// AccountName			:			Name of the Imap account 
// FolderName			:			Source Folder
// LookUpIn				:			Folder(LOCAL/REMOTE) under which the source folder exists
// Subject				:			Subject of the message to be used
// ExpectedErr			:			Expected Error (for Negative Testing)
// Looks up for the message in "FolderName" under "LookUp" and the deletes the message.
// 
//

//User Includes
#include "T_DeleteMessage.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>

// Literals Used
_LIT(KFolderName,"FolderName");
_LIT(KParentFolderName,"ParentFolderName");
_LIT(KLookUpIn, "LookUpIn");
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KSubject,"Subject");
_LIT(KExpectedErr, "ExpectedErr");

/**
  Function : CT_MsgDeleteMsg
  Description : Constructor
  @internalTechnology
  @param :
  @return : N/A
*/
CT_MsgDeleteMsg::CT_MsgDeleteMsg(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iOperation(NULL)
	{
	SetTestStepName(KDeleteMessage);
	}

	
	
/**
  Function : ~CT_MsgDeleteMsg
  Description : Destructor
  @internalTechnology
  @param :
  @return : N/A
*/
CT_MsgDeleteMsg::~CT_MsgDeleteMsg()
	{
	delete iOperation;
	iOperation = NULL;
	}


/**
  Function : NewL
  Description : Creates an object of CT_MsgDeleteMsg
  @internalTechnology
  @param :
  @return : N/A
*/
CT_MsgDeleteMsg* CT_MsgDeleteMsg::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgDeleteMsg* self = new(ELeave) CT_MsgDeleteMsg(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 // Call CT_AsyncStep::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : ProgressL
  Description : Displays the status of the asynchronous operation
  @return : N/A
*/
void CT_MsgDeleteMsg::ProgressL(TBool bFinal)
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
void CT_MsgDeleteMsg::CancelStep()
	{
	iOperation->Cancel();
	}

/**
  Function : doTestStepL
  Description : Deletes Specified Message
  @internalTechnology
  @param :
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgDeleteMsg::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : DeleteMessage"));
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
					//TRAP(err, folderId = CT_MsgUtils::FindEntryByNameL(iSharedDataIMAP.iSession, KMsvLocalServiceIndexEntryId,iFolderName,iParentFolderName));
					folderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(iFolderName);
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

			
				// Retrieves the message Id based ont he message subject from the given local folder
				TMsvId messageId = KMsvNullIndexEntryId;
				TRAP(err,messageId = CT_MsgUtils::SearchMessageBySubjectL(iSharedDataIMAP.iSession, folderId , iSubject));
				if( messageId == KMsvNullIndexEntryId)
					{
					INFO_PRINTF1(_L("The given message is not found"));
										
					if((iExpectedErr != KErrNone)||(err != KErrNone))
						{
						if(err == iExpectedErr )
							{
							INFO_PRINTF1(_L("Failed with Expected Error"));
							SetTestStepResult(EPass);
							}
						else
							{
							ERR_PRINTF2(_L("Failed with %d error"),err);
							SetTestStepResult(EFail);
							}
						}
					}
				// Message found	
				else
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
				
				CleanupStack::PopAndDestroy(entry);
				}
			}
		}
	return TestStepResult();
	}

TBool CT_MsgDeleteMsg::ReadIni()
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
	
	TPtrC subject;
	if( !GetStringFromConfig(ConfigSection(),KSubject,subject ))
		{
		ERR_PRINTF1(_L("The subject of the message is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iSubject.Set(subject);

	TInt expectedErr = 0;
	if(!GetIntFromConfig( ConfigSection(), KExpectedErr, expectedErr))
		{
		ERR_PRINTF1(_L("Expected Err is not specified"));
		SetTestStepResult(EFail);
		}
	iExpectedErr = expectedErr;
	
	return result;
	}
