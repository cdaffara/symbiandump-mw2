// CMSVBODYTEXT.H

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

/**
 @file
 @internalTechnology
*/

#ifndef __CMSVBODYTEXT_H__
#define __CMSVBODYTEXT_H__

#include<e32base.h>
#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <msvids.h>
#endif

/**
The number of characters that will be decoded at any one time.  Used to limit memory consumption.
*/
const TInt KMsvDecodeChunkLength = 512;

/**
The unique stream identifier of the 8 bit encoded body text data stored in the message store.
*/
const TUid KMsv8BitEncodedBodyData = {0x101FD0E2};

class CMsvStore;
class CRichText;

class CMsvBodyText : public CBase
/**
This class encapsulates 8 bit data and character set information for
storage within the Message Store.  It also populates a CRichText object with the
8 bit data decoded to the specified character set.  If a character set is not
specified, then the default character set is used.

@internalTechnology
@released
*/
	{
public:
	IMPORT_C static CMsvBodyText* NewL();
	IMPORT_C static CMsvBodyText* NewLC();
	IMPORT_C virtual ~CMsvBodyText();
	IMPORT_C void SetCharacterSet(const TUint aCharacterSetIdentifier);
	IMPORT_C TUint CharacterSet() const;
	IMPORT_C void SetDefaultCharacterSet(const TUint aCharacterSetIdentifier);
	IMPORT_C TUint DefaultCharacterSet() const;
	IMPORT_C void RestoreL(CMsvStore& aStore);
	IMPORT_C void StoreL(CMsvStore& aStore);
	IMPORT_C void StoreL(CMsvStore& aStore, const CBufBase& aData);
	IMPORT_C void GetBodyTextL(RFs& aFs, CMsvStore& aStore, CRichText& aBodyText);
	IMPORT_C void RemoveL(CMsvStore& aStore);
	IMPORT_C TBool IsPresentL(const CMsvStore& aStore) const;
	IMPORT_C TInt GetBodyLengthL(CMsvStore& aStore);
	IMPORT_C void GetBodyTextL(CMsvStore& aStore, TDes8& aBufer);
private:
	CMsvBodyText();

private:
	TUint           iCharSet;
	TUint           iDefaultCharSet;
	};

#endif
