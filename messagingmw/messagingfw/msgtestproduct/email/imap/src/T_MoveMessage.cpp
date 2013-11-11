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
// MoveMessage
// [Paramaters]
// ImapAccountName		: Name of the IMAP account
// SrcFolderName		: Source folder name e.g. Inbox
// SrcParentFolderName 	: Source parent folder name e.g. ImapAccountName
// SrcLookUpIn			: Look up in either REMOTE or LOCAL
// TargetFolderName		: Target folder name e.g. Draft
// SrcParentFolderName 	: Target parent folder name e.g. ImapAccountName
// TargetLookUpIn		: Look up in either LOCAL or REMOTE
// Subject				: Message subject
// Move the message from the source folder to target folder
// 
//


// User includes
#include "T_MoveMessage.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>


// Literals used
_LIT(KSrcFolderName,"SrcFolderName");
_LIT(KSrcParentFolderName,"SrcParentFolderName");
_LIT(KTargetFolderName, "TargetFolderName");
_LIT(KTargetParentFolderName, "TargetParentFolderName");
_LIT(KSrcLookUpIn, "SrcLookUpIn");
_LIT(KTrgtLookUpIn, "TargetLookUpIn");
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KSubject,"Subject");
_LIT(KParentFolderName,"ParentFolderName");
_LIT(KExpectedErr, "ExpectedErr");


/**
  Function 		: CT_MsgMove
  Description 	: Constructor
  @param 		: aSharedDataIMAP	Reference to CT_MsgSharedDataImap
  @return 		: N/A
*/
CT_MsgMove::CT_MsgMove(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP),iOperation(NULL)
	{
	SetTestStepName(KMoveMessage);
	}

/**
  Function 		: ~CT_MsgMove
  Description 	: Destructor
  @return 		: N/A
*/
CT_MsgMove::~CT_MsgMove()
	{
	delete iOperation;
	iOperation=NULL;
	}

