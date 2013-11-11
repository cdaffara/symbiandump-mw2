// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "csendastestmessage.h"

#include <msvstore.h>

#include "csendastestaddress.h"

const TUid KUidSendAsTestMessageStream	= {0x10204289};
const TInt KSendAsTestMessageVersion	= 1;

EXPORT_C CSendAsTestMessage* CSendAsTestMessage::NewL()
	{
	CSendAsTestMessage* self = new (ELeave) CSendAsTestMessage();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;	
	}

EXPORT_C CSendAsTestMessage::~CSendAsTestMessage()
	{
	iAddresses.ResetAndDestroy();
	delete iSubject;
	}

CSendAsTestMessage::CSendAsTestMessage()
: CBase()
	{
	}

void CSendAsTestMessage::ConstructL()
	{
	}

EXPORT_C const RPointerArray<CSendAsTestAddress>& CSendAsTestMessage::Addresses() const
	{
	return iAddresses;
	}

EXPORT_C RPointerArray<CSendAsTestAddress>& CSendAsTestMessage::Addresses()
	{
	return iAddresses;
	}

EXPORT_C void CSendAsTestMessage::SetSubjectL(const TDesC& aSubject)
	{
	HBufC* temp = aSubject.AllocL();
	delete iSubject;
	iSubject = temp;	
	}

EXPORT_C const TDesC& CSendAsTestMessage::Subject() const
	{
	return iSubject == NULL ? KNullDesC() : *iSubject;
	}

EXPORT_C void CSendAsTestMessage::SetBioTypeL(const TUid aBioType)
	{
	iBioType = aBioType;
	}

EXPORT_C TUid CSendAsTestMessage::BioType() const
	{
	return iBioType;
	}

EXPORT_C void CSendAsTestMessage::RestoreL(CMsvStore& aStore)
	{
	RMsvReadStream in;
	in.OpenLC(aStore, KUidSendAsTestMessageStream);
	InternalizeL(in);
	CleanupStack::PopAndDestroy(&in);
	}
	
EXPORT_C void CSendAsTestMessage::StoreL(CMsvStore& aStore) const
	{
	RMsvWriteStream out;
	out.AssignLC(aStore, KUidSendAsTestMessageStream);
	ExternalizeL(out);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	}

void CSendAsTestMessage::InternalizeL(RReadStream& aStream)
	{
	aStream.ReadInt16L(); //version. Not used yet

	iAddresses.ResetAndDestroy();
	TInt count = aStream.ReadInt32L();
	while(count--)
		{
		CSendAsTestAddress* addressee = CSendAsTestAddress::NewL();  
		CleanupStack::PushL(addressee);
		addressee->InternalizeL(aStream);
		iAddresses.AppendL(addressee);
		CleanupStack::Pop(addressee);
		}
	HBufC* subject = HBufC::NewL(aStream, KMaxTInt);
	delete iSubject;
	iSubject = subject;
	aStream >> iBioType;
	}
	
void CSendAsTestMessage::ExternalizeL(RWriteStream& aStream) const
	{
	aStream.WriteInt16L(KSendAsTestMessageVersion);

	TInt count = iAddresses.Count();
	aStream.WriteInt32L(count);

	for (TInt i=0; i<count; ++i)
		iAddresses[i]->ExternalizeL(aStream);
	
	aStream << Subject();
	aStream << iBioType;
	}
