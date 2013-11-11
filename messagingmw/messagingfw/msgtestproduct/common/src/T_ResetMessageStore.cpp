// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// ResetMessageStore
// [Paramaters]
// none
// Deletes all entries in the message folder. If an entry cannot be deleted 
// ignores the entry.After deleting all entries that can be deleted reads 
// msgs.rsc and checks that the structure provided in msgs.rsc is the same as 
// the message store. 
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::ChildrenL
// CMsvEntry::DeleteL
// CMsvEntry::GetEntryL
// CEmailAccounts::GetPopAccountsL
// CEmailAccounts::DeletePopAccountL
// CEmailAccounts::DeleteImapAccountL
// CEmailAccounts::GetImapAccountsL
// CEmailAccounts::DeleteSmtpAccountL
// CEmailAccounts::GetSmtpAccountsL
// CMsvStore::HasStoreL
// CMsvStore::EditStoreL
// MMsvAttachmentManager::AttachmentManagerL
// MMsvAttachmentManager::RemoveAttachmentL
// 
//

/**
 @file  
*/



/* User Includes */
#include "T_ResetMessageStore.h"
#include <t_utilsdeleteallchildren.h>
#include <messagingtestutility2.h>

/* Epoc includes */
#include <barsc.h>
#include <bautils.h>
#include <msgs.rsg>
#include <mmsvattachmentmanager.h>
#include <cemailaccounts.h>



/* Literals Used */
_LIT(KServerResourceFile,	"z:\\resource\\messaging\\msgs.rsc");
_LIT(KStoreInitFileName,	"c:\\private\\1000484b\\StoreInit.tmp");




/**
  Function : NewL
  Description : Creates an object of CT_MsgResetMessageStore
  @param : aSharedDataCommon		A reference to CT_MsgSharedDataCommon
  @return : A pointer to an object of type CT_MsgResetMessageStore
  @leave  : KErrNoMemory if	sufficient memory is not available
*/
CT_MsgResetMessageStore* CT_MsgResetMessageStore::NewL(CT_MsgSharedDataCommon& aSharedDataCommon)
	{
	CT_MsgResetMessageStore* self = new(ELeave) CT_MsgResetMessageStore(aSharedDataCommon);
	CleanupStack::PushL(self);
	self->ConstructL(); 
	CleanupStack::Pop(self);
	return self;
	}


/**
  Function : CT_MsgResetMessageStore
  Description : Constructor
  @param : aSharedDataCommon	    A reference to CT_MsgSharedDataCommon
  @return : N/A
*/
CT_MsgResetMessageStore::CT_MsgResetMessageStore(CT_MsgSharedDataCommon& aSharedDataCommon)
:	CT_MsgAsyncStepCommon(aSharedDataCommon)
,	iDeleteAllChildren(NULL)
	{
	SetTestStepName(KResetMessageStore);
	}



/**
  Function : ConstructL
  Description : Calls the ConstrutL of CT_MsgAsyncStepCommon and creates an object of 
				CT_MsgUtilsDeleteAllChildren to delete the entries of the message store
  @return : N/A
*/
void CT_MsgResetMessageStore::ConstructL()
	{
	CT_MsgAsyncStepCommon::ConstructL();
	iDeleteAllChildren = new (ELeave) CT_MsgUtilsDeleteAllChildren(iSharedDataCommon.iSession, KMsvRootIndexEntryId);
	}



/**
  Function : ~CT_MsgResetMessageStore
  Description : Destructor
  @return : N/A
*/
CT_MsgResetMessageStore::~CT_MsgResetMessageStore()
	{
	delete iDeleteAllChildren;
	}



/**
  Function : doTestStepL
  Description : Deletes all entries in the message folder. If an entry cannot be deleted 
				ignores the entry.After deleting all entries that can be deleted reads 
				msgs.rsc and checks that the structure provided in msgs.rsc is the same as 
				the message store.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgResetMessageStore::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : ResetMessageStore"));
	
	//delelets Email acccounts from central repository
	ResetStoreEmailAccountsL();

	/* Creates an object of CT_MsgActive for asynchronous operations */
	CT_MsgActive&	active=Active();

	/* Deletes all the entries in the message store */
	iDeleteAllChildren->StartL(active.iStatus);

	active.Activate();
	CActiveScheduler::Start();
	User::LeaveIfError(active.Result());

	/* Performs the cleanup the Central Respository */
	ResetStoreL();

	return TestStepResult();
	}



/**
  Function : ProgressL
  Description : Provides the progress information of the asynchronous operation
  @param :	bFinal	Used to display the status of the asynchronous operation
  @return : none
*/
void CT_MsgResetMessageStore::ProgressL(TBool bFinal)
	{
	//	Stub
	if ( bFinal )
		{
		/*	Display final progress iDeleteAllChildren */
		}
	else
		{
		/*	Display current progress iDeleteAllChildren*/
		}
	}




/**
  Function : CancelStep
  Description : Used to cancel an asynchronous operation
  @return : none
*/
void CT_MsgResetMessageStore::CancelStep()
	{
	//	Stub
	//  Cancel iDeleteAllChildren
	}



