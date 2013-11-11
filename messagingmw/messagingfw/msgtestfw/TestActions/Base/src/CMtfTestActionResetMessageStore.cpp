// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// ResetMessageStore
// [Action Parameters]
// Session <input>: Reference to the session.
// [Action Description]
// Deletes all entries in the message folder. If an entry cannot be deleted ignores the entry. 
// After deleting all entries that can be deleted reads msgs.rsc and checks that the structure 
// provided in msgs.rsc is the same as the message store.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/



#include <msvapi.h>
#include <miutset.h>
#include <barsc.h>
#include <bautils.h>
#include <msgs.rsg>
	
#include "CMtfAsyncWaiter.h"

#include <mmsvattachmentmanager.h>
#include "messagingtestutility.h"
#include <cemailaccounts.h>

#include "CMtfTestActionResetMessageStore.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

_LIT(KServerResourceFile,"z:\\resource\\messaging\\msgs.rsc");

_LIT(KStoreInitFileName, "c:\\private\\1000484b\\StoreInit.tmp");

CMtfTestAction* CMtfTestActionResetMessageStore::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionResetMessageStore* self = new (ELeave) CMtfTestActionResetMessageStore(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionResetMessageStore::CMtfTestActionResetMessageStore(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionResetMessageStore::~CMtfTestActionResetMessageStore()
	{
	}


void CMtfTestActionResetMessageStore::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionResetMessageStore);
	iSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));

	iDeleteAllChildren = new (ELeave) CMtfTestActionUtilsDeleteAllChildren(iSession,KMsvRootIndexEntryId);
	iDeleteAllChildren->StartL(iStatus);
	CActiveScheduler::Add(this);
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionResetMessageStore);
	}


void CMtfTestActionResetMessageStore::DoCancel()
	{
	iDeleteAllChildren->Cancel();
	}

void CMtfTestActionResetMessageStore::RunL()
	{
	TInt undeleted = iDeleteAllChildren->Undeleted();
	delete iDeleteAllChildren;

	User::LeaveIfError(iStatus.Int());

	RFs fs;
	User::LeaveIfError(fs.Connect());

    TFileName fileName;
    TParse parse;
    parse.Set(KServerResourceFile,&fileName,NULL);
	fileName = parse.FullName();
	BaflUtils::NearestLanguageFile(fs, fileName);

	RResourceFile resource;
	resource.OpenL(fs,fileName);
	CleanupClosePushL(resource);

	TResourceReader reader;
	HBufC8* buf = resource.AllocReadLC(R_SERVER_INDEX_STARTUP);
	reader.SetBuffer(buf);	

	const TInt numberOfEntries = reader.ReadInt16();
	if (undeleted != numberOfEntries)
		User::Leave(KErrGeneral);

	RMessagingTestUtilityServer ts;
	User::LeaveIfError(ts.Connect());
	CleanupClosePushL(ts);
	// delete "StoreInit.tmp"
	ts.DeleteFile(KStoreInitFileName);
	CleanupStack::PopAndDestroy(&ts);

	// central repository cleanup
	CEmailAccounts* accounts = CEmailAccounts::NewLC();

	// delete pop3 accounts
	RArray<TPopAccount> pop3Accounts;
	CleanupClosePushL(pop3Accounts);
	accounts->GetPopAccountsL(pop3Accounts);
	for (TInt i=0; i<pop3Accounts.Count(); ++i)
		{
		accounts->DeletePopAccountL(pop3Accounts[i]);
		}

	// delete imap4 accounts
	RArray<TImapAccount> imap4Accounts;
	CleanupClosePushL(imap4Accounts);
	accounts->GetImapAccountsL(imap4Accounts);
	for (TInt j=0; j<imap4Accounts.Count(); ++j)
		{
		accounts->DeleteImapAccountL(imap4Accounts[j]);
		}
		
	// delete smtp accounts
	RArray<TSmtpAccount> smtpAccounts;
	CleanupClosePushL(smtpAccounts);
	accounts->GetSmtpAccountsL(smtpAccounts);
	for (TInt k=0; k<smtpAccounts.Count(); ++k)
		{
		accounts->DeleteSmtpAccountL(smtpAccounts[k]);
		}

	CleanupStack::PopAndDestroy(4, accounts);		
	
	for (TInt i=0; i<numberOfEntries; i++)
		{
		TMsvId id = reader.ReadInt32();
		CMsvEntry* clientEntry = iSession->GetEntryL(id);
		CleanupStack::PushL(clientEntry);
		

		if (clientEntry->HasStoreL())
			{
			CMsvStore* store = clientEntry->EditStoreL();
			CleanupStack::PushL(store);

			MMsvAttachmentManager& attManager = store->AttachmentManagerL();																			 
			TInt count = attManager.AttachmentCount();
			CMtfAsyncWaiter* waiter = CMtfAsyncWaiter::NewL();
			CleanupStack::PushL(waiter);
			for( TInt i=0; i<count; ++i )
				{
				// remember as we remove an attachment the next attachment becomes index 0
				attManager.RemoveAttachmentL( 0, waiter->iStatus );
				waiter->StartAndWait();
				User::LeaveIfError(waiter->Result());
				}						
			store->CommitL();
			CleanupStack::PopAndDestroy(waiter);
			
			store->DeleteL();
			CleanupStack::PopAndDestroy(store);
			}

		TMsvEntry indexEntry = clientEntry->Entry();

		if (!((indexEntry.Parent()   == reader.ReadInt32()) &&
			 (indexEntry.iServiceId == reader.ReadInt32()) &&
			 (indexEntry.iType.iUid == reader.ReadInt32()) &&
			 (indexEntry.iMtm.iUid  == reader.ReadInt32()) &&
//		 (indexEntry.iData      == reader.ReadInt32()) &&
			 ((reader.ReadInt32()) | 1) &&
			 (!indexEntry.iDescription.Compare(reader.ReadTPtrC())) &&
			 (!indexEntry.iDetails.Compare(reader.ReadTPtrC()))))
				User::Leave(KErrGeneral);

		CleanupStack::PopAndDestroy(clientEntry);
		}

	CleanupStack::PopAndDestroy(buf); 
	CleanupStack::PopAndDestroy();	// resource
	
	fs.Close();
	TestCase().INFO_PRINTF1(_L("CMtfTestActionResetMessageStore completed"));
	TestCase().ActionCompletedL(*this);
	}