/**
  Function 		: NewL
  Description 	: Creates an object of CT_MsgMove
  @param 		: aSharedDataIMAP	Reference to CT_MsgSharedDataImap
  @return 		: N/A
*/
CT_MsgMove* CT_MsgMove::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgMove* self = new(ELeave) CT_MsgMove(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL();		// Call CT_MsgAsyncStepIMAP::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function 		: ProgressL
  Description 	: Displays the status of the asynchronous operation
  @param 		: bFinal	Variable of TBool
  @return 		: N/A
*/
void CT_MsgMove::ProgressL(TBool /*bFinal*/)
	{}

/**
  Function 		: CancelStep
  Description 	: Cancels the asynchronous operation
  @return 		: N/A
*/
void CT_MsgMove::CancelStep()
	{
	iOperation->Cancel();
	}

/**
  Function 		: doTestStepL
  Description 	: Reads the  Imap account name and the folder under that from the ini file.
				  It obtains a selection of the enries under the specified folder.
  @return 		: TVerdict - Test step result
*/
TVerdict CT_MsgMove::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: MoveMessage"));

	if(TestStepResult() == EPass)
		{
		// Read the name of the source parent folder from the ini file
		TPtrC srcParentFolderName;
		if(!GetStringFromConfig( ConfigSection(), KSrcParentFolderName, srcParentFolderName))
			{
			ERR_PRINTF1(_L("Src Parent Folder name is not specified"));
			SetTestStepResult(EFail);
			}		
		// Read the name of the source folder from the ini file
		TPtrC srcfolderName;
		if(!GetStringFromConfig( ConfigSection(), KSrcFolderName, srcfolderName))
			{
			ERR_PRINTF1(_L("Folder name is not specified"));
			SetTestStepResult(EFail);
			}		
		// Read the name of the target parent folder from the ini file
		TPtrC trgtParentFolderName;
		if(!GetStringFromConfig( ConfigSection(), KTargetParentFolderName, trgtParentFolderName))
			{
			ERR_PRINTF1(_L("Trgt Parent Folder name is not specified"));
			SetTestStepResult(EFail);
			}
		// Read the name of the target folder from the ini file
		TPtrC trgtfolderName;
		if(!GetStringFromConfig( ConfigSection(), KTargetFolderName, trgtfolderName))
			{
			ERR_PRINTF1(_L("Folder name is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{

			// Read the IMAP account name from the ini file
			TPtrC imapAccountName;
			if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
				{
				ERR_PRINTF1(_L("Imap account name is not specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				TInt err = KErrNone;
				// Read the name of source Lookup Folder from the ini file
				TPtrC srcLookupIn;
				if(!GetStringFromConfig( ConfigSection(), KSrcLookUpIn, srcLookupIn))
					{
					ERR_PRINTF1(_L("LookUpFolder name is not specified"));
					SetTestStepResult(EFail);
					}
				// Lookup the source folder
				TMsvId folderId;
				if(srcLookupIn.Compare(_L("REMOTE")) == 0)
					{
					// Get the Id of the remote folder
					TRAP(err,folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,imapAccountName,srcfolderName,srcParentFolderName));
					}
				else
					{
					if(srcLookupIn.Compare(_L("LOCAL"))==0)
						{
						// Get the Id of the local folder
						TRAP(err, folderId = CT_MsgUtils::FindEntryByNameL(iSharedDataIMAP.iSession, KMsvLocalServiceIndexEntryId,srcfolderName,srcParentFolderName));
						//TRAP(err,folderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(srcfolderName));
						}
					else
						{
						ERR_PRINTF1(_L("Wrong Input"));
						SetTestStepResult(EFail);
						}
					}
					
				INFO_PRINTF2(_L(" The srcfolder Id is %d"),folderId);

				// Read the name of target Lookup Folder from the ini file
				TPtrC trgtLookupIn;
				if(!GetStringFromConfig( ConfigSection(), KTrgtLookUpIn, trgtLookupIn))
					{
					ERR_PRINTF1(_L("LookUpFolder name is not specified"));
					SetTestStepResult(EFail);
					}
				
				// Lookup the target folder
				TMsvId targetId;
				if(trgtLookupIn.Compare(_L("REMOTE")) == 0)
					{
					// Get the Id of the remote folder
					TRAP(err,targetId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,imapAccountName,trgtfolderName,trgtParentFolderName));
					}
				else
					{
					if(trgtLookupIn.Compare(_L("LOCAL"))==0)
						{
						// Get the Id of the local folder
						TRAP(err, targetId = CT_MsgUtils::FindEntryByNameL(iSharedDataIMAP.iSession, KMsvLocalServiceIndexEntryId,trgtfolderName,trgtParentFolderName));
						}
					else
						{
						ERR_PRINTF1(_L("Wrong Input"));
						SetTestStepResult(EFail);
						}
					}

				INFO_PRINTF2(_L(" The trgtfolder Id is %d"),targetId);

				CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataIMAP.iSession,folderId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
				CleanupStack::PushL(entry);
				entry->SetEntryL(folderId);

				TPtrC aSubject;
				if( !GetStringFromConfig(ConfigSection(),KSubject,aSubject ))
					{
					ERR_PRINTF1(_L("The subject of the message is not specified"));
					SetTestStepResult(EFail);
					}
				else
					{
					// Retrieves the message Id based on the message subject
					TMsvId messageId = CT_MsgUtils::SearchMessageBySubjectL(iSharedDataIMAP.iSession, folderId , aSubject);
					if( messageId == KMsvNullIndexEntryId)
						{
						ERR_PRINTF1(_L("The given message is not found"));
						SetTestStepResult(EFail);
						}
					else
						{
						INFO_PRINTF2(_L("The Message Id is %d"),messageId );

						TInt expectedErr = 0;
						GetIntFromConfig(ConfigSection(), KExpectedErr, expectedErr);

						CT_MsgActive&	active=Active();
						delete iOperation;
						iOperation=NULL;
						
						TRAP(err, iOperation = entry->MoveL(messageId, targetId, active.iStatus));

						if (err == KErrNone)
							{
							active.Activate();
							CActiveScheduler::Start();
							err = active.Result();

							if (err == KErrNone)
								{
								TImap4GenericProgress temp;	
								TPckgC<TImap4GenericProgress> paramPack(temp);
								paramPack.Set(iOperation->ProgressL());
								TImap4GenericProgress progress=paramPack();
								err = progress.iErrorCode;
								}
							}

						if(expectedErr != KErrNone || err != KErrNone)
							{
							if(err == expectedErr)
								{
								INFO_PRINTF2(_L("Failed with Expected Error %d"), err);
								}
							else
								{
								ERR_PRINTF2(_L("Failed with %d error"),err);
								SetTestStepResult(EFail);
								}
							}
						}
					}

				CleanupStack::PopAndDestroy(entry);
				}
			}
		}

	return TestStepResult();
	}
