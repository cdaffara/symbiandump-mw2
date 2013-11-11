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

#ifndef __CSENDASMESSAGETYPES_H__
#define __CSENDASMESSAGETYPES_H__

#include <e32base.h>
#include <badesca.h>

// Forward declarations
class RReadStream;
class RWriteStream;

/**
Provides human-readable names and UIDs for the filtered list of message types.

@publishedAll
@released
*/
class CSendAsMessageTypes : public CBase
	{
public:
	IMPORT_C static CSendAsMessageTypes* NewL();
	IMPORT_C virtual ~CSendAsMessageTypes();
	
	IMPORT_C const MDesCArray& AvailableMessageTypes() const;
	IMPORT_C TUid MessageTypeUid(TInt aIndex) const;
	IMPORT_C TPtrC NameFromUidL(const TUid aUid) const;
	IMPORT_C TUid UidFromNameL(const TDesC& aName) const;
	IMPORT_C TInt Count() const;
	IMPORT_C void Reset();

	IMPORT_C void InternalizeL(RReadStream& aReadStream);
	IMPORT_C void ExternalizeL(RWriteStream& aWriteStream) const;
	
	IMPORT_C void AppendMessageTypeL(const TDesC& aMessageTypeName,	TUid aMessageTypeUid);
	IMPORT_C void RemoveMessageType(TInt aIndex);
	IMPORT_C TInt Size() const;
	
private:
	CSendAsMessageTypes();
	void ConstructL();
	
private:
	CDesCArrayFlat* iMessageTypeNames;
	RArray<TUid>	iMessageTypeUids;
	};

#endif	// __CSENDASMESSAGETYPES_H__
