// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if !defined(__MCLENTRY_H__)
#define __MCLENTRY_H__

//**********************************
// CMsvEntry
//**********************************
//
//
//

/**
@internalComponent
@released
*/
enum TMsvClientEntryType {	EMsvClientNull,
							EMsvClientContext,
							EMsvClientChangedContext,
							EMsvClientChild};

class CMsvClientEntry : public CBase
/**
@internalComponent
@released
*/
	{
public:
	static CMsvClientEntry* NewLC(const TMsvEntry& aEntry, TMsvClientEntryType aType);
	~CMsvClientEntry();
	//
	inline const TMsvEntry& Entry() const;
	inline TMsvClientEntryType Type() const;
	inline void SetType(TMsvClientEntryType aType);
	inline void SetOwnerFlag(TBool aState);
	inline void SetId(TMsvId id);
	//
private:
	CMsvClientEntry(const TMsvEntry& aEntry, TMsvClientEntryType aType);
	void ConstructL();
	//
private:
	TMsvEntry	iEntry;
	TMsvClientEntryType iType;
	HBufC*		iDescription;
	HBufC*		iDetails;
	};

#include "MCLENTRY.INL"

#endif
