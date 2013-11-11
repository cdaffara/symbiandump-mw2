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
// CreateImap4Folder
// [Paramaters]
// ParentFolderName			:Name of the local/remote parent folder for the new local/remote folder to be created under
// MtmUidName				:MTM plugin UID; for REMOTE creation specify "KUidMsgTypeIMAP4" and for LOCAL
// creation specify "KUidMsvLocalServiceMtm"
// EntryDetails				:Name of the local/remote folder to be created
// EntryDescription			:Description of the folder created
// Target					:Specify either REMOTE or LOCAL depending on where you want to create the folder
// ImapAccountName			:Name of the Imap account
// Creates an IMAP4 folder under a parent remote/local entry
// 
//

//User includes
#include "T_CreateImap4Folder.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Literals used
_LIT(KParentFolderName,"ParentFolderName");
_LIT(KMtmUidName,"MtmUidName");
_LIT(KEntryDescription, "EntryDescription");
_LIT(KEntryDetails, "EntryDetails");
_LIT(KTarget,"Target");
_LIT(KImapAccountName,"ImapAccountName");

/**
  Function : CT_MsgCreateImap4Folder
  Description : Constructor 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgCreateImap4Folder::CT_MsgCreateImap4Folder(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,iOperation(NULL)
	{
	SetTestStepName(KCreateImap4Folder);
	}
	
/**
  Function : ~CT_MsgCreateImap4Folder
  Description : Destructor
  @return : N/A
*/
CT_MsgCreateImap4Folder::~CT_MsgCreateImap4Folder()
	{
	delete iOperation;
	iOperation=NULL;
	}
	
