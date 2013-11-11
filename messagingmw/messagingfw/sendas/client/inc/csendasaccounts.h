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

#ifndef __CSENDASACCOUNTS_H__
#define __CSENDASACCOUNTS_H__

#include <e32base.h>
#include <badesca.h>
#include <msvstd.h>

// Forward declarations
class RReadStream;
class RWriteStream;

/**
Identifies accounts for use by SendAs.

@publishedAll
@released
*/
typedef TMsvId TSendAsAccount;

/**
Provides human-readable names and IDs for the list of accounts for a given
message type.

@publishedAll
@released
*/
class CSendAsAccounts : public CBase
	{
public:
	IMPORT_C static CSendAsAccounts* NewL();
	IMPORT_C virtual ~CSendAsAccounts();
	
	IMPORT_C TUid MessageType() const;
	IMPORT_C void SetMessageType(TUid aMessageType);

	IMPORT_C const MDesCArray& AccountNames() const;
	IMPORT_C TSendAsAccount Account(TInt aIndex) const;
	IMPORT_C TPtrC NameFromAccountL(const TSendAsAccount aAccount) const;
	IMPORT_C TSendAsAccount AccountFromNameL(const TDesC& aName) const;
	IMPORT_C TInt Count() const;
	IMPORT_C void Reset();

	IMPORT_C void InternalizeL(RReadStream& aReadStream);
	IMPORT_C void ExternalizeL(RWriteStream& aWriteStream) const;
	
	IMPORT_C void AppendAccountL(const TDesC& aAccountName, TSendAsAccount aAccount);
	IMPORT_C void RemoveAccount(TInt aIndex);
	IMPORT_C TInt Size() const;
	
private:
	CSendAsAccounts();
	void ConstructL();
	
private:
	TUid 					iMessageType;
	CDesCArrayFlat*			iAccountNames;
	RArray<TSendAsAccount>	iAccounts;
	};

#endif	// __CSENDASACCOUNTS_H__