/**
  Function : ResetStoreL
  Description : Reads from the file msg.rsc and checks the message store structure
				
  @return : none
  @leave  : If any of the leaving funcitons fail
*/
void CT_MsgResetMessageStore::ResetStoreL()
	{
	/* Returns the number of the children entries that are not deleted */
	TInt undeleted = iDeleteAllChildren->Undeleted();

	RFs	fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

    TFileName fileName;
    TParse	  parse;

	/* Parses the file specification, allowing wildcards in the filename and extension */
    parse.Set(KServerResourceFile,&fileName,NULL);

	/* Gets the complete file specification */
	fileName = parse.FullName();
	BaflUtils::NearestLanguageFile(fs, fileName);

	/* Opens the resource file reader */
	RResourceFile	resource;
	resource.OpenL(fs,fileName);
	CleanupClosePushL(resource);

	/* 
	 * Reads a resource into a heap descriptor, returns a pointer to that 
	 * descriptor and pushes the pointer onto the cleanup stack
	 * And sets the buffer containing the resource data.
	 */
	TResourceReader	reader;
	HBufC8*	buf = resource.AllocReadLC(R_SERVER_INDEX_STARTUP);
	reader.SetBuffer(buf);	

	/* 
	 * Interprets the data at the current buffer position as a 
	 * TInt16 type and returns the value as a TInt and compares with 
	 * the number of undeleted entries
	 */
	const TInt numberOfEntries = reader.ReadInt16();
	if (undeleted != numberOfEntries)
		{
		User::Leave(KErrGeneral);
		}


	RMessagingTestUtilityServer2	ts;
	User::LeaveIfError(ts.Connect());
	CleanupClosePushL(ts);

	/* Deletes the file "StoreInit.tmp" */
	ts.DeleteFile(KStoreInitFileName);
	CleanupStack::PopAndDestroy(&ts);

	
	/* Creates an object of CT_MsgActive for asynchronous operation */
	CT_MsgActive& active=Active();

	for (TInt i=0; i<numberOfEntries; i++)
		{
		/* 
		 * Interprets the data at the current buffer position as a
		 * TInt32 type and returns the value as a TInt
		 */
		TMsvId	id = reader.ReadInt32();

		/* Creates an entry object for the id returned */
		CMsvEntry*	clientEntry = iSharedDataCommon.iSession->GetEntryL(id);
		CleanupStack::PushL(clientEntry);

		/* Checks if the context has an associated message store
		 * If yes then gets the message store for the current context with read-write access
		 */
		if (clientEntry->HasStoreL())
			{
			CMsvStore*	store = clientEntry->EditStoreL();
			CleanupStack::PushL(store);

			/* Defines the attachment management interface and
			 * obtains the count the number of attachments
			 */
			MMsvAttachmentManager&	attManager = store->AttachmentManagerL();																			 
			TInt count = attManager.AttachmentCount();
			for( TInt i=0; i<count; ++i )
				{
				/* 
				 * Removes the attachment from the message entry
				 * As one attachment is removed, the next attachment becomes index 0 
				 */
				attManager.RemoveAttachmentL(0, active.iStatus);
				active.Activate();
				CActiveScheduler::Start();
				User::LeaveIfError(active.Result());
				}
			/* Commits changes to the message store, leaving if an error occurs
			 * and then deletes the message store */
			store->CommitL();
			store->DeleteL();
			CleanupStack::PopAndDestroy(store);
			}
		
		/* 
		 * Gets the index entry for the context
		 * Checks if it is not deleted 
		 * If yes, leave with KErrGeneral
		 */
		TMsvEntry indexEntry = clientEntry->Entry();

		if (!((indexEntry.Parent()   == reader.ReadInt32()) &&
			 (indexEntry.iServiceId == reader.ReadInt32()) &&
			 (indexEntry.iType.iUid == reader.ReadInt32()) &&
			 (indexEntry.iMtm.iUid  == reader.ReadInt32()) &&
			 ((reader.ReadInt32()) | 1) &&
			 (!indexEntry.iDescription.Compare(reader.ReadTPtrC())) &&
			 (!indexEntry.iDetails.Compare(reader.ReadTPtrC()))))
			{
			User::Leave(KErrGeneral);
			}

		CleanupStack::PopAndDestroy(clientEntry);
		}
	CleanupStack::PopAndDestroy(3, &fs);	/* buf, resource, fs*/
	}


/**
  Function : ResetStoreEmailAccountsL
  Description : Deletes all the IMAP,POP and SMTP accounts in the Central Respository
  @return : none
  @leave  : If any of the leaving funcitons fail
*/	

void CT_MsgResetMessageStore::ResetStoreEmailAccountsL()
	{
		/*Performs the cleanup of the Central repository */
	CEmailAccounts*	accounts = CEmailAccounts::NewLC();

	/* 
	 * Gets a list of the POP accounts
	 * and deletes POP3 accounts
	 */
	RArray<TPopAccount>	pop3Accounts;
	CleanupClosePushL(pop3Accounts);
	accounts->GetPopAccountsL(pop3Accounts);

	for (TInt i=0; i<pop3Accounts.Count(); ++i)
		{
		accounts->DeletePopAccountL(pop3Accounts[i]);
		}

	/* 
	 * Gets a list of the IMAP accounts
	 * and deletes IMAP accounts
	 */
	RArray<TImapAccount> imap4Accounts;
	CleanupClosePushL(imap4Accounts);
	accounts->GetImapAccountsL(imap4Accounts);

	for (TInt j=0; j<imap4Accounts.Count(); ++j)
		{
		accounts->DeleteImapAccountL(imap4Accounts[j]);
		}

	// Delete any IMAP transport buffer sizes
	accounts->DeleteImapTransportBufferSizesL();

	/* 
	 * Gets a list of the SMTP accounts
	 * and deletes SMTP accounts 
	 */
	RArray<TSmtpAccount> smtpAccounts;
	CleanupClosePushL(smtpAccounts);
	accounts->GetSmtpAccountsL(smtpAccounts);

	for (TInt k=0; k<smtpAccounts.Count(); ++k)
		{
		accounts->DeleteSmtpAccountL(smtpAccounts[k]);
		}

	CleanupStack::PopAndDestroy(4, accounts);
	}
	



