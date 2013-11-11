// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// FindMtm.CPP
// 
//

#include <mtmuids.h>
#include <mtmdef.h>
#include <msvftext.h>
#include <txtrich.h>

#include "brokenfindmtm.h"

const TUid KUidTestAddresseeList = {0x100051A8};

enum {
	ENothingToFind
	};

LOCAL_C void Panic(TInt aReason)
	{
	User::Panic(_L("BROKENFINDMTM"), aReason);
	}

EXPORT_C CBrokenFindMtm* CBrokenFindMtm::NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession)
//
//
//
	{
	CBrokenFindMtm* myclntmtm=new(ELeave) CBrokenFindMtm(aRegisteredMtmDll, aSession);
	myclntmtm->ConstructL();
	return myclntmtm;
	}

CBrokenFindMtm::~CBrokenFindMtm()
//
//
//
	{
	delete iFindText;
	}

void CBrokenFindMtm::ConstructL()
//
//
//
	{
	iFindText = CMsvFindText::NewL(CActive::EPriorityStandard);
	}

void CBrokenFindMtm::HandleEntryEvent(TMsvEntryEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
//
//
//
	{
	}

void CBrokenFindMtm::StoreRecipientsL(CMsvStore& aStore)
//
//
//
	{
	RMsvWriteStream write;
	write.AssignL(aStore, KUidTestAddresseeList);

	TInt count = iAddresseeList->Count();
	write << TCardinality(count);
	while(count--)
		{
		TCardinality length = TCardinality((*iAddresseeList)[count].Length());
		write << length;
		write.WriteL((*iAddresseeList)[count], (*iAddresseeList)[count].Length());
		}

	write.CommitL();
	write.Close();
	}

void CBrokenFindMtm::SaveMessageL()
//
//
//
	{
	CMsvStore* store = iMsvEntry->EditStoreL();
	CleanupStack::PushL(store);
	StoreBodyL(*store);
	StoreRecipientsL(*store);
	store->CommitL();
	CleanupStack::PopAndDestroy(); // store
	}

void CBrokenFindMtm::RestoreRecipientsL(CMsvStore& aStore)
//
//
//
	{
	iAddresseeList->Reset();

	RMsvReadStream read;
	read.OpenL(aStore, KUidTestAddresseeList);

	TCardinality card;
	read >> card;

	TBuf<256> buf;
	TInt count = TInt(card);
	while(count--)
		{
		TCardinality length;
		read >> length;
		read.ReadL(buf, TInt(length));
		iAddresseeList->AppendL(buf);
		}

	read.Close();
	}

void CBrokenFindMtm::LoadMessageL()
//
//
//
	{
	/*
	CMsvStore* store = iMsvEntry->ReadStoreL();
	CleanupStack::PushL(store);
	RestoreBodyL(*store);
	RestoreRecipientsL(*store);
	CleanupStack::PopAndDestroy(); // store
	*/

	User::Leave(KErrNotSupported);
	}

CMsvOperation* CBrokenFindMtm::ReplyL  (TMsvId /*aDestination*/,   TMsvPartList /*aPartlist*/, TRequestStatus& /*aCompletionStatus*/)
//
//
//
	{
	return NULL;
	}

CMsvOperation* CBrokenFindMtm::ForwardL(TMsvId /*aDestination*/, TMsvPartList /*aPartList*/, TRequestStatus& /*aCompletionStatus*/)
//
//
//
	{
	return NULL;
	}

TMsvPartList CBrokenFindMtm::Find(const TDesC& aTextToFind, TMsvPartList aPartList)
//
//
//
	{
	TMsvPartList result = 0;
	TRAPD(error, result = DoFindSyncL(aTextToFind, aPartList));
	return result;
	}

TMsvPartList CBrokenFindMtm::DoFindSyncL(const TDesC& aTextToFind, TMsvPartList aPartList)
//
//
//
	{
	__ASSERT_ALWAYS(aTextToFind.Length() > 0 && aPartList > 0, Panic(ENothingToFind));

	// Iterate through all the parts
	TMsvPartList result = 0;
	for(TMsvPartList current = KMsvMessagePartBody; current <= KMsvMessagePartAttachments; current<<=1)
		{
		// Were we asked to search in current part?
		if (aPartList & current)
			{
			switch (current)
				{
				case KMsvMessagePartBody:
					if (iFindText->FindRichTextL(aTextToFind, Body(), aPartList))
						result |= current;
					break;

				// Search for recipients
				case KMsvMessagePartRecipient:
					if (FindRecipientL(aTextToFind, aPartList))
						result |= current;
					break;

				case KMsvMessagePartOriginator:
					if (iFindText->FindTextL(aTextToFind, iMsvEntry->Entry().iDetails, aPartList))
						result |= current;
					break;

				// Search for description
				case KMsvMessagePartDescription:
					if (iFindText->FindTextL(aTextToFind, iMsvEntry->Entry().iDescription, aPartList))
						result |= current;
					break;
				}
			}
		}

	return result;
	}

TBool CBrokenFindMtm::FindRecipientL(const TDesC& aTextToFind, TMsvPartList aFlags)
//
//
//
	{
	TInt count = iAddresseeList->Count();
	while(count--)
		{
		if (iFindText->FindTextL(aTextToFind, (*iAddresseeList)[count], aFlags))
			return ETrue;
		}

	return EFalse;
	}

const CMsvRecipientList& CBrokenFindMtm::AddresseeList()
	{
	return *iAddresseeList;
	}

void CBrokenFindMtm::AddAddresseeL(const TDesC& aRealAddress)
//
//
//
	{
	iAddresseeList->AppendL(aRealAddress);
	}

void CBrokenFindMtm::AddAddresseeL(const TDesC& aRealAddress, const TDesC& aAlias)
//
//
//
	{
	TBuf<256> buf;
	buf.Copy(aAlias);
	buf.Append(_L(" <"));
	buf.Append(aRealAddress);
	buf.Append(_L(">"));
	iAddresseeList->AppendL(buf);
	}

void CBrokenFindMtm::RemoveAddressee(TInt aIndex)
//
//
//
	{
	iAddresseeList->Delete(aIndex);
	}

void CBrokenFindMtm::ContextEntrySwitched()
//
//
//
	{
	}

void CBrokenFindMtm::InvokeSyncFunctionL(TInt /*aFunctionId*/,const CMsvEntrySelection& /*aSelection*/, TDes8& /*aParameter*/)
//
//
//
	{
	User::Leave(KErrNotSupported);
	}

CMsvOperation* CBrokenFindMtm::InvokeAsyncFunctionL(TInt /*aFunctionId*/,const CMsvEntrySelection& /*aSelection*/, TDes8& /*aParameter*/, TRequestStatus& aCompletionStatus)
//
//
//
	{
	TRequestStatus* status=&aCompletionStatus;
	User::RequestComplete(status,KErrNotSupported);
	return NULL;;
	}

TInt CBrokenFindMtm::QueryCapability(TUid /*aCapability*/, TInt& /*aResponse*/)
//
//
//
	{
	return KErrNotSupported;
	}

void CBrokenFindMtm::SetSubjectL(const TDesC& /*aSubject*/)
//
//
//
	{
	}

TUint CBrokenFindMtm::ValidateMessage(TMsvPartList /*aPartList*/)
//
//
//
	{
	return 0;
	}

CBrokenFindMtm::CBrokenFindMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession)
: CBaseMtm(aRegisteredMtmDll, aSession)
	{
	}

