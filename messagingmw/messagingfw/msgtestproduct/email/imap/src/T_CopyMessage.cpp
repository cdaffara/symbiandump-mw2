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
// CopyMessage
// [Paramaters]
// AccountName			:			Name of the Imap account 
// SrcFolderName		:			Source Folder
// TargetFolderName		:			Destination Folder
// SrcLookUpIn			:			Folder(LOCAL/REMOTE) under which the source folder exists
// TargetLookUpIn		:			Folder(LOCAL/REMOTE) under which the destination folder exists
// Subject				:			Subject of the message to be used
// ExpectedErr			:			Expected Error (for Negative Testing)
// Looks up for the message in "srcFolder" under "SrcLookUp"
// Copies the Message to "trgtFolder" under "TrgtLookUp"
// [APIs Used]
// CMsvEntry::CopyL()
// 
//



//User Includes
#include "T_CopyMessage.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>

// Literals Used
_LIT(KSrcFolderName,"SrcFolderName");
_LIT(KSrcParentFolderName,"SrcParentFolderName");
_LIT(KTargetFolderName, "TargetFolderName");
_LIT(KTargetParentFolderName, "TargetParentFolderName");
_LIT(KSrcLookUpIn, "SrcLookUpIn");
_LIT(KTrgtLookUpIn, "TargetLookUpIn");
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KSubject,"Subject");
_LIT(KExpectedErr, "ExpectedErr");

/**
  Function : CT_MsgCopyMsg
  Description : Constructor
  @internalTechnology
  @param :
  @return : N/A
*/
CT_MsgCopyMsg::CT_MsgCopyMsg(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,	iOperation(NULL)
	{
	SetTestStepName(KCopyMessage);
	}

	
	
/**
  Function : ~CT_MsgCopyMsg
  Description : Destructor
  @internalTechnology
  @param :
  @return : N/A
*/
CT_MsgCopyMsg::~CT_MsgCopyMsg()
	{
	delete iOperation;
	}


/**
  Function : NewL
  Description : Creates an object of CT_MsgCopyMsg 				 
  @internalTechnology
  @param :
  @return : N/A
*/
CT_MsgCopyMsg* CT_MsgCopyMsg::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgCopyMsg* self = new(ELeave) CT_MsgCopyMsg(aSharedDataIMAP);
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
void CT_MsgCopyMsg::ProgressL(TBool bFinal)
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
void CT_MsgCopyMsg::CancelStep()
	{
	iOperation->Cancel();
	}

/**
  Function : doTestStepL
  Description :Looks up for the message in "srcFolder" under "SrcLookUp",
  				Copies the Message to "trgtFolder" under "TrgtLookUp"
  @internalTechnology
  @param :
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgCopyMsg::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : CopyMessage"));
	if(TestStepResult() == EPass)
		{
		if(ReadIni())
			{
			TMsvId folderId;
			TInt err = KErrNone;
			if(iSrcLookupIn.Compare(_L("REMOTE")) == 0)
				{
				// Get the Id of the remote folder
				folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,
														iImapAccountName,iSrcfolderName,iSrcParentFolderName);
				}
			else
				{
				if(iSrcLookupIn.Compare(_L("LOCAL"))==0)
					{
					TRAP(err, folderId = CT_MsgUtils::FindEntryByNameL(iSharedDataIMAP.iSession, KMsvLocalServiceIndexEntryId,iSrcfolderName,iSrcParentFolderName));
					//TRAP(err,folderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(iSrcfolderName));
					}
				else
					{
					ERR_PRINTF1(_L("Wrong Input"));
					SetTestStepResult(EFail);
					}
				}
			
			//trgtfolderId lookup
			
			TMsvId targetId;
			if(iTrgtLookupIn.Compare(_L("REMOTE")) == 0)
				{
				// Get the Id of the remote folder
				TRAP(err, targetId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,
														iImapAccountName,iTrgtfolderName,iTrgtParentFolderName));
				}
			else
				{
				if(iTrgtLookupIn.Compare(_L("LOCAL"))==0)
					{
					TRAP(err, targetId = CT_MsgUtils::FindEntryByNameL(iSharedDataIMAP.iSession, KMsvLocalServiceIndexEntryId,iTrgtfolderName,iTrgtParentFolderName));
					}
				else
					{
					ERR_PRINTF1(_L("Wrong Input"));
					SetTestStepResult(EFail);
					}
				}
		
			INFO_PRINTF2(_L(" The trgtfolder Id is %d"),targetId);

			if(targetId == KMsvNullIndexEntryId)
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
					ERR_PRINTF1(_L("The given message is not found"));
												
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
					TRAP(err, iOperation = entry->CopyL(messageId, targetId, active.iStatus));

					if((iExpectedErr != KErrNone)||(err != KErrNone))
						{
						if(err == iExpectedErr)
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

					if (err == KErrNone)
						{
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
  Description : Reads inputs from Ini and returns EFalse if any one of it is not read.
  @internalTechnology
  @param :
  @return : TBool
*/
TBool CT_MsgCopyMsg::ReadIni()
	{
	TBool result = ETrue;
	
	TPtrC subject;
	if( !GetStringFromConfig(ConfigSection(),KSubject,subject))
		{
		ERR_PRINTF1(_L("The subject of the message is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}	
	iSubject.Set(subject);
	
		// Read the name of trgt Lookup Folder from the ini file

	TPtrC trgtLookUpIn;
	if(!GetStringFromConfig( ConfigSection(), KTrgtLookUpIn, trgtLookUpIn))
		{
		ERR_PRINTF1(_L("LookUpFolder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iTrgtLookupIn.Set(trgtLookUpIn);
	
		// Read the name of src Lookup Folder from the ini file
	TPtrC srcLookupIn;
	if(!GetStringFromConfig( ConfigSection(), KSrcLookUpIn, srcLookupIn))
		{
		ERR_PRINTF1(_L("LookUpFolder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iSrcLookupIn.Set(srcLookupIn);
	
		// Read the name of the source folder from the ini file
	TPtrC srcfolderName;
	if(!GetStringFromConfig( ConfigSection(), KSrcFolderName, srcfolderName))
		{
		ERR_PRINTF1(_L("Src Folder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iSrcfolderName.Set(srcfolderName);
	
	TPtrC srcParentFolderName;
	if(!GetStringFromConfig( ConfigSection(), KSrcParentFolderName, srcParentFolderName))
		{
		ERR_PRINTF1(_L("Src Parent Folder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iSrcParentFolderName.Set(srcParentFolderName);
	
	// Read the name of the source folder from the ini file
	TPtrC trgtfolderName;
	if(!GetStringFromConfig( ConfigSection(), KTargetFolderName, trgtfolderName))
		{
		ERR_PRINTF1(_L("Trgt Folder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iTrgtfolderName.Set(trgtfolderName);
	
		// Read the name of the source folder from the ini file
	TPtrC trgtParentFolderName;
	if(!GetStringFromConfig( ConfigSection(), KTargetParentFolderName, trgtParentFolderName))
		{
		ERR_PRINTF1(_L("Trgt Parent Folder name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iTrgtParentFolderName.Set(trgtParentFolderName);
	
	// Read the IMAP account name from the ini file
	TPtrC imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	iImapAccountName.Set(imapAccountName);
	
	TInt expectedErr = 0;
	if(!GetIntFromConfig( ConfigSection(), KExpectedErr, expectedErr))
		{
		INFO_PRINTF1(_L("Expected Err is not specified"));
		}
	iExpectedErr = expectedErr;
	
	return result;
	}
