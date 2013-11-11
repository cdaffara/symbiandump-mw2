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

#ifndef __CMSVRECIPIENTLIST_H__
#define __CMSVRECIPIENTLIST_H__

#include <e32base.h>
#include <badesca.h>



// must be 4 bytes so that RArray works.
typedef TUint32 TMsvRecipientType;

/**
@publishedAll
@released
*/
enum TMsvRecipientTypeValues
	{
	EMsvRecipientTo = 0,
	EMsvRecipientCc,
	EMsvRecipientBcc,
	};

/**
@publishedAll
@released
*/
class CMsvRecipientList : public CBase
	{
public:
	static CMsvRecipientList* NewL();
	static CMsvRecipientList* NewLC();
	virtual ~CMsvRecipientList();
	//
	IMPORT_C void Reset();
	IMPORT_C TInt Count() const;
	//
	IMPORT_C void AppendL(const TDesC& aPtr);
	IMPORT_C void AppendL(TMsvRecipientType aValue, const TDesC& aPtr);
	//
	IMPORT_C void Delete(TInt aPos);
	IMPORT_C void Delete(TInt aPos, TInt aCount);
	//
	IMPORT_C TMsvRecipientType Type(TInt aPos) const;
	IMPORT_C const TDesC& operator[](TInt aIndex) const;
	//
	IMPORT_C const MDesCArray& RecipientList() const;
private:
	CMsvRecipientList();
	void ConstructL();
private:
	RArray<TMsvRecipientType> iRecipientType;
	CDesCArrayFlat*	iRecipients;
	};

#endif //__CMSVRECIPIENTLIST_H__