/**
  Function : NewL
  Description : Creates an object of CT_MsgCreateImap4Folder 				 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : Pointer to CT_MsgCreateImap4Folder
*/
CT_MsgCreateImap4Folder* CT_MsgCreateImap4Folder::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgCreateImap4Folder* self = new(ELeave) CT_MsgCreateImap4Folder(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgCreateImap4Folder::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KParentFolderName, iParentFolderName))
		{
		ERR_PRINTF1(_L("Local/Remote parent folder is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KMtmUidName, iMtmUidName))
		{
		ERR_PRINTF1(_L("The MTM uid of the entry is not specified: Eg. KUidMsvLocalServiceMtm or KUidMsgTypeIMAP4"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KEntryDescription, iEntryDescription))
		{
		ERR_PRINTF1(_L("The description of the entry is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KEntryDetails, iEntryDetails))
		{
		ERR_PRINTF1(_L("The details of the entry is not specified"));
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
		
	return result;
	}

/**
  Function : doTestStepL
  Description : Creates an IMAP4 folder under the specified parent remote/local entry
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgCreateImap4Folder::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : CreateImap4Folder"));		
	if(ReadIni())
		{
		// Local folder creation...
		if(iTarget.Compare(_L("LOCAL"))==0)
			{
			// Retrieve the folder Id based on the local folder name read from the ini file
			TMsvId parentFolderId;	
			if (iParentFolderName.Compare(_L("KMsvLocalServiceIndexEntryId")) == 0)
				{
				parentFolderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(iParentFolderName);
				if(parentFolderId == KErrNotFound)
					{
					ERR_PRINTF1(_L("Invalid local parent folder name specified"));
					SetTestStepResult(EFail);
					return TestStepResult();
					}
				INFO_PRINTF2(_L("The local parent folder Id is %d"),parentFolderId);
				}
			else
				{
				TRAPD(err,parentFolderId = CT_MsgUtils::FindEntryByNameL(iSharedDataIMAP.iSession, KMsvLocalServiceIndexEntryId,iParentFolderName));
				if(err == KErrNotFound)
					{
					ERR_PRINTF1(_L("Invalid local parent folder name specified"));
					SetTestStepResult(EFail);
					return TestStepResult();
					}
				INFO_PRINTF2(_L("The local parent folder Id is %d"),parentFolderId);
				}			
					
			TUid mtmUid;
			mtmUid.Null();
			if(iMtmUidName.Compare(_L("KUidMsvLocalServiceMtm"))==0)
				{
				mtmUid = KUidMsvLocalServiceMtm;
				}
			else
				{
				ERR_PRINTF1(_L("Invalid Local Service MTM Uid for creating folders under"));
				SetTestStepResult(EFail);
				return TestStepResult();
				}
					
			CreateEntryL(KMsvLocalServiceIndexEntryId, parentFolderId, KUidMsvFolderEntry, mtmUid, iEntryDescription, iEntryDetails);
			}
		// Remote folder creation...
		else if(iTarget.Compare(_L("REMOTE"))==0)
			{
			TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)iImapAccountName);
				
			TUid mtmUid;
			mtmUid.Null();
			if(iMtmUidName.Compare(_L("KUidMsgTypeIMAP4"))==0)
				{
				mtmUid = KUidMsgTypeIMAP4;
				}
			else
				{
				ERR_PRINTF1(_L("Invalid Remote Service MTM Uid for creating folders under"));
				SetTestStepResult(EFail);
				return TestStepResult();
				}
				
			TMsvId parentFolderId;
			if(iParentFolderName.Compare(iImapAccountName)==0)
				{
				parentFolderId = imapServiceId;
				}
			else
				{
				TRAPD(err,parentFolderId = CT_MsgUtils::GetRemoteFolderIdByNameL((iSharedDataIMAP.iSession), iImapAccountName, iParentFolderName));
				if(err == KErrNotFound)
					{
					ERR_PRINTF1(_L("Invalid remote parent folder name specified"));
					SetTestStepResult(EFail);
					return TestStepResult();
					}	
				INFO_PRINTF2(_L(" The remote parent folder Id for deletion is %d"),parentFolderId);
				}
			
			CreateEntryL(imapServiceId, parentFolderId, KUidMsvFolderEntry, mtmUid, iEntryDescription, iEntryDetails);
			}
		// Target Local or Remote not specified...
		else
			{
			ERR_PRINTF1(_L("Invalid Target for folder creation"));
			SetTestStepResult(EFail);
			}
		}
	return TestStepResult();
	}
	
/**
  Function : CreateEntryL
  Description : Creates either a local or remote folder
  @return : void
*/
void CT_MsgCreateImap4Folder::CreateEntryL(TMsvId aServiceId, TMsvId aParentEntryId, TUid aEntryType, TUid aMtmUid,TPtrC paramDescription,TPtrC paramDetails)
	{
	iIndexEntry.SetMailbox(ETrue);
	iIndexEntry.iType = aEntryType;
	iIndexEntry.iMtm = aMtmUid;	
	iIndexEntry.iServiceId = aServiceId;
	iIndexEntry.SetPriority(EMsvMediumPriority);
	iIndexEntry.SetReadOnly(EFalse);
	iIndexEntry.SetVisible(ETrue);
	iIndexEntry.iDescription.Set(paramDescription);
	iIndexEntry.iDetails.Set(paramDetails);
	iIndexEntry.iDate.HomeTime();
	
	CT_MsgActive& active = Active();
	delete iOperation;
	iOperation=NULL;
	
	CMsvEntry* entry = CMsvEntry::NewL(*(iSharedDataIMAP.iSession),aParentEntryId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(entry);
	if (entry->OwningService() == KMsvLocalServiceIndexEntryId)
		{
		entry->CreateL(iIndexEntry);
		}
	else
		{
		iOperation = entry->CreateL(iIndexEntry,active.iStatus);
		active.Activate();
		CActiveScheduler::Start();
		User::LeaveIfError(active.Result());
		}
	CleanupStack::PopAndDestroy(entry);
	}

/**
  Function : ProgressL
  Description : Displays the progress information of the asynchronous operation
  @return : void
*/
void CT_MsgCreateImap4Folder::ProgressL(TBool bFinal)
	{
	// Stub
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
void CT_MsgCreateImap4Folder::CancelStep()
	{
	iOperation->Cancel();
	}
