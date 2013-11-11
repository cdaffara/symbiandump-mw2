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
// CheckImap4Subscription
// [Paramaters]
// ImapAccountName		:Name of the Imap account
// ParentFolderName		:Name of the remote parent folder
// FolderName			:Name of the remote folder to check for subscription
// ExpectedSubscription	:Specify either SUBSCRIBE or UNSUBSCRIBE depending on whether you expect the
// remote folder to be subscribed or not
// Check if a given remote folder name is subscribed or unsubscribed
// 
//

//User includes
#include "T_CheckImap4Subscription.h"
#include <t_utilscentralrepository.h>
#include <t_utils.h>

//Epoc include
#include <miuthdr.h>

// Literals used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KParentFolderName,"ParentFolderName");
_LIT(KFolderName,"FolderName");
_LIT(KExpectedSubscription,"ExpectedSubscription");

/**
  Function : CT_MsgCheckImap4Subscription
  Description : Constructor
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgCheckImap4Subscription::CT_MsgCheckImap4Subscription(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,iOperation(NULL)
	{
	SetTestStepName(KCheckImap4Subscription);
	}

/**
  Function : NewL
  Description : Creates an object of CT_MsgCheckImap4Subscription 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap				 
  @return : Pointer to CT_MsgCheckImap4Subscription
*/
CT_MsgCheckImap4Subscription* CT_MsgCheckImap4Subscription::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgCheckImap4Subscription* self = new(ELeave) CT_MsgCheckImap4Subscription(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Function : ~CT_MsgCheckImap4Subscription
  Description : Destructor
  @return : N/A
*/
CT_MsgCheckImap4Subscription::~CT_MsgCheckImap4Subscription()
	{
	delete iOperation;
	iOperation=NULL;
	}
	
/**
  Function : doTestStepL
  Description : Check if the given remote folder name is subscribed or unsubscribed
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgCheckImap4Subscription::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: CheckImap4Subscription"));
	if(ReadIni())
		{
		// Get the Id of the remote folder
		TMsvId folderId;
		TRAPD(err,folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession, iImapAccountName, iFolderName, iParentName));
		if(err == KErrNotFound)
			{
			ERR_PRINTF1(_L("Invalid remote folder name specified"));
			SetTestStepResult(EFail);
			return TestStepResult();		
			}
		
		CMsvEntry* entry = CMsvEntry::NewL(*(iSharedDataIMAP.iSession),folderId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
		CleanupStack::PushL(entry);
			
		TMsvEntry temp = entry->Entry();	
		TMsvEmailEntry emailEntry(temp);
		TBool subRemote = emailEntry.Subscribed();	
			
		// 	Folder not expected to be subscribed...	
		if(iExpectedSubscription.Compare(_L("UNSUBSCRIBE"))==0)
			{
			if(!subRemote)
				{
				INFO_PRINTF1(_L("Folder is unsubscribed"));
				}
			else
				{
				ERR_PRINTF1(_L("Folder is not unsubscribed as expected!"));
				SetTestStepResult(EFail);
				}
			}
		// 	Folder is expected to be subscribed...
		else if(iExpectedSubscription.Compare(_L("SUBSCRIBE"))==0)
			{
			if(subRemote)
				{
				INFO_PRINTF1(_L("Folder is subscribed"));
				}
			else
				{
				ERR_PRINTF1(_L("Folder is not subscribed as expected!"));
				SetTestStepResult(EFail);
				}
			}
		else
			{
			ERR_PRINTF1(_L("Invalid Expected Subscription parameter! - Should use either SUBSCRIBE or UNSUBSCRIBE"));
			SetTestStepResult(EFail);
			}
		CleanupStack::PopAndDestroy(entry);
		}		
	return TestStepResult();
	}
	
/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgCheckImap4Subscription::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap Account Name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KParentFolderName, iParentName))
		{
		ERR_PRINTF1(_L("Parent Name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KFolderName, iFolderName))
		{
		ERR_PRINTF1(_L("Folder Name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KExpectedSubscription, iExpectedSubscription))
		{
		ERR_PRINTF1(_L("Expected Subscription is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	return result;
	}
	
/**
  Function : ProgressL
  Description : Displays the progress information of the asynchronous operation
  @return : void
*/
void CT_MsgCheckImap4Subscription::ProgressL(TBool bFinal)
	{
	//	Stub
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
void CT_MsgCheckImap4Subscription::CancelStep()
	{
	iOperation->Cancel();
	}
