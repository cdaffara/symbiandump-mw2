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
//

#include <ipaddr.h>

EXPORT_C CIpAddress* CIpAddress::NewL(const TDesC& aAddr)
/** Allocates and constructs a new IP address object with a specified address.

A buffer is allocated on the heap and aAddr copied into it.

@param aAddr IP address string
@return New IP address object */
	{
	CIpAddress* self = NewLC(aAddr);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CIpAddress* CIpAddress::NewLC(const TDesC& aAddr)
/** Allocates and constructs a new IP address object with a specified address, 
leaving the object on the cleanup stack.

A buffer is allocated on the heap and aAddr copied into it.

@param aAddr IP address string
@return New IP address object */
	{
	CIpAddress* self = new (ELeave) CIpAddress();
	CleanupStack::PushL(self);
	self->SetAddrL(aAddr);
	return self;
	}

CIpAddress::CIpAddress()
	{
	}

EXPORT_C CIpAddress::~CIpAddress()
/** Destructor. */
	{
	delete iAddr;
	}

EXPORT_C void CIpAddress::SetAddrL(const TDesC& aAddr)
/** Sets the IP address.

A buffer is allocated on the heap and aAddr copied into it.

@param aAddr IP address string */
	{
	HBufC* tempAddr = NULL;
	TInetAddr t;
	const TInt err = t.Input(aAddr);

	if (err == KErrNone)
		{
		tempAddr = HBufC::NewL(KIpAddressSize);
		TPtr buf(tempAddr->Des());
		t.Output(buf);
		}
	else
		tempAddr = aAddr.AllocL();

	delete iAddr;
	iAddr = tempAddr;
	}

EXPORT_C const TDesC& CIpAddress::Addr() const
/** Gets the IP address.

@return IP address string */
	{
	return *iAddr;
	}
