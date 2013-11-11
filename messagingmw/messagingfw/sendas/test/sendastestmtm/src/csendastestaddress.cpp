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

#include "csendastestaddress.h"

#include <s32strm.h>

const TInt KSendAsTestAddressVersion	= 1;

EXPORT_C CSendAsTestAddress* CSendAsTestAddress::NewL()
	{
	return new (ELeave) CSendAsTestAddress();
	}
	
EXPORT_C CSendAsTestAddress::~CSendAsTestAddress()
	{
	delete iAddress;
	delete iAlias;
	}

CSendAsTestAddress::CSendAsTestAddress()
: CBase()
	{
	}

void CSendAsTestAddress::InternalizeL(RReadStream& aStream)
	{
	aStream.ReadInt16L(); //version. Not used yet

	HBufC* address = HBufC::NewL(aStream, KMaxTInt);
	delete iAddress;
	iAddress = address;

	HBufC* alias = HBufC::NewL(aStream, KMaxTInt);
	delete iAlias;
	iAlias = alias;
	
	iAddressType = static_cast<CSendAsTestMessage::TSendAsTestAddressType>(aStream.ReadInt32L());
	}

void CSendAsTestAddress::ExternalizeL(RWriteStream& aStream) const
	{
	aStream.WriteInt16L(KSendAsTestAddressVersion);

	aStream << Address();
	aStream << Alias();

	aStream.WriteInt32L(iAddressType);
	}

EXPORT_C const TDesC& CSendAsTestAddress::Address() const
	{
	return iAddress != NULL ? *iAddress : KNullDesC();	
	}
	
EXPORT_C void CSendAsTestAddress::SetAddressL(const TDesC& aAddress)
	{
	HBufC* address = aAddress.AllocL();
	delete iAddress;
	iAddress = address;
	}

EXPORT_C const TDesC& CSendAsTestAddress::Alias() const
	{
	return iAlias != NULL ? *iAlias : KNullDesC();		
	}
	
EXPORT_C void CSendAsTestAddress::SetAliasL(const TDesC& aAlias)
	{
	HBufC* alias = aAlias.AllocL();
	delete iAlias;
	iAlias = alias;	
	}

EXPORT_C CSendAsTestMessage::TSendAsTestAddressType CSendAsTestAddress::AddressType() const
	{
	return iAddressType;
	}
	
EXPORT_C void CSendAsTestAddress::SetAddressType(CSendAsTestMessage::TSendAsTestAddressType aAddressType)
	{
	iAddressType = aAddressType;
	}
