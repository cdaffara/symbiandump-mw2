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
//

#include <mtmuids.h>
#include <mtmdef.h>
#include <msvftext.h>
#include <txtrich.h>

#include "FINDMTM.H"

const TUid KUidTestAddresseeList = {0x100051A8};

enum {
	ENothingToFind
	};

LOCAL_C void Panic(TInt aReason)
	{
	User::Panic(_L("FINDMTM"), aReason);
	}

EXPORT_C CFindMtm* CFindMtm::NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession)
//
//
//
	{
	CFindMtm* myclntmtm=new(ELeave) CFindMtm(aRegisteredMtmDll, aSession);
	myclntmtm->ConstructL();
	return myclntmtm;
	}

CFindMtm::~CFindMtm()
//
//
//
	{
	delete iFindText;
	}

void CFindMtm::ConstructL()
//
//
//
	{
	iFindText = CMsvFindText::NewL(CActive::EPriorityStandard);
	}

void CFindMtm::HandleEntryEvent(TMsvEntryEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
//
//
//
	{
	}

void CFindMtm::StoreRecipientsL(CMsvStore& aStore)
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

void CFindMtm::SaveMessageL()
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

void CFindMtm::RestoreRecipientsL(CMsvStore& aStore)
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

void CFindMtm::LoadMessageL()
//
//
//
	{
	CMsvStore* store = iMsvEntry->ReadStoreL();
	CleanupStack::PushL(store);
	RestoreBodyL(*store);
	RestoreRecipientsL(*store);
	CleanupStack::PopAndDestroy(); // store
	}

CMsvOperation* CFindMtm::ReplyL  (TMsvId /*aDestination*/,   TMsvPartList /*aPartlist*/, TRequestStatus& /*aCompletionStatus*/)
//
//
//
	{
	return NULL;
	}

CMsvOperation* CFindMtm::ForwardL(TMsvId /*aDestination*/, TMsvPartList /*aPartList*/, TRequestStatus& /*aCompletionStatus*/)
//
//
//
	{
	return NULL;
	}

TMsvPartList CFindMtm::Find(const TDesC& aTextToFind, TMsvPartList aPartList)
//
//
//
	{
	TMsvPartList result = 0;
	TRAPD(error, result = DoFindSyncL(aTextToFind, aPartList));
	return result;
	}

TMsvPartList CFindMtm::DoFindSyncL(const TDesC& aTextToFind, TMsvPartList aPartList)
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

TBool CFindMtm::FindRecipientL(const TDesC& aTextToFind, TMsvPartList aFlags)
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

const CMsvRecipientList& CFindMtm::AddresseeList()
	{
	return *iAddresseeList;
	}

void CFindMtm::AddAddresseeL(const TDesC& aRealAddress)
//
//
//
	{
	iAddresseeList->AppendL(aRealAddress);
	}

void CFindMtm::AddAddresseeL(const TDesC& aRealAddress, const TDesC& aAlias)
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

void CFindMtm::RemoveAddressee(TInt aIndex)
//
//
//
	{
	iAddresseeList->Delete(aIndex);
	}

void CFindMtm::ContextEntrySwitched()
//
//
//
	{
	}

void CFindMtm::InvokeSyncFunctionL(TInt /*aFunctionId*/,const CMsvEntrySelection& /*aSelection*/, TDes8& /*aParameter*/)
//
//
//
	{
	User::Leave(KErrNotSupported);
	}

CMsvOperation* CFindMtm::InvokeAsyncFunctionL(TInt /*aFunctionId*/,const CMsvEntrySelection& /*aSelection*/, TDes8& /*aParameter*/, TRequestStatus& aCompletionStatus)
//
//
//
	{
	TRequestStatus* status=&aCompletionStatus;
	User::RequestComplete(status,KErrNotSupported);
	return NULL;;
	}

TInt CFindMtm::QueryCapability(TUid /*aCapability*/, TInt& /*aResponse*/)
//
//
//
	{
	return KErrNotSupported;
	}

void CFindMtm::SetSubjectL(const TDesC& /*aSubject*/)
//
//
//
	{
	}

TUint CFindMtm::ValidateMessage(TMsvPartList /*aPartList*/)
//
//
//
	{
	return 0;
	}

CFindMtm::CFindMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession)
: CBaseMtm(aRegisteredMtmDll, aSession)
	{
	}

