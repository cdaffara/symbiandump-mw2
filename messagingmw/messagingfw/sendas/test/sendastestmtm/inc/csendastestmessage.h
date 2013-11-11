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

#ifndef __CSENDASTESTMESSAGE_H__
#define __CSENDASTESTMESSAGE_H__

#include <e32base.h>
#include <cmsvrecipientlist.h>

class CSendAsTestAddress;
class CMsvStore;
class RReadStream;
class RWriteStream;

class CSendAsTestMessage : public CBase
	{
public:
	enum TSendAsTestAddressType
		{
		ESendAsTestAddressTypeTo	= EMsvRecipientTo,
		ESendAsTestAddressTypeCc,
		ESendAsTestAddressTypeBcc
		};

public:
	IMPORT_C static CSendAsTestMessage* NewL();
	IMPORT_C virtual ~CSendAsTestMessage();

	IMPORT_C const RPointerArray<CSendAsTestAddress>& Addresses() const;
	IMPORT_C RPointerArray<CSendAsTestAddress>& Addresses();

	IMPORT_C void SetSubjectL(const TDesC& aSubject);
	IMPORT_C const TDesC& Subject() const;
	
	IMPORT_C void RestoreL(CMsvStore& aStore);
	IMPORT_C void StoreL(CMsvStore& aStore) const;

	IMPORT_C void SetBioTypeL(const TUid aBioType);
	IMPORT_C TUid BioType() const;
private:
	CSendAsTestMessage();
	void ConstructL();

	void InternalizeL(RReadStream& aStream);
	void ExternalizeL(RWriteStream& aStream) const;
private:
	RPointerArray<CSendAsTestAddress>	iAddresses;
	HBufC*		iSubject;
	TUid		iBioType;
	};

#endif	// __CSENDASTESTMESSAGE_H__
