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
// InitialiseImap4Account
// [Paramaters]
// ImapAccountName					:Name of the Imap account
// InboxFolderName (OPTIONAL)		:Name of default remote INBOX folder to re-create if deleted (Default: "Inbox")
// OutboxFolderName (OPTIONAL)		:Name of default remote OUTBOX folder to re-create if deleted (Default: "Outbox")
// DraftsFolderName (OPTIONAL)		:Name of default remote DRAFTS folder to re-create if deleted (Default: "Drafts")
// SentFolderName (OPTIONAL)		:Name of default remote SENT folder to re-create if deleted (Default: "Sent Items")
// DeletedFolderName (OPTIONAL)	:Name of default remote DELETED folder to re-create if deleted (Default: "Deleted Items")
// Initialises the remote IMAP4 account by remotely unsubscribing to and deleting all remote folders
// (including all sub folders) barring the special/pre-defined default folders.  It also subsequently
// deletes all the messages within these non-deleted special/pre-defined default folders such as
// INBOX.
// 
//

//User includes
#include "T_InitialiseImap4Account.h"
#include <t_utilsenumconverter.h>
#include <t_utils.h>
#include <t_utilscentralrepository.h>

//Literals used
_LIT(KImapAccountName, "ImapAccountName");
_LIT(KInboxFolderName,"InboxFolderName");
_LIT(KOutboxFolderName,"OutboxFolderName");
_LIT(KDraftsFolderName,"DraftsFolderName");
_LIT(KSentFolderName,"SentFolderName");
_LIT(KDeletedFolderName,"DeletedFolderName");

_LIT(KInbox,"Inbox");
_LIT(KOutbox,"Outbox");
_LIT(KDrafts,"Drafts");
_LIT(KSentItems,"Sent Items");
_LIT(KDeletedItems,"Deleted Items");

