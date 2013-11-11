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

#ifndef __CSENDASTESTADDRESS_H__
#define __CSENDASTESTADDRESS_H__

#include <e32base.h>

#include "csendastestmessage.h"

class CSendAsTestAddress : public CBase
	{
public:
	IMPORT_C static CSendAsTestAddress* NewL();
	IMPORT_C virtual ~CSendAsTestAddress();
	
	void InternalizeL(RReadStream& aStream);
	void ExternalizeL(RWriteStream& aStream) const;

	IMPORT_C const TDesC& Address() const;
	IMPORT_C void SetAddressL(const TDesC& aAddress);

	IMPORT_C const TDesC& Alias() const;
	IMPORT_C void SetAliasL(const TDesC& aAlias);	
	
	IMPORT_C CSendAsTestMessage::TSendAsTestAddressType AddressType() const;
	IMPORT_C void SetAddressType(CSendAsTestMessage::TSendAsTestAddressType aAddressType);
private:
	CSendAsTestAddress();
private:
	HBufC*		iAddress;
	HBufC*		iAlias;
	CSendAsTestMessage::TSendAsTestAddressType iAddressType;
	};

#endif	//  __CSENDASTESTADDRESS_H__