/**
  Function : CT_MsgInitialiseImap4Account
  Description : Constructor
  @param : aSharedDataIMAP	A reference to CT_MsgSharedDataImap
*/
CT_MsgInitialiseImap4Account::CT_MsgInitialiseImap4Account(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
,iOperation(NULL)
,iFolderCounterForDelete(0)
,iFolderCounterForUnsubscribe(0)
	{
	SetTestStepName(KInitialiseImap4Account);
	}

/**
  Function : NewL
  Description : Creates an object of CT_MsgInitialiseImap4Account 				 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : Pointer to CT_MsgInitialiseImap4Account
*/
CT_MsgInitialiseImap4Account* CT_MsgInitialiseImap4Account::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_MsgInitialiseImap4Account* self = new(ELeave) CT_MsgInitialiseImap4Account(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : ~CT_MsgInitialiseImap4Account
  Description : Destructor
  @return : N/A
*/
CT_MsgInitialiseImap4Account::~CT_MsgInitialiseImap4Account()
	{
	delete iOperation;
	iOperation=NULL;
	}

/**
  Function : doTestStepL
  Description : Initialises the remote IMAP4 account by remotely unsubscribing to and deleting all remote folders
  				(including all sub folders) barring the special/pre-defined default folders.  It also subsequently
  				deletes all the messages within these non-deleted special/pre-defined default folders such as INBOX.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgInitialiseImap4Account::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : InitialiseImap4Account"));
	if(ReadIni())
		{
		iImapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)iImapAccountName);

		INFO_PRINTF1(_L("Name of all folders including sub folders under the IMAP4 service entry:"));
		INFO_PRINTF1(_L("BEFORE cleanup:"));
		PrintFolderTreeL(iImapServiceId, *(iSharedDataIMAP.iSession));
		
		// UNSUBSCRIBE all folders barring INBOX, thus remove from the remote IMAP4 server's set of subscribed mailboxes 
		INFO_PRINTF1(_L("***Unsubscribing to all folders including sub folders under IMAP4 service entry taking place barring INBOX..."));
		UnsubscribeChildrenL(iImapServiceId, *(iSharedDataIMAP.iSession));

		// SYNC to update the remote subscription flags
		INFO_PRINTF1(_L("Doing a sync to update remote subscription flags..."));
		SynchroniseAccountL();
		
		//DELETE all folders barring special/pre-defined default folders
		INFO_PRINTF1(_L("***Deletion of all folders including sub folders under IMAP4 service entry taking place barring special/pre-defined default folders..."));
		DeleteChildrenL(iImapServiceId, *(iSharedDataIMAP.iSession));
		
		INFO_PRINTF1(_L("Name of all folders including sub folders under the IMAP4 service entry:"));
		INFO_PRINTF1(_L("AFTER cleanup:"));
		PrintFolderTreeL(iImapServiceId, *(iSharedDataIMAP.iSession));
		
		// Obtains the count of the non-deletable special/pre-defined default remote folders under the IMAP4 service entry
		TInt count = GetFolderCountL(iImapServiceId, *(iSharedDataIMAP.iSession));
		INFO_PRINTF2(_L("Non-deletable special/pre-defined default remote folder count for the Email server that this test executes on is %d"), count);
		
		// RE-CREATE deleted default folders that user expects to exist after clean up
		INFO_PRINTF1(_L("***Re-creating the expected deleted default folders..."));
		CreateDefaultFoldersL(iImapServiceId);
		
		INFO_PRINTF1(_L("Name of all folders including sub folders under the IMAP4 service entry:"));
		INFO_PRINTF1(_L("AFTER re-creating any deleted default folders:"));
		PrintFolderTreeL(iImapServiceId, *(iSharedDataIMAP.iSession));
		}
	return TestStepResult();
	}
	
/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/	
TBool CT_MsgInitialiseImap4Account::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap Account Name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	return result;
	}

/**
  Function : SynchroniseAccountL
  Description : Performs full synchronisation with the IMAP4 service
  @return : void
*/
void CT_MsgInitialiseImap4Account::SynchroniseAccountL()
	{
	iSharedDataIMAP.iMtm->SwitchCurrentEntryL(iImapServiceId);
	
	// Creates the selection object
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	
	// Appends the iImapServiceId onto the end of the array
	selection->AppendL(iImapServiceId);
	TBuf8<1> param;

	CT_MsgActive& active=Active();
	// Cleanup any existing operation object
	delete iOperation;
	iOperation=NULL;

	// Synchronise with the IMAP4 service
	iOperation = iSharedDataIMAP.iMtm->InvokeAsyncFunctionL(KIMAP4MTMFullSync, *selection, param, active.iStatus);
	active.Activate();
	CActiveScheduler::Start();
	User::LeaveIfError(active.Result());	

	CleanupStack::PopAndDestroy(selection);
	}

/**
  Function : CreateDefaultFoldersL
  Description : Create user expected default remote folders
  @return : void
*/
void CT_MsgInitialiseImap4Account::CreateDefaultFoldersL(TMsvId aFolderId)
	{
	TPtrC folder(KInbox);
	GetStringFromConfig(ConfigSection(), KInboxFolderName, folder);
	CreateEntryL(aFolderId, folder);
	
	folder.Set(KOutbox);
	GetStringFromConfig(ConfigSection(), KOutboxFolderName, folder);
	CreateEntryL(aFolderId, folder);
	
	folder.Set(KDrafts);
	GetStringFromConfig(ConfigSection(), KDraftsFolderName, folder);
	CreateEntryL(aFolderId, folder);
	
	folder.Set(KSentItems);
	GetStringFromConfig(ConfigSection(), KSentFolderName, folder);
	CreateEntryL(aFolderId, folder);
	
	folder.Set(KDeletedItems);
	GetStringFromConfig(ConfigSection(), KDeletedFolderName, folder);
	CreateEntryL(aFolderId, folder);
	}
	
/**
  Function : CreateEntryL
  Description : Creates remote IMAP4 folder
  @return : void
*/	
void CT_MsgInitialiseImap4Account::CreateEntryL(TMsvId aFolderId,const TDesC& aParamDetails)
	{
	TMsvEmailEntry tmsvEntry;
	tmsvEntry.SetMailbox(ETrue);
	tmsvEntry.iType = KUidMsvFolderEntry;
	tmsvEntry.iMtm = KUidMsgTypeIMAP4;
	tmsvEntry.iServiceId = aFolderId;
	tmsvEntry.SetPriority(EMsvMediumPriority);
	tmsvEntry.SetReadOnly(EFalse);
	tmsvEntry.SetVisible(ETrue);
	tmsvEntry.iDetails.Set(aParamDetails);
	tmsvEntry.iDate.HomeTime();
	
	CT_MsgActive& active = Active();
	delete iOperation;
	iOperation=NULL;
	
	CMsvEntry* entry = CMsvEntry::NewL(*(iSharedDataIMAP.iSession),aFolderId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(entry);

	iOperation = entry->CreateL(tmsvEntry,active.iStatus);
	active.Activate();
	CActiveScheduler::Start();
	User::LeaveIfError(active.Result());
	CleanupStack::PopAndDestroy(entry);
	}

/**
  Function : DeleteChildrenL
  Description : Deletes all folders including sub folders under IMAP4 service entry
  				barring the special/pre-defined default folders
  @return : void
*/
void CT_MsgInitialiseImap4Account::DeleteChildrenL(TMsvId aFolderId, CMsvSession& aSession)
	{
	// Sets the context on the specified folder whose entries count needs to be obtained
	CMsvEntry*	entryContext = CMsvEntry::NewL(
									aSession, aFolderId, 
		    						TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(entryContext);

	// Creates a selection object for the children entries
	CMsvEntrySelection*	selection = entryContext->ChildrenL();
	CleanupStack::PushL(selection);

	// Obtains the count of the children entries
	TInt count = selection->Count();

	// Loops through each child entry and sets the context on it
	for(TInt child=0;child<count;child++)
		{
		entryContext->SetEntryL((*selection)[child]);
		TMsvEntry entry = entryContext->Entry();
		
		/*
		 * Temp fix: Extra check below needed for this folder ("Public Folders/" or "Public Folders").
		 * MS Exchange 2000 server pre-defines it and prohibits it's deletion, but strange behaviour is
		 * observed if you attempt to delete it.  Note: Attempting to delete other special folders like
		 * INBOX doesn't cause CMsvEntry::DeleteL to leave so logic below is OK.
		 */
		if (entry.iType==KUidMsvFolderEntry && 
			((entry.iDetails).Compare(_L("Public Folders")) == 0 || (entry.iDetails).Compare(_L("Public Folders/")) == 0))
			{
			INFO_PRINTF2(_L("Non-selectable folder found and cannot be deleted called: %S"),&(entry.iDetails));
			}
		else
			{
			if (entry.iType==KUidMsvFolderEntry)
				{
				TInt childFolderCount = GetFolderCountL((*selection)[child],aSession);
				if (childFolderCount)
					{
					DeleteChildrenL((*selection)[child],aSession);
					}
				iFolderCounterForDelete++;
				INFO_PRINTF2(_L("Folder no. %d deletion attempt taking place..."),iFolderCounterForDelete);
				INFO_PRINTF3(_L("Name of folder attempted to be deleted: %S ---> TMsvId: %d"),&(entry.iDetails), entry.Id());
				
				DeleteFolderL(entry.Id());
				}
			}
		}
		
	CleanupStack::PopAndDestroy(2,entryContext); // selection,entryContext
	}
	
/**
  Function : UnsubscribeChildrenL
  Description : Unsubscribes to all folders including sub folders under IMAP4 service entry barring INBOX
  @return : void
*/
void CT_MsgInitialiseImap4Account::UnsubscribeChildrenL(TMsvId aFolderId, CMsvSession& aSession)
	{
	// Sets the context on the specified folder whose entries count needs to be obtained
	CMsvEntry*	entryContext = CMsvEntry::NewL(
									aSession, aFolderId, 
		    						TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(entryContext);

	// Creates a selection object for the children entries
	CMsvEntrySelection*	selection = entryContext->ChildrenL();
	CleanupStack::PushL(selection);

	// Obtains the count of the children entries
	TInt count = selection->Count();

	// Loops through the each child entry and sets the context on it
	for(TInt child=0;child<count;child++)
		{
		entryContext->SetEntryL((*selection)[child]);
		TMsvEntry entry = entryContext->Entry();
		
		if (entry.iType==KUidMsvFolderEntry && (entry.iDetails).Compare(_L("INBOX")) == 0)
			{
			INFO_PRINTF1(_L("[[[INBOX found and unsubscription of this folder will not be attempted]]]"));
			UnsubscribeChildrenL((*selection)[child],aSession);
			}
		else if (entry.iType==KUidMsvFolderEntry)
			{
			TInt childFolderCount = GetFolderCountL((*selection)[child],aSession);
			if (childFolderCount)
				{
				UnsubscribeChildrenL((*selection)[child],aSession);
				}
			iFolderCounterForUnsubscribe++;
			INFO_PRINTF2(_L("Folder no. %d unsubscribe attempt taking place..."),iFolderCounterForUnsubscribe);
			INFO_PRINTF3(_L("Name of folder attempted to be unsubscribed: %S ---> TMsvId: %d"),&(entry.iDetails), entry.Id());
				
			UnsubscribeFolderL(entry.Id());
			}
		}
	CleanupStack::PopAndDestroy(2,entryContext); // selection,entryContext
	}	

/**
  Function : UnsubscribeFolderL
  Description : Unsubscribes to remote IMAP4 folder
  @return : void
*/
void CT_MsgInitialiseImap4Account::UnsubscribeFolderL(TMsvId aFolderId)
	{
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	
	selection->AppendL(aFolderId);
	TBuf8<1> param;
	
	CT_MsgActive& active = Active();
	delete iOperation;
	iOperation=NULL;
	iOperation = (iSharedDataIMAP.iMtm)->InvokeAsyncFunctionL(KIMAP4MTMLocalUnsubscribe,*selection,param,active.iStatus);
	active.Activate();
	CActiveScheduler::Start();
	User::LeaveIfError(active.Result());
	
	CleanupStack::PopAndDestroy(selection);
	}
	
/**
  Function : DeleteFolderL
  Description : Deletes a remote IMAP4 folder
  @return : void
*/
void CT_MsgInitialiseImap4Account::DeleteFolderL(TMsvId aFolderId)
	{
	CMsvEntry* entry = CMsvEntry::NewL(*(iSharedDataIMAP.iSession),aFolderId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(entry);
	entry->SetEntryL(entry->Entry().Parent());
	
	CT_MsgActive& active = Active();
	delete iOperation;
	iOperation=NULL;
	iOperation = entry->DeleteL(aFolderId,active.iStatus);
	active.Activate();
	CActiveScheduler::Start();
	
	if(active.Result())
		{
		ERR_PRINTF1(_L("ERROR: Name of all folders including sub folders under the IMAP4 service entry before leaving!!!:"));
		PrintFolderTreeL(iImapServiceId, *(iSharedDataIMAP.iSession));
		}
		
	User::LeaveIfError(active.Result());
	
	CleanupStack::PopAndDestroy(entry);
	}

/**
  Function : GetFolderCountL
  Description : Obtains the count of the remote folders under the IMAP4 service entry.
  				This method does not recurse into subfolders.  It only counts the folders
  				at the level below the supplied entry id.
  @return : TInt
*/
TInt CT_MsgInitialiseImap4Account::GetFolderCountL(TMsvId aFolderId, CMsvSession& aSession)
	{
	TInt folderCount=0;
	
	// Sets the context on the specified folder whose entries count needs to be obtained
	CMsvEntry*	entryContext = CMsvEntry::NewL(
									aSession, aFolderId, 
		    						TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(entryContext);

	// Creates a selection object for the children entries
	CMsvEntrySelection*	selection = entryContext->ChildrenL();
	CleanupStack::PushL(selection);

	// Obtains the count of the children entries
	TInt count = selection->Count();
	
	// Loops through the each child entry and sets the context on it
	for(TInt child=0;child<count;child++)
		{
		entryContext->SetEntryL((*selection)[child]);
		TMsvEntry entry = entryContext->Entry();
		if (entry.iType==KUidMsvFolderEntry)
			{
			++folderCount;
			}
		}
	CleanupStack::PopAndDestroy(2,entryContext); // selection,entryContext
	return folderCount;
	}

/**
  Function : PrintFolderTreeL
  Description : Prints the tree of remote folders including all sub folders under the IMAP4 service entry
  @return : void
*/
void CT_MsgInitialiseImap4Account::PrintFolderTreeL(TMsvId aFolderId, CMsvSession& aSession)
	{
	// Sets the context on the specified folder whose entries count needs to be obtained
	CMsvEntry*	entryContext = CMsvEntry::NewL(
									aSession, aFolderId, 
		    						TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(entryContext);

	// Creates a selection object for the children entries
	CMsvEntrySelection*	selection = entryContext->ChildrenL();
	CleanupStack::PushL(selection);

	// Obtains the count of the children entries
	TInt count = selection->Count();
	
	// Loops through the each child entry and sets the context on it
	for(TInt child=0;child<count;child++)
		{
		entryContext->SetEntryL((*selection)[child]);

		TMsvEntry entry = entryContext->Entry();
		
		if (entry.iType==KUidMsvFolderEntry)
			{
			TInt childFolderCount = GetFolderCountL((*selection)[child],aSession);
			if (childFolderCount)
				{
				INFO_PRINTF3(_L("##### [%S] has following %d sub folder[s]"),&(entry.iDetails),childFolderCount);
				PrintFolderTreeL((*selection)[child],aSession);
				}
			INFO_PRINTF3(_L("# [%S] ---> TMsvId: %d"),&(entry.iDetails), entry.Id());
			}
		}
	CleanupStack::PopAndDestroy(2,entryContext); // selection,entryContext
	}

/**
  Function : ProgressL
  Description : Displays the progress information of the asynchronous operation
  @return : void
*/
void CT_MsgInitialiseImap4Account::ProgressL(TBool bFinal)
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
void CT_MsgInitialiseImap4Account::CancelStep()
	{
	iOperation->Cancel();
	}
